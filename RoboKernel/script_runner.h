#ifndef SCRIPT_RUNNER_H_INCLUDED
#define SCRIPT_RUNNER_H_INCLUDED

int StartScript(char * script_name , char * from, int recursion_level);
int StopScript(char * script_name);

int StartScriptRunnerServer();
int StopScriptRunnerServer();

#endif // SCRIPT_RUNNER_H_INCLUDED
