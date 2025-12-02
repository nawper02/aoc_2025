#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <cmath>
#include <sstream>

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
      // if i is a sequence repeated any number of times, it is an invalid ID and we should increment the total by that ID's value
      bool isBadId = false;
      std::string iStr = std::to_string(i);
      for (int j = 1; j <= iStr.length()/2; j++) // j is the possible sequence length to check
      {
        // if iStr is made up of only the j-length slice of iStr repeated, then it is a bad ID
        if (iStr.length() % j != 0) continue; // j slice cannot fit into iStr evenly
        // we know a slice of j fits into iStr, now we need to see if iStr is made up of jStr repeated over and over
        // to check for this, duplicate jStr as many times as it would take to be the same length as iStr
        std::string jStr = iStr.substr(0, j); // j-length slice of iStr
        std::stringstream jStrRepeated;
        for (int i = 0; i < (iStr.length()/jStr.length()); i++) {
          jStrRepeated << jStr;
        }
        if (iStr == jStrRepeated.str()) isBadId = true;
      }
      if (isBadId) total += i;
    }
  }

  std::cout << "Total: " << total << std::endl;

  // cleanup and exit
  inputFile.close();
  return 0;
}