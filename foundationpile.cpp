#include "foundationpile.h"

FoundationPile::FoundationPile(QWidget *parent, Game *_game, QPoint _pos,
                               Suit _suit)
    : QWidget(parent), game(_game), suit(_suit), cards(nullptr), pos(_pos),
      size(0), capacity(1) {

  cards = new Card *[capacity];
}

void FoundationPile::push(Card *card) {
  if (size == capacity) {
    grow();
  }
  cards[size] = card;
  ++size;
}

void FoundationPile::grow() {
  capacity *= 2;
  Card **newCards = new Card *[capacity];
  for (int i = 0; i < size; ++i) {
    newCards[i] = cards[i];
  }
  if (cards != nullptr) {
    delete[] cards;
  }
  cards = newCards;
}

void FoundationPile::pop() {
  if (size <= 0) {
    throw std::out_of_range("Bruv i hath contain no power to destroy smthng "
                            "that don't exist (in FoundationPile)..");
  }
  --size;
}

Card *FoundationPile::top() const {
  if (size < 0) {
    throw std::out_of_range("Bruv i hath contain no power to show smthng that "
                            "don't exist (in FoundationPile)..");
  }
  return cards[size - 1];
}

QPoint FoundationPile::getPos() const { return pos; }

int FoundationPile::getSize() const { return size; }

Suit FoundationPile::getSuit() const { return suit; }

Rank FoundationPile::getRank() const { return currentRank; }

void FoundationPile::setRank(const Rank r) { currentRank = r; }
