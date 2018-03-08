#include "StarJob.h"
#include "MightyMacroMaker/MightyConstants.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
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

/*
 * Constructor that sets the job file path assuming  <star_jobData>
 * is in the current directory by default
 */
StarJob::StarJob(const std::string& _jobFilePath, bool _batchMode) :
        jobFilePath(_jobFilePath),
        batchModeOption(_batchMode),

        // Auto save defaults (might not be assigned by user)
        autoSaveSimulation(Default::autoSaveSimulation),
        numAutoSaveFiles(Default::numAutosaveFiles),
        iterationInterval(Default::iterationInterval) {}

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

    // PARSER AUXILIARY VARIABLES (flags)

    // Star job tags
    bool hasBeginStarJob    = false;
    bool hasEndStarJob      = false;

    // Setup options
    bool hasBeginJobSetup      = false;
    bool hasJobName            = false;
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

    // Physics model options
    bool hasBeginPhysicsModel = false;
    bool hasDynamicViscosity  = false;
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

    // Job status options
    bool jobSetup         = false;
    bool meshModel        = false;
    bool physicsModel     = false;
    bool solverOptions    = false;
    bool stoppingCriteria = false;
    bool regions          = false;

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
            busyElsewhere = (hasBeginRegions       ||
                             hasBeginMeshModel     ||
                             hasBeginPhysicsModel  ||
                             hasBeginSolverOptions ||
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
                    } else
                        throw "<job_name> is empty";
                }

                // Check for client_directory
                if(hasBeginJobSetup && (word == "client_directory")){
                    if(issLine >> word) {
                        clientDirectory = word;
                        hasClientDirectory = true;
                    } else
                        throw "<client_directory> is empty";
                }

                // Check for server_directory
                if(hasBeginJobSetup && (word == "server_directory")){
                    if(issLine >> word){
                        serverDirectory = word;
                        hasServerDirectory = true;
                    } else
                        throw "<server_directory> is empty";
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
                        } else
                            throw "usage <save_sim_file> yes/no";
                    } else
                        throw "<save_sim_file> is empty";
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
                        } else
                            throw "usage <clean_server> yes/no";
                    } else
                        throw "<clean_server> is empty";
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
                        } else
                            throw "usage <auto_save> yes/no";
                    } else
                        throw "<auto_save> is empty";
                }

                // Check auto_save_files
                if(hasBeginJobSetup && (word == "auto_save_files")){
                    if(issLine >> word){
                        numAutoSaveFiles = std::stoi(word, nullptr);
                        hasAutoSaveFiles = true;
                    } else
                        throw "<auto_save_files> is empty";
                }

                // Check auto_save_iterations
                if(hasBeginJobSetup && (word == "auto_save_iterations")){
                    if(issLine >> word){
                        iterationInterval = std::stoi(word, nullptr);
                        hasAutoSaveIterations = true;
                    } else
                        throw "<auto_save_iterations> is empty";
                }

                // Check #END_JOB_SETUP
                if(hasBeginJobSetup && (word == "#END_JOB_SETUP")){
                    // Check if job setup data is complete
                    if(hasJobName && hasClientDirectory && hasServerDirectory && hasSaveSimFile && hasCleanServer
                            && (hasAutoSave && hasAutoSaveFiles && hasAutoSaveIterations || !hasAutoSave)) {
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Job setup"
                                                       << std::right << std::setw(mediumColumn) << "Loaded" << std::endl;
                        // jobSetup complete
                        jobSetup = true;
                        hasBeginJobSetup = false;
                    } else {
                        // Missing data
                        if(!hasJobName)
                            throw "<job_name> is missing";
                        if(!hasServerDirectory)
                            throw "<server_directory> is missing";
                        if(!hasClientDirectory)
                            throw "<client_directory> is missing";
                        if(!hasSaveSimFile)
                            throw "<save_sim_file> is missing";
                        if(!hasCleanServer)
                            throw "<clean_server> is missing";
                        if(hasAutoSave && !hasAutoSaveFiles)
                            throw "<auto_save_files> is missing";
                        if(hasAutoSave && !hasAutoSaveIterations)
                            throw "<auto_save_iterations> is missing";
                    }
                }
            }
            // END JOB SETUP

            busyElsewhere = (hasBeginJobSetup      ||
                             hasBeginMeshModel     ||
                             hasBeginPhysicsModel  ||
                             hasBeginSolverOptions ||
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
                    } else
                        throw "wall region is missing name";
                }

                // Check free stream region
                if(hasBeginRegions && (word == "free_stream")){
                    boundaryCondition.emplace_back("freeStream");
                    if(issLine >> word){
                        regionName.push_back(word);
                        hasOneFreeStream = true;
                    } else
                        throw "free stream region is missing name";
                }

                // Check symmetry plane region
                if(hasBeginRegions && (word == "symmetry_plane")){
                    boundaryCondition.emplace_back("symmetryPlane");
                    if(issLine >> word)
                        regionName.push_back(word);
                    else
                        throw "symmetry plane region is missing";;
                }

                // Check #END_REGIONS
                if(hasBeginRegions && (word == "#END_REGIONS")){
                    // Check if regions data is complete
                    if(hasOneWall && hasOneFreeStream){
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Regions"
                                                       << std::right << std::setw(mediumColumn) << "Loaded" << std::endl;
                        // regions setup complete
                        regions = true;
                        hasBeginRegions = false;
                    } else {
                        // Missing data
                        if(!hasOneWall)
                            throw "wall region is missing";
                        if(!hasOneFreeStream)
                            throw "free stream region is missing";
                    }
                }
            }
            // END REGIONS

            // MESH MODEL
            busyElsewhere = (hasBeginJobSetup      ||
                             hasBeginRegions       ||
                             hasBeginPhysicsModel  ||
                             hasBeginSolverOptions ||
                             hasBeginStoppingCriteria);
            if(!meshModel && !busyElsewhere){

                // Check #BEGIN_MESH_MODEL tag
                if(hasBeginStarJob && (word == "#BEGIN_MESH_MODEL"))
                    hasBeginMeshModel = true;

                // Check base_size
                if(hasBeginMeshModel && (word == "base_size")){
                    if(issLine >> word){
                        baseSize = std::stod(word, nullptr);
                        hasBaseSize = true;
                    } else
                        throw "<base_size> is empty";
                }

                // Check num_prism_layers
                if(hasBeginMeshModel && (word == "num_prism_layers")){
                    if(issLine >> word){
                        prismLayers = std::stoi(word, nullptr);
                        hasPrismLayers = true;
                    } else
                        throw "<num_prism_layers> is empty";
                }

                // Check prism_layer_thickness
                if(hasBeginMeshModel && (word == "prism_layer_thickness")){
                    if(issLine >> word){
                        prismLayerThickness = std::stod(word, nullptr);
                        hasPrismLayerThickness = true;
                    } else
                        throw "<prism_layer_thickness> is empty";
                }

                // Check near_wall_thickness
                if(hasBeginMeshModel && (word == "near_wall_thickness")){
                    if(issLine >> word){
                        nearWallThickness = std::stod(word, nullptr);
                        hasNearWallThickness = true;
                    } else
                        throw "<near_wall_thickness> is empty";
                }

                // Check surface_size
                if(hasBeginMeshModel && (word == "surface_size")){
                    // First get the surface name
                    if(issLine >> word){
                        surfaceName.push_back(word);
                        // Then get surface size
                        if(issLine >> word){
                            surfaceSize.push_back(stod(word, nullptr));
                        } else
                            throw "<surface_size> has no surface size";
                    } else
                        throw "<surface_size> has no name";

                    // Check if at leas two surface sizes have been entered (aircraft + freestream)
                    if(surfaceSize.size() == 2)
                        hasTwoSurfaceSizes = true;
                }

                // Check #END_MESH_MODEL tag
                if(hasBeginMeshModel && (word == "#END_MESH_MODEL")){
                    // Check mesh model data is complete
                    if(hasBaseSize &&
                            hasPrismLayers         &&
                            hasPrismLayerThickness &&
                            hasNearWallThickness   &&
                            hasTwoSurfaceSizes){
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Mesh model"
                                                       << std::right << std::setw(mediumColumn) << "Loaded" << std::endl;
                        // mesh model complete
                        meshModel = true;
                        hasBeginMeshModel = false;
                    } else {
                        // Missing data
                        if(!hasPrismLayers)
                            throw "<num_prism_layers> is missing";
                        if(!hasPrismLayerThickness)
                            throw "<prism_layer_thickness> is missing";
                        if(!hasNearWallThickness)
                            throw "<near_wall_thickness> is missing";
                        if(!hasTwoSurfaceSizes)
                            throw "needs at least two surface sizes";
                    }
                }

            }
            // END MESH MODEL

            // PHYSICS MODEL
            busyElsewhere = (hasBeginJobSetup      ||
                             hasBeginRegions       ||
                             hasBeginMeshModel     ||
                             hasBeginSolverOptions ||
                             hasBeginStoppingCriteria);
            if(!physicsModel && !busyElsewhere){

                // Check #BEGIN_PHYSICS_MODEL tag
                if(hasBeginStarJob && (word == "#BEGIN_PHYSICS_MODEL"))
                    hasBeginPhysicsModel = true;

                // Check dynamic_viscosity
                if(hasBeginPhysicsModel && (word == "dynamic_viscosity")){
                    if(issLine >> word){
                        dynamicViscosity = std::stod(word, nullptr);
                        hasDynamicViscosity = true;
                    } else
                        throw "<dynamic_viscosity> is empty";
                }

                // Check static_temperature
                if(hasBeginPhysicsModel && word == ("static_temperature")){
                    if(issLine >> word){
                        staticTemperature = std::stod(word, nullptr);
                        hasStaticTemperature = true;
                    } else
                        throw "<static_temperature> is empty";
                }

                // Check reference_pressure
                if(hasBeginPhysicsModel && (word == "reference_pressure")){
                    if(issLine >> word){
                        referencePressure = std::stod(word, nullptr);
                        hasReferencePressure = true;
                    } else
                        throw "<reference_pressure> is empty";
                }

                // Check reference_velocity
                if(hasBeginPhysicsModel && (word == "reference_velocity")){
                    if(issLine >> word){
                        referenceVelocity = std::stod(word, nullptr);
                        hasReferenceVelocity = true;
                    } else
                        throw "<reference_velocity> is empty";
                }

                // Check reference_density
                if(hasBeginPhysicsModel && (word == "reference_density")){
                    if(issLine >> word){
                        referenceDensity = std::stod(word, nullptr);
                        hasReferenceDensity = true;
                    } else
                        throw "<reference_density> is empty";
                }

                // Check mach_number
                if(hasBeginPhysicsModel && (word == "mach_number")){
                    if(issLine >> word){
                        machNumber = std::stod(word, nullptr);
                        hasMachNumber = true;
                    } else
                        throw "<mach_number> is missing";
                }

                // Check flow_direction_X
                if(hasBeginPhysicsModel && (word == "flow_direction_X")){
                    if(issLine >> word){
                        flowDirectionX = std::stod(word, nullptr);
                        hasFlowDirectionX = true;
                    } else
                        throw "<flow_direction_X> is missing";
                }

                // Check flow_direction_Y
                if(hasBeginPhysicsModel && (word == "flow_direction_Y")){
                    if(issLine >> word){
                        flowDirectionY = std::stod(word, nullptr);
                        hasFlowDirectionY = true;
                    } else
                        throw "<flow_direction_Y> is missing";
                }

                // Check flow_direction_Z
                if(hasBeginPhysicsModel && (word == "flow_direction_Z")){
                    if(issLine >> word){
                        flowDirectionZ = std::stod(word, nullptr);
                        hasFlowDirectionZ = true;
                    } else
                        throw "<flow_direction_Z> is missing";
                }

                // Check #END_PHYSICS_MODEL
                if(hasBeginPhysicsModel && (word == "#END_PHYSICS_MODEL")){
                    // Check physics model data is complete
                    if(hasDynamicViscosity       &&
                            hasStaticTemperature &&
                            hasReferencePressure &&
                            hasReferenceVelocity &&
                            hasReferenceDensity  &&
                            hasMachNumber        &&
                            hasFlowDirectionX    &&
                            hasFlowDirectionY    &&
                            hasFlowDirectionZ){
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Physics model"
                                                       << std::right << std::setw(mediumColumn) << "Loaded" << std::endl;
                        // physics model complete
                        physicsModel = true;
                        hasBeginPhysicsModel = false;
                    } else{
                        // Missing data
                        if(!hasDynamicViscosity)
                            throw "<dynamic_viscosity> is missing";
                        if(!hasStaticTemperature)
                            throw "<static_temperature> is missing";
                        if(!hasReferencePressure)
                            throw "<reference_pressure> is missing";
                        if(!hasReferenceVelocity)
                            throw "<reference_velocity> is missing";
                        if(!hasReferenceDensity)
                            throw "<reference_density> is missing";
                        if(!hasMachNumber)
                            throw "<mach_number> is missing";
                        if(!hasFlowDirectionX)
                            throw "<flow_direction_X> is missing";
                        if(!hasFlowDirectionY)
                            throw "<flow_direction_Y> is missing";
                        if(!hasFlowDirectionZ)
                            throw "<flow_direction_Z> is missing";
                    }
                }
            }
            // END PHYSICS MODEL

            // SOLVER OPTION (OPTIONAL)
            busyElsewhere = (hasBeginJobSetup     ||
                             hasBeginRegions      ||
                             hasBeginMeshModel    ||
                             hasBeginPhysicsModel ||
                             hasBeginStoppingCriteria);
            if(!solverOptions && !busyElsewhere){

                // Check #BEGIN_SOLVER_OPTIONS tag
                if(hasBeginStarJob && (word == "#BEGIN_SOLVER_OPTIONS"))
                    hasBeginSolverOptions = true;

                // Check CFL
                if(hasBeginSolverOptions && (word == "CFL")){
                    if(issLine >> word){
                        CFL = std::stod(word, nullptr);
                        hasCFL = true;
                    } else
                        throw "<CFL> is empty";
                }

                if(hasBeginSolverOptions && (word == "#END_SOLVER_OPTIONS")){
                    // Check solver options data is complete
                    if(hasCFL){
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Solver options"
                                                       << std::right << std::setw(mediumColumn) << "Loaded" << std::endl;
                        // solver options complete
                        solverOptions = true;
                        hasBeginSolverOptions = false;
                        // Tell starJob we have a custom CFL
                        customSolverOptions = true;
                    } else {
                        // Missing data
                        throw "<CFL> is missing";
                    }
                }
            }
            // END SOLVER OPTION

            // STOPPING CRITERIA
            busyElsewhere = (hasBeginJobSetup     ||
                             hasBeginRegions      ||
                             hasBeginMeshModel    ||
                             hasBeginPhysicsModel ||
                             hasBeginSolverOptions);
            if(!stoppingCriteria && !busyElsewhere){

                // Check #BEGIN_STOPPING_CRITERIA tag
                if(hasBeginStarJob && (word == "#BEGIN_STOPPING_CRITERIA"))
                    hasBeginStoppingCriteria = true;

                // Check max_steps
                if(hasBeginStoppingCriteria && (word == "max_steps")){
                    if(issLine >> word){
                        maxSteps = std::stoi(word, nullptr);
                        hasMaxSteps = true;
                    } else
                        throw "<max_steps> is empty";
                }

                // Check num_samples
                if(hasBeginStoppingCriteria && (word == "num_samples")){
                    if(issLine >> word){
                        numSamples = std::stoi(word, nullptr);
                        hasSamples = true;
                    } else
                        throw "<num_samples> is empty";
                }

                // Check asymptotic_CL
                if(hasBeginStoppingCriteria && (word == "asymptotic_CL")){
                    if(issLine >> word){
                        asymptoticCL = std::stod(word, nullptr);
                        hasAsymptotycCL = true;
                    } else
                        throw "<asymptotic_CL> is empty";
                }

                // Check #END_STOPPING_CRITERIA tag
                if(hasBeginStoppingCriteria && (word == "#END_STOPPING_CRITERIA")){
                    // Check if stopping criteria data is complete
                    if(hasMaxSteps && hasSamples && hasAsymptotycCL){
                        std::cout << std::setfill('.') << std::left  << std::setw(largeColumn) << "Stopping criteria"
                                                       << std::right << std::setw(mediumColumn) << "Loaded" << std::endl;
                        // Stopping criteria complete
                        stoppingCriteria = true;
                        hasBeginStoppingCriteria = false;
                    } else {
                        if(!hasMaxSteps)
                            throw "<max_steps> is missing";
                        if(!hasSamples)
                            throw "<num_samples> is missing";
                        if(!hasAsymptotycCL)
                            throw "<asymptotic_CL> is missing";
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
    if(!jobSetup)
        throw "job setup is missing";
    if(!regions)
        throw "regions' section is missing";
    if(!meshModel)
        throw "mesh model is missing";
    if(!stoppingCriteria)
        throw "stopping criteria is missing";

    // CHECK REGION NAME AND SURFACE NAME ARE THE SAME
    if(regionName != surfaceName)
        std::cerr << "ERROR: Region names and surface names are not the same" << std::endl;
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
    std::cout << "\n:::::::::::: JOB " + jobName + "\n" << std::endl;

    printTwoColumns(" SETUP", " ", '-');
    printTwoColumns("Client:", getClientJobDirectory());
    printTwoColumns("Server:", getServerJobDirectory());
    printTwoColumns("Clean server:", cleanServer? "yes" : "no");
    printTwoColumns("Download sim file:", saveSimFile? "yes" : "no");
    printTwoColumns(" Auto save:", autoSaveSimulation? "yes" : "no");
    if(autoSaveSimulation){
        printTwoColumns("Auto save files:", std::to_string(numAutoSaveFiles));
        printTwoColumns("Iteration interval:", std::to_string(iterationInterval));
    }
    std::cout << std::endl;

    printTwoColumns(" REGIONS", " ", '-');
    printThreeColumns("Region", "Boundary", "Surface Size");
    for(int i = 0; i < getRegionName().size(); i++){
        printThreeColumns(regionName[i], boundaryCondition[i], std::to_string(surfaceSize[i]) + " m");
    }
    std::cout << std::endl;

    printTwoColumns(" MESH", " ", '-');
    printTwoColumns("Prism layers:", std::to_string(prismLayers));
    printTwoColumns("PL thickness:", std::to_string(prismLayerThickness) + " m");
    printTwoColumns("Near wall thickness:", std::to_string(nearWallThickness) + " m");

    // If batch mode, just wait Default::pauseTime, otherwise expect user input
    if(!batchModeOption){
        std::cout << "\n Press <enter> to continue..." << std::endl;
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
        system("clear");
#endif
    }
}