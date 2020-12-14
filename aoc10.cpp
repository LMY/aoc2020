#include <algorithm>
#include <cmath> 
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

auto es1()
{
  auto lines = readFile<int>(PUZZLE_INPUT_FILENAME);
  std::sort(lines.begin(), lines.end());

  int diff1 = 0;
  int diff3 = 1;  // +1 for the last at "higher + 3"

  for (uint i=0; i<lines.size(); i++)
  {
    auto diff = lines[i] - (i == 0 ? 0 : lines[i - 1]);

    if (diff == 1)
      diff1++;
    else if (diff == 3)
      diff3++;
  }

  return diff1 * diff3;
}

auto es2()
{
  auto lines = readFile<int>(PUZZLE_INPUT_FILENAME);
  std::sort(lines.begin(), lines.end());

  int consec_ones = 0;

  // track consecutive 1-deltas (ie 0, 1, 2 = 3 consecutive 1-deltas)
  std::vector<int> consec(lines.size());

  for (uint i=0; i<lines.size(); i++)
  {
    auto diff = lines[i] - (i == 0 ? 0 : lines[i - 1]);

    if (diff == 1)
      consec_ones++;
    else
      {
        consec[consec_ones]++;
        consec_ones = 0;
      }
  }

  consec[consec_ones]++;

  // 0 1 2 5     -> delta = 1 1 3,     2 consec1, the "1" is optional -> x2
  // 0 1 2 3 6   -> delta = 1 1 1 3,   3 consec1, the "1" and "2" are optional -> x4
  // 0 1 2 3 4 7 -> delta = 1 1 1 1 3, 4 consec1, the "1","2","3" are optional x8 BUT they can't be all 3 missing... -1 -> x7
  // 5 consec1 are x13, never happens in my input
  return (uint64_t)std::pow(2, consec[2]) * (uint64_t)std::pow(4, consec[3]) * (uint64_t)std::pow(7, consec[4]) * (uint64_t)std::pow(13, consec[5]);
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
