#ifndef IPhoneHelperAbstraction_class
#define IPhoneHelperAbstraction_class

#include "./IPhoneHelper-C-Interface.h"

class IPhoneHelperAbstraction{
private:
    static IPhoneHelperAbstraction* instance;
    void * objectiveCIPhoneHelperInstance;
public:
    static IPhoneHelperAbstraction* getInstance();
    IPhoneHelperAbstraction(void * objectiveCIPhoneHelperInstance);
    unsigned char * callObjectiveCToLoadImage(
        std::string imageName,
        int * width,
        int * height,
        int * numberOfChannels
    );
    void callObjectiveCToLog(std::string text);
};

#endif
