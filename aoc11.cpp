#include <algorithm>
#include <cmath> 
#include <fstream>
#include <iostream>
#include <numeric>
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

static inline bool areValidCoords(int x, int y, int lenx, int leny)
{
  return x >= 0 && y >= 0 && x < lenx && y < leny;
}

static auto countNeigh(const std::vector<std::string> input, int x, int y)
{
  int cnt = 0;

  for (int dy=-1; dy<=+1; dy++)
    for (int dx=-1; dx<=+1; dx++)
      if ((dx != 0 || dy != 0) && areValidCoords(x + dx, y + dy, input[0].size(), input.size()))
        if (input[y+dy][x+dx] == '#')
          cnt++;

  return cnt;
}

static auto evolve(const std::vector<std::string> input)
{
  // allocate enough space
  std::vector<std::string> output = input;

  for (uint y=0; y<input.size(); y++)
    for (uint x=0; x<input[y].size(); x++)
      if (input[y][x] != '.')
      {
        const int neigh = countNeigh(input, x, y);  

        if (input[y][x] == 'L' && neigh == 0)
          output[y][x] = '#';
        else if (input[y][x] == '#' && neigh >= 4)
          output[y][x] = 'L';
      }

  return output;
}

static auto countSeated(const std::vector<std::string> input)
{
  return std::accumulate(input.begin(), input.end(), 0, [](int value, const auto s) { return value + std::count(s.begin(), s.end(), '#'); });
}

// static void printSeats(const std::vector<std::string> input)
// {
//   std::for_each(input.begin(), input.end(), [](auto x) { std::cout << x << std::endl; });
// }

auto es1()
{
  auto lines = readFile<std::string>(PUZZLE_INPUT_FILENAME);
  auto newone = lines;

  do
  {
    lines = newone;
    // printSeats(newone);
    newone = evolve(lines);
  }
  while (newone != lines);

  return countSeated(lines);
}

auto countNeigh2(const std::vector<std::string> input, int x, int y)
{
  const static int DIR_X[] = { -1,  0, +1, +1, +1,  0, -1, -1 };
  const static int DIR_Y[] = { -1,  -1, -1, 0, +1, +1, +1,  0 };

  int cnt = 0;

  const auto leny = input.size();
  const auto lenx = input[0].size();

  for (int i=0; i<8; i++)
    for (int dist = 1; ; dist++)
    {
      const auto dx = DIR_X[i] * dist;
      const auto dy = DIR_Y[i] * dist;

      if (!areValidCoords(x + dx, y + dy, lenx, leny) || input[y + dy][x + dx] == 'L')
        break;

      if (input[y + dy][x + dx] == '#')
      {
        cnt++;
        break;
      }
    }

  return cnt;
}

auto evolve2(const std::vector<std::string> input)
{
  // allocate enough space
  std::vector<std::string> output = input;

  for (uint y=0; y<input.size(); y++)
    for (uint x=0; x<input[y].size(); x++)
      if (input[y][x] != '.')
      {
        const int neigh = countNeigh2(input, x, y);  

        if (input[y][x] == 'L' && neigh == 0)
          output[y][x] = '#';
        else if (input[y][x] == '#' && neigh >= 5)
          output[y][x] = 'L';
      }

  return output;
}

auto es2()
{
  auto lines = readFile<std::string>(PUZZLE_INPUT_FILENAME);
  auto newone = lines;

  do
  {
    lines = newone;
    // printSeats(newone);
    newone = evolve2(lines);
  }
  while (newone != lines);

  return countSeated(lines);
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
