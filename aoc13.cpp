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

template<class T>
T modInverse(T a, T m) 
{
  a = a % m;
  for (T x = 1; x < m; x++)
    if ((a * x) % m == 1)
      return x;

  return 0lu;
} 

auto es2()
{
  // http://homepages.math.uic.edu/~leon/mcs425-s08/handouts/chinese_remainder.pdf
  auto lines = readFile<std::string>(PUZZLE_INPUT_FILENAME);

  std::vector<uint64_t> m;  // modules
  std::vector<uint64_t> a;  // remainders

  // read input
  size_t pos = 0;
  uint64_t where = 0;

  while ((pos = lines[1].find(",")) != std::string::npos)
  {
    std::string token = lines[1].substr(0, pos);

    if (token != "x")
    {
      auto bus = (uint64_t)std::stoi(token);
      m.push_back(bus);
      // (t+i) mod m[i] = i ->  t mod m[i] = -i  ->   t mod m[i] = m[i] - i ->  a[i] = m[i] - i
      a.push_back(bus - where);
    }

    lines[1].erase(0, pos + 1);
    where++;
  }

  if (lines[1] != "x")
  {
    auto bus = (uint64_t) std::stoi(lines[1]);
    m.push_back(bus);
    a.push_back(bus - where);
  }

  // std::cout << std::endl << "m:" << std::endl;
  // std::for_each(m.begin(), m.end(), [](uint64_t x) { std::cout << x << std::endl; });
  // std::cout << std::endl << "a:" << std::endl;
  // std::for_each(a.begin(), a.end(), [](uint64_t x) { std::cout << x << std::endl; });

  // calculate product M = prod m[i]
  auto product = std::accumulate(m.begin(), m.end(), 1lu, [](uint64_t x, const uint64_t entry){ return entry * x; });
  
  // std::cout << "product is " << product << std::endl;

  // calculate z[i] = product/m[i]
  std::vector<uint64_t> z;
  std::for_each(m.begin(), m.end(), [product, &z](uint64_t x) { z.push_back(product/x); });

  // std::cout << std::endl << "z:" << std::endl;
  // std::for_each(z.begin(), z.end(), [](uint64_t x) { std::cout << x << std::endl; });

  // calculate y[i] = z[i] mod_inverse m[i]
  std::vector<uint64_t> y(z.size());

  for (uint i=0; i<y.size(); i++)
    y[i] = modInverse(z[i], m[i]);

  // std::cout << std::endl << "y:" << std::endl;
  // std::for_each(y.begin(), y.end(), [](uint64_t x) { std::cout << x << std::endl; });

  // calculate w[i] = y[i] * z[i]
  std::vector<uint64_t> w(z.size());
  for (uint i=0; i<w.size(); i++)
    w[i] = y[i] * z[i];

  // std::cout << std::endl << "w:" << std::endl;
  // std::for_each(w.begin(), w.end(), [](uint64_t x) { std::cout << x << std::endl; });
  // std::cout << std::endl;

  // calculate x = sum a[i]*w[i]
  // solutions are all X : X = x + M k, for any uint k
  uint64_t ret = 0;

  for (uint i=0; i<w.size(); i++)
    ret += a[i]*w[i];

  return ret % product;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
