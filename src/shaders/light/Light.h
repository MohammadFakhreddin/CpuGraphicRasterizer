#ifndef Light_class
#define Light_class

#include "./../../data_types/VectorTemplate.h"

class Light{
public:
    virtual void computeLightIntensity(
        Vec3DFloat& surfaceNormalVector,
        Vec3DFloat& surfaceCenter,
        short int surfaceDirectionFactor,
        Vec3DFloat& output
    );
};

#endif