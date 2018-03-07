#include "star_client.h"

#include <iostream>
#include <cstdlib>
#include <fstream>

// Windows support
#ifdef _WIN32
#include <Windows.h>
#endif

// Linux support
#ifdef linux
#include <unistd.h>
#include <spawn.h>              // Because of posix_spawn()
#include <sys/wait.h>           // Because of waitpid()
#include <linux/limits.h>       // Because of the constant MAX_PATH
#include <sstream>              // String stream classes
#include <thread>               // To halt the thread while loadingScreen()
#include <chrono>               // Because of miliseconds()
#include <cstring>

// Use same Windows.h definitions (TRUE FALSE PATH_MAX)
#define TRUE 1
#define FALSE 0
#define MAX_PATH PATH_MAX
#endif

// GENERAL FUNCTIONALITY

// Print interpreted error message and exit
void errorInterpreted(const std::string& _errorMessage) {
    perror(_errorMessage.c_str());
    exit(EXIT_FAILURE);
}

// Print error message to std::cerr and exit
void exitNow(const std::string& _errorMessage) {
    std::cerr << _errorMessage << std::endl;
    exit(EXIT_FAILURE);
}

// Change current directory
void changeWorkingDirectory(const std::string &_workingDirectory) {
//    std::string workingDirectory;
//    std::cout << "\nPlease enter working directory: " << std::endl;
//    std::cin >> workingDirectory; std::cin.ignore();
#ifdef linux
    if(chdir(_workingDirectory.c_str())<0)
        errorInterpreted("ERROR: cannot change directory");
#endif
}

// Get current working directory
std::string getWorkingDirectory() {
    char currentDirectory[MAX_PATH];
    memset(currentDirectory, '\0', MAX_PATH);
#ifdef _WIN32
    _getcwd(currentDirectory, MAX_PATH);
#endif
#ifdef linux
    getcwd(currentDirectory, MAX_PATH);
#endif
    return std::string(currentDirectory);
}

// Check if file exists in a directory
bool fileExists(const std::string& _filePath) {
    std::ifstream testFile(_filePath.c_str());
    return testFile.good();
}

// STAR CLIENT SPECIFIC FUNCTIONS

/*
 * executeProcess()
 *
 * DESCRIPTION
 * Executes a Windows process using createProcessA.
 * Executes a Linux process using spawn. When connecting screen,
 * Linux clients use system().
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int executeProcess(char * _moduleName, char * _commandArgs) {

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

    // Start the child process
    spawnStatus = posix_spawn(&processID, _moduleName, nullptr, nullptr, charPtrConstPtrArguments, nullptr);
    if((spawnStatus) == 0) {
        if (waitpid(processID, &spawnStatus, 0) == -1) {
            errorInterpreted("ERROR: waitpid");
            return FALSE;
        }
    } else {
        std::cerr << "ERROR: posix_spawn error number " << spawnStatus << std::endl;
        return FALSE;
    }
#endif
    return TRUE;
}

/*
 * strArrayToCharPtrConstPtr()
 *
 * DESCRIPTION
 * Using spawn_posix() requires the command line arguments to be type char * const *.
 * strArrayToCharPtrConstPtr will convert the split arguments in a vector<string> into char * const *
 *
 * NOTES
 * Last command argument must be NULL
 *
 * RETURN
 * Spit arguments as char * const *
 */
#ifdef linux
char* const* strArrayToCharPtrConstPtr(std::vector<std::string> _stringArray)
{
    size_t nArguments = _stringArray.size();                            // Number of arguments
    char** convertedString = new char*[nArguments + 1];                 // Extra argument for NULL at the end
    for (size_t i = 0; i < nArguments; ++i) {
        convertedString[i] = new char[_stringArray[i].size() + 1];      // Allocate enough space
        strcpy(convertedString[i], _stringArray[i].c_str());            // Copy string
    }
    convertedString[nArguments] = nullptr;                              // NULL at the end
    return convertedString;
}
#endif

/*
 * secureShell()
 *
 * DESCRIPTION
 * Execute SSH commands which must be concatenated like "cmd1 && cmd2 && cmd3 & ...".
 * Must use an authentication key for automatic login.
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int secureShell(const SSH& _sshConnection, const std::string& _commandToExecute) {
#ifdef _WIN32
    // Get the ssh.exe module. Important: folder is Sysnative to access 64-bit System32 folder from 32-bit program
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\ssh.exe";
#endif
#ifdef linux
    // Get the ssh module
    char moduleName[] = "/usr/bin/ssh";
#endif
    // Generate the ssh command assuming its maximum size is MAX_PATH*3
    char commandArgs[MAX_PATH*3];
    memset(commandArgs, '\0', sizeof(commandArgs));
    sprintf(commandArgs, "ssh -t %s@%s ""%s""", _sshConnection.getUser().c_str(), _sshConnection.getServer().c_str(),
            _commandToExecute.c_str());

    // Message to user
    std::cout << "\n:::::::::::: SSH COMMAND" << std::endl;
    std::cout << "    COMMAND: " << _commandToExecute << "\n" << std::endl;

    // Check if screen command
    bool screenConnect = false;
    if(strncmp(_commandToExecute.c_str(), (char *) "screen", 6) == 0)
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
        // Use system()
        system(commandArgs);
    } else {
        // Run command using executeProcess()
        if(!(executeProcess(moduleName, commandArgs))) {
            std::cerr << "ERROR: cannot execute SSH command" << std::endl;
            return FALSE;
        }
    }
#endif
    return TRUE;
}

/*
 * secureShellScreen()
 *
 * DESCRIPTION
 * Execute screen instruction needed to start STAR CCM+ as an independent process
 * that keeps running in case of ssh connection failure.
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int secureShellScreen(const SSH& _sshConnection, const std::string& _commandToExecute) {
#ifdef _WIN32
    // Get the ssh.exe module. Important: folder is Sysnative to access 64-bit System32 folder from 32-bit program
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\ssh.exe";
#endif
#ifdef linux
    // Get the ssh module
    char moduleName[] = "/usr/bin/ssh";
#endif

    // Generate the ssh command assuming its maximum size is MAX_PATH*3
    char commandArgs[MAX_PATH*3];
    memset(commandArgs, '\0', sizeof(commandArgs));
    sprintf(commandArgs, "ssh %s@%s ""%s""", _sshConnection.getUser().c_str(), _sshConnection.getServer().c_str(),
            _commandToExecute.c_str());

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

/*
 * secureCopy()
 *
 * DESCRIPTION
 * Execute SCP from client computer to SSH server. Client must use an authentication key.
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int secureCopy(const SSH& _sshConnection, const std::string& _sourceFilePath,
               const std::string& _destinationPath, CopyDirection _copyDirection, CopyOptions _copyOptions) {
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
    memset(commandArgs, '\0', MAX_PATH*3);

    // Check copy options
    std::string scp("scp");
    if(_copyOptions == COPY_FOLDER){
        // Add recursive option
        scp = scp + " -r";
    }

    // Check copy direction
    if(_copyDirection == TO_SERVER){
        sprintf(commandArgs, "%s %s %s@%s:%s", scp.c_str(), _sourceFilePath.c_str(), _sshConnection.getUser().c_str(),
                _sshConnection.getServer().c_str(), _destinationPath.c_str());
        // Message to user
        std::cout << "\n:::::::::::: SENDING" << std::endl;
    } else {
        sprintf(commandArgs, "%s %s@%s:%s %s",scp.c_str(), _sshConnection.getUser().c_str(),
                _sshConnection.getServer().c_str(), _sourceFilePath.c_str(), _destinationPath.c_str());
        // Message to user
        std::cout << "\n:::::::::::: RECEIVING" << std::endl;
    }
    // Message to user (cont)
    std::cout << "       FILE: " << _sourceFilePath << std::endl;
    std::cout << "DESTINATION: " << _destinationPath << std::endl;

    // Execute SCP
    if(!executeProcess(moduleName, commandArgs)) {
        std::cerr << "ERROR: cannot execute SCP" << std::endl;
        return FALSE;
    }
    return TRUE;
}

/*
 * loadingScreen()
 *
 * DESCRIPTION
 * Loading screen while STAR CCM+ is loading.
 * A custom text message can be displayed line-by-line while the client is connecting to the server.
 */
void loadingScreen(const SSH& _sshConnection){
    // Clear the console
#ifdef _WIN32
    system("cls");
#endif
#ifdef linux
    system("clear");
#endif
    // Connecting to
    std::cout << "\n    CONNECTING TO " << _sshConnection.getServer() << std::endl;
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

/*
 * initializeSSH()
 *
 * DESCRIPTION
 * Loads the SSH server data from file <star_sshServer> and initializes
 * the _sshConnection
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int initializeSSH(SSH& _sshConnection) {
    // Load ssh server data
    try {
        _sshConnection.loadSSH();
    } catch (const char* loadSshException) {
        // Error loading ssh server data from <star_sshServer>
        std::cerr << "\nERROR: " << loadSshException << std::endl;
        return FALSE;
    }
    // Show SSH server
    _sshConnection.printConnectionData();
    return TRUE;
}

/*
 * initializeStarHost()
 *
 * DESCRIPTION
 * Loads the host list from file <star_hostList> and generates the
 * shell script that starts up STAR CCM+
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int initializeStarHost(StarHost& _starHost, const StarJob& _starJob) {
    // Load hosts and write shell script
    try {
        _starHost.loadHostList();

        // Write shell script to run simulation on hosts loaded
        // Open file
        std::ofstream shellRunScriptFile(_starJob.getClientJobDirectory("resources/star_runScript"));

        // Check if file is open
        if(!shellRunScriptFile)
            throw "Cannot write shell script <star_runScript>";

        // Initialize sheBang and STAR CCM+ command line arguments
        std::string sheBang = "#!/bin/sh";
        std::string starPath = "/opt/CD-adapco/12.02.011-R8/STAR-CCM+12.02.011-R8/star/bin/starccm+ ";
        std::string starLicense = "-power ";
        std::string starExit = "-noexit ";
        std::string starInfiniBand = "-fabric IBV ";
        std::string starHost;
        std::string starMacro = " -batch ";
        std::string macroPath = _starJob.getServerJobDirectory("resources/MightyMacro.java");

        // Generate host list
        int nHosts = _starHost.getNumHosts();
        if(nHosts == 1){
            // One host: has to be submitted to the same ssh server and only need to add -np
            starHost = std::string("-np ") + std::to_string(_starHost.getProcesses(0));
        } else {
            // Generate host list
            starHost = starInfiniBand + std::string("-on ");
            for (int i = 0; i < nHosts ; ++i) {
                if(_starHost.getHostType(i) == LOCALHOST){
                    starHost += "localhost:" + std::to_string(_starHost.getProcesses(i));
                } else {
                    starHost += _starHost.getAddress(i).append(":") + std::to_string(_starHost.getProcesses(i));
                }
                // Add comma between hosts
                if(i < nHosts-1)
                    starHost += ",";
            }
        }
        // Output to file and close
        shellRunScriptFile << sheBang << std::endl;
        shellRunScriptFile << starPath << starLicense << starExit << starHost << starMacro << macroPath;
        shellRunScriptFile.close();
    } catch (const char* loadHostException) {
        // Error loading hosts from <star_hostList>
        std::cerr << "\nERROR: " << loadHostException << std::endl;
        return FALSE;
    }
    // Show host list
    _starHost.printHostList();

//    std::cout << "\n Press <enter> to continue..." << std::endl;
//    std::cin.get();
    return TRUE;
}

/*
 * initializeStarJob()
 *
 * DESCRIPTION
 * Loads the job data from file <star_jobData
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int initializeStarJob(StarJob& _starJob) {
    // Load job data and check sim files exist
    try {
        // Load job
        _starJob.loadStarJob();

        // Check aircraft geometry
        if(!fileExists(_starJob.getClientJobDirectory("resources/SurfMesh.stl")))
            throw "Aircraft geometry not found";

        // Check domain geometry
        if(!fileExists(_starJob.getClientJobDirectory("resources/DomainGeometry.x_b")))
            throw "Domain geometry not found";

    } catch(const char * loadJobException) {
        // Error loading hosts from <star_hostList>
        std::cerr << "ERROR: " << loadJobException << std::endl;
        return FALSE;
    }
    // Check resources: <star_sshServer> <star_hostList>

    // Print job data
       _starJob.printJobData();

    // Change directory
    changeWorkingDirectory(_starJob.getClientDirectory());
    return TRUE;
}

/*
 * submitJob()
 *
 * DESCRIPTION
 * Sends resources to SSH server, submits job to hosts and connects screen to SSH server
 */
void submitJob(const SSH& _sshConnection, const StarJob& _starJob) {

    // SSH command: create job folder
    std::string createServerFolders = "mkdir -p " + _starJob.getServerJobDirectory();
    secureShell(_sshConnection, createServerFolders);

    // SCP client resources folder
    secureCopy(_sshConnection, _starJob.getClientJobDirectory("resources/"), _starJob.getServerJobDirectory(),
               TO_SERVER, COPY_FOLDER);

    // SSH command: set script permissions on server
    std::string setScriptPermissions = "cd " + _starJob.getServerJobDirectory("resources/") +
            " && chmod 775 star_runScript";
    secureShell(_sshConnection, setScriptPermissions);

    // SSH command: run using screen  -d -m means new screen session in detached mode
    std::string newScreenSession = "screen -S starSession -d -m " +
            _starJob.getServerJobDirectory("resources/star_runScript");
    secureShellScreen(_sshConnection, newScreenSession);

    // Connect to screen to monitor
    secureShell(_sshConnection, "screen -r starSession");
}

/*
 * initializeStarHost()
 *
 * DESCRIPTION
 * Gets the results from the server
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int fetchResults(const SSH& _sshConnection, const StarJob& _starJob) {

    // Forces.csv
    bool filesFetched = true;
    secureCopy(_sshConnection, _starJob.getServerJobDirectory("Forces.csv"),
               _starJob.getClientJobDirectory(), FROM_SERVER, COPY_FILE);
    if(!fileExists(_starJob.getClientJobDirectory("Forces.csv"))){
        std::cerr << "ERROR: Unable to fetch Forces.csv from server!" << std::endl;
        filesFetched = false;
    }

    // Sim File
    if(_starJob.getSaveSimFile()){
        secureCopy(_sshConnection, _starJob.getServerJobDirectory(_starJob.getJobName() + ".sim"),
                   _starJob.getClientJobDirectory(), FROM_SERVER, COPY_FILE);
        if (!fileExists(_starJob.getClientJobDirectory(_starJob.getJobName() + ".sim"))){
            std::cerr << "ERROR: Unable to fetch Sim File from server" << std::endl;
            filesFetched = false;
        }
    }

    // Clean server only if files have been correctly fetched
    if(_starJob.getCleanServer() && filesFetched){
        secureShell(_sshConnection, "rm -r " + _starJob.getServerJobDirectory());
        std::cerr << "\nNOTE: job folder deleted from server" << std::endl;
    }

    return (filesFetched? TRUE : FALSE);
}