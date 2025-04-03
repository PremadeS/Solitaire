#ifndef SHUFFLEQUEUEHARD_H
#define SHUFFLEQUEUEHARD_H

#include "shufflequeue.h"


class Game;

class ShuffleQueueHard : public ShuffleQueue {
  QVector<ShuffleQueueHard> original;
public:
  ShuffleQueueHard();
  ShuffleQueueHard(QWidget* parent, Game* _game, const size_t _size, const QPoint _initPos);


  void deque() override;
};

#endif // SHUFFLEQUEUEHARD_H
