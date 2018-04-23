/*
 * CLASS AutoSave
 *
 * DESCRIPTION
 * Generates the macro autosave settings
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 */
#ifndef AUTOSAVE_H
#define AUTOSAVE_H

#include <vector>
#include <string>
#include "MightyConstants.h"

class AutoSave {
private:
    std::string simFilePath;
    int         numAutoSaveFiles;           // Number of files (0 to replace)
    int         iterationInterval;          // Interation interval between saving
    bool        autoSaveMesh;               // Save mesh
    bool        autoSaveSimulation;         // Enable auto save with intervals

public:
    // Constructor
    explicit AutoSave(const std::string& _simFilePath        = Default::filePath,
                      bool               _autoSaveMesh       = Default::boolean,
                      bool               _autoSaveSimulation = Default::boolean,
                      int                _numAutoSaveFiles   = Default::scalar,
                      int                _iterationInterval  = Default::scalar);

    // Member function
    std::vector<std::string> autoSaveCode();
};
#endif //AUTOSAVE_H
