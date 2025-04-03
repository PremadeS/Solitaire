#ifndef NONREPEATINGRANDOMGEN_H
#define NONREPEATINGRANDOMGEN_H
#include <QList>
#include <QRandomGenerator>

class NonRepeatingRandomGen {
  QList<int> nums;
  int currIndex;

public:
  NonRepeatingRandomGen(int min, int max);
  int next();
};

#endif // NONREPEATINGRANDOMGEN_H
