#ifndef GRAPHICS_H
#define GRAPHICS_H


#include <QWidget>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QShortcut>
#include <QTimer>

#include "ui_graphics.h"

#include "graphicsscene.h"



namespace Ui {
class Graphics;
}

class Graphics : public QWidget, Ui_Graphics
{
    Q_OBJECT

public:
    explicit Graphics(QWidget *parent = 0);
    ~Graphics();
    bool eventFilter(QObject *object, QEvent *event);


private:
    GraphicsScene *scene;
    Ui::Graphics *ui;
    QShortcut *keyEsc;

    int resizeCount;
    int gridScale;
    void initScene();
    void setEmptyDots(QGraphicsLineItem *lineItem);


private slots:
    void slotSetCenterOn(DotItem *dotItem, int dotNumber);
    void slotShowTurnChange();
    void slotchangeScore(int scoreChange,QColor playerColor);
    void slotSaveHistoryScore(DotItem *);
    void slotRegainHistoryScore(DotItem *);
    void slotClearGameProgress();
    void slotProgressBar();
};

#endif // GRAPHICS_H
