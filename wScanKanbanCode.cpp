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
