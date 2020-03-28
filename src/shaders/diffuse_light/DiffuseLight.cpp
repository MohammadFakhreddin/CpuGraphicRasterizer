#include "./DiffuseLight.h"

DiffuseLight::DiffuseLight(float positionX,float positionY,float positionZ)
  : Light(positionX, positionY, positionZ)
{}

void DiffuseLight::computeLightIntensity(
    Vec3DFloat& surfaceNormalVector,
    Vec3DFloat& surfaceCenter,
    Vec3DFloat& output
){
  
  lightDirectionVector.setX(surfaceCenter.getX() - worldPoint.get(0,0));
  lightDirectionVector.setY(surfaceCenter.getY() - worldPoint.get(1,0));
  lightDirectionVector.setZ(surfaceCenter.getZ() - worldPoint.get(2,0));
  
  float dotProductValue = 
      lightDirectionVector.dotProduct(surfaceNormalVector) 
      * -1;

  float angleValue = float(
    dotProductValue / (
      surfaceNormalVector.size() * lightDirectionVector.size()
    )
  );
  
  float lightIntensityFactor = Math::clamp(angleValue,0.0f,1.0f);
  output.setX(lightIntensityFactor);
  output.setY(lightIntensityFactor);
  output.setZ(lightIntensityFactor);

}