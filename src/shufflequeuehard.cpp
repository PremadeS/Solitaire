#include "shufflequeuehard.h"
#include "game.h"
#include <QTimer>

ShuffleQueueHard::ShuffleQueueHard() {}

ShuffleQueueHard::ShuffleQueueHard(QWidget *parent, Game *_game, size_t _size,
                                   QPoint _initPos)
    : ShuffleQueue(parent, _game, _size, _initPos) {}

void ShuffleQueueHard::deque() {

  for (int i = 0; i < 3; ++i) {
    if (rear == 0) {
      ShuffleQueue::deque();
      break;
    }
    ShuffleQueue::deque();
  }
}
