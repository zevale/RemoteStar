#include "SolutionMonitors.h"
#include "MightyMath.h"

SolutionMonitors::SolutionMonitors(double                          _referenceDensity,
                                   double                          _referenceVelocity,
                                   const LiftDirection&            _liftDirection,
                                   const DragDirection&            _dragDirection,
                                   const ViewUp&                   _viewUp,
                                   const Normal&                   _normal,
                                   const std::vector<std::string>& _boundaryCondition) :
        referenceDensity (_referenceDensity),
        referenceVelocity(_referenceVelocity),
        liftDirection    (_liftDirection),
        dragDirection    (_dragDirection),
        viewUp           (_viewUp),
        normal           (_normal),
        boundaryCondition(_boundaryCondition) {}

std::vector<std::string> SolutionMonitors::solutionMonitorsCode() {
    std::vector<std::string> code;
    std::vector<std::string> codeBuffer;
    code = {
            "",
            "    private void solutionMonitors(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Data",
            "        double liftDirectionX         = " + javaScientific(liftDirection.X) + ";",
            "        double liftDirectionY         = " + javaScientific(liftDirection.Y) + ";",
            "        double liftDirectionZ         = " + javaScientific(liftDirection.Z) + ";",
            "        double dragDirectionX         = " + javaScientific(dragDirection.X) + ";",
            "        double dragDirectionY         = " + javaScientific(dragDirection.Y) + ";",
            "        double dragDirectionZ         = " + javaScientific(dragDirection.Z) + ";",
            "        double coordinateViewUpX      = " + javaScientific(viewUp.X) + ";",
            "        double coordinateViewUpY      = " + javaScientific(viewUp.Y) + ";",
            "        double coordinateViewUpZ      = " + javaScientific(viewUp.Z) + ";",
            "        double coordinateNormalX      = " + javaScientific(normal.X) + ";",
            "        double coordinateNormalY      = " + javaScientific(normal.Y) + ";",
            "        double coordinateNormalZ      = " + javaScientific(normal.Z) + ";",
            "        double valueReferenceDensity  = " + javaScientific(referenceDensity) + ";",
            "        double valueReferenceVelocity = " + javaScientific(referenceVelocity) + ";",
            "",
            "        // Get region object",
            "        Region regionAir = activeSimulation.getRegionManager().getRegion(\"Air\");",
            "        // Get aircraft object",
            "        Boundary boundaryAircraft = regionAir.getBoundaryManager().getBoundary(\"Subtract.SurfMesh.Surface\");",
            "",
            "        // LIFT FORCE REPORT",
            "        // Create report",
            "        ForceReport forceReportLift = activeSimulation.getReportManager().createReport(ForceReport.class);",
            "        // Force direction",
            "        forceReportLift.getDirection().setComponents(liftDirectionX, liftDirectionY, liftDirectionZ);",
            "        // Part selection",
            "        forceReportLift.getParts().setQuery(null);",
            "        forceReportLift.getParts().setObjects(boundaryAircraft);",
            "        // Optional: presentation name",
            "        forceReportLift.setPresentationName(\"Lift\");",
            "",
            "        // DRAG FORCE REPORT",
            "        ForceReport forceReportDrag = activeSimulation.getReportManager().createReport(ForceReport.class);",
            "        forceReportDrag.getDirection().setComponents(dragDirectionX, dragDirectionY, dragDirectionZ);",
            "        forceReportDrag.getParts().setQuery(null);",
            "        forceReportDrag.getParts().setObjects(boundaryAircraft);",
            "        forceReportDrag.setPresentationName(\"Drag\");",
            "",
            "        // FRONTAL AREA REPORT - for force coefficients",
            "        // Create report",
            "        FrontalAreaReport frontalAreaReportAircraft = activeSimulation.getReportManager().createReport(FrontalAreaReport.class);",
            "        // Get view up and normal coordinate objects",
            "        Coordinate coordinateViewUp = frontalAreaReportAircraft.getViewUpCoordinate();",
            "        Coordinate coordinateNormal = frontalAreaReportAircraft.getNormalCoordinate();",
            "        // Get length units",
            "        Units unitsLength = ((Units) activeSimulation.getUnitsManager().getObject(\"m\"));",
            "        // Set view up and normal coordinates",
            "        coordinateViewUp.setCoordinate(unitsLength, unitsLength, unitsLength, new DoubleVector(new double[] {coordinateViewUpX, coordinateViewUpY, coordinateViewUpZ}));",
            "        coordinateNormal.setCoordinate(unitsLength, unitsLength, unitsLength, new DoubleVector(new double[] {coordinateNormalX, coordinateNormalY, coordinateNormalZ}));",
            "        // Set parts - Aircraft surface mesh part",
            "        frontalAreaReportAircraft.getParts().setQuery(null);",
            "        MeshPart meshPartAircraft = ((MeshPart) activeSimulation.get(SimulationPartManager.class).getPart(\"SurfMesh\"));",
            "        PartSurface partSurfaceAircraft = ((PartSurface) meshPartAircraft.getPartSurfaceManager().getPartSurface(\"Surface\"));",
            "        frontalAreaReportAircraft.getParts().setObjects(partSurfaceAircraft);",
            "        // Get the reference area",
            "        double valueReferenceArea = frontalAreaReportAircraft.getReportMonitorValue();"
    };

    // Check for symmetry, in which case reference area is half
    for(const std::string& currentBoundaryCondition : boundaryCondition){
        if(currentBoundaryCondition == "symmetryPlane") {
            codeBuffer = {
                    "        // ONLY BECAUSE THE FLOW IS SYMMETRIC",
                    "        valueReferenceArea = valueReferenceArea/2;"
            };
            code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());
            break;
        };
    }

    codeBuffer = {
            "        // OPTIONAL",
            "        frontalAreaReportAircraft.printReport();",
            "        frontalAreaReportAircraft.setPresentationName(\"AircraftArea\");",
            "",
            "        // LIFT COEFFICIENT REPORT",
            "        // Create report",
            "        ForceCoefficientReport forceCoefficientReportLift = activeSimulation.getReportManager().createReport(ForceCoefficientReport.class);",
            "        // Set force direction",
            "        forceCoefficientReportLift.getDirection().setComponents(liftDirectionX, liftDirectionY, liftDirectionZ);",
            "        // Reference values",
            "        forceCoefficientReportLift.getReferenceDensity().setValue(valueReferenceDensity);",
            "        forceCoefficientReportLift.getReferenceVelocity().setValue(valueReferenceVelocity);",
            "        forceCoefficientReportLift.getReferenceArea().setValue(valueReferenceArea);",
            "        // Set parts",
            "        forceCoefficientReportLift.getParts().setQuery(null);",
            "        forceCoefficientReportLift.getParts().setObjects(boundaryAircraft);",
            "        // OPTIONAL",
            "        forceCoefficientReportLift.setPresentationName(\"CL\");",
            "",
            "        // DRAG COEFFICIENT REPORT",
            "        ForceCoefficientReport forceCoefficientReportDrag = activeSimulation.getReportManager().createReport(ForceCoefficientReport.class);",
            "        forceCoefficientReportDrag.getDirection().setComponents(dragDirectionX, dragDirectionY, dragDirectionZ);",
            "        // Reference values",
            "        forceCoefficientReportDrag.getReferenceDensity().setValue(valueReferenceDensity);",
            "        forceCoefficientReportDrag.getReferenceVelocity().setValue(valueReferenceVelocity);",
            "        forceCoefficientReportDrag.getReferenceArea().setValue(valueReferenceArea);",
            "        // Set parts",
            "        forceCoefficientReportDrag.getParts().setQuery(null);",
            "        forceCoefficientReportDrag.getParts().setObjects(boundaryAircraft);",
            "        // OPTIONAL",
            "        forceCoefficientReportDrag.setPresentationName(\"CD\");",

            "        // MONITORS",
            "        // Monitor: lift",
            "        activeSimulation.getMonitorManager().createMonitorAndPlot(new NeoObjectVector(new Object[] {forceReportLift}), true, \"%1$s Plot\");",
            "        ReportMonitor reportMonitorLift = ((ReportMonitor) activeSimulation.getMonitorManager().getMonitor(\"Lift Monitor\"));",
            "        // Monitor: drag",
            "        activeSimulation.getMonitorManager().createMonitorAndPlot(new NeoObjectVector(new Object[] {forceReportDrag}), true, \"%1$s Plot\");",
            "        ReportMonitor reportMonitorDrag = ((ReportMonitor) activeSimulation.getMonitorManager().getMonitor(\"Drag Monitor\"));",
            "        // Monitor: lift coefficient",
            "        activeSimulation.getMonitorManager().createMonitorAndPlot(new NeoObjectVector(new Object[] {forceCoefficientReportLift}), true, \"%1$s Plot\");",
            "        ReportMonitor reportMonitorCL = ((ReportMonitor) activeSimulation.getMonitorManager().getMonitor(\"CL Monitor\"));",
            "        // Monitor: drag coefficient",
            "        activeSimulation.getMonitorManager().createMonitorAndPlot(new NeoObjectVector(new Object[] {forceCoefficientReportDrag}), true, \"%1$s Plot\");",
            "        ReportMonitor reportMonitorCD = ((ReportMonitor) activeSimulation.getMonitorManager().getMonitor(\"CD Monitor\"));",
            "",
            "        // MONITOR PLOTS",
            "        MonitorPlot monitorPlotLift = activeSimulation.getPlotManager().createMonitorPlot(new NeoObjectVector(new Object[] {reportMonitorLift}), \"Lift Monitor\");",
            "        MonitorPlot monitorPlotDrag = activeSimulation.getPlotManager().createMonitorPlot(new NeoObjectVector(new Object[] {reportMonitorDrag}), \"Drag Monitor\");",
            "        MonitorPlot monitorPlotCL = activeSimulation.getPlotManager().createMonitorPlot(new NeoObjectVector(new Object[] {reportMonitorCL}), \"CL Monitor\");",
            "        MonitorPlot monitorPlotCD = activeSimulation.getPlotManager().createMonitorPlot(new NeoObjectVector(new Object[] {reportMonitorCD}), \"CD Monitor\");",
            "        monitorPlotLift.open();",
            "        monitorPlotDrag.open();",
            "        monitorPlotCL.open();",
            "        monitorPlotCD.open();",
            "    }",
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    return code;
}
