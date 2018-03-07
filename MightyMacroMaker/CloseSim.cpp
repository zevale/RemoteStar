#include "CloseSim.h"
CloseSim::CloseSim(const std::string &_simFilePath) : simFilePath(_simFilePath) {}

std::vector<std::string> CloseSim::closeSimCode() {
    std::vector<std::string> code;
    code = {
            "",
            "    private void closeSimulation(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Data",
            "        String saveSimPath = \"" + simFilePath + ("\";"),
            "        activeSimulation.saveState(resolvePath(saveSimPath));",
            "    }"
    };
    return code;
}