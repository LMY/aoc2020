#include <algorithm>
#include <fstream>
#include <iostream>
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

const uint64_t PAR_OPEN = std::numeric_limits<uint64_t>::max();
const uint64_t PAR_CLOSE = std::numeric_limits<uint64_t>::max()-1;
const uint64_t PLUS = std::numeric_limits<uint64_t>::max()-2;
const uint64_t TIMES = std::numeric_limits<uint64_t>::max()-3;

/**
 * @brief Calculate the value of an expression that does not contain parentheses
 * 
 * @param advanced if true, es2(), requires PLUS has precedence over TIMES
 */
auto doMath(std::vector<uint64_t> x, int from, int to, bool advanced)
{
  if (!advanced)
  {
do_normal_math:    
    uint64_t val;

    for (val = x[from++]; from < to; from += 2)
      if (x[from] == PLUS)
        val += x[from+1];
      else if (x[from] == TIMES)
        val *= x[from+1];

    return val;
  }
  else
  {
    // find first PLUS sign
    auto first_sum = std::find(x.begin()+from, x.begin()+to, PLUS);

    // if there is no PLUS sign, just fallback to non-advanced mode
    if (first_sum == x.begin()+to)
      goto do_normal_math;

    // otherwise, we create a vector with everything
    std::vector<uint64_t> newone;

    // that comes before
    std::copy(x.begin()+from, first_sum-1, std::back_inserter(newone));

    // add the two values
    newone.push_back(*(first_sum-1) + *(first_sum+1));

    // and add everything else
    std::copy(first_sum+2, x.begin() + to + 1, std::back_inserter(newone));

    return doMath(newone, 0, newone.size()-1, advanced);    
  }
}

auto evaluate(std::vector<uint64_t> x, bool advanced)
{
  int last_close = -1;
  int last_open = -1;

  // look for last open and close parentheses
  for (uint i=0; i<x.size(); i++)
    if (x[i] == PAR_CLOSE)
      {
        last_close = i;
        break;
      }
    else if (x[i] == PAR_OPEN)
      last_open = i;

  std::vector<uint64_t> newone;

  if (last_close < 0)
    // no parentheses, just do the math
    newone.push_back(doMath(x, 0, x.size() - 1, advanced));
  else
  {
    // insert everything before (
    std::copy(x.begin(), x.begin() + last_open, std::back_inserter(newone));

    // calculate value of the (...)
    newone.push_back(doMath(x, last_open + 1, last_close - 1, advanced));

    // insert everything after )
    std::copy(x.begin()+last_close+1, x.end(), std::back_inserter(newone));
  }
 
  return newone;
}

/**
 * @brief Converts a string into a std::vector<uint64_t> with numbers and PLUS, TIMES, PAR_OPEN, PAR_CLOSE
 */
auto toExpression(std::string x)
{
  std::vector<uint64_t> ret;

  uint64_t num = 0;

  for (uint i=0; i<x.size(); i++)
    if (x[i] == '(')
      ret.push_back(PAR_OPEN);
    else if (x[i] == ')')
    {
      ret.push_back(num);
      num = 0;
      ret.push_back(PAR_CLOSE);
    }
    else if (x[i] == '+')
      ret.push_back(PLUS);
    else if (x[i] == '*')
      ret.push_back(TIMES);
    else if (isdigit(x[i]))
    {
      num = num*10 + x[i]-'0';
    }
    else if (x[i] == ' ')
    {
      if (num > 0)
        ret.push_back(num);
      num = 0;
    }

  if (num > 0)
    ret.push_back(num);

  return ret;
}

auto evaluate(std::string x, bool advanced)
{
  auto expr = toExpression(x);

  while (expr.size() > 1)
    expr = evaluate(expr, advanced);

  return expr[0];
}

auto es1()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  return std::accumulate(lines.begin(), lines.end(), 0lu, [](uint64_t x, const std::string line){ return x + evaluate(line, false); });
}

auto es2()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  return std::accumulate(lines.begin(), lines.end(), 0lu, [](uint64_t x, const std::string line){ return x + evaluate(line, true); });
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
