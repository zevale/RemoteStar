/*
 * star_client.h
 *
 * Functions used in the client that sends data and jobs
 * to the STAR CCM+ server.
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 */

#ifndef STAR_CLIENT_H
#define STAR_CLIENT_H

#include "SSH.h"        // Class SSH used to store ssh connection data
#include "StarHost.h"

#ifdef linux            // Linux systems require an additional function to parse arguments for spawn
#include <vector>
#endif

// FORWARD DECLARATIONS

// Print interpreted error message
void error(const char* _errorMessage);

/* Execute SSH commands which must be concatenated like "cmd1 && cmd2 && cmd3 & ...".
 * Must use an authentication key for automatic login.
 */
int secureShell(SSH _sshConnection, char *_commandToExecute);

/*
 * Execute screen instruction needed to start STAR CCM+ as an independent process
 * that keeps running in case of ssh connection failure.
 */
int secureShellScreen(SSH _sshConnection, char *_commandToExecute);

/* Execute SCP from client computer to SSH server.
 * Must use an authentication key.
 */
int secureCopy(SSH _sshConnection, char *_sourceFilePath, char *_destinationPath);

/*
 * Executes a Windows process using createProcessA.
 * Executes a Linux process using spawn. When connecting screen, Linux clients use system().
 */
int executeProcess(char * _moduleName, char * _commandArgs);

/* Loading screen while STAR CCM+ is loading.
 * A custom text message can be displayed line-by-line while the client is connecting to the server.
 */
void loadingScreen(SSH _sshConnection);

/*
 * Using spanw_posix() requires the command line arguments to be type char * const *.
 * strArrayToCharPtrConstPtr will convert the split arguments in a vector<string> into char * const *
 */
#ifdef linux
char* const* strArrayToCharPtrConstPtr(std::vector<std::string> _stringArray);
#endif

/*
 * This function will generate the shell script that starts up STAR CCM+
 * Will also probably need a StarMacro class that contains information about the macro file path
 */
int initializeStarHost(StarHost& _starHost);


#endif //SSH_H