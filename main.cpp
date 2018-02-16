/*
 * RemoteStar
 *
 * This program copies the files needed to run a STAR CCM+ simulation
 * on a remote server and launches the simulation
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 */

#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <curses.h>


// FORWARD DECLARATIONS

// Print interpreted error message
void error(const char* errorMessage);

// Execute SSH commands which must be concatenated like "cmd1 && cmd2 && cmd3 & ...". Must use an authentication key.
bool secureShell(char *_userSSH, char *_serverSSH, char *_commandToExecute);

// Execute SCP from client computer to SSH server. Must use an authentication key.
bool secureCopy(char *_userSSH, char *_serverSSH, char *_sourceFilePath, char *_destinationPath);

// Executes a process using createProcessA.
bool executeProcess(char * _moduleName, char * _commandArgs);

int main(int argc, char * argv[])
{
    // SSH DATA
    char userSSH[] = "nuno";
    char serverSSH[] = "cfar-athena.me.uvic.ca";

    // SSH command
    secureShell(userSSH, serverSSH, "cd ~/Desktop && mkdir SendTest");

    // SCP file
    secureCopy(userSSH, serverSSH, "C:\\Users\\Nuno\\Desktop\\STAR-CCM+11.06.010_02_linux-x86_64-r8.tar.gz",
               "/home/nuno/Desktop/SendTest");

    /* secureShell and secureCopy output eventual errors to cerr. These errors must be handled so that the simulation
     * does not stop.
     */

    return EXIT_SUCCESS;
}

// FUNCTIONS DEFINITIONS

void error(const char* errorMessage)
{
    perror(errorMessage);
    exit(EXIT_FAILURE);
}

bool executeProcess(char * _moduleName, char * _commandArgs)
{
    // Check if the moduleName exists first!

    // CreateProcess API requires these two structures
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInformation;

    // Initialize the structures (ZeroMemory = memset)
    ZeroMemory( &startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInformation, sizeof(processInformation));

    // Start the child process - SCP
    if( !CreateProcessA(
            moduleName,             // No module name
            commandArgs,            // Command line
            nullptr,                // Process handle not inheritable
            nullptr,                // Thread handle not inheritable
            FALSE,                  // Set handle inheritance to FALSE
            0,                      // No creation flags
            nullptr,                // Use parent's environment block
            nullptr,                // Use parent's starting directory
            &startupInfo,           // Pointer to STARTUPINFO structure
            &processInformation))   // Pointer to PROCESS_INFORMATION structure
    {
        // Failed to create process
        return FALSE;
    }

    // Wait until child process exits
    WaitForSingleObject(processInformation.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(processInformation.hProcess);
    CloseHandle(processInformation.hThread);

    return TRUE;
}

bool secureCopy(char *_userSSH, char *_serverSSH, char *_sourceFilePath, char *_destinationPath)
{
    // Get the scp.exe module
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\scp.exe";

    // Generate the scp command assuming its size is less than MAX_PATH*3 (2 paths + commands)
    char commandArgs[MAX_PATH*3];
    memset(commandArgs, '\0', sizeof(commandArgs));
    sprintf(commandArgs, "scp %s %s@%s:%s", _sourceFilePath, _userSSH, _serverSSH, _destinationPath);

    // Message to user
    std::cout << "\n:::::::::::: SENDING" << std::endl;
    std::cout << "       FILE: " << _sourceFilePath << std::endl;
    std::cout << "DESTINATION: " << _userSSH << "@" << _serverSSH << ":" << _destinationPath << std::endl;

    // Execute SCP
    if(!executeProcess(moduleName, commandArgs)) {
        std::cerr << "ERROR: cannot execute SCP" << std::endl;
        return FALSE;
    }

    return TRUE;
}

bool secureShell(char *_userSSH, char *_serverSSH, char *_commandToExecute)
{
    // Get the ssh.exe module
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\ssh.exe";

    // Generate the scp command
    int commandArgsSize = 1024;
    char commandArgs[commandArgsSize];
    memset(commandArgs, '\0', sizeof(commandArgs));
    sprintf(commandArgs, "ssh  -t %s@%s %s", _userSSH, _serverSSH, _commandToExecute);

    // Message to user
    std::cout << ":::::::::::: SSH COMMAND" << std::endl;
    std::cout << "    COMMAND: " << _commandToExecute << std::endl;

    // Execute SSH
    if(!(executeProcess(moduleName, commandArgs))) {
        std::cerr << "ERROR: cannot execute SSH command" << std::endl;
        return FALSE;
    }

    return TRUE;
}