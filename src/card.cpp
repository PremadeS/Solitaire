#include "card.h"
#include "game.h"
#include "shufflequeue.h"

Card::Card() {}

Card::Card(QWidget *parent, Game *_game, MainStack *_mainStackParent,
           const QPoint initPos, bool _visible, Suit _suit, Rank _rank)
    : QLabel(parent), mainStackParent(_mainStackParent),
      foundationPileParent(nullptr), game(_game), visible(_visible),
      dragging(false), initialPos(initPos), shuffleQueueParent(nullptr),
      outsideShuffleQueue(true), pickable(true), rank(_rank), suit(_suit) {

  int ra = static_cast<int>(rank);
  int su = static_cast<int>(suit);

  QString r = QString::number(ra);
  QString s = QString::number(su);

  QPixmap cardImg("../assets/Cards/Modern/" + s + r + ".png");

  if (!visible) {
    cardImg.load("../assets/Backs/Card-Back-01.png");
  }
  this->setPixmap(cardImg.scaled(100, 150));
  move(initialPos);
}

Card::Card(const Card *card, MainStack *_mainStackParent)
    : QLabel(static_cast<QWidget *>(card->parent())),
      mainStackParent(_mainStackParent), game(card->game),
      visible(card->visible), dragging(false), initialPos(card->initialPos) {

  QPixmap cardImg("../assets/Cards/Modern/c07");

  if (!visible) {
    cardImg.load("../assets/Backs/Card-Back-01.png");
  }
  this->setPixmap(cardImg.scaled(100, 150));
  move(initialPos);
}

void Card::mousePressEvent(QMouseEvent *event) {
  // if we click MainStack cards...
  if (pickable && visible &&
      (mainStackParent != nullptr || outsideShuffleQueue) &&
      event->button() == Qt::LeftButton) {

    if (mainStackParent != nullptr) {
      game->raiseGroupOfCards(
          this); // cannot raise a group of cards other than in mainstack..
    } else {     // else only raise the picked card..
      raise();
    }

    startPos = event->pos(); // Capture where the card is picked up
    dragging = true;
    qDebug() << "Card picked from:" << this->pos();
  }

  // if we click ShuffleQueue cards...
  else if (pickable && mainStackParent == nullptr &&
           event->button() == Qt::LeftButton) {
    shuffleQueueParent->deque();
  }
}

void Card::mouseMoveEvent(QMouseEvent *event) {
  if ((mainStackParent != nullptr || outsideShuffleQueue) && dragging) {
    QPoint change = event->pos() - startPos;
    move(mapToParent(change)); // Drag the card

    if (mainStackParent != nullptr)
      game->moveGroupOfCards(this, change);
  }
}

void Card::mouseReleaseEvent(QMouseEvent *event) {
  if (dragging && event->button() == Qt::LeftButton) {
    dragging = false;
    qDebug() << "Card dropped at:" << this->pos();
  }
  if (!game->placeCards(this, this->pos())) {
    if (mainStackParent != nullptr)
      game->moveCardsToInitialPosMainStack(this, this->pos());
    else if (outsideShuffleQueue)
      game->moveCardToInitialPos(this, this->pos());
  } else if (game->checkWin()) {
    qDebug() << " END *************";
  }
}

bool Card::isValidEndPos(const QPoint &pos) {
  return (pos.x() >= 0 && pos.x() <= 200) && (pos.y() >= 0 && pos.y() <= 200);
}

MainStack *Card::getMainStackParent() { return mainStackParent; }

void Card::moveToPos(const QPoint pos) { move(pos); }

void Card::setMainStackParent(MainStack *_mainStackParent) {
  mainStackParent = _mainStackParent;
}

void Card::setVisibility(bool val) {

  if (!visible && val) {
    QPixmap cardImg("../assets/Cards/Modern/" +
                    QString::number(static_cast<int>(suit)) +
                    QString::number(static_cast<int>(rank)) + ".png");
    this->setPixmap(cardImg.scaled(100, 150));
  } else if (visible && !val) {
    QPixmap cardImg("../assets/Backs/Card-Back-01.png");
    this->setPixmap(cardImg.scaled(100, 150));
  }
  visible = val;
}

bool Card::isVisible() { return visible; }

void Card::setInitialPos(QPoint _initialPos) { initialPos = _initialPos; }

QPoint Card::getInitialPos() { return initialPos; }

void Card::setShuffleQueueParent(ShuffleQueue *_sq) {
  shuffleQueueParent = _sq;
}

ShuffleQueue *Card::getShuffleQueueParent() { return shuffleQueueParent; }

void Card::setOutsideShuffleQueue(bool val) { outsideShuffleQueue = val; }

bool Card::getOutsideShuffleQueue() { return outsideShuffleQueue; }

void Card::setPickable(bool val) { pickable = val; }

bool Card::getPickable() { return pickable; }

void Card::setFoundationPileParent(FoundationPile *sp) {
  foundationPileParent = sp;
}

FoundationPile *Card::getFoundationPileParent() const {
  return foundationPileParent;
}

Suit Card::getSuit() const { return suit; }

Rank Card::getRank() const { return rank; }
