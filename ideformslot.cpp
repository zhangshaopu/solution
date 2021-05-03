#include "ideform.h"
#include "ui_ideform.h"

#include <QDebug>
#include <QDesktopServices>
#include <QFontDialog>
#include <QInputDialog>
#include <QPrintDialog>

#include <Qsci/qsciprinter.h>

char IDEForm::num[64]={0};

void IDEForm::slotOpen()
{
    QString path=showFileDialog(tr("Open"),QFileDialog::AcceptOpen);
    OpenFile(path);
}

void IDEForm::slotSave()
{
    SaveFile(getPath());
}

void IDEForm::slotSaveAs()
{
    SaveFile();
}

void IDEForm::slotRemove()
{
    QString path=getPath();
    QFile file(path);

     if(file.exists())
     {
         int ret=showQuestionDialog(tr("delete"),tr("delete this file?"));
         switch(ret)
         {
         case QMessageBox::Yes:
             watcher->removePath(path);
             if(file.remove())
             {
                 deleteTab(ui->tabWidget->currentIndex());
                 //statusBar()->showMessage("Remove"+path,3000);
             }
             break;
         case QMessageBox::No:
         case QMessageBox::Cancel:
         default:break;
         }
     }
}

void IDEForm::slotRename()
{
    QString path  =getPath();
    QFile oldName(path);
    //路径不为空，则可以重命名
    if(path != "")
    {
        QString filePath=showFileDialog(tr("Save As"),QFileDialog::AcceptSave);
        if(oldName.exists())
        {
            QFileInfo newName=(filePath);
            watcher->removePath(path);
            bool b=oldName.rename(newName.fileName());
            if(!b)
            {
                watcher->addPath(path);
                showErrorMessage(tr("rename fialed"));
            }
            else
            {
                watcher->addPath(filePath);
                mainEditor->setFilePath(newName.absoluteFilePath());
                setTitle();
                //statusBar()->showMessage("Rename:"+newName.fileName(),3000);
            }
        }
        else
        {
            showErrorMessage(oldName.errorString());
        }
    }
}

void IDEForm::slotPrint()
{
    QPrintDialog dialog(this);
    dialog.setWindowTitle(tr("Print"));

    if(dialog.exec() == QPrintDialog::Accepted)
    {
        (new QsciPrinter)->printRange(mainEditor);
    }
}

void IDEForm::slotFind()
{
    findDialog.setTextEdit(mainEditor);
    findDialog.show();
}

void IDEForm::slotGoto()
{
    bool ok;
    int line=mainEditor->lines();

    int ln=QInputDialog::getInt(this,tr("Goto"),tr("line:"),1,1,line,1,&ok,
                                Qt::WindowCloseButtonHint);
    if(ok)
        mainEditor->setCursorPosition(ln-1,0);
}

void IDEForm::slotTabChanged(int index)
{
    qDebug() << "tab changed :" << index << Qt:: endl;
    mainEditor=dynamic_cast<TextEdit*>(ui->tabWidget->widget(index));//获取当前文本编辑器地址
    //qDebug() << getLineString() << Qt:: endl;
    //qDebug() << font << Qt:: endl;
    mainEditor->setTextFont(font,getLineString());//设置当前文本框字体
    showLength();

    setTitle();

    if(!findDialog.isHidden())
        findDialog.setTextEdit(mainEditor);
}

void IDEForm::slotTabClosed(int index)
{
    int ret=-1;
    if(mainEditor->isModified())
    {
        ret=showQuestionDialog(tr("Save"),tr("Save this file?"));
    }

    QString path = getPath();
    switch(ret)
    {
    case QMessageBox::Yes:
        SaveFile(path);
        if(path != "")//如果path不为空，则先删除监视器，再移除tab
            watcher->removePath(path);
        deleteTab(index);
        break;
    case QMessageBox::Cancel:
        break;
    case QMessageBox::No:
    default:
        if(path != "")//如果path不为空，则先删除监视器，再移除tab
            watcher->removePath(path);
        deleteTab(index);
        break;
    }
}

void IDEForm::slotCursorChanged(int line,int index)
{
    showLength();
    QString str=QString("ln:%1 col:%2").arg(line+1).arg(index+1);
    label->setText(str);

}

//settings
void IDEForm::slotFont()
{
    bool ok;
    font=QFontDialog::getFont(&ok,mainEditor->font());
    if(ok)
        mainEditor->setTextFont(font,getLineString());
}

void IDEForm::slotChangeFont(QFont f)
{
    font=f;
}

//help
void IDEForm::slotUser()
{
    QFileInfo file("./Readme.txt");

    if(file.isFile())
        OpenFile(file.absoluteFilePath());
    else
        QDesktopServices::openUrl(QUrl("www.baidu.com"));
}

// 找到该文件路径的Tab,重载这个文件
void IDEForm::slotFileWatcher(const QString &path)
{
    int i;
    TextEdit* temp=NULL;
    for(i=0;i<ui->tabWidget->count();i++)
    {
        temp=dynamic_cast<TextEdit*>(ui->tabWidget->widget(i));
        if(temp->getFilePath() == path)
            break;
    }
    if(temp)
    {
        watcher->blockSignals(true);
        mainEditor=temp;
        QString text=path+"\n"+tr("has been modified by another application \n reload?");
        int ret = showQuestionDialog(tr("Reload"),text);
        watcher->blockSignals(false);
        if(ret == QMessageBox::Yes)
        {
            deleteTab(i);
            OpenFile(path);
        }
        else
        {
            setTitle();
        }
        qDebug()<<watcher->signalsBlocked();
    }

}

void IDEForm::slotInitCPP()
{
    mainEditor->initCPPLexer();
    mainEditor->setTextFont(font,getLineString());
}

void IDEForm::slotInitCS()
{
    mainEditor->initCS();
    mainEditor->setTextFont(font,getLineString());
}

void IDEForm::slotInitJava()
{
    mainEditor->initJava();
    mainEditor->setTextFont(font,getLineString());
}

void IDEForm::slotInitPython()
{
    mainEditor->initPython();
    mainEditor->setTextFont(font,getLineString());
}

void IDEForm::slotChangeLanguage()
{
    bool status=ui->actionChinese->isEnabled();
    ui->actionChinese->setEnabled(!status);
    ui->actionEnglish->setEnabled(status);

    if(!ui->actionChinese->isEnabled())
    {
        if(trans->load("./EasyCode.qm"))
        {
            qApp->installTranslator(trans);
            ui->retranslateUi(this);
        }
        else
        {
            //statusBar()->showMessage(tr("can not find this translation file!"),5000);
        }
    }
    else
    {
        qApp->removeTranslator(trans);
        ui->retranslateUi(this);
    }
}

void IDEForm::slotTextChanged()
{
    ui->actionRedo->setEnabled(mainEditor->isRedoAvailable());
    ui->actionUndo->setEnabled(mainEditor->isUndoAvailable());

    if(!mainEditor->isModified())
        mainEditor->setModified(true);
    setTitle();
}

void IDEForm::slotLineChanged()
{
    mainEditor->setMarginWidth(0,getLineString());
}

void IDEForm::on_actionCompile_triggered()
{
    //qDebug() << "compile" << endl;
    SaveFile(getPath()); // 自动保存
    //打开编译信息框

    int index = ui->tabWidget->currentIndex();
    QString nowFilePath = mainEditor->getFilePath();

    //配置exe文件路径
    QString exeFilePath = nowFilePath;
    qDebug() << "nowFilePath:" << nowFilePath << Qt::endl;
    exeFilePath.replace(".cpp","");
    exeFilePath +=".exe";

    QString cmd = QString("g++ %1 -o %2 -g").arg(nowFilePath).arg(exeFilePath) + " 2> tmp.txt";   //使用 2> 输出错误信息
    qDebug() << "点击编译后的cmd:" << cmd << Qt::endl;

    //阻塞当前进程直到command命令执行完毕
    int r = system(cmd.toStdString().c_str());
    qDebug() << r << Qt::endl;
    if(r == 0)// gcc命名返回0表示正常
    {
        //计算输出文件大小和单位
        QFileInfo info(exeFilePath);
        float size = info.size();
        qDebug() << size << Qt::endl;
        QString unit ="B";
        if(size > 1024)
        {
            size/=1024;
            unit ="KB";
            if(size>1024)
            {
                size/=1024;
                unit ="MB";
                if(size>1024)
                {
                    size/=1024;
                    unit ="GB";
                }
            }
        }
        QString text = "编译成功 \n"
                       "输出文件名："+exeFilePath+"\n"
                       "最后修改时间："+info.lastModified().toString("yyyy-MM-dd hh:mm:ss")+"\n"
                       "文件大小："+QString::number(size)+unit+"\n";
        ComPileText.setPlainText(text);
        outputWidget->setCurrentIndex(0);
        /*ui->plainTextEdit_compile->setPlainText("编译成功 \n"
                                           "输出文件名："+exe+"\n"
                                           "最后修改时间："+info.lastModified().toString("yyyy-MM-dd hh:mm:ss")+"\n"
                                           "文件大小："+QString::number(size)+unit+"\n");*/
        //QMessageBox::information(this,"提示","编译成功");
    }else{//假如gcc命令返回-1,将报错输出到textedit中，并提示编译失败
        QFile tmpFile("tmp.txt");
        tmpFile.open(QIODevice::ReadWrite | QIODevice::Text);
        // ui->plainTextEdit_compile->setPlainText(tmpFile.readAll());
        ComPileText.setPlainText(tmpFile.readAll());
        tmpFile.close();

        //ComPileText.setPlainText(tmpFile);
        //QMessageBox::information(this,"提示","编译失败");
    }
    //删除报错文件
    system("del tmp.txt");

}

void IDEForm::on_actionRun_triggered()
{
    SaveFile(getPath()); // 自动保存
    //打开编译信息框

    int index = ui->tabWidget->currentIndex();
    QString nowFilePath = mainEditor->getFilePath();

    //配置exe文件路径
    QString exeFilePath = nowFilePath;
    exeFilePath.replace(".cpp","");
    exeFilePath +=".exe";

    QString cmd = QString("%1").arg(exeFilePath) + ">res.txt";
    qDebug() << "点击运行后的cmd:" << cmd << Qt::endl;

    int r=system(cmd.toStdString().c_str());
    if(r == 0)
    {
        //QMessageBox::information(this,"提示","运行成功");
        QFile resFile("res.txt");
        resFile.open(QIODevice::ReadWrite | QIODevice::Text);
        //ui->plainTextEdit_compile->setPlainText(resFile.readAll());
        OutPutText.setPlainText(resFile.readAll());
        outputWidget->setCurrentIndex(1);
        resFile.close();
    }
    else
    {
        QMessageBox::information(this,"提示","运行失败");
    }
    system("del res.txt");
}

void IDEForm::on_actionComPile_Run_triggered()
{
    on_actionCompile_triggered();
    on_actionRun_triggered();
}
