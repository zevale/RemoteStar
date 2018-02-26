/*
 * CLASS StarJob
 *
 * DESCRIPTION
 * StarJob holds information on the job submitted to STAR CCM+
 *
 * NOTES
 *
 * DEPENDENCIES
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 */
#ifndef STARJOB_H
#define STARJOB_H

#include <string>

class StarJob {
private:
    std::string jobFilePath;
    std::string jobName;
    std::string clientDirectory;
    std::string serverDirectory;
    bool        saveSimFile;
public:
    // Constructor
    explicit StarJob(const std::string& _jobFilePath = "star_jobData");
    // Getters
    std::string getJobName()                                       const;
    std::string getClientDirectory()                               const;
    std::string getClientJobDirectory()                            const;
    std::string getClientJobDirectory(const std::string& _subPath) const;
    std::string getServerDirectory()                               const;
    std::string getServerDirectory(const std::string& _subPath)    const;
    bool        getSaveSimFile()                                   const;
    // Class methods
    void loadStarJob();
    void printJobData();
};
#endif // STARJOB_H