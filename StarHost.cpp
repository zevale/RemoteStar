#include "StarHost.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

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
    if(!hostListFile.is_open())
        throw "Cannot open file <star_hostList>";

    int countHostWord = 0;          // Counts host keywords
    int countHosts = 0;             // Counts number of hosts
    while(hostListFile >> word) {
        // Check new host
        if(word == "#HOST"){
            countHostWord = 0;
            ++countHosts;
        }
        // Check that at least one host tag has been found
        if(countHosts > 0) {
            switch(countHostWord) {
                case 0:
                    break;
                case 1:
                    if(word != "alias")
                        throw "Missing alias in file <star_hostList>";
                    break;
                case 2:
                    alias.insert(alias.end(), word);
                    break;
                case 3:
                    if(word != "address")
                        throw "Missing address in file <star_hostList>";
                    break;
                case 4:
                    address.insert(address.end(), word);
                    break;
                case 5:
                    if(word != "type")
                        throw "Missing type in file <star_hostList>";
                    break;
                case 6:
                    if(word == "localhost"){
                        hostType.insert(hostType.end(), LOCALHOST);
                    } else if (word == "remote"){
                        hostType.insert(hostType.end(), REMOTE_HOST);
                    } else {
                        throw "Missing or wrong host type in file <star_hostList>";
                    }
                    break;
                case 7:
                    if(word != "processes")
                        throw "Missing processes in file <star_hostList>";
                    break;
                case 8:
                    processes.insert(processes.end(), std::stoi(word));
                    break;
                default:
                    throw "Wrong syntax in file <star_hostList>";
            }
        }
        ++countHostWord;
    }
    nHosts = countHosts;
    // Final check
    if(hostListFile.bad())
        throw "Cannot read file <star_hostList>";

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
 * Required by iinitializeStarHost(StarHost& _starHost).
 */
void StarHost::printHostList() {
    size_t nHosts = alias.size();
    std::cout << "\n:::::::::::: HOST LIST" << std::endl;
    for (int i = 0; i < nHosts; ++i) {
        std::cout << "       Host: " << getAlias(i) << std::endl;
        std::cout << "    Address: " << getAddress(i) << std::endl;
        std::cout << "       Type: " << ((hostType[i] == LOCALHOST) ? "localhost" : "remote host") << std::endl;
        std::cout << "  Processes: " << getProcesses(i) << std::endl;
        if(nHosts > 1 && i < nHosts-1)
            std::cout << "------------ " << std::endl;
    }
}