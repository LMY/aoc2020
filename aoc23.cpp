#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
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
  {
    state = move(state);
    // std::cout << state << std::endl;
  }

  // std::cout << "[" << state << "]" << std::endl;
  state = arrange_to_1(state);
  // std::cout << "[" << state << "]" << std::endl;

  return state;
}

static inline void move2(std::list<uint> x)
{
  const auto len = x.size();

  auto destination = x.front();
  x.pop_front();

  auto pu1 = x.front();
  x.pop_front();

  auto pu2 = x.front();
  x.pop_front();

  auto pu3 = x.front();
  x.pop_front();

  auto lookfor = destination == 1 ? len : destination -1;

  while (lookfor == pu1 || lookfor == pu2 || lookfor == pu3)
    lookfor = (lookfor == 1 ? len : lookfor-1);

  // std::cout << destination << " -> look for " << lookfor << std::endl;

  auto where = std::find(x.begin(), x.end(), lookfor);

  where++;
  x.insert(where, pu1);
  x.insert(where, pu2);
  x.insert(where, pu3);
  x.push_back(destination);
}

int where_is1(std::vector<uint> state)
{
  for (uint i=0; i<state.size(); i++)
    if (state[i] == 1)
      return i;

  return -1;
}

void print_first_tot(std::list<uint> state)
{
  for (auto x : state)
    std::cout << x << " ";

  std::cout << std::endl;
}

auto es2()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::list<uint> state(1000000);

  for (uint i=0; i<lines[0].size(); i++)
    state.push_back(lines[0][i] - '0');

  for (uint i=10; i<=1000000; i++)
    state.emplace_back(i);

//  for (int i=0; i< 100; i++)
  for (int i=0; i< 10000000; i++)
  {
    // if (i % 1000 == 0)
      std::cout << i << std::endl;
    // std::cout << "one is at " << where_is1(state) << std::endl;
    // print_first_tot(state);
    move2(state);
  }

  // print_first_tot(state);*(++destination_pos)

  auto destination_pos = std::find(state.begin(), state.end(), 1); // - state.begin();
  auto p1 = *(++destination_pos);
  auto p2 = *(++destination_pos);
  std::cout << "P[pos+1] " << p1 << std::endl;
  std::cout << "P[pos+2] " << p2 << std::endl;
  // std::cout << "P[pos+2] " << state[(destination_pos+2) % state.size()] << std::endl;

  // return (uint64_t)state[(destination_pos+1) % state.size()] * state[(destination_pos+2) % state.size()];

  return p1*p2;

  // uint destination_pos = 0;
  
  // for (uint i=0; i<state.size(); i++)
  //   if (state[i] == 1)
  //     {
  //       destination_pos = i;
  //       break;
  //     }

  // //  = std::find(state.begin(), state.end(), 1); // - state.begin();

  // std::cout << "POS " << destination_pos << std::endl;
  // std::cout << "P[pos+1] " << state[(destination_pos+1) % state.size()] << std::endl;
  // std::cout << "P[pos+2] " << state[(destination_pos+2) % state.size()] << std::endl;

  // return (uint64_t)state[(destination_pos+1) % state.size()] * state[(destination_pos+2) % state.size()];
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;

  return 0;
}
