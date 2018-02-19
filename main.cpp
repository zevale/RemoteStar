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

#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef linux
#include <spawn.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <chrono>
#include <thread>
#include <sstream>
#include <vector>
#include <cstring>
#include <fstream>

// Use same Windows.h definitions
#define TRUE 1
#define FALSE 0
#define MAX_PATH PATH_MAX
#endif


// SSH Class hold the data to establish the connection to the SSH server. Required by secureShell() and secureCopy().
class SSH {
    char * _userSSH;
    char * _serverSSH;
public:
    SSH () {
        _userSSH = (char *) "nuno";
        _serverSSH = (char *) "cfar-athena.me.uvic.ca";
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

#ifdef linux
char* const* strArrayToCharPtrConstPtr(std::vector<std::string> _stringArray);
void loadingScreen();
#endif

int main(int argc, char * argv[])
{
    char * output;
    // SSH DATA
    SSH sshConnection;

    // Print connection data
    sshConnection.printConnectionData();

#ifdef _WIN32
    char runStarSource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\runStar";
    char macroSource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\MacroClean.java";
    char domainGeometrySource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\DomainGeometry.x_b";
    char aircraftGeometrySource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\SurfMesh.stl";
#endif
#ifdef linux
    char runStarSource[] = "/home/nuno/Documents/RemoteStar/runStar";
    char macroSource[] = "/home/nuno/Documents/RemoteStar/MacroClean.java";
    char domainGeometrySource[] = "/home/nuno/Documents/RemoteStar/DomainGeometry.x_b";
    char aircraftGeometrySource[] = "/home/nuno/Documents/RemoteStar/SurfMesh.stl";
#endif
    char serverDestination[] = "/home/nuno/Desktop/RemoteStar";

    // SSH command: create RemoteStar folder
    secureShell(sshConnection, (char *) "cd ~/Desktop && mkdir RemoteStar");

    // SCP file - shell script runStar
    secureCopy(sshConnection, runStarSource, serverDestination);

    // SCP file - macro MacroClean.java
    secureCopy(sshConnection, macroSource, serverDestination);

    // SCP file - domain geometry
    secureCopy(sshConnection, domainGeometrySource, serverDestination);

    // SCP file - aircraft geometry SurfMesh.stl
    // secureCopy(sshConnection, aircraftGeometrySource, serverDestination);

    // SSH command: set script permissions
    secureShell(sshConnection, (char *) "cd /home/nuno/Desktop/RemoteStar && chmod 775 runStar");

    // SSH command: run using screen  -d -m means new screen session in detached mode
    secureShellScreen(sshConnection, (char *) "screen -S starSession -d -m /home/nuno/Desktop/RemoteStar/runStar");

    // Connect to screen to monitor
    secureShell(sshConnection, (char *) "screen -r starSession");


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

#ifdef _WIN32
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
#endif

#ifdef linux
    pid_t processID;                                // Holds the child process ID
    int spawnStatus;                                // A nonzero number in case of error

    // Split arguments
    std::string commandArgumentsBuffer;             // Create buffer
    std::stringstream sStream(_commandArgs);        // Put arguments into stream
    std::vector<std::string> stringSplitArguments;  // Vector to store split arguments

    while(sStream >> commandArgumentsBuffer)
        stringSplitArguments.push_back(commandArgumentsBuffer);

    // Convert std::vector<std::string> stringSplitArguments to char* const* charPtrConstPtrArguments
    char* const* charPtrConstPtrArguments = strArrayToCharPtrConstPtr(stringSplitArguments);

    spawnStatus = posix_spawn(&processID, _moduleName, nullptr, nullptr, charPtrConstPtrArguments, nullptr);
    if((spawnStatus) == 0) {
        std::cout << "SPAWN_POSIX PID = " << processID << std::endl;
        if (waitpid(processID, &spawnStatus, 0) != -1) {
            std::cout << "SPAWN_POSIX CHILD EXITED WITH STATUS = " << spawnStatus << std::endl;
        } else {
            error("ERROR: waitpid");
            return FALSE;
        }
    } else {
        std::cerr << "ERROR: posix_spawn error number " << spawnStatus << std::endl;
        return FALSE;
    }
#endif
    return TRUE;
}

// Additional function required to convert the arguments in a string array to a char* const* for use in spaw_posix
#ifdef linux
char* const* strArrayToCharPtrConstPtr(std::vector<std::string> _stringArray)
{
    size_t nArguments = _stringArray.size();
    char** convertedString = new char*[nArguments + 1];                 // Extra argument for NULL at the end
    for (size_t i = 0; i < nArguments; ++i) {
        convertedString[i] = new char[_stringArray[i].size() + 1];      // Allocate enough space
        strcpy(convertedString[i], _stringArray[i].c_str());            // Copy string
    }
    convertedString[nArguments] = nullptr;
    return convertedString;
}
#endif


int secureCopy(SSH _sshConnection, char *_sourceFilePath, char *_destinationPath)
{
#ifdef _WIN32
    // Get the scp.exe module. Important: folder is Sysnative to access 64-bit System32 folder from 32-bit program
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\scp.exe";
#endif

#ifdef linux
    // Get the scp module
    char moduleName[] = "/usr/bin/scp";
#endif
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
#ifdef _WIN32
    // Get the ssh.exe module. Important: folder is Sysnative to access 64-bit System32 folder from 32-bit program
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\ssh.exe";
#endif

#ifdef linux
    // Get the ssh module
    char moduleName[] = "/usr/bin/ssh";
#endif

    // Generate the ssh command
    int commandArgsSize = 1024;
    char commandArgs[commandArgsSize];
    memset(commandArgs, '\0', sizeof(commandArgs));
    sprintf(commandArgs, "ssh -t %s@%s ""%s""", _sshConnection.getUserSSH(), _sshConnection.getServerSSH(),
            _commandToExecute);

    // Message to user
    std::cout << "\n:::::::::::: SSH COMMAND" << std::endl;
    std::cout << "    COMMAND: " << _commandToExecute << "\n" << std::endl;

    // Execute SSH
#ifdef _WIN32
    if(!(executeProcess(moduleName, commandArgs))) {
        std::cerr << "ERROR: cannot execute SSH command" << std::endl;
        return FALSE;
    }
#endif

#ifdef linux
    // Check if it is a screen command
    if(strncmp(_commandToExecute, (char *) "screen", 6) == 0){
        // Use system for screen command
        std::cout << "    CONNECTING TO " << _sshConnection.getServerSSH() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        loadingScreen();
        system(commandArgs);
    } else {
        // Run command using execute
        if(!(executeProcess(moduleName, commandArgs))) {
            std::cerr << "ERROR: cannot execute SSH command" << std::endl;
            return FALSE;
        }
    }
#endif
    return TRUE;
}


int secureShellScreen(SSH _sshConnection, char *_commandToExecute)
{
#ifdef _WIN32
    // Get the ssh.exe module. Important: folder is Sysnative to access 64-bit System32 folder from 32-bit program
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\ssh.exe";
#endif

#ifdef linux
    // Get the ssh module
    char moduleName[] = "/usr/bin/ssh";
#endif

    // Generate the scp command
    int commandArgsSize = 1024;
    char commandArgs[commandArgsSize];
    memset(commandArgs, '\0', sizeof(commandArgs));
    sprintf(commandArgs, "ssh %s@%s ""%s""", _sshConnection.getUserSSH(), _sshConnection.getServerSSH(),
            _commandToExecute);

    // Message to user
    std::cout << "\n:::::::::::: SSH SCREEN" << std::endl;
    std::cout << "    COMMAND: " << _commandToExecute << std::endl;

    // Execute SSH screen
    if(!(executeProcess(moduleName, commandArgs))) {
        std::cerr << "ERROR: cannot execute SSH screen" << std::endl;
        return FALSE;
    }
#ifdef _WIN32
    system("cls");
#endif

#ifdef linux
    system("clear");
#endif
    return TRUE;
}

#ifdef linux
void loadingScreen(){
    system("clear");
    std::string buffer;
    std::fstream textFile;
    textFile.open("screen1");
    if(!textFile) {
        std::cerr << "ERROR: cannot open loading screen" << std::endl;
    }
    while(!textFile.eof()) {
        getline(textFile, buffer);
        std::this_thread::sleep_for(std::chrono::milliseconds(32));
        std::cout << buffer << std::endl;
    }
    textFile.close();
}
#endif