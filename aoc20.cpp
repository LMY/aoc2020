#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
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

struct tile
{
  uint name;
  std::vector<std::string> content;

  std::vector<std::string> border;

  tile(uint _name) : name(_name), content(), border() {}

  void calculateBorder()
  {
    std::string col0 = "";
    std::string coln = "";

    for (uint i=0, imax=content.size(); i<imax; i++)
    {
      col0 += content[i][0];
      coln += content[i][content.size()-1];
    }

    insertBorder(content[0]);
    insertBorder(content[content.size()-1]);
    insertBorder(col0);
    insertBorder(coln);
  }

  int matches(std::string x) const
  {
    for (uint i=0; i<border.size(); i++)
      if (x == border[i])
        return i/2;

    return -1;
  }

  void rotate()
  {
    std::vector<std::string> newcontent(content.size());

    for (auto& x : newcontent)
      x.resize(content[0].size());

    for (int y=0, ymax=content.size(); y<ymax; y++)
      for (int x=0, xmax=content[y].size(); x<xmax; x++)
        newcontent[x][ymax-1-y] = content[y][x];

    content = newcontent;
  }

  std::string getBorderLeft() const
  {
    std::string col0 = "";

    for (uint i=0, imax=content.size(); i<imax; i++)
      col0 += content[i][0];

    return col0;
  }

  std::string getBorderRight() const
  {
    std::string coln = "";

    for (uint i=0, imax=content.size(); i<imax; i++)
      coln += content[i][content.size()-1];

    return coln;
  }

  std::string getBorderUp() const
  {
    return content[0];
  }

  std::string getBorderDown() const
  {
    return content[content.size()-1];
  }

  void flip()
  {
    for (auto& x : content)
      reverse(x.begin(), x.end()); 
  }

private:
  void insertBorder(std::string x)
  {
    border.push_back(x);
    reverse(x.begin(), x.end()); 
    border.push_back(x);    
  }
};


auto es1()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::vector<tile> tiles;

  for (uint i=0; i<lines.size(); i++)
    if (lines[i].empty())
      tiles[tiles.size()-1].calculateBorder();
    else if (lines[i].substr(0, 4) == "Tile")
      tiles.push_back(tile(std::stoi(lines[i].substr(5, lines[i].size()-6))));
    else
      tiles[tiles.size()-1].content.push_back(lines[i]);

  std::vector<std::vector<bool>> visits(tiles.size());
  for (auto& v : visits)
    v.resize(4);

  // for each tile
  for (uint i=0, imax=tiles.size(); i<imax; i++)
    // for each of the 4 borders
    for (uint k=0; k<4; k++)
      if (!visits[i][k])
        // for each other tile
        for (uint j=i+1; j<imax; j++)
        {
          auto mat = tiles[j].matches(tiles[i].border[k*2]);

          // if matches, flag visited
          if (mat >= 0)
          {
            visits[i][k] = visits[j][mat] = true;
            break;
          }
        }

  uint64_t ret = 1;

  for (uint i=0, imax=tiles.size(); i<imax; i++)
  {
    uint notvisited = 0;

    for (uint k=0; k<4; k++)
      if (!visits[i][k])
        notvisited++;

    if (notvisited == 2)
      ret *= tiles[i].name;
  }

  return ret;
}

auto es2()
{
const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::vector<tile> tiles;

  for (uint i=0; i<lines.size(); i++)
    if (lines[i].empty())
      tiles[tiles.size()-1].calculateBorder();
    else if (lines[i].substr(0, 4) == "Tile")
      tiles.push_back(tile(std::stoi(lines[i].substr(5, lines[i].size()-6))));
    else
      tiles[tiles.size()-1].content.push_back(lines[i]);

  std::vector<std::vector<bool>> visits(tiles.size());
  for (auto& v : visits)
    v.resize(4);

  // for each tile
  for (uint i=0, imax=tiles.size(); i<imax; i++)
    // for each of the 4 borders
    for (uint k=0; k<4; k++)
      if (!visits[i][k])
        // for each other tile
        for (uint j=i+1; j<imax; j++)
        {
          auto mat = tiles[j].matches(tiles[i].border[k*2]);

          // if matches, flag visited
          if (mat >= 0)
          {
            visits[i][k] = visits[j][mat] = true;
            break;
          }
        }

  std::vector<uint> corners;
  for (uint i=0, imax=tiles.size(); i<imax; i++)
  {
    uint notvisited = 0;

    for (uint k=0; k<4; k++)
      if (!visits[i][k])
        notvisited++;

    if (notvisited == 2)
      corners.push_back(tiles[i].name);
  }

  std::vector<bool> image(100*400);

  std::vector<tile> ordered_tiles;
  ordered_tiles.push_back(tiles[corners[0]]);

ordered_tiles[0].flip();
ordered_tiles[0].rotate();
ordered_tiles[0].rotate();
ordered_tiles[0].rotate();
ordered_tiles[0].rotate();

  std::string right = ordered_tiles[ordered_tiles.size()-1].getBorderRight();

  std::cout << ordered_tiles[ordered_tiles.size()-1].name << std::endl;



  for (uint i=1; i<tiles.size(); i++)
  {
    bool found = false;


    for (int k=0; k<4; k++)
      if (tiles[i].getBorderLeft() == right)
      {
        found = true;
        break;
      }
      else
        tiles[i].rotate();


    if (!found)
    {
      tiles[i].flip();
      for (int k=0; k<4; k++)
        if (tiles[i].getBorderLeft() == right)
        {
          found = true;
          break;
        }
        else
          tiles[i].rotate();
    }
    
    if (found)
    {
      std::cout << "FOUND " << i << std::endl;
      right = tiles[i].getBorderRight();
      break;
    }
    else
    {
      std::cout << "NOT FOUND" << std::endl;
      break;
    }

  }




  return 0;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
