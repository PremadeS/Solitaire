#ifndef GAME_H
#define GAME_H

#include <QPropertyAnimation>

#include "animator.h"
#include "card.h"
#include "foundationpile.h"
#include "mainstack.h"
#include "nonrepeatingrandomgen.h"
#include "shufflequeue.h"
#include "shufflequeuehard.h"

class Game {

  MainStack **mainStacks;
  size_t mainStacksSize;
  ShuffleQueue *shuffleQueue;

  FoundationPile **foundationPiles;
  size_t foundationPilesSize;

  NonRepeatingRandomGen *rng; // for randomly placing cards...
  Animator *animator;

public:
  // TODO: check which functions should be private.. (helpers)..
  Game(QWidget *parent);

  MainStack **getMainStacks();
  void raiseGroupOfCards(Card *card);
  void moveGroupOfCards(Card *card, QPoint pos);
  bool placeCards(Card *card, const QPoint pos); // returns false if card cannot
                                                 // be placed on that pos...
  bool updateMainStacks(Card *card, MainStack *finalStack);
  bool moveFromQueueToMainStack(
      Card *card,
      MainStack
          *finalStack); // returns false if cannot be placed on that stack..
  bool moveFromPileToMainStack(Card *card, MainStack *finalStack);
  void moveCardsToInitialPosMainStack(
      Card *card,
      QPoint pos); // if the placement isn't valid (can be group of cards)..
  void moveCardToInitialPos(Card *card, QPoint finalpos); // for singleCard...
  bool updateFoundationPile(Card *card, FoundationPile *sp);
  bool checkWin();

  NonRepeatingRandomGen *getNonRepeatingRng();
  Animator *getAnimator();
  void convertValToSuitAndRank(int value, Suit &suit, Rank &rank);

private:
  // helpers bruv..
  void moveFromPileToPile(Card *card, FoundationPile *sp);
  void moveFromShuffleToPile(Card *card, FoundationPile *sp);
  void moveFromMainStackToPile(Card *card, FoundationPile *sp);
  bool isValidMainStackMove(Card *card, MainStack *finalStack);
  bool isValidFoundationPileMove(Card *card, FoundationPile *sp);
};

#endif // GAME_H
