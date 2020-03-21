#ifndef Light_class
#define Light_class

#include "./../../data_types/VectorTemplate.h"

class Light{
public:
    virtual void computeLightIntensity(
        Vec3DFloat& surfaceNormalVector,
        Vec3DFloat& surfaceCenter,
        Vec3DFloat& output
    );
    void transformX(float value);
    void transformY(float value);
    void transformZ(float value);
    Vec3DFloat& getLightPosition();
protected:
    Vec3DFloat lightPositionVector = Vec3DFloat(0.0f,0.0f,0.0f);
};

#endif