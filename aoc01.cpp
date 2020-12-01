#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
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

int es1()
{
  const auto elements = readFile<int>(PUZZLE_INPUT_FILENAME);
  const int n = elements.size();

  for (int y=0; y<n; y++)
    for (int x=y+1; x<n; x++)
      if (elements[y] + elements[x] == 2020)
        return elements[y] * elements[x];

  return INT_MIN;
}

int es2()
{
  const auto elements = readFile<int>(PUZZLE_INPUT_FILENAME);
  const int n = elements.size();

  for (int z=0; z<n; z++)
    for (int y=z+1; y<n; y++)
      for (int x=y+1; x<n; x++)
        if (elements[y] + elements[x] + elements[z] == 2020)
          return elements[z] * elements[y] * elements[x];

  return INT_MIN;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
