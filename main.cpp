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

int main(int argc, char * argv[]) {
    // Initialize starJob: get all relevant data for the sim
    std::string jobFilePath =
            ((argc > 1)? std::string(argv[1]) :  std::string("/home/nuno/Dev/RemoteStar/star_jobData"));
    StarJob starJob(jobFilePath);
    if(!initializeStarJob(starJob))
        exitNow("TERMINATING: cannot load job data");

    // Connection to localhost: initialize sshConnection
    SSH sshConnection;
    if(!initializeSSH(sshConnection))
        exitNow("TERMINATING: SSH connection cannot be established");

    // STAR CCM+ hosts: initialize starHost
    StarHost starHost;
    if(!initializeStarHost(starHost))
        exitNow("TERMINATING: cannot initialize hosts");

    // Set paths to source files and path to destination folder
#ifdef _WIN32
    char runStarSource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\star_runScript";
    char macroSource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\MacroClean.java";
    char domainGeometrySource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\DomainGeometry.x_b";
    char aircraftGeometrySource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\SurfMesh.stl";
#endif
#ifdef linux
    std::string runStarSource = starJob.getClientDirectory("/sim/star_runScript");
    std::string macroSource = starJob.getClientDirectory("/sim/MacroClean.java");
    std::string domainGeometrySource = starJob.getClientDirectory("/sim/DomainGeometry.x_b");
    std::string aircraftGeometrySource = starJob.getClientDirectory("/sim/SurfMesh.stl");
#endif
    std::string serverDestination = starJob.getServerDirectory("/RemoteStar/sim");

    // SSH command: create RemoteStar folders
    std::string createServerFolders = std::string("cd ") + starJob.getServerDirectory() +
            std::string(" && mkdir -p RemoteStar/sim && mkdir -p RemoteStar/results");
    secureShell(sshConnection, createServerFolders);

    // SCP file - shell script runStar
    secureCopy(sshConnection, runStarSource, serverDestination, TO_SERVER);

    // SCP file - macro MacroClean.java
    secureCopy(sshConnection, macroSource, serverDestination, TO_SERVER);

    // SCP file - domain geometry
    secureCopy(sshConnection, domainGeometrySource, serverDestination, TO_SERVER);

    // SCP file - aircraft geometry SurfMesh.stl
    secureCopy(sshConnection, aircraftGeometrySource, serverDestination, TO_SERVER);

    // SSH command: set script permissions
    std::string setScriptPermissions = std::string("cd ") + starJob.getServerDirectory() +
            std::string("/RemoteStar/sim ") + std::string("&& chmod 775 star_runScript");
    secureShell(sshConnection, setScriptPermissions);

    // SSH command: run using screen  -d -m means new screen session in detached mode
    std::string newScreenSession = std::string("screen -S starSession -d -m ") +
            starJob.getServerDirectory() + std::string("/RemoteStar/sim/star_runScript");
    secureShellScreen(sshConnection, newScreenSession);

    // Connect to screen to monitor
    secureShell(sshConnection, "screen -r starSession");

    // Get results from server
    std::string serverResults = starJob.getServerDirectory("/RemoteStar/results/Forces.csv");
    std::string clientResults = starJob.getClientDirectory("/results/");
    secureCopy(sshConnection, serverResults, clientResults, FROM_SERVER);

    // Check if scp was successful
    if(fileExists(starJob.getClientDirectory("/results/Forces.csv")))
        std::cout << "\n FETCHED RESULTS FROM SERVER!" << std::endl;
    else
        std::cout << "\n UNABLE TO FETCH RESULTS FROM SERVER!" << std::endl;

    /* secureShell and secureCopy output eventual errors to cerr. These errors must be handled so that the simulation
     * does not stop.
     */

    return EXIT_SUCCESS;
}