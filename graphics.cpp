#include "graphics.h"

//todo: отмена рисования и стирание всех линий по кнопке Esc

Graphics::Graphics(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    setWindowTitle("Dots");
    //graphicsView->resi

    gridScale = 20;
    scene = new GraphicsScene(gridScale);
    //graphicsView->setGeometry(0,0,500,600);
    //verticalLayout->
    resizeCount=0;


    graphicsView->setScene(scene);
    graphicsView_player1->setScene(scene);
    graphicsView_player2->setScene(scene);
    graphicsView->setMouseTracking(1);
    graphicsView->viewport()->setMouseTracking(1);
    graphicsView->installEventFilter(this);
    //scene->ins
    label_player1->hide();
    label_player2->hide();
    graphicsView_player1->hide();
    graphicsView_player2->hide();
    progressBar->setRange(0,300);
    progressBar->setValue(0);
    QTimer::singleShot(50,this,SLOT(slotProgressBar()));

    keyEsc = new QShortcut(this);
    keyEsc->setKey(Qt::Key_Escape);

    connect(keyEsc, SIGNAL(activated()), scene, SLOT(slotUndo()));
    connect(pushButton_clear,SIGNAL(clicked(bool)),this,SLOT(slotClearGameProgress()));
    connect(pushButton_undo,SIGNAL(clicked(bool)),scene,SLOT(slotUndo()));
    connect(scene,SIGNAL(signal_firstDotClicked(DotItem *,int)),this,SLOT(slotSetCenterOn(DotItem *,int)));
    connect(scene,SIGNAL(signal_changeScore(int,QColor)),this,SLOT(slotchangeScore(int,QColor)));
    connect(scene,SIGNAL(signal_turnChanged()),this,SLOT(slotShowTurnChange()));
    connect(scene,SIGNAL(signal_saveHistoryScore(DotItem *)),this,SLOT(slotSaveHistoryScore(DotItem*)));
    connect(scene,SIGNAL(signal_regainHistoryScore(DotItem *)),this,SLOT(slotRegainHistoryScore(DotItem*)));
}

void Graphics::slotProgressBar()
{
    if(progressBar->value()<50)
    {
        progressBar->setValue(progressBar->value()+1);
        QTimer::singleShot(50,this,SLOT(slotProgressBar()));
    }
    else if(progressBar->value()==50)
    {
        initScene();
        progressBar->setValue(progressBar->value()+1);
        QTimer::singleShot(100,this,SLOT(slotProgressBar()));
    }
    else if(progressBar->value()<300)
    {
        progressBar->setValue(progressBar->value()+1);
        QTimer::singleShot(5,this,SLOT(slotProgressBar()));
    }
    else
    {
        label_player1->show();
        label_player2->show();
        graphicsView_player1->show();
        graphicsView_player2->show();
        progressBar->hide();
    }
    //else if(progressBar->value()<100)

}

void Graphics::slotSaveHistoryScore(DotItem *dotItem)
{
    dotItem->setHistoryScore(label_player1->text()+'-'+label_player2->text());
}

void Graphics::slotRegainHistoryScore(DotItem *dotItem)
{
    QStringList strList = dotItem->getHistoryScore().split('-');
    if(strList.count()==2)
    {
        label_player1->setText(strList.first());
        label_player2->setText(strList.last());
    }
}

void Graphics::slotchangeScore(int scoreChange,QColor playerColor)
{
    if(label_player1->whatsThis()==playerColor.name())
        label_player1->setText(QString::number(label_player1->text().toInt()+scoreChange));
    else if(label_player2->whatsThis()==playerColor.name())
        label_player2->setText(QString::number(label_player2->text().toInt()+scoreChange));
}

void Graphics::slotShowTurnChange()
{
    label_turn->setText("Turn: "+QString::number(DotItem::dotsCounter));
    QPalette palet = label_turn->palette();
    if(DotItem::dotsCounter%2)
        palet.setColor(label_turn->foregroundRole(),Qt::red);  //(QColor()));
    else
        palet.setColor(label_turn->foregroundRole(),Qt::blue);
    label_turn->setPalette(palet);
}

void Graphics::slotSetCenterOn(DotItem *dotItem,int dotNumber)
{
    if(dotNumber==1)
    {
        graphicsView_player1->centerOn(dotItem->scenePos());
        label_player1->setWhatsThis(dotItem->currentColor().name());
    }
    else
    {
        graphicsView_player2->centerOn(dotItem->scenePos());
        label_player2->setWhatsThis(dotItem->currentColor().name());
    }
}

bool Graphics::eventFilter(QObject *object, QEvent *event)
{
//    if(event->type() == QEvent::Resize&&resizeCount<2)
//    {
//        if(resizeCount)
//        {
//            initScene();

//            graphicsView->removeEventFilter(this);
//        }
//        resizeCount++;

//        return true;
//    }

    return QObject::eventFilter(object,event);
}

void Graphics::setEmptyDots(QGraphicsLineItem *lineItem)
{
    QList<QGraphicsItem *> collidingLines = lineItem->collidingItems();
    QLineF mainLine=lineItem->line();
    foreach (QGraphicsItem *item, collidingLines)
    {
        QGraphicsLineItem *graphicsLineItem = qgraphicsitem_cast<QGraphicsLineItem *>(item);
        QLineF collidingLine = graphicsLineItem->line();
        QPointF *intersectionPoint = new QPointF();
        mainLine.intersect(collidingLine,intersectionPoint);
        //QPen dotPen(QBrush(QColor(Qt::red)),1);
        //QRectF ellipseRect(intersectionPoint->x()-2,intersectionPoint->y()-2,4,4);
        //QGraphicsEllipseItem *emptyDot=scene->addEllipse(ellipseRect,dotPen,QBrush(QColor(Qt::red)));
        //emptyDot->setAcceptHoverEvents(1);

        DotItem *dotItem = new DotItem(intersectionPoint->x(),intersectionPoint->y());
        scene->addItem(dotItem);
        //QPointF point = dotItem->pos();
        dotItem->setPos(intersectionPoint->x(),intersectionPoint->y());
        //dotItem->setCoordinates(intersectionPoint->x()/gridScale,intersectionPoint->y()/gridScale);
    }
}

void Graphics::initScene()
{
    QSize viewSize = graphicsView->geometry().size();
    //qreal sceneHypotenuse = qSqrt(qPow(viewSize.width(),2)+qPow(viewSize.height(),2));
    qreal maxSceneHeight = viewSize.height();
    qreal maxSceneWidth = viewSize.width();
    if(maxSceneHeight>20)
        maxSceneHeight=maxSceneHeight-10;
    if(maxSceneWidth>20)
        maxSceneWidth=maxSceneWidth-10;
    QPen linePen(QBrush(QColor(Qt::gray)),1);


    int iterations = (maxSceneWidth/2)/gridScale;
    for(int i=0;i<iterations;i++)
    {
        QLineF verticalLine1(i*gridScale,-maxSceneHeight/2,i*gridScale,maxSceneHeight/2);
        QGraphicsLineItem *verticalItem1 = scene->addLine(verticalLine1,linePen);
        verticalItem1->setData(DATA_IS_DOT,0);
        if(i!=0)
        {
            QLineF verticalLine2(-i*gridScale,-maxSceneHeight/2,-i*gridScale,maxSceneHeight/2);
            QGraphicsLineItem *verticalItem2 = scene->addLine(verticalLine2,linePen);
            verticalItem2->setData(DATA_IS_DOT,0);
        }
        if(progressBar->isVisible())
            progressBar->setValue(progressBar->value()+1);
    }

    iterations = (maxSceneHeight/2)/gridScale;
    for(int i=0;i<iterations;i++)
    {
        QLineF horizontalLine1(-maxSceneWidth/2,i*gridScale,maxSceneWidth/2,i*gridScale);
        QGraphicsLineItem *hLineItem1 = scene->addLine(horizontalLine1,linePen);
        scene->setEmptyDots(hLineItem1);
        hLineItem1->setData(DATA_IS_DOT,0);
        if(i!=0)
        {
            QLineF horizontalLine2(-maxSceneWidth/2,-i*gridScale,maxSceneWidth/2,-i*gridScale);
            QGraphicsLineItem *hLineItem2 = scene->addLine(horizontalLine2,linePen);
            scene->setEmptyDots(hLineItem2);
            hLineItem2->setData(DATA_IS_DOT,0);
        }
        if(progressBar->isVisible())
            progressBar->setValue(progressBar->value()+1);
    }
    scene->setSceneRect(scene->itemsBoundingRect());
}

void Graphics::slotClearGameProgress()
{
    scene->clear();
    initScene();
    label_player1->setText("0");
    label_player2->setText("0");
    //label_turn->setText("Turn:");
    DotItem::dotsCounter=0;
    slotShowTurnChange();
}

Graphics::~Graphics()
{

}
