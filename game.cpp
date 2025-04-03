#include "game.h"

#define totalMainStacks 7 // pretty self explanatory (big words I know)..
#define totalFoundationPiles 4
#define offsetX 200
#define offsetY 30

// TODO UNCOMMENT ALL OF THE VALID MOVESSSSSS!!!!!!!!!!!!!!!!!!....

Game::Game(QWidget *parent)
    : mainStacksSize(totalMainStacks),
      foundationPilesSize(totalFoundationPiles) {
  rng = new NonRepeatingRandomGen(1, 52);
  animator = new Animator;

  mainStacks = new MainStack *[mainStacksSize];
  for (int i = 0; i < mainStacksSize; ++i) {
    mainStacks[i] = new MainStack(parent, this, i + 1,
                                  QPoint(offsetX + (i * 130), offsetY));
  }
  shuffleQueue = new ShuffleQueueHard(parent, this, 20, QPoint(30, 30));

  foundationPiles = new FoundationPile *[foundationPilesSize];
  for (int i = 0; i < foundationPilesSize; ++i) {
    foundationPiles[i] =
        new FoundationPile(parent, this, QPoint(1150, 30 + (i * 170)), static_cast<Suit>(i));
  }
}

MainStack **Game::getMainStacks() { return mainStacks; }

// for dragging animation of stack of cards...
void Game::moveGroupOfCards(Card *card, QPoint pos) {

  MainStack *stack = card->getMainStackParent();
  int index = stack->getCardIndex(card);
  size_t size = stack->getSize();
  for (int i = index + 1; i < size; ++i) {
    Card *currentCard = stack->getCard(i);
    currentCard->move(currentCard->pos() + pos);
  }
}

void Game::raiseGroupOfCards(Card *card) {
  MainStack *stack = card->getMainStackParent();
  int index = stack->getCardIndex(card);
  int size = stack->getSize();
  for (int i = index; i < size; ++i) {
    Card *c = stack->getCard(i);
    c->raise();
  }
}

bool Game::placeCards(Card *card, const QPoint pos) {
  // check placement on mainStack..
  for (int i = 0; i < mainStacksSize; ++i) {
    QPoint stackPos = mainStacks[i]->getStackPos();
    if (abs(stackPos.x() - pos.x()) <= 50 &&
        abs(stackPos.y() - pos.y()) <= 50) {
      if (card->getMainStackParent() == nullptr &&
          card->getOutsideShuffleQueue() == true &&
          card->getFoundationPileParent() == nullptr)
        return moveFromQueueToMainStack(card, mainStacks[i]);
      else if (card->getFoundationPileParent() == nullptr)
        return updateMainStacks(card, mainStacks[i]);
      else
        return moveFromPileToMainStack(card, mainStacks[i]);
    }
  }
  // check placement on foundationpile..
  for (int j = 0; j < totalFoundationPiles; ++j) {
    QPoint foundationPilePos = foundationPiles[j]->getPos();
    if (abs(foundationPilePos.x() - pos.x()) <= 50 &&
        abs(foundationPilePos.y() - pos.y()) <= 50) {
      return updateFoundationPile(card, foundationPiles[j]);
    }
  }
  return false;
}

bool Game::updateMainStacks(Card *card, MainStack *finalStack) {

  if (!isValidMainStackMove(card, finalStack))
    return false;

  MainStack *initialStack = card->getMainStackParent();

  if (finalStack == initialStack ||
      finalStack->getSize() ==
          0) // will automatically move cards back to initialPos..
    return false;

  int index = initialStack->getCardIndex(card);
  int cardsSize =
      initialStack->getSize(); // get the stack of cards (if it is a stack)...

  QPoint finalStackPos = finalStack->getStackPos();
  for (int i = index; i < cardsSize;
       ++i) { // copy all of the cards to new stack..

    Card *initialCard = initialStack->getCard(i);
    QPoint newPos(finalStackPos.x(),
                  offsetY + (finalStack->getSize() * offsetY));

    initialCard->setInitialPos(newPos);
    initialCard->moveToPos(newPos);
    initialCard->setMainStackParent(finalStack);

    finalStack->pushCard(initialCard); // add to new stack..
  }

  initialStack->popCard(cardsSize - index); // remove from previous stack..
  return true;
}

bool Game::isValidMainStackMove(Card *card, MainStack *finalStack) {
  if (finalStack->top()->getSuit() == card->getSuit()) {
    return false; // can't put card of same suit on main stack..
  }

  if (static_cast<int>(finalStack->top()->getRank()) !=
      static_cast<int>(card->getRank()) +
          1) { // new card need to be of one less rank than the older one..
    return false;
  }

  return true;
}

// TODO:

/* +------------------------------------+
   |  * mainStack -> foundationPile --- done |
   |  * foundationPile -> foundationPile --- done |
   |  * foundationPile -> mainStack --- done |
   |  * shuffleQueue -> foundationPile --- d |
   +------------------------------------+  */

bool Game::updateFoundationPile(Card *card, FoundationPile *sp) {

  if (!isValidFoundationPileMove(card, sp))
    return false;

  MainStack *ms = card->getMainStackParent();

  /*--- didn't pick the card from mainstack.. (could be from other
   * foundationpile or shuffleQueue) ---*/
  if (card->getMainStackParent() == nullptr) {
    if (card->getFoundationPileParent() ==
        sp) { // trying to place it on the same pile..
      return false;
    }
    if (card->getFoundationPileParent() !=
        nullptr) { // picked from another foundationpile..
      moveFromPileToPile(card, sp);
    } else { // picked card from shuffleQueue..
      moveFromShuffleToPile(card, sp);
    }
    return true;
  }

  if (ms->getCardIndex(card) !=
      ms->getSize() -
          1) { // cannot put more than 1 cards at a time on foundationPile..
    return false;
  }

  moveFromMainStackToPile(card, sp); // moved from mainstack to foundationpile..
  return true;
}

bool Game::isValidFoundationPileMove(Card *card, FoundationPile *sp) {

  if (sp->getSuit() != card->getSuit())
    return false;

  if (sp->getSize() == 0 &&
      card->getRank() == Rank::Ace) // first card must be Ace..
    return true;
  else if (sp->getSize() == 0)
    return false;

  if (static_cast<int>(sp->getRank()) !=
      static_cast<int>(card->getRank()) -
          1) // Can only place card of one bigger rank on top..
    return false;

  return true;
}

bool Game::moveFromPileToMainStack(Card *card, MainStack *ms) {

  if (!isValidMainStackMove(card, ms))
    return false;

  FoundationPile *sp = card->getFoundationPileParent();
  QPoint newPos = ms->getStackPos();
  sp->pop();
  if (sp->getSize() != 0)
    sp->setRank(sp->top()->getRank()); // update sp currnet rank..
  card->setInitialPos(newPos);
  card->moveToPos(newPos);
  card->setMainStackParent(ms);
  card->setFoundationPileParent(nullptr);

  ms->pushCard(card);
  return true;
}

void Game::moveFromShuffleToPile(Card *card, FoundationPile *sp) {
  ShuffleQueue *sq = card->getShuffleQueueParent();
  QPoint newPos = sp->getPos();
  sq->removeCard();
  card->setInitialPos(newPos);
  card->moveToPos(newPos);
  card->setFoundationPileParent(sp);
  card->setShuffleQueueParent(nullptr);
  sp->push(card);
  sp->setRank(card->getRank()); // change current rank of sp..
}

void Game::moveFromMainStackToPile(Card *card, FoundationPile *sp) {
  MainStack *ms = card->getMainStackParent();
  QPoint newPos = sp->getPos();
  ms->popCard();
  card->setFoundationPileParent(sp);
  card->setMainStackParent(nullptr);
  card->moveToPos(newPos);
  card->setInitialPos(newPos);
  sp->push(card);
  sp->setRank(card->getRank()); // change current rank of sp..
}

void Game::moveFromPileToPile(Card *card, FoundationPile *sp) {
  FoundationPile *currFoundationPileParent = card->getFoundationPileParent();
  QPoint newPos = sp->getPos();
  currFoundationPileParent->pop(); // remove the card from curr pile..
  card->setFoundationPileParent(sp);
  card->moveToPos(newPos);
  card->setInitialPos(newPos);
  sp->push(card);               // add to new pile..
  sp->setRank(card->getRank()); // change current rank of sp..
}

void Game::moveCardsToInitialPosMainStack(Card *card, QPoint finalPos) {

  MainStack *stack = card->getMainStackParent();
  int size = stack->getSize();
  int index = stack->getCardIndex(card);
  for (int i = index; i < size; ++i) {
    Card *c = stack->getCard(i);
    moveCardToInitialPos(
        c, QPoint(finalPos.x(), finalPos.y() + ((i - index) * 30)));
  }
}

void Game::moveCardToInitialPos(Card *card, QPoint finalPos) {

  animator->animateCardToPos(card, finalPos, card->getInitialPos());

  // card->set(card->getInitialPos());
}

// TODO: remove from queue..
bool Game::moveFromQueueToMainStack(Card *card, MainStack *finalStack) {
  if (!isValidMainStackMove(card, finalStack))
    return false;

  if (finalStack->getSize() == 0) // cannot place cards on empty stack...
    return false;

  // move to new position..
  QPoint finalStackPos = finalStack->getStackPos();
  QPoint newPos(finalStackPos.x(), offsetY + (finalStack->getSize() * offsetY));
  card->moveToPos(newPos);
  card->setInitialPos(newPos);

  // other logical stuff..
  card->setMainStackParent(finalStack);

  ShuffleQueue *sq = card->getShuffleQueueParent();
  sq->removeCard();

  card->setShuffleQueueParent(nullptr); // no longer in shuffleQueue :(

  finalStack->pushCard(card);

  return true;
}

bool Game::checkWin() {

  for (int i = 0; i < foundationPilesSize; ++i) {
    if (foundationPiles[i]->getSize() == 0 ||
        foundationPiles[i]->top()->getRank() != Rank::King) {
      return false;
    }
  }
  return true;
}

NonRepeatingRandomGen *Game::getNonRepeatingRng() { return rng; }
void Game::convertValToSuitAndRank(int val, Suit &suit, Rank &rank) {
  suit = static_cast<Suit>((val - 1) / 13);
  rank = static_cast<Rank>(((val - 1) % 13) + 1);
}

Animator *Game::getAnimator() { return animator; }
