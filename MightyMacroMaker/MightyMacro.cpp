#include "MightyMacro.h"

/*
 * CONSTRUCTOR
 *
 * NOTES
 * Assignment inside constructor requires constructor + destructor + assignment
 * Initializer list requires constructor + destructor (faster)
 */
MightyMacro::MightyMacro(StarJob *_currentStarJob): currentStarJob (_currentStarJob) {
    // Macro file will be written to [clientJobDirectory]/resources/
    macroFilePath = currentStarJob->getClientJobDirectory("resources/MightyMacro.java");

    // Get ofstream object to be able to write the macro
    macroFile = openMightyMacroFile();

    // Import assignment
    import = Import({Default::boundaryMarchAngle,
                     Default::minimumThickness,
                     Default::layerChoppingPercentage,
                     currentStarJob->getPrismLayers(),
                     currentStarJob->getPrismLayerThickness(),
                     currentStarJob->getNearWallThickness()});

    // Auto save assignment
    autoSave = AutoSave(currentStarJob->getServerJobDirectory(currentStarJob->getJobName() + ".sim"),
                        Default::autoSaveMesh,
                        currentStarJob->getAutoSaveSimulation(),
                        currentStarJob->getNumAutoSaveFiles(),
                        currentStarJob->getIterationInterval());

    // Aircraft assignment (geometry file path on the server)
    aircraft = Aircraft(currentStarJob->getServerJobDirectory("resources/SurfMesh.stl"));

    // Domain assignment (geometry file path on the server, region name, boundary condition)
    domain = Domain(currentStarJob->getServerJobDirectory("resources/DomainGeometry.x_b"));

    // Mesh continuum assignment (prism layer)
    meshContinuum = MeshContinuum({Default::boundaryMarchAngle,
                                   Default::minimumThickness,
                                   Default::layerChoppingPercentage,
                                   currentStarJob->getPrismLayers(),
                                   currentStarJob->getPrismLayerThickness(),
                                   currentStarJob->getNearWallThickness()});

    // Show domain assignment
    showDomain = ShowDomain(currentStarJob->getRegionName(),
                            currentStarJob->getBoundaryCondition());

    // MeshValues assignment (base size, prism layer, surface size, region name, boundary conditions)
    meshValues = MeshValues(currentStarJob->getBaseSize(),
                            // Prism layer with default config values and loaded data
                            {Default::boundaryMarchAngle,
                             Default::minimumThickness,
                             Default::layerChoppingPercentage,
                             currentStarJob->getPrismLayers(),
                             currentStarJob->getPrismLayerThickness(),
                             currentStarJob->getNearWallThickness()},
                            currentStarJob->getSurfaceSize(),
                            // Region names and boundary conditions
                            currentStarJob->getRegionName(),
                            currentStarJob->getBoundaryCondition());

    // Physics values assignment (mach, viscosity, ref. pressure, static temp., flow direction, velocity components)
    physicsValues = PhysicsValues(currentStarJob->getMachNumber(),
                                  currentStarJob->getDynamicViscosity(),
                                  currentStarJob->getReferencePressure(),
                                  currentStarJob->getStaticTemperature(),
                                  // Flow direction
                                  {currentStarJob->getFlowDirectionX(),
                                   currentStarJob->getFlowDirectionY(),
                                   currentStarJob->getFlowDirectionZ()},
                                  // Velocity components computed from flow direction and reference velocity
                                  {(currentStarJob->getFlowDirectionX())*(currentStarJob->getReferenceVelocity()),
                                   (currentStarJob->getFlowDirectionY())*(currentStarJob->getReferenceVelocity()),
                                   (currentStarJob->getFlowDirectionZ())*(currentStarJob->getReferenceVelocity())},
                                  // Region names and boundary conditions
                                  currentStarJob->getRegionName(),
                                  currentStarJob->getBoundaryCondition());

    // Solver options assignment (CFL) (if not provided => CFL = 30/Mach)
    solverOptions = ((currentStarJob->getCustomSolverOptions())?
                     SolverOptions(currentStarJob->getCFL()) : SolverOptions(30.0/(currentStarJob->getMachNumber())));

    // Solution monitor assignment (ref. density, ref, velocity, lift direction, drag direction, viewUp, normal)
    solutionMonitors = SolutionMonitors(currentStarJob->getReferenceDensity(),
                                        currentStarJob->getReferenceVelocity(),
                                        // Lift is flow direction (assuming no slip) rotates 90 deg (-ez, 0, ex)
                                        {-(currentStarJob->getFlowDirectionZ()),
                                         0,
                                         currentStarJob->getFlowDirectionX()},
                                        // Drag direction and flow direction are the same
                                        {currentStarJob->getFlowDirectionX(),
                                         currentStarJob->getFlowDirectionY(),
                                         currentStarJob->getFlowDirectionZ()},
                                        // ViewUp uses the standard configuration [c.f. MightyConstants.h]
                                        {Default::viewUpX, Default::viewUpY, Default::viewUpZ},
                                        // Normal uses the standard configuration [c.f. MightyConstants.h]
                                        {Default::normalX, Default::normalY, Default::normalZ},
                                        // Boundary condition
                                        currentStarJob->getBoundaryCondition());

    // Stopping criteria assignment (maxSteps, number of samples for asymptotic convergence, asymptotic convergence)
    stoppingCriteria = StoppingCriteria(currentStarJob->getMaxSteps(),
                                        currentStarJob->getNumSamples(),
                                        currentStarJob->getAsymptoticCL());

    // Export results assignment (results file path on the server)
    exportResults = ExportResults(currentStarJob->getServerJobDirectory("Forces.csv"));

    // Close sim assignment (sim file path on the server)
    closeSim = CloseSim(currentStarJob->getServerJobDirectory(currentStarJob->getJobName()) + ".sim");
}

/*
 * writeMacro()
 *
 * DESCRIPTION
 * Uses all write functions to write the entire macro
 */
void MightyMacro::writeMacro() {
    writeImport();
    beginStarMacro();
    writeExecute();
    writeAutoSave();
    writeAircraft();
    writeDomain();
    writeRegion();
    writeMeshContinuum();
    writePhysicsContinuum();
    writeShowDomain();
    writeMeshValues();
    writePhysicsValues();
    writeSolverOptions();
    writeSolutionMonitors();
    writeStoppingCriteria();
    writeGenerateMesh();
    writeRunSimulation();
    writeExportResults();
    writeCloseSim();
    endStarMacro();
}

std::ofstream MightyMacro::openMightyMacroFile() {
    // Open file in macroFilePath
    std::ofstream mightyMacroFile(macroFilePath.c_str());
    if(!mightyMacroFile)
        perror("ERROR: cannot open macro file");

    return mightyMacroFile;
}

void MightyMacro::writeToFile(const std::vector<std::string>& _code){
    for(const std::string& codeLine : _code)
        macroFile << codeLine << std::endl;
}

void MightyMacro::writeImport() {
    writeToFile(import.importCode());
}

void MightyMacro::beginStarMacro() {
    std::vector<std::string> code;
    code = {
            "// StarMacro is implemented by STAR-CCM+",
            "public class MightyMacro extends StarMacro {",
    };

    // Write to macro
    writeToFile(code);
}

void MightyMacro::endStarMacro() {
    // Closing bracket
    macroFile << "}";

    // Close file
    macroFile.close();
}

void MightyMacro::writeExecute() {
    std::vector<std::string> code;

    // Execute code
    code = {
            "    public void execute(){",
            "        autoSaveSimulation();",
            "        importAircraftGeometry();",
            "        importDomainGeometry();",
            "        generateAirDomain();",
            "        generateRegions();",
            "        meshContinuum();",
            "        physicsContinuum();",
            "        showDomain();",
            "        meshValues();",
            "        physicsValues();",
            "        solverOptions();",
            "        solutionMonitors();",
            "        stoppingCriteria();",
            "        generateMesh();",
            "        runSimulation();",
            "        exportResults();",
            "        closeSimulation();",
            "    }"
    };

    // Write execute code to macro
    writeToFile(code);
}

void MightyMacro::writeAutoSave() {
    writeToFile(autoSave.autoSaveCode());
}

void MightyMacro::writeAircraft() {
    writeToFile(aircraft.aircraftCode());
}

void MightyMacro::writeDomain() {
    writeToFile(domain.domainCode());
}

void MightyMacro::writeRegion() {
    std::vector<std::string> code;
    code = {
            "",
            "    private void generateRegions(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Rename \"Region\" to \"Air\"",
            "        Region regionAir = activeSimulation.getRegionManager().createEmptyRegion();",
            "        regionAir.setPresentationName(\"Air\");",
            "",
            "        // Remove the \"Default\" boundary",
            "        Boundary boundaryDefault = regionAir.getBoundaryManager().getBoundary(\"Default\");",
            "        regionAir.getBoundaryManager().removeObjects(boundaryDefault);",
            "",
            "        // Remove the \"Default feature\" curves",
            "        FeatureCurve featureCurveDefault = ((FeatureCurve) regionAir.getFeatureCurveManager().getObject(\"Default\"));",
            "        regionAir.getFeatureCurveManager().removeObjects(featureCurveDefault);",
            "",
            "        // Assign part \"Subtract\" to region \"Air\"",
            "        MeshOperationPart meshOperationObj = ((MeshOperationPart) activeSimulation.get(SimulationPartManager.class).getPart(\"Subtract\"));",
            "        activeSimulation.getRegionManager().newRegionsFromParts(new NeoObjectVector(new Object[] {meshOperationObj}), \"OneRegion\", regionAir, \"OneBoundaryPerPartSurface\", null, \"OneFeatureCurvePerPartCurve\", null, RegionManager.CreateInterfaceMode.BOUNDARY);",
            "    }",
    };
    writeToFile(code);
}

void MightyMacro::writeMeshContinuum() {
    writeToFile(meshContinuum.meshContinuumCode());
}

void MightyMacro::writePhysicsContinuum() {
    writeToFile(physicsContinuum.physicsContinuumCode());
}

void MightyMacro::writeShowDomain() {
    writeToFile(showDomain.showDomainCode());
}

void MightyMacro::writeMeshValues() {
    writeToFile(meshValues.meshValuesCode());
}

void MightyMacro::writePhysicsValues() {
    writeToFile(physicsValues.physicsValuesCode());
}

void MightyMacro::writeSolverOptions() {
    writeToFile(solverOptions.solverOptionsCode());
}

void MightyMacro::writeSolutionMonitors() {
    writeToFile(solutionMonitors.solutionMonitorsCode());
}

void MightyMacro::writeStoppingCriteria() {
    writeToFile(stoppingCriteria.stoppingCriteriaCode());
}

void MightyMacro::writeGenerateMesh() {
    std::vector<std::string> code;
    code = {
            "",
            "    private void generateMesh(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Mesh pipeline controller",
            "        MeshPipelineController meshPipelineControllerObj = activeSimulation.get(MeshPipelineController.class);",
            "",
            "        // Generate volume mesh",
            "        meshPipelineControllerObj.generateVolumeMesh();",
            "",
            "        // Optional: show mesh",
            "        activeSimulation.getSceneManager().createGeometryScene(\"Mesh Scene\", \"Outline\", \"Mesh\", 3);",
            "    }"
    };
    writeToFile(code);
}

void MightyMacro::writeRunSimulation() {
    std::vector<std::string> code;
    code = {
            "",
            "    private void runSimulation(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Optional",
            "        ResidualPlot residualPlotObj = ((ResidualPlot) activeSimulation.getPlotManager().getPlot(\"Residuals\"));",
            "        residualPlotObj.open();",
            "",
            "        // Run",
            "        activeSimulation.getSimulationIterator().run();",
            "    }"
    };
    writeToFile(code);
}

void MightyMacro::writeExportResults() {
    writeToFile(exportResults.exportResultsCode());
}

void MightyMacro::writeCloseSim() {
    writeToFile(closeSim.closeSimCode());
}