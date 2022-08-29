#include "mainmenu.h"


MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    QDir::setCurrent(QApplication::applicationDirPath());
//    if(QFileInfo::exists("explosion_PNG15367.png"))
//        label->setPixmap(QPixmap("explosion_PNG15367.png"));


    connect(pushButton_2,SIGNAL(clicked(bool)),this,SLOT(exitGame()));
}

void MainMenu::exitGame()
{
    exit(0);
}

MainMenu::~MainMenu()
{

}
