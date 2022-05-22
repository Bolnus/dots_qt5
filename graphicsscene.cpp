#include "graphicsscene.h"

GraphicsScene::GraphicsScene(int scale)
{
    drawing = 0;
    currentStartItem=0;
    chainStartItem=0;
    currentLineItem=0;
    gridScale=scale;
    //firstFirstDot=0;
    //firstSecondDot=0;

}

void GraphicsScene::addDotItem(qreal posX,qreal posY)
{
    DotItem *dotItem = new DotItem(posX,posY);
    addItem(dotItem);
    dotItem->setPos(posX,posY);
}

void GraphicsScene::setEmptyDots(QGraphicsLineItem *lineItem)
{
    QList<QGraphicsItem *> collidingLines = lineItem->collidingItems();
    QLineF mainLine=lineItem->line();
    foreach(QGraphicsItem *item, collidingLines)
    {
        QGraphicsLineItem *graphicsLineItem = qgraphicsitem_cast<QGraphicsLineItem *>(item);
        QLineF collidingLine = graphicsLineItem->line();
        QPointF *intersectionPoint = new QPointF();
        mainLine.intersect(collidingLine,intersectionPoint);
        //QPen dotPen(QBrush(QColor(Qt::red)),1);
        //QRectF ellipseRect(intersectionPoint->x()-2,intersectionPoint->y()-2,4,4);
        //QGraphicsEllipseItem *emptyDot=scene->addEllipse(ellipseRect,dotPen,QBrush(QColor(Qt::red)));
        //emptyDot->setAcceptHoverEvents(1);

        qreal posX = intersectionPoint->x();
        qreal posY = intersectionPoint->y();
        addDotItem(posX,posY);
//        DotItem *dotItem = new DotItem(intersectionPoint->x(),intersectionPoint->y());
//        addItem(dotItem);
//        dotItem->setPos(intersectionPoint->x(),intersectionPoint->y());
    }
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF point = mouseEvent->scenePos();
    if(currentStartItem)
    {
        DotItem *dotItem = getDotItem(point);
        if(dotItem)
        {
            if(dotItem!=currentStartItem&&isNeighbouringDotItem(currentStartItem,dotItem)&&!dotItem->isBlocked())
            {

                currentLineItem->setLine(currentStartItem->pos().x(),currentStartItem->pos().y(),dotItem->scenePos().x(),dotItem->scenePos().y());
                lineChain.append(currentLineItem);
                currentLineItem=0;
                dotsChain.append(dotItem);
                if(dotItem==chainStartItem)
                {
                    currentStartItem=0;
                    //dotItem->setLineChain(lineChain); //polygon!!!


                    bool paintSuccess = paintArea();
                    if(!paintSuccess)
                        resetDot(chainStartItem);
                    while(!lineChain.isEmpty())
                        delete lineChain.takeFirst();
                    chainStartItem=0;
                    dotsChain.clear();
                    lineChain.clear();
                }
                else
                {

                    currentStartItem=dotItem;
                }
            }
        }
    }
    else if(mouseEvent->button()==Qt::LeftButton)
    {
        //
        QGraphicsItem *clickedItem = itemAt(point,QTransform());
        if(clickedItem->data(DATA_IS_DOT).toBool())
        {
            DotItem *dotItem = qgraphicsitem_cast<DotItem *>(clickedItem);
            if(!dotItem->isSet())
            {
                dotItem->setDot();
                emit signal_turnChanged();
            }
            //qDebug("mousePressEvent");
            if(DotItem::dotsCounter == 1)
            {
                //firstFirstDot=1;
                emit signal_firstDotClicked(dotItem,1);
            }
            else if(DotItem::dotsCounter == 2)
            {
                //firstSecondDot=1;
                emit signal_firstDotClicked(dotItem,2);
            }
        }
    }
    else if(mouseEvent->button()==Qt::RightButton&&!chainStartItem)
    {
        //QGraphicsItem *clickedItem = itemAt(point);
        DotItem *dotItem = getDotItem(point);
        if(dotItem)
        {
            //DotItem *dotItem = qgraphicsitem_cast<DotItem *>(clickedItem);
            if(!dotItem->isSet())
            {
                dotItem->setDot();
                emit signal_turnChanged();
                currentStartItem=dotItem;
                chainStartItem=dotItem;
                dotsChain.append(dotItem);
            }

            //qDebug("mousePressEvent");
        }
    }
}

DotItem * GraphicsScene::getDotItem(QPointF point)
{
    QList<QGraphicsItem *> itemsList = items(point,Qt::IntersectsItemBoundingRect,Qt::AscendingOrder);
    QGraphicsItem *clickedItem = 0;//itemAt(point);//Qt::IntersectsItemBoundingRect
    foreach (QGraphicsItem *item, itemsList)
    {
        if(item->data(DATA_IS_DOT).toBool())
        {
            clickedItem=item;
            break;
        }
    }
    if(clickedItem)
    {
        DotItem *dotItem = qgraphicsitem_cast<DotItem *>(clickedItem);
        return dotItem;
    }
    else
        return 0;
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(currentStartItem)
    {
        QColor color = currentStartItem->currentColor();
        QPen linePen(QBrush(color),2);
        QLineF verticalLine1(currentStartItem->pos(),mouseEvent->scenePos());
        if(!currentLineItem)
            currentLineItem = this->addLine(verticalLine1,linePen);
        else
            currentLineItem->setLine(currentStartItem->pos().x(),currentStartItem->pos().y(),mouseEvent->scenePos().x(),mouseEvent->scenePos().y());
    }
    else
        QGraphicsScene::mouseMoveEvent(mouseEvent);
}

bool GraphicsScene::isNeighbouringDotItem(DotItem *dotItemStart,DotItem *dotItemFinish)
{
    if(dotItemStart->currentColor()==dotItemFinish->currentColor())
    {
        if(QLineF(dotItemStart->pos(),dotItemFinish->pos()).length()<((qSqrt(qPow(gridScale,2)+qPow(gridScale,2)))+1))
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

void GraphicsScene::slotClearGameProgress()
{
    foreach (QGraphicsItem *item, items())
    {
        if(item->data(DATA_IS_DOT).toBool())
        {
            DotItem *dotItem = qgraphicsitem_cast<DotItem *>(item);
            if(dotItem->isSet())
            {
                resetDot(dotItem);
            }
        }
    }
}

void GraphicsScene::slotUndo()
{
    currentStartItem=0;
    chainStartItem=0;
    if(currentLineItem)
    {
        delete currentLineItem;
        currentLineItem=0;
    }

    foreach (QGraphicsItem *item, items())
    {
        if(item->data(DATA_IS_DOT).toBool())
        {
            DotItem *dotItem = qgraphicsitem_cast<DotItem *>(item);
            if(dotItem->getSetNumber()==DotItem::dotsCounter&&dotItem->isSet())
            {
                resetDot(dotItem);
                break;
            }
        }
    }
    while(!lineChain.isEmpty())
        delete lineChain.takeFirst();
}

void GraphicsScene::resetDot(DotItem *dotItem)
{
    //QColor dotItemColor = dotItem->currentColor();
    QGraphicsPolygonItem *polygonItem = dotItem->getLineCHain();
    if(polygonItem)
    {
        QList<QGraphicsItem *> collidingLines = polygonItem->collidingItems(Qt::IntersectsItemBoundingRect); //
        foreach (QGraphicsItem *item, collidingLines)
        {
            if(item->data(DATA_IS_DOT).toBool())
            {
                DotItem *insideDot = qgraphicsitem_cast<DotItem *>(item);
                if(polygonItem->contains(QPointF(insideDot->scenePos().x()-polygonItem->scenePos().x(),insideDot->scenePos().y()-polygonItem->scenePos().y())))
                {
                    insideDot->setBlocked(0);
                    //inside polygons bug
                    QGraphicsPolygonItem *insidePolygon = insideDot->getLineCHain();
                    if(insidePolygon)
                    {
                        insideDot->setPolygonPaintedLevel(insideDot->polygonPaintedLevel()-1);
                    }
                }
            }
        }
        emit signal_regainHistoryScore(dotItem);
    }
    dotItem->resetDot(); //int scoreAmplification =
    //if(scoreAmplification)
    //    emit signal_regainHistoryScore(dotItem);
    emit signal_turnChanged();
}

bool GraphicsScene::paintArea()
{

    emit signal_saveHistoryScore(chainStartItem);
    QPolygonF polygon;
    QList<QPointF> pointsList;
    foreach (DotItem *dotItem, dotsChain)
    {
        pointsList.append(QPointF(dotItem->scenePos().x()-chainStartItem->scenePos().x(),dotItem->scenePos().y()-chainStartItem->scenePos().y()));
        //polygon << dotItem->scenePos();
    }
    for(int i=0;i<pointsList.count();i++)
    {
        polygon << pointsList[i];
    }
    QColor borderColor = chainStartItem->currentColor();
    if(borderColor.isValid())
    {
        QColor polygonColor = borderColor;
        polygonColor.setAlpha(75);
        if(!polygon.isEmpty()&&polygon.isClosed())
        {
            QGraphicsPolygonItem *polygonItem = addPolygon(polygon,QPen(QBrush(borderColor),2),QBrush(polygonColor));
            polygonItem->setPos(chainStartItem->scenePos());
            QRectF polygonRect = polygonItem->boundingRect();
            polygonRect.moveTo(chainStartItem->scenePos());
            //addRect(polygonRect,QPen(QColor(Qt::green)));
            qDebug() << "\n";
            int paintedItemsNumber = 0;
//            int blockedEnemyItemsNumber = 0;
//            int blockedAllyItemsNumber = 0;
            QList<QGraphicsItem *> collidingLines = polygonItem->collidingItems(Qt::IntersectsItemBoundingRect); //

            foreach (QGraphicsItem *item, collidingLines)
            {
                if(item->data(DATA_IS_DOT).toBool())
                {
                    DotItem *insideDot = qgraphicsitem_cast<DotItem *>(item);
                    if(polygonItem->contains(QPointF(insideDot->scenePos().x()-chainStartItem->scenePos().x(),insideDot->scenePos().y()-chainStartItem->scenePos().y())))
                    {
                        if(insideDot->currentColor()!=chainStartItem->currentColor()&&insideDot->currentColor()!=Qt::black) //&&!insideDot->isBlocked()
                        {
                            paintedItemsNumber++;
//                            QGraphicsPolygonItem *insidePolygon = insideDot->getLineCHain();
//                            if(insidePolygon)
//                            {
//                                int scoreChange = -insideDot->getScoreNumber();
//                                emit signal_changeScore(scoreChange,insideDot->currentColor());
//                            }
                            //insideDot->markDot();
                            insideDot->setBlocked(1);
                        }
                        else if(insideDot->currentColor()==Qt::black)
                            insideDot->setBlocked(1);
                    }

                }
            }
            chainStartItem->setLineChain(polygonItem,paintedItemsNumber);
            if(!paintedItemsNumber)
            {
                return 0;
            }
            else
            {

                foreach (QGraphicsItem *item, collidingLines)
                {
                    if(item->data(DATA_IS_DOT).toBool())
                    {
                        DotItem *insideDot = qgraphicsitem_cast<DotItem *>(item);
                        if(polygonItem->contains(QPointF(insideDot->scenePos().x()-chainStartItem->scenePos().x(),insideDot->scenePos().y()-chainStartItem->scenePos().y())))
                        {
                            QGraphicsPolygonItem *insidePolygon = insideDot->getLineCHain();
                            if(insidePolygon)
                            {
                                bool polygonIsInside = 1;
                                QPointF startPoint = chainStartItem->scenePos();
                                QPointF insideDotPoint = insideDot->scenePos();
                                QPointF insidePolygonStartPointInParentPolygonLocals = QPointF(insideDot->scenePos().x()-chainStartItem->scenePos().x(),insideDot->scenePos().y()-chainStartItem->scenePos().y());
                                foreach (QPointF polPoint, insidePolygon->polygon())
                                {
                                    QPointF insidePolygonPointInParentPolygonLocals = QPointF(polPoint.x()+insidePolygonStartPointInParentPolygonLocals.x(),polPoint.y()+insidePolygonStartPointInParentPolygonLocals.y());
                                    if(!polygonItem->contains(insidePolygonPointInParentPolygonLocals))
                                    {
                                        polygonIsInside = 0;
                                        break;
                                    }
                                }
                                if(insideDot->polygonPaintedLevel()==1&&polygonIsInside) // chainStartItem!=insideDot !dotsChain.contains(insideDot) insidePolygon->isObscuredBy(polygonItem)
                                {
                                    int blockedItemsNumber = insideDot->getScoreNumber();
                                    emit signal_changeScore(-blockedItemsNumber,insideDot->currentColor());
                                    //if(insideDot->currentColor()!=chainStartItem->currentColor())
                                    //    emit signal_changeScore(-blockedItemsNumber,insideDot->currentColor()); //внутренний полигон цвета противника
                                    //else
                                    //    emit signal_changeScore(blockedItemsNumber,insideDot->currentColor()); //внутренний полигон того же цвета
                                    insideDot->setPolygonPaintedLevel(insideDot->polygonPaintedLevel()+1);
                                }
                                else if(!dotsChain.contains(insideDot))
                                    insideDot->setPolygonPaintedLevel(insideDot->polygonPaintedLevel()+1);
                            }

                        }
                    }
                }
                chainStartItem->setPolygonPaintedLevel(1);
                emit signal_changeScore(paintedItemsNumber,polygonColor);
            }
        }
        else
            return 0;
    }
    else
        return 0;

    return 1;
}
