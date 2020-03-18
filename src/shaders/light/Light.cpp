#include "./Light.h"

#include <cassert>

#include "./../../utils/log/Logger.h"

void Light::computeLightIntensity(
    Vec3DFloat& surfaceNormalVector,
    Vec3DFloat& surfaceCenter,
    short int surfaceDirectionFactor,
    Vec3DFloat& output
){
    Logger::log("Method getLightIntensity is not implemented correctly in child class");
    assert(false);
}
