
#ifndef STAR_CLIENT_H
#define STAR_CLIENT_H

#include "SSH.h"

// FORWARD DECLARATIONS

// Print interpreted error message
void error(const char* errorMessage);

// Execute SSH commands which must be concatenated like "cmd1 && cmd2 && cmd3 & ...". Must use an authentication key.
int secureShell(SSH _sshConnection, char *_commandToExecute);

int secureShellScreen(SSH _sshConnection, char *_commandToExecute);

// Execute SCP from client computer to SSH server. Must use an authentication key.
int secureCopy(SSH _sshConnection, char *_sourceFilePath, char *_destinationPath);

// Executes a process using createProcessA.
int executeProcess(char * _moduleName, char * _commandArgs);

// Loading screen while STAR CCM+ is loading
void loadingScreen();

#ifdef linux
char* const* strArrayToCharPtrConstPtr(std::vector<std::string> _stringArray);
#endif

#endif //SSH_H
