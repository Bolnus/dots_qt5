#ifndef GAME_H
#define GAME_H

#include <QWidget>

#include "ui_game.h"
#include "graphics.h"
#include "mainmenu.h"

namespace Ui {
class Game;
}

class Game : public QWidget, Ui_Game
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = 0);
    ~Game();

private:
    Ui::Game *ui;
    Graphics *graphics;
    MainMenu *mainMenu;

private slots:
    void playGame();
};

#endif // GAME_H
