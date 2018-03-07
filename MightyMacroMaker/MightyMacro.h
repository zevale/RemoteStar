/*
 * CLASS MightyMacro
 *
 * DESCRIPTION
 *
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 *
 */
#ifndef MIGHTYMACRO_H
#define MIGHTYMACRO_H

#include <string>
#include <fstream>
#include <vector>
#include "MightyConstants.h"
#include "../StarJob.h"
#include "Import.h"
#include "Aircraft.h"
#include "Domain.h"
#include "MeshContinuum.h"
#include "PhysicsContinuum.h"
#include "ShowDomain.h"
#include "MeshValues.h"
#include "PhysicsValues.h"
#include "SolverOptions.h"
#include "SolutionMonitors.h"
#include "StoppingCriteria.h"
#include "ExportResults.h"
#include "CloseSim.h"

class MightyMacro {
private:
    std::string      macroFilePath;    // File path to the macro
    std::ofstream    macroFile;        // To write

    StarJob          *currentStarJob;
    Import           import;
    Aircraft         aircraft;
    Domain           domain;
    MeshContinuum    meshContinuum;
    PhysicsContinuum physicsContinuum;
    ShowDomain       showDomain;
    MeshValues       meshValues;
    PhysicsValues    physicsValues;
    SolverOptions    solverOptions;
    SolutionMonitors solutionMonitors;
    StoppingCriteria stoppingCriteria;
    ExportResults    exportResults;
    CloseSim         closeSim;

public:
    // Constructor
    explicit MightyMacro(StarJob *_currentStarJob = nullptr);

    // Member functions
    void writeMacro();

private:
    void          writeToFile(const std::vector<std::string>& _code);
    std::ofstream openMightyMacroFile();
    void          writeImport();
    void          beginStarMacro();
    void          endStarMacro();
    void          writeExecute();
    void          writeAircraft();
    void          writeDomain();
    void          writeRegion();
    void          writeMeshContinuum();
    void          writePhysicsContinuum();
    void          writeShowDomain();
    void          writeMeshValues();
    void          writePhysicsValues();
    void          writeSolverOptions();
    void          writeSolutionMonitors();
    void          writeStoppingCriteria();
    void          writeGenerateMesh();
    void          writeRunSimulation();
    void          writeExportResults();
    void          writeCloseSim();

};
#endif //MIGHTYMACRO_H
