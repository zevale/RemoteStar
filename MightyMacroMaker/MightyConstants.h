#ifndef MIGHTYCONSTANTS_H
#define MIGHTYCONSTANTS_H

#include <string>

// Math
namespace Math {
    constexpr int precision (10);
}

namespace Default{
    // ---------- GENERIC
    const char* const filePath = "empty_path";
    constexpr     int scalar   = 32;

    // ---------- PRISM LAYER MESH (STANDARD CONFIG AS PER SIEMENS RECOMMENDATIONS)
    constexpr double  boundaryMarchAngle      (85);
    constexpr double  minimumThickness        (5);
    constexpr double  layerChoppingPercentage (10);

    // ---------- SOLUTION MONITORS

    // ViewUp is along fuselage (X)
    constexpr double viewUpX (1);
    constexpr double viewUpY (0);
    constexpr double viewUpZ (0);

    // Normal is perpendicular to fuselage (Z)
    constexpr double normalX (0);
    constexpr double normalY (0);
    constexpr double normalZ (1);
}
#endif //MIGHTYCONSTANTS_H