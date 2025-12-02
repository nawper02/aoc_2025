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

  // find how many times the dial hits 0
  std::string line;
  int dialPos = 50;
  int password = 0;
  while (std::getline(inputBuffer, line))
  {
    int direction = line[0] == 'L' ? -1 : 1;            // -1 for L, 1 for R
    line.erase(0, 1);                                   // erase first char from string (L or R)
    int increment = std::stoi(line);                    // number part of line

    dialPos = (increment * direction + dialPos) % 100;  // make dialPos wrap at 99 so we can see if it hits 0
    if (dialPos == 0) password++;
  }

  std::cout << "Password: " << password << std::endl;

  // cleanup and exit
  inputFile.close();
  return 0;
}