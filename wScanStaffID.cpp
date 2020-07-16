#include "wScanStaffID.h"
#include "ui_wScanStaffid.h"

wScanStaffID::wScanStaffID(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanStaffID)
{
    ui->setupUi(this);
}

wScanStaffID::~wScanStaffID()
{
    delete ui;
}

void wScanStaffID::setStaffIDLable(QString text)
{
    ui->lblStaffID->setText(text);
}
