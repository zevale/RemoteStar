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
#include <vector>

constexpr int largeColumn  = 32;
constexpr int mediumColumn = 20;
constexpr int smallColumn  = 15;

class StarJob {
private:
    // Job setup
    bool                     batchModeOption;
    std::string              jobFilePath;
    std::string              jobName;
    std::string              clientDirectory;
    std::string              serverDirectory;
    bool                     saveSimFile;
    bool                     cleanServer;
    bool                     autoSaveSimulation;
    int                      numAutoSaveFiles;
    int                      iterationInterval;

    // Regions
    std::vector<std::string> regionName;
    std::vector<std::string> boundaryCondition;

    // Mesh model
    double                   baseSize;
    int                      prismLayers;
    double                   prismLayerThickness;
    double                   nearWallThickness;
    std::vector<std::string> surfaceName;
    std::vector<double>      surfaceSize;

    // Physics model
    double                   machNumber;
    double                   dynamicViscosity;
    double                   staticTemperature;
    double                   referencePressure;
    double                   referenceVelocity;
    double                   referenceDensity;
    double                   flowDirectionX;
    double                   flowDirectionY;
    double                   flowDirectionZ;

    // Solver options
    bool                     customSolverOptions;
    double                   CFL;

    // Stopping criteria
    int                      maxSteps;
    int                      numSamples;
    double                   asymptoticCL;

private:
    void printTwoColumns(std::string _c1, std::string _c2) const;
    void printTwoColumns(std::string _c1, std::string _c2, char _fill) const;
    void printThreeColumns(std::string _c1, std::string _c2, std::string _c3) const;

public:
    // Constructor
   explicit StarJob(const std::string& _jobFilePath = "star_jobData", bool _batchMode = false);

    // Getters
    std::string              getJobName()                                       const;
    std::string              getClientDirectory()                               const;
    std::string              getClientJobDirectory()                            const;
    std::string              getClientJobDirectory(const std::string& _subPath) const;
    std::string              getServerDirectory()                               const;
    std::string              getServerJobDirectory()                            const;
    std::string              getServerJobDirectory(const std::string &_subPath) const;
    bool                     getSaveSimFile()                                   const;
    bool                     getCleanServer()                                   const;
    bool                     getAutoSaveSimulation()                            const;
    int                      getNumAutoSaveFiles()                              const;
    int                      getIterationInterval()                             const;
    std::vector<std::string> getRegionName()                                    const;
    std::vector<std::string> getBoundaryCondition()                             const;
    double                   getBaseSize()                                      const;
    int                      getPrismLayers()                                   const;
    double                   getPrismLayerThickness()                           const;
    double                   getNearWallThickness()                             const;
    std::vector<std::string> getSurfaceName()                                   const;
    std::vector<double>      getSurfaceSize()                                   const;
    double                   getMachNumber()                                    const;
    double                   getDynamicViscosity()                              const;
    double                   getStaticTemperature()                             const;
    double                   getReferencePressure()                             const;
    double                   getReferenceVelocity()                             const;
    double                   getReferenceDensity()                              const;
    double                   getFlowDirectionX()                                const;
    double                   getFlowDirectionY()                                const;
    double                   getFlowDirectionZ()                                const;
    bool                     getCustomSolverOptions()                           const;
    double                   getCFL()                                           const;
    int                      getMaxSteps()                                      const;
    int                      getNumSamples()                                    const;
    double                   getAsymptoticCL()                                  const;

    // Class methods
    void loadStarJob();
    void printJobData();
};
#endif // STARJOB_H