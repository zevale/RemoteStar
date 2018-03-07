#include "SolverOptions.h"
#include "MightyMath.h"

SolverOptions::SolverOptions(double _CFL) : CFL(_CFL){}

std::vector<std::string> SolverOptions::solverOptionsCode() {
    std::vector<std::string> code;
    code = {
            "",
            "    private void solverOptions(){",
            "        Simulation activeSimulation = getActiveSimulation();  ",
            "",
            "        // Data",
            "        double valueCFL = " + javaScientific(CFL) + ";",
            "",
            "        // Get coupled implicit solver object",
            "        CoupledImplicitSolver coupledImplicitSolverObj = ((CoupledImplicitSolver) activeSimulation.getSolverManager().getSolver(CoupledImplicitSolver.class));",
            "",
            "        // Set expert initialization - grid sequencing",
            "        coupledImplicitSolverObj.getExpertInitManager().getExpertInitOption().setSelected(ExpertInitOption.Type.GRID_SEQ_METHOD);",
            "        // Set expert driver",
            "        // Set expert driver",
            "        coupledImplicitSolverObj.getSolutionDriverManager().getExpertDriverOption().setSelected(ExpertDriverOption.Type.EXPERT_DRIVER);",
            "",
            "        // Courant Number",
            "        coupledImplicitSolverObj.setCFL(valueCFL);",
            "    }"
    };
    return code;
}