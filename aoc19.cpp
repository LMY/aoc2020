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

std::vector<std::string> split_string(std::string x)
{
  std::istringstream iss(x);
  std::vector<std::string> res;

  std::string k;

  while (iss >> k)
    res.push_back(k);

  return res;
}

static bool matches(std::map<std::string, std::string> rules, std::string rulename, std::string text, uint& cursor)
{
  // std::cout << "matching {" << rulename << "} text=[" << text << "]" << std::endl;
  
  if (cursor >= text.size())
    return false;

  if (rulename[0] == '\"') {
    if (text[cursor] == rulename[1]) {
      cursor++;
      return true;
    } else
      return false;
  }

  const auto pos_pipe = rulename.find('|');

  if (pos_pipe != std::string::npos) {
    const std::string rule1 = rulename.substr(0, pos_pipe - 1);
    const std::string rule2 = rulename.substr(pos_pipe + 2);

    uint newcursor;
    newcursor = cursor;

    if (matches(rules, rule1, text, newcursor)) {
      cursor = newcursor;
      return true;
    }
    newcursor = cursor;
    if (matches(rules, rule2, text, newcursor)) {
      cursor = newcursor;
      return true;
    } else
      return false;
  } else {
    std::vector<std::string> names = split_string(rulename);

    uint newcursor = cursor;

    for (uint i=0; i<names.size(); i++)
      if (!matches(rules, rules[names[i]], text, newcursor))
        return false;

    cursor = newcursor;
    return true;
  }
}

auto es1()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::map<std::string, std::string> rules;
  uint i;
  for (i = 0; i < lines.size() && !lines[i].empty(); i++)
  {
    const auto pos_dpt = lines[i].find(':');
    std::string name = lines[i].substr(0, pos_dpt);
    std::string text = lines[i].substr(pos_dpt + 2);

    rules[name] = text;
  }

  i++;
  // read strings
  int count = 0;
  
  for (;i < lines.size() && !lines[i].empty(); i++)
  {
    uint cursor = 0;
    if (matches(rules, "0", lines[i], cursor))
      if (cursor == lines[i].size())
      count++;
  }

  return count;
}

static std::vector<uint> matches2(std::map<std::string, std::string> rules, std::string rulename, std::string text, uint cursor)
{
  // std::cout << "matching {" << rulename << "} text=[" << text << "]" << std::endl;
  std::vector<uint> ret;

  if (cursor >= text.size())
    return ret;

  if (rulename[0] == '\"')
  {
    if (text[cursor] == rulename[1])
      ret.push_back(cursor+1);
  }
  else
  {
    const auto pos_pipe = rulename.find('|');

    if (pos_pipe != std::string::npos)
    {
      const std::string rule1 = rulename.substr(0, pos_pipe - 1);
      const std::string rule2 = rulename.substr(pos_pipe + 2);

      auto m1 = matches2(rules, rule1, text, cursor);
      auto m2 = matches2(rules, rule2, text, cursor);

      std::copy(m1.begin(), m1.end(), std::back_inserter(ret));
      std::copy(m2.begin(), m2.end(), std::back_inserter(ret));
    }
    else
    {
      std::vector<std::string> names = split_string(rulename);

      // at each step, try from these indexes
      std::vector<uint> nextstep = { cursor };
      
      // ..applying these rules in order...
      for (auto name : names)
      {
        // to these
        std::vector<uint> nextnextstep;

        for (auto x : nextstep)
        {
          // inserting all possible results
          auto pos_nextnextstep = matches2(rules, rules[name], text, x);

          std::copy(pos_nextnextstep.begin(), pos_nextnextstep.end(), std::back_inserter(nextnextstep));
        }

        nextstep = nextnextstep;
      }

      ret = nextstep;
    }
  }

  return ret;
}

auto es2()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::map<std::string, std::string> rules;
  uint i;
  for (i = 0; i < lines.size() && !lines[i].empty(); i++)
  {
    const auto pos_dpt = lines[i].find(':');
    std::string name = lines[i].substr(0, pos_dpt);
    std::string text = lines[i].substr(pos_dpt + 2);

    rules[name] = text;
  }

  rules["8"] = "42 | 42 8";
  rules["11"] = "42 31 | 42 11 31";

  i++;
  // read strings
  int count = 0;
  
  for (;i < lines.size() && !lines[i].empty(); i++)
  {
    const auto results = matches2(rules, "0", lines[i], 0);

    // if matching we reached the end of the string, cursor = lines[i].size() -> found
    if (std::find(results.begin(), results.end(), lines[i].size()) != results.end())
      count++;
  }

  return count;
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
