#ifndef DOMAIN_H
#define DOMAIN_H

#include <string>
#include <vector>
#include "MacroEnum.h"
#include "MightyConstants.h"

class Domain {
private:
    std::string geometryFilePath;
public:
    // Constructor
    explicit Domain(const std::string& _geometryFilePath = Default::filePath);

    // Member function
    std::vector<std::string> domainCode();
};


#endif //DOMAIN_H
