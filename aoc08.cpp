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

struct Instr
{
  std::string op;
  int value;

  friend std::istream &operator>>(std::istream &in, Instr &entry)
  {
    return in >> entry.op >> entry.value;
  }  
};

auto es1()
{
  const auto lines = readFile<Instr>(PUZZLE_INPUT_FILENAME);

  std::vector<int> history(lines.size());
  std::fill(history.begin(), history.end(), 99999);

  int ip = 0;
  int acc = 0;

  while (1)
  {
    if (history[ip] != 99999)
      return acc;
    else
      history[ip] = acc;

    if (lines[ip].op == "acc")
      acc += lines[ip++].value;
    else if (lines[ip].op == "nop")
      ip++;
    else if (lines[ip].op == "jmp")
      ip += lines[ip].value;
  }

  return 0;
}

int doesItLoop(std::vector<Instr> lines)
{
  std::vector<int> history(lines.size());
  std::fill(history.begin(), history.end(), 99999);

  uint ip = 0;
  int acc = 0;

  while (ip < lines.size())
  {
    if (history[ip] != 99999)
      return 99999;
    else
      history[ip] = acc;

    if (lines[ip].op == "acc")
      acc += lines[ip++].value;
    else if (lines[ip].op == "nop")
      ip++;
    else if (lines[ip].op == "jmp")
      ip += lines[ip].value;
  }

  return acc;
}

auto es2()
{
  auto lines = readFile<Instr>(PUZZLE_INPUT_FILENAME);

  for (uint i=0; i<lines.size(); i++)
    {
      if (lines[i].op == "acc")
        continue;

      lines[i].op = (lines[i].op == "jmp" ? "nop" : "jmp");

      auto res = doesItLoop(lines);
      if (res != 99999)
        return res;
      
      lines[i].op = (lines[i].op == "jmp" ? "nop" : "jmp");
    }

  return 0;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
