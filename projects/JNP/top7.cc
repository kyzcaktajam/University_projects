// Authors: Anna Fąferek, Maja Tkaczyk

#include <iostream>
#include <iterator>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static constexpr size_t c_rankSize{7};

enum class action 
{
  MAX, 
  TOP, 
  VOTING 
};

struct rankComparator
{
  bool operator()(const std::pair<int, int32_t> &p1,
                  const std::pair<int, int32_t> &p2) const
  {
    return (p1.first > p2.first) ||
           ((p1.first == p2.first) && (p1.second < p2.second));
  }
};

// Function that determines the type of command based on the first word of
// the line.
action pickAction(const std::string &word)
{
  if (word == "NEW")
  {
    return action::MAX;
  }
  else if (word == "TOP")
  {
    return action::TOP;
  }
  else
  {
    return action::VOTING;
  }
}

// Function that processes and validates the NEW command.
int readMaxCommand(const std::vector<std::string> &commandFromUser,
                   const int32_t currMax)
  {
  if (commandFromUser.size() == 2 && commandFromUser[0] == "NEW" &&
      std::regex_match(commandFromUser[1], std::regex("[0]*[1-9]\\d{0,7}")))
  {
    int32_t num = std::stoi(commandFromUser[1],
                        nullptr); // Will not throw out_of_range exception
    // after passing regex_match condition.

    if (num < currMax) // Num is in ranges after passing regex_match condition.
    {
      throw std::invalid_argument{""};
    }

    return num;
  }
  else
  {
    throw std::invalid_argument{""};
  }
}

// Function that updates the ranking after changing the number of points / votes
// of the piece with the number pieceNumber.
void updateRank(std::set<std::pair<int, int32_t>, rankComparator> &rank,
                const int32_t pieceNumber, const int newScore,
                const int addedPoints)
  {
  auto it = rank.find({newScore - addedPoints, pieceNumber});

  if (it == rank.end())
  {
    rank.insert({newScore, pieceNumber});
  }
  else
  {
    rank.erase(it);
    rank.insert({newScore, pieceNumber});
  }

  if (rank.size() > c_rankSize)
  {
    auto it = rank.begin();

    for (int i = 0; i < 7; ++i)
      ++it;

    rank.erase(it);
  }
}

// Function that processes assigning points to pieces when closing the current
// voting.
void addPointsForVoting(
    const std::vector<int32_t> &mostVotedPieces,
    std::unordered_map<int, int32_t> &collectedPointsOverall,
    std::set<std::pair<int, int32_t>, rankComparator> &topPieces)
{
  int reward = c_rankSize;

  for (int pieceOfMusic : mostVotedPieces)
  {
    collectedPointsOverall[pieceOfMusic] += reward;
    updateRank(topPieces, pieceOfMusic, collectedPointsOverall[pieceOfMusic],
               reward);
    reward--;
  }
}

// Function that prints the current pieces ranking.
void printListing(const std::vector<int32_t> &prevMostVotedPieces,
                  const std::vector<int32_t> &currMostVotedPieces)
  {
  int currPos{1};

  for (int i : currMostVotedPieces)
  {
    auto it =
        std::find(prevMostVotedPieces.begin(), prevMostVotedPieces.end(), i);

    if (it == prevMostVotedPieces.end())
    {
      std::cout << i << " -\n";
    }
    else
    {
      int posPre{
          static_cast<int>(std::distance(prevMostVotedPieces.begin(), it) + 1)};
      int changeInPosition{posPre - currPos};
      std::cout << i << ' ' << changeInPosition << '\n';
    }

    currPos++;
  }
}

// Function that saves ranking of currently closed voting.
void updateListing(std::vector<int32_t> &prevMostVotedPieces,
                   const std::vector<int32_t> &currMostVotedPieces)
{
  prevMostVotedPieces = currMostVotedPieces;
}

// Function that notes pieces thrown out of voting.
void removePiecesFromVoting(std::unordered_set<int32_t> &piecesOutOfVotingList,
                            const std::vector<int32_t> &prevMostVotedPieces,
                            const std::vector<int32_t> &currMostVotedPieces)
{
  for (int i : prevMostVotedPieces)
  {
    auto it =
        std::find(currMostVotedPieces.begin(), currMostVotedPieces.end(), i);

    if (it == currMostVotedPieces.end())
    {
      piecesOutOfVotingList.insert(i);
    }
  }
}

// Function that processes the list of votes.
void processVotes(
    const std::vector<std::string> &commandFromUser,
    const std::unordered_set<int32_t> &piecesOutOfVotingList,
    std::unordered_map<int, int32_t> &countedVotes,
    std::set<std::pair<int, int32_t>, rankComparator> &currMostVotedPieces,
    const int32_t currMax)
{
  std::unordered_set<int32_t> voted;

  for (const auto& piece : commandFromUser)
  {
    if (std::regex_match(piece, std::regex("[0]*[1-9]\\d{0,7}")))
    {
      int32_t num = std::stoi(piece, nullptr);

      if (num > currMax || num < 1 || voted.find(num) != voted.end() ||
          piecesOutOfVotingList.find(num) != piecesOutOfVotingList.end())
      {
        throw std::invalid_argument{""};
      }
      else
      {
        voted.insert(num);
      }
    }
    else
    {
      throw std::invalid_argument{""};
    }
  }

  for (int num : voted)
  {
    countedVotes[num]++;
    updateRank(currMostVotedPieces, num, countedVotes[num], 1);
  }
}

std::vector<int32_t> formatRanking(
    const std::set<std::pair<int, int32_t>, 
                  rankComparator> &currMostVotedPieces)
{
  std::vector<int32_t> currMostVotedPiecesFormated;
  currMostVotedPiecesFormated.reserve(currMostVotedPieces.size());

  for (const auto& piece : currMostVotedPieces)
  {
    currMostVotedPiecesFormated.push_back(piece.second);
  }

  return currMostVotedPiecesFormated;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  
  int lineCounter{0};
  int numberOfPieces{0};
  std::unordered_set<int32_t> piecesOutOfVotingList;
  std::vector<int32_t> prevMostVotedPieces;
  std::vector<int32_t> prevTopPieces;
  std::unordered_map<int, int32_t> countedVotes;
  std::unordered_map<int, int32_t> collectedPointsOverall;
  std::set<std::pair<int, int32_t>, rankComparator> currMostVotedPieces;
  std::set<std::pair<int, int32_t>, rankComparator> currTopPieces;

  std::string actionToTake;
  while (std::getline(std::cin, actionToTake))
  {
    ++lineCounter;
    std::istringstream ss(actionToTake);
    std::vector<std::string> commandFromUser{
        std::istream_iterator<std::string>{ss},
        std::istream_iterator<std::string>{}};

    try
    {
      if (commandFromUser.size() == 0)
        continue; // emptyLine
      
      switch (pickAction(commandFromUser[0]))
      {
        case action::MAX:
        {
            numberOfPieces = readMaxCommand(commandFromUser, numberOfPieces);
            std::vector<int32_t> currMostVotedPiecesFormated =
                formatRanking(currMostVotedPieces);
            addPointsForVoting(currMostVotedPiecesFormated, collectedPointsOverall,
                            currTopPieces);
            removePiecesFromVoting(piecesOutOfVotingList, prevMostVotedPieces,
                                currMostVotedPiecesFormated);
            printListing(prevMostVotedPieces, currMostVotedPiecesFormated);
            updateListing(prevMostVotedPieces, currMostVotedPiecesFormated);
            currMostVotedPieces.clear();
            countedVotes.clear();
            break;
        }
        case action::TOP:
        {
            if (commandFromUser.size() != 1)
            throw std::invalid_argument{""};
            
            std::vector<int32_t> currTopPiecesFormated = formatRanking(currTopPieces);
            printListing(prevTopPieces, currTopPiecesFormated);
            updateListing(prevTopPieces, currTopPiecesFormated);
            break;
        }
        case action::VOTING:
        {
            processVotes(commandFromUser, piecesOutOfVotingList, countedVotes,
                        currMostVotedPieces, numberOfPieces);
            break;
        }
      }
    }
    catch (const std::invalid_argument &)
    {
      std::cerr << "Error in line " << lineCounter << ": " << actionToTake
                << '\n';
    }
  }
}
