#include "./Light.h"

#include <cassert>

#include "./../../utils/log/Logger.h"

void Light::computeLightIntensity(
    Vec3DFloat& surfaceNormalVector,
    Vec3DFloat& surfaceCenter,
    Vec3DFloat& output
){
    Logger::log("Method getLightIntensity is not implemented correctly in child class");
    assert(false);
}

void Light::transformX(float value){
    lightPositionVector.setX(value + lightPositionVector.getX());
}

void Light::transformY(float value){
    lightPositionVector.setY(value + lightPositionVector.getY());
}

void Light::transformZ(float value){
    lightPositionVector.setZ(value + lightPositionVector.getZ());
}

Vec3DFloat& Light::getLightPosition(){
    return lightPositionVector;
} 