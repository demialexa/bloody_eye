#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>

struct Pixel {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

struct Point {
  Point(int a_x, int a_y) : x(a_x), y(a_y) {}
  int x;
  int y;
};


class Image {
 public:
  static constexpr auto kTileSize = 64;
  static const Pixel kBackgroundColor;
  explicit Image(const std::string &a_path);
  Image(Pixel pix = kBackgroundColor, int a_width = kTileSize, int a_height = kTileSize);
  Image(const Image& other) = default;


  int Save(const std::string &a_path);

  int Width()    const { return width_; }
  int Height()   const { return height_; }
  int Channels() const { return channels_; }
  size_t Size()  const { return size_; }
  const Pixel *Data() const { return  data_; }


  Pixel GetPixel(int x, int y) const;
  void  PutPixel(int x, int y, const Pixel &pix);

  void Delete();

private:
  int width_ = -1;
  int height_ = -1;
  int channels_ = 4;
  size_t size_ = 0;
  Pixel *data_;
  bool self_allocated_ = false;
};



#endif //MAIN_IMAGE_H
