#ifndef CARD_H
#define CARD_H

// TODO: check const..

#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
// #include "mainstack.h"

// forawrd declarations...
class MainStack;
class ShuffleQueue;
class FoundationPile;
class Game;

enum Suit {
  Hearts,
  Diamonds,
  Clubs,
  Spades,
};

enum Rank {
  Ace = 1,
  Two,
  Three,
  Four,
  Five,
  Six,
  Seven,
  Eight,
  Nine,
  Ten,
  Jack,
  Queen,
  King,
};

class Card : public QLabel {

  MainStack *mainStackParent;
  ShuffleQueue *shuffleQueueParent;
  FoundationPile *foundationPileParent;
  Game *game;

  QPoint initialPos;
  QPoint startPos;
  bool dragging;
  bool visible;
  bool outsideShuffleQueue;
  bool pickable;

  Rank rank;
  Suit suit;

public:
  Card();

  Card(QWidget *parent = nullptr, Game *_game = nullptr,
       MainStack *_mainStackParent = nullptr,
       const QPoint initPos = QPoint(0, 0), bool _visible = false,
       Suit _suit = Hearts, Rank _rank = Ace);

  Card(const Card *card, MainStack *_mainStackParent); // copy constructor

  void moveToPos(const QPoint pos);

  void setMainStackParent(MainStack *_mainStackParent);

  MainStack *getMainStackParent();

  void setVisibility(bool val);

  bool isVisible();

  void setInitialPos(QPoint _initialPos);

  QPoint getInitialPos();

  void setShuffleQueueParent(ShuffleQueue *_sq);

  ShuffleQueue *getShuffleQueueParent();

  void setOutsideShuffleQueue(bool val);

  bool getOutsideShuffleQueue();

  void setPickable(bool val);

  bool getPickable();

  void setFoundationPileParent(FoundationPile *sp);

  FoundationPile *getFoundationPileParent() const;

  Suit getSuit() const;

  Rank getRank() const;

protected:
  void mousePressEvent(QMouseEvent *event) override;

  void mouseMoveEvent(QMouseEvent *event) override;

  void mouseReleaseEvent(QMouseEvent *event) override;

  bool isValidEndPos(const QPoint &pos);
};

#endif // CARD_H
