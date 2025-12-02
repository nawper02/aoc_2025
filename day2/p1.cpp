#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <cmath>

int main()
{
  // load input into a stringstream
  std::ifstream inputFile("../input.txt");
  std::stringstream inputBuffer;
  inputBuffer << inputFile.rdbuf();

  std::string inputStr(inputBuffer.str());
  std::string tmpStr("");
  std::vector<std::string> idRangesStrs;
  for (auto c : inputStr) 
  {
    if (c == ',') 
    {
      idRangesStrs.push_back(tmpStr);
      tmpStr = "";
    }
    else
    {
      tmpStr += c;
    }
  }

  std::vector<std::tuple<std::string, std::string>> idRangesTup;
  std::string idStart("");
  std::string idEnd("");
  for (auto s : idRangesStrs)
  {
    bool onStart = true;
    for (auto c : s) 
    {
      if (c == '-') 
      {
        onStart = false; 
        continue;
      }
      onStart ? idStart += c : idEnd +=c;
    }
    idRangesTup.emplace_back(idStart, idEnd);
    idStart = "";
    idEnd = "";
  }

  long total = 0;
  for (auto t : idRangesTup)
  {
    auto [l,r] = t;
    for (long i = std::stol(l); i <= std::stol(r); i++)
    {
      // i is an int in the range
      // if i is a sequence repeated twice, we should add its value to the total

      // if i has an odd number of digits, it cannot be a sequence repeated twice so we don't need to investigate further
      // couldve just done iStr.length()...
      if (long(std::log10(i) + 1) % 2 != 0) continue;

      // turn the int in the range back into a string and get its first and second halves
      std::string iStr = std::to_string(i);
      std::string lS = iStr.substr(0, iStr.length()/2);
      std::string rS = iStr.substr(iStr.length()/2, iStr.length());

      // compare the halves. if equal, i is a sequence repeated twice.
      if (lS==rS) total += i;
    }
  }

  std::cout << "Total: " << total << std::endl;

  // cleanup and exit
  inputFile.close();
  return 0;
}