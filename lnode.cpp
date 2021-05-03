#include "lnode.h"

lnode::lnode(QString dt, lnode *nt)
{
    data=dt;
    next=nt;
    pointerRect=valueRect=NULL;
    valueText=pointerText=NULL;
}

lnode::~lnode()
{

}

void lnode::setValueRect(QGraphicsRectItem* vRect)
{
    valueRect=vRect;
}

void lnode::setPointerRect(QGraphicsRectItem* pRect)
{
    pointerRect=pRect;
}

void lnode::setTextRect(QGraphicsTextItem* vText)
{
    valueText=vText;
}

void lnode::setArrowVector(std::vector<MyArrowItem*> aVector)
{
    arrowVector=aVector;
}

void lnode::setNodeStatus(QBrush brush)
{
    valueRect->setBrush(brush);
}

void lnode::removeAll(QGraphicsScene *scene)
{
    if(valueRect)
    {
        scene->removeItem(valueRect);
        delete valueRect;
        valueRect=NULL;
    }

    if(pointerRect)
    {
        scene->removeItem(pointerRect);
        delete pointerRect;
        pointerRect=NULL;
    }

    if(valueText)
    {
        scene->removeItem(valueText);
        delete valueText;
        valueText=NULL;
    }

    if(pointerText)
    {
        scene->removeItem(pointerText);
        delete pointerText;
        pointerText=NULL;
    }

    for(auto &a:arrowVector)
        scene->removeItem(a);
    arrowVector.clear();
}
