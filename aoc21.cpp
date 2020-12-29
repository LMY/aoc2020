#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <set>

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

struct food
{
  std::set<std::string> allergenes;
  std::set<std::string> ingredients;

  food() : allergenes(), ingredients() {}

  friend std::istream &operator>>(std::istream &in, food &entry)
  {
    std::string word;
    bool allergenes = false;

    entry.allergenes.clear();
    entry.ingredients.clear();

    while (in >> word)
      if (word[0] == '(') // "(contains", skip this word
        allergenes = true;
      else if (allergenes)
      {
        // remove last char: ',' or ')'
        entry.allergenes.insert(word.substr(0, word.size()-1));

        if (word[word.length()-1] == ')')
          return in;
      }
      else
        entry.ingredients.insert(word);

    return in;      
  }
};


void output(std::set<std::string> theset)
{
  for (auto x : theset)
    std::cout << x << " ";
  std::cout << std::endl;
}

auto getAllergenesMap(std::vector<food> foods)
{
  // map  allergene -> ingredient that has it
  std::map<std::string, std::string> allergenes;

  // all allergenes are unknown
  for (auto f : foods)
    for (auto a : f.allergenes)
      allergenes[a] = "";

  // till there is at least one associate
  while (std::any_of(allergenes.begin(), allergenes.end(), [](auto x) { return x.second.empty(); }))
    for (auto x : allergenes)
      if (x.second.empty())
      {
        bool starting = true;
        std::set<std::string> possibles;

        // for all foods that contain that allergene
        for (auto f : foods)
          if (f.allergenes.count(x.first))
          {
            if (starting)
            {
              // first one, assign all ingredients
              starting = false;
              possibles = f.ingredients;
            }
            else
            {
              // another one, intersect
              std::set<std::string> inter;
              std::set_intersection(possibles.begin(), possibles.end(), f.ingredients.begin(), f.ingredients.end(), std::inserter(inter, inter.end()));
              possibles = inter;
            }
          }

        // remove the ones we already know
        for (auto y : allergenes)
          possibles.erase(y.second);

        // if only one remains, we found a new association
        if (possibles.size() == 1)
          allergenes[x.first] = *possibles.begin();
      }

  return allergenes;
}

auto es1()
{
  const auto foods = readFile<food>(PUZZLE_INPUT_FILENAME);
  auto allergenes = getAllergenesMap(foods);

  int count = 0;

  for (auto f : foods)
    for (auto t : f.ingredients)
      if (!std::any_of(allergenes.begin(), allergenes.end(), [&t](auto x) { return t == x.second; }))
        count++;

  return count;
}

auto es2()
{
  const auto foods = readFile<food>(PUZZLE_INPUT_FILENAME);
  auto allergenes = getAllergenesMap(foods);

  // get a sorted vector of all allergenes
  std::vector<std::string> only_allerg;

  for (auto x : allergenes)
    only_allerg.push_back(x.first);

  std::sort(only_allerg.begin(), only_allerg.end());

  // get elements in order and concatenate them into result string
  std::string ret = allergenes[only_allerg[0]];
  for (uint i=1; i<only_allerg.size(); i++)
    ret += "," + allergenes[only_allerg[i]];

  return ret;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
