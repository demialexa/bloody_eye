#ifndef LOGIC_H_SENTRY
#define LOGIC_H_SENTRY

#include <vector>
#include "Image.h"
#include "Player.h"

class Room;

class Logic {
 public:

  #define LOGIC_LABIRINT_EXIT ((Room*)-1)
  static constexpr auto kWindowWidth = 960;
  static constexpr auto kWindowHeight = 576;
  static constexpr auto kInitialDepth = 3;
  static constexpr auto kBorder= 64;

  Logic();
  void Draw(double current_time);
  void PlayerProcessInput(MovementDir dir);
  void Process();
  const std::vector<Image>& GetLayers() const;

 private:
  using Rooms = std::vector<std::vector<Room*>>;

  enum class Dir {
    kRight,
    kLeft,
    kDown,
    kUp
  };

  struct Indices {
    bool operator<(const Indices& other) const {
      return i < other.i || i == other.i && j < other.j;
    }
    int i, j;
  };

  static constexpr auto kPath = "resources/master";
  static const Image kYouDiedImage;
  static const Image kYouWinImage;

  void InitLayers();
  void InitRooms();
  void InitDoors();
  void ChangeRoom(Dir dir);

  Room *room_;
  Rooms rooms_;
  Indices room_indices_;
  Player player_;
  bool game_over_;
  std::vector<Image> layers_;
  std::vector<std::string> map_;
};

#endif // LOGIC_H_SENTRY
