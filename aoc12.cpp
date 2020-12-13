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

struct Entry
{
  char c;
  int len;

  friend std::istream &operator>>(std::istream &in, Entry &entry)
  {
    return in >> entry.c >> entry.len;
  }
};

auto es1()
{
  const auto lines = readFile<Entry>(PUZZLE_INPUT_FILENAME);

  const int dx[] = { +1, 0, -1, 0 };
  const int dy[] = { 0, -1, 0, +1 };

  int dir = 0;
  int posx = 0;
  int posy = 0;

  for (const auto l : lines)
  {
    // std::cout << "(" << posx << ", " << posy << ")\t[" << dir << "]\t" << l.c << " " << l.len << std::endl;

    if (l.c == 'N')
      posy += l.len;
    else if (l.c == 'S')
      posy -=  l.len;
    else if (l.c == 'E')
      posx +=  l.len;
    else if (l.c == 'W')
      posx -=  l.len;
    else if (l.c == 'R')
      dir = (dir + (l.len / 90)) % 4;
    else if (l.c == 'L')
      dir = (dir + 4 - (l.len / 90)) % 4;
    else if (l.c == 'F')
    {
      posx += l.len * dx[dir];
      posy += l.len * dy[dir];
    }
  }
  // std::cout << "(" << posx << ", " << posy << ")\t[" << dir << "]" << std::endl;

  return std::abs(posx) + std::abs(posy);
}

auto es2()
{
  const auto lines = readFile<Entry>(PUZZLE_INPUT_FILENAME);

  int posx = 0;
  int posy = 0;
  int wayx = 10;
  int wayy = 1;

  for (const auto l : lines)
  {
    // std::cout << "(" << posx << ", " << posy << ")\t(" << wayx << ", " << wayy << ")\t" << l.c << " " << l.len << std::endl;

    if (l.c == 'N')
      wayy += l.len;
    else if (l.c == 'S')
      wayy -=  l.len;
    else if (l.c == 'E')
      wayx +=  l.len;
    else if (l.c == 'W')
      wayx -=  l.len;
    else if (l.c == 'R')
    {
      for (int i=0, imax = l.len / 90; i<imax; i++)
      {
        const int newwayx = wayy;
        const int newwayy = -wayx;
        wayx = newwayx;
        wayy = newwayy;
      }
    }
    else if (l.c == 'L')
    {
      for (int i=0, imax = l.len / 90; i<imax; i++)
      {
        const int newwayx = -wayy;
        const int newwayy = wayx;
        wayx = newwayx;
        wayy = newwayy;
      }
    }
    else if (l.c == 'F')
    {
      posx += l.len * wayx;
      posy += l.len * wayy;
    }
  }
  
  // std::cout << "(" << posx << ", " << posy << ")\t(" << wayx << ", " << wayy << ")" << std::endl;

  return std::abs(posx) + std::abs(posy);
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
