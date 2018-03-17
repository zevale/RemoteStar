cmake_minimum_required(VERSION 3.9)
project(RemoteStar)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_executable(RemoteStar
        main.cpp
        StarJob.h
        StarJob.cpp
        SSH.h
        SSH.cpp
        StarHost.h
        StarHost.cpp
        star_client.h
        star_client.cpp
        MightyMacroMaker/AutoSave.h
        MightyMacroMaker/AutoSave.cpp
        MightyMacroMaker/Aircraft.h
        MightyMacroMaker/Aircraft.cpp
        MightyMacroMaker/CloseSim.h
        MightyMacroMaker/CloseSim.cpp
        MightyMacroMaker/Domain.h
        MightyMacroMaker/Domain.cpp
        MightyMacroMaker/ExportResults.h
        MightyMacroMaker/ExportResults.cpp
        MightyMacroMaker/Import.h
        MightyMacroMaker/Import.cpp
        MightyMacroMaker/MeshContinuum.h
        MightyMacroMaker/MeshContinuum.cpp
        MightyMacroMaker/MeshValues.h
        MightyMacroMaker/MeshValues.cpp
        MightyMacroMaker/MightyConstants.h
        MightyMacroMaker/MightyMacro.h
        MightyMacroMaker/MightyMacro.cpp
        MightyMacroMaker/MightyMath.h
        MightyMacroMaker/MightyMath.cpp
        MightyMacroMaker/MightyTypes.h
        MightyMacroMaker/PhysicsContinuum.h
        MightyMacroMaker/PhysicsContinuum.cpp
        MightyMacroMaker/PhysicsValues.h
        MightyMacroMaker/PhysicsValues.cpp
        MightyMacroMaker/ShowDomain.h
        MightyMacroMaker/ShowDomain.cpp
        MightyMacroMaker/SolutionMonitors.h
        MightyMacroMaker/SolutionMonitors.cpp
        MightyMacroMaker/SolverOptions.h
        MightyMacroMaker/SolverOptions.cpp
        MightyMacroMaker/StoppingCriteria.h
        MightyMacroMaker/StoppingCriteria.cpp
        MightyMacroMaker/VolumetricControls.cpp
        MightyMacroMaker/VolumetricControls.h)