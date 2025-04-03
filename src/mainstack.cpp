#include "mainstack.h"
#include "game.h"
#include <QRandomGenerator>

MainStack::MainStack() {}

MainStack::MainStack(QWidget *parent, Game *_game, const size_t _size,
                     const QPoint _initPos)
    : QWidget(parent), game(_game), size(_size), pos(_initPos) {
  cards = new Card *[size];
  for (int i = 0; i < size; ++i) {

    Suit s;
    Rank r;
    game->convertValToSuitAndRank(game->getNonRepeatingRng()->next(), s, r);

    // CHANGE RANK..
    cards[i] = new Card(parent, game, this, QPoint(pos.x(), pos.y() + (i * 30)),
                        i == size - 1 || i == size - 2, s,
                        r); // make last card visible..
  }
}

int MainStack::getCardIndex(Card *card) {
  for (int i = 0; i < size; ++i) {
    if (cards[i] == card) {
      return i;
    }
  }
  return -1; // card not found...
}

Card *MainStack::getCard(int index) {
  if (index < 0 || index >= size) {
    throw "Err index is out of range bruv.. No card found :(";
  }
  return cards[index];
}

size_t MainStack::getSize() { return size; }
QPoint MainStack::getStackPos() {
  return QPoint(pos.x(), pos.y() + (size * 30));
}

// TODO: change to actual stack impl...
void MainStack::popCard() { popCard(1); }

void MainStack::popCard(
    int range) { // for when we want to remova a stack of cards..
  size -= range;
  if (size > 0) {
    cards[size - 1]->setVisibility(true);
  }
}

void MainStack::pushCard(Card *card) {
  Card **newCards = new Card *[size + 1];
  for (int i = 0; i < size; ++i) {
    newCards[i] = cards[i];
  }
  newCards[size] = card;
  if (cards != nullptr) {
    delete[] cards;
  }
  cards = newCards;

  ++size;
}

Card *MainStack::top() const { return cards[size - 1]; }
