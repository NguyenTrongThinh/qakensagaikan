#include "cSettingDialog.h"
#include "ui_cSettingDialog.h"
#include <QPainter>
#include "cConfigureUtils.h"
#include <QDebug>
#include <QStyleOption>

cSettingDialog::cSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cSettingDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->ckhManual->setText("Chế độ thủ công");
    ui->ckhManual->setStyleSheet("QCheckBox::indicator { width: 40px; height: 40px;};");
    m_NumberPad = new cNumberPad2();
    connect(m_NumberPad, SIGNAL(buttonClicked(QString)), this, SLOT(onKeypadClicked(QString)));
    ui->hBoxKayPad->addStretch();
    ui->hBoxKayPad->addWidget(m_NumberPad);
    ui->hBoxKayPad->addStretch();
    if (QString::compare(cConfigureUtils::getManualMode(), "0") == 0)
        ui->ckhManual->setChecked(false);
    else
        ui->ckhManual->setChecked(true);
}

cSettingDialog::~cSettingDialog()
{
    ui->hBoxKayPad->removeWidget(m_NumberPad);
    delete m_NumberPad;
    delete ui;
}

void cSettingDialog::setLineEditText(const QString &text)
{
    bool isconvertOK;
    int number  = text.toInt(&isconvertOK, 10);
    if (!isconvertOK)
    {
        if (QString::compare(text, "") == 0) {
            ui->leditTimeDelay->clear();
        } else {
        qDebug() << "Invalid input number";
        }

    } else {
        qDebug() << "Append input number: " << number;
    }
    if (QString::compare(ui->leditTimeDelay->text(), "0") == 0)
    {
        ui->leditTimeDelay->clear();
        ui->leditTimeDelay->setText(text);
    }
    else {
        ui->leditTimeDelay->setText(ui->leditTimeDelay->text() + text);
    }

    ui->leditTimeDelay->setFocus();
}

void cSettingDialog::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void cSettingDialog::on_btnCancel_clicked()
{
    this->close();
}


void cSettingDialog::on_btnOK_clicked()
{
    bool isConverOK = false;
    QString sDekay = ui->leditTimeDelay->text();
    int delay = sDekay.toInt(&isConverOK, 10);
    if (ui->ckhManual->isChecked()) {
        cConfigureUtils::setManaualMode("1");
    } else {
        cConfigureUtils::setManaualMode("0");
    }
    if (isConverOK && delay >= 0) {
        cConfigureUtils::setDelayValue(delay);
    }
    this->close();
}

void cSettingDialog::onKeypadClicked(QString val)
{
    if (QString::compare(val, "del") == 0) {
        setLineEditText("");
    } else {
        setLineEditText(val);
    }
}
