#include "wSelectLine.h"
#include "ui_wSelectLine.h"
#include <QPainter>
#include "cConfigureUtils.h"
#include <QDebug>
#include <QStyleOption>
#include "cMessageBox.h"

wSelectLine::wSelectLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wSelectLine)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    m_NumberPad = new cNumberPad2();
    connect(m_NumberPad, SIGNAL(buttonClicked(QString)), this, SLOT(onKeypadClicked(QString)));
    ui->hBoxKayPad->addStretch();
    ui->hBoxKayPad->addWidget(m_NumberPad);
    ui->hBoxKayPad->addStretch();

}

wSelectLine::~wSelectLine()
{
    ui->hBoxKayPad->removeWidget(m_NumberPad);
    delete m_NumberPad;
    delete ui;
}

void wSelectLine::setLineEditText(const QString &text)
{
    bool isconvertOK;
    int number  = text.toInt(&isconvertOK, 10);
    if (!isconvertOK)
    {
        if (QString::compare(text, "") == 0) {
            ui->leLine->clear();
        } else {
        qDebug() << "wSelectLine::setLineEditText-Invalid input number";
        }

    } else {
        qDebug() << "wSelectLine::setLineEditText-Append input number: " << number;
    }

    if (QString::compare(ui->leLine->text(), "0") == 0)
    {
        ui->leLine->clear();
        ui->leLine->setText(text);
    }
    else {
        ui->leLine->setText(ui->leLine->text() + text);
    }




}

void wSelectLine::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void wSelectLine::on_btnCancel_clicked()
{
    this->close();
}


void wSelectLine::on_btnOK_clicked()
{
    QString sLine = ui->leLine->text();

    if (sLine != "") {
        cConfigureUtils::setLine(sLine);
    }
    else
    {
        cMessageBox *m_MessageBox = new cMessageBox();
        m_MessageBox->setText("Line không hợp lệ");
        m_MessageBox->setInformativeText("Vui lòng kiểm tra lại");
        m_MessageBox->setIcon(QPixmap(":/images/resources/critical_80x80.png"));
        m_MessageBox->setHideRejectButton();
        m_MessageBox->exec();
        delete m_MessageBox;
    }
    this->close();
}

void wSelectLine::onKeypadClicked(QString val)
{
    if (QString::compare(val, "del") == 0) {
        setLineEditText("");
    } else {
        setLineEditText(val);
    }
}
