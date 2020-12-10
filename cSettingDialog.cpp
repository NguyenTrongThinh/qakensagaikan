#include "cSettingDialog.h"
#include "ui_cSettingDialog.h"
#include <QPainter>
#include "cConfigureUtils.h"
#include <QDebug>
#include <QStyleOption>
#include "cMessageBox.h"

cSettingDialog::cSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cSettingDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->ckhManual->setText("Chế độ thủ công");
    ui->ckhManual->setStyleSheet("QCheckBox::indicator { width: 40px; height: 40px;};");
    m_NumberPad = new cNumberPad2();
    m_lineedit = ui->leditTimeDelay;
    connect(m_NumberPad, SIGNAL(buttonClicked(QString)), this, SLOT(onKeypadClicked(QString)));
    connect(ui->leditTimeDelay, SIGNAL(sigMousePressEvent()), this, SLOT(focusTimeEdit()));
    connect(ui->leNumOfSameScan, SIGNAL(sigMousePressEvent()), this, SLOT(focusNUmOfSameEdit()));
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
            m_lineedit->clear();
        } else {
        qDebug() << "cSettingDialog::setLineEditText-Invalid input number";
        }

    } else {
        qDebug() << "cSettingDialog::setLineEditText-Append input number: " << number;
    }

    if (QString::compare(m_lineedit->text(), "0") == 0)
    {
        m_lineedit->clear();
        m_lineedit->setText(text);
    }
    else {
        m_lineedit->setText(m_lineedit->text() + text);
    }




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
        if (isConverOK && delay >= 0) {
            cConfigureUtils::setManaualMode("0");
            cConfigureUtils::setDelayValue(delay);
        }
        else
        {
            cMessageBox *m_MessageBox = new cMessageBox();
            m_MessageBox->setText("Thm_lineeditời gian delay không hợp lệ");
            m_MessageBox->setInformativeText("Vui lòng kiểm tra lại");
            m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
            m_MessageBox->setHideRejectButton();
            m_MessageBox->exec();
            delete m_MessageBox;
        }
    }
    bool isConvertOK = false;
    QString sNumSameScan = ui->leNumOfSameScan->text();
    int numSameScan = sNumSameScan.toInt(&isConvertOK, 10);
    if(isConvertOK && sNumSameScan >= 1)
    {
        cConfigureUtils::setNumOfSameScan(numSameScan);
    }
    else
    {
        cMessageBox *m_MessageBox = new cMessageBox();
        m_MessageBox->setText("Số lần lặp không hợp lệ");
        m_MessageBox->setInformativeText("Vui lòng kiểm tra lại");
        m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
        m_MessageBox->setHideRejectButton();
        m_MessageBox->exec();
        delete m_MessageBox;

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

void cSettingDialog::setInputObject(QLineEdit *objLineEdit)
{
    m_lineedit = objLineEdit;
    m_lineedit->setFocus();
}

void cSettingDialog::focusTimeEdit()
{
    setInputObject(ui->leditTimeDelay);
}

void cSettingDialog::focusNumOfRepeatEdit()
{

}

void cSettingDialog::focusNUmOfSameEdit()
{
    setInputObject(ui->leNumOfSameScan);
}
