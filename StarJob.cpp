#include "StarJob.h"

#include <fstream>
#include <iostream>

/*
 * Getters
 *
 * NOTE
 * Overloaded getters will append a _subPath to either ClientJobDirectory or ServerJobDirectory .
 * There is no need to start _subPath with "/"
 */
std::string StarJob::getJobName() const {
    return jobName;
}

std::string StarJob::getClientDirectory() const {
    return clientDirectory;
}

std::string StarJob::getClientJobDirectory() const {
    return (clientDirectory + jobName + std::string("/"));
}

std::string StarJob::getClientJobDirectory(const std::string& _subPath) const {
    return (clientDirectory + jobName + std::string("/") + _subPath);
}

std::string StarJob::getServerDirectory() const {
    return serverDirectory;
}

std::string StarJob::getServerDirectory(const std::string& _subPath) const {
    return (serverDirectory + _subPath);
}

bool StarJob::getSaveSimFile() const{
    return saveSimFile;
}

/*
 * Constructor that sets the job file path assuming  <star_jobData>
 * is in the current directory by default
 */
StarJob::StarJob(const std::string& _jobFilePath){
    jobFilePath = _jobFilePath;
}

/*
 * loadStarJob()
 *
 * DESCRIPTION
 * Loads the data for the class members from the file <star_jobData>
 * located in the folder .../StarClient/.
 *
 * RETURN
 * Void, but throws an exception if the data cannot be loaded.
 * This exception is caught by initializeStarJob(StarJob& _starJob).
 *
 * DEPENDENCIES
 * Required by initializeStarJob(StarJob& _starJob).
 */
void StarJob::loadStarJob() {
    // Holds every word read in from <star_hostList>
    std::string word;

    // Open file
    std::ifstream starJobFile(jobFilePath);

    // Check if file is open
    if(!starJobFile.is_open())
        throw "Cannot open file <star_jobData>";

    int countJobWord = 0;           // Count job keywords
    while(starJobFile >> word) {

        switch(countJobWord){
            case 0:
                if(word != "#BEGIN_STAR_JOB")
                    throw "Opening tag <#BEGIN_STAR_JOB> not found";
                break;
            case 1:
                if(word != "job_name")
                    throw "Tag <job_name> not found";
                break;
            case 2:
                jobName = word;
                break;
            case 3:
                if(word != "client_directory")
                    throw "Tag <client_directory> not found";
                break;
            case 4:
                clientDirectory = word;
                break;
            case 5:
                if(word != "server_directory")
                    throw "Tag <server_directory> not found";
                break;
            case 6:
                serverDirectory = word;
                break;
            case 7:
                if(word != "save_sim_file")
                    throw "Tag <save_sim_file> not found";
                break;
            case 8:
                if(word == "yes")
                    saveSimFile = true;
                else if(word == "no")
                    saveSimFile = false;
                else
                    throw "Usage <save_sim_file> yes/no";
                break;
            default:
                if(word != "#END_STAR_JOB")
                    throw "Closing tag <#END_STAR_JOB> not found";
                break;
        }
        ++countJobWord;
    }
}

void StarJob::printJobData(){
    std::cout << "\n:::::::::::: JOB " << getJobName() << std::endl;
    std::cout << "     Client: " << getClientDirectory() << std::endl;
    std::cout << "     Server: " << getServerDirectory() << std::endl;
}