#include "linklistform.h"
#include "ui_linklistform.h"
#include "MyArrowItem.h"

#include<QDebug>
#include <math.h>

void sleep(unsigned int msec);

const QBrush LinkListForm::normalBursh=QBrush(Qt::GlobalColor::darkGray);
const QBrush LinkListForm::visitedBrush=QBrush(Qt::GlobalColor::yellow);
const QBrush LinkListForm::markBrush=QBrush(Qt::GlobalColor::green);

const QFont LinkListForm::headLabelFont=QFont("Consolas");
const QFont LinkListForm::dataFont=QFont("Consolas",8);

const QIntValidator LinkListForm::dataValidator(-999999999,999999999);

LinkListForm::LinkListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkListForm)
{
    //初始化数据
    ui->setupUi(this);
    initTextBrowser();
    initUI();

    countNode=0;
    head=NULL;
    headLabel=NULL;
    headArrow=NULL;
    scene=NULL;
    sleepTime=MAX_SLEEP_TIME>>1;
    ui->horizontalSlider->setValue(MAX_SLIDER>>1);
    srand(time(NULL));

    this->setWindowTitle("单链表");
}

//析构函数
LinkListForm::~LinkListForm()
{
    destroySelf();
    delete ui;
}

//初始设置文本显示区
void LinkListForm::initTextBrowser(){
    //加载html文件
    QFile file(":/html/html/LinkList.html");
    file.open(QIODevice::ReadOnly);
    QString htmlString=file.readAll();
    ui->textBrowser->setHtml(htmlString);
    ui->textBrowser->setOpenLinks(true);
    ui->textBrowser->setOpenExternalLinks(true);

    file.close();
}

//初始UI控件
void LinkListForm::initUI()
{
    //set background image
    QPixmap background(":/ico/resource/background.png");
    QPalette palette;
    palette.setBrush(QPalette::Background,background);
    this->setPalette(palette);

    //未创建时，除创建按钮外，其余按键一律无效
    ui->pushButtonClear->setEnabled(false);
    ui->pushButtonInsert->setEnabled(false);
    ui->pushButtonRandomInsert5->setEnabled(false);
    ui->comboBoxInsert->setEnabled(false);
    ui->lineEditInsert->setEnabled(false);

    ui->pushButtonDelete->setEnabled(false);
    ui->comboBoxDelete->setEnabled(false);
    ui->lineEditDelete->setEnabled(false);

    ui->pushButtonLocate->setEnabled(false);
    ui->comboBoxLocate->setEnabled(false);
    ui->lineEditLocate->setEnabled(false);

    ui->lineEditState->setEnabled(false);

    ui->lineEditInsert->setFont(dataFont);
    ui->lineEditDelete->setFont(dataFont);
    ui->lineEditLocate->setFont(dataFont);
    ui->lineEditState->setFont(dataFont);
    ui->lineEditInsert->setPlaceholderText("插入值：Int");
    ui->lineEditLocate->setPlaceholderText("查找值：Int");
    ui->lineEditInsert->setValidator(&LinkListForm::dataValidator);
    ui->lineEditLocate->setValidator(&LinkListForm::dataValidator);
    ui->lineEditState->setText("请选择操作");

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(MAX_SLIDER);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBelow);
}


//初始化视图框架
void LinkListForm::initSceneView()
{
    //为 view 配置相应 scene
    scene=new QGraphicsScene;
    scene->setSceneRect(0,0,SCENE_MAX_W,SCENE_MAX_H);
    ui->graphicsView->setScene(scene);

    //添加链表符号及指针箭头
    headLabel=scene->addText("L",headLabelFont);
    headLabel->setPos((VALUE_RECT_W-RECT_H)>>1,SPACING);
    sleep(sleepTime);
    headArrow=new MyArrowItem(ARROW_LEN);
    scene->addItem(headArrow);
    headArrow->setPos(ARROW_H_OFFSET,ARROW_V_OFFSET);
}

//操作之后调整右侧输入和显示控件
void LinkListForm::adjustController()
{
    //当经过插入或删除操作后，节点个数可能会改变，需考虑一些操作的合法性
    ui->pushButtonDelete->setEnabled(countNode);
    ui->comboBoxDelete->setEnabled(countNode);

    if(ui->comboBoxDelete->count()!=countNode)
    {
        QStringList qStringList;
        for(int i=1;i<=countNode;++i)
            qStringList.push_back(QString::number(i));

        ui->comboBoxInsert->clear();
        ui->comboBoxInsert->addItems(qStringList);

        ui->comboBoxDelete->clear();
        ui->comboBoxDelete->addItems(qStringList);
    }
    if(ui->comboBoxInsert->count()!=countNode+1)
        ui->comboBoxInsert->addItem(QString::number(countNode+1));

    ui->lineEditDelete->setText(" ");
    ui->comboBoxLocate->clear();
}

//计算节点的Scene坐标
QPoint LinkListForm::getLNodePos(int nodeNumber)
{
    const static int rowN=SCENE_MAX_W/NODE_W;
    return QPoint((nodeNumber+1)%rowN*NODE_W,(nodeNumber+1)/rowN*NODE_H);
}

//添加节点的GraphicsItem
void LinkListForm::addLNodeGraphicsItem(lnode *pl, QPoint coord)
{
    int x=coord.x(), y=coord.y();
    qDebug()<<"x="<<x<<" y="<<y<<"\n";
    pl->valueRect   =scene->addRect(x,y+SPACING,VALUE_RECT_W,RECT_H,QPen(),LinkListForm::markBrush);
    pl->pointerRect =scene->addRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pl->valueText   =scene->addText(pl->data,LinkListForm::dataFont);
    pl->valueText->setPos(x,y+SPACING+5);
    if(pl->next==NULL)
    {
        pl->pointerText=scene->addText(" ^",LinkListForm::dataFont);
        pl->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);
    }
}

//调整节点的箭头
void LinkListForm::adjustLNodeArrow(lnode *pLNode, int nodeNumber)
{
    for(auto &a:pLNode->arrowVector)
        scene->removeItem(a);
    if(pLNode->next==NULL)
        return ;

    QPoint myCoord=getLNodePos(nodeNumber);
    QPoint nextCoord=getLNodePos(nodeNumber+1);

    MyArrowItem *pArrow;
    if(myCoord.y()==nextCoord.y())
    {
        //节点不是一行最后节点
        pArrow=new MyArrowItem(ARROW_LEN);
        scene->addItem(pArrow);
        pArrow->setPos(myCoord.x()+ARROW_H_OFFSET,myCoord.y()+ARROW_V_OFFSET);
        pLNode->arrowVector.push_back(pArrow);
    }
    else
    {
        //节点是一行中最后节点
        QPoint point1(myCoord.x()+ARROW_H_OFFSET,myCoord.y()+ARROW_V_OFFSET);
        QPoint point2(point1.x(),point1.y()+NODE_H/2);
        QPoint point3(VALUE_RECT_W>>1,point2.y());

        pArrow=new MyArrowItem(NODE_H>>1,2,0);
        scene->addItem(pArrow);
        pArrow->setPos(point1);
        pLNode->arrowVector.push_back(pArrow);

        pArrow=new MyArrowItem(point2.x()-point3.x(),3,0);
        scene->addItem(pArrow);
        pArrow->setPos(point2);
        pLNode->arrowVector.push_back(pArrow);

        pArrow=new MyArrowItem(SPACING,2);
        scene->addItem(pArrow);
        pArrow->setPos(point3);
        pLNode->arrowVector.push_back(pArrow);
    }
}

//调整节点Scene坐标
void LinkListForm::adjustLNodePos(lnode *pLNode, QPoint coord)
{
    int x=coord.x(), y=coord.y();
    pLNode->valueRect->setRect(x,y+SPACING,VALUE_RECT_W,RECT_H);
    pLNode->pointerRect->setRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pLNode->valueText->setPos(x,y+SPACING+5);
    if(pLNode->pointerText)
        pLNode->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);
}

//设置链表所有节点显示模式
void LinkListForm::setLinkListNormalBrush()
{
    for(lnode *pLNode=head;pLNode;pLNode=pLNode->next)
        pLNode->setNodeStatus(LinkListForm::normalBursh);
}

//创建链表初始化
void LinkListForm::initLinkList()
{
    initSceneView();
sleep(sleepTime);
    head=new lnode("头结点", NULL);
    addLNodeGraphicsItem(head, getLNodePos(0));
}

//插入节点到链表
void LinkListForm::insertLNode(int pos, QString elem)
{
    lnode *pInsertNode=NULL;
    lnode *pLNode=head;

    head->setNodeStatus(LinkListForm::visitedBrush);

    //找到前驱节点指针
    for(int i=0;i<pos-1;++i)
    {
        sleep(sleepTime);

        pLNode=pLNode->next;
        pLNode->setNodeStatus(LinkListForm::visitedBrush);
    }
sleep(sleepTime);
    if(pLNode->next==NULL){
        scene->removeItem(pLNode->pointerText);
        pLNode->pointerText=NULL;
    }

    //新节点插入到链表中
    pInsertNode=new lnode(elem,pLNode->next);
    pLNode->next=pInsertNode;
    ++countNode;

    //添加图形Item
    addLNodeGraphicsItem(pInsertNode,getLNodePos(pos));

    for(--pos;pos<=countNode;++pos)
    {
        sleep(sleepTime);

        adjustLNodePos(pLNode,getLNodePos(pos));    //调整节点坐标位置
        adjustLNodeArrow(pLNode,pos);               //调整每个节点箭头
        pLNode=pLNode->next;
    }
}

//删除链表节点
void LinkListForm::deleteLNode(int pos, QString &elem)
{
    lnode *pDeleteNode=NULL;
    lnode *pLNode=head;

    head->setNodeStatus(LinkListForm::visitedBrush);

    //找到前驱节点指针
    for(int i=0;i<pos-1;++i)
    {
        sleep(sleepTime);

        pLNode=pLNode->next;
        pLNode->setNodeStatus(LinkListForm::visitedBrush);
    }
    sleep(sleepTime);
    pDeleteNode=pLNode->next;
    pLNode->next=pDeleteNode->next;
    elem=pDeleteNode->data;

    //删除节点，移除图形Item
    pDeleteNode->removeAll(scene);
    delete pDeleteNode;
    --countNode;

    if(pLNode->next==NULL){
        QPoint coord=getLNodePos(pos-1);
        pLNode->pointerText=scene->addText(" ^",LinkListForm::dataFont);
        pLNode->pointerText->setPos(coord.x()+VALUE_RECT_W, coord.y()+SPACING+5);
    }

    for(--pos;pos<=countNode;++pos)
    {
        sleep(sleepTime);

        adjustLNodePos(pLNode,getLNodePos(pos));    //调整节点坐标位置
        adjustLNodeArrow(pLNode,pos);               //调整节点箭头
        pLNode=pLNode->next;
    }
}

//查找链表节点
bool LinkListForm::locateLNode(int &pos, QString elem)
{
    lnode *pLNode=head;

    head->setNodeStatus(LinkListForm::visitedBrush);

    for(pos=1;pLNode&&pLNode->next&&pLNode->next->data!=elem;++pos)
    {
        sleep(sleepTime);

        pLNode=pLNode->next;
        pLNode->setNodeStatus(LinkListForm::visitedBrush);
    }
sleep(sleepTime);
    //找到相应节点
    if(pLNode&&pLNode->next){
        pLNode->next->setNodeStatus(LinkListForm::markBrush);
        return true;
    }
    return false;
}

//释放申请的内存空间
void LinkListForm::destroySelf()
{
    if(scene==NULL)
        return ;

    lnode *pLNode=head, *qLNode;
    for(;pLNode;pLNode=qLNode)
    {
        sleep(sleepTime);
        qLNode=pLNode->next;
        pLNode->removeAll(scene);       //移除每个节点的图形Item
        delete pLNode;      //释放内存
    }
sleep(sleepTime);
    scene->removeItem(headLabel);   //移除链表符号Item
    scene->removeItem(headArrow);   //移除链表符号后的箭头Item
    delete headLabel;       //释放内存
    delete headArrow;       //释放内存
    scene=NULL;

    countNode=0;
}

//槽函数：点击创建
void LinkListForm::on_pushButtonInit_clicked()
{
    //若已经建立，需要清除重建
    destroySelf();

    initLinkList();

    ui->pushButtonClear->setEnabled(true);
    ui->pushButtonInsert->setEnabled(true);
    ui->pushButtonRandomInsert5->setEnabled(true);
    ui->comboBoxInsert->setEnabled(true);
    ui->lineEditInsert->setEnabled(true);
    ui->pushButtonLocate->setEnabled(true);
    ui->lineEditLocate->setEnabled(true);

    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Create Success!");
}

//槽函数：点击清空
void LinkListForm::on_pushButtonClear_clicked()
{
    destroySelf();
    initUI();
}

//槽函数：点击插入
void LinkListForm::on_pushButtonInsert_clicked()
{
    setLinkListNormalBrush();
sleep(sleepTime);
    QString edit=ui->lineEditInsert->text();

    //若输入无效或未输入
    if(edit.isEmpty())
    {
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("Please Input!");
        return ;
    }

    insertLNode(ui->comboBoxInsert->currentText().toInt(),edit);

    //调整右侧控件状态及值
    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Insert Success!");
}

//随机插入五个节点到链表末尾
void LinkListForm::on_pushButtonRandomInsert5_clicked()
{
    for(int i=0;i<5;++i)
    {
        setLinkListNormalBrush();
sleep(sleepTime);
        insertLNode(countNode+1,QString::number(rand()%999999999));
        adjustController();
        ui->lineEditState->setPalette(Qt::GlobalColor::green);
        ui->lineEditState->setText("Insert Success!");
    }
}

//槽函数：点击删除
void LinkListForm::on_pushButtonDelete_clicked()
{
    setLinkListNormalBrush();
sleep(sleepTime);
    QString deleteData;
    deleteLNode(ui->comboBoxDelete->currentText().toInt(),deleteData);

    //调整右侧控件状态及值
    adjustController();
    ui->lineEditDelete->setText(deleteData);
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Delete Success!");
}

//槽函数：点击查找
void LinkListForm::on_pushButtonLocate_clicked()
{
    setLinkListNormalBrush();
sleep(sleepTime);
    QString edit=ui->lineEditLocate->text();

    //若输入无效或未输入
    if(edit.isEmpty())
    {
        adjustController();
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("Please Input!");
        return;
    }

    //根据查找结果给出结果输出
    int pos;
    if(locateLNode(pos,edit))
    {
        ui->comboBoxLocate->addItem(QString::number(pos));
        ui->comboBoxLocate->setCurrentText(QString::number(pos));
        ui->lineEditState->setPalette(Qt::GlobalColor::green);
        ui->lineEditState->setText("Locate Success!");
    }
    else
    {
        adjustController();
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("Locate Fail!");
    }
}

//调整演示的速度快慢
void LinkListForm::on_horizontalSlider_valueChanged(int value)
{
    sleepTime=MAX_SLEEP_TIME/(value+1);
}

