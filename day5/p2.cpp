#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main()
{
  // load input into a stringstream
  std::ifstream inputFile("../input.txt");
  std::stringstream inputBuffer;
  inputBuffer << inputFile.rdbuf();

  // iterate over the input
  std::string line;
  while (std::getline(inputBuffer, line))
  {
    std::cout << line << std::endl;
  }

  // cleanup and exit
  inputFile.close();
  return 0;
}