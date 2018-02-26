/*
 * CLASS StarHost
 *
 * DESCRIPTION
 * Holds the data on the hosts that will be used in parallel.
 * One and only one of the hosts must be of HostType LOCALHOST.
 * The localhost will be the machine running the ssh server and
 * must be invoked as such when lauching STAR CCM+.
 *
 * DEPENDENCIES
 * Required by star_client library.
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 *
 */
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
    std::vector<std::string> alias;        // Host name
    std::vector<std::string> address;      // Host address
    std::vector<HostType>    hostType;     // localhost -> macro is sent to this host
    std::vector<int>         processes;    // Number of processes
    int                      nHosts;       // Number of hosts
public:
    // Getters for private members
    std::string getAlias    (int _iHost) const;
    std::string getAddress  (int _iHost) const;
    HostType    getHostType (int _iHost) const;
    int         getProcesses(int _iHost) const;
    int         getNumHosts ()           const;

    // Class methods
    void loadHostList();
    void printHostList();
};
#endif //STARHOSTS_H