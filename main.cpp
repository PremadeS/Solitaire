#include <QApplication>
#include <QLabel>
#include <QMovie>
#include <QWidget>

#include "game.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QWidget window;
  window.setFixedSize(1280, 720);

  QLabel *backgroundLabel = new QLabel(&window);
  backgroundLabel->setGeometry(0, 0, window.width(), window.height());

  QMovie *movie = new QMovie("../assets/bg/bg.gif");
  backgroundLabel->setMovie(movie);
  movie->start();

  Game game(&window);

  window.show();

  return a.exec();
}
