#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

// "aocXY.extension" -> "aocXY.txt"
#define PUZZLE_INPUT_FILENAME (std::string(__FILE__).substr(0, std::string(__FILE__).find('.') + 1) + "txt")

static auto readFileLines(const std::string filename)
{
  std::ifstream infile(filename);
  std::vector<std::string> ret;
  std::string x;

  while (std::getline(infile, x))
    ret.push_back(x);

  return ret;
}

auto apply_mask(uint64_t value, std::string mask)
{
  for (uint i = 0, len = mask.size(); i < len; i++)
    if (mask[len - 1 - i] == '1')
      value |= (1lu << i);
    else if (mask[len - 1 - i] == '0')
      value &= ~(1lu << i);

  return value;
}

auto es1()
{
  auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::map<uint64_t, uint64_t> memory;
  std::string mask;

  for (auto s : lines)
    if (s.rfind("mask", 0) == 0)
      mask = s.substr(s.find("=") + 2);
    else
    {
      const uint64_t address = std::stoi(s.substr(s.find("[") + 1));
      const uint64_t value = std::stoi(s.substr(s.find("=") + 2));

      memory[address] = apply_mask(value, mask);
    }

  return std::accumulate(memory.begin(), memory.end(), 0lu, [](uint64_t x, const std::pair<uint64_t, uint64_t> entry){ return x + entry.second; });
}

void apply_mask2(std::map<uint64_t, uint64_t>& memory, uint64_t address, uint64_t value, std::string mask)
{
  std::vector<int> floating;

  for (uint i = 0, len = mask.size(); i < len; i++)
    if (mask[len - 1 - i] == '1')
      address |= (1lu << i);
    else if (mask[len - 1 - i] == 'X')
      floating.push_back(i);

  const auto len = floating.size();

  // fill bits in positions contained in floating with all possible values
  for (uint i=0, imax=std::pow(2, len); i<imax; i++)
  {
    // address X-bits will be overwritten with all possible values
    // right-left or left-right is not important, we are covering *all* values
    auto kkk = i;

    for (uint k = 0; k < len; k++)
    {
      if (kkk & 1)
        address |= (1lu << floating[k]);
      else
        address &= ~(1lu << floating[k]);

      kkk /= 2;
    }

    memory[address] = value;
  }
}

auto es2()
{
  auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::map<uint64_t, uint64_t> memory;
  std::string mask;

  for (auto s : lines)
    if (s.rfind("mask", 0) == 0)
      mask = s.substr(s.find("=") + 2);
    else
    {
      const uint64_t address = std::stoi(s.substr(s.find("[") + 1));
      const uint64_t value = std::stoi(s.substr(s.find("=") + 2));

      apply_mask2(memory, address, value, mask);
    }

  return std::accumulate(memory.begin(), memory.end(), 0lu, [](uint64_t x, const std::pair<uint64_t, uint64_t> entry){ return x + entry.second; });
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
