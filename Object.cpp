#include "Object.h"
#include "Player.h"
#include "Room.h"
#include "Image.h"

const Image Key::kLib[] = {
    Image("resources/key.png")
};
const Image Door::kLib[] = {
    Image("resources/door.png"),
    Image("resources/door_lock_closed.png")
};

template <class Derived>
SubObject<Derived>::SubObject(int x, int y)
    : Object(x, y)
    , image_(&Derived::kLib[0]) {}

Key::Key(int x, int y, int id, Room *room)
    : SubObject<Key>(x, y)
    , id_(id)
    , room_(room)
    , exists_(true) {
}

void Object::Draw(Image &screen, double current_time) {
  const Image& image = GetImage();
  for (auto x = 0; x < GetWidth(); x++) {
    for (auto y = 0; y < GetHeight(); y++) {
      screen.PutPixel(coords_.x + x, coords_.y + image.Height() - y,
                      image.GetPixel(frame_ * Image::kTileSize + x, y));
    }
  }
}

void Key::Draw(Image &screen, double current_time) {
  if (!Frame() && current_time - old_time_ > GetIdleTime()) {
    old_time_ = current_time;
    Frame()++;
  } else if (Frame() && current_time - old_time_ > GetAnimationTime()) {
    old_time_ = current_time;
    Frame() = (Frame() + 1) % GetFrames();
  }
  if (!exists_) {
    Erase(screen);
    room_->RemoveObject(id_);
    return;
  }
  Object::Draw(screen, current_time);
}

void Object::Erase(Image& screen) const {
  const Image& image = GetImage();
  for (auto x = 0; x < GetWidth(); x++) {
    for (auto y = 0; y < GetHeight(); y++) {
      screen.PutPixel(coords_.x + x, coords_.y + image.Height() - y,
                      Image::kBackgroundColor);
    }
  }
}

void Key::Interact(Player& player) {
  if (!IsDeepEnough(player, kSoftDepth)) {
    return;
  }
  player.GiveKey();
  Eliminate();
}

bool Object::IsDeepEnough(const Player& player, int depth) const {
  const Image& image = GetImage();
  return player.GetX() < Coords().x + GetWidth() - depth &&
         player.GetX() + player.GetWidth() > Coords().x + depth &&
         player.GetY() < Coords().y + GetHeight() - depth &&
         player.GetY() + player.GetHeight() > Coords().y + depth;
}


Door::Door(int x, int y, const std::vector<Attributes>& attrs)
    : SubObject<Door>(x, y)
    , locked_(false)
    , closed_(true)
    , pair_(nullptr)
    , exit_(false) {
  for (auto attr : attrs) {
    if (attr == Attributes::kLocked) {
      Lock();
    }
    if (attr == Attributes::kExit)
      exit_ = true;
  }
}

void Door::Lock() {
  if (locked_)
    return;
  locked_ = true;
  SetImage(&kLib[1]);
  if (pair_ != nullptr)
    pair_->Lock();
}

void Door::Unlock() {
  if (!locked_)
    return;
  locked_ = false;
  SetImage(&kLib[0]);
  if (pair_ != nullptr)
    pair_->Unlock();
}

void Door::SetPair(DoorPtr& pair) {
  pair_ = pair;
  if (pair_->IsLocked()) {
    Lock();
  }
}

void Door::Interact(Player &player) {
  if (!IsDeepEnough(player, kSoftDepth)) {
    closed_ = true;
    return;
  }
  if (IsLocked()) {
    if (player.WasteKey()) {
      Unlock();
    } else {
      player.GoBack();
    }
  } else {
    closed_ = false;
    if (exit_) {
      player.YouWin();
    }
  }
#ifdef DEBUG
  Image tmp(Pixel({255, 0, 0, 50}));
  SetImage(tmp);
#endif
}

void Door::Draw(Image& screen, double current_time) {
  if (closed_ && Frame() != 0) {
    Frame()--;
  } else if (!closed_ && GetImage().Width() >= (Frame() + 2) * Image::kTileSize) {
    Frame()++;
  }
  Object::Draw(screen, current_time);
}
