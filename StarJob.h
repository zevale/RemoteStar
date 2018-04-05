/*
 * CLASS StarJob
 *
 * DESCRIPTION
 * StarJob loads the simulation data from the job file passed to the
 * RemoteStar client.
 * This data are shared with:
 *    - MightyMacro framework to generate the macro that will setup the simulation
 *    - StarHost to write the run script that launches STAR-CCM+
 *    - submitJob() to send files and commands to the server
 *    - fetchResults() to get the results from the server
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 */
#ifndef STARJOB_H
#define STARJOB_H

#include <string>
#include <vector>

// Constants defining the column width to print job data
constexpr int largeColumn  = 32;
constexpr int mediumColumn = 20;
// constexpr int smallColumn  = 15;

class StarJob {
private:
    // Job setup
    bool                     batchModeOption;       // Batch mode - no user input required
    std::string              jobFilePath;           // Path to job file - must end in slash
    std::string              jobName;               // Name of the folder containing the resources
    std::string              initializationJob;     // Name of the job used (on the server) used for initialization
    bool                     newMesh;               // If true, will remesh the model
    std::string              clientDirectory;       // File path to the client folder - must end in slash
    std::string              serverDirectory;       // File path to the server folder - must end in slash
    bool                     saveSimFile;           // If true, will copy the sim file to the client
    bool                     cleanServer;           // If true, will delete the sim file on the server
    bool                     autoSaveSimulation;    // If true, will setup auto save during the simulation based on:
    int                      numAutoSaveFiles;      //    - Number of auto save files. Zero will replace results.
    int                      iterationInterval;     //    - Interval between auto saves.

    // Regions
    std::vector<std::string> regionName;            // Name of regions (first is the aircraft, then domain regions)
    std::vector<std::string> boundaryCondition;     // Type of boundary conditions to apply to each region

    // Mesh model
    double                   baseSize;              // STAR-CCM+ base size                    [m]
    int                      prismLayers;           // Number of prism layers                 [ ]
    double                   prismLayerThickness;   // Thickness of the prism layer           [m]
    double                   nearWallThickness;     // Hight of the first prism layer element [m]
    std::vector<std::string> surfaceName;           // Same order as regionName
    std::vector<double>      surfaceSize;           // Target and minimum surface size        [m]

    // Volumetric controls
    std::vector<double> blockX1;                    // X coordinate first block corner                       [m]
    std::vector<double> blockY1;                    // Y coordinate first block corner                       [m]
    std::vector<double> blockZ1;                    // Z coordinate first block corner                       [m]
    std::vector<double> blockX2;                    // X coordinate second block corner                      [m]
    std::vector<double> blockY2;                    // Y coordinate second block corner                      [m]
    std::vector<double> blockZ2;                    // Z coordinate second block corner                      [m]
    std::vector<double> blockSurfaceSize;           // Target and minimum mesh surface size for the block    [m]
    std::vector<double> cylinderX1;                 // X coordinate first cylinder axis end                  [m]
    std::vector<double> cylinderY1;                 // Y coordinate first cylinder axis end                  [m]
    std::vector<double> cylinderZ1;                 // Z coordinate first cylinder axis end                  [m]
    std::vector<double> cylinderX2;                 // X coordinate second cylinder axis end                 [m]
    std::vector<double> cylinderY2;                 // Y coordinate second cylinder axis end                 [m]
    std::vector<double> cylinderZ2;                 // Z coordinate second cylinder axis end                 [m]
    std::vector<double> cylinderRadius;             // Cylinder radius                                       [m]
    std::vector<double> cylinderSurfaceSize;        // Target and minimum mesh surface size for the cylinder [m]
    std::vector<double> coneX1;                     // X coordinate first cone axis end                      [m]
    std::vector<double> coneY1;                     // Y coordinate first cone axis end                      [m]
    std::vector<double> coneZ1;                     // Z coordinate first cone axis end                      [m]
    std::vector<double> coneX2;                     // X coordinate second cone axis end                     [m]
    std::vector<double> coneY2;                     // Y coordinate second cone axis end                     [m]
    std::vector<double> coneZ2;                     // Z coordinate second cone axis end                     [m]
    std::vector<double> coneRadius1;                // Cone radius on the first base                         [m]
    std::vector<double> coneRadius2;                // Cone radius on the second base                        [m]
    std::vector<double> coneSurfaceSize;

    // Physics model
    double                   machNumber;            // Freestream Mach number         [ ]
    double                   dynamicViscosity;      // If present, simulation is RANS [Pa.s] - OPTIONAL
    double                   staticTemperature;     // Static temperature             [K]
    double                   referencePressure;     // Freestream static pressure     [Pa]
    double                   referenceVelocity;     // Reference velocity (CL and CD) [m/s]
    double                   referenceDensity;      // Reference air density          [kg/m3]
    double                   flowDirectionX;        // X component director vector    [ ]
    double                   flowDirectionY;        // Y component director vector    [ ]
    double                   flowDirectionZ;        // Z component director vector    [ ]

    // Solver options
    bool                     customSolverOptions;   // If true, will use custom CFL
    double                   CFL;                   // Courant number

    // Stopping criteria
    int                      maxSteps;              // Maximum number of iterations
    int                      numSamples;            // Number of samples to study asymptotic convergence - OPTIONAL
    double                   asymptoticCL;          //    - Precision for asymptotic CL convergence      - OPTIONAL
    double                   asymptoticCD;          //    - Precision for asymptotic CD convergence      - OPTIONAL

private:
    // Print text in columns
    void printTwoColumns(std::string _c1, std::string _c2)                    const;
    void printTwoColumns(std::string _c1, std::string _c2, char _fill)        const;
    void printThreeColumns(std::string _c1, std::string _c2, std::string _c3) const;

public:
    // Constructor
   explicit StarJob(bool _batchMode, const std::string& _jobFilePath = "star_jobData");

    // Getters
    std::string              getJobName()                                       const;
    std::string              getInitializationJob()                             const;
    bool                     getNewMesh()                                       const;
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
//    std::vector<std::string> getSurfaceName()                                   const;
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
    std::vector<double>      getConeX1()                                        const;
    std::vector<double>      getConeY1()                                        const;
    std::vector<double>      getConeZ1()                                        const;
    std::vector<double>      getConeX2()                                        const;
    std::vector<double>      getConeY2()                                        const;
    std::vector<double>      getConeZ2()                                        const;
    std::vector<double>      getConeRadius1()                                   const;
    std::vector<double>      getConeRadius2()                                   const;
    std::vector<double>      getConeSurfaceSize()                               const;
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
    void printJobData()                                                         const;
};
#endif // STARJOB_H
