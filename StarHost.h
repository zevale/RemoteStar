//
// Created by nuno on 20/02/18.
//

#ifndef STARHOSTS_H
#define STARHOSTS_H


#include <vector>
#include <string>

enum HostType {
    LOCALHOST=0,
    REMOTE_HOST,
};

class StarHost {
private:
    std::vector<std::string> _alias;        // Host name
    std::vector<std::string> _address;      // Host address
    std::vector<HostType>    _hostType;     // localhost -> macro is sent to this host
    std::vector<int>         _processes;    // Number of processes
public:
    // Getters for private members
    std::string getAlias    (int _iHost);
    std::string getAddress  (int _iHost);
    std::string getHostType (int _iHost);
    int         getProcesses(int _iHost);

    // Class methods
    void loadHostList();
    void printHostList();
};
#endif //STARHOSTS_H
