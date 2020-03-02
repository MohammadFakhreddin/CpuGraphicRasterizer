#ifndef FaTexture_class
#define FaTexture_class

#include <string>

class FaTexture
{
private:
  int width;
  int height;
  int numberOfChannels;
  std::string address;
  unsigned char* data;
  float virtualImageWidth;
  float virtualImageHeight;
  float scaleX;
  float scaleY;
public:
  FaTexture(
    std::string address,
    float virtualImageWidth,
    float virtualImageHeight
  );
  ~FaTexture();
  int getWidth();
  int getHeight();
  int getNumberOfChannels();
  void getColorForPosition(
    float positionX,
    float positionY,
    float* red,
    float* green,
    float* blue
  );
  void print();
};

#endif