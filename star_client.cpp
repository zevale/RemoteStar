#include <iostream>
#include <cstdlib>
#include <fstream>

#include "star_client.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef linux
#include <spawn.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <sstream>
#include <cstring>
#include <thread>
#include <chrono>

// Use same Windows.h definitions
#define TRUE 1
#define FALSE 0
#define MAX_PATH PATH_MAX
#endif

// FUNCTIONS DEFINITIONS

void error(const char* _errorMessage)
{
    perror(_errorMessage);
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
        if (waitpid(processID, &spawnStatus, 0) == -1) {
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

    // Check is screen command
    bool screenConnect = false;
    if(strncmp(_commandToExecute, (char *) "screen", 6) == 0)
        screenConnect = true;

    if(screenConnect)
        loadingScreen(_sshConnection);

    // Execute SSH
#ifdef _WIN32
    if(!(executeProcess(moduleName, commandArgs))) {
        std::cerr << "ERROR: cannot execute SSH command" << std::endl;
        return FALSE;
    }
#endif

#ifdef linux
    // Check if it is a screen command
    if(screenConnect){
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
    return TRUE;
}

void loadingScreen(SSH _sshConnection){
#ifdef _WIN32
    system("cls");
#endif
#ifdef linux
    system("clear");
#endif

    // Connecting
    std::cout << "\n    CONNECTING TO " << _sshConnection.getServerSSH() << std::endl;
#ifdef _WIN32
    Sleep(3000);
    system("cls");
#endif
#ifdef linux
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    system("clear");
#endif

    // Loading screen
    std::string buffer;
    std::ifstream textFile;
#ifdef _WIN32
    textFile.open("C:\\Users\\Nuno\\Dev\\RemoteStar\\screen1");
#endif
#ifdef linux
    textFile.open("/home/nuno/Dev/RemoteStar/screen1");
#endif
    if(!textFile) {
        std::cerr << "ERROR: cannot open loading screen" << std::endl;
    } else {
        while(!textFile.eof()) {
            getline(textFile, buffer);
#ifdef _WIN32
            Sleep(32);
#endif
#ifdef linux
            std::this_thread::sleep_for(std::chrono::milliseconds(32));
#endif
            std::cout << buffer << std::endl;
        }
        textFile.close();
    }
}

int initializeStarHost(StarHost& _starHost)
{
    // Load hosts
    try
    {
        _starHost.loadHostList();
    }
    catch (const char* loadHostException)
    {
        // Error loading hosts from <host_list>
        std::cerr << "ERROR: " << loadHostException << std::endl;
        return FALSE;
    }
    // Show host list
    _starHost.printHostList();

    // Write shell script to run simulation on hosts loaded
    // Open file
    std::ofstream shellRunScript("/home/nuno/Dev/RemoteStar/run_star");

    // Check if file is open
    if(!shellRunScript.is_open())
        throw "Cannot write shell script <run_star>";

    std::cout << "\n Press any key to continue..." << std::endl;
    std::cin.get();
    return TRUE;
}