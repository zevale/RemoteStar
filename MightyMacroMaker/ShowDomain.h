#ifndef SHOWDOMAIN_H
#define SHOWDOMAIN_H


#include <vector>
#include<string>

class ShowDomain {
private:
    std::vector<std::string> regionName;
    std::vector<std::string> boundaryCondition;
public:
    // Constructor
    explicit ShowDomain(const std::vector<std::string>& _regionName        = {},
                        const std::vector<std::string>& _boundaryCondition = {});

    std::vector<std::string> showDomainCode();
};


#endif //MIGHTYMACROMAKER_SHOWDOMAIN_H