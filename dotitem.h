#ifndef DOTITEM_H
#define DOTITEM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QRectF>

#define DATA_IS_DOT Qt::UserRole

class DotItem : public QGraphicsItem
{
public:
    DotItem(qreal posx,qreal posy);
    virtual QRectF boundingRect() const;
    bool isSet();
    void setDot();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QColor currentColor();
    int resetDot();
    int getSetNumber();
    void setLineChain(QGraphicsPolygonItem *newLineChain, int scoreAmplification);
    QGraphicsPolygonItem * getPolygonItem();
    QGraphicsPolygonItem *getLineCHain();
    void setBlocked(bool isBlocked);
    bool isBlocked();
    void moveDotItem(qreal posX,qreal posY);
    void markDot();
    int getScoreNumber();
    void setHistoryScore(QString newHistoryScore);
    QString getHistoryScore();
    int polygonPaintedLevel();
    void setPolygonPaintedLevel(int polPaintedLevel);

    //static bool turn;
    static int dotsCounter;


protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    //void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    //void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QColor color;
    bool set;
    bool blocked;
    QGraphicsPolygonItem *lineChain;
    QRectF dotBoundingRect;
    int score;
    int setNumber;
    QString historyScore;
    //bool active; //содержит полигон, который влияет на счет (незакрашенный)
    int paintedLevel;
};

#endif // DOTITEM_H
