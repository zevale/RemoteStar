#include "SSH.h"

#include <iostream>
#include <fstream>
#include <vector>

/*
 * GETTERS
 */
std::string SSH::getAlias() {
    return _alias;
}

char * SSH::getUser() {
    return &_userSSH[0u];
}

char* SSH::getServer() {
    return &_serverSSH[0u];
}

/*
 * loadSSH()
 *
 * DESCRIPTION
 * Loads the data for the class members from the file <star_sshServer>
 * located in the folder .../StarClient/servers.
 *
 * RETURN
 * Void, but throws an exception if the data cannot be loaded.
 * This exception is caught by initializeSSH(SSH& _sshConnection).
 *
 * DEPENDENCIES
 * Required by initializeSSH(SSH& _sshConnection).
 */
void SSH::loadSSH() {
    // Holds every word read in from <star_hostList>
    std::string word;

    // Open file
    std::ifstream sshServerFile("./servers/star_sshServer");

    // Check if file is open
    if(!sshServerFile.is_open())
        throw "Cannot load file <star_sshServer>";

    int countWord = 0;          // Counts ssh server keywords
    int countServers = 0;       // Counts ssh servers - should be just one
    while(sshServerFile >> word){
        if (word == "#SSH_SERVER"){
            ++countServers;
        }
        // Check there is only one server
        if(countServers == 1) {
            switch (countWord) {
                case 0:
                    break;
                case 1:
                    if(word != "alias")
                        throw "Missing alias in file <star_sshServer>";
                    break;
                case 2:
                    _alias = word;
                    break;
                case 3:
                    if(word != "address")
                        throw "Missing address in file <star_sshServer>";
                    break;
                case 4:
                    _serverSSH = word;
                    break;
                case 5:
                    if(word != "user")
                        throw "Missing user in file <star_sshServer>";
                case 6:
                    _userSSH = word;
                    break;
                default:
                    throw "Wrong syntax in file <star_sshServer>";
            }
        } else {
            throw "There should be one ssh server in file <star_sshStar>";
        }
        ++countWord;
    }
    // User is the sixth word but countWord gets incremented before exiting the loop
    if(countWord != 7)
        throw "No user in file <star_sshServer>";
}

/*
 * printConnectionData()
 *
 * DESCRIPTION
 * Prints the connection data in case the user wants to double check
 *
 * DEPENDENCIES
 * Required by initializeSSH(SSH& _sshConnection).
 */
void SSH::printConnectionData() {
    std::cout << "\n:::::::::::: SSH CONNECTION to " << getAlias() << std::endl;
    std::cout << "       USER: " << _userSSH << std::endl;
    std::cout << "     SERVER: " << _serverSSH << std::endl;
}