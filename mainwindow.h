#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
QString rot13( const QString &input,int offset );
private slots:


    void on_generatebtn_clicked();

    void on_decodebtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
