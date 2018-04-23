#ifndef MIGHTYTYPES_H
#define MIGHTYTYPES_H

#include <vector>

struct PrismLayer {
    // Mesh Models > Prism Layer Mesher
    double boundaryMarchAngle;         // "Boundary March Angle"           [deg]
    double minimumThickness;           // "Minimum Thickness Percentage"   [% global thickness]
    double layerChoppingPercentage;    // "Layer Reduction Percentage"     [% global thickness]

    // Reference Values > Prism Layer Mesher
    int    numPrismLayers;             // "Number of Prism Layers"         [integer]
    double prismLayerThickness;        // "Prism Layer Thickness"          [m]
    double nearWallThickness;          // "Thickness of Near Wall PL"      [m]
};

struct FlowDirection {
    // Regions > Boundaries > Free stream boundary > Physics Values > Flow Direction
    double X; // []
    double Y; // []
    double Z; // []
};

struct Velocity {
    // Continua > Physics 1 > Initial conditions > Velocity
    double X; // [m/s]
    double Y; // [m/s]
    double Z; // [m/s]
};

struct LiftDirection {
    // Reports > Lift, CL > Direction
    double X; // []
    double Y; // []
    double Z; // []
};

struct DragDirection {
    // Reports > Drag, CD > Direction
    double X; // []
    double Y; // []
    double Z; // []
};

struct ViewUp {
    // Reports > AircraftArea > View Up
    double X; // []
    double Y; // []
    double Z; // []
};

struct Normal {
    // Reports > AircraftArea > Normal
    double X; // []
    double Y; // []
    double Z; // []
};

struct Block {
    // Parts > New Shape Part > Block
    std::vector<double> x1;
    std::vector<double> y1;
    std::vector<double> z1;
    std::vector<double> x2;
    std::vector<double> y2;
    std::vector<double> z2;
    std::vector<double> surfaceSize;
};

struct Cylinder {
    // Parts > New Shape Part > Cylinder
    std::vector<double> x1;
    std::vector<double> y1;
    std::vector<double> z1;
    std::vector<double> x2;
    std::vector<double> y2;
    std::vector<double> z2;
    std::vector<double> radius;
    std::vector<double> surfaceSize;
};

struct Cone {
    // Parts > New Shape Part > Cone
    std::vector<double> x1;
    std::vector<double> y1;
    std::vector<double> z1;
    std::vector<double> x2;
    std::vector<double> y2;
    std::vector<double> z2;
    std::vector<double> radius_1;
    std::vector<double> radius_2;
    std::vector<double> surfaceSize;
};
#endif //MIGHTYTYPES_H
