#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QApplication>
#include <qmath.h>

#include "dotitem.h"



class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(int scale);
    void setEmptyDots(QGraphicsLineItem *lineItem);
    void addDotItem(qreal posX,qreal posY);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    //virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    bool drawing;
    QGraphicsLineItem *currentLineItem;
    QList<QGraphicsLineItem *> lineChain;
    QList<DotItem *> dotsChain;
    DotItem *currentStartItem;
    DotItem *chainStartItem;
    int gridScale;
    //bool firstFirstDot;
    //bool firstSecondDot;

    DotItem * getDotItem(QPointF point);
    bool isNeighbouringDotItem(DotItem *dotItemStart,DotItem *dotItemFinish);
    bool paintArea();
    void resetDot(DotItem *dotItem);

private slots:
    void slotClearGameProgress();
    void slotUndo();

signals:
    void signal_firstDotClicked(DotItem *,int);
    void signal_turnChanged();
    void signal_changeScore(int,QColor);
    void signal_saveHistoryScore(DotItem *);
    void signal_regainHistoryScore(DotItem *);
};

#endif // GRAPHICSSCENE_H
