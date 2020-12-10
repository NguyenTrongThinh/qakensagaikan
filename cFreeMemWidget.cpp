#include "cFreeMemWidget.h"
#include <QStorageInfo>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>

cFreeMemWidget::cFreeMemWidget(QWidget *parent) : QWidget(parent)
{
    Q_UNUSED(parent);
    m_MemLabel = new QLabel();
    m_MemLabel->setAlignment(Qt::AlignCenter);
    QFont ubuntuTimeFont("Ubuntu", 15, QFont::Bold);
    m_MemLabel->setFont(ubuntuTimeFont);
    m_HBoxLayout = new QHBoxLayout();
    m_HBoxLayout->setMargin(0);
    m_HBoxLayout->setSpacing(0);
    m_HBoxLayout->addWidget(m_MemLabel);
    this->setLayout(m_HBoxLayout);
    setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(145, 132, 132); border: 4px; border-radius: 10px;");
    m_Timer = new QTimer();
    m_Timer->setInterval(60000);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
    onTimerTimeout();
    m_Timer->start();
}

cFreeMemWidget::~cFreeMemWidget()
{
    m_Timer->stop();
    delete m_Timer;
    m_HBoxLayout->removeWidget(m_MemLabel);
    delete m_MemLabel;
    delete m_HBoxLayout;
}

void cFreeMemWidget::mouseReleaseEvent(QMouseEvent *event)
{
    emit sigMouseReleaseEvent();
    QFont ubuntuTimeFont("Ubuntu", 15, QFont::Bold);
    setFont(ubuntuTimeFont);
    QWidget::mouseReleaseEvent(event);
}

void cFreeMemWidget::mousePressEvent(QMouseEvent *event)
{
    QFont ubuntuTimeFont("Ubuntu", 12, QFont::Bold);
    setFont(ubuntuTimeFont);
    QWidget::mouseReleaseEvent(event);
}

void cFreeMemWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void cFreeMemWidget::onTimerTimeout()
{
    if (m_Timer != nullptr) {
        QStorageInfo storage = QStorageInfo::root();
        long available = storage.bytesAvailable()/1000/1000;
        int storageGB = available/1000;

        if (m_MemLabel != nullptr) {
            if (storageGB > 1)
                m_MemLabel->setText(QString::number(storageGB) + " GB");
            else
                m_MemLabel->setText(QString::number(available) + " MB");
        }
        qDebug() << storage.rootPath();
        if (storage.isReadOnly())
            qDebug() << "cFreeMemWidget::onTimerTimeout-isReadOnly:" << storage.isReadOnly();

        qDebug() << "cFreeMemWidget::onTimerTimeout-name:" << storage.name();
        qDebug() << "cFreeMemWidget::onTimerTimeout-fileSystemType:" << storage.fileSystemType();
        qDebug() << "cFreeMemWidget::onTimerTimeout-size:" << storage.bytesTotal()/1000/1000 << "MB";
        qDebug() << "cFreeMemWidget::onTimerTimeout-availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
    }
}
