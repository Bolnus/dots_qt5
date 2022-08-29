#include "game.h"

Game::Game(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    graphics = 0;
    mainMenu = new MainMenu(); //выделение памяти под объект MainMenu
    stackedWidget->addWidget(mainMenu); // вставляет в stackedWidget объeкт graphics, при этом номер объекта graphics становится 2
    stackedWidget->setCurrentIndex(2); // устанавливает в stackedWidget текущую страницу по номеру


    connect(mainMenu->pushButton,SIGNAL(clicked(bool)),this,SLOT(playGame()));
}

void Game::playGame()
{
    if(graphics == 0)
    {
        graphics = new Graphics();//выделение памяти под объект graphics
        stackedWidget->addWidget(graphics); // вставляет в stackedWidget объeкт graphics, при этом номер объекта graphics становится 2
    }
    stackedWidget->setCurrentIndex(3);
}

Game::~Game()
{
    //delete ui;
}
