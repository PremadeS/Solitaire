#include "animator.h"
#include "card.h"

#include <QApplication>
#include <QPropertyAnimation>

Animator::Animator() {}

bool Animator::eventFilter(QObject *obj, QEvent *event) {

  if (isAnimating && event->type() == QEvent::MouseButtonPress) {
    return true;
  }
  return QObject::eventFilter(obj, event);
}

void Animator::animateCardToPos(Card *card, QPoint initPos, QPoint finalPos) {
  if (!isAnimating) {
    isAnimating = true;
    QApplication::instance()->installEventFilter(this);
  }

  QPropertyAnimation *animation = new QPropertyAnimation(card, "pos");
  animation->setDuration(500);
  animation->setStartValue(initPos);
  animation->setEndValue(finalPos);
  animation->setEasingCurve(QEasingCurve::InOutCubic);

  connect(animation, &QPropertyAnimation::finished, this, [this]() {
    isAnimating = false;
    QApplication::instance()->removeEventFilter(this);
  });

  animation->start(QAbstractAnimation::DeleteWhenStopped);
}
