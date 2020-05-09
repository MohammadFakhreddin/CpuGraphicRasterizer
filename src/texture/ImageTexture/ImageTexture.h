#ifndef ImageTexture_Class
#define ImageTexture_Class

#include <functional>

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

public:

  ImageTexture(
    const std::string& address,
    const float& virtualImageWidth,
    const float& virtualImageHeight
  );

  ImageTexture(
    unsigned char* data,
    const int& numberOfChannels,
    const int& realImageWidth,
    const int& realImageHeight,
    const float& virtualImageWidth,
    const float& virtualImageHeight
  );

  ~ImageTexture() override;

  template <typename A>
  ImageTexture& operator=(const ImageTexture& rhs) = delete;

  template <typename A>
  ImageTexture(const ImageTexture& other) = delete;

  ImageTexture(ImageTexture&& other) = delete; // move constructor

  ImageTexture& operator=(const ImageTexture& other) = delete;// copy assignment

  ImageTexture& operator=(ImageTexture&& other) = delete; // move assignment

  void getPixelForPosition(
    const float& x,
    const float& y,
    float* red,
    float* green,
    float* blue
  ) const override;
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

  void assignFilter(std::function<void(float* red, float* green, float* blue)>* filter);

private:

  void getDirectPixelColor(
    const unsigned int& position,
    float* red,
    float* green,
    float* blue
  ) const;

  std::function<void(float* red, float* green, float* blue)>* filter = nullptr;

};

#endif // !ImageTexture_Class
