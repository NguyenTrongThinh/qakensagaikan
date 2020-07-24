#include "wScanKanbanCode.h"
#include "ui_wScanKanbanCode.h"

wScanKanbanCode::wScanKanbanCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanKanbanCode)
{
    ui->setupUi(this);
}

wScanKanbanCode::~wScanKanbanCode()
{
    delete ui;
}

void wScanKanbanCode::setKanbanLable(QString kanbancode)
{
    ui->lbCodeMH->setText(kanbancode);
}

void wScanKanbanCode::setMaAB1(QString maab1)
{
    ui->lbCodeAB1->setText(maab1);
}

void wScanKanbanCode::setMaAB2(QString maab2)
{
    ui->lbCodeAB2->setText(maab2);
}
