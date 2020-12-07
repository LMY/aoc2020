#include <fstream>
#include <iostream>
#include <numeric>
#include <map>
#include <vector>
#include <set>
#include <stack>

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

struct Rule
{
  std::string name;
  std::vector<std::pair<std::string, int>> reqs;

  friend std::istream &operator>>(std::istream &in, Rule &entry)
  {
    // clear entry
    entry.name = "";
    entry.reqs.clear();

    // read name
    std::string x;

    if (!(in >> entry.name))
      return in;

    while (in >> x)
      if (x == "bag" || x == "bags")
        break;
      else
        entry.name += " " + x;

    in >> x;  // "contains"

    // read content
    std::pair<std::string, int> r;

    do {
      // content quantity
      in >> x;

      r.second = x == "no" ? 0 : std::stoi(x);

      // content name
      in >> r.first;
      
      while (1) {
        in >> x;
        if (x.rfind("bag", 0) == 0)
          break;

        r.first += " " + x;      
      }

      if (r.second > 0)
        entry.reqs.push_back(r);
      
    } while (x[x.size()-1] == ',');

    return in;
  }  
};

static auto whoContains(std::string name, std::vector<Rule> rules)
{
  std::set<std::string> contains;

  for (auto x : rules)
    for (auto r : x.reqs)
      if (r.first == name)
        {
          contains.insert(x.name);
          break;
        }

  return contains;
}

auto es1()
{
  const auto lines = readFile<Rule>(PUZZLE_INPUT_FILENAME);

  std::set<std::string> goodones;
  std::stack<std::string> newones;
  newones.push("shiny gold");

  while (!newones.empty())
  {
    auto thisone = newones.top();
    newones.pop();

    for (auto w : whoContains(thisone, lines))
      if (!goodones.count(w))
        {
          newones.push(w);
          goodones.insert(w);
        }
  }

  return goodones.size();
}

static auto whatContains(std::pair<std::string, int> thisone, std::vector<Rule> rules)
{
  std::map<std::string, int> contains;

  for (auto x : rules)
    if (x.name == thisone.first)
      for (auto r : x.reqs)
        contains.insert(std::pair<std::string, int>(r.first, r.second*thisone.second));

  return contains;
}

auto es2()
{
  const auto lines = readFile<Rule>(PUZZLE_INPUT_FILENAME);

  std::map<std::string, int> goodones;
  std::stack<std::pair<std::string, int>> newones;
  newones.push(std::pair<std::string, int>("shiny gold", 1));

  while (!newones.empty())
  {
    auto thisone = newones.top();
    newones.pop();

    for (auto w : whatContains(thisone, lines))
    {
      newones.push(w);
      goodones[w.first] += w.second;
    }
  }

  return std::accumulate(goodones.begin(), goodones.end(), 0, [](int value, auto p) { return value + p.second; });
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
