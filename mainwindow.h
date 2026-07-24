#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <vector>
#include "tabs_button.h"

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QList<Tabs_button> opened_tabs;


    explicit MainWindow(const QString &title, QWidget *parent = nullptr);
    ~MainWindow() override;
    QString m_title;

private slots:

    // Первое меню
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionOpen_file_triggered();
    void on_textEdit_textChanged();

    // Второе меню
    void on_actionCreate_New_triggered();
    void on_actionClose_triggered();

    void on_pushButton_Add_clicked();

    void on_pushButton_File_clicked();

    void on_tabWidget_currentChanged(int index);
    void on_actionOpen_folder_triggered();

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_actionCreate_new_tab_triggered();

    void on_actionClose_tab_triggered();

    void on_actionHide_file_tree_triggered();

private:

    bool saveToFile(const QString &filePath);

    Ui::MainWindow *ui;

    int num_of_opened_windows = 1;
    int num_of_opened_tabs = 1;
    std::vector<bool> List_saved_files;
    QString File_path = "";
    bool file_tree_is_hide = false;
};



#endif // MAINWINDOW_H