#include "dotitem.h"

DotItem::DotItem(qreal posx, qreal posy)
{
    setAcceptHoverEvents(1);
    //setAcceptedMouseButtons(1);
    //setFlags();
    qreal penWidth = 1;
    //QRectF rect();
    dotBoundingRect=QRectF(-10-penWidth/2, -10-penWidth/2, 20+penWidth, 20+penWidth);
    color=QColor(Qt::black);
    set=0;
    blocked=0;
    setData(DATA_IS_DOT,1);
    setNumber=0;
    lineChain=0;
    score=0;
    historyScore="";
    paintedLevel=0;
}

//bool DotItem::turn = 0;
int DotItem::dotsCounter = 0;

void DotItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if(DotItem::dotsCounter%2)
        color=QColor(Qt::red);
    else
        color=QColor(Qt::blue);
    //qDebug("hover move");
    update();

}

bool DotItem::isSet()
{
    return set;
}

//void DotItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
//{
//    if(DotItem::turn)
//        color=QColor(Qt::red);
//    else
//        color=QColor(Qt::blue);
//    qDebug("hover move");
//    update();
//}

void DotItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
//    if(acceptsHoverEvents())
//        qDebug("wtf?");
    color=QColor(Qt::black);
    //qDebug("hover leave");
    update();
}

//void DotItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(!set&&event->button()==Qt::LeftButton)
//    {

//    }
//    else if(!set&&event->button()==Qt::RightButton)
//    {

//    }
//}

void DotItem::setDot()
{
    if((DotItem::dotsCounter%2&&color==QColor(Qt::red))||(DotItem::dotsCounter%2==0&&color==QColor(Qt::blue)))
    {
        set=1;
        DotItem::dotsCounter++;
        setNumber=DotItem::dotsCounter;
        setAcceptHoverEvents(0);
    }
}

int DotItem::resetDot()
{
    set=0;
    DotItem::dotsCounter--;
    setNumber=0;
    setAcceptHoverEvents(1);
    color=QColor(Qt::black);
    update();
    if(lineChain)
    {
        delete lineChain;
        lineChain=0;
        historyScore="";
        int scoreAmplification = -score;
        score=0;
        paintedLevel=0;
        return scoreAmplification;
    }
    return 0;
}

QColor DotItem::currentColor()
{
    return color;
}

QGraphicsPolygonItem * DotItem::getPolygonItem()
{
    if(lineChain)
        return lineChain;
    else
        return 0;
}

QRectF DotItem::boundingRect() const
{
//    qreal penWidth = 1;
//    QRectF rect(-10-penWidth/2, -10-penWidth/2, 20+penWidth, 20+penWidth);
    return dotBoundingRect;
}

void DotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(color);
    painter->setPen(color);
    if(color==QColor(Qt::black))
        painter->drawRoundedRect(-0.5, -0.5, 1, 1, 1, 1);
    else
        painter->drawRoundedRect(-3, -3, 6, 6, 3, 3);
    //setPos(dotBoundingRect.center());
}

int DotItem::getSetNumber()
{
    return setNumber;
}

void DotItem::setLineChain(QGraphicsPolygonItem *newLineChain, int scoreAmplification)
{
    lineChain = newLineChain;
    score=scoreAmplification;
}

QGraphicsPolygonItem *DotItem::getLineCHain()
{
    return lineChain;
}

void DotItem::setBlocked(bool isBlocked)
{
    blocked=isBlocked;
    if(blocked)
        setAcceptHoverEvents(0);
    else if(!set)
        setAcceptHoverEvents(1);
}

bool DotItem::isBlocked()
{
    return blocked;
}

void DotItem::moveDotItem(qreal posX,qreal posY)
{
    setPos(posX,posY);
    qreal penWidth = 1;
    dotBoundingRect=QRectF(-10-penWidth/2+posX, -10-penWidth/2+posY, 20+penWidth, 20+penWidth);
}

void DotItem::markDot()
{
    color=Qt::green;
    update();
}

int DotItem::getScoreNumber()
{
    return score;
}

void DotItem::setHistoryScore(QString newHistoryScore)
{
    historyScore=newHistoryScore;
}

QString DotItem::getHistoryScore()
{
    return historyScore;
}

//bool DotItem::polygonIsActive()
//{
//    return active;
//}

//void DotItem::setPolygonActive(bool polygonActive)
//{
//    active = polygonActive;
//}

int DotItem::polygonPaintedLevel()
{
    return paintedLevel;
}

void DotItem::setPolygonPaintedLevel(int polPaintedLevel)
{
    paintedLevel=polPaintedLevel;
}
