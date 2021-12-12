#include "Tile.h"
#include <iostream>

const Image Wall::kLib[] = { Image("resources/wall.png") };
const Image Floor::kLib[] = { Image("resources/floor.png") };
const Image Void::kLib[] = { Image("resources/void.png") };

Tile::Tile(int i, int j)
    : coords_{ .x = j * Image::kTileSize, .y = i * Image::kTileSize } {
}


template<class Derived>
void SubTile<Derived>::Draw(Image &screen) {
  for (auto x = 0; x < Image::kTileSize; x++) {
    for (size_t y = 0; y < Image::kTileSize; y++) {
      screen.PutPixel(GetCoords().x + x, GetCoords().y + Image::kTileSize - y,
                      GetImage().GetPixel(x, y));
    }
  }
}

template<class Derived>
bool SubTile<Derived>::IsDeepEnough(Player &player) {
  return (player.GetX() < GetCoords().x + Image::kTileSize - Derived::kSoft &&
          player.GetX() + Image::kTileSize > GetCoords().x + Derived::kSoft &&
          player.GetY() < GetCoords().y + Image::kTileSize - Derived::kSoft &&
          player.GetY() + Image::kTileSize > GetCoords().y + Derived::kSoft);
}

void Wall::Interact(Player &player) {
  if (IsDeepEnough(player)) {
    player.GoBack();
  }
#if DEBUG
  Image tmp(Pixel({0, 255, 0, 255}));
  SetImage(tmp);
#endif
}

void Void::Interact(Player &player) {
  if (IsDeepEnough(player))
    player.YouDied();
}

void Floor::Interact(Player &player) {
#if DEBUG
  Image tmp(Pixel({0, 255, 0, 255}));
  SetImage(tmp);
#endif
}
