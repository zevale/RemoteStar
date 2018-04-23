/*
 * CLASS MightyMacro
 *
 * DESCRIPTION
 * Main class for the high-level macro API
 * This class needs a properly initialized StarJob instance to be constructed
 * To write a macro call method writeMacro on the MightyMacro instance
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
#include "AutoSave.h"
#include "Aircraft.h"
#include "Domain.h"
#include "MeshContinuum.h"
#include "VolumetricControls.h"
#include "PhysicsContinuum.h"
#include "ShowDomain.h"
#include "MeshValues.h"
#include "PhysicsValues.h"
#include "SolverOptions.h"
#include "SolutionMonitors.h"
#include "StoppingCriteria.h"
#include "ExportResults.h"
#include "CloseSim.h"
#include "MightyScene.h"

class MightyMacro {
private:
    std::string      macroFilePath;             // File path to the macro
    std::ofstream    macroFile;                 // Handle to write macro
    bool             hasInitialization;         // An initialization file has been provided
    bool             newMesh;                   // Check the need to mesh

    StarJob            *currentStarJob;         // StarJob instance parsed by star_client
    Import             import;                  // Section: import macro packages
    AutoSave           autoSave;                // Section: autosave settings
    Aircraft           aircraft;                // Section: import aircraft geometry
    Domain             domain;                  // Section: import domain geometry
    MeshContinuum      meshContinuum;           // Section: mesh models
    PhysicsContinuum   physicsContinuum;        // Section: physics models
    ShowDomain         showDomain;              // Section: generate geometry displayer
    MeshValues         meshValues;              // Section: mesh surface size and prism layer settings
    VolumetricControls volumetricControls;      // Section: create volumetric controls
    PhysicsValues      physicsValues;           // Section: initial and boundary conditions
    SolverOptions      solverOptions;           // Section: grid sequencing initialization, expert driver, CFL
    SolutionMonitors   solutionMonitors;        // Section: monitors for aerodynamic coefficients and loads
    MightyScene        mightyScene;             // Section: Cp and Mach contours
    StoppingCriteria   stoppingCriteria;        // Section: max steps and asymptotic stopping criteria
    ExportResults      exportResults;           // Section: pressure (and friction) forces
    CloseSim           closeSim;                // Section: save and close

public:
    // Constructor
    explicit MightyMacro(StarJob *_currentStarJob);

    // Member function
    void writeMacro();

private:
    std::ofstream openMightyMacroFile();
    void          writeToFile(const std::vector<std::string>& _code);
    void          writeImport();
    void          beginStarMacro();
    void          endStarMacro();
    void          writeExecute();
    void          writeAutoSave();
    void          writeAircraft();
    void          writeDomain();
    void          writeRegion();
    void          writeMeshContinuum();
    void          writePhysicsContinuum();
    void          writeShowDomain();
    void          writeMeshValues();
    void          writeVolumetricControls();
    void          writePhysicsValues();
    void          writeSolverOptions();
    void          writeSolutionMonitors();
    void          writeStoppingCriteria();
    void          writeGenerateMesh();
    void          writeMightyScene();
    void          writeRunSimulation();
    void          writeExportResults();
    void          writeCloseSim();
    void          writeSimCleanup();

};
#endif //MIGHTYMACRO_H
