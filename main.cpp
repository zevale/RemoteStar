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
    // Ask for directory
    changeDirectory("/home/nuno/Desktop/RemoteStar");
    std::cout << "Working in: " << getDirectory() << std::endl;


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
    char runStarSource[] = "./sim/star_runScript";
    char macroSource[] = "./sim/MacroClean.java";
    char domainGeometrySource[] = "./sim/DomainGeometry.x_b";
    char aircraftGeometrySource[] = "./sim/SurfMesh.stl";
#endif
    char serverDestination[] = "/home/nuno/Desktop/RemoteStar/sim";

    // SSH command: create RemoteStar folders
    secureShell(sshConnection, (char *) "cd ~/Desktop && mkdir -p RemoteStar/sim && mkdir -p RemoteStar/results");

    // SCP file - shell script runStar
    secureCopy(sshConnection, runStarSource, serverDestination, TO_SERVER);

    // SCP file - macro MacroClean.java
    secureCopy(sshConnection, macroSource, serverDestination, TO_SERVER);

    // SCP file - domain geometry
    secureCopy(sshConnection, domainGeometrySource, serverDestination, TO_SERVER);

    // SCP file - aircraft geometry SurfMesh.stl
    // secureCopy(sshConnection, aircraftGeometrySource, serverDestination, TO_SERVER);

    // SSH command: set script permissions
    secureShell(sshConnection,
                (char *) "cd /home/nuno/Desktop/RemoteStar/sim && chmod 775 star_runScript");

    // SSH command: run using screen  -d -m means new screen session in detached mode
    secureShellScreen(sshConnection,
                      (char *) "screen -S starSession -d -m /home/nuno/Desktop/RemoteStar/sim/star_runScript");

    // Connect to screen to monitor
    secureShell(sshConnection, (char *) "screen -r starSession");

    // Get results from server
    secureCopy(sshConnection, (char *) "/home/nuno/Desktop/RemoteStar/results/Forces.csv",
               (char *) "/home/nuno/Desktop/RemoteStar/results/", FROM_SERVER);
    if(fileExists("/home/nuno/Desktop/RemoteStar/results/Forces.csv"))
        std::cout << "\n FETCHED RESULTS FROM SERVER!" << std::endl;
    else
        std::cout << "\n UNABLE TO FETCH RESULTS FROM SERVER!" << std::endl;

    /* secureShell and secureCopy output eventual errors to cerr. These errors must be handled so that the simulation
     * does not stop.
     */

    return EXIT_SUCCESS;
}