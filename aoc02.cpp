#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// "aocXY.extension" -> "aocXY.txt"
#define PUZZLE_INPUT_FILENAME (std::string(__FILE__).substr(0, std::string(__FILE__).find('.') + 1) + "txt")


class Entry
{
public:
  int min;
  int max;
  char c;
  std::string password;

  friend std::istream &operator>>(std::istream &in, Entry &entry)
  {
    char unused;
    return in >> entry.min >> unused >> entry.max >> entry.c >> unused >> entry.password;
  }
};

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
  const auto elements = readFile<Entry>(PUZZLE_INPUT_FILENAME);

  return std::count_if(elements.begin(), elements.end(), [](const Entry &e) {
    const int occ = std::count(e.password.begin(), e.password.end(), e.c);
    return occ >= e.min && occ <= e.max;
  });
}

auto es2()
{
  const auto elements = readFile<Entry>(PUZZLE_INPUT_FILENAME);

  return std::count_if(elements.begin(), elements.end(), [](const Entry &e) {
    return (e.password[e.min - 1] == e.c) ^ (e.password[e.max - 1] == e.c);
  });
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
