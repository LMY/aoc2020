#include <algorithm>
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

bool isSumOf2In25Before(uint idx, std::vector<uint64_t> values)
{
  for (uint y=idx-25; y<idx; y++)
    for (uint x=y+1; x<idx; x++)
      if (values[idx] == values[y] + values[x])
        return true;

  return false;
}

auto es1()
{
  const auto lines = readFile<uint64_t>(PUZZLE_INPUT_FILENAME);

  for (uint i=25; i<lines.size(); i++)
    if (!isSumOf2In25Before(i, lines))
      return lines[i];

  return 0lu;
}

auto es2()
{
  const auto lines = readFile<uint64_t>(PUZZLE_INPUT_FILENAME);

  const auto search_for = es1();

  for (uint from=0; from<lines.size(); from++)
    for (uint to=from+1; to<lines.size(); to++)
      {
        // if the sum of the elements: lines[from]+...+lines[to] = es1()
        uint sum = std::accumulate(lines.begin()+from, lines.begin()+to, 0);

        if (sum == search_for)
          // return the min + max in lines[from..to]
          return *std::min_element(lines.begin()+from, lines.begin()+to) + *std::max_element(lines.begin()+from, lines.begin()+to);
        // optimization: if the sum is greater than our element, no point in extending the range even more
        else if (sum > search_for)
          break;
      }

  return 0lu;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
