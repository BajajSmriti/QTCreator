#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFont>
#include <QLabel>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::maybeSave(){
    if (! ui->textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return on_actionSave_As_triggered();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::on_actionNew_triggered()
{
    if(maybeSave()){
        file_path="";
        ui->textEdit->setText(file_path);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Open the file");
    if(maybeSave()){
    QFile file(file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"..","Unable to open the file.");
        return;
    }
    setWindowTitle(file_name);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    }
}

void MainWindow::on_actionSave_triggered()
{
    QFile file(file_path);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"..","Unable to save the file.");
        return ;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();
}

bool MainWindow::on_actionSave_As_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(this,"Save the file");
    if(! file_name.isEmpty()){
        on_actionSave_triggered();
        return true;
    }
    QFile file(file_name);
    file_path = file_name;

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"..","Unable to save the file.");
        return false;
    }
    setWindowTitle(file_name);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();
    return true;
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionAbout_me_triggered()
{
    QString about_text;
    about_text = "Author:Smriti";
    QMessageBox::about(this,"About me",about_text);
}

void MainWindow::on_actionbold_triggered()
{
    QTextCharFormat font;

    if(!dirty){
        font.setFontWeight(QFont::Bold);

        dirty = true;
    }
    else{
        font.setFontWeight(QFont::Normal);

        dirty=false;
    }
    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(font);
    ui->textEdit->mergeCurrentCharFormat(font);
}

void MainWindow::on_actionSuperscript_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QString selected_text = cursor.selectedText();
    ui->textEdit->textCursor().removeSelectedText();
    ui->textEdit->insertHtml(QString("<sup>%1</sup>").arg(selected_text));
}

void MainWindow::on_actionSubscript_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QString selected_text = cursor.selectedText();
    ui->textEdit->textCursor().removeSelectedText();
    ui->textEdit->insertHtml(QString("<sub>%1</sub>").arg(selected_text));
}
