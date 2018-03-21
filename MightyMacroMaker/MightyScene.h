//
// Created by Nuno on 3/20/2018.
//

#ifndef CREATESCENES_H
#define CREATESCENES_H


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

#endif //CREATESCENES_H