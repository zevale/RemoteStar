/*
 * RemoteStar
 *
 * This program copies the files needed to run a STAR CCM+ simulation
 * on a remote server and launches the simulation.
 *
 * Files are copied with scp and commands are sent with ssh.
 * This requires using Microsoft's native OpenSSH client on
 * windows machines.
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 */

#include <iostream>
#include "star_client.h"
#include "MightyMacroMaker/MightyMacro.h"

int main(int argc, char * argv[]) {
    // Assume there is a <star_jobData> file on the CWDirectory by default
    std::string jobFilePath =
            ((argc > 1)? std::string(argv[1]) :  std::string("/Users/Nuno/Dev/RemoteStar/star_jobData"));

    // Initialize starJob: get all relevant data for the sim
    StarJob starJob(jobFilePath);
    if(!initializeStarJob(starJob))
        exitNow("TERMINATING: cannot load job data");

    // Write Mighty Macro
    MightyMacro mightyMacro(&starJob);
    mightyMacro.writeMacro();


    // Connection to ssh server: initialize sshConnection
    SSH sshConnection;
    if(!initializeSSH(sshConnection))
        exitNow("TERMINATING: SSH connection cannot be established");

    // STAR CCM+ hosts: initialize starHost and write <star_runScript>
    StarHost starHost;
    if(!initializeStarHost(starHost, starJob))
        exitNow("TERMINATING: cannot initialize hosts");

    // Submit job
    submitJob(sshConnection, starJob);

    // Fetch results
    if(!fetchResults(sshConnection, starJob))
        exitNow("\nTERMINATING: error(s) while fetching results");
    else
        std::cout << ("\nFETCHED RESULTS FROM SERVER!") << std::endl;

    return EXIT_SUCCESS;
}