#include "AutoSave.h"

AutoSave::AutoSave(const std::string& _simFilePath, bool _autoSaveMesh, bool _autoSaveSimulation,
                   int _numAutoSaveFiles, int _iterationInterval) :
        simFilePath       (_simFilePath),
        autoSaveMesh      (_autoSaveMesh),
        autoSaveSimulation(_autoSaveSimulation),
        numAutoSaveFiles  (_numAutoSaveFiles),
        iterationInterval (_iterationInterval) {}

std::vector<std::string> AutoSave::autoSaveCode() {
    std::vector<std::string> code;
    code = {
            "",
            "    private void autoSaveSimulation(){",
            "        // Get java object for current simulation",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Data",
            "        String  saveSimPath        = \"" + simFilePath + ("\";"),
            "        int     numAutoSaveFiles   = " + std::to_string(numAutoSaveFiles) + ";",
            "        int     iterationInterval  = " + std::to_string(iterationInterval) + ";",
            "        boolean autoSaveMesh       = " + std::string((autoSaveMesh? "true" : "false")) + ";",
            "        boolean autoSaveSimulation = " + std::string((autoSaveSimulation? "true" : "false")) + ";",
            "",
            "        // Save simulation now",
            "        activeSimulation.saveState(resolvePath(saveSimPath));",
            "",
            "        // Get auto save object",
            "        AutoSave autoSave = activeSimulation.getSimulationIterator().getAutoSave();",
            "",
            "        // Set number of auto save files",
            "        autoSave.setMaxAutosavedFiles(numAutoSaveFiles);",
            "",
            "        // Save mesh file",
            "        autoSave.setAutoSaveMesh(autoSaveMesh);",
            "",
            "        // Auto save frequency",
            "        StarUpdate starUpdate = autoSave.getStarUpdate();",
            "        starUpdate.setEnabled(autoSaveSimulation);",
            "",
            "        IterationUpdateFrequency iterationUpdateFrequency = ",
            "                starUpdate.getIterationUpdateFrequency();",
            "        iterationUpdateFrequency.setIterations(iterationInterval);",
            "    }",
    };
    return code;
}
