#ifndef GSM_MODEM_H_INCLUDED
#define GSM_MODEM_H_INCLUDED


int StartGSMModem(char * device,char * spool_dir);
int StopGSMModem();
int SendSMS(char * number,char * sms_message);
int ReceiveSMS(char * number,unsigned int number_size,char * sms_message,int sms_message_size);

#endif // GSM_MODEM_H_INCLUDED
