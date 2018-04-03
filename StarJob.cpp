#include "StarJob.h"
#include "MightyMacroMaker/MightyConstants.h"
#include "star_client.h"
#include "MightyMacroMaker/MightyMath.h"
#include "exit_codes.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
#endif

#if defined(linux) || defined(__APPLE__)
#include <thread>               // To halt the thread while loadingScreen()
#include <chrono>               // Because of miliseconds()
#endif

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

std::string StarJob::getInitializationJob() const {
    return initializationJob;
}

bool StarJob::getNewMesh() const {
    return newMesh;
}

std::string StarJob::getClientDirectory() const {
    return clientDirectory;
}

std::string StarJob::getClientJobDirectory() const {
    return (clientDirectory + jobName + CrossPlatform::separator);
}

std::string StarJob::getClientJobDirectory(const std::string& _subPath) const {
    return (clientDirectory + jobName + CrossPlatform::separator + _subPath);
}

std::string StarJob::getServerDirectory() const {
    return serverDirectory;
}

std::string StarJob::getServerJobDirectory() const {
    return (serverDirectory + jobName + "/");
}

std::string StarJob::getServerJobDirectory(const std::string &_subPath) const {
    return (serverDirectory + jobName + "/" + _subPath);
}

bool StarJob::getSaveSimFile() const{
    return saveSimFile;
}

bool StarJob::getCleanServer() const {
    return cleanServer;
}

bool StarJob::getAutoSaveSimulation() const {
    return autoSaveSimulation;
}

int StarJob::getNumAutoSaveFiles() const {
    return numAutoSaveFiles;
}

int StarJob::getIterationInterval() const {
    return iterationInterval;
}

std::vector<std::string> StarJob::getRegionName() const {
    return regionName;
}

std::vector<std::string> StarJob::getBoundaryCondition() const {
    return boundaryCondition;
}

double StarJob::getBaseSize() const {
    return baseSize;
}

int StarJob::getPrismLayers() const {
    return prismLayers;
}

double StarJob::getPrismLayerThickness() const {
    return prismLayerThickness;
}

double StarJob::getNearWallThickness() const {
    return nearWallThickness;
}

std::vector<std::string> StarJob::getSurfaceName() const {
    return surfaceName;
}

std::vector<double> StarJob::getSurfaceSize() const {
    return surfaceSize;
}

std::vector<double> StarJob::getBlockX1() const {
    return blockX1;
}

std::vector<double> StarJob::getBlockY1() const {
    return blockY1;
}

std::vector<double> StarJob::getBlockZ1() const {
    return blockZ1;
}

std::vector<double> StarJob::getBlockX2() const {
    return blockX2;
}

std::vector<double> StarJob::getBlockY2() const {
    return blockY2;
}

std::vector<double> StarJob::getBlockZ2() const {
    return blockZ2;
}

std::vector<double> StarJob::getBlockSurfaceSize() const {
    return blockSurfaceSize;
}

std::vector<double> StarJob::getCylinderX1() const {
    return cylinderX1;
}

std::vector<double> StarJob::getCylinderY1() const {
    return cylinderY1;
}

std::vector<double> StarJob::getCylinderZ1() const {
    return cylinderZ1;
}

std::vector<double> StarJob::getCylinderX2() const {
    return cylinderX2;
}

std::vector<double> StarJob::getCylinderY2() const {
    return cylinderY2;
}

std::vector<double> StarJob::getCylinderZ2() const {
    return cylinderZ2;
}

std::vector<double> StarJob::getCylinderRadius() const {
    return cylinderRadius;
}

std::vector<double> StarJob::getCylinderSurfaceSize() const {
    return cylinderSurfaceSize;
}

std::vector<double> StarJob::getConeX1() const {
    return coneX1;
}

std::vector<double> StarJob::getConeY1() const {
    return coneY1;
}

std::vector<double> StarJob::getConeZ1() const {
    return coneZ1;
}

std::vector<double> StarJob::getConeX2() const {
    return coneX2;
}

std::vector<double> StarJob::getConeY2() const {
    return coneY2;
}

std::vector<double> StarJob::getConeZ2() const {
    return coneZ2;
}

std::vector<double> StarJob::getConeRadius1() const {
    return coneRadius1;
}

std::vector<double> StarJob::getConeRadius2() const {
    return coneRadius2;
}

std::vector<double> StarJob::getConeSurfaceSize() const {
    return coneSurfaceSize;
}

double StarJob::getMachNumber() const {
    return machNumber;
}

double StarJob::getDynamicViscosity() const {
    return dynamicViscosity;
}

double StarJob::getStaticTemperature() const {
    return staticTemperature;
}

double StarJob::getReferencePressure() const {
    return referencePressure;
}

double StarJob::getReferenceVelocity() const {
    return referenceVelocity;
}

double StarJob::getReferenceDensity() const {
    return referenceDensity;
}

double StarJob::getFlowDirectionX() const {
    return flowDirectionX;
}

double StarJob::getFlowDirectionY() const {
    return flowDirectionY;
}

double StarJob::getFlowDirectionZ() const {
    return flowDirectionZ;
}

bool StarJob::getCustomSolverOptions() const {
    return customSolverOptions;
}

double StarJob::getCFL() const {
    return CFL;
}

int StarJob::getMaxSteps() const {
    return maxSteps;
}

int StarJob::getNumSamples() const {
    return numSamples;
}

double StarJob::getAsymptoticCL() const {
    return asymptoticCL;
}

double StarJob::getAsymptoticCD() const {
    return asymptoticCD;
}

/*
 * Constructor that sets the job file path assuming  <star_jobData>
 * is in the current directory by default
 */
StarJob::StarJob(const std::string& _jobFilePath, bool _batchMode) :
        // Command line arguments
        jobFilePath    (_jobFilePath),
        batchModeOption(_batchMode),

        // Initialization job defaults
        initializationJob(Default::initializationJob),
        newMesh           (Default::newMesh),

        // Auto save defaults (might not be assigned by user)
        autoSaveSimulation(Default::autoSaveSimulation),
        numAutoSaveFiles  (Default::numAutosaveFiles),
        iterationInterval (Default::iterationInterval),

        // Prism layer defaults
        prismLayers(Default::zeroLayers),

        // Volumetric controls defaults
        // blockSurfaceSize({Default::surfaceSize}),

        // Physics values defaults
        dynamicViscosity(Default::dynamicViscosity),

        // Stopping criteria defaults
        numSamples  (Default::numSamples),
        asymptoticCL(Default::asymptoticCL),
        asymptoticCD(Default::asymptoticCD) {}

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
    if(!starJobFile.is_open()){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CANNOT_OPEN);
        throw "Cannot open file <star_jobData>";
    }

    // PARSER AUXILIARY VARIABLES (flags)

    // Star job tags
    bool hasBeginStarJob    = false;
    bool hasEndStarJob      = false;

    // Setup options
    bool hasBeginJobSetup      = false;
    bool hasJobName            = false;
    bool hasInitialization     = false;
    bool hasClientDirectory    = false;
    bool hasServerDirectory    = false;
    bool hasSaveSimFile        = false;
    bool hasCleanServer        = false;
    bool hasAutoSave           = false;
    bool hasAutoSaveFiles      = false;
    bool hasAutoSaveIterations = false;

    // Regions options
    bool hasBeginRegions  = false;
    bool hasOneWall       = false;
    bool hasOneFreeStream = false;

    // Mesh model options
    bool hasBeginMeshModel      = false;
    bool hasBaseSize            = false;
    bool hasPrismLayers         = false;
    bool hasPrismLayerThickness = false;
    bool hasNearWallThickness   = false;
    bool hasTwoSurfaceSizes     = false;

    // Volumentric control options
    bool hasBeginVolumetricControls = false;
//    bool hasBlock                   = false;
    bool hasBlockCorner1            = false;
    bool hasBlockCorner2            = false;
    bool hasBlockSurfaceSize        = false;
//    bool hasCylinder                = false;
    bool hasCylinderBase1           = false;
    bool hasCylinderBase2           = false;
    bool hasCylinderRadius          = false;
    bool hasCylinderSurfaceSize     = false;
//    bool hasCone                    = false;
    bool hasConeBase1               = false;
    bool hasConeBase2               = false;
    bool hasConeRadius1             = false;
    bool hasConeRadius2             = false;
    bool hasConeSurfaceSize         = false;

    // Physics model mandatory options
    bool hasBeginPhysicsModel = false;
//    bool hasDynamicViscosity  = false;
    bool hasStaticTemperature = false;
    bool hasReferencePressure = false;
    bool hasReferenceVelocity = false;
    bool hasReferenceDensity  = false;
    bool hasMachNumber        = false;
    bool hasFlowDirectionX    = false;
    bool hasFlowDirectionY    = false;
    bool hasFlowDirectionZ    = false;

    // Solver options
    bool hasBeginSolverOptions = false;
    bool hasCFL                = false;

    // Stopping criteria options
    bool hasBeginStoppingCriteria = false;
    bool hasMaxSteps              = false;
    bool hasSamples               = false;
    bool hasAsymptotycCL          = false;
    bool hasAsymptotycCD          = false;

    // Job status options
    bool jobSetup              = false;
    bool meshModel             = false;
    bool volumetricControls    = false;
    bool physicsModel          = false;
    bool solverOptions         = false;
    bool stoppingCriteria      = false;
    bool regions               = false;

    // Parser status
    bool busyElsewhere = false;

    // PARSE <star_jobData>

    std::cout << "\n:::::::::::: LOADING JOB" << std::endl;

    // Get line from file
    std::string line;
    while(std::getline(starJobFile, line)){

        // Get words from line
        std::istringstream issLine(line);
        while(issLine >> word){
//            std::cout << word << std::endl;

            // Check #BEGIN_STAR_JOB tag
            if(!hasBeginStarJob && (word == "#BEGIN_STAR_JOB"))
                hasBeginStarJob = true;

            // Check #END_STAR_JOB tag and exit
            if(!hasEndStarJob && (word == "#END_STAR_JOB")){
                hasEndStarJob = true;
                break;
            }

            // JOB SETUP
            busyElsewhere = (hasBeginRegions            ||
                             hasBeginMeshModel          ||
                             hasBeginVolumetricControls ||
                             hasBeginPhysicsModel       ||
                             hasBeginSolverOptions      ||
                             hasBeginStoppingCriteria);
            if(!jobSetup && !busyElsewhere){

                // Check for #BEGIN_JOB_SETUP tag
                if(hasBeginStarJob && (word == "#BEGIN_JOB_SETUP"))
                    hasBeginJobSetup = true;

                // Check for job_name
                if(hasBeginJobSetup && (word == "job_name")){
                    if(issLine >> word) {
                        jobName = word;
                        hasJobName = true;
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_JOB_NAME_EMPTY);
                        throw "<job_name> is empty";
                    }
                }

                // Check for initialization_job
                if(hasBeginJobSetup && (word == "initialization_job")){
                    if(issLine >> word) {
                        initializationJob = word;
                        hasInitialization = true;
                    } else{
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_INITIALIZATION_JOB_EMPTY);
                        throw "<initialization_job> is empty";
                    }
                }

                // Check for client_directory
                if(hasBeginJobSetup && (word == "client_directory")){
                    if(issLine >> word) {
                        clientDirectory = word;
                        hasClientDirectory = true;
                    } else{
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CLIENT_DIRECTORY_EMPTY);
                        throw "<client_directory> is empty";
                    }
                }

                // Check for server_directory
                if(hasBeginJobSetup && (word == "server_directory")){
                    if(issLine >> word){
                        serverDirectory = word;
                        hasServerDirectory = true;
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SERVER_DIRECTORY_EMPTY);
                        throw "<server_directory> is empty";
                    }
                }

                // Check save_sim_file
                if(hasBeginJobSetup && (word == "save_sim_file")){
                    if(issLine >> word){
                        if(word == "yes"){
                            saveSimFile = true;
                            hasSaveSimFile = true;
                        } else if(word == "no"){
                            saveSimFile = false;
                            hasSaveSimFile = true;
                        } else {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SAVE_SIM_FILE_USAGE);
                            throw "usage <save_sim_file> yes/no";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SAVE_SIM_FILE_EMPTY);
                        throw "<save_sim_file> is empty";
                    }
                }

                // Check clean_server
                if(hasBeginJobSetup && (word == "clean_server")){
                    if(issLine >> word){
                        if(word == "yes"){
                            cleanServer = true;
                            hasCleanServer = true;
                        } else if(word == "no"){
                            cleanServer = false;
                            hasCleanServer = true;
                        } else {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CLEAN_SERVER_USAGE);
                            throw "usage <clean_server> yes/no";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CLEAN_SERVER_EMPTY);
                        throw "<clean_server> is empty";
                    }
                }

                // Check auto_save
                if(hasBeginJobSetup && (word == "auto_save")){
                    if(issLine >> word){
                        if(word == "yes"){
                            autoSaveSimulation = true;
                            hasAutoSave = true;
                        } else if(word == "no"){
                            autoSaveSimulation = false;
                            hasAutoSave = false;
                        } else {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_AUTO_SAVE_USAGE);
                            throw "usage <auto_save> yes/no";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_AUTO_SAVE_EMPTY);
                        throw "<auto_save> is empty";
                    }
                }

                // Check auto_save_files
                if(hasBeginJobSetup && (word == "auto_save_files")){
                    if(issLine >> word){
                        try {
                            numAutoSaveFiles = std::stoi(word, nullptr);
                            // Check it is positive
                            if(numAutoSaveFiles < 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_AUTO_SAVE_FILES_INPUT);
                                throw "invalid argument for <auto_save_files> (int >= 0)";
                            }
                            // Successful
                            hasAutoSaveFiles = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_AUTO_SAVE_FILES_INPUT);
                            throw "invalid argument for <auto_save_files> (int >= 0)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_AUTO_SAVE_FILES_EMPTY);
                        throw "<auto_save_files> is empty";
                    }
                }

                // Check auto_save_iterations
                if(hasBeginJobSetup && (word == "auto_save_iterations")){
                    if(issLine >> word){
                        try {
                            iterationInterval = std::stoi(word, nullptr);
                            // Check it is >= 1
                            if(iterationInterval < 1){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_AUTO_SAVE_ITERATIONS_INPUT);
                                throw "invalid argument for <auto_save_iterations> (int >= 1)";
                            }
                            // Successful
                            hasAutoSaveIterations = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_AUTO_SAVE_ITERATIONS_INPUT);
                            throw "invalid argument for <auto_save_iterations> (int >= 1)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_AUTO_SAVE_ITERATIONS_EMPTY);
                        throw "<auto_save_iterations> is empty";
                    }
                }

                // Check #END_JOB_SETUP
                if(hasBeginJobSetup && (word == "#END_JOB_SETUP")){
                    // Check if job setup data is complete
                    if(hasJobName && hasClientDirectory && hasServerDirectory && hasSaveSimFile && hasCleanServer
                            && ((hasAutoSave && hasAutoSaveFiles && hasAutoSaveIterations) || !hasAutoSave)) {
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Job setup"
                                                       << std::right << std::setw(mediumColumn) << ".";
                        colorText("Loaded\n", GREEN);
                        // jobSetup complete
                        jobSetup = true;
                        hasBeginJobSetup = false;
                    } else {
                        // Missing data
                        if(!hasJobName){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_JOB_NAME_MISSING);
                            throw "<job_name> is missing";
                        }
                        if(!hasClientDirectory){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CLIENT_DIRECTORY_MISSING);
                            throw "<client_directory> is missing";
                        }
                        if(!hasServerDirectory){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SERVER_DIRECTORY_MISSING);
                            throw "<server_directory> is missing";
                        }
                        if(!hasSaveSimFile){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SAVE_SIM_FILE_MISSING);
                            throw "<save_sim_file> is missing";
                        }

                        if(!hasCleanServer){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CLEAN_SERVER_MISSING);
                            throw "<clean_server> is missing";
                        }
                        if(hasAutoSave && !hasAutoSaveFiles){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_AUTO_SAVE_FILES_MISSING);
                            throw "<auto_save_files> is missing";
                        }
                        if(hasAutoSave && !hasAutoSaveIterations){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_AUTO_SAVE_ITERATIONS_MISSING);
                            throw "<auto_save_iterations> is missing";
                        }
                    }
                }
            }
            // END JOB SETUP

            busyElsewhere = (hasBeginJobSetup           ||
                             hasBeginMeshModel          ||
                             hasBeginVolumetricControls ||
                             hasBeginPhysicsModel       ||
                             hasBeginSolverOptions      ||
                             hasBeginStoppingCriteria);
            // REGIONS
            if(!regions && !busyElsewhere){

                // Check #BEGIN_REGIONS
                if(hasBeginStarJob && (word == "#BEGIN_REGIONS"))
                    hasBeginRegions = true;

                // Check wall region
                if(hasBeginRegions && (word == "wall")){
                    boundaryCondition.emplace_back("wall");
                    if(issLine >> word){
                        regionName.push_back(word);
                        hasOneWall = true;
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_WALL_REGION_MISSING_NAME);
                        throw "wall region is missing name";
                    }
                }

                // Check free stream region
                if(hasBeginRegions && (word == "free_stream")){
                    boundaryCondition.emplace_back("freeStream");
                    if(issLine >> word){
                        regionName.push_back(word);
                        hasOneFreeStream = true;
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_FREE_STREAM_REGION_MISSING_NAME);
                        throw "free stream region is missing name";
                    }
                }

                // Check symmetry plane region
                if(hasBeginRegions && (word == "symmetry_plane")){
                    boundaryCondition.emplace_back("symmetryPlane");
                    if(issLine >> word)
                        regionName.push_back(word);
                    else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SYMMETRY_PLANE_REGION_MISSING_NAME);
                        throw "symmetry plane region is missing name";
                    }
                }

                // Check #END_REGIONS
                if(hasBeginRegions && (word == "#END_REGIONS")){
                    // Check if regions data is complete
                    if(hasOneWall && hasOneFreeStream){
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Regions"
                                                       << std::right << std::setw(mediumColumn) << ".";
                        colorText("Loaded\n", GREEN);
                        // regions setup complete
                        regions = true;
                        hasBeginRegions = false;
                    } else {
                        // Missing data
                        if(!hasOneWall) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_WALL_REGION_MISSING);
                            throw "wall region is missing";
                        }
                        if(!hasOneFreeStream){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_FREE_STREAM_REGION_MISSING);
                            throw "free stream region is missing";
                        }
                    }
                }
            }
            // END REGIONS

            // MESH MODEL
            busyElsewhere = (hasBeginJobSetup           ||
                             hasBeginRegions            ||
                             hasBeginVolumetricControls ||
                             hasBeginPhysicsModel       ||
                             hasBeginSolverOptions      ||
                             hasBeginStoppingCriteria);
            if(!meshModel && !busyElsewhere){

                // Check #BEGIN_MESH_MODEL tag
                if(hasBeginStarJob && (word == "#BEGIN_MESH_MODEL"))
                    hasBeginMeshModel = true;

                // Check base_size
                if(hasBeginMeshModel && (word == "base_size")){
                    if(issLine >> word){
                        try {
                            baseSize = std::stod(word, nullptr);
                            // Check data
                            if(baseSize <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BASE_SIZE_INPUT);
                                throw "invalid argument for <base_size> (double > 0)";
                            }
                            // Successful
                            hasBaseSize = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BASE_SIZE_INPUT);
                            throw "invalid argument for <base_size> (double > 0)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BASE_SIZE_EMPTY);
                        throw "<base_size> is empty";
                    }
                }

                // Check num_prism_layers
                if(hasBeginMeshModel && (word == "num_prism_layers")){
                    if(issLine >> word){
                        try {
                            prismLayers = std::stoi(word, nullptr);
                            // Check data
                            // Check data
                            if(prismLayers < 1){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_NUM_PRISM_LAYER_INPUT);
                                throw "invalid argument for <num_prism_layers> (int >= 1)";
                            }
                            // Successful
                            hasPrismLayers = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_NUM_PRISM_LAYER_INPUT);
                            throw "invalid argument for <num_prism_layers> (int >= 1)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_PRISM_LAYER_EMPTY);
                        throw "<num_prism_layers> is empty";
                    }
                }

                // Check prism_layer_thickness
                if(hasBeginMeshModel && (word == "prism_layer_thickness")){
                    if(issLine >> word){
                        try {
                            prismLayerThickness = std::stod(word, nullptr);
                            // Check data
                            if(prismLayerThickness <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_PRISM_LAYER_THICKNESS_INPUT);
                                throw "invalid argument invalid argument for <prism_layer_thickness> (double > 0)";
                            }
                            // Successful
                            hasPrismLayerThickness = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_PRISM_LAYER_THICKNESS_INPUT);
                            throw "invalid argument invalid argument for <prism_layer_thickness> (double > 0)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_PRISM_LAYER_THICKNESS_EMPTY);
                        throw "<prism_layer_thickness> is empty";
                    }
                }

                // Check near_wall_thickness
                if(hasBeginMeshModel && (word == "near_wall_thickness")){
                    if(issLine >> word){
                        try {
                            nearWallThickness = std::stod(word, nullptr);
                            // Check data
                            if(nearWallThickness <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_NEAR_WALL_THICKNESS_INPUT);
                                throw "invalid argument for <near_wall_thickness> (double > 0)";
                            }
                            // Successful
                            hasNearWallThickness = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_NEAR_WALL_THICKNESS_INPUT);
                            throw "invalid argument for <near_wall_thickness> (double > 0)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_NEAR_WALL_THICKNESS_EMPTY);
                        throw "<near_wall_thickness> is empty";
                    }
                }

                // Check surface_size
                if(hasBeginMeshModel && (word == "surface_size")){
                    // First get the surface name
                    if(issLine >> word){
                        surfaceName.push_back(word);
                        // Then get surface size
                        if(issLine >> word){
                            try {
                                surfaceSize.push_back(stod(word, nullptr));
                                // Check data
                                if(surfaceSize[surfaceSize.size()-1] <= 0){
                                    g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SURFACE_SIZE_VALUE_INPUT);
                                    throw "invalid argument for <surface_size> (double > 0)";
                                }
                            } catch (const std::invalid_argument &invalidArgument) {
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SURFACE_SIZE_VALUE_INPUT);
                                throw "invalid argument for <surface_size> (double > 0)";
                            }
                        } else {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SURFACE_SIZE_VALUE_EMPTY);
                            throw "<surface_size> value is empty";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SURFACE_SIZE_NAME_EMPTY);
                        throw "<surface_size> name is empty";
                    }

                    // Check if at leas two surface sizes have been entered (aircraft + freestream)
                    if(surfaceSize.size() == 2)
                        hasTwoSurfaceSizes = true;
                }

                // Check #END_MESH_MODEL tag
                if(hasBeginMeshModel && (word == "#END_MESH_MODEL")){
                    // Check mesh model data is complete
                    if(     hasBaseSize
                       &&
                            // Has prism layer and options or no prism layer.
                            ((hasPrismLayers && hasPrismLayerThickness && hasNearWallThickness) ||
                             (!hasPrismLayers && !hasPrismLayerThickness && !hasNearWallThickness))
                       &&
                            hasTwoSurfaceSizes){
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Mesh model"
                                                       << std::right << std::setw(mediumColumn) << ".";
                        colorText("Loaded\n", GREEN);
                        // Mesh model complete
                        meshModel = true;
                        hasBeginMeshModel = false;
                    } else {
                        // Missing data
                        if(hasPrismLayers && !hasPrismLayerThickness){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_PRISM_LAYER_THICKNESS_MISSING);
                            throw "<prism_layer_thickness> is missing";
                        }
                        if(hasPrismLayers && !hasNearWallThickness){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_NEAR_WALL_THICKNESS_MISSING);
                            throw "<near_wall_thickness> is missing";
                        }
                        if(!hasPrismLayers && (hasPrismLayerThickness || hasNearWallThickness)){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_NUM_PRISM_LAYER_MISSING);
                            throw "<num_prism_layers> is missing";
                        }
                        if(!hasTwoSurfaceSizes){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SURFACE_SIZE_ENTRIES);
                            throw "needs at least two surface sizes";
                        }
                    }
                }

            }
            // END MESH MODEL

            // VOLUMETRIC CONTROLS
            busyElsewhere = (hasBeginJobSetup      ||
                             hasBeginMeshModel     ||
                             hasBeginRegions       ||
                             hasBeginPhysicsModel  ||
                             hasBeginSolverOptions ||
                             hasBeginStoppingCriteria);
            if(!volumetricControls && !busyElsewhere) {

                // Check #BEGIN_VOLUMETRIC_CONTROLS tag
                if(hasBeginStarJob && (word == "#BEGIN_VOLUMETRIC_CONTROLS"))
                    hasBeginVolumetricControls = true;

                while(hasBeginVolumetricControls && std::getline(starJobFile, line)){
                    // Get first word from line into issLine and word
                    issLine.str("");            // Empty issLine
                    issLine.clear();            // Clear stream's error state
                    issLine.str(line);          // Update stream
                    issLine >> word;            // Input first word

                    // BLOCK

                    // Check block tag
                    if(word == "block"){
//                        hasBlock = true;
                        // Set block data members to false until data arrives
                        hasBlockCorner1 = false;
                        hasBlockCorner2 = false;
                        hasBlockSurfaceSize = false;

                        // Get block data
                        while(std::getline(starJobFile, line)){
                            // Get words from line into issLine
                            issLine.str("");            // Empty issLine
                            issLine.clear();            // Clear stream's error state
                            issLine.str(line);          // Update stream
                            while(issLine >> word){

                                // Check block corner_1
                                if(word == "corner_1"){
                                    int coordinates = 0;
                                    while((issLine >> word) && coordinates < 3){
                                        if(coordinates == 0)
                                            try {
                                                blockX1.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_CORNER_1_X1_INPUT);
                                                throw "invalid argument for X in <corner_1> (double)";
                                            }
                                        else if(coordinates == 1)
                                            try {
                                                blockY1.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_CORNER_1_Y1_INPUT);
                                                throw "invalid argument for Y in <corner_1> (double)";
                                            }
                                        else if(coordinates == 2){
                                            try {
                                                blockZ1.emplace_back((std::stod(word, nullptr)));
                                                // Successful
                                                hasBlockCorner1 = true;
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_CORNER_1_Z1_INPUT);
                                                throw "invalid argument for Z in <corner_1> (double)";
                                            }
                                        }
                                        coordinates++;
                                    }
                                    // Check all coordinates
                                    if(coordinates != 3){
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_CORNER_1_MISSING_DATA);
                                        throw "block <corner_1> is missing data";
                                    }
                                }

                                // Check block corner_2
                                if(word == "corner_2"){
                                    int coordinates = 0;
                                    while((issLine >> word) && coordinates < 3){
                                        if(coordinates == 0)
                                            try {
                                                blockX2.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_CORNER_2_X2_INPUT);
                                                throw "nvalid argument for X in <corner_2> (double)";
                                            }
                                        else if(coordinates == 1)
                                            try {
                                                blockY2.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_CORNER_2_Y2_INPUT);
                                                throw "invalid argument for Y in <corner_2> (double)";
                                            }
                                        else if(coordinates == 2){
                                            try {
                                                blockZ2.emplace_back((std::stod(word, nullptr)));
                                                // Successful
                                                hasBlockCorner2 = true;
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_CORNER_2_Z2_INPUT);
                                                throw "invalid argument for Z in <corner_2> (double)";
                                            }
                                        }
                                        coordinates++;
                                    }
                                    // Check all coordinates
                                    if(coordinates != 3){
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_CORNER_2_MISSING_DATA);
                                        throw "<corner_2> is missing data";
                                    }
                                }

                                // Check surface_size
                                if(word == "surface_size"){
                                    if(issLine >> word){
                                        try {
                                            blockSurfaceSize.emplace_back(std::stod(word, nullptr));
                                            // Check data
                                            if(blockSurfaceSize[blockSurfaceSize.size()-1] <= 0){
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_SURFACE_SIZE_INPUT);
                                                throw "invalid argument for block <surface_size> (double > 0)";
                                            }
                                            // Successful
                                            hasBlockSurfaceSize = true;
                                        } catch (const std::invalid_argument &invalidArgument) {
                                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_SURFACE_SIZE_INPUT);
                                            throw "invalid argument for block <surface_size> (double > 0)";
                                        }
                                    } else {
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_SURFACE_SIZE_EMPTY);
                                        throw "block <surface_size> is empty";
                                    }
                                }

                                // Check for new part tags arriving too early and stop getting words
                                if(word == "block" || word == "cone" || word == "cylinder")
                                    break;
                            }
                            // Check if all block data members have been entered
                            if(hasBlockCorner1 && hasBlockCorner2 & hasBlockSurfaceSize){
                                // Stop getting block lines
                                break;
                            }

                            // Check for new parts and stop getting lines
                            if(word == "block" || word == "cone" || word == "cylinder")
                                break;
                        }
                        // Missing data
                        if(!hasBlockCorner1){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_CORNER_1_MISSING);
                            throw "block <corner_1> is missing";
                        }
                        if(!hasBlockCorner2){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_CORNER_2_MISSING);
                            throw "block <corner_2> is missing";
                        }
                        if(!hasBlockSurfaceSize){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_BLOCK_SURFACE_SIZE_MISSING);
                            throw "block <surface_size> is missing";
                        }
//                        // Get words from line into issLine and word
//                        issLine.str("");            // Empty issLine
//                        issLine.clear();            // Clear stream's error state
//                        issLine.str(line);          // Update stream
//                        issLine >> word;            // Input first word
                    }

                    // CYLINDER

                    // Check cylinder tag
                    if(word == "cylinder"){
//                        hasCylinder = true;
                        // Set cylinder data members to false until data arrives
                        hasCylinderBase1       = false;
                        hasCylinderBase2       = false;
                        hasCylinderRadius      = false;
                        hasCylinderSurfaceSize = false;

                        // Get cylinder data
                        while(std::getline(starJobFile, line)) {
                            // Get words from line into issLine
                            issLine.str("");            // Empty issLine
                            issLine.clear();            // Clear stream's error state
                            issLine.str(line);          // Update stream
                            while (issLine >> word) {
                                // Check cylinder base_1
                                if(word == "base_1"){
                                    int coordinates = 0;
                                    while((issLine >> word) && coordinates < 3){
                                        if(coordinates == 0)
                                            try {
                                                cylinderX1.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_BASE_1_X1_INPUT);
                                                throw "invalid argument for X in cylinder <base_1> (double)";
                                            }
                                        else if(coordinates == 1)
                                            try {
                                                cylinderY1.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_BASE_1_Y1_INPUT);
                                                throw "invalid argument for Y in cylinder <base_1> (double)";
                                            }
                                        else if(coordinates == 2){
                                            try {
                                                cylinderZ1.emplace_back((std::stod(word, nullptr)));
                                                // Successful
                                                hasCylinderBase1 = true;
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_BASE_1_Z1_INPUT);
                                                throw "invalid argument for Z in cylinder <base_1> (double)";
                                            }
                                        }
                                        coordinates++;
                                    }
                                    // Check all coordinates
                                    if(coordinates != 3){
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_BASE_1_MISSING_DATA);
                                        throw "cylinder <base_1> is missing data";
                                    }
                                }

                                // Check cylinder base_2
                                if(word == "base_2"){
                                    int coordinates = 0;
                                    while((issLine >> word) && coordinates < 3){
                                        if(coordinates == 0)
                                            try {
                                                cylinderX2.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_BASE_2_X1_INPUT);
                                                throw "invalid argument for X in cylinder <base_2> (double)";
                                            }
                                        else if(coordinates == 1)
                                            try {
                                                cylinderY2.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_BASE_2_Y1_INPUT);
                                                throw "invalid argument for Y in cylinder <base_2> (double)";
                                            }
                                        else if(coordinates == 2){
                                            try {
                                                cylinderZ2.emplace_back((std::stod(word, nullptr)));
                                                // Successful
                                                hasCylinderBase2 = true;
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_BASE_2_Z1_INPUT);
                                                throw "invalid argument for Z in cylinder <base_2> (double)";
                                            }
                                        }
                                        coordinates++;
                                    }
                                    // Check all coordinates
                                    if(coordinates != 3){
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_BASE_2_MISSING_DATA);
                                        throw "cylinder <base_2> is missing data";
                                    }
                                }

                                // Check radius tag
                                if(word == "radius"){
                                    if(issLine >> word){
                                        try {
                                            cylinderRadius.emplace_back(std::stod(word, nullptr));
                                            // Check data
                                            if(cylinderRadius[cylinderRadius.size()-1] <= 0){
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_RADIUS_INPUT);
                                                throw "invalid argument for cylinder <radius> (double > 0)";
                                            }
                                            // Successful
                                            hasCylinderRadius = true;
                                        } catch (const std::invalid_argument &invalidArgument) {
                                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_RADIUS_INPUT);
                                            throw "invalid argument for cylinder <radius> (double > 0)";
                                        }
                                    } else {
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_RADIUS_EMPTY);
                                        throw "cylinder <radius> is empty";
                                    }
                                }

                                // Check surface_size
                                if(word == "surface_size"){
                                    if(issLine >> word){
                                        try {
                                            cylinderSurfaceSize.emplace_back(std::stod(word, nullptr));
                                            // Check data
                                            if(cylinderSurfaceSize[cylinderSurfaceSize.size()-1] <= 0){
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_SURFACE_SIZE_INPUT);
                                                throw "invalid argument for cylinder <surface_size> (double > 0)";
                                            }
                                            // Successful
                                            hasCylinderSurfaceSize = true;
                                        } catch (const std::invalid_argument &invalidArgument) {
                                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_SURFACE_SIZE_INPUT);
                                            throw "invalid argument for cylinder <surface_size> (double > 0)";
                                        }
                                    } else {
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_SURFACE_SIZE_EMPTY);
                                        throw "cylinder <surface_size> is empty";
                                    }
                                }

                                // Check for new part tags arriving too early and stop getting words
                                if(word == "block" || word == "cone" || word == "cylinder")
                                    break;
                            }
                            // Check if all cylinder data members have been entered
                            if(hasCylinderBase1 && hasCylinderBase2 && hasCylinderRadius && hasCylinderSurfaceSize){
                                // Stop getting cylinder data
                                break;
                            }

                            // Check for new parts and stop getting lines
                            if(word == "block" || word == "cone" || word == "cylinder")
                                break;
                        }
                        // Missing data
                        if(!hasCylinderBase1){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_BASE_1_MISSING);
                            throw "cylinder <base_1> is missing";
                        }
                        if(!hasCylinderBase2){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_BASE_2_MISSING);
                            throw "cylinder <base_2> is missing";}
                        if(!hasCylinderRadius){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_RADIUS_MISSING);
                            throw "cylinder <radius> is missing";}
                        if(!hasCylinderSurfaceSize){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CYLINDER_SURFACE_SIZE_MISSING);
                            throw "cylinder <surface_size> is missing";}
//                        // Get words from line into issLine and word
//                        issLine.str("");            // Empty issLine
//                        issLine.clear();            // Clear stream's error state
//                        issLine.str(line);          // Update stream
//                        issLine >> word;            // Input first word
                    }

                    // CONE

                    // Check cone tag
                    if(word == "cone"){
//                        hasCone = true;
                        // Set cone data members to false until data arrives
                        hasConeBase1       = false;
                        hasConeBase2       = false;
                        hasConeRadius1     = false;
                        hasConeRadius2     = false;
                        hasConeSurfaceSize = false;

                        // Get cone data
                        while(std::getline(starJobFile, line)) {
                            // Get words from line into issLine
                            issLine.str("");            // Empty issLine
                            issLine.clear();            // Clear stream's error state
                            issLine.str(line);          // Update stream
                            while (issLine >> word) {
                                // Check cone base_1
                                if(word == "base_1"){
                                    int coordinates = 0;
                                    while((issLine >> word) && coordinates < 3){
                                        if(coordinates == 0)
                                            try {
                                                coneX1.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_BASE_1_X1_INPUT);
                                                throw "invalid argument for X in cone <base_1> (double)";
                                            }
                                        else if(coordinates == 1)
                                            try {
                                                coneY1.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_BASE_1_Y1_INPUT);
                                                throw "invalid argument for Y in cone <base_1> (double)";
                                            }
                                        else if(coordinates == 2){
                                            try {
                                                coneZ1.emplace_back((std::stod(word, nullptr)));
                                                // Successful
                                                hasConeBase1 = true;
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_BASE_1_Z1_INPUT);
                                                throw "invalid argument for Z in cone <base_1> (double)";
                                            }
                                        }
                                        coordinates++;
                                    }
                                    // Check all coordinates
                                    if(coordinates != 3){
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_BASE_1_MISSING_DATA);
                                        throw "cone <base_1> is missing data";
                                    }
                                }

                                // Check cone base_2
                                if(word == "base_2"){
                                    int coordinates = 0;
                                    while((issLine >> word) && coordinates < 3){
                                        if(coordinates == 0)
                                            try {
                                                coneX2.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_BASE_2_X2_INPUT);
                                                throw "invalid argument for X in cone <base_2> (double)";
                                            }
                                        else if(coordinates == 1)
                                            try {
                                                coneY2.emplace_back(std::stod(word, nullptr));
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_BASE_2_Y2_INPUT);
                                                throw "invalid argument for Y in cone <base_2> (double)";
                                            }
                                        else if(coordinates == 2){
                                            try {
                                                coneZ2.emplace_back((std::stod(word, nullptr)));
                                                // Successful
                                                hasConeBase2 = true;
                                            } catch (const std::invalid_argument &invalidArgument) {
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_BASE_2_Z2_INPUT);
                                                throw "invalid argument for Z in cone <base_2> (double)";
                                            }
                                        }
                                        coordinates++;
                                    }
                                    // Check all coordinates
                                    if(coordinates != 3){
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_BASE_2_MISSING_DATA);
                                        throw "cone <base_2> is missing data";
                                    }
                                }

                                // Check radius_1 tag
                                if(word == "radius_1"){
                                    if(issLine >> word){
                                        try {
                                            coneRadius1.emplace_back(std::stod(word, nullptr));
                                            // Check data
                                            if(coneRadius1[coneRadius1.size()-1] <= 0){
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_RADIUS_1_INPUT);
                                                throw "invalid argument for cone <radius_1> (double > 0)";
                                            }
                                            // Successful
                                            hasConeRadius1 = true;
                                        } catch (const std::invalid_argument &invalidArgument) {
                                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_RADIUS_1_INPUT);
                                            throw "invalid argument for cone <radius_1> (double > 0)";
                                        }
                                    } else {
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_RADIUS_1_EMPTY);
                                        throw "cone <radius_1> is empty";
                                    }
                                }

                                // Check radius_2 tag
                                if(word == "radius_2"){
                                    if(issLine >> word){
                                        try {
                                            coneRadius2.emplace_back(std::stod(word, nullptr));
                                            // Check data
                                            if(coneRadius2[coneRadius2.size()-1] <= 0){
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_RADIUS_2_INPUT);
                                                throw "invalid argument for cone <radius_2> (double > 0)";
                                            }
                                            // Successful
                                            hasConeRadius2 = true;
                                        } catch (const std::invalid_argument &invalidArgument) {
                                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_RADIUS_2_INPUT);
                                            throw "invalid argument for cone <radius_2> (double > 0)";
                                        }
                                    } else {
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_RADIUS_2_EMPTY);
                                        throw "cone <radius_2> is empty";
                                    }
                                }

                                // Check surface_size
                                if(word == "surface_size"){
                                    if(issLine >> word){
                                        try {
                                            coneSurfaceSize.emplace_back(std::stod(word, nullptr));
                                            // Check data
                                            if(coneSurfaceSize[coneSurfaceSize.size()-1] <= 0){
                                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_SURFACE_SIZE_INPUT);
                                                throw "invalid argument for cone <surface_size> (double > 0)";
                                            }
                                            // Successful
                                            hasConeSurfaceSize = true;
                                        } catch (const std::invalid_argument &invalidArgument) {
                                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_SURFACE_SIZE_INPUT);
                                            throw "invalid argument for cone <surface_size> (double > 0)";
                                        }
                                    } else {
                                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_SURFACE_SIZE_EMPTY);
                                        throw "cone <surface_size> is empty";
                                    }
                                }

                                // Check for new part tags arriving too early and stop getting words
                                if(word == "block" || word == "cylinder" || word == "cone" )
                                    break;
                            }
                            // Check if all cone data members have been entered
                            if(hasConeBase1 && hasConeBase2 &&
                               hasConeRadius1 && hasConeRadius2 && hasConeSurfaceSize){
                                // Stop getting cylinder data
                                break;
                            }

                            // Check for new parts and stop getting lines
                            if(word == "block" || word == "cone" || word == "cylinder")
                                break;
                        }
                        // Missing data
                        if(!hasConeBase1){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_BASE_1_MISSING);
                            throw "cone <base_1> is missing";
                        }
                        if(!hasConeBase2){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_BASE_2_MISSING);
                            throw "cone <base_2> is missing";
                        }
                        if(!hasConeRadius1){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_RADIUS_1_MISSING);
                            throw "cone <radius_1> is missing";
                        }
                        if(!hasConeRadius2){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_RADIUS_2_MISSING);
                            throw "cone <radius_2> is missing";
                        }
                        if(!hasConeSurfaceSize){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_CONE_SURFACE_SIZE_MISSING);
                            throw "cone <surface_size> is missing";
                        }
//                        // Get words from line into issLine and word
//                        issLine.str("");            // Empty issLine
//                        issLine.clear();            // Clear stream's error state
//                        issLine.str(line);          // Update stream
//                        issLine >> word;            // Input first word
                    }

                    // Check ##END_VOLUMETRIC_CONTROLS tag
                    if(hasBeginVolumetricControls && (word == "#END_VOLUMETRIC_CONTROLS")){
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Volumetric controls"
                                  << std::right << std::setw(mediumColumn) << ".";
                        colorText("Loaded\n", GREEN);
                        // Volumetric controls complete
                        volumetricControls = true;
                        hasBeginVolumetricControls = false;
                        // Stop getting lines
                        break;
                    }
                }
            } // END VOLUMETRIC CONTROLS

            // PHYSICS MODEL
            busyElsewhere = (hasBeginJobSetup           ||
                             hasBeginRegions            ||
                             hasBeginMeshModel          ||
                             hasBeginVolumetricControls ||
                             hasBeginSolverOptions      ||
                             hasBeginStoppingCriteria);
            if(!physicsModel && !busyElsewhere){

                // Check #BEGIN_PHYSICS_MODEL tag
                if(hasBeginStarJob && (word == "#BEGIN_PHYSICS_MODEL"))
                    hasBeginPhysicsModel = true;

                // Check dynamic_viscosity
                if(hasBeginPhysicsModel && (word == "dynamic_viscosity")){
                    if(issLine >> word){
                        try {
                            dynamicViscosity = std::stod(word, nullptr);
                            // Check data
                            if(dynamicViscosity <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_DYNAMIC_VISCOSITY_INPUT);
                                throw "invalid argument for <dynamic_viscosity> (double > 0)";
                            }
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_DYNAMIC_VISCOSITY_INPUT);
                            throw "invalid argument for <dynamic_viscosity> (double > 0)";
                        }
//                        hasDynamicViscosity = true;
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_DYNAMIC_VISCOSITY_EMPTY);
                        throw "<dynamic_viscosity> is empty";
                    }
                }

                // Check static_temperature
                if(hasBeginPhysicsModel && word == ("static_temperature")){
                    if(issLine >> word){
                        try {
                            staticTemperature = std::stod(word, nullptr);
                            // Check data
                            if(staticTemperature <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_STATIC_TEMPERATURE_INPUT);
                                throw "invalid argument for <static_temperature> (double > 0)";
                            }
                            // Successful
                            hasStaticTemperature = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_STATIC_TEMPERATURE_INPUT);
                            throw "invalid argument for <static_temperature> (double > 0)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_STATIC_TEMPERATURE_EMPTY);
                        throw "<static_temperature> is empty";
                    }
                }

                // Check reference_pressure
                if(hasBeginPhysicsModel && (word == "reference_pressure")){
                    if(issLine >> word){
                        try {
                            referencePressure = std::stod(word, nullptr);
                            // Check data
                            if(referencePressure <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_PRESSURE_INPUT);
                                throw "invalid argument for <reference_pressure> (double > 0)";
                            }
                            // Successful
                            hasReferencePressure = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_PRESSURE_INPUT);
                            throw "invalid argument for <reference_pressure> (double > 0)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_PRESSURE_EMPTY);
                        throw "<reference_pressure> is empty";
                    }
                }

                // Check reference_velocity
                if(hasBeginPhysicsModel && (word == "reference_velocity")){
                    if(issLine >> word){
                        try {
                            referenceVelocity = std::stod(word, nullptr);
                            // Check data
                            if(referenceVelocity <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_VELOCITY_INPUT);
                                throw "invalid argument for <reference_velocity> (double > 0)";
                            }
                            // Successful
                            hasReferenceVelocity = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_VELOCITY_INPUT);
                            throw "invalid argument for <reference_velocity> (double > 0)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_VELOCITY_EMPTY);
                        throw "<reference_velocity> is empty";
                    }
                }

                // Check reference_density
                if(hasBeginPhysicsModel && (word == "reference_density")){
                    if(issLine >> word){
                        try {
                            referenceDensity = std::stod(word, nullptr);
                            // Check data
                            if(referenceDensity <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_DENSITY_INPUT);
                                throw "invalid argument for <reference_density> (double > 0)";
                            }
                            // Successful
                            hasReferenceDensity = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_DENSITY_INPUT);
                            throw "invalid argument for <reference_density> (double > 0)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_DENSITY_EMPTY);
                        throw "<reference_density> is empty";
                    }
                }

                // Check mach_number
                if(hasBeginPhysicsModel && (word == "mach_number")){
                    if(issLine >> word){
                        try {
                            machNumber = std::stod(word, nullptr);
                            // Check data
                            if(machNumber <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_MACH_NUMBER_INPUT);
                                throw "invalid argument for <mach_number> (double > 0)";
                            }
                            // Successful
                            hasMachNumber = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_MACH_NUMBER_INPUT);
                            throw "invalid argument for <mach_number> (double > 0)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_MACH_NUMBER_EMPTY);
                        throw "<mach_number> is empty";
                    }
                }

                // Check flow_direction_X
                if(hasBeginPhysicsModel && (word == "flow_direction_X")){
                    if(issLine >> word){
                        try {
                            flowDirectionX = std::stod(word, nullptr);
                            // Successful
                            hasFlowDirectionX = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_FLOW_DIRECTION_X_INPUT);
                            throw "invalid argument for <flow_direction_X>";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_FLOW_DIRECTION_X_EMPTY);
                        throw "<flow_direction_X> is empty";
                    }
                }

                // Check flow_direction_Y
                if(hasBeginPhysicsModel && (word == "flow_direction_Y")){
                    if(issLine >> word){
                        try {
                            flowDirectionY = std::stod(word, nullptr);
                            // Successful
                            hasFlowDirectionY = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_FLOW_DIRECTION_Y_INPUT);
                            throw "invalid argument for <flow_direction_Y>";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_FLOW_DIRECTION_Y_EMPTY);
                        throw "<flow_direction_Y> is empty";
                    }
                }

                // Check flow_direction_Z
                if(hasBeginPhysicsModel && (word == "flow_direction_Z")){
                    if(issLine >> word){
                        try {
                            flowDirectionZ = std::stod(word, nullptr);
                            // Successful
                            hasFlowDirectionZ = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_FLOW_DIRECTION_Z_INPUT);
                            throw "invalid argument for <flow_direction_Z>";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_FLOW_DIRECTION_Z_EMPTY);
                        throw "<flow_direction_Z> is empty";
                    }
                }

                // Check #END_PHYSICS_MODEL
                if(hasBeginPhysicsModel && (word == "#END_PHYSICS_MODEL")){
                    // Check physics model data is complete
                    if(hasStaticTemperature      &&
                            hasReferencePressure &&
                            hasReferenceVelocity &&
                            hasReferenceDensity  &&
                            hasMachNumber        &&
                            hasFlowDirectionX    &&
                            hasFlowDirectionY    &&
                            hasFlowDirectionZ){
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Physics model"
                                                       << std::right << std::setw(mediumColumn) << ".";
                        colorText("Loaded\n", GREEN);
                        // physics model complete
                        physicsModel = true;
                        hasBeginPhysicsModel = false;
                    } else{
                        // Missing data
                        if(!hasStaticTemperature){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_STATIC_TEMPERATURE_MISSING);
                            throw "<static_temperature> is missing";
                        }
                        if(!hasReferencePressure){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_PRESSURE_MISSING);
                            throw "<reference_pressure> is missing";
                        }
                        if(!hasReferenceVelocity){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_VELOCITY_MISSING);
                            throw "<reference_velocity> is missing";
                        }
                        if(!hasReferenceDensity){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REFERENCE_DENSITY_MISSING);
                            throw "<reference_density> is missing";
                        }
                        if(!hasMachNumber){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_MACH_NUMBER_MISSING);
                            throw "<mach_number> is missing";
                        }
                        if(!hasFlowDirectionX){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_FLOW_DIRECTION_X_MISSING);
                            throw "<flow_direction_X> is missing";
                        }
                        if(!hasFlowDirectionY){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_FLOW_DIRECTION_Y_MISSING);
                            throw "<flow_direction_Y> is missing";
                        }
                        if(!hasFlowDirectionZ){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_FLOW_DIRECTION_Z_MISSING);
                            throw "<flow_direction_Z> is missing";
                        }
                    }
                }
            }
            // END PHYSICS MODEL

            // SOLVER OPTION (OPTIONAL)
            busyElsewhere = (hasBeginJobSetup           ||
                             hasBeginRegions            ||
                             hasBeginMeshModel          ||
                             hasBeginVolumetricControls ||
                             hasBeginPhysicsModel       ||
                             hasBeginStoppingCriteria);
            if(!solverOptions && !busyElsewhere){

                // Check #BEGIN_SOLVER_OPTIONS tag
                if(hasBeginStarJob && (word == "#BEGIN_SOLVER_OPTIONS"))
                    hasBeginSolverOptions = true;

                // Check CFL
                if(hasBeginSolverOptions && (word == "CFL")){
                    if(issLine >> word){
                        try {
                            CFL = std::stod(word, nullptr);
                            // Check data
                            if(CFL <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SOLVER_OPTIONS_CFL_INPUT);
                                throw "invalid argument for <CFL> (double > 0)";
                            }
                            // Successful
                            hasCFL = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SOLVER_OPTIONS_CFL_INPUT);
                            throw "invalid argument for <CFL> (double > 0)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SOLVER_OPTIONS_CFL_EMPTY);
                        throw "<CFL> is empty";
                    }
                }

                if(hasBeginSolverOptions && (word == "#END_SOLVER_OPTIONS")){
                    // Check solver options data is complete
                    if(hasCFL){
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Solver options"
                                                       << std::right << std::setw(mediumColumn) << ".";
                        colorText("Loaded\n", GREEN);
                        // solver options complete
                        solverOptions = true;
                        hasBeginSolverOptions = false;
                        // Tell starJob we have a custom CFL
                        customSolverOptions = true;
                    } else {
                        // Missing data
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_SOLVER_OPTIONS_CFL_MISSING);
                        throw "<CFL> is missing";
                    }
                }
            }
            // END SOLVER OPTION

            // STOPPING CRITERIA
            busyElsewhere = (hasBeginJobSetup           ||
                             hasBeginRegions            ||
                             hasBeginMeshModel          ||
                             hasBeginVolumetricControls ||
                             hasBeginPhysicsModel       ||
                             hasBeginSolverOptions);
            if(!stoppingCriteria && !busyElsewhere){

                // Check #BEGIN_STOPPING_CRITERIA tag
                if(hasBeginStarJob && (word == "#BEGIN_STOPPING_CRITERIA"))
                    hasBeginStoppingCriteria = true;

                // Check max_steps
                if(hasBeginStoppingCriteria && (word == "max_steps")){
                    if(issLine >> word){
                        try {
                            maxSteps = std::stoi(word, nullptr);
                            // Check data
                            if(maxSteps <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_MAX_STEPS_INPUT);
                                throw "invalid argument for <max_steps> (int >= 1)";
                            }
                            // Successful
                            hasMaxSteps = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_MAX_STEPS_INPUT);
                            throw "invalid argument for <max_steps> (int >= 1)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_MAX_STEPS_EMPTY);
                        throw "<max_steps> is empty";
                    }
                }

                // Check num_samples
                if(hasBeginStoppingCriteria && (word == "num_samples")){
                    if(issLine >> word){
                        try {
                            numSamples = std::stoi(word, nullptr);
                            // Check data
                            if(numSamples <= 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_NUM_SAMPLES_INPUT);
                                throw "invalid argument for <num_samples> (int >= 1)";
                            }
                            // Successful
                            hasSamples = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_NUM_SAMPLES_INPUT);
                            throw "invalid argument for <num_samples> (int >= 1)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_NUM_SAMPLES_EMPTY);
                        throw "<num_samples> is empty";
                    }
                }

                // Check asymptotic_CL
                if(hasBeginStoppingCriteria && (word == "asymptotic_CL")){
                    if(issLine >> word){
                        try {
                            asymptoticCL = std::stod(word, nullptr);
                            // Successful
                            hasAsymptotycCL = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_ASYMPTOTIC_CL_INPUT);
                            throw "invalid argument for <asymptotic_CL> (double)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_ASYMPTOTIC_CL_EMPTY);
                        throw "<asymptotic_CL> is empty";
                    }
                }

                // Check asymptotic_CD
                if(hasBeginStoppingCriteria && (word == "asymptotic_CD")){
                    if(issLine >> word){
                        try {
                            asymptoticCD = std::stod(word, nullptr);
                            // Check data
                            if(asymptoticCD < 0){
                                g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_ASYMPTOTIC_CD_INPUT);
                                throw "invalid argument for <asymptotic_CD> (double >-= 0)";
                            }
                            // Successful
                            hasAsymptotycCD = true;
                        } catch (const std::invalid_argument &invalidArgument) {
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_ASYMPTOTIC_CD_INPUT);
                            throw "invalid argument for <asymptotic_CD> (double >-= 0)";
                        }
                    } else {
                        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_ASYMPTOTIC_CD_EMPTY);
                        throw "<asymptotic_CD> is empty";
                    }
                }

                // Check #END_STOPPING_CRITERIA tag
                if(hasBeginStoppingCriteria && (word == "#END_STOPPING_CRITERIA")){
                    // Check if stopping criteria data is complete
                    if(hasMaxSteps &&
                            // If has samples, it must have criteria for CL and/or CD
                            (
                                    (hasSamples && (hasAsymptotycCL || hasAsymptotycCD))    ||
                                    (!hasSamples && (!hasAsymptotycCL && !hasAsymptotycCD))
                            )
                            ){
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Stopping criteria"
                                                       << std::right << std::setw(mediumColumn) << ".";
                        colorText("Loaded\n", GREEN);
                        // Stopping criteria complete
                        stoppingCriteria = true;
                        hasBeginStoppingCriteria = false;
                    } else {
                        if(!hasMaxSteps){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_MAX_STEPS_MISSING);
                            throw "<max_steps> is missing";
                        }
                        if((hasSamples && (!hasAsymptotycCL || !hasAsymptotycCD))){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_ASYMPTOTIC_CL_OR_CD_MISSING);
                            throw "<asymptotic_CL> or <asymptotic_CD> is missing";
                        }
                        if((!hasSamples && (hasAsymptotycCL || hasAsymptotycCD))){
                            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_NUM_SAMPLES_MISSING);
                            throw "<num_samples> is missing";
                        }
                    }
                }
            }
            // END STOPPING CRITERIA
        }
        // Out of word loop
        if(hasEndStarJob)
            break;
    }

    // POST CHECKS

    // Check that job has #END_STAR_JOB tag
    if(!hasEndStarJob)
        std::cout << "WARNING: <star_jobData> is missing #END_STAR_JOB tag" << std::endl;

    // Check that all mandatory section have been loaded
    if(!jobSetup){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_JOB_SETUP_MISSING);
        throw "job setup is missing";}
    if(!regions){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REGIONS_SECTION_MISSING);
        throw "regions section is missing";}
    if(!meshModel && !hasInitialization){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_MESH_MODEL_MISSING);
        throw "mesh model is missing";
    }
    if(volumetricControls && !meshModel){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_MESH_MODEL_MISSING);
        throw "mesh model is missing";
    }
    if(!physicsModel){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_PHYSICAL_MODEL_MISSING);
        throw "physics model is missing";
    }
    if(!stoppingCriteria){
        g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_STOPPING_CRITERIA_MISSING);
        throw "stopping criteria is missing";
    }

    // Check newMesh
    if(hasInitialization && !meshModel && !volumetricControls)
        newMesh = false;

    if(newMesh && meshModel){
        // CHECK REGION NAME AND SURFACE NAME ARE THE SAME
        if(regionName != surfaceName){
            g_exitStatus = static_cast<int>(ExitCodes::FAILURE_JOB_FILE_REGION_SURFACE_NAMES_MISMATCH);
            throw "region names and surface names are not the same";
        }
    }
}

void StarJob::printTwoColumns(std::string _c1, std::string _c2) const {
    std::cout << std::setfill(' ') << std::right << std::setw(mediumColumn) << _c1
                                   << std::left << std::setw(mediumColumn)  << " " + _c2 << std::endl;
}

void StarJob::printTwoColumns(std::string _c1, std::string _c2, char _fill) const {
    std::cout << std::setfill(_fill) << std::right << std::setw(mediumColumn) << _c1
                                     << std::left << std::setw(mediumColumn)  << _c2 << std::endl;
}

void StarJob::printThreeColumns(std::string _c1, std::string _c2, std::string _c3) const {
    std::cout << std::setfill(' ') << std::left << std::setw(mediumColumn) << _c1
                                   << std::left << std::setw(mediumColumn) << _c2
                                   << std::left << std::setw(mediumColumn) << _c3 << std::endl;
}

void StarJob::printJobData(){
    std::cout << "\n:::::::::::: JOB "; colorText(jobName + "\n\n", WHITE_BLUE);

    // Job setup
    printTwoColumns(" SETUP", " ", '-');
    printTwoColumns("Client:", getClientJobDirectory());
    printTwoColumns("Server:", getServerJobDirectory());
    if(initializationJob != Default::initializationJob)
        printTwoColumns("Initialization:", initializationJob);
    printTwoColumns("Clean server:", cleanServer? "yes" : "no");
    printTwoColumns("Download sim file:", saveSimFile? "yes" : "no");
    printTwoColumns(" Auto save:", autoSaveSimulation? "yes" : "no");
    if(autoSaveSimulation){
        printTwoColumns("Auto save files:", (numAutoSaveFiles == 0)? "replace" : std::to_string(numAutoSaveFiles));
        printTwoColumns("Iteration interval:", std::to_string(iterationInterval));
    }
    std::cout << std::endl;

    // Physics data
    printTwoColumns(" PHYSICS", " ", '-');
    printTwoColumns("Type:", (dynamicViscosity > 0) ? "RANS" : "Inviscid");
    printTwoColumns("Mach =", std::to_string(machNumber));
    printTwoColumns("T (static) =", std::to_string(staticTemperature) + " K");
    printTwoColumns("V (ref) =", std::to_string(referenceVelocity) + " m/s");
    printTwoColumns("p (ref) =", javaScientific(referencePressure) + " Pa");
    printTwoColumns("rho (ref) =", javaScientific(referenceDensity) + " kg/m3");
    if(dynamicViscosity > 0)
        printTwoColumns("mu (air) =", javaScientific(dynamicViscosity) + " Pa.s");
    std::cout << std::endl;

    // Region data
    if(!surfaceSize.empty()){
        printTwoColumns(" REGIONS", " ", '-');
        printThreeColumns("Region", "Boundary", "Surface Size");
        for(int i = 0; i < regionName.size(); i++){
            printThreeColumns(regionName[i], boundaryCondition[i], std::to_string(surfaceSize[i]) + " m");
        }
        std::cout << std::endl;
    }

    // Prism layer data
    if(prismLayers > 0){
        printTwoColumns(" MESH", " ", '-');
        printTwoColumns("Prism layers:", std::to_string(prismLayers));
        printTwoColumns("PL thickness:", std::to_string(prismLayerThickness) + " m");
        printTwoColumns("Near wall thickness:", std::to_string(nearWallThickness) + " m");
    }
    std::cout << std::endl;

    // Volumetric controls
    if(!blockSurfaceSize.empty() || !cylinderSurfaceSize.empty())
        printTwoColumns("VOLUMETRIC CONTROLS", " ", '-');

    // Block parts
    if(!blockSurfaceSize.empty()){
        colorText("Block part(s)\n", AQUA);
        for(int i = 0; i < blockSurfaceSize.size(); i++){
            printTwoColumns("Block " + std::to_string(i + 1) + ":", "surface size = " +
                    std::to_string(blockSurfaceSize[i]) + " m");
            std::cout << "Corners" << std::endl;
            printThreeColumns("X [m]", "Y [m]", "Z [m]");
            printThreeColumns(std::to_string(blockX1[i]), std::to_string(blockY1[i]), std::to_string(blockZ1[i]));
            printThreeColumns(std::to_string(blockX2[i]), std::to_string(blockY2[i]), std::to_string(blockZ2[i]));
            std::cout << std::endl;
        }
    }

    // Cylinder parts
    if(!cylinderSurfaceSize.empty()){
        colorText("Cylinder part(s)\n", AQUA);
        for(int i = 0; i < cylinderSurfaceSize.size(); i++){
            printTwoColumns("Cylinder " + std::to_string(i + 1) + ":", "surface size = " +
                                                                    std::to_string(cylinderSurfaceSize[i]) + " m");
            printTwoColumns("", "      radius = " + std::to_string(cylinderRadius[i]) + " m");
            std::cout << "Axis" << std::endl;
            printThreeColumns("X [m]", "Y [m]", "Z [m]");
            printThreeColumns(std::to_string(cylinderX1[i]), std::to_string(cylinderY1[i]), std::to_string(cylinderZ1[i]));
            printThreeColumns(std::to_string(cylinderX2[i]), std::to_string(cylinderY2[i]), std::to_string(cylinderZ2[i]));
            std::cout << std::endl;
        }
    }

    // Cone parts
    if(!coneSurfaceSize.empty()){
        colorText("Cone part(s)\n", AQUA);
        for(int i = 0; i < coneSurfaceSize.size(); i++){
            printTwoColumns("Cone " + std::to_string(i + 1) + ":", "surface size = " +
                                                                       std::to_string(coneSurfaceSize[i]) + " m");
            printTwoColumns("", "    radius 1 = " + std::to_string(coneRadius1[i]) + " m");
            printTwoColumns("", "    radius 2 = " + std::to_string(coneRadius2[i]) + " m");
            std::cout << "Axis" << std::endl;
            printThreeColumns("X [m]", "Y [m]", "Z [m]");
            printThreeColumns(std::to_string(coneX1[i]), std::to_string(coneY1[i]), std::to_string(coneZ1[i]));
            printThreeColumns(std::to_string(coneX2[i]), std::to_string(coneY2[i]), std::to_string(coneZ2[i]));
            std::cout << std::endl;
        }
    }

    // Notes
    if(initializationJob != Default::initializationJob && !newMesh){
        colorText("Note: will not remesh model", YELLOW);
        std::cout << std::endl;
    }

    // If batch mode, just wait Default::pauseTime, otherwise expect user input
    if(!batchModeOption){
        std::cout << "\n Press "; colorText("<enter>", GREEN); std::cout << " to continue..." << std::endl;
        std::cin.get();
#ifdef _WIN32
        system("cls");
#endif
#if defined(linux) || defined(__APPLE__)
        system("clear");
#endif
    } else {
#ifdef _WIN32
        Sleep(Default::pauseTime);
        system("cls");
#endif
#if defined(linux) || defined(__APPLE__)
        std::this_thread::sleep_for(std::chrono::milliseconds(Default::pauseTime));
        system("clear");
#endif
    }
}