#ifndef DiffuseLight_class
#define DiffuseLight_class

#include "./../../utils/math/Math.h"
#include "./../../data_types/VectorTemplate.h"
#include "./../light/Light.h"

class DiffuseLight:public Light{
public:
    DiffuseLight(float positionX,float positionY,float positionZ);
    void computeLightIntensity(
        Vec3DFloat& surfaceNormalVector,
        Vec3DFloat& surfaceCenter,
        short int surfaceDirectionFactor,
        Vec3DFloat& output
    ) override;
private:
    /**
     * 
     * Temporary data which is used to boost performance
     * 
    */
    Vec3DFloat lightDirectionVectorPlaceholder = Vec3DFloat(0.0f,0.0f,0.0f);
    float dotProductPlaceholder;
    float angleValuePlaceholder;
    float lightIntensityFactorPlaceholder;
};

#endif