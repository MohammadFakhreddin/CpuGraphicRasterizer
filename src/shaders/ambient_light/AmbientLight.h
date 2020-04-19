#ifndef AmbientLight_Class
#define AmbientLight_Class

#include "../../data_types/MatrixTemplate.h"

class AmbientLight{

public:

  AmbientLight(
    const float& colorR,
    const float& colorG,
    const float& colorB
  );

  AmbientLight(
    const MatrixFloat& color
  );

  void computeLightIntensity(
    MatrixFloat& output
  ) const;

private:

  MatrixFloat color;

};

#endif // !AmbiantLight_Class
