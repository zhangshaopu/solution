#ifndef IDEFORM_H
#define IDEFORM_H

#include <QMainWindow>
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
#include <QWidget>

#include "textedit.h"
#include "finddialog.h"
#include "appconfig.h"
namespace Ui {
class IDEForm;
}

class IDEForm : public QWidget
{
    Q_OBJECT

public:
    explicit IDEForm(QWidget *parent = 0);
    ~IDEForm();
    void OpenFile(QString path);//

protected:
    void closeEvent(QCloseEvent* evt);
    void dragEnterEvent(QDragEnterEvent* evt);
    void dropEvent(QDropEvent* evt);
    void wheelEvent(QWheelEvent* evt);



private:
    Ui::IDEForm *ui;
    //QTabWidget* tabWidget;
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

    void initMainWindow();
    void initTabWidget();
    void initStatusBar();
    int CreateNewTab();
    void ConnectTextEdit();//

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
    void on_actionCompile_triggered();

    void on_actionRun_triggered();

    void on_actionComPile_Run_triggered();
};

#endif // IDEFORM_H
