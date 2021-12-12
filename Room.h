#ifndef ROOM_H_SENTRY
#define ROOM_H_SENTRY

#include <memory>
#include <vector>
#include <map>

#include "Player.h"
#include "Tile.h"
#include "Object.h"

class Object;
class Door;

class Room {
 public:
  enum class Dir {
    kRight,
    kLeft,
    kUp,
    kDown
  };

  static constexpr auto kWidth = 15;
  static constexpr auto kHeight = 9;
  static const std::map<Dir, Dir> kOpposite;

  Room(char ch);
  void Draw(Image &tile_screen, Image &obj_screen, double current_time) const;
  void Erase(Image &obj_screen) const;
  void ProcessCollisions(Player& player);
  Door::DoorPtr& AddDoor(Dir dir, bool is_exit);
  Door::DoorPtr& GetDoor(Dir dir);
  void RemoveObject(int id);
  bool IsDoorLocked(Dir dir) const ;
  bool DoorExists(Dir dir) const;

  void LockDoor(Dir dir);

 private:
  static constexpr auto kPathPrefix = "resources/room_";
  static const std::map<Dir, Point> kDoorCoords;

  void ReadFile(char ch);
  void InitWalls();
  void AddDoor(Dir dir);

  using TilePtr = std::unique_ptr<Tile>;
  using TileVector = std::vector<TilePtr>;
  using TileMatrix = std::vector<TileVector>;
  using ObjectPtr = std::unique_ptr<Object>;
  using ObjectVector = std::vector<ObjectPtr>;
  using DoorMap = std::map<Dir, Door::DoorPtr>;

  int keys_;
  DoorMap doors_;
  TileMatrix tiles_;
  ObjectVector objects_;
};


#endif // ROOM_H_SENTRY
