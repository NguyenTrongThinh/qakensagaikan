#include "cCameraControllerWidg.h"
#include <QStyleOption>
#include <QPainter>

cCameraControllerWidget::cCameraControllerWidget(QWidget *parent) : QWidget(parent)
{
    m_SaveButton = new cSaveButton();
    m_TrashButton = new cTrashButton();
    connect(m_SaveButton, SIGNAL(sigMouseReleaseEvent()), this, SIGNAL(sigSaveButtonClicked()));
    connect(m_TrashButton, SIGNAL(sigMouseReleaseEvent()), this, SIGNAL(sigTrashButtonClicked()));

    m_VBoxLayout = new QVBoxLayout();
    m_VBoxLayout->setMargin(0);
    m_VBoxLayout->setSpacing(0);

    m_VBoxLayout->addStretch();
    m_VBoxLayout->addWidget(m_TrashButton);
    m_VBoxLayout->addStretch();
    m_VBoxLayout->addWidget(m_SaveButton);
    m_VBoxLayout->addStretch();

    setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(75, 73, 73); border: 4px; border-radius: 7px;");
    this->setLayout(m_VBoxLayout);

}

cCameraControllerWidget::~cCameraControllerWidget()
{
    disconnect(m_SaveButton, SIGNAL(sigMouseReleaseEvent()), this, SIGNAL(sigSaveButtonClicked()));
    disconnect(m_TrashButton, SIGNAL(sigMouseReleaseEvent()), this, SIGNAL(sigTrashButtonClicked()));
    m_VBoxLayout->removeWidget(m_SaveButton);
    m_VBoxLayout->removeWidget(m_SaveButton);
    delete m_SaveButton;
    delete m_SaveButton;
    delete m_VBoxLayout;
}

void cCameraControllerWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
