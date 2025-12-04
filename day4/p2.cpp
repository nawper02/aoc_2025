#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <utility>

constexpr std::array<std::pair<int, int>, 8> directions = {{
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
}};

bool lessThanFourNeighbors(int r, int c, const std::vector<std::string>& grid) {
    int numNeighbors = 0;
    for (auto [dr, dc] : directions) {
        int nR = r + dr;
        int nC = c + dc;
        if (nR >= 0 && nR < grid.size() && nC >= 0 && nC < grid[0].length()) {
            if (grid[nR][nC] == '@') {
              numNeighbors++;
            }
        }
    }
    return numNeighbors < 4;
}

int main()
{
  std::ifstream inputFile("../input.txt");
  std::stringstream inputBuffer;
  inputBuffer << inputFile.rdbuf();

  std::string line;
  std::vector<std::string> grid;
  while (std::getline(inputBuffer, line))
  {
    grid.push_back(line);
  }

  std::vector<std::string> gridCpy = grid;
  int numRollsAccessible = 0;
  int delta = 1; // to enter loop

  while (delta != 0) {
    delta=0;
    for (int r = 0; r < grid.size(); r++)
    {
      for (int c = 0; c < grid[r].length(); c++)
      {
        if (grid[r][c] != '@') continue;
        if (lessThanFourNeighbors(r,c,grid)) {
          gridCpy[r][c] = '.';
          delta++;
        }
      }
    }
    numRollsAccessible += delta;
    grid = gridCpy;
  }

  std::cout << "Num rolls accessible: " << numRollsAccessible << std::endl;

  // cleanup and exit
  inputFile.close();
  return 0;
}