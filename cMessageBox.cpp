#include "cMessageBox.h"
#include "ui_cMessageBox.h"
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

cMessageBox::cMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cMessageBox)
{
    ui->setupUi(this);
    connect(ui->lblAcept, SIGNAL(sigMouseReleaseEvent()), this, SLOT(accept()));
    connect(ui->lblReject, SIGNAL(sigMouseReleaseEvent()), this, SLOT(reject()));
    setWindowTitle("Kensa Bonder");
    this->setWindowFlags(windowFlags() &(~Qt::WindowMinMaxButtonsHint));
}

cMessageBox::~cMessageBox()
{
    delete ui;
}

void cMessageBox::setText(QString text)
{
    ui->lblText->setText(text);
}

void cMessageBox::setInformativeText(QString text)
{
    ui->lblInformativeText->setText(text);
}

void cMessageBox::setIcon(QPixmap pixmap)
{
    ui->lblIcon->setPixmap(pixmap);
}

void cMessageBox::setHideRejectButton()
{
    ui->lblReject->setVisible(false);
}

void cMessageBox::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
            if (isMinimized()){
                qDebug() << "it was minimised";
        }
        else{
                qDebug() << "it's normal or maximised";
        }
    }
    QDialog::changeEvent(event);
}

void cMessageBox::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}
