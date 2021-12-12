#include <stdexcept>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Logic.h"
#include "Room.h"

const Image Logic::kYouDiedImage("resources/youdied.png");
const Image Logic::kYouWinImage("resources/youwin.png");

Logic::Logic()
    : room_()
    , player_(Point(kWindowWidth / 2, kWindowHeight / 2))
    , layers_() {
  InitLayers();
  InitRooms();
  InitDoors();
}

void Logic::InitLayers() {
  for (int i = 0; i < kInitialDepth; i++) {
    layers_.push_back(Image(Image::kBackgroundColor,
                            kWindowWidth, kWindowHeight));
  }
}

void Logic::Draw(double current_time) {
  room_->Draw(layers_[0], layers_[1], current_time);
  player_.Draw(layers_[2], current_time);
}

void Logic::InitRooms() {
  std::string line;
  int i = 0;
  std::ifstream master_file(kPath);
  if (!master_file.is_open())
    throw std::runtime_error("Bad map file");
  while (getline(master_file, line)) {
    if (line == "")
      continue;
    std::vector<Room*> new_row(line.size());
    rooms_.push_back(new_row);
    for (int j = 0; j < line.size(); j++) {
      if (line[j] == '.')
        rooms_[i][j] = nullptr;
      else if (line[j] == 'Q') {
        rooms_[i][j] = LOGIC_LABIRINT_EXIT;
      } else {
        rooms_[i][j] = new Room(line[j]);
        if (line[j] == '0') {
          room_ = rooms_[i][j];
          room_indices_.i = i;
          room_indices_.j = j;
        }
      }
    }
    i++;
  }
}

void Logic::InitDoors() {
  auto height = rooms_.size();
  for (int i = 0; i < height; i++) {
    auto width = rooms_[i].size();
    for (int j = 0; j < width; j++) {
      auto current = rooms_[i][j];
      if (!current || current == LOGIC_LABIRINT_EXIT)
        continue;

      std::map<Indices, Room::Dir> adjacents = {
          { { i + 1, j     }, Room::Dir::kDown  },
          { { i - 1, j     }, Room::Dir::kUp    },
          { { i    , j + 1 }, Room::Dir::kRight },
          { { i    , j - 1 }, Room::Dir::kLeft  } };


      for (auto [ind, dir] : adjacents) {
        if (ind.i >= height || ind.j >= width || !rooms_[ind.i][ind.j])
          continue;
        auto adjacent = rooms_[ind.i][ind.j];
        if (adjacent == LOGIC_LABIRINT_EXIT) {
          current->AddDoor(dir, true);
          continue;
        }
        auto door1 = current->AddDoor(dir, false);
        auto opposite_dir = Room::kOpposite.find(dir)->second;
        if (!adjacent->DoorExists(opposite_dir))
          continue;
        auto door2 = adjacent->GetDoor(opposite_dir);
        door1->SetPair(door2);
        door2->SetPair(door1);
      }
    }
  }
}

void Logic::PlayerProcessInput(MovementDir dir) {
  if (!game_over_)
    player_.ProcessInput(dir);
}

void Logic::Process() {

  if (player_.IsDead()) {
    layers_.push_back(kYouDiedImage);
    game_over_ = true;
  } else if (player_.HasWon()) {
    layers_.push_back(kYouWinImage);
    game_over_ = true;
  }

  room_->ProcessCollisions(player_);

  if (player_.GetX() > kWindowWidth) {
    ChangeRoom(Dir::kRight);
  } else if (player_.GetX() < 0) {
    ChangeRoom(Dir::kLeft);
  } else if (player_.GetY() > kWindowHeight) {
    ChangeRoom(Dir::kDown);
  } else if (player_.GetY() < 0) {
    ChangeRoom(Dir::kUp);
  }
}

void Logic::ChangeRoom(Dir dir) {
  if (dir == Dir::kRight) {
    room_indices_.j += 1;
    player_.SetX(kWindowWidth - player_.GetX() + kBorder);
  } else if (dir == Dir::kLeft) {
    room_indices_.j -= 1;
    player_.SetX(kWindowWidth - player_.GetX() - kBorder);
  } else if (dir == Dir::kDown) {
    room_indices_.i += 1;
    player_.SetY(kWindowHeight - player_.GetY() + kBorder);
  } else if (dir == Dir::kUp) {
    room_indices_.i -= 1;
    player_.SetY(kWindowHeight - player_.GetY() - kBorder);
  }
  if(!rooms_[room_indices_.i][room_indices_.j]) {
    player_.GoBack();
  } else {
    room_->Erase(layers_[1]);  // Clear screen from objects
    room_ = rooms_[room_indices_.i][room_indices_.j];
  }
}

const std::vector<Image>& Logic::GetLayers() const {
  return layers_;
}
