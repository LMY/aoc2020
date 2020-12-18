#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>
#include <set>

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

struct pt3
{
  int x;
  int y;
  int z;

  pt3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

  bool operator< (const pt3& p2) const
  {
    if (x < p2.x)
      return true;
    else if (x > p2.x)
      return false;

    if (y < p2.y)
      return true;
    else if (y > p2.y)
      return false;

    return z < p2.z;            
  }
};

auto countNeigh3(std::set<pt3> state, uint x, uint y, int z)
{
  int count = 0;

  for (int dz = -1; dz <= +1; dz++)
    for (int dy = -1; dy <= +1; dy++)
      for (int dx = -1; dx <= +1; dx++)
        if (dz != 0 || dy != 0 || dx != 0)
          if (state.count(pt3(x + dx, y + dy, z + dz)) == 1)
            count++;

  return count;
}

auto evolve3(std::set<pt3> state)
{
  std::set<pt3> ret;

  int minx = 999;
  int maxx = -999;
  int miny = 999;
  int maxy = -999;
  int minz = 999;
  int maxz = -999;

  for (auto x : state)
  {
    if (x.x > maxx) maxx = x.x;
    if (x.x < minx) minx = x.x;

    if (x.y > maxy) maxy = x.y;
    if (x.y < miny) miny = x.y;

    if (x.z > maxz) maxz = x.z;
    if (x.z < minz) minz = x.z;
  }

  for (int z=minz-1; z<=maxz+1; z++)
    for (int y=miny-1; y<=maxy+1; y++)
      for (int x=minx-1; x<=maxx+1; x++)
      {
        int neigh = countNeigh3(state, x, y, z);

        const pt3 pos(x, y, z);

        // If a cube is active and exactly 2 or 3 of its neighbors are also active, the cube remains active.
        // Otherwise, the cube becomes inactive.
        if (state.count(pos) == 1)
        {
          if (neigh == 2 || neigh == 3)
            ret.insert(pos);
        }
        // If a cube is inactive but exactly 3 of its neighbors are active, the cube becomes active.
        // Otherwise, the cube remains inactive.          
        else
          if (neigh == 3)
            ret.insert(pos);
      }

  return ret;
}

auto es1()
{
  auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::set<pt3> state;

  for (uint y=0; y<lines.size(); y++)
    for (uint x=0, xmax=lines[y].size(); x<xmax; x++)
      if (lines[y][x] == '#')
        state.insert(pt3(x, y, 0));

  for (int i=0; i<6; i++)
    state = evolve3(state);

  return state.size();
}





struct pt4
{
  int x;
  int y;
  int z;
  int w;

  pt4(int _x, int _y, int _z, int _w) : x(_x), y(_y), z(_z), w(_w) {}

  bool operator< (const pt4& p2) const
  {
    if (x < p2.x)
      return true;
    else if (x > p2.x)
      return false;

    if (y < p2.y)
      return true;
    else if (y > p2.y)
      return false;

    if (z < p2.z)
      return true;
    else if (z > p2.z)
      return false;

    return w < p2.w;            
  }
};

auto countNeigh4(std::set<pt4> state, uint x, uint y, int z, int w)
{
  #pragma GCC diagnostic ignored "-Wmisleading-indentation"
  int count = 0;

  for (int dw = -1; dw <= +1; dw++)
    for (int dz = -1; dz <= +1; dz++)
      for (int dy = -1; dy <= +1; dy++)
        for (int dx = -1; dx <= +1; dx++)
          if (dw != 0 || dz != 0 || dy != 0 || dx != 0)
            if (state.count(pt4(x + dx, y + dy, z + dz, w + dw)) == 1)
              count++;

  return count;
}

auto evolve4(std::set<pt4> state)
{
  std::set<pt4> ret;

  int minx = 999;
  int maxx = -999;
  int miny = 999;
  int maxy = -999;
  int minz = 999;
  int maxz = -999;
  int minw = 999;
  int maxw = -999;

  for (auto x : state)
  {
    if (x.x > maxx) maxx = x.x;
    if (x.x < minx) minx = x.x;

    if (x.y > maxy) maxy = x.y;
    if (x.y < miny) miny = x.y;

    if (x.z > maxz) maxz = x.z;
    if (x.z < minz) minz = x.z;

    if (x.w > maxw) maxw = x.w;
    if (x.w < minw) minw = x.w;    
  }

for (int w=minw-1; w<=maxw+1; w++)
  for (int z=minz-1; z<=maxz+1; z++)
    for (int y=miny-1; y<=maxy+1; y++)
      for (int x=minx-1; x<=maxx+1; x++)
      {
        int neigh = countNeigh4(state, x, y, z, w);

        const pt4 pos(x, y, z, w);

        // If a cube is active and exactly 2 or 3 of its neighbors are also active, the cube remains active.
        // Otherwise, the cube becomes inactive.
        if (state.count(pos) == 1)
        {
          if (neigh == 2 || neigh == 3)
            ret.insert(pos);
        }
        // If a cube is inactive but exactly 3 of its neighbors are active, the cube becomes active.
        // Otherwise, the cube remains inactive.          
        else
          if (neigh == 3)
            ret.insert(pos);
      }

  return ret;
}

auto es2()
{
  auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::set<pt4> state;

  for (uint y=0; y<lines.size(); y++)
    for (uint x=0, xmax=lines[y].size(); x<xmax; x++)
      if (lines[y][x] == '#')
        state.insert(pt4(x, y, 0, 0));

  for (int i=0; i<6; i++)
    state = evolve4(state);

  return state.size();
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
