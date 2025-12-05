#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <optional>

// Gemini wrote this for me because I don't want to deal with iterators
/**
 * Finds the index of the range containing the given id.
 * * Assumptions:
 * 1. The 'ranges' vector is sorted by the first element (start of range).
 * 2. Ranges are inclusive [start, end].
 * * Complexity: O(log N) due to binary search.
 */

std::optional<int> idInRange(long id, const std::vector<std::pair<long, long>>& ranges)
{
    // std::upper_bound returns the first element where (id < range.first) is true.
    // In other words, it finds the first range that starts strictly AFTER our id.
    auto it = std::upper_bound(ranges.begin(), ranges.end(), id, 
        [](long val, const std::pair<long, long>& range) {
            return val < range.first;
        });

    // If 'it' is the beginning, it means ALL ranges start after the id.
    // Therefore, the id cannot be in any range.
    if (it == ranges.begin()) {
        return std::nullopt;
    }

    // Since 'it' is the first range starting AFTER the id, the only candidate
    // that could possibly contain the id is the range immediately before 'it'.
    auto candidate = std::prev(it);

    // We already know id >= candidate->first (implied by upper_bound logic).
    // We just need to check if id is within the upper bound (candidate->second).
    if (id <= candidate->second) {
        return static_cast<int>(std::distance(ranges.begin(), candidate));
    }

    return std::nullopt;
}

int main()
{
  // load input into a stringstream
  std::ifstream inputFile("../input.txt");
  std::stringstream inputBuffer;
  inputBuffer << inputFile.rdbuf();

  // iterate over the input
  std::string line;
  bool rangesParsed = false;
  std::vector<std::pair<long, long>> idRanges;
  int numInRange = 0;
  while (std::getline(inputBuffer, line))
  {
    if (line == "")
    {
      rangesParsed = true;
      continue;
    }
    if (!rangesParsed) // we are still looping over the ranges
    {
      // assemble a sorted structure of ranges that don't overlap.
      size_t dashPos    = line.find("-");
      long idRangeStart = std::stol(line.substr(0,dashPos));
      long idRangeEnd   = std::stol(line.substr(dashPos+1));
      // if its the first range, just add it
      if (idRanges.empty()) 
      {
        idRanges.emplace_back(idRangeStart, idRangeEnd);
        continue;
      }

      // are left or right in any of the ranges? there are four parent cases
      std::optional<int> pairIndexLeft = idInRange(idRangeStart, idRanges);
      std::optional<int> pairIndexRight = idInRange(idRangeEnd, idRanges);

      // L and R in range
      if (pairIndexLeft && pairIndexRight)
      {
        // L and R in same range: no need to add it
        if (*pairIndexLeft == *pairIndexRight) continue;

        // L and R in different ranges: extend first range to cover second range
        else
        {
          idRanges[*pairIndexLeft].second = idRanges[*pairIndexRight].second;
          // does this work if the two ranges are neighbors in idRanges? we erase (left,right] so that would be (2,3] which is just 3 and that should be fine?
          idRanges.erase(idRanges.begin()+*pairIndexLeft+1, idRanges.end()+*pairIndexRight+1);
        }
      }

      // neither L nor R in range
      if (!pairIndexLeft && !pairIndexRight)
      {
        // find existing ranges contained by new range
        std::vector<int> idxsToRemove;
        for (int i = idRangeStart; i<= idRangeEnd; i++)
        {
          std::optional<int> idRangesIdx = idInRange(i, idRanges);
          if (!idRangesIdx) continue;
          else idxsToRemove.push_back(*idRangesIdx);
        }
        // remove them
        idRanges.erase(/* how to erase a bunch of specific indexes? */);
      }

      // Only L in range
      if (pairIndexLeft)
      {
        // extend pairIndexLeft range to cover idRangeStart to idRangeEnd

      }

      // only R in range
      if (pairIndexRight)
      {
        // inspect id's from ? to ?

        // extend pairIndexRight to cover new range regardless

      }

      // look from idRangeStart to idRangeEnd, find and eliminate existing ranges covered so no overlaps.

    }
    else // now we are checking id's
    {
      long id = std::stol(line);
      if (idInRange(id, idRanges)) numInRange++;
    }
  }

  for (auto r : idRanges)
  {
    std::cout << r.first << " " << r.second << std::endl;
  }

  std::cout << "Number of ID's in ranges: " << numInRange << std::endl;

  // cleanup and exit
  inputFile.close();
  return 0;
}




      // // CASE ONE -- both left and right are in ranges
      // // if both left and right are in ranges,
      // if (pairIndexLeft != -1 && pairIndexRight != -1)
      // {
      //    // already completely enclosed by the same range, no need to add it
      //    if (pairIndexLeft == pairIndexRight) continue;

      //    // otherwise, we span multiple ranges. We should merge the left and right pair into one and absorb any in between.
      //    idRanges[pairIndexLeft].second = idRanges[pairIndexRight].second; // left now spans the entire distance

      //    // remove up to and including pairIndexRight (but not pairIndexLeft)
      //    idRanges.erase(idRanges.begin()+pairIndexLeft+1, idRanges.end()+pairIndexRight+1);
      // }
    
      // // CASE TWO -- either left or right are in a range
      // // check left. if it is in an existing range, we should replace the second item in that range with this new right value and continue. 
      // // but, we should ONLY do that if this new right value is greater than the existing second value. otherwise this range is just a sub-range.
      // if (pairIndexLeft != -1)
      // {
      //   if (idRangeEnd > idRanges[pairIndexLeft].second) idRanges[pairIndexLeft].second = idRangeEnd;
      //   continue;
      // }
      // // check right. if it is in an existing range, we should replace the first item in that range with this new left value and continiue.
      // // but, we should ONLY do that if this new left value is less than the existing first value.
      // if (pairIndexRight != -1)
      // {
      //   if (idRangeStart < idRanges[pairIndexRight].first) idRanges[pairIndexRight].first = idRangeStart;
      //   continue;
      // }

      // // CASE THREE -- neither left nor right are in a range. This either means it is a new range or it contains other ranges.
      // // if it is a new pair, we should add it to either the front or back of idRanges depending on if its larger. 
      // // it is smaller if the second item is less than the first item in the first entry of idRanges and we should emplace front, else, it is greater and should be emplaced back. 
      // if (idRangeEnd < idRanges.front().first) idRanges.emplace(idRanges.begin(), idRangeStart, idRangeEnd);
      // else idRanges.emplace_back(idRangeStart, idRangeEnd);

      // // at this point, we may still have ranges that overlap under the following conditions: 
      // // 1. both left and right were not in a range but enclosed a range
      // // 2. left or right was in a range, the other was not, but enclosed an existing range