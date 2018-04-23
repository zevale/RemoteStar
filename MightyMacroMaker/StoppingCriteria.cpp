#include "StoppingCriteria.h"
#include "MightyMath.h"

StoppingCriteria::StoppingCriteria(int    _maximumSteps,
                                   int    _numSamples,
                                   double _asymptoticCL,
                                   double _asymptoticCD) :
        maximumSteps(_maximumSteps),
        numSamples  (_numSamples),
        asymptoticCL(_asymptoticCL),
        asymptoticCD(_asymptoticCD) {}

std::vector<std::string> StoppingCriteria::stoppingCriteriaCode() {
    std::vector<std::string> code;
    std::vector<std::string> codeBuffer;
    code = {
            "",
            "    private void stoppingCriteria(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Data",
            "        int    numberMaxSteps    = " + std::to_string(maximumSteps) + ";"
    };

    // Check asymptotic stopping criteria
    if(numSamples > 0) {
        // Add numSamples
        code.emplace_back("        int    numberSamples     = " + std::to_string(numSamples) + ";");

        // Check asymptotic CL
        if(asymptoticCL > 0)
            code.emplace_back("        double valueAsymptoticCL = " + javaScientific(asymptoticCL) + ";");

        // Check asymptotic CD
        if(asymptoticCD > 0)
            code.emplace_back("        double valueAsymptoticCD = " + javaScientific(asymptoticCD) + ";");
    }

    // Max steps criterion
    codeBuffer = {
            "",
            "        // NUMBER OF ITERATIONS",
            "        // Create stopping criterion",
            "        StepStoppingCriterion stepStoppingCriterionMaxSteps = ((StepStoppingCriterion) activeSimulation.getSolverStoppingCriterionManager().getSolverStoppingCriterion(\"Maximum Steps\"));",
            "        // Maximum number of steps",
            "        stepStoppingCriterionMaxSteps.setMaximumNumberSteps(numberMaxSteps);",
            "        // Logical rule",
            "        stepStoppingCriterionMaxSteps.getLogicalOption().setSelected(SolverStoppingCriterionLogicalOption.Type.OR);"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Asymptotic CL
    if(asymptoticCL > 0){
        codeBuffer = {
                "",
                "        // ASYMPTOTIC CL",
                "        // Get CL monitor",
                "        ReportMonitor reportMonitorCL = ((ReportMonitor) activeSimulation.getMonitorManager().getMonitor(\"CL Monitor\"));",
                "        // Create stopping criterion",
                "        MonitorIterationStoppingCriterion monitorIterationStoppingCriterionCL = reportMonitorCL.createIterationStoppingCriterion();",
                "        // Asymptotic type",
                "        ((MonitorIterationStoppingCriterionOption) monitorIterationStoppingCriterionCL.getCriterionOption()).setSelected(MonitorIterationStoppingCriterionOption.Type.ASYMPTOTIC);",
                "        // Logical rule",
                "        monitorIterationStoppingCriterionCL.getLogicalOption().setSelected(SolverStoppingCriterionLogicalOption.Type." + std::string((asymptoticCD > 0) ? "AND" : "OR") + ");",
                "        // Set maximum amplitude",
                "        MonitorIterationStoppingCriterionAsymptoticType monitorIterationStoppingCriterionAsymptoticTypeCL = ((MonitorIterationStoppingCriterionAsymptoticType) monitorIterationStoppingCriterionCL.getCriterionType());",
                "        monitorIterationStoppingCriterionAsymptoticTypeCL.getMaxWidth().setValue(valueAsymptoticCL);",
                "        monitorIterationStoppingCriterionAsymptoticTypeCL.setNumberSamples(numberSamples);",
        };
        code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());
    }

    // Asymptotic CD
    if(asymptoticCD > 0){
        codeBuffer = {
                "",
                "        // ASYMPTOTIC CD",
                "        // Get CD monitor",
                "        ReportMonitor reportMonitorCD = ((ReportMonitor) activeSimulation.getMonitorManager().getMonitor(\"CD Monitor\"));",
                "        // Create stopping criterion",
                "        MonitorIterationStoppingCriterion monitorIterationStoppingCriterionCD = reportMonitorCD.createIterationStoppingCriterion();",
                "        // Asymptotic type",
                "        ((MonitorIterationStoppingCriterionOption) monitorIterationStoppingCriterionCD.getCriterionOption()).setSelected(MonitorIterationStoppingCriterionOption.Type.ASYMPTOTIC);",
                "        // Logical rule",
                "        monitorIterationStoppingCriterionCD.getLogicalOption().setSelected(SolverStoppingCriterionLogicalOption.Type." + std::string((asymptoticCL > 0) ? "AND" : "OR") + ");",
                "        // Set maximum amplitude",
                "        MonitorIterationStoppingCriterionAsymptoticType monitorIterationStoppingCriterionAsymptoticTypeCD = ((MonitorIterationStoppingCriterionAsymptoticType) monitorIterationStoppingCriterionCD.getCriterionType());",
                "        monitorIterationStoppingCriterionAsymptoticTypeCD.getMaxWidth().setValue(valueAsymptoticCD);",
                "        monitorIterationStoppingCriterionAsymptoticTypeCD.setNumberSamples(numberSamples);"
        };
        code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());
    }
    code.emplace_back("    }");

    return code;
}
