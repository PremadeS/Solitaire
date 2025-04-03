#include "game.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QWidget window;
  window.setFixedSize(1280, 720);

  Game game(&window);

  window.setStyleSheet("background-color: green");
  window.show();

  return a.exec();
}
