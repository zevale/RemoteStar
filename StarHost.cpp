//
// Created by nuno on 20/02/18.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "StarHost.h"

std::string StarHost::getAlias(int _iHost)
{
    return _alias[_iHost];
}

std::string StarHost::getAddress(int _iHost)
{
    return _address[_iHost];
}

HostType StarHost::getHostType(int _iHost)
{
    return _hostType[_iHost];
}

int StarHost::getProcesses(int _iHost)
{
    return _processes[_iHost];
}

int StarHost::getNumHosts()
{
    return _nHosts;
}

void StarHost::loadHostList()
{
    std::string word;         // Holds every word read in from host_list
    std::string line;         // Temporary string


    // Open file
    std::ifstream hostListFile("/home/nuno/Dev/RemoteStar/host_list");

    // Check if file is open
    if(!hostListFile.is_open())
        throw "Cannot open file <host_list>";

    int countHostWord = 0;          // Counts host keywords
    int countHosts = 0;             // Counts number of hosts
    while(hostListFile >> word) {
        // Check new host
        if(word == "#HOST"){
            countHostWord = 0;
            ++countHosts;
        }
        switch(countHostWord)
        {
            case 0:
                break;
            case 1:
                if(word != "alias")
                    throw "Missing alias in file <host_list>";
                break;
            case 2:
                _alias.insert(_alias.end(), word);
                break;
            case 3:
                if(word != "address")
                    throw "Missing address in file <host_list>";
                break;
            case 4:
                _address.insert(_address.end(), word);
                break;
            case 5:
                if(word != "type")
                    throw "Missing type in file <host_list>";
                break;
            case 6:
                if(word == "localhost"){
                    _hostType.insert(_hostType.end(), LOCALHOST);
                } else if (word == "remote"){
                    _hostType.insert(_hostType.end(), REMOTE_HOST);
                } else {
                    throw "Missing or wrong host type in file <host_list>";
                }
                break;
            case 7:
                if(word != "processes")
                    throw "Missing processes in file <host_list>";
                break;
            case 8:
                _processes.insert(_processes.end(), std::stoi(word));
                break;
            default:
                throw "Wrong syntax in file <host_list>";
        }
        ++countHostWord;
    }
    _nHosts = countHosts;
    if(hostListFile.bad())
        throw "Cannot read file";

    // Close file
    hostListFile.close();
}

void StarHost::printHostList()
{
    size_t nHosts = _alias.size();
    std::cout << "\n:::::::::::: HOST LIST" << std::endl;
    for (int i = 0; i < nHosts; ++i) {
        std::cout << "       Host: " << getAlias(i) << std::endl;
        std::cout << "    Address: " << getAddress(i) << std::endl;
        std::cout << "       Type: " << ((_hostType[i] == LOCALHOST) ? "localhost" : "remote host") << std::endl;
        std::cout << "  Processes: " << getProcesses(i) << std::endl;
        if(nHosts > 1 && i < nHosts-1)
            std::cout << "------------ " << std::endl;
    }
}