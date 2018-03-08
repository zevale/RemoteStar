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
#include <cstring>
#include "star_client.h"
#include "MightyMacroMaker/MightyMacro.h"

int main(int argc, char * argv[]) {
    bool    batchModeOption = false;
    std::string jobFilePath = "C:\\Users\\Nuno\\Dev\\RemoteStar\\star_jobData";
    // Check command line arguments
    switch(argc){
        case 1:
            // Default settings: batchMode (false), star_jobData in current working directory
            break;
        case 2:
            if(!(strcmp(argv[1], "-batch"))){
                // User provided -batch
                batchModeOption = true;
            } else {
                // User provided a file path to star_jobData
                jobFilePath = std::string(argv[1]);
            }
            break;
        case 3:
            // User provided an option and a file path to star_jobData
            if(!(strcmp(argv[1], "-batch"))){
                batchModeOption = true;
                jobFilePath = std::string(argv[2]);
                break;
            }
        default:
            std::cout << ":::::::::::: RemoteStar\n";
            std::cout << "Usage [job file path]\n";
            std::cout << "      [-batch batch mode on] [job file path]" << std::endl;
            return EXIT_FAILURE;
    }

    // Initialize starJob: get all relevant data for the sim
    StarJob starJob(jobFilePath, batchModeOption);
    if(!initializeStarJob(starJob))
        exitNow("TERMINATING: cannot load job data");

    // Initialize mighty macro using job data and write macro
    MightyMacro mightyMacro(&starJob);
    mightyMacro.writeMacro();


    // Connection to ssh server: initialize sshConnection
    SSH sshConnection;
    if(!initializeSSH(sshConnection))
        exitNow("TERMINATING: SSH connection cannot be established");

    // STAR CCM+ hosts: initialize starHost and write <star_runScript>
    StarHost starHost(batchModeOption);
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