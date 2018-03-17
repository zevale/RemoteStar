/*
 * LIBRARY star_client
 *
 * DESCRIPTION
 * Functions used in the client that sends data and jobs
 * to the STAR CCM+ server.
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 */

#ifndef STAR_CLIENT_H
#define STAR_CLIENT_H

#include "SSH.h"        // Class SSH used to store ssh connection data
#include "StarHost.h"   // Class used to store the host list for parallel session
#include "StarJob.h"    // Class used to store STAR CCM+ job data

// Linux ans Mac support
#if defined(linux) || defined(__APPLE__)
#include <vector>       // Linux systems require an additional function to parse arguments for spawn
#include <unistd.h>     // To work with directories
#endif

// When using scp the command syntax changes if copying from or to server
enum CopyDirection{
    TO_SERVER = 0,
    FROM_SERVER,
};

// When using scp the commend syntax changes is copying a file or a folder
enum CopyOptions{
    COPY_FILE = 0,
    COPY_FOLDER,
};

enum Color{
    RED = 0,
    GREEN,
    BLUE,
    AQUA,
    YELLOW,
    WHITE_BLUE,
};

// GENERAL FUNCTIONALITY

// Print interpreted error message and exit
void errorInterpreted(const std::string& _errorMessage);

// Print error message to std::cerr and exit
void exitNow(const std::string& _errorMessage);

// Change current directory
void changeWorkingDirectory(const std::string &_workingDirectory);

// Get current working directory
std::string getWorkingDirectory();

// Check if file exists in a directory
bool fileExists(const std::string& _filePath);

// STAR CLIENT SPECIFIC FUNCTIONS

/*
 * Executes a Windows process using createProcessA.
 * Executes a Linux process using spawn. When connecting screen, Linux clients use system().
 */
int executeProcess(char * _moduleName, char * _commandArgs);

/*
 * Using spawn_posix() requires the command line arguments to be type char * const *.
 * strArrayToCharPtrConstPtr will convert the split arguments in a vector<string> into char * const *
 */
#if defined(linux) || defined(__APPLE__)
char* const* strArrayToCharPtrConstPtr(std::vector<std::string> _stringArray);
#endif

/* Execute SSH commands which must be concatenated like "cmd1 && cmd2 && cmd3 & ...".
 * Must use an authentication key for automatic login.
 */
int secureShell(const SSH& _sshConnection, const std::string& _commandToExecute);

/*
 * Execute screen instruction needed to start STAR CCM+ as an independent process
 * that keeps running in case of ssh connection failure.
 */
int secureShellScreen(const SSH& _sshConnection, const std::string& _commandToExecute);

/* Execute SCP from client computer to SSH server.
 * Must use an authentication key.
 */
int secureCopy(const SSH& _sshConnection, const std::string& _sourceFilePath,
               const std::string& _destinationPath, CopyDirection _copyDirection, CopyOptions _copyOption);

/* Loading screen while STAR CCM+ is loading.
 * A custom text message can be displayed line-by-line while the client is connecting to the server.
 */
void loadingScreen(const SSH& _sshConnection);

/*
 * Loads the SSH server data from file <star_sshServer> and initializes the _sshConnection
 */
int initializeSSH(SSH& _sshConnection);

/*
 * Loads the host list from file <star_hostList> and generates the shell script that starts up STAR CCM+
 * Will also probably need a StarMacro class that contains information about the macro file path
 */
int initializeStarHost(StarHost& _starHost, const StarJob& _starJob);

/*
 * Loads the job data file <star_jobData> required to run the STAR CCM+ simulation
 */
int initializeStarJob(StarJob& _starJob);

/*
 * Sends resources to SSH server, submits job to hosts and connects screen to SSH server
 */
void submitJob(const SSH& _sshConnection, const StarJob& _starJob);

/*
 * Gets the results from the server
 */
int fetchResults(const SSH& _sshConnection, const StarJob& _starJob);
#endif // STAR_CLIENT_H

/*
 * Prints colored text to the console
 */
void colorText(const std::string& _text, Color _color);