#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
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

struct Range
{
  std::string name;
  int r0[2];
  int r1[2];

  static Range fromString(std::string x)
  {
    Range r;

    auto pos = x.find(':');

    r.name = x.substr(0, pos);

    r.r0[0] = std::stoi(x.substr(pos+2));
    pos = x.find('-', pos);
    r.r0[1] = std::stoi(x.substr(pos+1));

    pos = x.find('r', pos);
    r.r1[0] = std::stoi(x.substr(pos+2));
    pos = x.find('-', pos);
    r.r1[1] = std::stoi(x.substr(pos+1));

    return r;
  }

  bool inside(int x) const { return (r0[0] <= x && x <= r0[1]) || (r1[0] <= x && x <= r1[1]); }
};

std::vector<int> parseTicket(std::string x)
{
  std::vector<int> ret;

  size_t pos = 0;

  while ((pos = x.find(",")) != std::string::npos)
  {
    std::string token = x.substr(0, pos);
    ret.push_back(std::stoi(token));
    x.erase(0, pos + 1);
  }
  ret.push_back(std::stoi(x));

  return ret;
}

auto es1()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::vector<Range> ranges;

  uint i=0;

  while (!lines[i].empty())
    ranges.push_back(Range::fromString(lines[i++]));

  i+=2;
  auto myticket = parseTicket(lines[i]);

  i+=3;
  std::vector<std::vector<int>> tickets;
  while (i < lines.size() && !lines[i].empty())
    tickets.push_back(parseTicket(lines[i++]));


  uint count = 0;

  for (auto t : tickets)
    for (auto v : t)
      if (!std::any_of(ranges.begin(), ranges.end(), [v](auto r){ return r.inside(v); }))
      {
        count += v;
        break;
      }

  return count;
}

auto getPerm(std::vector<std::vector<int>> chances)
{
  const uint len = chances.size();

  std::vector<int> ret(len);

  // until someone is still to be determined
  while (std::any_of(chances.begin(), chances.end(), [](auto x) { return x.size() > 0; }))
    for (uint i=0; i<len; i++)
      if (chances[i].size() == 1)
      {
        ret[i] = chances[i][0];

        for (auto& c : chances)
          c.erase(std::remove(c.begin(), c.end(), ret[i]), c.end());

        break;
      }

  return ret;
}

auto es2()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::vector<Range> ranges;

  uint i=0;

  while (!lines[i].empty())
    ranges.push_back(Range::fromString(lines[i++]));

  i+=2;
  auto myticket = parseTicket(lines[i]);

  i+=3;
  std::vector<std::vector<int>> tickets;
  while (i < lines.size() && !lines[i].empty())
    tickets.push_back(parseTicket(lines[i++]));

  // keep only valid tickets
  std::vector<std::vector<int>> valid_tickets;

  for (auto t : tickets)
    if (!std::any_of(t.begin(), t.end(), [ranges](auto x) { return !std::any_of(ranges.begin(), ranges.end(), [x](auto r){ return r.inside(x); }); }))
      valid_tickets.push_back(t);

  // find for each range -> all possible valid fields
  std::vector<std::vector<int>> chances(ranges.size());

  for (int k=0, kmax=ranges.size(); k<kmax; k++)
  {
    const Range r = ranges[k];

    for (uint i = 0; i<valid_tickets[0].size(); i++)
      if (!std::any_of(valid_tickets.begin(), valid_tickets.end(), [r, i](auto x) { return !r.inside(x[i]); }))
        chances[k].push_back(i);
  }

  // find which field is which index
  auto perm = getPerm(chances);

  // multiply the requested fields
  uint64_t ret = 1;

  for (int r=0,rmax=ranges.size(); r<rmax; r++)
    if (ranges[r].name.find("departure") != std::string::npos)
      ret *= myticket[perm[r]];

  return ret;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
