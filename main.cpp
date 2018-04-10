/*
 * RemoteStar
 *
 * Client for STAR-CCM+ simulations using the MightyMacro framework.
 *
 * The program will read a job file with the instructions for the
 * simulation setup and will then generate the appropriate macro.
 *
 * It will also copy the simulation resources on the client machine
 * to the server and launch the STAR-CCM+ simulation.
 *
 * Once the simulation is completed, the results are copied to the
 * client machine.
 *
 * Files are copied and commands are sent using SSH protocol.
 * This requires using Microsoft's native OpenSSH client on
 * Windows machines or the default ssh clients on Mac and Linux
 * machines.
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 */

#include "star_client.h"
#include "exit_codes.h"
#include "MightyMacroMaker/MightyMacro.h"

#include <iostream>
#include <cstring>

// Program status must be declared as global variable "g_exitStatus"
int g_exitStatus = static_cast<int>(ExitCodes::SUCCESS);

int main(int argc, char * argv[]) {

    // Handle command line arguments
    bool        batchModeOption = false; // Batch mode is off by default
    std::string jobFilePath     = "C:\\Users\\Nuno\\Dev\\RemoteStar\\star_jobData"; // "/Users/Nuno/Dev/RemoteStar/star_jobData";
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
            // Check option is "-batch"
            if(!(strcmp(argv[1], "-batch"))){
                batchModeOption = true;
                jobFilePath = std::string(argv[2]);
                break;
            }
            // Wrong command line usage
        default:
            std::cout << ":::::::::::: RemoteStar\n";
            std::cout << "Usage [job file path]\n";
            std::cout << "      [-batch (batch mode on)] [job file path]" << std::endl;
            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_COMMAND_LINE_USAGE);
            exit(g_exitStatus);
    }

    /*
     *  RemoteStar client
     */

    // Initialize starJob: get all relevant simulation data from the job file
    StarJob starJob(batchModeOption, jobFilePath);
    if(!initializeStarJob(starJob))
        exitNow("TERMINATING: cannot load job data\n");

    // Initialize mighty macro framework using job data and write macro
    MightyMacro mightyMacro(&starJob);
    mightyMacro.writeMacro();

    // Connection to ssh server: initialize sshConnection
    SSH sshConnection;
    if(!initializeSSH(sshConnection))
        exitNow("TERMINATING: SSH connection cannot be established\n");

    // STAR CCM+ hosts: initialize starHost and write <star_runScript>
    StarHost starHost(batchModeOption);
    if(!initializeStarHost(starHost, starJob))
        exitNow("TERMINATING: cannot initialize hosts");

    // Submit job to the server, copy files and launch simulation
    if(!submitJob(sshConnection, starJob))
        exitNow("TERMINATING: cannot submit job");

    // Fetch results
    if(!fetchResults(sshConnection, starJob))
        exitNow("\nTERMINATING: error(s) while fetching results\n");
    else
        colorText("\nFETCHED RESULTS FROM SERVER!\n", GREEN);

    return g_exitStatus;
}
