#include "StarJob.h"

#include <fstream>
#include <iostream>

/*
 * Getters
 */

std::string StarJob::getJobName() const {
    return jobName;
}

std::string StarJob::getClientDirectory() const {
    return clientDirectory;
}

std::string StarJob::getClientDirectory(const std::string& _subPath) const {
    return (clientDirectory + _subPath);
}

std::string StarJob::getServerDirectory() const {
    return serverDirectory;
}

std::string StarJob::getServerDirectory(const std::string& _subPath) const {
    return (serverDirectory + _subPath);
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