#ifndef MAINSTACK_H
#define MAINSTACK_H

#include "card.h"
#include "nonrepeatingrandomgen.h"

class MainStack : public QWidget {

  Card ** cards;
  size_t size;          // using size_t instead of int to look cool..
  QPoint pos;
  Game* game;

public:
  MainStack();
  MainStack(QWidget* parent = nullptr,Game* _game=nullptr, const size_t size = 0,
      const QPoint _initPos = QPoint(0,0));

  int getCardIndex(Card * card);
  Card* getCard(int index);
  size_t getSize();
  QPoint getStackPos();       // returns the position of last card (for obvious reasons...)

  void popCard();
  void popCard(int range);  // for when we want to remova a stack of cards..

  void pushCard(Card* card);
  Card* top() const;
};

#endif // MAINSTACK_H
