#include "StarHost.h"
#include "MightyMacroMaker/MightyConstants.h"
#include "star_client.h"
#include "exit_codes.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#endif

/*
 * Constructor
 */
StarHost::StarHost(bool _batchModeOption) : batchModeOption(_batchModeOption) {}

/*
 * GETTERS
 */
std::string StarHost::getAlias(int _iHost) const {
    return alias[_iHost];
}

std::string StarHost::getAddress(int _iHost) const {
    return address[_iHost];
}

HostType StarHost::getHostType(int _iHost) const {
    return hostType[_iHost];
}

int StarHost::getProcesses(int _iHost) const {
    return processes[_iHost];
}

int StarHost::getNumHosts() const {
    return nHosts;
}

/*
 * loadHostList()
 *
 * DESCRIPTION
 * Loads the data for the class members from the file <star_hostList>
 * located in the folder .../StarClient/servers.
 *
 * RETURN
 * Void, but throws an exception if the data cannot be loaded.
 * This exception is caught by initializeStarHost(StarHost& _starHost).
 *
 * DEPENDENCIES
 * Required by initializeStarHost(StarHost& _starHost).
 */
void StarHost::loadHostList() {
    // Holds every word read in from <star_hostList>
    std::string word;

    // Open file
    std::ifstream hostListFile("./servers/star_hostList");

    // Check if file is open
    if(!hostListFile.is_open()){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_HOST_LIST_CANNOT_OPEN);
        throw "Cannot open file <star_hostList>";
    }

    // Read <star_hostList> status
    int numLocalhost = 0;

    // Read <star_hostList>
    int countHostWord = 0;          // Counts host keywords
    int countHosts = 0;             // Counts number of hosts
    while(hostListFile >> word) {
        // Check new host
        if(word == "#HOST"){
            // First host is always added
            if(countHosts == 0)
                ++countHosts;
            // Check previous host data (8 fields but countHostWord is 9 after the while)
            else if(countHostWord == 9)
                ++countHosts;
            else if(countHostWord == 1 || countHostWord == 2){
                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_ALIAS_MISSING);
                throw "Missing alias in file <star_hostList>";
            } else if(countHostWord == 3 || countHostWord == 4) {
                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_ADDRESS_MISSING);
                throw "Missing address in file <star_hostList>";
            } else if(countHostWord == 5 || countHostWord == 6) {
                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_TYPE_MISSING);
                throw "Missing type in file <star_hostList>";
            } else if(countHostWord == 7 || countHostWord == 8) {
                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_PROCESSES_MISSING);
                throw "Missing processes in file <star_hostList>";
            }

            // Reset host words
            countHostWord = 0;
        }
        // Check that at least one host tag has been found
        if(countHosts > 0) {
            switch(countHostWord) {
                case 0:
                    break;
                case 1:
                    if(word != "alias"){
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_ALIAS_MISSING);
                        throw "Missing alias in file <star_hostList>";
                    }
                    break;
                case 2:
                    alias.insert(alias.end(), word);
                    break;
                case 3:
                    if(word != "address") {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_ADDRESS_MISSING);
                        throw "Missing address in file <star_hostList>";
                    }
                    break;
                case 4:
                    address.insert(address.end(), word);
                    break;
                case 5:
                    if(word != "type"){
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_TYPE_MISSING);
                        throw "Missing type in file <star_hostList>";
                    }
                    break;
                case 6:
                    if(word == "localhost"){
                        if(numLocalhost == 0){
                            hostType.insert(hostType.end(), LOCALHOST);
                            ++numLocalhost;
                        } else {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_LOCALHOST_NOT_UNIQUE);
                            throw "there is more than one localhost in file <star_hostList>";
                        }
                    } else if (word == "remote"){
                        hostType.insert(hostType.end(), REMOTE_HOST);
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_TYPE_MISSING_USAGE);
                        throw "host type in file <star_hostList> must be localhost or remote";
                    }
                    break;
                case 7:
                    if(word != "processes"){
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_PROCESSES_MISSING);
                        throw "Missing processes in file <star_hostList>";
                    }
                    break;
                case 8:
                    // Check stoi validity
                    try {
                        processes.insert(processes.end(), std::stoi(word));

                        // If stoi successful, processes >= 1
                        if (processes[processes.size()-1] < 1) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_PROCESSES_INPUT);
                            throw "invalid argument for processes in file <star_hostList> (int >= 1)";
                        }
                    } catch (const std::invalid_argument &invalidArgument) {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_PROCESSES_INPUT);
                        throw "invalid argument for processes in file <star_hostList> (int >= 1)";
                    }
                    break;
                default:
                    g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_USAGE);
                    throw "Wrong syntax in file <star_hostList>";
            }
        }
        ++countHostWord;
    }

    // Check for hosts
    if(countHosts == 0){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_NO_HOST);
        throw "missing host in file <star_hostList>";
    }

    // Check one localhost
    if(numLocalhost == 0){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_NO_LOCALHOST);
        throw "no localhost in file <star_hostList>";
    }

    // Check last host data (8 fields but countHostWord is 9 after the while)
    if(countHostWord == 1 || countHostWord == 2){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_ALIAS_MISSING);
        throw "Missing alias in file <star_hostList>";
    } else if(countHostWord == 3 || countHostWord == 4) {
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_ADDRESS_MISSING);
        throw "Missing address in file <star_hostList>";
    } else if(countHostWord == 5 || countHostWord == 6) {
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_TYPE_MISSING);
        throw "Missing type in file <star_hostList>";
    } else if(countHostWord == 7 || countHostWord == 8) {
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_STAR_HOST_PROCESSES_MISSING);
        throw "Missing processes in file <star_hostList>";
    }

    // Final check
    if(hostListFile.bad())
        throw "Cannot read file <star_hostList>";

    // Number of hosts
    nHosts = countHosts;

    // Close file
    hostListFile.close();
}

/*
 * printHostList()
 *
 * DESCRIPTION
 * Prints the host list in case the user wants to double check
 *
 * DEPENDENCIES
 * Required by initializeStarHost(StarHost& _starHost).
 */
void StarHost::printHostList() {
    size_t nHosts = alias.size();
    std::cout << "\n:::::::::::: HOST LIST" << std::endl;
    for (int i = 0; i < nHosts; ++i) {
        std::cout << "       Host: " << alias[i] << std::endl;
        std::cout << "    Address: " << address[i] << std::endl;
        std::cout << "       Type: " << ((hostType[i] == LOCALHOST) ? "localhost" : "remote host") << std::endl;
        std::cout << "  Processes: " << processes[i] << std::endl;
        if(nHosts > 1 && i < nHosts-1)
            std::cout << "------------ " << std::endl;
    }

    // If batch mode, just wait Default::pauseTime, otherwise expect user input
    if(!batchModeOption){
        std::cout << "\n Press "; colorText("<enter>", GREEN); std::cout << " to continue..." << std::endl;
        std::cin.get();
    } else {
#ifdef _WIN32
        Sleep(Default::pauseTime);
//        system("cls");
#endif
#if defined(linux) || defined(__APPLE__)
//        system("clear");
#endif
    }
}