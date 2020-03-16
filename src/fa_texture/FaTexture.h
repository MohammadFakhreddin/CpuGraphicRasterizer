#ifndef FaTexture_class
#define FaTexture_class

#include <string>

class FaTexture
{
private:
  int width;
  int height;
  int numberOfChannels;
  unsigned int dataLength;
  std::string address;
  float * data;
  float virtualImageWidth;
  float virtualImageHeight;
  float scaleX;
float scaleY;
  void getPixelForPosition(int x,int y,float *red,float *green,float *blue);
public:
  FaTexture(
    std::string address,
    float virtualImageWidth,
    float virtualImageHeight
  );
  ~FaTexture();
  int getRealImageWidth();
  int getRealImageHeight();
  float getVirtualImageWidth();
  float getVirtualImageHeight();
  int getNumberOfChannels();
  void getColorForPosition(
    float positionX,
    float positionY,
    float* red,
    float* green,
    float* blue
  );
  void render();
  std::string getAddress();
};

#endif