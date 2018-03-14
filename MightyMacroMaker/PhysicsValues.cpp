#include "PhysicsValues.h"
#include "MightyMath.h"

PhysicsValues::PhysicsValues(double                          _machNumber,
                             double                          _dynamicViscosity,
                             double                          _referencePressure,
                             double                          _staticTemperature,
                             const FlowDirection&            _flowDirection,
                             const Velocity&                 _velocity,
                             const std::vector<std::string>& _regionName,
                             const std::vector<std::string>& _boundaryCondition):
        machNumber       (_machNumber),
        dynamicViscosity (_dynamicViscosity),
        referencePressure(_referencePressure),
        staticTemperature(_staticTemperature),
        flowDirection    (_flowDirection),
        velocity         (_velocity),
        regionName       (_regionName),
        boundaryCondition(_boundaryCondition){}

std::vector<std::string> PhysicsValues::physicsValuesCode() {
    std::vector<std::string> code;
    std::vector<std::string> codeBuffer;

    // Data
    code = {
            "",
            "    private void physicsValues(){",
            "        Simulation activeSimulation = getActiveSimulation();",
            "",
            "        // Data",
            "        double valueMachNumber        = " + javaScientific(machNumber) + ";"
    };

    // Dynamic viscosity for RANS simulation
    if(dynamicViscosity > 0) {
        code.emplace_back("        double valueDynamicViscosity  = " + javaScientific(dynamicViscosity) + ";");
    }

    // More data and physics continuum object
    codeBuffer = {
            "        double valueReferencePressure = " + javaScientific(referencePressure) + ";",
            "        double valueStaticTemperature = " + javaScientific(staticTemperature) + ";",
            "        double valueVelocityX         = " + javaScientific(velocity.X) + ";",
            "        double valueVelocityY         = " + javaScientific(velocity.Y) + ";",
            "        double valueVelocityZ         = " + javaScientific(velocity.Z) + ";",
            "        double valueDirectionX        = " + javaScientific(flowDirection.X) + ";",
            "        double valueDirectionY        = " + javaScientific(flowDirection.Y) + ";",
            "        double valueDirectionZ        = " + javaScientific(flowDirection.Z) + ";",
            "",
            "        // Get physics continuum object",
            "        PhysicsContinuum physicsContinuumObj = ((PhysicsContinuum) activeSimulation.getContinuumManager().getContinuum(\"Physics 1\"));",
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Set AUSM scheme
    codeBuffer = {
            "",
            "        // Inviscid flux - AUSM SCHEME",
            "        CoupledFlowModel coupledFlowModelObj = physicsContinuumObj.getModelManager().getModel(CoupledFlowModel.class);",
            "        coupledFlowModelObj.getCoupledInviscidFluxOption().setSelected(CoupledInviscidFluxOption.Type.AUSM_SCHEME);"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Initial conditions
    codeBuffer = {
            "",
            "        // INITIAL CONDITIONS",
            "",
            "        // Reference pressure",
            "        physicsContinuumObj.getReferenceValues().get(ReferencePressure.class).setValue(valueReferencePressure);",
            "",
            "        // Static temperature",
            "        StaticTemperatureProfile staticTemperatureProfileObj = physicsContinuumObj.getInitialConditions().get(StaticTemperatureProfile.class);",
            "        staticTemperatureProfileObj.getMethod(ConstantScalarProfileMethod.class).getQuantity().setValue(valueStaticTemperature);",
            "",
            "        // Velocity",
            "        VelocityProfile velocityProfileObj = physicsContinuumObj.getInitialConditions().get(VelocityProfile.class);",
            "        velocityProfileObj.getMethod(ConstantVectorProfileMethod.class).getQuantity().setComponents(valueVelocityX, valueVelocityY, valueVelocityZ);"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Initial condition viscosity
    if(dynamicViscosity > 0){
        codeBuffer = {
                "",
                "        // Dynamic viscosity",
                "        SingleComponentGasModel singleComponentGasModelObj =  physicsContinuumObj.getModelManager().getModel(SingleComponentGasModel.class);",
                "        Gas gasAir = ((Gas) singleComponentGasModelObj.getMaterial());",
                "        ConstantMaterialPropertyMethod constantMaterialPropertyMethodDynamicViscosity = ((ConstantMaterialPropertyMethod) gasAir.getMaterialProperties().getMaterialProperty(DynamicViscosityProperty.class).getMethod());",
                "        constantMaterialPropertyMethodDynamicViscosity.getQuantity().setValue(valueDynamicViscosity);",
        };
        code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());
    }

    // Boundary conditions
    codeBuffer = {
            "",
            "        // BOUNDARY CONDITIONS",
            "",
            "        // Get the region object",
            "        Region regionAir = activeSimulation.getRegionManager().getRegion(\"Air\");"
    };
    code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());

    // Physics values for stream boundary conditions
    for(int i = 0; i < regionName.size(); i++) {
        if(boundaryCondition[i] == "freeStream"){
            codeBuffer = {
                    "",
                    "        // Boundary: " + regionName[i],
                    "        Boundary boundary" + regionName[i] + " = regionAir.getBoundaryManager().getBoundary(\"Subtract.Domain." + regionName[i] + "\");",
                    "        // Flow direction",
                    "        FlowDirectionProfile flowDirectionProfile"  + regionName[i] + " = boundary"  + regionName[i] + ".getValues().get(FlowDirectionProfile.class);",
                    "        flowDirectionProfile"  + regionName[i] + ".getMethod(ConstantVectorProfileMethod.class).getQuantity().setComponents(valueDirectionX, valueDirectionY, valueDirectionZ);",
                    "        // Mach number",
                    "        MachNumberProfile machNumberProfile"  + regionName[i] + " = boundary"  + regionName[i] + ".getValues().get(MachNumberProfile.class);",
                    "        // Static temperature",
                    "        StaticTemperatureProfile staticTemperatureProfile" + regionName[i] + " = boundary"  + regionName[i] + ".getValues().get(StaticTemperatureProfile.class);",
                    "        staticTemperatureProfile"  + regionName[i] + ".getMethod(ConstantScalarProfileMethod.class).getQuantity().setValue(valueStaticTemperature);"
            };
            code.insert(code.end(), codeBuffer.begin(), codeBuffer.end());
        }
    }
    code.emplace_back("    }");

    return code;
}