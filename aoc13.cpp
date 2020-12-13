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
  auto lines = readFile<std::string>(PUZZLE_INPUT_FILENAME);

  uint64_t init = std::stoi(lines[0]);

  std::vector<int> bus;

  // get all the ints in "2,4,x,x,x,17,x,31,x" -> { 2, 4, 17, 31 }
  // https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
  size_t pos = 0;

  while ((pos = lines[1].find(",")) != std::string::npos)
  {
      std::string token = lines[1].substr(0, pos);
      
      if (token != "x")
        bus.push_back(std::stoi(token));

      lines[1].erase(0, pos + 1);
  }

  if (lines[1] != "x")
    bus.push_back(std::stoi(lines[1]));

  for (auto selected=init; ; selected++)
    for (int x : bus)
      if (selected % x == 0)
        return (selected-init) * x;

  return 0lu;
}

auto es2()
{
  auto lines = readFile<std::string>(PUZZLE_INPUT_FILENAME);


  std::vector<int> bus;

  size_t pos = 0;

  while ((pos = lines[1].find(",")) != std::string::npos)
  {
      std::string token = lines[1].substr(0, pos);

      bus.push_back(token != "x" ? std::stoi(token) : -1);

      lines[1].erase(0, pos + 1);
  }

  bus.push_back(lines[1] != "x" ? std::stoi(lines[1]) : -1);
  // uint64_t init = std::stoi(lines[0]);

  for (uint64_t selected = 0; ; selected++)
  {
    if (selected % 1000 == 0)
      std::cout << selected << std::endl;

    bool win = true;

    for (uint i=0; i<bus.size(); i++)
      if (bus[i] >= 0)
        if ((selected + i) % bus[i] != 0)
        {
          win = false;
          // std::cout << "lost because [" << i << "] " << selected << " % " << bus[i] << " = " << ((selected + i) % bus[i]) << " != " << 0 << std::endl;
          break;
        }

    if (win)
      return selected;
  }

  return 0lu;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
