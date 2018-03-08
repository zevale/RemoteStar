#ifndef MIGHTYTYPES_H
#define MIGHTYTYPES_H
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
    double X; // []
    double Y; // []
    double Z; // []
};

struct Velocity {
    double X; // [m/s]
    double Y; // [m/s]
    double Z; // [m/s]
};

struct LiftDirection {
    double X; // []
    double Y; // []
    double Z; // []
};

struct DragDirection {
    double X; // []
    double Y; // []
    double Z; // []
};

struct ViewUp {
    double X; // []
    double Y; // []
    double Z; // []
};

struct Normal {
    double X; // []
    double Y; // []
    double Z; // []
};
#endif //MIGHTYTYPES_H