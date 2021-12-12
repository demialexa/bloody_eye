#ifndef TILE_H_SENTRY
#define TILE_H_SENTRY

#include<iostream>
#include <vector>
#include "Image.h"
#include "Player.h"

class Tile {
 public:
   Tile(int i, int j);

  virtual void Draw(Image &screen) = 0;
  virtual void Interact(Player &player) = 0;

  Point GetCoords() const { return coords_; }

  virtual ~Tile() {}

 private:
  Point coords_;
};

// Curiously Recurring Template Pattern
template<class Derived>
class SubTile : public Tile {
 public:
  SubTile(int i, int j)
      : Tile(i, j)
      , image_(Derived::kLib[0]) {}

  void Draw(Image &screen) override;

  const Image& GetImage() const { return image_; }
  void SetImage(Image img) { image_ = img; }

  bool IsDeepEnough(Player &player);

  virtual ~SubTile<Derived>() {}

 private:
  Image image_;
};


class Wall : public SubTile<Wall> {
 public:
  Wall(int x, int y) : SubTile<Wall>(x, y) {}

  void Interact(Player &player) override;

  static constexpr auto kSoft = Image::kTileSize / 4;
  static const Image kLib[];

  virtual ~Wall() {}

 private:
};


class Floor : public SubTile<Floor> {
 public:
  Floor(int x, int y)
      : SubTile<Floor>(x, y) {}

  void Interact(Player &player) override;

  static constexpr auto kSoft = Image::kTileSize / 2;
  static const Image kLib[];

  virtual ~Floor() {}

 private:
};


class Void : public SubTile<Void> {
 public:
  Void(int x, int y) : SubTile<Void>(x, y) {}

  void Interact(Player &player) override;

  static constexpr auto kSoft = Image::kTileSize / 2;
  static const Image kLib[];

  virtual ~Void() {}

 private:
};

#endif // TILE_H_SENTRY
