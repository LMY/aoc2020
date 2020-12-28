#include <algorithm>
#include <fstream>
#include <iostream>
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

struct point
{ 
  int x;
  int y;

  point(int _x, int _y) : x(_x), y(_y) {}

  bool operator< (const point& p2) const
  {
    if (x < p2.x)
      return true;
    else if (x > p2.x)
      return false;

    if (y < p2.y)
      return true;
    else //if (y > p2.y)
      return false;
  }  
};

auto parse(std::string x)
{
  point ret = { 0, 0 };

  for (int idx=0; x[idx] != '\0'; )
    if (x[idx] == 'e')
    {
      ret.x += 2;
      idx++;
    }
    else if (x[idx] == 'w')
    {
      ret.x -= 2;
      idx++;
    }
    else if (x[idx] == 'n' && x[idx+1] == 'e')
    {
      ret.x++;
      ret.y--;
      idx += 2;
    }    
    else if (x[idx] == 'n' && x[idx+1] == 'w')
    {
      ret.x--;
      ret.y--;
      idx += 2;
    }
    else if (x[idx] == 's' && x[idx+1] == 'e')
    {
      ret.x++;
      ret.y++;
      idx += 2;
    }    
    else if (x[idx] == 's' && x[idx+1] == 'w')
    {
      ret.x--;
      ret.y++;
      idx += 2;
    }        

  return ret;
}

auto es1()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::set<point> pts;

  for (auto l : lines)
  {
    point pt = parse(l);

    // std::cout << l << "    " << pt.x << " " << pt.y << std::endl;

    if (pts.count(pt))
      pts.erase(pt);
    else
      pts.insert(pt);
  }

  return pts.size();
}

int neigh(std::set<point> pts, int x, int y)
{
  int count = 0;

  if (pts.count(point(x+2, y)))
    count++;
  if (pts.count(point(x-2, y)))
    count++;
  if (pts.count(point(x+1, y+1)))
    count++;
  if (pts.count(point(x+1, y-1)))
    count++;
  if (pts.count(point(x-1, y+1)))
    count++;
  if (pts.count(point(x-1, y-1)))
    count++;

  return count;
}

auto evolve(std::set<point> pts)
{
  std::set<point> ret;

  int minx= 999;
  int maxx= -999;
  int miny= 999;
  int maxy= -999;

  for (auto x : pts)
  {
    if (x.x > maxx)
      maxx = x.x;
    if (x.x < minx)
      minx = x.x;

    if (x.y > maxy)
      maxy = x.y;
    if (x.y < miny)
      miny = x.y;
  }

  for (int y=miny-2;y<=maxy+2; y++)
    for (int x=minx-1; x<=maxx+1; x++)
    {
      auto pos = point(x, y);
      int cnei = neigh(pts, x, y);
      bool isblack = (pts.count(pos)==1);

      if ((!isblack && cnei == 2) || (isblack && (cnei == 1 || cnei == 2)))
        ret.insert(pos);
    }

  return ret;
}

auto es2()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::set<point> pts;

  for (auto l : lines)
  {
    point pt = parse(l);

    if (pts.count(pt))
      pts.erase(pt);
    else
      pts.insert(pt);
  }

  for (int i=0; i<100; i++)
  {
    // std::cout << i << "   " << pts.size() << std::endl;
    pts = evolve(pts);
  }

  return pts.size();
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;

  return 0;
}
