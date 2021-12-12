#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"

enum class MovementDir {
  kUp,
  kDown,
  kLeft,
  kRight
};

class Player {
 public:
  explicit Player(Point pos = {.x = 65, .y = 65})
      : coords_(pos)
      , old_coords_(coords_) {}
  Player(const Player& other) = delete;
  Player(Player&& other) = delete;

  bool Moved() const;
  void ProcessInput(MovementDir dir); /* to delete */
  void Draw(Image &screen, double current_time);
  void Erase(Image& screen) const;
  void Update(double current_time);
  int GetWidth() const { return kWidth; }
  int GetHeight() const { return kHeight; }
  int GetFrames() const { return image_.Width() / kWidth; }
  double GetAnimationTime() const { return kAnimationTime; }
  double GetIdleTime() const { return kIdleTime; }
  const Image& GetImage() const { return image_; }
  void SetY(int y) { coords_.y = y; }
  void SetX(int x) { coords_.x = x; }
  void SetOldX(int x) { old_coords_.x = x; }
  void SetOldY(int y) { old_coords_.y = y; }
  int GetX() const { return coords_.x; }
  int GetY() const { return coords_.y; }
  void GoBack() {
    coords_ = old_coords_; }
  void YouDied() { is_dead_ = true; }
  void YouWin() { win_ = true; }
  bool IsDead() { return is_dead_; }
  bool HasWon() { return win_; }
  void GiveKey() { keys_++; }
  bool WasteKey() { return keys_ ? keys_-- : 0; }

 private:
  static constexpr auto kFramesInRow = 2;
  static constexpr auto kNumberOfFrames = 6;
  static constexpr auto kAnimationTime = 0.7;
  static constexpr auto kIdleTime = 0;
  static constexpr auto kImagePath = "resources/eye.png";
  static constexpr auto kWidth = 64;
  static constexpr auto kHeight = 64;



  Point coords_ {.x = 10, .y = 10};
  Point old_coords_ {.x = 10, .y = 10};
  double old_time_ = 0;
  int frame_ = 0;
  int move_rate_ = 9;
  bool is_dead_ = false;
  bool win_ = false;
  int keys_ = 0;
  Image image_ = Image(kImagePath);

};

#endif //MAIN_PLAYER_H
