#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <QObject>
#include <QPropertyAnimation>
#include <QPoint>
#include <QEvent>

class Card;

class Animator : public QObject {
  Q_OBJECT
public:
  Animator();
  void animateCardToPos(Card* card, QPoint initPos, QPoint finalPos);

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  bool isAnimating = false;
};

#endif // ANIMATOR_H
