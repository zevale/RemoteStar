#include "MeshValues.h"
#include "MightyMath.h"
#include "MightyConstants.h"

MeshValues::MeshValues(double                     _baseSize,
                       const PrismLayer&          _prismLayer,
                       const std::vector<double>& _surfaceSize) :
        baseSize   (_baseSize),
        prismLayer (_prismLayer),
        surfaceSize(_surfaceSize){}

// Precision: 6 decimal places (check if this is ok)
std::vector<std::string> MeshValues::meshValuesCode(){
    std::vector<std::string> code;
    code = {
            "",
            "    private void meshValues(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Data",
            "        double valueBaseSize                = " + javaScientific(baseSize) + ";",
            "        double valueBoundaryMarchAngle      = " + javaScientific(prismLayer.boundaryMarchAngle) + ";",
            "        double valueMinimumThickness        = " + javaScientific(prismLayer.minimumThickness) + ";",
            "        double valueLayerChoppingPercentage = " + javaScientific(prismLayer.layerChoppingPercentage) + ";",
            "        int    valueNumPrismLayers          = " + std::to_string(prismLayer.numPrismLayers) + ";",
            "        double valuePrismLayerThickness     = " + javaScientific(prismLayer.prismLayerThickness) + ";",
            "        double valueNearWallThickness       = " + javaScientific(prismLayer.nearWallThickness) + ";",
            "",
            "        double valueMinimumTargetAircraft   = " + javaScientific(surfaceSize[0]) + ";",
            "        double valueMinimumTargetFarSide    = " + javaScientific(surfaceSize[1]) + ";",
            "        double valueMinimumTargetInlet      = " + javaScientific(surfaceSize[2]) + ";",
            "        double valueMinimumTargetOutlet     = " + javaScientific(surfaceSize[3]) + ";",
            "        double valueMinimumTargetSym        = " + javaScientific(surfaceSize[4]) + ";",
            "",
            "        // Get the mesh continuum object",
            "        MeshContinuum meshContinuumObj = ((MeshContinuum) activeSimulation.getContinuumManager().getContinuum(\"Mesh 1\"));",
            "",
            "        // Base size",
            "        meshContinuumObj.getReferenceValues().get(BaseSize.class).setValue(valueBaseSize);",
            "",
            "        // Set parallel meshing",
            "        meshContinuumObj.setMeshInParallel(true);",
            "",
            "        // PRISM MESHER",
            "        // Model",
            "        PrismMesherModel prismMesherModelObj = meshContinuumObj.getModelManager().getModel(PrismMesherModel.class);",
            "        prismMesherModelObj.setBoundaryMarchAngle(valueBoundaryMarchAngle);",
            "        prismMesherModelObj.setMinimumThickness(valueMinimumThickness);",
            "        prismMesherModelObj.setLayerChoppingPercentage(valueLayerChoppingPercentage);",
            "        prismMesherModelObj.getPrismStretchingOption().setSelected(PrismStretchingOption.Type.WALL_THICKNESS);",
            "        // Number of prism layers",
            "        NumPrismLayers numPrismLayersObj = meshContinuumObj.getReferenceValues().get(NumPrismLayers.class);",
            "        numPrismLayersObj.setNumLayers(valueNumPrismLayers);",
            "        // Prism layer thickness and near wall thickness",
            "        PrismThickness prismThicknessObj = meshContinuumObj.getReferenceValues().get(PrismThickness.class);",
            "        prismThicknessObj.getRelativeOrAbsoluteOption().setSelected(RelativeOrAbsoluteOption.Type.ABSOLUTE);",
            "        ((ScalarPhysicalQuantity) prismThicknessObj.getAbsoluteSizeValue()).setValue(valuePrismLayerThickness);",
            "        meshContinuumObj.getReferenceValues().get(PrismWallThickness.class).setValue(valueNearWallThickness);",
            "",
            "        // TYPE OF BOUNDARY CONDITIONS",
            "        Region regionAir = activeSimulation.getRegionManager().getRegion(\"Air\");",
            "        // FREE STREAM",
            "        FreeStreamBoundary freeStreamBoundaryObj = ((FreeStreamBoundary) activeSimulation.get(ConditionTypeManager.class).get(FreeStreamBoundary.class));",
            "        // FarSide",
            "        Boundary boundaryFarSide = regionAir.getBoundaryManager().getBoundary(\"Subtract.Domain.FarSide\");",
            "        boundaryFarSide.setBoundaryType(freeStreamBoundaryObj);",
            "        // Inlet",
            "        Boundary boundaryInlet = regionAir.getBoundaryManager().getBoundary(\"Subtract.Domain.Inlet\");",
            "        boundaryInlet.setBoundaryType(freeStreamBoundaryObj);",
            "        // Outlet",
            "        Boundary boundaryOutlet = regionAir.getBoundaryManager().getBoundary(\"Subtract.Domain.Outlet\");",
            "        boundaryOutlet.setBoundaryType(freeStreamBoundaryObj);",
            "        // SYMETRY PLAME",
            "        SymmetryBoundary symmetryBoundaryObj = ((SymmetryBoundary) activeSimulation.get(ConditionTypeManager.class).get(SymmetryBoundary.class));",
            "        // Sym",
            "        Boundary boundarySym = regionAir.getBoundaryManager().getBoundary(\"Subtract.Domain.Sym\");",
            "        boundarySym.setBoundaryType(symmetryBoundaryObj);",
            "        // WALL",
            "        // Wall type is default but is explicitly enforced here for consistency",
            "        WallBoundary wallBoundaryObj = ((WallBoundary) activeSimulation.get(ConditionTypeManager.class).get(WallBoundary.class));",
            "        // SurfaceMesh (Aircraft)",
            "        Boundary boundaryAircraft = regionAir.getBoundaryManager().getBoundary(\"Subtract.SurfMesh.Surface\");",
            "        boundaryAircraft.setBoundaryType(wallBoundaryObj);",
            "",
            "        // MESH SIZING",
            "",
            "        // FarSide",
            "        // Option surface size - ON",
            "        SurfaceSizeOption surfaceSizeOptionFarSide = boundaryFarSide.get(MeshConditionManager.class).get(SurfaceSizeOption.class);",
            "        surfaceSizeOptionFarSide.setSurfaceSizeOption(true);",
            "        // Surface size relative or absolute - ABSOLUTE",
            "        SurfaceSize surfaceSizeFarSide = boundaryFarSide.get(MeshValueManager.class).get(SurfaceSize.class);",
            "        surfaceSizeFarSide.getRelativeOrAbsoluteOption().setSelected(RelativeOrAbsoluteOption.Type.ABSOLUTE);",
            "        // Set surface size - Minimum = Target",
            "        AbsoluteMinimumSize absoluteMinimumSizeFarSide = surfaceSizeFarSide.getAbsoluteMinimumSize();",
            "        absoluteMinimumSizeFarSide.getValue().setValue(valueMinimumTargetFarSide);",
            "        AbsoluteTargetSize absoluteTargetSizeFarSide = surfaceSizeFarSide.getAbsoluteTargetSize();",
            "        absoluteTargetSizeFarSide.getValue().setValue(valueMinimumTargetFarSide);",
            "",
            "        // Inlet",
            "        // Option surface size - ON",
            "        SurfaceSizeOption surfaceSizeOptionInlet = boundaryInlet.get(MeshConditionManager.class).get(SurfaceSizeOption.class);",
            "        surfaceSizeOptionInlet.setSurfaceSizeOption(true);",
            "        // Surface size absolute",
            "        SurfaceSize surfaceSizeInlet = boundaryInlet.get(MeshValueManager.class).get(SurfaceSize.class);",
            "        surfaceSizeInlet.getRelativeOrAbsoluteOption().setSelected(RelativeOrAbsoluteOption.Type.ABSOLUTE);",
            "        // Set surface size - minimum = target",
            "        AbsoluteMinimumSize absoluteMinimumSizeInlet = surfaceSizeInlet.getAbsoluteMinimumSize();",
            "        absoluteMinimumSizeInlet.getValue().setValue(valueMinimumTargetInlet);",
            "        AbsoluteTargetSize absoluteTargetSizeInlet = surfaceSizeInlet.getAbsoluteTargetSize();",
            "        absoluteTargetSizeInlet.getValue().setValue(valueMinimumTargetInlet);"
            "",
            "        // Outlet",
            "        // Option surface size - ON",
            "        SurfaceSizeOption surfaceSizeOptionOutlet = boundaryOutlet.get(MeshConditionManager.class).get(SurfaceSizeOption.class);",
            "        surfaceSizeOptionOutlet.setSurfaceSizeOption(true);",
            "        // Surface size absolute",
            "        SurfaceSize surfaceSizeOutlet = boundaryOutlet.get(MeshValueManager.class).get(SurfaceSize.class);",
            "        surfaceSizeOutlet.getRelativeOrAbsoluteOption().setSelected(RelativeOrAbsoluteOption.Type.ABSOLUTE);",
            "        // Set surface size - minimum = target",
            "        AbsoluteMinimumSize absoluteMinimumSizeOutlet = surfaceSizeOutlet.getAbsoluteMinimumSize();",
            "        absoluteMinimumSizeOutlet.getValue().setValue(valueMinimumTargetOutlet);",
            "        AbsoluteTargetSize absoluteTargetSizeOutlet = surfaceSizeOutlet.getAbsoluteTargetSize();",
            "        absoluteTargetSizeOutlet.getValue().setValue(valueMinimumTargetOutlet);",
            "",
            "        // Sym",
            "        // Option surface size - ON",
            "        SurfaceSizeOption surfaceSizeOptionSym = boundarySym.get(MeshConditionManager.class).get(SurfaceSizeOption.class);",
            "        surfaceSizeOptionSym.setSurfaceSizeOption(true);",
            "        // Surface size absolute",
            "        SurfaceSize surfaceSizeSym = boundarySym.get(MeshValueManager.class).get(SurfaceSize.class);",
            "        surfaceSizeSym.getRelativeOrAbsoluteOption().setSelected(RelativeOrAbsoluteOption.Type.ABSOLUTE);",
            "        // Set surface size - minimum = target",
            "        AbsoluteMinimumSize absoluteMinimumSizeSym = surfaceSizeSym.getAbsoluteMinimumSize();",
            "        absoluteMinimumSizeSym.getValue().setValue(valueMinimumTargetSym);",
            "        AbsoluteTargetSize absoluteTargetSizeSym = surfaceSizeSym.getAbsoluteTargetSize();",
            "        absoluteTargetSizeSym.getValue().setValue(valueMinimumTargetSym);",
            "",
            "        // Aicraft",
            "        // Option surface size - ON",
            "        SurfaceSizeOption surfaceSizeOptionAircraft = boundaryAircraft.get(MeshConditionManager.class).get(SurfaceSizeOption.class);",
            "        surfaceSizeOptionAircraft.setSurfaceSizeOption(true);",
            "        // Surface size absolute",
            "        SurfaceSize surfaceSizeAircraft = boundaryAircraft.get(MeshValueManager.class).get(SurfaceSize.class);",
            "        surfaceSizeAircraft.getRelativeOrAbsoluteOption().setSelected(RelativeOrAbsoluteOption.Type.ABSOLUTE);",
            "        // Set surface size - minimum = target",
            "        AbsoluteMinimumSize absoluteMinimumSizeAircraft = surfaceSizeAircraft.getAbsoluteMinimumSize();",
            "        absoluteMinimumSizeAircraft.getValue().setValue(valueMinimumTargetAircraft);",
            "        AbsoluteTargetSize absoluteTargetSizeAircraft = surfaceSizeAircraft.getAbsoluteTargetSize();",
            "        absoluteTargetSizeAircraft.getValue().setValue(valueMinimumTargetAircraft);",
            "    }"
    };
    return code;
}