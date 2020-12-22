#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
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

void play_round(std::vector<uint>& player1, std::vector<uint>& player2)
{
  const auto len1 = player1.size();
  const auto len2 = player2.size();

  std::vector<uint> newplayer1;
  std::vector<uint> newplayer2;

  if (len1 > len2)
    std::copy(player1.begin()+len2, player1.end(), std::back_inserter(newplayer1));
  else if (len2 > len1)
    std::copy(player2.begin()+len1, player2.end(), std::back_inserter(newplayer2));

  for (uint i=0; i<std::min(len1, len2); i++)
    if (player1[i] > player2[i])
    {
      newplayer1.push_back(player1[i]);
      newplayer1.push_back(player2[i]);
    }
    else
    {
      newplayer2.push_back(player2[i]);
      newplayer2.push_back(player1[i]);
    }


  player1 = newplayer1;
  player2 = newplayer2;
}

auto calc_points(std::vector<uint> cards)
{
  uint64_t ret = 0;

  for (uint i=0, imax=cards.size(); i<imax; i++)
    ret += cards[i] * (imax-i);

  return ret;
}

auto es1()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::vector<uint> player1;
  std::vector<uint> player2;

  uint i;
  for (i=1; i<lines.size() && !lines[i].empty(); i++)
    player1.push_back(std::stoi(lines[i]));

  for (i+=2; i<lines.size(); i++)
      player2.push_back(std::stoi(lines[i]));

  while (player1.size() != 0 && player2.size() != 0)
    play_round(player1, player2);

  // std::for_each(player1.begin(), player1.end(), [](auto x) { std::cout << x << " "; });
  // std::cout << std::endl;
  // std::for_each(player2.begin(), player2.end(), [](auto x) { std::cout << x << " "; });
  // std::cout << std::endl;

  return calc_points(player1.size() == 0 ? player2 : player1);
}



struct Game
{
  Game(std::vector<uint> _player1, std::vector<uint> _player2) : player1(_player1), player2(_player2), configs(), outcome(0) { }

  std::vector<uint> player1;
  std::vector<uint> player2;

  std::set<std::vector<uint>> configs;

  
  int outcome;



  auto getState() const
  {
    std::vector<uint> state;

    std::copy(player1.begin(), player1.end(), std::back_inserter(state));
    state.push_back(9999);
    std::copy(player2.begin(), player2.end(), std::back_inserter(state));

    return state;
  }


  int play_round()
  {
    auto state = getState();

    // avoid infinite loop rule
    if (configs.count(state) != 0)
      return 1;
    else
      configs.insert(state);

    const auto len1 = player1.size();
    const auto len2 = player2.size();

    std::vector<uint> newplayer1;
    std::vector<uint> newplayer2;

    std::copy(player1.begin()+1, player1.end(), std::back_inserter(newplayer1));      
    std::copy(player2.begin()+1, player2.end(), std::back_inserter(newplayer2));  

    int result = 0;

    // std::cout << "Player1: ";
    // std::for_each(player1.begin(), player1.end(), [](auto x) { std::cout << x << " "; });
    // std::cout << std::endl;
    // std::cout << "Player2: ";
    // std::for_each(player2.begin(), player2.end(), [](auto x) { std::cout << x << " "; });
    // std::cout  << std::endl;

    // recursive game
    if (player1[0] <= (len1-1) && player2[0] <= (len2-1))
    {
      std::vector<uint> temp1;
      std::vector<uint> temp2;

      std::copy(player1.begin()+1, player1.begin()+1+player1[0], std::back_inserter(temp1));      
      std::copy(player2.begin()+1, player2.begin()+1+player2[0], std::back_inserter(temp2));  

      Game newgame(temp1, temp2);
      result = newgame.play();
    }
    else
      // standard round
      result = player1[0] > player2[0] ? 1 : player1[0] < player2[0] ? 2 : 0;
    
    // std::cout << result << std::endl<< std::endl;

    if (result == 1)
    {
      newplayer1.push_back(player1[0]);
      newplayer1.push_back(player2[0]);
    }
    else if (result == 2)
    {
      newplayer2.push_back(player2[0]);
      newplayer2.push_back(player1[0]);
    }

    player1 = newplayer1;
    player2 = newplayer2;

    return player1.empty() ? 2 : player2.empty() ? 1 : 0;
  }

  int play()
  {
    int winner;

    do { winner = play_round(); }
    while (winner == 0);

    return winner;
  }
};

auto es2()
{
  const auto lines = readFileLines(PUZZLE_INPUT_FILENAME);

  std::vector<uint> player1;
  std::vector<uint> player2;

  uint i;
  for (i=1; i<lines.size() && !lines[i].empty(); i++)
    player1.push_back(std::stoi(lines[i]));

  for (i+=2; i<lines.size(); i++)
      player2.push_back(std::stoi(lines[i]));

  Game game(player1, player2);
  int winner = game.play();

  // std::cout << "END Player1: ";
  // std::for_each(game.player1.begin(), game.player1.end(), [](auto x) { std::cout << x << " "; });
  // std::cout << std::endl;
  // std::cout << "END Player2: ";
  // std::for_each(game.player2.begin(), game.player2.end(), [](auto x) { std::cout << x << " "; });
  // std::cout << std::endl << std::endl;

  return calc_points(winner == 1 ? game.player1 : game.player2);
}

int main()
{
  std::cout << "1: " << es1() << std::endl;
  std::cout << "2: " << es2() << std::endl;

  return 0;
}
