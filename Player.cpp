#include "Player.h"
#include <iostream>


bool Player::Moved() const
{
  if(coords_.x == old_coords_.x && coords_.y == old_coords_.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir) {
  if (dir == MovementDir::kUp) {
    coords_.y -= move_rate_;
  } else if (dir == MovementDir::kDown) {
    coords_.y += move_rate_;
  } else if (dir == MovementDir::kLeft) {
    coords_.x -= move_rate_;
  } else if (dir == MovementDir::kRight) {
    coords_.x += move_rate_;
  }
}

void Player::Update(double current_time) {
  if (!frame_ && current_time - old_time_ > GetIdleTime()) {
    old_time_ = current_time;
    frame_++;
  } else if (frame_ && current_time - old_time_ > GetAnimationTime()) {
    old_time_ = current_time;
    frame_ = (frame_ + 1) % GetFrames();
  }
}

void Player::Erase(Image& screen) const {
  const Image& image = GetImage();
  for (auto x = 0; x < GetWidth(); x++) {
    for (auto y = 0; y < GetHeight(); y++) {
      screen.PutPixel(old_coords_.x + x, old_coords_.y + GetHeight() - y,
                      Image::kBackgroundColor);
    }
  }
}

void Player::Draw(Image &screen, double current_time ) {
  const Image& image = GetImage();
  Update(current_time);
  if (Moved())
    Erase(screen);
  old_coords_ = coords_;
  for (auto x = 0; x < GetWidth(); x++) {
    for (auto y = 0; y < GetHeight(); y++) {
      screen.PutPixel(GetX() + x, GetY() + GetHeight() - y,
                      image.GetPixel(frame_ * Image::kTileSize + x, y));
    }
  }
}
