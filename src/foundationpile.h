#ifndef FOUNDATIONPILE_H
#define FOUNDATIONPILE_H

#include "card.h"

class FoundationPile : public QWidget {

  QPoint pos;
  Card **cards;
  Game *game;

  int size;
  int capacity;

  Suit suit;
  Rank currentRank;

public:
  FoundationPile(QWidget *parent, Game *_game, QPoint _pos, Suit _suit);
  void push(Card *card);
  void pop();
  Card *top() const;

  QPoint getPos() const;
  int getSize() const;

  void setRank(const Rank r);

  Suit getSuit() const;
  Rank getRank() const;

private:
  // helper..
  void grow();
};

#endif // FOUNDATIONPILE_H
