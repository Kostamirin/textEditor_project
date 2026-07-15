#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
MainWindow::MainWindow(const QString &title, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_title(title)
{
    ui->setupUi(this);
    this->setWindowTitle("Enter the text");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSave_triggered()
{

    QString input_text_user = ui->textEdit->toPlainText();
    if(File_path.isEmpty())// проверка есть ли уже файл
    {
        File_path = QFileDialog::getSaveFileName(this,"file save","text.txt","txt (*.txt);;Any file (*)",nullptr);
        if(File_path.isEmpty())//проверяем путь не пустой
        {
            QMessageBox::critical(this,"ERROR","Not find file path");//ошибка если путь к файлу не найден
            qDebug()<<"Patch not find";
            return;
        }


        QFile file(File_path);//Открываем файл и записываем
        QTextStream file_strim(&file);
        file.open(QIODeviceBase::WriteOnly);
        file_strim <<input_text_user;
        file.close();
        qDebug()<<"Path:" + File_path;//Отладка
        this->setWindowTitle(File_path);//после сохранения изменяем кнопку и выводим сообщение
        ui->statusbar->showMessage("File saved");
        QFileInfo file_info(File_path);
        ui->pushButton_2->setText(file_info.baseName());
        ui->pushButton_2->setStyleSheet("background-color: rgb(0, 111, 0);");

        return;
    }
    else//если файл есть но нужно изменить текст внутри
    {
        QFile file(File_path);
        QTextStream file_strim(&file);
        file.open(QIODeviceBase::WriteOnly);
        file_strim <<input_text_user;
        file.close();
        qDebug()<<"Path:" + File_path;
        this->setWindowTitle(File_path);  
        ui->statusbar->showMessage("File saved");
        ui->pushButton_2->setStyleSheet("background-color: rgb(0, 111, 0);");// потом стоит записать все это в класс и хранить классы в листе
        return;
    }

}



void MainWindow::on_actionSave_as_triggered()
{
    QString input_text_user = ui->textEdit->toPlainText();
    File_path = QFileDialog::getSaveFileName(this,"file save","text.txt","txt (*.txt);;Any file (*)",nullptr);
    if(File_path.isEmpty())
    {
        QMessageBox::critical(this,"ERROR","Not find file path");//ошибка если путь к файлу не найден
        qDebug()<<"ERROR patch not find";
        return;
    }

    QFile file(File_path);
    QTextStream file_strim(&file);
    file.open(QIODeviceBase::WriteOnly);
    file_strim << input_text_user;
    file.close();
    qDebug()<<"Path:" + File_path;
    this->setWindowTitle(File_path);
    ui->statusbar->showMessage("File saved");
    QFileInfo file_info(File_path);
    ui->pushButton_2->setText(file_info.baseName());
    ui->pushButton_2->setStyleSheet("background-color: rgb(0, 111, 0);");
    return;

}


void MainWindow::on_actionOpen_file_triggered()
{
    File_path =  QFileDialog::getOpenFileName(this,"Select file","","txt (*.txt);;Any file (*)",nullptr);
    if(File_path.isEmpty())
    {
        QMessageBox::critical(this,"ERROR","File path empty");
        qDebug()<<"File patch empty";
        return;
    }
    QFile file_open(File_path);
    QTextStream file_write(&file_open);
    file_open.open(QIODeviceBase::ReadOnly);
    QString text_from_file = file_write.readAll();
    ui->textEdit->setText(text_from_file);
    file_open.close();

    this->setWindowTitle(File_path);
    ui->statusbar->showMessage("File loaded");
    QFileInfo file_info(File_path);
    ui->pushButton_2->setText(file_info.baseName());
    ui->pushButton_2->setStyleSheet("background-color: rgb(0, 111, 0);");
    return;
}


void MainWindow::on_textEdit_textChanged()
{
    ui->pushButton_2->setStyleSheet("background-color:rgb(148, 110, 6);");
}

