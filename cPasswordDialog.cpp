#include "cPasswordDialog.h"
#include "ui_cPasswordDialog.h"
#include <QPainter>
#include <QDebug>
#include <QStyleOption>

cPasswordDialog::cPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cPasswordDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    m_NumberPad = new cNumberPad2();

    ui->hBoxKeyPad->addStretch();
    ui->hBoxKeyPad->addWidget(m_NumberPad);
    ui->hBoxKeyPad->addStretch();


    connect(m_NumberPad, SIGNAL(buttonClicked(QString)), this, SLOT(onKeypadClicked(QString)));
    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));


}

cPasswordDialog::~cPasswordDialog()
{
    ui->hBoxKeyPad->removeWidget(m_NumberPad);

    delete m_NumberPad;

    delete ui;
}

QString cPasswordDialog::getPassword()
{
    if (ui->lnPassword != nullptr)
        return ui->lnPassword->text();
    return "";
}

void cPasswordDialog::setLineEditText(const QString &text)
{
    bool isconvertOK;
    int number  = text.toInt(&isconvertOK, 10);
    if (!isconvertOK)
    {
        if (QString::compare(text, "") == 0) {
            ui->lnPassword->clear();
        } else {
        qDebug() << "Invalid input number";
        }

    } else {
        qDebug() << "Append input number: " << number;
        if (QString::compare(ui->lnPassword->text(), "0") == 0)
        {
            ui->lnPassword->clear();
            ui->lnPassword->setText(text);
        }
        else {
            ui->lnPassword->setText(ui->lnPassword->text() + text);
        }
    }
    ui->lnPassword->setFocus();
}

void cPasswordDialog::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void cPasswordDialog::onKeypadClicked(QString val)
{
    if (QString::compare(val, "del") == 0) {
        setLineEditText("");
    } else {
        setLineEditText(val);
    }
}
