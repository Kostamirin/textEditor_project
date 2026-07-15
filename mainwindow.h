#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &title, QWidget *parent = nullptr);
    ~MainWindow() override;
    QString m_title;

private slots:


    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionOpen_file_triggered();

    void on_textEdit_textChanged();

private:
    Ui::MainWindow *ui;

    QString File_path = "";
};
#endif // MAINWINDOW_H
