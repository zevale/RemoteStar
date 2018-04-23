#include "star_client.h"
#include "exit_codes.h"
#include "MightyMacroMaker/MightyConstants.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

// Windows support
#ifdef _WIN32
#include <Windows.h>
#include <unistd.h>   // _chdir and _getcwd
#endif

// Linux and Mac support
#if defined(linux) || defined(__APPLE__)
#include <unistd.h>             // chdir and _getcwd
#include <spawn.h>              // Because of posix_spawn()
#include <sys/wait.h>           // Because of waitpid()
#include <climits>              // Because of the constant MAX_PATH
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

// Print error message in and exit
void exitNow(const std::string& _errorMessage) {
    colorText(_errorMessage, RED);
    exit(g_exitStatus);
}

// Change current directory
void changeWorkingDirectory(const std::string &_workingDirectory) {
#if _WIN32
    if(_chdir(_workingDirectory.c_str())<0)
        errorInterpreted("ERROR: cannot change directory");
#endif
#if defined(linux) || defined(__APPLE__)
    if(chdir(_workingDirectory.c_str())<0)
        errorInterpreted("ERROR: cannot change directory");
#endif
}

// Check if file exists in a directory
bool fileExists(const std::string& _filePath) {
    std::ifstream testFile(_filePath.c_str());
    return testFile.good();
}

// STAR CLIENT SPECIFIC FUNCTIONS

/*
 * executeProcess(char * _moduleName, char * _commandArgs)
 *
 * DESCRIPTION
 * Executes a Windows process using createProcessA.
 * Executes a Linux or Mac process using spawn. When connecting screen,
 * Linux and Mac clients use system().
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
#if defined(linux) || defined(__APPLE__)
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
 * strArrayToCharPtrConstPtr(std::vector<std::string> _stringArray)
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
#if defined(linux) || defined(__APPLE__)
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
 * secureShell(const SSH& _sshConnection, const std::string& _commandToExecute)
 *
 * DESCRIPTION
 * Execute SSH commands which must be concatenated like "cmd1 && cmd2 && cmd3 & ...".
 * Must use an authentication key for automatic login.
 *
 * RETURN
 * Void, but throws an exception if the command cannot be executed.
 * This exception is caught by submitJob(const SSH& _sshConnection, const StarJob& _starJob)
 *
 */
void secureShell(const SSH& _sshConnection, const std::string& _commandToExecute) {
#ifdef _WIN32
    // Get the ssh.exe module. Important: folder is Sysnative to access 64-bit System32 folder from 32-bit program
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\ssh.exe";
#endif
#if defined(linux) || defined(__APPLE__)
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
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SECURE_SHEEL);
        throw "cannot execute SSH command";
    }
#endif
#if defined(linux) || defined(__APPLE__)
    // UPDATE WITH EXCEPTION
    // Check if it is a screen command
    if(screenConnect){
        // Use system()
        system(commandArgs);
    } else {
        // Run command using executeProcess()
        if(!(executeProcess(moduleName, commandArgs))) {
            std::cerr << "ERROR: cannot execute SSH command" << std::endl;
        }
    }
#endif
}

/*
 * secureShellScreen(const SSH& _sshConnection, const std::string& _commandToExecute)
 *
 * DESCRIPTION
 * Execute screen instruction needed to start STAR CCM+ as an independent process
 * that keeps running in case of ssh connection failure.
 *
 * RETURN
 * Void, but throws an exception if the command cannot be executed.
 * This exception is caught by submitJob(const SSH& _sshConnection, const StarJob& _starJob)
 *
 */
void secureShellScreen(const SSH& _sshConnection, const std::string& _commandToExecute) {
#ifdef _WIN32
    // Get the ssh.exe module. Important: folder is Sysnative to access 64-bit System32 folder from 32-bit program
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\ssh.exe";
#endif
#if defined(linux) || defined(__APPLE__)
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
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SECURE_SHEEL_SCREEN);
        throw "cannot execute SSH screen";
    }
}

/*
 * secureCopy(const SSH& _sshConnection, const std::string& _sourceFilePath,
 *            const std::string& _destinationPath, CopyDirection _copyDirection, CopyOptions _copyOptions)
 *
 * DESCRIPTION
 * Execute SCP from client computer to SSH server. Client must use an authentication key.
 *
 * RETURN
 * Void, but throws an exception if the command cannot be executed.
 * This exception is caught by submitJob(const SSH& _sshConnection, const StarJob& _starJob)
 *
 */
void secureCopy(const SSH& _sshConnection, const std::string& _sourceFilePath,
               const std::string& _destinationPath, CopyDirection _copyDirection, CopyOptions _copyOptions) {
#ifdef _WIN32
    // Get the scp.exe module. Important: folder is Sysnative to access 64-bit System32 folder from 32-bit program
    char moduleName[] = "C:\\Windows\\Sysnative\\OpenSSH\\scp.exe";
#endif
#if defined(linux) || defined(__APPLE__)
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
        scp += " -r";
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
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SECURE_COPY);
        throw "cannot execute SCP command";
    }
}

/*
 * loadingScreen(const SSH& _sshConnection)
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
#if defined(linux) || defined(__APPLE__)
    system("clear");
#endif
    // Connecting to
    std::cout << "\n    CONNECTING TO " << _sshConnection.getServer() << std::endl;
#ifdef _WIN32
    Sleep(3000);
    system("cls");
#endif
#if defined(linux) || defined(__APPLE__)
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    system("clear");
#endif
    // Loading screen
    std::string buffer;
    std::ifstream textFile;
#ifdef _WIN32
    // Current working directory is Client Directory
    textFile.open(".\\servers\\screen1");
#endif
#if defined(linux) || defined(__APPLE__)
    textFile.open("./servers/screen1");
#endif
    if(!textFile) {
        // There is no text to output
        return;
    } else {
        while(!textFile.eof()) {
            getline(textFile, buffer);
#ifdef _WIN32
            Sleep(32);
#endif
#if defined(linux) || defined(__APPLE__)
            std::this_thread::sleep_for(std::chrono::milliseconds(32));
#endif
            std::cout << buffer << std::endl;
        }
        textFile.close();
    }
}

/*
 * initializeSSH(SSH& _sshConnection)
 *
 * DESCRIPTION
 * Loads the SSH server data from file <star_sshServer> and initializes
 * the _sshConnection
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int initializeSSH(SSH& _sshConnection) {
    // Load ssh server data and check OpenSSH installation
    try {
        _sshConnection.loadSSH();
#ifdef _WIN32
        if(!fileExists("C:\\Windows\\Sysnative\\OpenSSH\\ssh.exe")){
            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_OPEN_SSH_NOT_FOUND);
            throw "please install OpenSSH for Windows";
        }
#endif
    } catch (const char* loadSshException) {
        // Error loading ssh server data from <star_sshServer>
        colorText("\n     ERROR: " + std::string(loadSshException) + "\n", RED);
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
        // Open file in binary mode to avoid Windows adding \r on new lines
        std::ofstream shellRunScriptFile(_starJob.getClientJobDirectory("resources" +
                                      std::string(CrossPlatform::separator) + "star_runScript"), std::ios_base::binary);

        // Check if file is open
        if(!shellRunScriptFile){
            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_SHELL_SCRIPT_CANNOT_WRITE);
            throw "Cannot write shell script <star_runScript>";
        }

        // Initialize sheBang and STAR CCM+ command line arguments
        std::string sheBang = "#!/bin/sh\n";
        std::string starPath(Default::starPath);
        std::string starLicense = "-power ";
        std::string starInfiniBand = "-fabric IBV ";
        std::string starHost;
        std::string starMacro = " -batch ";
        std::string macroPath = _starJob.getServerJobDirectory("resources/MightyMacro.java");
        std::string initializationJobPath = (_starJob.getInitializationJob() == Default::initializationJob)?
                                            // No initialization file
                                            "" :
                                            // Open initialization file in original folder
                                            " " + _starJob.getServerDirectory() +
                                                    _starJob.getInitializationJob() + "/" +
                                                    _starJob.getInitializationJob() + ".sim";

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
        shellRunScriptFile.write(sheBang.c_str(), sheBang.size());
        std::string runCommand(starPath + starLicense + starHost + starMacro + macroPath + initializationJobPath);
        shellRunScriptFile.write(runCommand.c_str(), runCommand.size());
        shellRunScriptFile.close();
    } catch (const char* loadHostException) {
        // Error loading hosts from <star_hostList>
        colorText("\n      ERROR: " + std::string(loadHostException) + "\n", RED);
        return FALSE;
    }
    // Show host list
    _starHost.printHostList();

    return TRUE;
}

/*
 * initializeStarJob(StarJob& _starJob)
 *
 * DESCRIPTION
 * Loads the job data from file <star_jobData>
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int initializeStarJob(StarJob& _starJob) {
    // Load job data and check sim files exist
    try {
        // Load job
        _starJob.loadStarJob();

        // If there is no initialization, check files in "resources" folder
        if(_starJob.getInitializationJob() == Default::initializationJob){
            // Check aircraft geometry
            std::string test = _starJob.getClientJobDirectory("resources" + std::string(CrossPlatform::separator)
                                                              + "SurfMesh.stl");
            if(!fileExists(test)){
                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_AIRCRAFT_GEOMETRY_NOT_FOUND);
                throw "Aircraft geometry not found";
            }

            // Check domain geometry
            test = _starJob.getClientJobDirectory("resources" +  std::string(CrossPlatform::separator)
                                                  + "DomainGeometry.x_b");
            if(!fileExists(test)){
                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_DOMAIN_GEOMETRY_NOT_FOUND);
                throw "Domain geometry not found";
            }
        }

    } catch(const char * loadJobException) {
        // Error loading hosts from <star_hostList>
        colorText("\n      ERROR: " + std::string(loadJobException) + "\n", RED);
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
 * submitJob(const SSH& _sshConnection, const StarJob& _starJob)
 *
 * DESCRIPTION
 * Sends resources to SSH server, submits job to hosts and connects screen to SSH server
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int submitJob(const SSH& _sshConnection, const StarJob& _starJob) {

    try {
        // SSH command: create job folder
        std::string createServerFolders = "mkdir -p " + _starJob.getServerJobDirectory();
        secureShell(_sshConnection, createServerFolders);

        // SCP client resources folder
        secureCopy(_sshConnection, _starJob.getClientJobDirectory("resources" +  std::string(CrossPlatform::separator)),
                   _starJob.getServerJobDirectory(), TO_SERVER, COPY_FOLDER);

        // SSH command: set script permissions on server
        std::string setScriptPermissions =
                "cd " + _starJob.getServerJobDirectory("resources/") + " && chmod 775 star_runScript";
        secureShell(_sshConnection, setScriptPermissions);

        // SSH command: run using screen  -d -m: session in detached mode and log output
        std::string newScreenSession = "screen -dmS starSession " +
                _starJob.getServerJobDirectory("resources/star_runScript") +
                " && screen -S starSession -X logfile " + _starJob.getServerJobDirectory(_starJob.getJobName() + ".log") +
                " && screen -S starSession -X log";
        secureShellScreen(_sshConnection, newScreenSession);

        // Connect to screen to monitor
        secureShell(_sshConnection, "screen -r starSession");
    } catch(const char* submitJobException) {
        colorText("\n      ERROR: " + std::string(submitJobException) + "\n", RED);
        return FALSE;
    }
    return TRUE;
}

/*
 * fetchResults(const SSH& _sshConnection, const StarJob& _starJob)
 *
 * DESCRIPTION
 * Gets the results from the server
 *
 * RETURN
 * TRUE (1) in case of success or FALSE (0) otherwise
 */
int fetchResults(const SSH& _sshConnection, const StarJob& _starJob) {
    // Clear screen
#ifdef _WIN32
    system("cls");
#endif
#if defined(linux) || defined(__APPLE__)
    system("clear");
#endif

    bool forcesFetched = true;
    bool simFetched    = true;
    // Forces.csv
    secureCopy(_sshConnection, _starJob.getServerJobDirectory("Forces.csv"),
               _starJob.getClientJobDirectory(), FROM_SERVER, COPY_FILE);
    if(!fileExists(_starJob.getClientJobDirectory("Forces.csv"))){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_RESULTS_FORCES_UNABLE_TO_FETCH);
        colorText("      ERROR: Unable to fetch Forces.csv from server!", RED);
        forcesFetched = false;
    }

    // Sim File
    if(_starJob.getSaveSimFile()){
        secureCopy(_sshConnection, _starJob.getServerJobDirectory(_starJob.getJobName() + ".sim"),
                   _starJob.getClientJobDirectory(), FROM_SERVER, COPY_FILE);
        if (!fileExists(_starJob.getClientJobDirectory(_starJob.getJobName() + ".sim"))){
            colorText("      ERROR: Unable to fetch sim File from server", RED);
            simFetched = false;
            if(forcesFetched)
                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_RESULTS_SIM_UNABLE_TO_FETCH);
            else
                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_RESULTS_FORCES_SIM_UNABLE_TO_FETCH);
        }
    }

    // Log file
    secureCopy(_sshConnection, _starJob.getServerJobDirectory(_starJob.getJobName() + ".log"),
               _starJob.getClientJobDirectory(), FROM_SERVER, COPY_FILE);
    if(!fileExists(_starJob.getClientJobDirectory(_starJob.getJobName() + ".log"))){
        colorText("    WARNING: Unable to fetch log file from server!\n", YELLOW);
    }

    // Clean server only if files have been correctly fetched
    if(_starJob.getCleanServer() && forcesFetched && simFetched){
        secureShell(_sshConnection, "rm -r " + _starJob.getServerJobDirectory());
        colorText("\nNOTE: job folder deleted from server\n\n", YELLOW);
    }

    return ((forcesFetched && simFetched)? TRUE : FALSE);
}

/*
 * colorText(const std::string& _text, Color _color)
 *
 * DESCRIPTION
 * Prints colored text to the console
 */
void colorText(const std::string& _text, Color _color){
#ifdef _WIN32
    // Get console handle
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // color your text in Windows console mode
    // colors are 0=black 1=blue 2=green and so on to 15=white
    // colorAttribute = foreground + background * 16
    // to get red text on yellow use 4 + 14*16 = 228
    // light red on yellow would be 12 + 14*16 = 236

    int consoleAttribute;

    switch(_color){
        case RED:
            consoleAttribute = 0xC + 0*0xF;
            break;
        case GREEN:
            consoleAttribute = 0x2 + 0*0xF;
            break;
        case BLUE:
            consoleAttribute = 0x9 + 0*0xF;
            break;
        case AQUA:
            consoleAttribute = 0x3 + 0*0xF;
            break;
        case YELLOW:
            consoleAttribute = 0xE + 0*0xF;
            break;
        case WHITE_BLUE:
            consoleAttribute = 0xF + 9*0xF;
            break;
    }

    FlushConsoleInputBuffer(hConsole);
    SetConsoleTextAttribute(hConsole, consoleAttribute);

    // Print colored text
    std::cout << _text;

    // Restore default white text and black background
    SetConsoleTextAttribute(hConsole, 15);
#endif
#if defined(linux) || defined(__APPLE__)
    // UNTESTED CODE

    std::string escapeSequence;

    // Color escape sequences
    // ESC[ = Control Sequence Introducer
    char normal[] = { 0x1b, '[', '0', 'm', 0 };

    switch(_color){
        case RED:
            escapeSequence = { 0x1b, '[', '9', '1' , ';', '4', '0', 'm', 0 };
            break;
        case GREEN:
            escapeSequence = { 0x1b, '[', '3', '2' , ';', '4', '0', 'm', 0 };
            break;
        case BLUE:
            escapeSequence = { 0x1b, '[', '3', '4' , ';', '4', '0', 'm', 0 };
            break;
        case AQUA:
            escapeSequence = { 0x1b, '[', '9', '6' , ';', '4', '0', 'm', 0 };
            break;
        case YELLOW:
            escapeSequence = { 0x1b, '[', '9', '3' , ';', '4', '0', 'm', 0 };
            break;
        case WHITE_BLUE:
            escapeSequence = { 0x1b, '[', '3', '7' , ';', '1', '0', '4', 'm', 0 };
            break;
    }

    // Print colored text
    std::cout << escapeSequence << _text << normal;
#endif
}