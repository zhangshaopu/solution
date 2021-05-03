#ifndef LNODE_H
#define LNODE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QBrush>

#include "MyArrowItem.h"

class LinkList;

class lnode
{
public:
    lnode();
    lnode(QString dt, lnode *nt);       //构造函数
    ~lnode();                           //析构函数
    void setValueRect(QGraphicsRectItem* vRect);
    void setPointerRect(QGraphicsRectItem* pRect);
    void setTextRect(QGraphicsTextItem* vText);
    void setArrowVector(std::vector<MyArrowItem*> aVector);
    void setNodeStatus(QBrush brush);
    void removeAll(QGraphicsScene *scene);


public:
    QString data;       //节点数据域
    lnode * next;       //节点指针域

    //节点图形项
    QGraphicsRectItem * valueRect, *pointerRect;
    QGraphicsTextItem * valueText, *pointerText;
    std::vector<MyArrowItem*> arrowVector;

public:
    friend class LinkList;
    //friend class CLinkList;
};
#endif // LNODE_H
