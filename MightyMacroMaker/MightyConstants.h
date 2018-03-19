#ifndef MIGHTYCONSTANTS_H
#define MIGHTYCONSTANTS_H

#include <string>

// Math
namespace Math {
    constexpr int precision (10);
}

namespace Default{
    // ---------- GENERIC
    const char* const filePath      ("empty_path");
    constexpr     int scalar        (32);
    constexpr    bool boolean       (false);

    // ---------- PAUSE
    constexpr int pauseTime = 3000; // Milliseconds


    // ---------- AUTO SAVE
    constexpr int  numAutosaveFiles   (0);
    constexpr int  iterationInterval  (50);
    constexpr bool autoSaveMesh       (true);
    constexpr bool autoSaveSimulation (true);

    // ---------- SURFACE SIZE
    constexpr double minimumTargetSurfaceSize (0.001); // [m]

    // ---------- PRISM LAYER MESH (STANDARD CONFIG AS PER SIEMENS RECOMMENDATIONS)
    constexpr double  boundaryMarchAngle      (85);
    constexpr double  minimumThickness        (5);
    constexpr double  layerChoppingPercentage (10);

    // ---------- NO PRISM LAYER BY DEFAULT
    constexpr int zeroLayers(0);

    // ---------- VOLUMETRIC CONTROLS
    constexpr double surfaceSize (-1e-3); // Negative means no volumetric control

    // ---------- PHYSICS VALUES
    constexpr double dynamicViscosity (-1e3); // Negative means field is missing (inviscid)

    // ---------- SOLUTION MONITORS

    // ViewUp is along fuselage (X)
    constexpr double viewUpX (1);
    constexpr double viewUpY (0);
    constexpr double viewUpZ (0);

    // Normal is perpendicular to fuselage (Z)
    constexpr double normalX (0);
    constexpr double normalY (0);
    constexpr double normalZ (1);

    // ---------- STOPPING CRITERIA
    constexpr    int numSamples  (0);
    constexpr double asymptoticCL(-1e3); // Negative means field is missing
    constexpr double asymptoticCD(-1e3);
}

namespace CrossPlatform {
#ifdef _WIN32
    const char* const separator = "\\";
#endif
#if defined(linux) || defined(__APPLE__)
    const char* const separator = "/";
#endif
}

#endif //MIGHTYCONSTANTS_H