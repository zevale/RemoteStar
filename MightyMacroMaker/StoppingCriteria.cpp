#include "StoppingCriteria.h"
#include "MightyMath.h"

StoppingCriteria::StoppingCriteria(int    _maximumSteps,
                                   int    _numSamples,
                                   double _asymptoticCL) : maximumSteps(_maximumSteps),
                                                           numSamples  (_numSamples),
                                                           asymptoticCL(_asymptoticCL) {}

std::vector<std::string> StoppingCriteria::stoppingCriteriaCode() {
    std::vector<std::string> code;
    code = {
            "",
            "    private void stoppingCriteria(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Data",
            "        int    numberMaxSteps    = " + std::to_string(maximumSteps) + ";",
            "        int    numberSamples     = " + std::to_string(numSamples) + ";",
            "        double valueAsymptoticCL = " + javaScientific(asymptoticCL) + ";",
            "",
            "        // NUMBER OF ITERATIONS",
            "        // Create stopping criterion",
            "        StepStoppingCriterion stepStoppingCriterionMaxSteps = ((StepStoppingCriterion) activeSimulation.getSolverStoppingCriterionManager().getSolverStoppingCriterion(\"Maximum Steps\"));",
            "        // Maximum number of steps",
            "        stepStoppingCriterionMaxSteps.setMaximumNumberSteps(numberMaxSteps);",
            "        // Logical rule",
            "        stepStoppingCriterionMaxSteps.getLogicalOption().setSelected(SolverStoppingCriterionLogicalOption.Type.OR);",
            "",
            "        // ASYMPTOTIC CL",
            "        // Get CL monitor",
            "        ReportMonitor reportMonitorCL = ((ReportMonitor) activeSimulation.getMonitorManager().getMonitor(\"CL Monitor\"));",
            "        // Create stopping criterion",
            "        MonitorIterationStoppingCriterion monitorIterationStoppingCriterionCL = reportMonitorCL.createIterationStoppingCriterion();",
            "        // Asymptotic type",
            "        ((MonitorIterationStoppingCriterionOption) monitorIterationStoppingCriterionCL.getCriterionOption()).setSelected(MonitorIterationStoppingCriterionOption.Type.ASYMPTOTIC);",
            "        // Logical rule",
            "        monitorIterationStoppingCriterionCL.getLogicalOption().setSelected(SolverStoppingCriterionLogicalOption.Type.OR);",
            "        // Set maximum amplitude",
            "        MonitorIterationStoppingCriterionAsymptoticType monitorIterationStoppingCriterionAsymptoticTypeCL = ((MonitorIterationStoppingCriterionAsymptoticType) monitorIterationStoppingCriterionCL.getCriterionType());",
            "        monitorIterationStoppingCriterionAsymptoticTypeCL.getMaxWidth().setValue(valueAsymptoticCL);",
            "        monitorIterationStoppingCriterionAsymptoticTypeCL.setNumberSamples(numberSamples);",
            "    }"
    };
    return code;
}