import random as rnd
from string import ascii_lowercase as alpha
import argparse

class Maze:
  def __init__(self):
    self.matrix = {}
    self.min_x = None
    self.min_y = None
    self.max_x = None
    self.max_y = None

  def __setitem__(self, key, value):
    self.min_x = key[0] if self.min_x is None else min(self.min_x, key[0])
    self.min_y = key[1] if self.min_y is None else min(self.min_y, key[1])
    self.max_x = key[0] if self.max_x is None else max(self.max_x, key[0])
    self.max_y = key[1] if self.max_y is None else max(self.max_y, key[1])
    self.matrix[key] = value

  def __getitem__(self, key):
    return self.matrix[key]

  def __contains__(self, key):
    return key in self.matrix

  def __str__(self):
    text = ""
    if self.min_x is None or self.max_x is None or self.min_y is None or self.max_y is None:
      return text
    for i in range(self.min_x, self.max_x + 1):
      for j in range(self.min_y, self.max_y + 1):
        if (i, j) not in self:
          text += "."
          continue
        text += self[(i, j)]
      text += "\n"
    return text


class Gen:

  def __init__(self, size, cardinality):
    self.maze = Maze()
    self.size = size
    self.cardinality = cardinality

  def generate(self):
    self.maze = Maze()
    coords = (0, 0)
    self.maze[coords] = '0';
    for i in range(self.size):
      coords = self.update_coords(coords)
      room_type = rnd.choice([alpha[i] for i in range(self.cardinality)])
      self.maze[coords] = room_type;
    self.maze[self.update_coords(coords)] = 'Q'
    return self.maze

  def update_coords(self, coords):
    choice = rnd.choice(["x+", "x-", "y+", "y-"])
    if choice == "x+":
      coords = (coords[0] + 1, coords[1])
    elif choice == "x-":
      coords = (coords[0] - 1, coords[1])
    elif choice == "y+":
      coords = (coords[0], coords[1] + 1)
    elif choice == "y-":
      coords = (coords[0], coords[1] - 1)
    if coords in self.maze:
      return self.update_coords(coords)
    return coords


def main():
  parser = argparse.ArgumentParser(description="Map generator")
  parser.add_argument("-n", "--types_number", type=int, default=4)
  parser.add_argument("-r", "--rooms_number", type=int, default=20)
  parser.add_argument("-s", "--size", type=int, default=[7, 10])
  args = parser.parse_args()

  gen = Gen(args.rooms_number, args.types_number)
  maze = gen.generate()
  print(maze, end="")

if __name__ == "__main__":
  main()
