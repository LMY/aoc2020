#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
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

auto es1()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  int count = 0;
  std::set<char> casts;

  for (auto l : lines)
  {
    if (l.empty())
    {
      count += casts.size();
      casts.clear();
    }
    else
      // insert all chars to th set
      std::for_each(l.begin(), l.end(), [&casts] (char c) { casts.insert(c); });
  }

  count += casts.size();

  return count;
}

auto setFromLine(std::string l)
{
  std::set<char> casts;
  std::for_each(l.begin(), l.end(), [&casts](char c) { casts.insert(c); });
  return casts;
}

auto es2()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  int count = 0;
  std::set<char> casts;
  bool nextinit = true;

  for (auto l : lines)
  {
    if (nextinit)
    {
      nextinit = false;
      casts = setFromLine(l);
    }
    else
    {
      if (l.empty())
      {
        count += casts.size();
        casts.clear();
        nextinit = true;
      }
      else
      {
        // get the set of chars for this line
        auto thisline = setFromLine(l);

        // calculate intersection: casts = casts \intersect thisline
        auto temp = casts;
        casts.clear();
        std::set_intersection(thisline.begin(), thisline.end(), temp.begin(), temp.end(), std::inserter(casts, casts.begin()));
      }
    }
  }

  count += casts.size();

  return count;
}

int main() {
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
