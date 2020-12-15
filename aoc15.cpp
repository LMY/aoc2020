#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

// "aocXY.extension" -> "aocXY.txt"
#define PUZZLE_INPUT_FILENAME (std::string(__FILE__).substr(0, std::string(__FILE__).find('.') + 1) + "txt")

static std::vector<int> readInput(const std::string filename)
{
  std::ifstream infile(filename);
  std::vector<int> ret;
  int x;
  char c;

  while (infile >> x)
  {
    ret.push_back(x);
    infile >> c;
  }

  return ret;
}

auto es1()
{
  auto input = readInput(PUZZLE_INPUT_FILENAME);

  while (input.size() != 2020)
  {
    auto len = input.size();

    int tobeinserted = 0;

    for (int i=len-2; i>=0; i--)
      if (input[i] == input[len - 1])
      {
        tobeinserted = (len-1) - i;
        break;
      }

    input.push_back(tobeinserted);
  }

  return input[input.size() - 1];
}

auto es2()
{
  auto input = readInput(PUZZLE_INPUT_FILENAME);

  // map: value -> last turn it happen
  std::map<int, int> memory;

  for (uint i=0; i<input.size()-1; i++)
    memory[input[i]] = i;

  auto last = input[input.size()-1];

  for (uint turns = input.size()-1; turns < 30000000 - 1; turns++)
    if (memory.count(last) == 0)
    {
      memory[last] = turns;
      last = 0;
    }
    else
    {
      auto oldv = memory[last];
      memory[last] = turns;
      last = turns-oldv;
    }

  return last;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
