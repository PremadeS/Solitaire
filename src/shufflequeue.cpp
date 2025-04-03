#include "shufflequeue.h"
#include "game.h"

ShuffleQueue::ShuffleQueue() {}

ShuffleQueue::ShuffleQueue(QWidget *parent, Game *_game, size_t _size,
                           QPoint _initPos)
    : QWidget(parent), game(_game), size(_size), initialPos(_initPos),
      currIndex(1), rear(size - 1), counter(size), emptyWastePile(true) {

  cards = new Card *[size];
  for (int i = 0; i < size; ++i) {
    Rank r;
    Suit s;
    game->convertValToSuitAndRank(game->getNonRepeatingRng()->next(), s, r);

    cards[i] = new Card(parent, game, nullptr, initialPos, false, s, r);
    cards[i]->setShuffleQueueParent(this);
    cards[i]->setOutsideShuffleQueue(
        false); // inside the shuffle queue at the time of making..
  }
  restartCard = new Card(parent, game, nullptr, initialPos, false, Suit::Clubs,
                         Rank::Eight);
  restartCard->setShuffleQueueParent(this);
  restartCard->setOutsideShuffleQueue(false);

  restartCard->lower();

  QPixmap cardImg("assets/Backs/Card-Back-05.png");
  restartCard->setPixmap(cardImg.scaled(100, 150));
}

void ShuffleQueue::deque() {

  if (counter == 0) {
    restart(); // when all of the cards have been drawn..
    qDebug() << "Empty Pile\n";
  }

  Card *card = cards[rear];

  QPoint newPos(initialPos.x(), initialPos.y() + 200 + ((currIndex - 1) * 30));

  // game->getAnimator()->animateCardToPos(card, card->getInitialPos());

  card->moveToPos(newPos);
  card->setInitialPos(newPos);
  card->setVisibility(true);
  card->raise();
  card->setOutsideShuffleQueue(true);

  if (currIndex != 1 &&
      rear + 1 <
          size) { // if currIndex = 0 meaning there is no card underneath..
    cards[rear + 1]->setPickable(false); // cannot move the card underneath..
  }

  if (currIndex == 3 && !emptyWastePile) {
    for (int i = 1; i <= 2 && rear + i < size; ++i) {
      QPoint initPos = cards[rear + i]->getInitialPos();
      QPoint newPos(initPos.x(), initPos.y() - 30);
      cards[rear + i]->moveToPos(newPos);
      cards[rear + i]->setInitialPos(newPos);
    }
  }

  if (currIndex + 1 > 3) {
    currIndex = 3;
    if (emptyWastePile)
      emptyWastePile = false;
  } else
    ++currIndex;

  --rear;
  --counter;
}

void ShuffleQueue::removeCard() {

  if (rear + 2 < size) {
    cards[rear + 2]->setPickable(true);
    // qDebug() << cards[rear + 2]->isVisible() << "\n";
  }

  Card **newCards = new Card *[size - 1];
  for (int i = 0; i <= rear; ++i) {
    newCards[i] = cards[i];
  }

  // remove the rear + 1 element (the last element that was dequed)..
  for (int j = rear + 2; j < size; ++j) {
    newCards[j - 1] = cards[j];
  }
  if (cards != nullptr)
    delete[] cards;

  cards = newCards;
  --size;

  /* --------------------------------------------------------------------------------------------
  * two possbilities if last card is removed currIndex will be 1 if all of the
  cards are removed
  * currIndex will be 1 so need to check both, if last card is removed index
  should be 3 else 1..
  -----------------------------------------------------------------------------------------------
*/

  if (rear + 3 < size) {
    for (int i = 1; i <= 2 && rear + i < size && counter - 3 < size; ++i) {
      QPoint initPos = cards[rear + i]->getInitialPos();
      QPoint newPos(initPos.x(), initPos.y() + 30);
      game->getAnimator()->animateCardToPos(cards[rear + i], initPos, newPos);
      // cards[rear + i]->moveToPos(newPos);
      cards[rear + i]->setInitialPos(newPos);
    }
  }

  if (size - rear <= 3) {
    emptyWastePile = true;
  }

  if (emptyWastePile) {
    if (rear + 2 < size) { // last card removed..
      currIndex = 3;
    } else if (currIndex <= 1) { // all cards removed.. (safe measures usually
                                 // wont go under 1..)
      currIndex = 1;
    } else {
      --currIndex;
    }
  }
}

void ShuffleQueue::swap(Card *&one, Card *&two) {
  Card *temp = one;
  one = two;
  two = temp;
}

void ShuffleQueue::restart() {
  // put all of the **visible** cards back..
  for (int i = rear + 1; i < size; ++i) {
    Card *c = cards[i];
    c->lower();
    c->setInitialPos(initialPos);
    c->moveToPos(initialPos);
    c->setPickable(true);
    c->setOutsideShuffleQueue(false);
    c->setVisibility(false);
  }
  counter = size;
  currIndex = 1;
  rear = size - 1;
  emptyWastePile = true;
  restartCard->lower();
}
