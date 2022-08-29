#ifndef MAINMENU_H
#define MAINMENU_H

#include "ui_mainmenu.h"
#include <QFileInfo>
#include <QDir>
#include <QWidget>

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget, public Ui_MainMenu
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private:
    Ui_MainMenu *ui;

private slots:
    void exitGame();

};

#endif // MAINMENU_H
