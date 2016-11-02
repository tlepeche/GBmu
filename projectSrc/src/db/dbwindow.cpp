#include "dbwindow.h"
#include "ui_dbwindow.h"

DbWindow::DbWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbWindow)
{
    ui->setupUi(this);
}

DbWindow::~DbWindow()
{
    delete ui;
}
