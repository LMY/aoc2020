#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
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

auto move(std::string x)
{
  std::string ret;
  ret.reserve(x.length());

  // std::cout << "Current " << x[0] << std::endl;
  // std::cout << "Selected " << x[1] << " " << x[2] << " " << x[3] << std::endl;

  char destination;

  for (int delta = 1; ; delta++)
  {
    destination = (char)(x[0]-delta);
    if (destination <= '0')
      destination += x.size();

    if (!(x[1] == destination || x[2] == destination || x[3] == destination))
      break;
  }

  uint destination_pos = std::find(x.begin(), x.end(), destination) - x.begin();
  // std::cout << "Destination " << destination << " @index=" << destination_pos << std::endl;

  for (int i=4, imax=(destination_pos<4?destination_pos+9:destination_pos); i<=imax; i++)
    ret += x[i % x.size()];

  ret += x[1];
  ret += x[2];
  ret += x[3];

  for (int i=destination_pos+1, imax=x.size(); i<=imax; i++)
    ret += x[i % x.size()];

  return ret;
}

auto arrange_to_1(std::string x)
{
  std::string ret;
  ret.reserve(x.length());

  uint pos = std::find(x.begin(), x.end(), '1') - x.begin();

  for (uint i=pos+1; i<x.size(); i++)
    ret += x[i];
  for (uint i=0; i<pos; i++)
    ret += x[i];

  return ret;
}

auto es1()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::string state = lines[0];

  for (int i=0; i<100; i++)
    state = move(state);

  state = arrange_to_1(state);

  return state;
}

static inline void move2(std::map<uint, uint>& next, uint& current)
{
  const auto len = next.size();

  auto pu1 = next[current];
  auto pu2 = next[pu1];
  auto pu3 = next[pu2];

  next[current] = next[pu3];

  auto lookfor = current == 1 ? len : current-1;

  while (lookfor == pu1 || lookfor == pu2 || lookfor == pu3)
    lookfor = (lookfor == 1 ? len : lookfor-1);

  auto savenext = next[lookfor];
  next[lookfor] = pu1;
  next[pu3] = savenext;

  current = next[current];
}

auto es2()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);
  std::string state = lines[0];

  std::map<uint, uint> next;

  for (uint i=0; i<state.size()-1; i++)
    next[state[i] - '0'] = state[i+1] - '0';

  next[state[state.size()-1] - '0'] = 10;

  for (uint i=10; i<=1000000; i++)
    next[i] = i+1;

  next[1000000] = state[0]-'0';

  uint current = lines[0][0] - '0';

  for (int i = 0; i < 10000000; i++)
    move2(next, current);

  auto ne1 = (uint64_t)next[1];
  auto nene1 = (uint64_t)next[ne1];

  return ne1*nene1;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;

  return 0;
}
