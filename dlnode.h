#ifndef DLNODE_H
#define DLNODE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QBrush>

#include "MyArrowItem.h"

class DLNode
{
public:
    DLNode(QString dt, DLNode *nt);         //构造函数
    ~DLNode();                              //析构函数

    void setNodeStatus(QBrush brush);
    void removeAll(QGraphicsScene *scene);

protected:
    QString data;               //节点数据域
    DLNode * next, *prior;      //节点指针域

    //节点图形项
    QGraphicsRectItem * valueRect, *pointerRect, *prePointerRect;
    QGraphicsTextItem * valueText, *pointerText, *prePointerText;
    std::vector<MyArrowItem*> arrowVector, preArrowVector;

public:
    friend class DLinkList;
};

#endif // DLNODE_H
