#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pGame(NULL)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if(m_pGame)
        delete m_pGame;
    delete ui;
}

void MainWindow::startGame()
{
    if(m_pGame)
        delete m_pGame;
    m_pGame = new Game(!ui->checkBox->isChecked(),
                       ui->spinBox_3->value());
    if(ui->checkBox->isChecked())
        m_pGame->bestPlay(ui->spinBox_4->value());
}

void MainWindow::on_pushButton_clicked()
{
    Position __tmp(ui->spinBox->value(),
                   ui->spinBox_2->value());
    m_pGame->play(__tmp);
    m_pGame->bestPlay(ui->spinBox_4->value());
}

void MainWindow::on_pushButton_2_clicked()
{
    startGame();
}
