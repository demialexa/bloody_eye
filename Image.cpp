#include "Image.h"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <memory>

const Pixel Image::kBackgroundColor {0, 0, 0, 0};

Image::Image(const std::string &a_path) {
  data_ = (Pixel*)stbi_load(a_path.c_str(), &width_, &height_, &channels_, 0);
  if (data_ == nullptr) {
    std::cout << stbi_failure_reason() << " " << a_path << std::endl;
    throw std::runtime_error("Bad picture\n");
  }
  size_ = width_ * height_ * channels_;
}


Image::Image(Pixel pix, int a_width, int a_height) {
  data_ = new Pixel[a_width * a_height];

  if (data_ == nullptr)
    throw std::runtime_error("Unable to allocate memory for image\n");

  width_ = a_width;
  height_ = a_height;
  size_ = a_width * a_height * channels_;
  self_allocated_ = true;

  for (size_t i = 0; i < width_ * height_; i++)
    data_[i] = pix;
}

#if 0
Image::Image(const Image& other)
    : width_(other.width_)
    , height_(other.height_)
    , channels_(other.channels_)
    , size_(other.size_)
    , self_allocated_(other.self_allocated_)
    , data_(other.data_) {
  data_ = new Pixel[size_];
  for (int i = 0; i < size_; i++)
    data_[i] = other.data_[i];
}
#endif



int Image::Save(const std::string &a_path) {
  auto extPos = a_path.find_last_of('.');
  if (a_path.substr(extPos, std::string::npos) == ".png"
      || a_path.substr(extPos, std::string::npos) == ".PNG") {
    stbi_write_png(a_path.c_str(), width_, height_,
                   channels_, data_, width_ * channels_);
  } else if (a_path.substr(extPos, std::string::npos) == ".jpg"
             || a_path.substr(extPos, std::string::npos) == ".JPG"
             || a_path.substr(extPos, std::string::npos) == ".jpeg"
             || a_path.substr(extPos, std::string::npos) == ".JPEG") {
    stbi_write_jpg(a_path.c_str(), width_, height_, channels_, data_, 100);
  } else {
    std::cerr << "Unknown file extension: "
              << a_path.substr(extPos, std::string::npos)
              << "in file name" << a_path << std::endl;
    return 1;
  }
  return 0;
}

Pixel Image::GetPixel(int x, int y) const {
  // if (x, y) is out of screen return blank pixel
  if (y < height_ && y >= 0 && x >= 0 && x < width_)
    return data_[width_ * (height_ - y - 1) + x];
  return Pixel{ 0, 0, 0, 0 };
}

void Image::PutPixel(int x, int y, const Pixel& pix) {
  // if (x, y) is out of screen do nothing
  if (y < height_ && y >= 0 && x >= 0 && x < width_)
    data_[width_* (height_ - y - 1) + x] = pix;
}

void Image::Delete() {
  if (self_allocated_) {
    delete [] data_;
    return;
  }
  stbi_image_free(data_);
}
