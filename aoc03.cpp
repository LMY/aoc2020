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

auto calc(std::vector<std::string> lines, int dx, int dy)
{
  uint count = 0; // "int" is not enough for es2()

  const int leny = lines.size();
  const int lenx = leny > 0 ? lines[0].size() : 0;

  for (int x = 0, y = 0; y < leny; x += dx, y += dy)
    if (lines[y][x % lenx] == '#')
      count++;

  return count;
}

auto es1()
{
  const auto elements = readFile<std::string>(PUZZLE_INPUT_FILENAME);
  return calc(elements, 3, 1);
}

auto es2()
{
  const auto elements = readFile<std::string>(PUZZLE_INPUT_FILENAME);
  return calc(elements, 1, 1) * calc(elements, 3, 1) * calc(elements, 5, 1) * calc(elements, 7, 1) * calc(elements, 1, 2);
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
