/*
 * name:	ircbot
 * date:	08-12-2003
 * version:	0.1
 * license:	public domain
 * website:	http://www.xs4all.nl/~mechiel/projects/ircbot/
 * author:	mechiel lukkien
 * e-mail:	mechiel@xs4all.nl or mechiel@ueber.net
 * compile:	cc -g -Wall -pthread -o ircbot ircbot.c
 * xlint:	lint -aabchruH -lposix ircbot.c
 * usage:	./ircbot nick host port listenhost listenport nickfile >> logfile
 * see also:	rfc2810, rfc2811, rfc2812 and rfc2813
 * tested on:	OpenBSD 3.3 (extensively), FreeBSD 4.7 and some Linux
 *
 *
 * Note that you also need a program/script `botcmd' in the same directory as
 * in which you executed ircbot.
 *
 * This ircbot is very simplistic:  For example, it will not reconnect when its
 * connection disappears, and keeps only a minimal of state (e.g. it does know
 * it own nick, but it doesn't know which channels it joined).  There is
 * virtually nothing to configure, but you can always modify the C code.
 *
 * When you start an instance of ircbot, it connects to host:port (as given on
 * the command line) and logs in as nick.  If nick is already in use, probably
 * nothing useful will happen.  If all goes well the bot will just stay
 * connected, replying to ping messages, but not joining channels or doing
 * anything on its own.  Ircbot also reads nickfile (also specified on the
 * command line) and keeps it in memory.  The file contains nicks that are
 * allowed to execute commands, one on each line.  All communications are
 * printed to stdout, you may want to log it.  Warnings, diagnostics and errors
 * are printed to stderr.  Ircbot also listens on the TCP port
 * listenhost:listenport, a connection to this port is called a control
 * connection, since by this means you control ircbot.  One can send the
 * following commands over the control connection (one command per line,
 * trailing \r\n or \n is stripped):
 *
 * - `#channel howdy folks!' sends `howdy folks!' to #channel
 * - `%user howdy user!' sends `howdy user!' to user
 * - `;raw-irc-command' sends raw-irc-command to ircd.  e.g. `;JOIN #channel'.
 * - `flush' will clear the queue for outgoing irc messages
 *    (except important messages like ping replies)
 * - `readnicks' causes nickfile to be reread
 *
 * You can also have ircbot execute commands by sending commands over irc.
 * Lines starting with a semicolon (;) are commands (most bots are triggered by
 * !, so yes, this is different).  Commands are accepted both in channels and
 * in private messages (of course, ircbot checks whether the nick that send the
 * command is allowed to execute commands).  Ircbot has no builtin commands.
 * For every command, ircbot forks, sets a number of environment variables (see
 * below), and executes ./botcmd (so botcmd must be present in the directory
 * ircbot was started) with the command as its only argument (but without the
 * leading semicolon).  In principle, a botcmd can do whatever it wants, but
 * the delivered botcmd script (a shell script for the rc shell
 * (http://www.star.le.ac.uk/~tjg/rc/) will execute the command and send the
 * output to the channel or user by means of a control connection.
 *
 * Environment variables set before executing ./botcmd (and thus available in
 * botcmd and whatever processes botcmd is creating):
 * - target	irc target to send the reply (e.g. #channel or user (actually,
 *              the default botcmd sets target to %$target if it doesn't start
 *              with a #. since the control connection needs a % prepended to
 *              send data to a user))
 * - nick	nick of the bot
 * - nickfile	file containing nicks allowed to execute commands
 * - fromuser	user sending the command
 * - bothost	host the bot listens on
 * - botport	port the bot listens on, on bothost
 * - botpid	pid of the bot
 * Also, when a command was given in a channel, fromchannel is set to that
 * channel.
 *
 * Hopefully, enough information has been provided to get you started with this
 * program.
 *
 *
 * Now for some comments on the implementation.  Ircbot is heavily
 * multithreaded, featuring the following threads:
 *
 * - ircreader, reading irc messages from the ircd
 * - ircwriter, writing irc messages to the ircd
 * - tcpacceptor, accepting incoming control connections
 * - tcpreader, one thread for each established control connection
 * - handlequeue, the main program that coordinates the data flow between the threads
 *
 * The main program/thread is the central part of ircbot.  It reads events from
 * an `inqueue', processes them, and possibly puts lines to be written to the
 * ircd in an `outqueue'.
 *
 * - Ircreader simply reads irc messages (read a line and parse it), and puts it
 *   into the event queue.
 * - Tcpacceptor does nothing with the queue, it only starts a tcpreader thread
 *   for each incoming control connection.
 * - Tcpreader reads and parses commands from the control channel and puts them
 *   into the event queue.
 * - Handlequeue reads an event, processes it and waits for the next event.  It
 *   will never block (except when waiting for incoming events of course).
 *   Processing sometimes includes putting an irc message in the outqueue to be
 *   read by ircwriter.
 * - Ircwriter reads lines from the outqueue and writes them to the ircd.  It
 *   makes sure to not flood the ircd because that wouldn't be nice.  This is
 *   done by simply keeping at least NOFLOOD_DELAY miliseconds between every
 *   two messages.
 *
 * Synchronisation is done by the queue functions using a mutex (to make sure a
 * thread has exclusive access to the queue for a short period) and a non-empty
 * condition (to make sure threads are woken up as soon as the queue is
 * non-empty).
 * http://www.llnl.gov/computing/tutorials/workshops/workshop/pthreads/MAIN.html
 * has some nice examples.
 *
 * Possible future improvements:
 * - Currently, the control command `flush' removes everything from the queue
 *   (except ping replies of course).  It would be nice to only flush output to
 *   the channel/user by which the flush was executed.
 * - Thread-safety of standard library functions could be an issue.  I have not
 *   tried really hard to make sure it won't cause problems.
 * - Make the exit code indicate if the error was fatal or non-fatal.  With
 *   non-fatal meaning we can try to reconnect.
 * - Find some mechanism to kill off old processes that somehow went wrong
 *   (e.g. when someone starts something like vim).
 */


#define _GNU_SOURCE	/* to make it compile without warning on glibc systems */
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <pwd.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define nil	NULL
#define strequiv(s1, s2)	(strcasecmp(s1, s2) == 0)
#define streql(s1, s2)		(strcmp(s1, s2) == 0)


#define IRCBOT_VERSION		"ircbot, version 0.1, http://www.xs4all.nl/~mechiel/projects/ircbot/"
enum
{
	IRCMSG_MAXLEN = 512,            	/* maximum length of an irc (protocol) message */
	BBUFFERSIZE   = IRCMSG_MAXLEN+1,	/* size of the buffer for incoming irc data */
	SOCK_MAX      = 32,			/* maximum number of sockets to listen on */
	NICK_MAXLEN   = 128,			/* `should be more than enough' */
	NOFLOOD_DELAY = 700			/* milliseconds between the sending of two irc messages */
};


/* types of the data send to the event processing main loop */
enum
{
	INTYPEircmsg,		/* irc message from ircd */
	INTYPEraw,		/* raw message to be send to the ircd */
	INTYPEbuiltin		/* builtin command */
};

/* type of the data send to the thread that sends irc messages to the ircd (without flooding) */
enum
{
	OUTTYPEimmediate,	/* accompanied data should be send immediately (e.g. for PING) */
	OUTTYPEnormal		/* normal data */
};


typedef unsigned long long uvlong;
typedef long long vlong;
typedef struct Source Source;
typedef struct Msg Msg;
typedef struct Elem Elem;
typedef struct Queue Queue;
typedef struct Nick Nick;
typedef struct Nicks Nicks;
typedef struct Buf Buf;


/* irc source of the message */
struct Source {
	char   *nick, *server;	/* when server is non-nil, nick will contain the same value as server */
	char   *user;		/* user an irc message-line originated from, may be nil */
	char   *host;		/* host the user is on, may be nil */
};

/* irc messages as received from the ircd */
struct Msg {
	Source *src;		/* source of the message, may be nil */
	char   *cmd;		/* irc command, always non-nil */
	char  **params;		/* parameters for the command, may be nil */
	int	nparams;	/* number of parameters */
};

/* element of the event processing queues */
struct Elem {
	void   *data;		/* data part of the element */
	int	type;		/* type of data (INTYPE* for incoming, OUTTYPE* for outgoing) */
	Elem   *next;		/* next element in the list, may be nil for the last element */
};

/* event processing queue */
struct Queue {
	Elem   *first;		/* head of the queue, may be nil */
	pthread_mutex_t	mutex;	/* to control access to the queue */
	pthread_cond_t	nonempty;	/* signalled when something is put in the queue */
};

/* nick with permission to let the bot execute commands */
struct Nick {
	char   *name;
	Nick   *next;
};

/* list of nicks with permission */
struct Nicks {
	Nick   *first;
};

/* buffer with incoming data from the ircd */
struct Buf {
	char    s[BBUFFERSIZE];	/* data buffer */
	int	slen;		/* length of data in the buffer `s' */
	int	fd;		/* file descriptor to read from when `s' is empty */
	int	eof;		/* whether or not we have seen eof on `fd' */
};


void	handlequeue(char *, const char *, const char *, const char *, Nicks *);	/* main event processing loop */
void   *Fircreader(void *);			/* thread: reads and parses lines from ircd, then puts them in incoming queue */
void   *Fircwriter(void *);			/* thread: writes lines in outgoing queue to ircd */
void   *Ftcpacceptor(void *);			/* thread: accepts incoming tcp control connections (and creates an Ftcpreader for it) */
void   *Ftcpreader(void *);			/* thread: reads data from tcp control connection, data is put in the outgoing queue */
int	tcpopen(const char *, const char *);	/* opens connection to ircd */
void    tcpbind(const char *, int []);		/* binds to localhost tcp ports for accepting incoming control connections */

Msg    *msg_parse(char *);			/* parses a line received from the irc connection, Msg is allocated */
Msg    *msg_new(void);				/* allocates and initializes a Msg */
void	msg_free(Msg *);			/* frees any memory allocated in Msg */
Source *source_parse(char *);			/* parse the source of the message (`prefix' in irc terms) */

Queue  *qnew(void);				/* allocate and initialize a queue */
void	qappend(Queue *, int, void *);		/* append message to the queue */
void	qprepend(Queue *, int, void *);		/* prepend message to the queue */
void   *qremove(Queue *, int *);		/* retrieve and remove first element from queue */
void	qappendf(Queue *, int, const char *, ...);	/* append printf-like string to the queue */
void	qprependf(Queue *, int, const char *, ...);	/* same as qappendf but prepend */
void	qflushout(void);			/* flushes all non-important lines in outgoing queue */

Nicks  *nnew(const char *);			/* initialize nicks from the specified file */
void    nfree(Nicks *);				/* free Nicks */
void    nadd(Nicks *, char *);			/* add a nick */
void    nremove(Nicks *, char *);		/* remove the nick */
int	nhas(Nicks *, const char *);		/* check if the nick is present (allowed) */
void	nprint(Nicks *);			/* print a list of nicks to stderr, for debugging */
void    nsave(Nicks *, const char *);		/* save the nicks to the specified file */

Buf    *bnew(int);				/* create new buffer for reading incoming data from ircd */
int	breadln(Buf *, char *, int);		/* read a line (an irc message) from the buffer, keeps the trailing newline */

void    *xmalloc(size_t s)		{ void *p; p = malloc(s); if (p == nil) err(2, "xmalloc"); return p; }
void    *xrealloc(void *p, size_t s)	{ p = realloc(p, s); if (p == nil) err(2, "xrealloc"); return p; }
char    *xstrdup(char *str)		{ char *p; p = malloc(strlen(str) + 1); if (p == nil) err(2, "xstrdup"); strcpy(p, str); return p; }


/*
 * these are the only global variables (accessed by more than one thread),
 * these are protected by a pthread mutex and condition.
 */
Queue *inq;
Queue *outq;


int
main(int argc, char *argv[])
{
	char   *listenhost, *listenport;
	pthread_t Tircreader, Tircwriter, Ttcpacceptor[SOCK_MAX];
	char   *host, *port;
	char   *nickfile;
	struct	passwd *pw;
	int	ircfd;
	int     bindfds[SOCK_MAX+1];
	int	i;
	char   *nick;
	char   *userinfo;
	Nicks  *nicks;


	/* to make it accept `--' and print warnings for options like it should */
	if (getopt(argc, argv, "") != -1) {
		fprintf(stderr, "usage: bot nick host port listenhost listenport nickfile\n");
		exit(1);
	}
	argc -= optind;
	argv += optind;

	if (argc != 6) {
		warnx("invalid number of arguments");
		fprintf(stderr, "usage: bot nick host port listenhost listenport nickfile\n");
		exit(1);
	}

	/*
	 * a copy of nick is needed because the nick may change, then the
	 * current nick is freed and a new one allocated
	 */
	nick = xstrdup(argv[0]);
	host = argv[1];
	port = argv[2];
	listenhost = argv[3];
	listenport = argv[4];
	nickfile = argv[5];

	/* find username and info to present to ircd */
	pw = getpwnam(getlogin());
	if (pw == nil)
		errx(1, "could not find passwd entry for %s", getlogin());

	/* read nicks that are allowed to execute commands */
	nicks = nnew(nickfile);

	/*
	 * allocate and initialize event queues.  one for incoming commands
	 * (from ircd or listenhost:listenport) and one for messages to be send
	 * to the ircd.
	 */
	inq = qnew();
	outq = qnew();

	/* commands to be executed need a clean environment */
	if (fcntl(STDIN_FILENO,  F_SETFD, 1) == -1 ||
	    fcntl(STDOUT_FILENO, F_SETFD, 1) == -1 ||
	    fcntl(STDERR_FILENO, F_SETFD, 1) == -1)
		err(1, "setting close-on-exec for stdin, stdout and stderr");

	/*
	 * connect to the ircd.  start listening for incoming commands.  on
	 * error, exit is called
	 */
	ircfd = tcpopen(host, port);
	tcpbind(listenport, bindfds);

	/* create the threads */
	pthread_create(&Tircreader, nil, Fircreader, &ircfd);
	pthread_create(&Tircwriter, nil, Fircwriter, &ircfd);
	for (i = 0; bindfds[i] != -1; ++i)
		pthread_create(&Ttcpacceptor[i], nil, Ftcpacceptor, &bindfds[i]);

	/* send the login commands to get the connection started */
	userinfo = pw->pw_gecos;
	if (streql(userinfo, ""))
		userinfo = "none";
	qappendf(outq, OUTTYPEimmediate, "NICK %s\r\n", nick);
	qappendf(outq, OUTTYPEimmediate, "USER %s 0 * :%s\r\n", pw->pw_name, userinfo);

	/* into message handling loop, this receives incoming matches and dispatches outgoing ones */
	handlequeue(nick, listenhost, listenport, nickfile, nicks);
	exit(0);	/* not reached */
}


void
handlequeue(char *nick, const char *listenhost, const char *listenport, const char *nickfile, Nicks *nicks)
{
	void   *data;
	int	type;
	Msg    *msg;
	char	botpidbuf[64];
	char   *target;
	char   *param;

	assert(snprintf(botpidbuf, sizeof botpidbuf, "%llu", (uvlong)getpid()) != -1);

	for (;;) {
		/*
		 * this call blocks (properly with mutexes and non-empty-queue
		 * condition) until something needs to be processed
		 */
		data = qremove(inq, &type);

		/* cleanup possible child processes, doesn't block */
		for (;;) {
			int	r;
			int	status;

			r = wait3(&status, WNOHANG, nil);
			if (r == 0 || (r == -1 && errno == ECHILD))
				break;		/* no children at all (0) or no exited children (-1) */
			if (r == -1) {
				warn("wait3");	/* error in wait3 */
				break;
			}
		}
		errno = 0;

		/* handle the data retrieved from the queue, based on the type of the data */
		switch (type) {
		case INTYPEircmsg:	/* incoming irc message from ircd */
			msg = (Msg *)data;

			/*
			 * ping message from ircd
			 *
			 * <<< PING :irc.snt.utwente.nl
			 * >>> PONG brein irc.snt.utwente.nl
			 */
			if (strequiv(msg->cmd, "PING") && msg->nparams >= 1)
				qprependf(outq, OUTTYPEimmediate, "PONG %s %s\r\n", nick, msg->params[0]);

			/*
			 * handle messages telling our nick changed
			 *
			 * >>> NICK newbrein
			 * <<< :brein!plan@tunnel4040.ipv6.xs4all.nl NICK :newbrein
			 */
			if (strequiv(msg->cmd, "NICK") && msg->nparams == 1 &&
			    msg->src != nil && msg->src->nick != nil && strequiv(msg->src->nick, nick)) {
				free(nick);
				nick = xstrdup(msg->params[0]);
			}

			/* private messages from nicks and with an argument may contain a command or a ctcp version */
			if (msg->src != nil && msg->src->nick != nil && strequiv(msg->cmd, "PRIVMSG") && msg->nparams >= 2) {

				param = msg->params[1];

				/*
				 * only accept commands from known people (both in a channel and in private)
				 * skip some common smileys
				 *
				 * <<< :Oksel!mechiel@tunnel4040.ipv6.xs4all.nl PRIVMSG #deadbeef :;help
				 * or
				 * <<< :Oksel!mechiel@tunnel4040.ipv6.xs4all.nl PRIVMSG brein :;wn whatsoever
				 */
				if (nhas(nicks, msg->src->nick) && param[0] == ';' && 
				    !(streql(param, ";)") || streql(param, ";-)") ||
				      streql(param, ";(") || streql(param, ";-("))) {

					target = msg->params[0];
					if (strequiv(msg->params[0], nick))	/* private message to bot, send response to nick, not channel */
						target = msg->src->nick;

					switch (fork()) {
					case 0:
						setenv("target", target, 1);
						setenv("nick", nick, 1);
						setenv("nickfile", nickfile, 1);
						setenv("fromuser", msg->src->nick, 1);
						if (target[0] == '#')
							setenv("fromchannel", target, 1);
						setenv("bothost", listenhost, 1);
						setenv("botport", listenport, 1);
						setenv("botpid", botpidbuf, 1);

						/* note: all fd's have close-on-exec set */
						execlp("./botcmd", "./botcmd", param + 1, (char *)nil);
						warn("execlp ./botcmd");
						break;
					case -1:
						warn("forking for execlp");
						break;
					}
				} else if (strequiv(param, "\001VERSION\001")) {
					/* handle a ctcp version request */
					qappendf(outq, OUTTYPEnormal, "NOTICE %s :\001VERSION %s\001\r\n", msg->src->nick, IRCBOT_VERSION);
				}
			}

			msg_free(msg); msg = nil;
			break;
		case INTYPEraw:		/* raw command to send to ircd */
			qappendf(outq, OUTTYPEnormal, "%s\r\n", (char *)data);
			free(data); data = nil;
			break;
		case INTYPEbuiltin:	/* built in commands */
			if (streql("flush", (char *)data)) {
				qflushout();
			} else if (streql("readnicks", (char *)data)) {
				nfree(nicks);
				nicks = nnew(nickfile);
			} else {
				warnx("unexpected builtin command read in handlequeue: %s", (char *)data);
			}
			free(data); data = nil;
			break;
		default:
			warnx("unknown message received in main, type=%d", type);
			break;
		}
	}
}


void *
Fircreader(void *ircfdp)
{
	char    buf[IRCMSG_MAXLEN+1];
	int	count;
	Buf    *bin;
	Msg    *msg;
	struct	timeval now;
	uvlong	msecs;

	bin = bnew(*(int *)ircfdp);
	for (;;) {
		/* read a message line from the ircd (including \r\n if present) */
		count = breadln(bin, buf, sizeof buf);
		if (count == 0)
			errx(1, "eof from ircd");
		if (count == -1)
			err(1, "reading from ircd");

		gettimeofday(&now, nil);
		msecs = (uvlong)now.tv_sec*1000 + (uvlong)now.tv_usec/1000;
		printf("<<< %llu %s", msecs, buf);
		fflush(stdout);
		msg = msg_parse(buf);
		if (msg == nil)
			continue;	/* message has been printed by msg_parse */

		/* insert the irc message into the event queue */
		if (strequiv(msg->cmd, "PING"))
			qprepend(inq, INTYPEircmsg, msg);
		else
			qappend(inq, INTYPEircmsg, msg);
	}
}


void *
Fircwriter(void *ircfdp)
{
	char   *line;
	int	type;
	uvlong	msecs;
	int	length, count;
	struct timeval	lastwrite = {0, 0};
	struct timeval  now;

	for (;;) {
		line = qremove(outq, &type);
		if (type == OUTTYPEimmediate) {
			length = strlen(line);
			count = write(*(int *)ircfdp, line, length);
			if (count != length)
				warnx("ircwriter: line not written entirely, length=%d count=%d", length, count);

			gettimeofday(&lastwrite, nil);
		} else {
			if (type != OUTTYPEnormal)
				warnx("ircwriter: unknown message, type=%d, writing as if type=OUTTYPEnormal", type);

			gettimeofday(&now, nil);
			msecs = (now.tv_sec-lastwrite.tv_sec)*1000 + (now.tv_usec-lastwrite.tv_usec)/1000;
			if (msecs < NOFLOOD_DELAY)
				usleep((NOFLOOD_DELAY-msecs) * 1000);

			length = strlen(line);
			count = write(*(int *)ircfdp, line, length);
			if (count != length)
				warnx("ircwriter: line not written entirely, length=%d count=%d", length, count);

			gettimeofday(&lastwrite, nil);
		}

		msecs = (uvlong)lastwrite.tv_sec*1000 + (uvlong)lastwrite.tv_usec/1000;
		printf(">>> %llu %s", msecs, line);
		fflush(stdout);
		free(line);
	}
}


void *
Ftcpacceptor(void *bindfdp)
{
	int	tcpfd;
	struct	sockaddr sa;
	socklen_t  salen;
	pthread_t *Ttcpreader;

	for (;;) {
		tcpfd = accept(*(int *)bindfdp, (struct sockaddr *)&sa, &salen);
		if (tcpfd < 0) {
			if (errno == EINTR)
				continue;
			err(1, "accept");
		}

		if (fcntl(tcpfd, F_SETFD, 1) == -1) {
			warn("setting close-on-exec flag for incoming connection");
			close(tcpfd);
			continue;
		}

		Ttcpreader = xmalloc(sizeof Ttcpreader[0]);
		pthread_create(Ttcpreader, nil, Ftcpreader, &tcpfd);
	}
}


void *
Ftcpreader(void *tcpfdp)
{
	char	buf[BBUFFERSIZE];
	int	count;
	Buf    *bin;
	char   *cp;
	char   *errstr;

	/*
	 * bnew and breadln were actually meant for reading irc messages with a
	 * maximum length of 512 characters.  the tcp connection has this limit
	 * too now, no problem though.
	 */
	bin = bnew(*(int *)tcpfdp);
	for (;;) {
		/* read a message line from the client (including \n if present) */
		count = breadln(bin, buf, sizeof buf);
		if (count == 0) {
			close(*(int *)tcpfdp);
			pthread_exit(nil);
		}
		if (count == -1) {
			warn("tcpreader: error reading from client");
			close(*(int *)tcpfdp);
			pthread_exit(nil);
		}
		if (buf[strlen(buf) - 1] == '\n') {
			buf[strlen(buf) - 1] = '\0';
			if (buf[strlen(buf) - 1] == '\r')	/* telnet(1) appends an \r, remove it */
				buf[strlen(buf) - 1] = '\0';
		}

		if (streql(buf, "flush") || streql(buf, "readnicks")) {
			qprepend(inq, INTYPEbuiltin, xstrdup(buf));
		} else if (buf[0] == ';') {
			qappend(inq, INTYPEraw, xstrdup(buf+1));
		} else if (buf[0] == '%') {
			cp = strchr(buf, ' ');
			if (cp == nil || cp == buf+1) {
				errstr = "no message to send to nick or empty nick\n";
				break;
			} else {
				*cp = '\0';
				qappendf(inq, INTYPEraw, "PRIVMSG %s :%s", buf+1, cp+1);
			}
		} else if (buf[0] == '#') {
			cp = strchr(buf, ' ');
			if (cp == nil || cp == buf+1) {
				errstr = "no message to send to channel or empty channel\n";
				break;
			} else {
				*cp = '\0';
				qappendf(inq, INTYPEraw, "PRIVMSG %s :%s", buf, cp+1);
			}
		} else {
			errstr = "no nick or channel specified\n";
			break;
		}
	}

	if (write(*(int *)tcpfdp, errstr, strlen(errstr)) != strlen(errstr))
		warnx("tcpreader: error string not written entirely to connection");
	close(*(int *)tcpfdp);
	pthread_exit(nil);
}


int
tcpopen(const char *host, const char *port)
{
	int	gaierrno;
	struct	addrinfo hints = { 0, PF_UNSPEC, SOCK_STREAM, 0, 0, nil, nil, nil };
	struct	addrinfo *res0, *res;
	char   *cause;
	int	save_errno;
	int	fd;

	gaierrno = getaddrinfo(host, port, &hints, &res0);
	if (gaierrno)
		errx(1, "getaddrinfo: %s", gai_strerror(gaierrno));

	fd = -1;
	for (res = res0; res != nil; res = res->ai_next) {
		fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); 
		if (fd < 0) {
			fd = -1;
			cause = "socket";
			continue;
		}

		if (connect(fd, res->ai_addr, res->ai_addrlen) < 0) {
			save_errno = errno;
			(void)close(fd);
			errno = save_errno;

			fd = -1;
			cause = "connect";
			continue;
		}

		break;
	}
	freeaddrinfo(res0);

	if (fd == -1)
		err(1, "connecting to %s:%s: %s", host, port, cause);

	if (fcntl(fd, F_SETFD, 1) == -1)
		err(1, "setting close-on-exec flag for irc connection");

	return fd;
}


void
tcpbind(const char *port, int bindfds[SOCK_MAX+1])
{
	struct	addrinfo hints = { AI_PASSIVE, PF_UNSPEC, SOCK_STREAM, 0, 0, nil, nil, nil };
	struct	addrinfo *res, *res0;
	int	gaierrno, save_errno;
	int	fd, nsock;
	char   *cause;

	gaierrno = getaddrinfo(nil, port, &hints, &res0);
	if (gaierrno)
		errx(1, "getaddrinfo: %s", gai_strerror(gaierrno));

	cause = nil;
	nsock = 0;
	for (res = res0; res != nil && nsock < SOCK_MAX; res = res->ai_next) {
		fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (fd < 0) {
			cause = "socket";
			continue;
		}

		if (bind(fd, res->ai_addr, res->ai_addrlen) < 0 || fcntl(fd, F_SETFD, 1) == -1) {
			cause = "bind or fcntl";
			save_errno = errno;
			close(fd);
			errno = save_errno;

			continue;
		}
		(void)listen(fd, 5);
		bindfds[nsock++] = fd;
		bindfds[nsock] = -1;
	}
	if (nsock == 0)
		err(1, cause);

	freeaddrinfo(res0);
}


/*
 * see rfc2812 for a detailed description of irc messages
 *
 * simply put, an irc message looks like this:
 *
 * message = (:$prefix)? $command ($param)* crlf
 * prefix = servername | nickname ((!user)?@host)?
 * command = string | three-digit-number
 * param = string
 *
 * a parameter starting with a colon is special.  the parameter ends at
 * the end of the line, i.e. does not end with the next space.
 */
Msg *
msg_parse(char *str)
{
	int	stop;
	char   *cp;
	Msg    *msg;

	assert(str != nil);

	msg = msg_new();

	if (str[0] == ':') {
		++str;
		cp = strchr(str, ' ');
		if (cp == nil) {
			warnx("parsing: message contains prefix but no space (and thus no command)");
			goto error;
		}
		*cp = '\0';

		msg->src = source_parse(str);
		if (msg->src == nil)
			goto error;	/* diagnostics printed by source_parse */

		str = cp+1;
	}

	stop = 0;

	cp = strchr(str, ' ');
	if (cp == nil) {
		cp = strchr(str, '\r');
		if (cp == nil) {
			warnx("parsing: message doesn't contain carriage return (for end) or space (for parameter delimiter) after command");
			goto error;
		}
		stop = 1;
	}

	*cp = '\0';
	msg->cmd = xstrdup(str);
	str = cp+1;

	while (!stop) {
		if (str[0] == ':') {
			++str;
			stop = 1;
		}

                /* when we saw a colon and thus `stop' is set, don't
                 * look for a space but for the closing carriage return */
		cp = strchr(str, stop ? '\r' : ' ');
		if (cp == nil) {
			/* the last argument is not required to have a colon */
			cp = strchr(str, '\r');
			if (cp == nil) {
				warnx("parsing: no carriage return after arguments");
				goto error;
			}
			stop = 1;
		}
		*cp = '\0';
		msg->params = xrealloc(msg->params, sizeof msg->params[0] * (msg->nparams+1));
		msg->params[msg->nparams++] = xstrdup(str);

		str = cp+1;
	}

	/* debug code... */
	if (0) {
		int	i;

		if (msg->src != nil) {
			if (msg->src->host != nil) {
				fprintf(stderr, "src->nick=%s\n", msg->src->nick);
				if (msg->src->user != nil)
					fprintf(stderr, "src->user=%s\n", msg->src->user);
				else
					fprintf(stderr, "src->user=(nil)\n");
				fprintf(stderr, "src->host=%s\n", msg->src->host);
			} else {
				fprintf(stderr, "nick=server=%s\n", msg->src->nick);
			}
		} else {
			fprintf(stderr, "src=(nil)\n");
		}

		fprintf(stderr, "cmd=%s\n", msg->cmd);
		fprintf(stderr, "nparams=%d\n", msg->nparams);
		for (i = 0; i < msg->nparams; ++i)
			fprintf(stderr, "params[%d]=%s\n", i, msg->params[i]);
	}

	return (Msg *)msg;

error:
	msg_free((Msg *)msg); msg = nil;
	return nil;
}


Source *
source_parse(char *str)
{
	Source *source;
	char   *cp;

	assert(str != nil);

	source = xmalloc(sizeof source[0]);
	source->nick = nil;
	source->server = nil;
	source->user = nil;
	source->host = nil;

	cp = strchr(str, '@');
	if (cp == nil) {
		/* the source is either a servername or a nick without user/host part */
		source->nick = source->server = xstrdup(str);
		return source;
	}

	source->host = xstrdup(cp+1);
	*cp = '\0';	/* make location of @ end of string, thus str only contains nick with optionally !user */

	cp = strchr(str, '!');
	if (cp == nil) {
		source->nick = xstrdup(str);
		return source;
	}

	source->user = xstrdup(cp+1);
	*cp = '\0';	/* make location of ! end of string, thus str only contains nick */
	source->nick = xstrdup(str);

	return source;
}


Msg *
msg_new(void)
{
	Msg    *msg;
	
	msg = xmalloc(sizeof msg[0]);
	msg->src = nil;
	msg->cmd = nil;
	msg->params = nil;
	msg->nparams= 0;

	return msg;
}


void
msg_free(Msg *msg)
{
	int	i;

	if (msg == nil)
		return;

	for (i = 0; i < msg->nparams; ++i)
		free(msg->params[i]);
	free(msg->params);
	free(msg->cmd);
	if (msg->src != nil) {
		free(msg->src->nick);
		msg->src->server = nil;		/* server is always same as nick or nil */
		free(msg->src->user);
		free(msg->src->host);
		free(msg->src);
	}
	free(msg);
}


Queue *
qnew(void)
{
	Queue *new;

	new = xmalloc(sizeof new[0]);
	new->first = nil;
	pthread_mutex_init(&new->mutex, nil);
	pthread_cond_init(&new->nonempty, nil);

	return new;
}


void
qappend(Queue *q, int type, void *data)
{
	Elem   *new;
	Elem   *oldlast;

	assert(q != nil);
	assert(data != nil);

	new = xmalloc(sizeof new[0]);
	new->data = data;
	new->type = type;
	new->next = nil;

	pthread_mutex_lock(&q->mutex);
	if (q->first == nil) {
		q->first = new;
	} else {
		oldlast = q->first;
		while (oldlast->next != nil)
			oldlast = oldlast->next;
		oldlast->next = new;
	}
	pthread_cond_signal(&q->nonempty);
	pthread_mutex_unlock(&q->mutex);
}


void
qprepend(Queue *q, int type, void *data)
{
	Elem   *new;

	assert(q != nil);
	assert(data != nil);

	new = xmalloc(sizeof new[0]);
	new->data = data;
	new->type = type;
	new->next = nil;

	pthread_mutex_lock(&q->mutex);
	new->next = q->first;
	q->first = new;
	pthread_cond_signal(&q->nonempty);
	pthread_mutex_unlock(&q->mutex);
}


void *
qremove(Queue *q, int *typep)
{
	void   *r;
	Elem   *newfirst;

	assert(q != nil);

	pthread_mutex_lock(&q->mutex);
	if (q->first == nil)
		pthread_cond_wait(&q->nonempty, &q->mutex);

	assert(q->first != nil);

	r = q->first->data;
	if (typep != nil)
		(*typep) = q->first->type;
	newfirst = q->first->next;
	free(q->first);
	q->first = newfirst;
	pthread_mutex_unlock(&q->mutex);

	return r;
}


void
qappendf(Queue *q, int type, const char *fmt, ...)
{
	char   *tmp;
	va_list ap;

	va_start(ap, fmt);
	vasprintf(&tmp, fmt, ap);
	va_end(ap);

	qappend(q, type, tmp);
}


void
qprependf(Queue *q, int type, const char *fmt, ...)
{
	char   *tmp;
	va_list ap;

	va_start(ap, fmt);
	vasprintf(&tmp, fmt, ap);
	va_end(ap);

	qprepend(q, type, tmp);
}


void
qflushout(void)
{
	Elem   *newfirst;
	Elem   *newcur;
	Elem   *cur, *tmp;

	pthread_mutex_lock(&outq->mutex);

	newfirst = newcur = nil;
	cur = outq->first;
	while (cur != nil) {
		if (cur->type == OUTTYPEimmediate) {
			if (newfirst == nil) {
				newfirst = newcur = cur;
				newfirst->next = nil;
			} else {
				newcur->next = cur;
				newcur = newcur->next;
				newcur->next = nil;
			}
			cur = cur->next;
		} else {
			free(cur->data);
			tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}
	outq->first = newfirst;

	pthread_mutex_unlock(&outq->mutex);
}


Nicks *
nnew(const char *file)
{
	Nicks  *n;
	FILE   *fp;
	char    buf[NICK_MAXLEN + 1];

	n = xmalloc(sizeof n[0]);
	n->first = nil;

	fp = fopen(file, "r");
	if (fp == nil)
		err(1, "opening %s", file);

	/* read nicks from file and add them to the Nicks structure */
	for (;;) {
		if (fgets(buf, sizeof buf, fp) == nil) {
			if (ferror(fp))
				err(1, "reading from %s", file);
			break;
		}
		if (strchr(buf, '\n') != nil)
			buf[strlen(buf) - 1] = '\0';
		nadd(n, xstrdup(buf));
	}

	fclose(fp);

	return n;
}


void
nfree(Nicks *n)
{
	Nick   *cur, *tmp;

	for (cur = n->first;  cur != nil;  tmp = cur->next, free(cur), cur = tmp)
		free(cur->name);
	free(n);
}


void
nadd(Nicks *n, char *name)
{
	Nick   *new;
	Nick  **ptr;
	int	wslen;

	/* remove leading and trailing whitespace from nick */
	wslen = strspn(name, " \t\n");
	memmove(name, name+wslen, strlen(name+wslen) + 1);
	if (strpbrk(name, " \t\n") != nil)
		*(strpbrk(name, " \t\n")) = '\0';

	if (nhas(n, name) || strlen(name) == 0)	/* avoid duplicates, refuse empty nicks */
		return;

	new = xmalloc(sizeof new[0]);
	new->name = name;
	new->next = nil;

	ptr = &n->first;
	while (*ptr != nil)
		ptr = &(*ptr)->next;
	*ptr = new;
}


void
nremove(Nicks *n, char *name)
{
	Nick   *tmp;
	Nick  **newnext;

	newnext = &n->first;
	while (*newnext != nil) {
		if (strequiv((*newnext)->name, name)) {
			tmp = *newnext;
			*newnext = (*newnext)->next;
			free(tmp->name);
			free(tmp);

			return;
		} else {
			newnext = &(*newnext)->next;
		}
	}
}


int
nhas(Nicks *n, const char *name)
{
	Nick   *cur;

	for (cur = n->first; cur != nil; cur = cur->next)
		if (strequiv(cur->name, name))
			return 1;
	return 0;
}


void
nprint(Nicks *n)
{
	Nick   *cur;

	fprintf(stderr, "bot: nprint: ");
	for (cur = n->first; cur != nil; cur = cur->next)
		fprintf(stderr, "`%s' ", cur->name);
	fprintf(stderr, "\n");
}


void
nsave(Nicks *n, const char *file)
{
	Nick   *cur;
	FILE   *fp;

	fp = fopen(file, "w");
	if (fp == nil) {
		warn("opening %s", file);
		return;
	}
	
	for (cur = n->first; cur != nil; cur = cur->next)
		fprintf(fp, "%s\n", cur->name);
	if (ferror(fp))
		warnx("ferror after writing %s", file);

	fclose(fp);
}


Buf *
bnew(int fd)
{
	Buf    *new;

	assert(fd >= 0);

	new = xmalloc(sizeof new[0]);
	new->fd = fd;
	new->s[0] = '\0';
	new->slen = 0;
	new->eof = 0;

	return new;
}


/* trailing newline is kept if present, there could be no newline at all */
int
breadln(Buf *b, char *dest, int destsize)
{
	int	count;
	char   *newline;
	int	flushtonewline;

	assert(destsize >= sizeof b->s);

	if (b->eof)
		return 0;

	flushtonewline = 0;
	while (strchr(b->s, '\n') == nil) {
		if (b->slen == sizeof b->s - 1) {
			/* line too long, flush entire line */
			flushtonewline = 1;
			b->s[0] = '\0';
			b->slen = 0;
		}

		count = read(b->fd, b->s + b->slen, (BBUFFERSIZE - 1) - b->slen);
		if (count == 0) {
			b->eof = 1;

			memmove(dest, b->s, b->slen);
			dest[b->slen] = '\0';
			count = b->slen;
			b->s[0] = '\0';
			b->slen = 0;

			return count;
		}
		if (count == -1)
			return -1;
		b->slen += count;
		b->s[b->slen] = '\0';

		if (flushtonewline) {
			newline = strchr(b->s, '\n');
			if (newline == nil) {
				b->s[0] = '\0';
				b->slen = 0;
			} else {
				memmove(b->s, newline+1, b->slen - (newline - b->s + 1) + 1);
				b->slen -= newline - b->s + 1;
				flushtonewline = 0;
			}
		}
	}

	newline = strchr(b->s, '\n');
	assert(newline != nil);

	count = newline - b->s + 1;
	strncpy(dest, b->s, count);
	dest[count] = '\0';
	memmove(b->s, b->s+count, b->slen - count + 1);
	b->slen -= count;

	return count;
}
