#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTextStream>
#include <QFile>
#include <QFileSystemModel>

MainWindow::MainWindow(const QString &title, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_title(title)
{
    ui->setupUi(this);
    this->setWindowTitle("Zubr");
    List_saved_files.push_back(false);
    QFileSystemModel *model = new QFileSystemModel(this); //открытие домашней папки
    model->setRootPath(QDir::homePath());
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(QDir::homePath()));
}
///
///                     Что нужно добавить:
///     1.Ошибки в открытии файла из под левого меню (256-268)
///     2.Добавить настройки шрифта и размера текста
///     3.Ввести список txt схожих файлов которые откроются (под вопросом)
///     4 опц. Добавить перевод на русский
///     5.Реализовать скрытие левого меню (✅)
///
/// from perforator-coder: Это уже хорошый блокнот но требующий улучшений и больше тестировки. Я не уверен есть ли ещё баги.
MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::saveToFile(const QString &filePath)
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
    List_saved_files[tab_index] = true;

    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),fileInfo.fileName());
    //здесь код для изменения заголовка и цвета выбраной страници

    qDebug() << "File saved:" << File_path;
    return true;
}

void MainWindow::on_actionSave_triggered()
{
    if (File_path.isEmpty()||List_saved_files[ui->tabWidget->currentIndex()] == false) {
        on_actionSave_as_triggered();
    } else {
        saveToFile(File_path);
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    const QString filePath = QFileDialog::getSaveFileName(this,
        "Save File",
        File_path.isEmpty() ? QDir::homePath() : File_path,
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
        "Select file", QDir::homePath(), "Text Files (*.txt);;All Files (*)", nullptr);

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
    QFileInfo file_info(file);
    stream.setEncoding(QStringConverter::Utf8);
    const QString text = stream.readAll();
    file.close();

    if (stream.status() != QTextStream::Ok) {
        QMessageBox::critical(this, "Error", "Failed to read file content");
        qDebug() << "Failed to read file:" << filePath;
        return;
    }

    int index = ui->tabWidget->currentIndex();
    QWidget *table = ui->tabWidget->widget(index);
    if(table)
    {
        QTextEdit *text_from_table = table->findChild<QTextEdit*>();
        if(text_from_table)
        {
            text_from_table->setText(text);
            List_saved_files[ui->tabWidget->currentIndex()] = true;
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),file_info.fileName());
        }
        else
        {
            QMessageBox::critical(this, "Error", QString("Can't find TextEdit"));
            qDebug() << "Can't find TextEdit";
            return ;
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", QString("Can't find Table"));
        qDebug() << "Can't find Table";
        return;
    }

    File_path = filePath;
    this->setWindowTitle(File_path);
    ui->statusbar->showMessage("File loaded");


}

void MainWindow::on_textEdit_textChanged()
{
    int index_tab = ui->tabWidget->currentIndex();
    List_saved_files[index_tab] = false;
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
        ui->tabWidget->insertTab(ui->tabWidget->count() - 1,new_tab,"Unsaved file");
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-2);
        num_of_opened_tabs += 1;
        List_saved_files.push_back(false);
    }
}


void MainWindow::on_actionOpen_folder_triggered()
{
    QString dir_path = QFileDialog::getExistingDirectory(
        this,
        "Select folder",
        QDir::homePath(),
        QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks
        );
    if(dir_path.isEmpty()||dir_path.isNull())
    {
        QMessageBox::critical(this, "Error", QString("Can't find dir_path"));
        qDebug() << "Can't find dir_path";
        return;
    }
    if(file_tree_is_hide){
        on_actionHide_file_tree_triggered();
    }
    QFileSystemModel *model_dir = new QFileSystemModel(this);
    model_dir->setRootPath(dir_path);
    ui->treeView->setModel(model_dir);
    ui->treeView->setRootIndex(model_dir->index(dir_path));
    ui->statusbar->showMessage("Folder load");

}


void MainWindow::on_treeView_doubleClicked(const QModelIndex &index) //не дописано
{
    QFileSystemModel *model_select = qobject_cast<QFileSystemModel*>(ui->treeView->model()); // получаем модель
    if(!model_select) //проверка что есть модель и проверка что за файл
    {
         //добавить ошибку
        return;
    }
    if(model_select->isDir(index))
    {
        //добавить ошибку
        return;
    }
    QString file_path = model_select->filePath(index); // находим файл и текущию вкладку
    int index_tab = ui->tabWidget->currentIndex();
    QFile file(file_path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream file_stream(&file_path);
    file_stream.setEncoding(QStringConverter::Utf8);
    QString text_from_tree = file.readAll();
    file.close();
    QFileInfo file_info(file);
    if(List_saved_files[index_tab] == false)
    {
        QMessageBox::StandardButton ansver;
        ansver = QMessageBox::question(this,"Open file problem","Current tab is not saved.\nYou want open new tab or open file in current?",QMessageBox::Yes|QMessageBox::No);
        if(ansver==QMessageBox::Yes)
        {
            on_tabWidget_currentChanged(ui->tabWidget->count());
            qDebug() << "Selected yes, open file in new  tab";
        }
        else if (ansver==QMessageBox::No)
        {
           qDebug() << "Selected no, open file in current tab";
        }
        else
        {
            qDebug() << "answer not geted show message about error";
            QMessageBox::critical(this, "Error", QString("Can't find answer"));
            return;
        }

    }

    QWidget *curet_tab = ui->tabWidget->widget(index_tab);// запись текста в текущю вкладку
    if(curet_tab)
    {
        QTextEdit *text_tab = curet_tab->findChild<QTextEdit*>();
        if(text_tab)
        {

            text_tab->setText(text_from_tree);
            List_saved_files[index_tab] = true;
            ui->statusbar->showMessage("File from folder loaded");
            ui->tabWidget->setTabText(index_tab,file_info.fileName());
        }
        else
        {
            QMessageBox::critical(this, "Error", QString("Can't find TextEdit"));
            qDebug() << "Can't find TextEdit";
            return;
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", QString("Can't find tab"));
        qDebug() << "Can't find tab";
        return ;
    }
}


void MainWindow::on_actionCreate_new_tab_triggered()
{
    on_tabWidget_currentChanged(ui->tabWidget->count() - 1);
    return;
}


void MainWindow::on_actionClose_tab_triggered()
{

    QMessageBox::StandardButtons answer;

    int index = ui->tabWidget->currentIndex();

    if(List_saved_files[index] == false)
    {
        answer = QMessageBox::question(this,"Close tab problem","Current tab is not saved.\n You want save and close or close?",QMessageBox::Yes|QMessageBox::No);
        if(answer == QMessageBox::Yes)
        {
            on_actionSave_triggered();
            qDebug() << "Selected yes, saveing file";
        }else if (answer == QMessageBox::No)
        {
           qDebug() << "Selected no, close tab";
        }
        else
        {
            qDebug() << "answer is null show error";
            QMessageBox::critical(this, "Error", QString("Can't find answer"));
            return;
        }
    }

    ui->tabWidget->blockSignals(true);
    List_saved_files.erase(List_saved_files.begin() + index);


    ui->tabWidget->removeTab(index);
    ui->tabWidget->setCurrentIndex(index - 1);
    ui->tabWidget->clearFocus();
    this->setFocus();
    ui->tabWidget->update();
    ui->tabWidget->repaint();
    ui->tabWidget->blockSignals(false);
    if(ui->tabWidget->count() == 1)
    {
        on_tabWidget_currentChanged(index);
    }


}


void MainWindow::on_actionHide_file_tree_triggered()
{
    if(!file_tree_is_hide){
        ui->treeView->hide();
        file_tree_is_hide = true;
        ui->actionHide_file_tree->setText("Show file tree");
    }
    else
    {
        file_tree_is_hide = false;
        ui->treeView->setVisible(true);
        ui->actionHide_file_tree->setText("Hide file tree");
    }

}

