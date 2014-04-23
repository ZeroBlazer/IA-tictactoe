#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pGame(new Game(0, 4))
{
    ui->setupUi(this);
    startGame();
}

MainWindow::~MainWindow()
{
    delete m_pGame;
    delete ui;
}

void MainWindow::startGame()
{
    m_pGame->playBestPlay();
}

void MainWindow::on_pushButton_clicked()
{
    Position __tmp(ui->spinBox->value(),
                   ui->spinBox_2->value());
    m_pGame->play(__tmp);
    m_pGame->playBestPlay();
}
