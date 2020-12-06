#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

// "aocXY.extension" -> "aocXY.txt"
#define PUZZLE_INPUT_FILENAME (std::string(__FILE__).substr(0, std::string(__FILE__).find('.') + 1) + "txt")

template <class T>
static auto readFile(const std::string filename)
{
  std::ifstream infile(filename);
  std::vector<T> ret;
  T x;

  while (infile >> x)
    ret.push_back(x);

  return ret;
}

int getRow(std::string x)
{
  return (x[0] == 'B' ? 64 : 0) | (x[1] == 'B' ? 32 : 0) |
         (x[2] == 'B' ? 16 : 0) | (x[3] == 'B' ? 8 : 0) |
         (x[4] == 'B' ? 4 : 0) | (x[5] == 'B' ? 2 : 0) |
         (x[6] == 'B' ? 1 : 0);
}

int getColumn(std::string x)
{
  return (x[7] == 'R' ? 4 : 0) | (x[8] == 'R' ? 2 : 0) | (x[9] == 'R' ? 1 : 0);
}

int getIdForEntry(std::string x)
{
  int row = getRow(x);
  int column = getColumn(x);

  return row * 8 + column;
}

auto es1()
{
  const auto lines = readFile<std::string>(PUZZLE_INPUT_FILENAME);

  int max = 0;

  for (auto x : lines)
  {
    auto id = getIdForEntry(x);

    if (id > max)
      max = id;
  }

  return max;
}

auto es2()
{
  const auto lines = readFile<std::string>(PUZZLE_INPUT_FILENAME);

  bool seats[500][8] = { 0 };

  int minrow = 499;
  int maxrow = 0;

  for (auto x : lines)
  {
      int row = getRow(x);
      int column = getColumn(x);
      seats[row][column] = true;

      if (row > maxrow) maxrow = row;
      if (row < minrow) minrow = row;
  }

  for (int y=minrow+1; y<maxrow; y++)
    {
      std::vector<int> selected;

      for (int x=0; x<8; x++)
        if (!seats[y][x])
          selected.push_back(x);

      if (selected.size() != 8)
        for (int x : selected)
          return y * 8 + x;
    }

  return 0;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
