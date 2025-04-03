#ifndef SHUFFLEQUEUE_H
#define SHUFFLEQUEUE_H

#include "card.h"

class ShuffleQueue: public QWidget {

protected:
  Card** cards;
  size_t size;

  Card* restartCard;      // temporary card just to reset the queue..
  Game* game;

  int currIndex;
  int rear;
  int counter;                                //to check whether all of the cards have been drawn..
  QPoint initialPos;
  bool emptyWastePile;

public:
  ShuffleQueue();
  ShuffleQueue(QWidget* parent, Game* _game, const size_t _size, const QPoint _initPos);

  virtual void deque();
  void removeCard();
  virtual void restart();

protected:
  // helpers..
  void swap(Card* &one, Card* &two);
};

#endif // SHUFFLEQUEUE_H
