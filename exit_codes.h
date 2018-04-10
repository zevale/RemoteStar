#ifndef EXIT_CODES_H
#define EXIT_CODES_H

// Forward declaration of global variable for exit status shared by files handling exceptions
extern int g_exitStatus;


// ExitCodes used for error tracking and reporting through updates on "g_exitStatus"
enum class ExitCodes {
    SUCCESS = 10                                       ,
    FAILURE_COMMAND_LINE_USAGE                         , // Wrong command line syntax
    FAILURE_SSH_SECURE_SHEEL                           , // SSH: Cannot execute SSH command
    FAILURE_SSH_SECURE_SHEEL_SCREEN                    , // SSH: Cannot execute SSH screen
    FAILURE_SSH_SECURE_COPY                            , // SSH: Cannot execute SCP command
    FAILURE_OPEN_SSH_NOT_FOUND                         , // Windows OpenSSH not found
    FAILURE_JOB_FILE_CANNOT_OPEN                       , // Unable to open job file
    FAILURE_JOB_FILE_JOB_NAME_EMPTY                    , // Job file: <job_name> is empty
    FAILURE_JOB_FILE_JOB_NAME_MISSING                  , // Job file: <job_name> is missing
    FAILURE_JOB_FILE_INITIALIZATION_JOB_EMPTY          , // Job file: <initialization_job> is empty
    FAILURE_JOB_FILE_CLIENT_DIRECTORY_EMPTY            , // Job file: <client_directory> is empty
    FAILURE_JOB_FILE_CLIENT_DIRECTORY_MISSING          , // Job file: <client_directory> is missing
    FAILURE_JOB_FILE_SERVER_DIRECTORY_EMPTY            , // Job file: <server_directory> is empty
    FAILURE_JOB_FILE_SERVER_DIRECTORY_MISSING          , // Job file: <server_directory> is missing
    FAILURE_JOB_FILE_SAVE_SIM_FILE_USAGE               , // Job file: usage <save_sim_file> yes/no
    FAILURE_JOB_FILE_SAVE_SIM_FILE_EMPTY               , // Job file: <save_sim_file> is empty
    FAILURE_JOB_FILE_SAVE_SIM_FILE_MISSING             , // Job file: <save_sim_file> is missing
    FAILURE_JOB_FILE_CLEAN_SERVER_USAGE                , // Job file: usage <clean_server> yes/no
    FAILURE_JOB_FILE_CLEAN_SERVER_EMPTY                , // Job file: <clean_server> is empty
    FAILURE_JOB_FILE_CLEAN_SERVER_MISSING              , // Job file: <clean_server> is missing
    FAILURE_JOB_FILE_AUTO_SAVE_USAGE                   , // Job file: usage <auto_save> yes/no
    FAILURE_JOB_FILE_AUTO_SAVE_EMPTY                   , // Job file: <auto_save> is empty,
    FAILURE_JOB_FILE_AUTO_SAVE_FILES_INPUT             , // Job file: invalid argument for <auto_save_files> (int >= 0)
    FAILURE_JOB_FILE_AUTO_SAVE_FILES_EMPTY             , // Job file: <auto_save_files> is empty
    FAILURE_JOB_FILE_AUTO_SAVE_FILES_MISSING           , // Job file: <auto_save_files> is missing
    FAILURE_JOB_FILE_AUTO_SAVE_ITERATIONS_INPUT        , // Job file: invalid argument for <auto_save_iterations> (int >= 1)
    FAILURE_JOB_FILE_AUTO_SAVE_ITERATIONS_EMPTY        , // Job file: <auto_save_iterations> is empty
    FAILURE_JOB_FILE_AUTO_SAVE_ITERATIONS_MISSING      , // Job file: <auto_save_iterations> is missing
    FAILURE_JOB_FILE_WALL_REGION_MISSING_NAME          , // Job file: wall region is missing name
    FAILURE_JOB_FILE_WALL_REGION_MISSING               , // Job file: wall region is missing
    FAILURE_JOB_FILE_FREE_STREAM_REGION_MISSING_NAME   , // Job file: free stream region is missing name
    FAILURE_JOB_FILE_FREE_STREAM_REGION_MISSING        , // Job file: free stream region is missing
    FAILURE_JOB_FILE_SYMMETRY_PLANE_REGION_MISSING_NAME, // Job file: symmetry plane region is missing name
    FAILURE_JOB_FILE_BASE_SIZE_INPUT                   , // Job file: invalid argument for <base_size> (double > 0)
    FAILURE_JOB_FILE_BASE_SIZE_EMPTY                   , // Job file: <base_size> is empty
    FAILURE_JOB_FILE_NUM_PRISM_LAYER_INPUT             , // Job file: invalid argument for <num_prism_layers> (int >= 1)
    FAILURE_JOB_FILE_NUM_PRISM_LAYER_MISSING           , // Job file: <num_prism_layers> >= 1
    FAILURE_JOB_FILE_PRISM_LAYER_EMPTY                 , // Job file: <num_prism_layers> is missing
    FAILURE_JOB_FILE_PRISM_LAYER_THICKNESS_INPUT       , // Job file: invalid argument for <prism_layer_thickness> (double > 0)
    FAILURE_JOB_FILE_PRISM_LAYER_THICKNESS_EMPTY       , // Job file: <prism_layer_thickness> is empty
    FAILURE_JOB_FILE_PRISM_LAYER_THICKNESS_MISSING     , // Job file: <prism_layer_thickness> is missing
    FAILURE_JOB_FILE_NEAR_WALL_THICKNESS_INPUT         , // Job file: invalid argument for <near_wall_thickness> (double > 0)
    FAILURE_JOB_FILE_NEAR_WALL_THICKNESS_EMPTY         , // Job file: <near_wall_thickness> is empty
    FAILURE_JOB_FILE_NEAR_WALL_THICKNESS_MISSING       , // Job file: <near_wall_thickness> is missing
    FAILURE_JOB_FILE_SURFACE_SIZE_VALUE_INPUT          , // Job file: invalid argument for <surface_size> (double > 0)
    FAILURE_JOB_FILE_SURFACE_SIZE_VALUE_EMPTY          , // Job file: <surface_size> value is empty
    FAILURE_JOB_FILE_SURFACE_SIZE_NAME_EMPTY           , // Job file: <surface_size> name is empty
    FAILURE_JOB_FILE_SURFACE_SIZE_ENTRIES              , // Job file: <surface_size> needs at least two entries
    FAILURE_JOB_FILE_BLOCK_CORNER_1_X1_INPUT           , // Job file: invalid argument for X in <corner_1> (double)
    FAILURE_JOB_FILE_BLOCK_CORNER_1_Y1_INPUT           , // Job file: invalid argument for Y in <corner_1> (double)
    FAILURE_JOB_FILE_BLOCK_CORNER_1_Z1_INPUT           , // Job file: invalid argument for Z in <corner_1> (double)
    FAILURE_JOB_FILE_BLOCK_CORNER_1_MISSING_DATA       , // Job file: block <corner_1> is missing data
    FAILURE_JOB_FILE_BLOCK_CORNER_1_MISSING            , // Job file: block <corner_1> is missing
    FAILURE_JOB_FILE_BLOCK_CORNER_2_X2_INPUT           , // Job file: invalid argument for X in <corner_2> (double)
    FAILURE_JOB_FILE_BLOCK_CORNER_2_Y2_INPUT           , // Job file: invalid argument for Y in <corner_2> (double)
    FAILURE_JOB_FILE_BLOCK_CORNER_2_Z2_INPUT           , // Job file: invalid argument for Z in <corner_2> (double)
    FAILURE_JOB_FILE_BLOCK_CORNER_2_MISSING_DATA       , // Job file: block <corner_2> is missing data
    FAILURE_JOB_FILE_BLOCK_CORNER_2_MISSING            , // Job file: block <corner_2> is missing
    FAILURE_JOB_FILE_BLOCK_SURFACE_SIZE_INPUT          , // Job file: invalid argument for block <surface_size> (double > 0)
    FAILURE_JOB_FILE_BLOCK_SURFACE_SIZE_EMPTY          , // Job file: block <surface_size> is empty
    FAILURE_JOB_FILE_BLOCK_SURFACE_SIZE_MISSING        , // Job file: block <surface_size> is missing
    FAILURE_JOB_FILE_CYLINDER_BASE_1_X1_INPUT          , // Job file: invalid argument for X in cylinder <base_1> (double)
    FAILURE_JOB_FILE_CYLINDER_BASE_1_Y1_INPUT          , // Job file: invalid argument for Y in cylinder <base_1> (double)
    FAILURE_JOB_FILE_CYLINDER_BASE_1_Z1_INPUT          , // Job file: invalid argument for Z in cylinder <base_1> (double)
    FAILURE_JOB_FILE_CYLINDER_BASE_1_MISSING_DATA      , // Job file: cylinder <base_1> is missing data
    FAILURE_JOB_FILE_CYLINDER_BASE_1_MISSING           , // Job file: cylinder <base_1> is missing
    FAILURE_JOB_FILE_CYLINDER_BASE_2_X1_INPUT          , // Job file: invalid argument for X in cylinder <base_2> (double)
    FAILURE_JOB_FILE_CYLINDER_BASE_2_Y1_INPUT          , // Job file: invalid argument for Y in cylinder <base_2> (double)
    FAILURE_JOB_FILE_CYLINDER_BASE_2_Z1_INPUT          , // Job file: invalid argument for Z in cylinder <base_2> (double)
    FAILURE_JOB_FILE_CYLINDER_BASE_2_MISSING_DATA      , // Job file: cylinder <base_2> is missing data
    FAILURE_JOB_FILE_CYLINDER_BASE_2_MISSING           , // Job file: cylinder <base_2> is missing
    FAILURE_JOB_FILE_CYLINDER_RADIUS_INPUT             , // Job file: invalid argument for cylinder <radius> (double > 0)
    FAILURE_JOB_FILE_CYLINDER_RADIUS_EMPTY             , // Job file: cylinder <radius> is empty
    FAILURE_JOB_FILE_CYLINDER_RADIUS_MISSING           , // Job file: cylinder <radius> is missing
    FAILURE_JOB_FILE_CYLINDER_SURFACE_SIZE_INPUT       , // Job file: invalid argument for cylinder <surface_size> (double > 0)
    FAILURE_JOB_FILE_CYLINDER_SURFACE_SIZE_EMPTY       , // Job file: cylinder <surface_size> is empty
    FAILURE_JOB_FILE_CYLINDER_SURFACE_SIZE_MISSING     , // Job file: cylinder <surface_size> is missing
    FAILURE_JOB_FILE_CONE_BASE_1_X1_INPUT              , // Job file: invalid argument for X in cone <base_1> (double)
    FAILURE_JOB_FILE_CONE_BASE_1_Y1_INPUT              , // Job file: invalid argument for Y in cone <base_1> (double)
    FAILURE_JOB_FILE_CONE_BASE_1_Z1_INPUT              , // Job file: invalid argument for Z in cone <base_1> (double)
    FAILURE_JOB_FILE_CONE_BASE_1_MISSING_DATA          , // Job file: cone <base_1> is missing data
    FAILURE_JOB_FILE_CONE_BASE_1_MISSING               , // Job file: cone <base_1> is missing
    FAILURE_JOB_FILE_CONE_BASE_2_X2_INPUT              , // Job file: invalid argument for X in cone <base_2> (double)
    FAILURE_JOB_FILE_CONE_BASE_2_Y2_INPUT              , // Job file: invalid argument for Y in cone <base_2> (double)
    FAILURE_JOB_FILE_CONE_BASE_2_Z2_INPUT              , // Job file: invalid argument for Z in cone <base_2> (double)
    FAILURE_JOB_FILE_CONE_BASE_2_MISSING_DATA          , // Job file: cone <base_2> is missing data
    FAILURE_JOB_FILE_CONE_BASE_2_MISSING               , // Job file: cone <base_2> is missing
    FAILURE_JOB_FILE_CONE_RADIUS_1_INPUT               , // Job file: invalid argument for cone <radius_1> (double > 0)
    FAILURE_JOB_FILE_CONE_RADIUS_1_EMPTY               , // Job file: cone <radius_1> is empty
    FAILURE_JOB_FILE_CONE_RADIUS_1_MISSING             , // Job file: cone <radius_1> is missing
    FAILURE_JOB_FILE_CONE_RADIUS_2_INPUT               , // Job file: invalid argument for cone <radius_2> (double > 0)
    FAILURE_JOB_FILE_CONE_RADIUS_2_EMPTY               , // Job file: cone <radius_2> is empty
    FAILURE_JOB_FILE_CONE_RADIUS_2_MISSING             , // Job file: cone <radius_2> is missing
    FAILURE_JOB_FILE_CONE_SURFACE_SIZE_INPUT           , // Job file: invalid argument for cone <surface_size> (double > 0)
    FAILURE_JOB_FILE_CONE_SURFACE_SIZE_EMPTY           , // Job file: cone <surface_size> is empty
    FAILURE_JOB_FILE_CONE_SURFACE_SIZE_MISSING         , // Job file: cone <surface_size> is missing
    FAILURE_JOB_FILE_DYNAMIC_VISCOSITY_INPUT           , // Job file: invalid argument for <dynamic_viscosity> (double > 0)
    FAILURE_JOB_FILE_DYNAMIC_VISCOSITY_EMPTY           , // Job file: <dynamic_viscosity> is empty
    FAILURE_JOB_FILE_STATIC_TEMPERATURE_INPUT          , // Job file: invalid argument for <static_temperature> (double > 0)
    FAILURE_JOB_FILE_STATIC_TEMPERATURE_EMPTY          , // Job file: <static_temperature> is empty
    FAILURE_JOB_FILE_STATIC_TEMPERATURE_MISSING        , // Job file: <static_temperature> is missing
    FAILURE_JOB_FILE_REFERENCE_PRESSURE_INPUT          , // Job file: invalid argument for <reference_pressure> (double > 0)
    FAILURE_JOB_FILE_REFERENCE_PRESSURE_EMPTY          , // Job file: <reference_pressure> is empty
    FAILURE_JOB_FILE_REFERENCE_PRESSURE_MISSING        , // Job file: <reference_pressure> is missing
    FAILURE_JOB_FILE_REFERENCE_VELOCITY_INPUT          , // Job file: invalid argument for <reference_velocity> (double > 0)
    FAILURE_JOB_FILE_REFERENCE_VELOCITY_EMPTY          , // Job file: <reference_velocity> is empty
    FAILURE_JOB_FILE_REFERENCE_VELOCITY_MISSING        , // Job file: <reference_velocity> is missing
    FAILURE_JOB_FILE_REFERENCE_DENSITY_INPUT           , // Job file: invalid argument for <reference_density> (double > 0)
    FAILURE_JOB_FILE_REFERENCE_DENSITY_EMPTY           , // Job file: <reference_density> is empty
    FAILURE_JOB_FILE_REFERENCE_DENSITY_MISSING         , // Job file: <reference_density> is missing
    FAILURE_JOB_FILE_MACH_NUMBER_INPUT                 , // Job file: invalid argument for <mach_number> (double > 0)
    FAILURE_JOB_FILE_MACH_NUMBER_EMPTY                 , // Job file: <mach_number> is empty
    FAILURE_JOB_FILE_MACH_NUMBER_MISSING               , // Job file: <mach_number> is missing
    FAILURE_JOB_FILE_FLOW_DIRECTION_X_INPUT            , // Job file: invalid argument for <flow_direction_X>
    FAILURE_JOB_FILE_FLOW_DIRECTION_X_EMPTY            , // Job file: <flow_direction_X> is empty
    FAILURE_JOB_FILE_FLOW_DIRECTION_X_MISSING          , // Job file: <flow_direction_X> is missing
    FAILURE_JOB_FILE_FLOW_DIRECTION_Y_INPUT            , // Job file: invalid argument for <flow_direction_Y>
    FAILURE_JOB_FILE_FLOW_DIRECTION_Y_EMPTY            , // Job file: <flow_direction_Y> is empty
    FAILURE_JOB_FILE_FLOW_DIRECTION_Y_MISSING          , // Job file: <flow_direction_Y> is missing
    FAILURE_JOB_FILE_FLOW_DIRECTION_Z_INPUT            , // Job file: invalid argument for <flow_direction_Z>
    FAILURE_JOB_FILE_FLOW_DIRECTION_Z_EMPTY            , // Job file: <flow_direction_Z> is empty
    FAILURE_JOB_FILE_FLOW_DIRECTION_Z_MISSING          , // Job file: <flow_direction_Z> is missing
    FAILURE_JOB_FILE_SOLVER_OPTIONS_CFL_INPUT          , // Job file: invalid argument for <CFL> (double > 0)
    FAILURE_JOB_FILE_SOLVER_OPTIONS_CFL_EMPTY          , // Job file: <CFL> is empty
    FAILURE_JOB_FILE_SOLVER_OPTIONS_CFL_MISSING        , // Job file: <CFL> is missing
    FAILURE_JOB_FILE_MAX_STEPS_INPUT                   , // Job file: invalid argument for <max_steps> (int >= 1)
    FAILURE_JOB_FILE_MAX_STEPS_EMPTY                   , // Job file: <max_steps> is empty
    FAILURE_JOB_FILE_MAX_STEPS_MISSING                 , // Job file: <max_steps> is missing
    FAILURE_JOB_FILE_NUM_SAMPLES_INPUT                 , // Job file: invalid argument for <num_samples> (int >= 1)
    FAILURE_JOB_FILE_NUM_SAMPLES_EMPTY                 , // Job file: <num_samples> is empty
    FAILURE_JOB_FILE_NUM_SAMPLES_MISSING               , // Job file: <num_samples> is missing
    FAILURE_JOB_FILE_ASYMPTOTIC_CL_INPUT               , // Job file: invalid argument for <asymptotic_CL> (double)
    FAILURE_JOB_FILE_ASYMPTOTIC_CL_EMPTY               , // Job file: <asymptotic_CL> is empty
    FAILURE_JOB_FILE_ASYMPTOTIC_CD_INPUT               , // Job file: invalid argument for <asymptotic_CD> (double >-= 0)
    FAILURE_JOB_FILE_ASYMPTOTIC_CD_EMPTY               , // Job file: <asymptotic_CD> is empty
    FAILURE_JOB_FILE_ASYMPTOTIC_CL_OR_CD_MISSING       , // Job file: <asymptotic_CL> or <asymptotic_CD> is missing
    FAILURE_JOB_FILE_JOB_SETUP_MISSING                 , // Job file: job setup is missing
    FAILURE_JOB_FILE_REGIONS_SECTION_MISSING           , // Job file: regions section is missing
    FAILURE_JOB_FILE_MESH_MODEL_MISSING                , // Job file: mesh model is missing
    FAILURE_JOB_FILE_PHYSICAL_MODEL_MISSING            , // Job file: Job file: physical is missing
    FAILURE_JOB_FILE_STOPPING_CRITERIA_MISSING         , // Job file: stopping criteria is missing
    FAILURE_JOB_FILE_REGION_SURFACE_NAMES_MISMATCH     , // Job file: region names and surface names are not the same
    FAILURE_AIRCRAFT_GEOMETRY_NOT_FOUND                , // Resources: aircraft geometry not found
    FAILURE_DOMAIN_GEOMETRY_NOT_FOUND                  , // Resources: domain geometry not found
    FAILURE_SSH_SERVER_CANNOT_OPEN                     , // SSH server: cannot open file <star_sshServer>
    FAILURE_SSH_SERVER_ALIAS_MISSING                   , // SSH server: missing <alias> in file <star_sshServer>
    FAILURE_SSH_SERVER_ADDRESS_MISSING                 , // SSH server: missing <address> in file <star_sshServer>
    FAILURE_SSH_SERVER_USAGE                           , // SSH server: wrong syntax in file <star_sshServer>
    FAILURE_SSH_SERVER_NO_SERVER                       , // SSH server: missing server in file <star_sshServer>
    FAILURE_SSH_SERVER_USER_MISSING                    , // SSH server: missing <user> in file <star_sshServer>
    FAILURE_HOST_LIST_CANNOT_OPEN                      , // Host list: cannot open file <star_hostList>
    FAILURE_STAR_HOST_SHELL_SCRIPT_CANNOT_WRITE        , // StarHost: cannot write shell script <star_runScript>
    FAILURE_STAR_HOST_ALIAS_MISSING                    , // StarHost: missing alias in file <star_hostList>
    FAILURE_STAR_HOST_ADDRESS_MISSING                  , // StarHost: missing address in file <star_hostList>
    FAILURE_STAR_HOST_TYPE_MISSING                     , // StarHost: missing type in file <star_hostList>
    FAILURE_STAR_HOST_TYPE_MISSING_USAGE               , // StarHost: host type in file <star_hostList> must be localhost or remote
    FAILURE_STAR_HOST_LOCALHOST_NOT_UNIQUE             , // StarHost: more than one localhost in file <star_hostList>
    FAILURE_STAR_HOST_NO_LOCALHOST                     , // StarHost: no localhost in file <star_hostList>
    FAILURE_STAR_HOST_PROCESSES_MISSING                , // StarHost: missing processes in file <star_hostList>
    FAILURE_STAR_HOST_PROCESSES_INPUT                  , // StarHost: invalid argument for processes in file <star_hostList> (int >= 1)
    FAILURE_STAR_HOST_NO_HOST                          , // StarHost: missing host in file <star_hostList>
    FAILURE_STAR_HOST_USAGE                            , // StarHost: wrong syntax in file <star_hostList>
    FAILURE_RESULTS_FORCES_UNABLE_TO_FETCH             , // Results: unable to fetch Forces.csv from server
    FAILURE_RESULTS_SIM_UNABLE_TO_FETCH                , // Results: unable to fetch sim file from server
    FAILURE_RESULTS_FORCES_SIM_UNABLE_TO_FETCH         , // Results: unable to fetch forces and sim file from server
};
#endif //EXIT_CODES_H
