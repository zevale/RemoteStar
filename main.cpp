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

int main(int argc, char * argv[])
{
    // Connection to localhost: initialize SSH and print connection data
    SSH sshConnection;
    sshConnection.printConnectionData();

    // STAR CCM+ hosts: initialize StarHost
    StarHost starHost;
    initializeStarHost(starHost);

    // Set paths to source files and path to destination folder
#ifdef _WIN32
    char runStarSource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\runStar";
    char macroSource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\MacroClean.java";
    char domainGeometrySource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\DomainGeometry.x_b";
    char aircraftGeometrySource[] = "C:\\Users\\Nuno\\Desktop\\RemoteStar\\SurfMesh.stl";
#endif
#ifdef linux
    char runStarSource[] = "/home/nuno/Desktop/RemoteStar/run_star";
    char macroSource[] = "/home/nuno/Desktop/RemoteStar/MacroClean.java";
    char domainGeometrySource[] = "/home/nuno/Desktop/RemoteStar/DomainGeometry.x_b";
    char aircraftGeometrySource[] = "/home/nuno/Desktop/RemoteStar/SurfMesh.stl";
#endif
    char serverDestination[] = "/home/nuno/Desktop/RemoteStar";

    // SSH command: create RemoteStar folder
    secureShell(sshConnection, (char *) "cd ~/Desktop && mkdir RemoteStar");

    // SCP file - shell script runStar
    secureCopy(sshConnection, runStarSource, serverDestination);

    // SCP file - macro MacroClean.java
    secureCopy(sshConnection, macroSource, serverDestination);

    // SCP file - domain geometry
    secureCopy(sshConnection, domainGeometrySource, serverDestination);

    // SCP file - aircraft geometry SurfMesh.stl
    secureCopy(sshConnection, aircraftGeometrySource, serverDestination);

    // SSH command: set script permissions
    secureShell(sshConnection, (char *) "cd /home/nuno/Desktop/RemoteStar && chmod 775 run_star");

    // SSH command: run using screen  -d -m means new screen session in detached mode
    secureShellScreen(sshConnection, (char *) "screen -S starSession -d -m /home/nuno/Desktop/RemoteStar/run_star");

    // Connect to screen to monitor
    secureShell(sshConnection, (char *) "screen -r starSession");

    /* secureShell and secureCopy output eventual errors to cerr. These errors must be handled so that the simulation
     * does not stop.
     */

    return EXIT_SUCCESS;
}