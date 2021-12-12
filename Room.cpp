#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include "Room.h"
#include "Object.h"

const std::map<Room::Dir, Room::Dir> Room::kOpposite = {
      { Dir::kDown , Dir::kUp    },
      { Dir::kUp   , Dir::kDown  },
      { Dir::kRight, Dir::kLeft  },
      { Dir::kLeft , Dir::kRight } };

const std::map<Room::Dir, Point> Room::kDoorCoords = {
      { Dir::kLeft , { 0, kHeight / 2 * Image::kTileSize  } },
      { Dir::kRight, { (kWidth - 1) * Image::kTileSize,
                       kHeight / 2 * Image::kTileSize     } },

      { Dir::kUp   , { kWidth / 2 * Image::kTileSize, 0   } },
      { Dir::kDown , { kWidth / 2 * Image::kTileSize,
                       (kHeight - 1) * Image::kTileSize } } };

Room::Room(char ch)
    : tiles_(kHeight)
    , keys_(0) {
  for (auto& row : tiles_)
    row.resize(kWidth);
  InitWalls();
  ReadFile(ch);
}

void Room::InitWalls() {
  for (int j = 0; j < kWidth; j++)
    tiles_[0][j] = TilePtr(new Wall(0, j));
  for (int j = 0; j < kWidth; j++)
    tiles_[kHeight - 1][j] = TilePtr(new Wall(kHeight - 1, j));
  for (int i = 0; i < kHeight; i++)
    tiles_[i][0] = TilePtr(new Wall(i, 0));
  for (int i = 0; i < kHeight; i++)
    tiles_[i][kWidth - 1] = TilePtr(new Wall(i, kWidth - 1));
}

void Room::ReadFile(char ch) {
  std::string line;
  size_t i = 1, j = 1;
  std::string path(kPathPrefix);
  path += ch;
  std::ifstream map_file(path);
  if (!map_file.is_open())
    throw std::runtime_error("Bad room file");
  while (getline(map_file, line)) {
    if (line.size() != kWidth - 2)
      throw std::runtime_error("Bad room");
    for (auto ch : line) {
      if (ch == '#') {
        tiles_[i][j] = TilePtr(new Wall(i, j));
      } else if (ch == '.') {
        tiles_[i][j] = TilePtr(new Floor(i, j));
      } else if (ch == ' ') {
        tiles_[i][j] = TilePtr(new Void(i, j));
      } else if (ch == 'K') {
        keys_++;
        int x = j * Image::kTileSize;
        int y = i * Image::kTileSize;
        int id = objects_.size();
        ObjectPtr key_ptr(new Key(x, y, id, this));
        objects_.push_back(std::move(key_ptr));
        tiles_[i][j] = TilePtr(new Floor(i, j));
      } else {
        throw std::runtime_error("Bad room");
      }
      j++;
    }
    i++;
    j = 1;
  }
  if (i != kHeight - 1)
    throw std::runtime_error("Bad room");
  map_file.close();
}

Door::DoorPtr& Room::AddDoor(Dir dir, bool is_exit=false) {
  const int x = kDoorCoords.find(dir)->second.x;
  const int y = kDoorCoords.find(dir)->second.y;

  std::vector<Door::Attributes> attrs;
  if (keys_) {
    keys_--;
    attrs.push_back(Door::Attributes::kLocked);
  }
  if (is_exit)
    attrs.push_back(Door::Attributes::kExit);
  int j = x / Image::kTileSize;
  int i = y / Image::kTileSize;
  tiles_[i][j] = TilePtr(new Floor(i, j));

  doors_[dir].reset(new Door(x, y, attrs));
  return doors_[dir];

}

Door::DoorPtr& Room::GetDoor(Dir dir) {
  return doors_[dir];
}

void Room::RemoveObject(int id) {
  objects_.erase(objects_.begin() + id);
}

bool Room::DoorExists(Dir dir) const {
  return doors_.find(dir) != doors_.end();
}

bool Room::IsDoorLocked(Dir dir) const {
  return doors_.find(dir)->second->IsLocked();
}

void Room::LockDoor(Dir dir) {
  doors_[dir]->Lock();
}

void Room::ProcessCollisions(Player& player) {
  auto i = player.GetY() / Image::kTileSize;
  auto j = player.GetX() / Image::kTileSize;
  std::vector<std::pair<int, int>> in_contact = {
      { i    , j     },
      { i + 1, j     },
      { i    , j + 1 },
      { i + 1, j + 1 } };

  for (auto tile : in_contact) {
    // check if tile is on screen
    if (tile.first >= kHeight || tile.first < 0
        || tile.second >= kWidth || tile.second < 0)
      continue;
    tiles_[tile.first][tile.second]->Interact(player);
  }
  for (const auto& obj : objects_) {
    obj->Interact(player);
  }
  for (const auto& [dir, door] : doors_) {
    if (!door)
      continue;
    door->Interact(player);
  }
}

void Room::Draw(Image &tile_screen, Image &obj_screen, double current_time) const {
  for (const auto& row : tiles_) {
    for (const auto& tile : row) {
      if (!tile)
        continue;
      tile->Draw(tile_screen);
    }
  }
  for (const auto& obj : objects_) {
    if (!obj)
      continue;
    obj->Draw(obj_screen, current_time);
  }
  for (const auto& [dir, door] : doors_) {
    if (!door)
      continue;
    door->Draw(obj_screen, current_time);
  }
}

void Room::Erase(Image &obj_screen) const {
  for (const auto& obj : objects_) {
    obj->Erase(obj_screen);
  }
  for (const auto& [dir, door] : doors_) {
    door->Erase(obj_screen);
  }
}
