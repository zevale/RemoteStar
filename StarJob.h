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

    // Volumetric controls
    std::vector<double> blockX1;
    std::vector<double> blockY1;
    std::vector<double> blockZ1;
    std::vector<double> blockX2;
    std::vector<double> blockY2;
    std::vector<double> blockZ2;
    std::vector<double> blockSurfaceSize;
    std::vector<double> cylinderX1;
    std::vector<double> cylinderY1;
    std::vector<double> cylinderZ1;
    std::vector<double> cylinderX2;
    std::vector<double> cylinderY2;
    std::vector<double> cylinderZ2;
    std::vector<double> cylinderRadius;
    std::vector<double> cylinderSurfaceSize;

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
    double                   asymptoticCD;

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
    std::vector<double>      getBlockX1()                                       const;
    std::vector<double>      getBlockY1()                                       const;
    std::vector<double>      getBlockZ1()                                       const;
    std::vector<double>      getBlockX2()                                       const;
    std::vector<double>      getBlockY2()                                       const;
    std::vector<double>      getBlockZ2()                                       const;
    std::vector<double>      getBlockSurfaceSize()                              const;
    std::vector<double>      getCylinderX1()                                    const;
    std::vector<double>      getCylinderY1()                                    const;
    std::vector<double>      getCylinderZ1()                                    const;
    std::vector<double>      getCylinderX2()                                    const;
    std::vector<double>      getCylinderY2()                                    const;
    std::vector<double>      getCylinderZ2()                                    const;
    std::vector<double>      getCylinderRadius()                                const;
    std::vector<double>      getCylinderSurfaceSize()                           const;
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
    double                   getAsymptoticCD()                                  const;

    // Class methods
    void loadStarJob();
    void printJobData();
};
#endif // STARJOB_H