#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTextStream>
#include <QFile>

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

bool MainWindow::saveToFile(const QString &filePath) //проблемма с сохранением при сохранении текста с 1 вкладки и переходе в другую и повторном сохранении не появляется окно сохранения и текст перезаписывается в тотже файл
{
     QString text;
    int tab_index = ui->tabWidget->currentIndex();
    QWidget *curet_tab = ui->tabWidget->widget(tab_index);
    if(curet_tab)
    {
        QTextEdit *text_tab = curet_tab->findChild<QTextEdit*>();
        if(text_tab)
        {
            text = text_tab->toPlainText();

        }
        else
        {
            QMessageBox::critical(this, "Error", QString("Can't find TextEdit"));
            qDebug() << "Can't find TextEdit";
            return false;
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", QString("Can't find tab"));
        qDebug() << "Can't find tab";
        return false;
    }

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", QString("Cannot open file for writing: %1").arg(file.errorString()));
        qDebug() << "Cannot open file for writing:" << filePath << file.errorString();
        return false;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << text;

    if (stream.status() != QTextStream::Ok) {
        QMessageBox::critical(this, "Error", QString("Failed to write to file: %1").arg(file.errorString()));
        qDebug() << "Failed to write to file:" << filePath << stream.status();
        file.close();
        return false;
    }

    if (!file.flush()) {
        QMessageBox::critical(this, "Error", QString("Failed to flush file: %1").arg(file.errorString()));
        qDebug() << "Failed to flush file:" << filePath << file.errorString();
        file.close();
        return false;
    }

    file.close();

    File_path = filePath;
    QFileInfo fileInfo(File_path);
    this->setWindowTitle(File_path);
    ui->statusbar->showMessage("File saved");
    //здесь код для изменения заголовка и цвета выбраной страници

    qDebug() << "File saved:" << File_path;
    return true;
}

void MainWindow::on_actionSave_triggered()
{
    if (File_path.isEmpty()) {
        on_actionSave_as_triggered();
    } else {
        saveToFile(File_path);
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    const QString filePath = QFileDialog::getSaveFileName(this,
        "Save File",
        File_path.isEmpty() ? "text.txt" : File_path,
        "Text Files (*.txt);;All Files (*)",
        nullptr);

    if (filePath.isEmpty()) {
        qDebug() << "Save cancelled by user";
        return;
    }
    saveToFile(filePath);
}

void MainWindow::on_actionOpen_file_triggered()
{
    const QString filePath = QFileDialog::getOpenFileName(this,
        "Select file", "", "Text Files (*.txt);;All Files (*)", nullptr);

    if (filePath.isEmpty()) {
        qDebug() << "File path empty";
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", QString("Cannot open file for reading: %1").arg(file.errorString()));
        qDebug() << "Cannot open file for reading:" << filePath << file.errorString();
        return;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    const QString text = stream.readAll();
    file.close();

    if (stream.status() != QTextStream::Ok) {
        QMessageBox::critical(this, "Error", "Failed to read file content");
        qDebug() << "Failed to read file:" << filePath;
        return;
    }

    ui->textEdit->setText(text);

    File_path = filePath;
    this->setWindowTitle(File_path);
    ui->statusbar->showMessage("File loaded");
    QFileInfo fileInfo(File_path);

}

void MainWindow::on_textEdit_textChanged()
{

}

void MainWindow::on_actionCreate_New_triggered()
{
    MainWindow *new_window = new MainWindow(QString("window_num%1")
                                                .arg(num_of_opened_windows), nullptr);
    new_window->show();
}

void MainWindow::on_actionClose_triggered()
{
    qDebug() << "Window named:" << this->m_title << " --CLOSED";
    this->close();
}

void MainWindow::on_pushButton_Add_clicked()
{
    //this.textEdit =
}


void MainWindow::on_pushButton_File_clicked()
{
    //if(this.currentFile !=  )
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == ui->tabWidget->count() - 1)
    {
        QWidget *new_tab = new QWidget;
        QTextEdit *new_text_box = new QTextEdit(new_tab);
        QVBoxLayout *new_loaut = new QVBoxLayout(new_tab);
        new_loaut->addWidget(new_text_box);
        new_tab->setLayout(new_loaut);
        ui->tabWidget->insertTab(ui->tabWidget->count() - 2,new_tab,"Unsaved file");
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-2);
    }
}

