#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
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

std::vector<std::string> split_string(std::string x)
{
  std::istringstream iss(x);
  std::vector<std::string> res;

  std::string k;

  while (iss >> k)
    res.push_back(k);

  return res;
}

struct food
{
  std::vector<std::string> ingredients;
  std::vector<std::string> allergenes;

  food() : ingredients(), allergenes() {}

  friend std::istream &operator>>(std::istream &in, food &entry)
  {
    std::string word;
    bool allergenes = false;
    bool done = false;

    entry.ingredients.clear();
    entry.allergenes.clear();

    do
    {
      if (!(in >> word))
        return in;      

      if (word[0] == '(') // (contains
        allergenes = true;
      else if (allergenes)
      {
        if (word[word.length()-1] == ')')
        {
          done = true;
          word[word.length()-1] = '\0';
        }

        entry.allergenes.push_back(word);
      }
      else
        entry.ingredients.push_back(word);

    } while (!done);

    return in;
  }  
};


auto es1()
{
  const auto foods = readFile<food>(PUZZLE_INPUT_FILENAME);

  // for (auto f : foods)
  // {
  //   for (auto t : f.ingredients)
  //     std::cout << t << " ";
  //   std::cout << std::endl;
  //   // std::cout << f.ingredients.size() << " " << f.allergenes.size() << std::endl;
  // }

  return 0;
}

auto es2()
{
  const auto foods = readFile<food>(PUZZLE_INPUT_FILENAME);

  return 0;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
