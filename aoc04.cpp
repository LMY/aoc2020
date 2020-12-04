#include <algorithm>
#include <fstream>
#include <iostream>
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

static bool is_integer_inside(std::string t, int min, int max)
{
  auto ivalue = std::stoi(t);
  return ivalue >= min && ivalue <= max;
}

static bool isValid(std::string x)
{
  auto field = x.substr(0, 3);
  auto value = x.substr(4);

  if (field == "byr")
    return is_integer_inside(value, 1920, 2002);
  else if (field == "iyr")
    return is_integer_inside(value, 2010, 2020);
  else if (field == "eyr")
    return is_integer_inside(value, 2020, 2030);
  else if (field == "hgt")
    {
      auto ivalue = std::stoi(value);
      auto mu = value.substr(std::to_string(ivalue).size());

      return mu == "cm" ? ivalue >= 150 && ivalue <= 193 :
             mu == "in" ? ivalue >= 59 && ivalue <= 76 :
             false;
    }    
  else if (field == "hcl")
    return value[0] == '#' && std::all_of(value.begin()+1, value.end(), [](auto c) {return std::isdigit(c) || (c>='a' && c<='f'); });
  else if (field == "ecl")
    {
      const auto VALID_ECL = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
      return std::find(VALID_ECL.begin(), VALID_ECL.end(), value) != VALID_ECL.end();
    } 
  else if (field == "pid")
    return value.size() == 9 && std::all_of(value.begin(), value.end(), [](auto c) {return std::isdigit(c); });

  return false;
}

static void updateFields(std::vector<bool>& fields, const std::vector<std::string> reqs, std::string x, bool perform_validity_test)
{
  auto pos = std::find(reqs.begin(), reqs.end(), x.substr(0, 3));

  if (pos != reqs.end())
    fields[pos - reqs.begin()] = perform_validity_test ? isValid(x) : true;
}

auto count_valid_passports(bool perform_validity_test)
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);
  int count = 0;

  const std::vector<std::string> REQS = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
  std::vector<bool> fields(REQS.size());

  for (auto x : lines)
    if (x.empty())
      {
        count += std::all_of(fields.begin(), fields.end(), [](bool i) { return i; });
        std::fill(fields.begin(), fields.end(), false);
      }
    else
      {
        size_t start = 0;
        size_t end;

        do {
          end = x.find(" ", start);
          updateFields(fields, REQS, x.substr(start, end - start), perform_validity_test);
          start = end + 1;
        }
        while (end != std::string::npos);
      }

  count += std::all_of(fields.begin(), fields.end(), [](bool i) { return i; });

  return count;
}

auto es1()
{
  return count_valid_passports(false);
}

auto es2()
{
  return count_valid_passports(true);
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;
  return 0;
}
