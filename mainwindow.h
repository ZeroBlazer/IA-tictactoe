#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <util.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void startGame();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Game *m_pGame;
};

#endif // MAINWINDOW_H
