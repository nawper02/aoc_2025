#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <numeric>

int main()
{
  // load input into a stringstream
  std::ifstream inputFile("../input.txt");
  std::stringstream inputBuffer;
  inputBuffer << inputFile.rdbuf();

  // iterate over the input
  std::string line;
  int charAsInt;
  static const int N = 12;
  std::vector<long long> joltages;
  while (std::getline(inputBuffer, line))
  {
    std::array<int, N> joltage;
    joltage.fill(-1);
    int nextStartingIndex = 0;
    for (int i = N-1; i >= 0; i--)
    {
      for (int j = nextStartingIndex; j < line.length()-i; j++)
      {
        charAsInt = line[j] - '0';
        if ((charAsInt > joltage[N-i-1])) 
        {
          joltage[N-i-1] = charAsInt;
          nextStartingIndex = j+1;
          //std::cout << "Search index is " << j << ", new largest value for battery " << N-i << " found and is equal to " << charAsInt << ". Starting next loop at index " << nextStartingIndex << std::endl;
        }
      }
    }
    long long combinedJoltage = 0;
    for (int digit : joltage) {
      combinedJoltage = (combinedJoltage * 10) + digit;
    }
    joltages.push_back(combinedJoltage);
  }

  long long totalSum = std::accumulate(joltages.begin(), joltages.end(), 0LL);
  
  std::cout << "Sum: " << totalSum << std::endl;

  // cleanup and exit
  inputFile.close();
  return 0;
}