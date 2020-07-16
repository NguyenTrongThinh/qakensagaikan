#include "cTitleBarWidget.h"
#include <QStyleOption>
#include <QPainter>

cTitleBarWidget::cTitleBarWidget(QWidget *parent) : QWidget(parent)
{
    m_CancalButton = new cCancelButton();
    connect(m_CancalButton, SIGNAL(sigMouseReleaseEvent()), this, SIGNAL(sigCancelButtonClicked()));
    m_ClockWidget = new cClockWidget();
    connect(m_ClockWidget, SIGNAL(onClockWidgetClicked()), this, SIGNAL(sigClockWidgetClicked()));
    m_TitleLabel = new QLabel();
    m_TitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setTitle("Default Text");
    m_WifiWidget = new cWifiWidget();

    connect(m_WifiWidget, SIGNAL(sigWifiWidgetClicked()), this, SIGNAL(sigWifiWidgetClicked()));
    connect(m_WifiWidget, SIGNAL(sigWifiConnected()), this, SIGNAL(sigWifiConnected()));
    connect(m_WifiWidget, SIGNAL(sigWifiDisconnected()), this, SIGNAL(sigWifiDisconnected()));

    m_SyncingWidget = new cSyncingWidget();

    m_SyncingWidget->setSyncDoneStateWithWarning();

    m_HBoxLayout = new QHBoxLayout();

    m_FreeMemWidget = new cFreeMemWidget();

    m_HBoxLayout->addWidget(m_TitleLabel);
    m_HBoxLayout->addStretch();
    m_HBoxLayout->addWidget(m_SyncingWidget);
    m_HBoxLayout->addWidget(m_WifiWidget);
    m_HBoxLayout->addWidget(m_FreeMemWidget);
    m_HBoxLayout->addWidget(m_ClockWidget);
    m_HBoxLayout->addWidget(m_CancalButton);
    m_HBoxLayout->setSpacing(10);
    setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(100, 101, 164); border: 4px; border-radius: 5px;");
    this->setLayout(m_HBoxLayout);

}

cTitleBarWidget::~cTitleBarWidget()
{
    m_HBoxLayout->removeWidget(m_CancalButton);
    m_HBoxLayout->removeWidget(m_TitleLabel);
    m_HBoxLayout->removeWidget(m_WifiWidget);
    m_HBoxLayout->removeWidget(m_ClockWidget);
    m_HBoxLayout->removeWidget(m_FreeMemWidget);
    delete m_FreeMemWidget;
    delete m_CancalButton;
    delete m_TitleLabel;
    delete m_WifiWidget;
    delete m_ClockWidget;
    delete m_HBoxLayout;
}

void cTitleBarWidget::setTitle(QString title)
{
    if (m_TitleLabel != nullptr) {
        QFont ubuntuFont("Ubuntu", 24, QFont::Bold);
        m_TitleLabel->setFont(ubuntuFont);
        m_TitleLabel->setText(title);
    }
}

void cTitleBarWidget::setSyncingState()
{
    if (m_SyncingWidget != nullptr) {
        m_SyncingWidget->setSyncingState();
    }
}

void cTitleBarWidget::setSyncingDoneState()
{
    if (m_SyncingWidget != nullptr) {
        m_SyncingWidget->setSyncDoneState();
    }
}

void cTitleBarWidget::setSyncingDoneStateWithWarn()
{
    if (m_SyncingWidget != nullptr) {
        m_SyncingWidget->setSyncDoneStateWithWarning();
    }
}

void cTitleBarWidget::hideCancelButton()
{
    if (m_CancalButton != nullptr) {
        m_CancalButton->setVisible(false);
    }
}

void cTitleBarWidget::showCancelButton()
{
    if (m_CancalButton != nullptr) {
        m_CancalButton->setVisible(true);
    }
}

void cTitleBarWidget::SetCancelButtonIcon(QString normal, QString clicked)
{
    if (m_CancalButton != nullptr) {
        m_CancalButton->setICon(normal, clicked);
    }
}

void cTitleBarWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
