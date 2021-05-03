#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <linklistform.h>
#include <dlinklist.h>

#include <QMainWindow>
#include <QTextCodec>
#include <QFileDialog>
#include <QCloseEvent>
#include <QFont>
#include <QMessageBox>
#include <QStatusBar>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMap>
#include <QTranslator>
#include <QFileSystemWatcher>
#include <QMimeData>
#include <QDateTime>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QVector>
#include "textedit.h"
#include "finddialog.h"
//#include "widget.h"
#include "visualization.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void OpenFile(QString path);//

protected:
    void closeEvent(QCloseEvent* evt);
    void dragEnterEvent(QDragEnterEvent* evt);
    void dropEvent(QDropEvent* evt);
    void wheelEvent(QWheelEvent* evt);

protected slots:
    //file
    void slotOpen();
    void slotSave();
    void slotSaveAs();
    void slotRemove();
    void slotRename();
    void slotPrint();

    //edit
    void slotFind();
    void slotGoto();

    //view
//--    void slotShowToolBar(bool a);
//    void slotShowStatusBar(bool a);

    //settings
    //--void slotAutoWrap(bool a);
    void slotFont();
    void slotChangeFont(QFont f);
    void slotChangeLanguage();

    //help
    void slotUser();

    //other
    void slotTabChanged(int index);
    void slotTabClosed(int index);
    void slotTextChanged();
    void slotCursorChanged(int line, int index);
    void slotFileWatcher(const QString& path);
    void slotLineChanged();

    void slotInitCPP();
    void slotInitCS();
    void slotInitJava();
    void slotInitPython();
private slots:

    void on_actionLinkedList_triggered();

    void on_actionDoubleLinkedList_triggered();


    void on_actionCompile_triggered();

    void on_actionRun_triggered();

    void on_actionComPile_Run_triggered();

    void on_actionSort_triggered();

    void Pre_Button_Clicked();

    void Next_Button_Clicked();
private:
    Ui::MainWindow *ui;

    LinkListForm linkedlist;
    DLinkList dlinklist;
    Widget sort;

private:
    QTabWidget* tabWidget;
    QTabWidget* outputWidget;
    TextEdit* mainEditor;
    FindDialog findDialog;
    QTranslator* trans;
    static char num[64];
    QLabel* label;
    QLabel* labelLength;
    QFileSystemWatcher* watcher;
    QFont font;
    QPlainTextEdit ComPileText , OutPutText;
    QSplitter *splitter;
    QTextEdit *timuedit;
    QPushButton *pre;
    QPushButton *next;


    void initMainWindow();
    void initTabWidget();
    void initStatusBar();
    int CreateNewTab();
    void ConnectTextEdit();//

    void InitTiKu();
    //辅助功能函数封装
    void deleteTab(int index);//
    QString SaveFile(QString path="");//
    QString showFileDialog(const QString& title, QFileDialog::AcceptMode mode);
    void showErrorMessage(const QString& content);
    int showQuestionDialog(const QString& title,const QString& content);
    void setTitle();////

    //用于每次切换tab时检查保存，重命名，删除文件，undo，redo，cut，copy的状态
    void checkStatus();
    QString getPath();
    void readConfig();
    void writeConfig(QStringList openList);
    void setNum0(int index);//
    void showLength();
    QString getLineString() const;

    //题目名称
    QString root = "D:/lunwen/solution/timu/"; // 题库根路径
    QVector<QString> filenamelist;
    QString defaltFilePath;
    int curfileidx;
    int maxnumoftimu;
    bool read_timu(); // 读取defaltFilePath路径下的题目
};
#endif // MAINWINDOW_H
