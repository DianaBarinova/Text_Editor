#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QFont>
#include <QSettings>
#include <QDebug>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();

    delete ui;
    delete mFile;
}

void MainWindow::on_actionSave_triggered()
{
    if (!mFile) {
        on_actionSave_As_triggered();
    } else {
        mFile->open(QFile::WriteOnly);
        QTextStream stream(mFile);
        stream << ui->textEdit->toPlainText();
        mFile->close();
    }
}


void MainWindow::on_actionOpen_triggered()
{
    // https://doc.qt.io/qt-5/qfiledialog.html#details
    QString filePath = QFileDialog::getOpenFileName(
                this,                    // parent
                tr("Open File"),         // dialog title
                tr(""),                  // starting directory
                tr("Text files (*.txt)") // filter
                );
    mFile = new QFile(filePath);
    updateTextEdit();
}

void MainWindow::on_actionSave_As_triggered()
{/*
    // comment the following call after implementing the method
    QMessageBox::warning(this,
                         tr("\"on_actionSave_As_triggered\" method is not implemented."),
                         tr("It is one of the methods you need to implement\n"
                            "to complete the assignment.")
                         );
    }*/
    QString filePath = QFileDialog::getSaveFileName(this,"file saved",tr(""),tr("Text files (*.txt)"));
      QFile file(filePath);
      if(!file.open(QFile::WriteOnly | QFile::Text)) {
          QMessageBox::warning(this,"..","file not saved");
          return;
        }
      QTextStream out(&file);
      QString text = ui->textEdit->toPlainText();
      out << text;
      file.flush();
      file.close();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionFont_triggered()
{
    /* comment the following call after implementing the method
    QMessageBox::warning(this,
                         tr("\"on_actionFont_triggered\" method is not implemented."),
                         tr("It is one of the methods you need to implement\n"
                            "to complete the assignment.")
                         );*/

    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Times", 12), this);
    if (ok) {
        // шрифт установлен на шрифт, выбранный пользователем
        ui->textEdit->selectAll();
         ui->textEdit->setFont(font);
        ui->textEdit->textCursor().clearSelection();
    } else {
        // пользователь отменил диалог, шрифт установлен на начальный (Times, 12).
    }

}


void MainWindow::on_actionColor_triggered()
{
    auto color = QColorDialog::getColor();
    if (!ui->textEdit->textCursor().hasSelection()){
        ui->textEdit->selectAll();
        ui->textEdit->setTextColor(color);
        ui->textEdit->textCursor().clearSelection();
    }

    ui->textEdit->setTextColor(color);
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionPaste_triggered()
{
  ui->textEdit->paste();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Title", "Message");
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionRedo_triggered()
{
     ui->textEdit->redo();
}

void MainWindow::updateTextEdit(QColor * color /* = nullptr */, QFont * font /* = nullptr */) {
    if (mFile){
        mFile->open(QFile::ReadOnly);

        QTextStream stream(mFile);
        QString text = stream.read(10000);

        mFile->close();

        ui->textEdit->setText(text);
    }

    if (color) {
        ui->textEdit->selectAll();
        ui->textEdit->setTextColor(*color);
        ui->textEdit->textCursor().clearSelection();
    }
    if (font) {
        ui->textEdit->setFont(*font);
    }
}

void MainWindow::saveSettings() {
    QSettings settings("org", "app");
    settings.beginGroup("textEdit");
    if (mFile) settings.setValue("file_name", mFile->fileName());
    settings.setValue("color", ui->textEdit->textColor());
    settings.setValue("font", ui->textEdit->font());
    settings.endGroup();

    settings.beginGroup("window");
    settings.setValue("size", this->size());
    settings.endGroup();
}

void MainWindow::loadSettings() {
    /* comment the following call after implementing the method
    QMessageBox::warning(this,
                         tr("\"loadSettings\" method is not implemented."),
                         tr("It is one of the methods you need to implement\n"
                            "to complete the assignment.")
                         );*/


    QSettings settings("org", "app");
        settings.beginGroup("textEdit");
        if (mFile) mFile->setFileName(settings.value("file_name", QFile().fileName()).value<QString>());
        ui->textEdit->setTextColor(settings.value("color", QColor()).value<QColor>());
        ui->textEdit->setFont(settings.value("font", QFont()).value<QFont>());
        settings.endGroup();

        settings.beginGroup("window");
        resize(settings.value("size", QSize()).toSize());
        settings.endGroup();
   }
