#include "../Texture.h"

class ImageTexture : public Texture {

private:

  int width;

  int height;

  int numberOfChannels;

  unsigned int dataLength;

  std::string address;

  float* data;

  float virtualImageWidth;

  float virtualImageHeight;

  float scaleX;

  float scaleY;
  /**
    Temporary variables
  */
  unsigned int currentPosition;

  unsigned int realPositionX;
  unsigned int realPositionY;

public:

  ImageTexture(
    std::string address,
    float virtualImageWidth,
    float virtualImageHeight
  );

  ~ImageTexture() override;

  void getPixelForPosition(
    const float& x,
    const float& y,
    float* red,
    float* green,
    float* blue
    ) override;
  /**
    For debugging texture
  */
  void update(double deltaTime, Camera& cameraInstance) override;

  int getRealImageWidth();

  int getRealImageHeight();

  float getVirtualImageWidth();

  float getVirtualImageHeight();

  int getNumberOfChannels();

  std::string getAddress();

private:

  void getDirectPixelColor(
    const unsigned int& positionX,
    const unsigned int& positionY,
    float* red,
    float* green,
    float* blue
    );

};