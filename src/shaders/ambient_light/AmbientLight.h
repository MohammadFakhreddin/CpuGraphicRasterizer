#ifndef AmbientLight_Class
#define AmbientLight_Class

#include "../../data_types/MatrixTemplate.h"

class AmbientLight{

public:

  AmbientLight(const float& colorR, const float& colorG, const float& colorB);

  AmbientLight(const Matrix3X1Float& paramColor);

  void computeLightIntensity(
    Matrix4X1Float& output
  ) const;

private:

  bool checkForColorValidation();

  Matrix3X1Float color;

};

#endif // !AmbiantLight_Class
