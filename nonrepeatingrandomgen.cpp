#include "nonrepeatingrandomgen.h"

NonRepeatingRandomGen::NonRepeatingRandomGen(int min, int max) : currIndex(0) {
  for (int i = min; i <= max; ++i) {
    nums.append(i);
  }

  // shuffle in random order bruv..
  QRandomGenerator rng;
  std::shuffle(nums.begin(), nums.end(), rng);
}

int NonRepeatingRandomGen::next() {
  if (currIndex == nums.size()) {
    throw std::out_of_range(
        "Error bruv no more elements left in generator lol..");
  }
  return nums[currIndex++];
}
