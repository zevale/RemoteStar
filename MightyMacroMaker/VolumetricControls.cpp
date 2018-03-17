#include "VolumetricControls.h"

VolumetricControls::VolumetricControls(const Block &_block, const Cylinder& _cylinder) :
        block(_block),
        cylinder(_cylinder) {}

/*
 * Getters
 */
Block VolumetricControls::getBlock() const {
    return block;
}

std::vector<std::string> VolumetricControls::volumetricControlsCode() {
    std::vector<std::string> code;
    std::vector<std::string> codeBuffer;

    // Check block exists
    if(!block.surfaceSize.empty()){
        code = {
                "    private void volumetricControls(){",
                "        Simulation activeSimulation = getActiveSimulation();",
                "",
                "        // Get mesh part factory object",
                "        MeshPartFactory meshPartFactoryObj = activeSimulation.get(MeshPartFactory.class);",
                "",
                "        // Get length units",
                "        Units unitsObj = activeSimulation.getUnitsManager().getPreferredUnits(new IntVector(new int[] {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));",
                "",
                "        // Get coordinate system",
                "        LabCoordinateSystem labCoordinateSystemObj = activeSimulation.getCoordinateSystemManager().getLabCoordinateSystem();",
                "",
                "        // Get mesh continuum object",
                "        MeshContinuum meshContinuumObj = ((MeshContinuum) activeSimulation.getContinuumManager().getContinuum(\"Mesh 1\"));"
        };
    }

    // Blocks
    if(!block.surfaceSize.empty()){
        for(int i = 0; i < block.surfaceSize.size(); i++){
            // Variable names
            std::string valueCornerX1("valueCornerX1_B" + std::to_string(i+1));
            std::string valueCornerY1("valueCornerY1_B" + std::to_string(i+1));
            std::string valueCornerZ1("valueCornerZ1_B" + std::to_string(i+1));
            std::string valueCornerX2("valueCornerX2_B" + std::to_string(i+1));
            std::string valueCornerY2("valueCornerY2_B" + std::to_string(i+1));
            std::string valueCornerZ2("valueCornerZ2_B" + std::to_string(i+1));
            std::string valueSurfaceSize("valueSurfaceSize_B" + std::to_string(i+1));
            std::string simpleBlockPartObj("simpleBlockPartObj_B" + std::to_string(i+1));
            std::string coordinateCorner1("coordinateCorner1_B" + std::to_string(i+1));
            std::string coordinateCorner2("coordinateCorner2_B" + std::to_string(i+1));
            std::string volumeSourceObj("volumeSourceObj_B" + std::to_string(i+1));
            std::string volumeSourceDualMesherSizeOptionObj("volumeSourceDualMesherSizeOptionObj_B" + std::to_string(i+1));
            std::string volumeSourceSizeObj("volumeSourceSizeObj_B" + std::to_string(i+1));
            std::string absoluteSizeObj("absoluteSizeObj_B" + std::to_string(i+1));
            std::string volumeSourceResurfacerSizeOptionObj("volumeSourceResurfacerSizeOptionObj_B" + std::to_string(i+1));
            codeBuffer = {
                    "",
                    "        /*",
                    "         * BLOCK " + std::to_string(i + 1),
                    "         */",
                    "",
                    "        // Data",
                    "        double " + valueCornerX1 + "    = " + std::to_string(block.x1[i]) + ";",
                    "        double " + valueCornerY1 + "    = " + std::to_string(block.y1[i]) + ";",
                    "        double " + valueCornerZ1 + "    = " + std::to_string(block.z1[i]) + ";",
                    "        double " + valueCornerX2 + "    = " + std::to_string(block.x2[i]) + ";",
                    "        double " + valueCornerY2 + "    = " + std::to_string(block.y2[i]) + ";",
                    "        double " + valueCornerZ2 + "    = " + std::to_string(block.z2[i]) + ";",
                    "        double " + valueSurfaceSize + " = " + std::to_string(block.surfaceSize[i]) + ";",
                    "",
                    "        // GENERATE BLOCK",
                    "        // Create new simple block part object",
                    "        SimpleBlockPart " + simpleBlockPartObj + " = meshPartFactoryObj.createNewBlockPart(activeSimulation.get(SimulationPartManager.class));",
                    "        // Assign coordinate system",
                    "        " + simpleBlockPartObj + ".setCoordinateSystem(labCoordinateSystemObj);",
                    "        // Corner 1",
                    "        Coordinate " + coordinateCorner1 + " = " + simpleBlockPartObj + ".getCorner1();",
                    "        " + coordinateCorner1 + ".setCoordinateSystem(labCoordinateSystemObj);",
                    "        " + coordinateCorner1 + ".setCoordinate(unitsObj, unitsObj, unitsObj, new DoubleVector(new double[] {" + valueCornerX1 + ", " + valueCornerY1 + ", " + valueCornerZ1+ "}));",
                    "        // Corner 2",
                    "        Coordinate " + coordinateCorner2 + " = " + simpleBlockPartObj + ".getCorner2();",
                    "        " + coordinateCorner2 + ".setCoordinateSystem(labCoordinateSystemObj);",
                    "        " + coordinateCorner2 + ".setCoordinate(unitsObj, unitsObj, unitsObj, new DoubleVector(new double[] {" + valueCornerX2 + ", " + valueCornerY2 + ", " + valueCornerZ2+ "}));",
                    "        // Presentation name",
                    "        " + simpleBlockPartObj + ".setPresentationName(\"Block " + std::to_string(i+1) +"\");",
                    "        // SIZING",
                    "        // Create volume source object and select the block part",
                    "        VolumeSource " + volumeSourceObj + " = meshContinuumObj.getVolumeSources().createVolumeSource();",
                    "        " + volumeSourceObj + " .getPartGroup().setQuery(null);",
                    "        " + volumeSourceObj + " .getPartGroup().setObjects(" + simpleBlockPartObj + ");",
                    "        // Set surface size absolute",
                    "        VolumeSourceDualMesherSizeOption " + volumeSourceDualMesherSizeOptionObj + " = " + volumeSourceObj + ".get(MeshConditionManager.class).get(VolumeSourceDualMesherSizeOption.class);",
                    "        " + volumeSourceDualMesherSizeOptionObj + ".setVolumeSourceDualMesherSizeOption(true);",
                    "        VolumeSourceSize " + volumeSourceSizeObj + " = " + volumeSourceObj + ".get(MeshValueManager.class).get(VolumeSourceSize.class);",
                    "        " + volumeSourceSizeObj + ".getRelativeOrAbsoluteOption().setSelected(RelativeOrAbsoluteOption.Type.ABSOLUTE);",
                    "        // Surface size",
                    "        AbsoluteSize " + absoluteSizeObj + " = ((AbsoluteSize) " + volumeSourceSizeObj + ".getAbsoluteSizeValue());",
                    "        " + absoluteSizeObj + ".getValue().setValue(" + valueSurfaceSize + ");",
                    "        // Surface remesher",
                    "        VolumeSourceResurfacerSizeOption " + volumeSourceResurfacerSizeOptionObj + " = " + volumeSourceObj + ".get(MeshConditionManager.class).get(VolumeSourceResurfacerSizeOption.class);",
                    "        " + volumeSourceResurfacerSizeOptionObj + ".setVolumeSourceResurfacerSizeOption(true);"
            };
            code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());
        }
    }

    // Cylinders
    if(!cylinder.surfaceSize.empty()){
        for(int i = 0; i < cylinder.surfaceSize.size(); i++){
            // Variable names
            std::string valueBaseX1("valueBaseX1_C" + std::to_string(i+1));
            std::string valueBaseY1("valueBaseY1_C" + std::to_string(i+1));
            std::string valueBaseZ1("valueBaseZ1_C" + std::to_string(i+1));
            std::string valueBaseX2("valueBaseX2_C" + std::to_string(i+1));
            std::string valueBaseY2("valueBaseY2_C" + std::to_string(i+1));
            std::string valueBaseZ2("valueBaseZ2_C" + std::to_string(i+1));
            std::string valueRadius("valueRadius_C" + std::to_string(i+1));
            std::string valueSurfaceSize("valueSurfaceSize_C" + std::to_string(i+1));
            std::string simpleCylinderPartObj("simpleCylinderPartObj_C" + std::to_string(i+1));
            std::string coordinateBase1("coordinateBase1_C" + std::to_string(i+1));
            std::string coordinateBase2("coordinateBase2_C" + std::to_string(i+1));
            std::string volumeSourceCylinderObj("volumeSourceCylinderObj_C" + std::to_string(i+1));
            std::string volumeSourceDualMesherSizeOptionCylinderObj("volumeSourceDualMesherSizeOptionCylinderObj_C" + std::to_string(i+1));
            std::string volumeSourceSizeCylinderObj("volumeSourceSizeCylinderObj_C" + std::to_string(i+1));
            std::string absoluteSizeCylinderObj("absoluteSizeCylinderObj_C" + std::to_string(i+1));
            std::string volumeSourceResurfacerSizeOptionCylinderObj("volumeSourceResurfacerSizeOptionCylinderObj_C" + std::to_string(i+1));
            codeBuffer = {
                    "",
                    "        /*",
                    "         * CYLINDER " + std::to_string(i + 1),
                    "         */",
                    "",
                    "        // Data",
                    "        double " + valueBaseX1 + "      = " + std::to_string(cylinder.x1[i]) + ";",
                    "        double " + valueBaseY1 + "      = " + std::to_string(cylinder.y1[i]) + ";",
                    "        double " + valueBaseZ1 + "      = " + std::to_string(cylinder.z1[i]) + ";",
                    "        double " + valueBaseX2 + "      = " + std::to_string(cylinder.x2[i]) + ";",
                    "        double " + valueBaseY2 + "      = " + std::to_string(cylinder.y2[i]) + ";",
                    "        double " + valueBaseZ2 + "      = " + std::to_string(cylinder.z2[i]) + ";",
                    "        double " + valueRadius + "      = " + std::to_string(cylinder.radius[i]) + ";",
                    "        double " + valueSurfaceSize + " = " + std::to_string(cylinder.surfaceSize[i]) + ";",
                    "",
                    "        // GENERATE CYLINDER",
                    "        // Create new simple cylinder part object",
                    "        SimpleCylinderPart " +  simpleCylinderPartObj + " = meshPartFactoryObj.createNewCylinderPart(activeSimulation.get(SimulationPartManager.class));",
                    "        // Set coordinate system",
                    "        " + simpleCylinderPartObj + ".setCoordinateSystem(labCoordinateSystemObj);",
                    "        // Base 1",
                    "        Coordinate " + coordinateBase1 + " = " + simpleCylinderPartObj + ".getStartCoordinate();",
                    "        " + coordinateBase1 + ".setCoordinateSystem(labCoordinateSystemObj);",
                    "        " + coordinateBase1 + ".setCoordinate(unitsObj, unitsObj, unitsObj, new DoubleVector(new double[] {" + valueBaseX1+ ", " + valueBaseY1 + ", " + valueBaseZ1 + "}));",
                    "        // Base 2",
                    "        Coordinate " + coordinateBase2 + " = " + simpleCylinderPartObj + ".getEndCoordinate();",
                    "        " + coordinateBase2 + ".setCoordinateSystem(labCoordinateSystemObj);",
                    "        " + coordinateBase2 + ".setCoordinate(unitsObj, unitsObj, unitsObj, new DoubleVector(new double[] {" + valueBaseX2+ ", " + valueBaseY2 + ", " + valueBaseZ2 + "}));",
                    "        // Radius",
                    "        " + simpleCylinderPartObj + ".getRadius().setUnits(unitsObj);",
                    "        " + simpleCylinderPartObj + ".getRadius().setValue(" + valueRadius + ");",
                    "        // Presentation name",
                    "        " + simpleCylinderPartObj + ".setPresentationName(\"Cylinder " + std::to_string(i+1) +"\");",
                    "",
                    "        // SIZING",
                    "        // Create volume source and select the cylinder",
                    "        VolumeSource " + volumeSourceCylinderObj + " = meshContinuumObj.getVolumeSources().createVolumeSource();",
                    "        " + volumeSourceCylinderObj + ".getPartGroup().setQuery(null);",
                    "        " + volumeSourceCylinderObj + ".getPartGroup().setObjects(" + simpleCylinderPartObj + ");",
                    "        // Set surface size absolute",
                    "         VolumeSourceDualMesherSizeOption " + volumeSourceDualMesherSizeOptionCylinderObj + " = " + volumeSourceCylinderObj + ".get(MeshConditionManager.class).get(VolumeSourceDualMesherSizeOption.class);",
                    "         " + volumeSourceDualMesherSizeOptionCylinderObj + ".setVolumeSourceDualMesherSizeOption(true);",
                    "         VolumeSourceSize " + volumeSourceSizeCylinderObj + " = " + volumeSourceCylinderObj + ".get(MeshValueManager.class).get(VolumeSourceSize.class);",
                    "         " + volumeSourceSizeCylinderObj + ".getRelativeOrAbsoluteOption().setSelected(RelativeOrAbsoluteOption.Type.ABSOLUTE);",
                    "         // Surface size",
                    "         AbsoluteSize " + absoluteSizeCylinderObj + " = ((AbsoluteSize) " + volumeSourceSizeCylinderObj + ".getAbsoluteSizeValue());",
                    "         " + absoluteSizeCylinderObj + ".getValue().setValue(" + valueSurfaceSize + ");",
                    "         // Surface remesher",
                    "         VolumeSourceResurfacerSizeOption " + volumeSourceResurfacerSizeOptionCylinderObj + " = " + volumeSourceCylinderObj+ ".get(MeshConditionManager.class).get(VolumeSourceResurfacerSizeOption.class);",
                    "         " + volumeSourceResurfacerSizeOptionCylinderObj + ".setVolumeSourceResurfacerSizeOption(true);"
            };
            code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());
        }
    }
    code.emplace_back("    }");

    return code;
}