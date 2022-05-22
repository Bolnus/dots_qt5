#include "game.h"


Game::Game(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    graphics = new Graphics();
    stackedWidget->addWidget(graphics);
    stackedWidget->setCurrentIndex(2);
}

Game::~Game()
{
    //delete ui;
}
