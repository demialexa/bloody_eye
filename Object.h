#ifndef OBJECT_H_SENTRY
#define OBJECT_H_SENTRY

#include <random>
#include "Image.h"
#include <memory>

class Room;
class Player;

class Object {
 public:
  Object(int x, int y)
      : coords_(x, y)
      , frame_(0) {}
  int &Frame() { return frame_; }

  virtual void Draw(Image& screen, double current_time);
  virtual void Erase(Image& screen) const;
  virtual const Image& GetImage() const = 0;
  virtual void SetImage(Image const * image) = 0;
  virtual void Interact(Player& player) = 0;
  virtual int GetWidth() const = 0;
  virtual int GetHeight() const = 0;
  virtual int GetFrames() const  = 0;
  virtual double GetIdleTime() const = 0;
  virtual double GetAnimationTime() const  = 0;


  bool IsDeepEnough(const Player& player, int depth) const;
  Point Coords() const { return coords_; }

  virtual ~Object() {}

 private:
  Point coords_;
  int frame_;
};

template<class Derived>
class SubObject: public Object {
 public:
  SubObject(int x, int y);
  int GetFrames() const override { return Derived::kFrames; }
  double GetIdleTime() const override { return Derived::kIdleTime; }
  double GetAnimationTime() const override { return Derived::kAnimationTime; }
  int GetHeight() const override { return Derived::kHeight; }
  int GetWidth() const override { return Derived::kWidth; }
  const Image& GetLib() { return Derived::kLib; }

  void SetImage(Image const * image) override { image_ = image; }
  const Image& GetImage() const override { return *image_; }

 private:
  Image const * image_;
};


class Key: public SubObject<Key> {
 public:
  static constexpr auto kSoftDepth = 32;
  static constexpr auto kFrames = 8;
  static constexpr auto kIdleTime = 5.0;
  static constexpr auto kAnimationTime = 0.1;
  static constexpr auto kHeight = 64;
  static constexpr auto kWidth = 64;
  static const Image kLib[];

  Key(int x, int y, int id, Room *room);

  int Id() const { return id_; }
  void Interact(Player& player) override;
  Room *GetRoom() { return room_; }
  void Draw(Image& screen, double current_time) override;
  void Eliminate() { exists_ = false; }
  bool Exists() const { return exists_; }


  virtual ~Key() {}

 private:

  bool exists_;
  int id_;
  double old_time_ = 0;
  Room *room_;
};

class Door : public SubObject<Door> {
 public:

   enum class Attributes {
     kLocked,
     kExit,
   };

  using DoorPtr = std::shared_ptr<Door>;

  static constexpr auto kSoftDepth = 32;
  static constexpr auto kFrames = 8;
  static constexpr auto kIdleTime = 5.0;
  static constexpr auto kAnimationTime = 0.1;
  static constexpr auto kHeight = 64;
  static constexpr auto kWidth = 64;
  static const Image kLib[];

  Door(int x, int y, const std::vector<Attributes>& attrs={});

  void Interact(Player &player) override;
  void Lock();
  void Unlock();
  bool IsLocked() const { return locked_; }
  void Draw(Image& screen, double current_time) override;
  void SetPair(DoorPtr& pair);

  virtual ~Door() {}

 private:
  bool locked_;
  bool closed_;
  bool exit_;
  DoorPtr pair_;
};



#endif // OBJECT_H_SENTRY
