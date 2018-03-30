#include "SSH.h"
#include "star_client.h"
#include "exit_codes.h"

#include <iostream>
#include <fstream>
#include <vector>

/*
 * GETTERS
 */
std::string SSH::getAlias() const {
    return alias;
}

std::string SSH::getUser() const {
    return userSSH;
}

std::string SSH::getServer() const {
    return serverSSH;
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
    if(!sshServerFile.is_open()){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SERVER_CANNOT_OPEN);
        throw "Cannot load file <star_sshServer>";
    }

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
                    if(word != "alias"){
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SERVER_ALIAS_MISSING);
                        throw "missing alias in file <star_sshServer>";
                    }
                    break;
                case 2:
                    alias = word;
                    break;
                case 3:
                    if(word != "address"){
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SERVER_ADDRESS_MISSING);
                        throw "missing address in file <star_sshServer>";
                    }
                    break;
                case 4:
                    serverSSH = word;
                    break;
                case 5:
                    if(word != "user"){
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SERVER_USER_MISSING);
                        throw "missing user in file <star_sshServer>";
                    }
                case 6:
                    userSSH = word;
                    break;
                default:
                    g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SERVER_USAGE);
                    throw "wrong syntax in file <star_sshServer>";
            }
        } else {
            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SERVER_NO_SERVER);
            throw "There should be one ssh server in file <star_sshStar>";
        }
        ++countWord;
    }

    // Check for server
    if(countServers != 1){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SERVER_NO_SERVER);
        throw "There should be one ssh server in file <star_sshStar>";
    }

    // User is the sixth word but countWord gets incremented before exiting the loop
    if(countWord == 7)
        return;
    else if(countWord == 1 || countWord == 2) {
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SERVER_ALIAS_MISSING);
        throw "missing alias in file <star_sshServer>";
    } else if (countWord == 3 || countWord == 4) {
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SERVER_ADDRESS_MISSING);
        throw "missing address in file <star_sshServer>";
    } else if (countWord == 5 || countWord == 6) {
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SERVER_USER_MISSING);
        throw "no user in file <star_sshServer>";
    } else if (countWord > 7) {
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_SSH_SERVER_USAGE);
        throw "wrong syntax in file <star_sshServer>";
    }
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
    std::cout << "       User: " << userSSH << std::endl;
    std::cout << "     Server: " << serverSSH << std::endl;
}