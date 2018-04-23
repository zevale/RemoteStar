/*
 * CLASS MightyScene
 *
 * DESCRIPTION
 * Scene: aircraft Cp contours and domain boundaries' mach contours
 *
 *          Creator: Nuno Alves de Sousa
 *           E-mail: nunoalvesdesousa@me.com
 *
 */
#ifndef MIGHTYSCENE_H
#define MIGHTYSCENE_H

#include <vector>
#include <string>

class MightyScene {
private:
    std::vector<std::string> regionName;

public:
    // Constructor
    explicit MightyScene(const std::vector<std::string>& _regionName = {});

    // Member function
    std::vector<std::string> mightySceneCode();
};
#endif //MIGHTYSCENE_H
