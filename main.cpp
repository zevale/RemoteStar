/*
 * RemoteStar
 *
 * This program copies the files needed to run a STAR CCM+ simulation
 * on a remote server and launches the simulation.
 *
 * Files are copied with scp and commands are sent with ssh.
 * This requires using Microsoft's native OpenSSH client.
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 */

#include <iostream>
#include <Windows.h>
#include <cstdlib>


// SSH Class hold the data to establish the connection to the SSH server. Required by secureShell() and secureCopy().
class SSH {
    char * _userSSH;
    char * _serverSSH;
public:
    SSH () {
        _userSSH = "nuno";
        _serverSSH = "cfar-athena.me.uvic.ca";
    }
    void setUserSSH(char * _newUserSSH) {
        _userSSH = _newUserSSH;
    }
    void setServerSSH(char * _newServerSSH) {
        _serverSSH = _newServerSSH;
    }
    char * getUserSSH() {
        return _userSSH;
    }
    char * getServerSSH() {
        return _serverSSH;
    }
    void printConnectionData() {
        std::cout << "\n:::::::::::: SSH CONNECTION" << std::endl;
        std::cout << "       USER: " << _userSSH << std::endl;
        std::cout << "     SERVER: " << _serverSSH << std::endl;
    }
};

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

int main(int argc, char * argv[])
{
    // SSH DATA
    SSH sshConnection;

    // Print connection data
    sshConnection.printConnectionData();

    // SSH command: create RemoteStar folder
    secureShell(sshConnection, "cd ~/Desktop && mkdir RemoteStar");

    // SCP file - shell script RunStar
    secureCopy(sshConnection, "C:\\Users\\Nuno\\Desktop\\RemoteStar\\runStar",
               "/home/nuno/Desktop/RemoteStar");

    // SCP file - shell script launcher
    secureCopy(sshConnection, "C:\\Users\\Nuno\\Desktop\\RemoteStar\\launcher",
               "/home/nuno/Desktop/RemoteStar");

    // SCP file - macro MacroClean.java
    secureCopy(sshConnection, "C:\\Users\\Nuno\\Desktop\\RemoteStar\\MacroClean.java",
               "/home/nuno/Desktop/RemoteStar");

    // SCP file - domain geometry
    secureCopy(sshConnection, "C:\\Users\\Nuno\\Desktop\\RemoteStar\\DomainGeometry.x_b",
               "/home/nuno/Desktop/RemoteStar");

    // SCP file - aircraft geometry SurfMesh.stl
    secureCopy(sshConnection, "C:\\Users\\Nuno\\Desktop\\RemoteStar\\SurfMesh.stl",
               "/home/nuno/Desktop/RemoteStar");

    // SSH command: set script permissions
    secureShell(sshConnection, "cd /home/nuno/Desktop/RemoteStar && chmod 775 runStar");

    // SSH command: run using screen  -d -m means new screen session in detached mode
    secureShellScreen(sshConnection, "screen -S starSession -d -m /home/nuno/Desktop/RemoteStar/runStar");

    // Connect to screen to monitor
    secureShell(sshConnection, "screen -r starSession");

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

int executeProcess(char * _moduleName, char * _commandArgs)
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
            _moduleName,            // No module name
            _commandArgs,           // Command line
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

int secureCopy(SSH _sshConnection, char *_sourceFilePath, char *_destinationPath)
{
    // Get the scp.exe module
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\scp.exe";

    // Generate the scp command assuming its size is less than MAX_PATH*3 (2 paths + commands)
    char commandArgs[MAX_PATH*3];
    memset(commandArgs, '\0', sizeof(commandArgs));
    sprintf(commandArgs, "scp %s %s@%s:%s", _sourceFilePath, _sshConnection.getUserSSH(),
            _sshConnection.getServerSSH(), _destinationPath);

    // Message to user
    std::cout << "\n:::::::::::: SENDING" << std::endl;
    std::cout << "       FILE: " << _sourceFilePath << std::endl;
    std::cout << "DESTINATION: " << _destinationPath << std::endl;

    // Execute SCP
    if(!executeProcess(moduleName, commandArgs)) {
        std::cerr << "ERROR: cannot execute SCP" << std::endl;
        return FALSE;
    }
    return TRUE;
}

int secureShell(SSH _sshConnection, char *_commandToExecute)
{
    // Get the ssh.exe module
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\ssh.exe";

    // Generate the scp command
    int commandArgsSize = 1024;
    char commandArgs[commandArgsSize];
    memset(commandArgs, '\0', sizeof(commandArgs));
    sprintf(commandArgs, "ssh  -t %s@%s ""%s""", _sshConnection.getUserSSH(), _sshConnection.getServerSSH(),
            _commandToExecute);

    // Message to user
    std::cout << "\n:::::::::::: SSH COMMAND" << std::endl;
    std::cout << "    COMMAND: " << _commandToExecute << "\n" << std::endl;

    // Execute SSH
    if(!(executeProcess(moduleName, commandArgs))) {
        std::cerr << "ERROR: cannot execute SSH command" << std::endl;
        return FALSE;
    }
    return TRUE;
}

int secureShellScreen(SSH _sshConnection, char *_commandToExecute)
{
    // Get the ssh.exe module
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\ssh.exe";

    // Generate the scp command
    int commandArgsSize = 1024;
    char commandArgs[commandArgsSize];
    memset(commandArgs, '\0', sizeof(commandArgs));
    sprintf(commandArgs, "ssh %s@%s ""%s""", _sshConnection.getUserSSH(), _sshConnection.getServerSSH(),
            _commandToExecute);

    // Message to user
    std::cout << "\n:::::::::::: SSH COMMAND" << std::endl;
    std::cout << "    COMMAND: " << _commandToExecute << std::endl;

    // Execute SSH
    if(!(executeProcess(moduleName, commandArgs))) {
        std::cerr << "ERROR: cannot execute SSH command" << std::endl;
        return FALSE;
    }
    system("cls");
    return TRUE;
}