#include "cRowErrorWidget.h"
#include <QDebug>

cRowErrorWidget::cRowErrorWidget(QWidget *parent) : QWidget(parent)
{
    m_HBoxLayout = new QHBoxLayout();
    m_CheckBox = new QCheckBox();
    m_LineEdit = new cCustomLineEdit();
    m_LineEdit->setVisible(false);
    m_LineEdit->setAlignment(Qt::AlignCenter);
    m_LineEdit->setText("0");

    m_CheckBox->setStyleSheet("font-size: 20pt;");
    m_HBoxLayout->addSpacing(1);
    m_HBoxLayout->addWidget(m_CheckBox, 0, Qt::AlignLeft);
    m_HBoxLayout->addStretch(500);
    m_HBoxLayout->addWidget(m_LineEdit, 0, Qt::AlignRight);
    m_HBoxLayout->addSpacing(0);
    this->setLayout(m_HBoxLayout);
    connect(m_CheckBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(m_LineEdit, SIGNAL(sigFocusInEvent()), this, SLOT(onLineEdirFocusInEvent()));
    connect(m_LineEdit, SIGNAL(sigFocusOutEvent()), this, SLOT(onLineEditFocusOutEvent()));
    connect(m_LineEdit, SIGNAL(sigMouseReleaseEvent()), this, SLOT(onLineEditClicked()));
}

cRowErrorWidget::~cRowErrorWidget()
{
    m_HBoxLayout->removeWidget(m_LineEdit);
    m_HBoxLayout->removeWidget(m_CheckBox);
    delete m_CheckBox;
    delete m_LineEdit;
    delete m_HBoxLayout;
}

void cRowErrorWidget::setLineEditText(const QString &text)
{

    bool isconvertOK;
    int number  = text.toInt(&isconvertOK, 10);
    if (!isconvertOK)
    {
        if (QString::compare(text, "") == 0) {
            if (m_LineEdit != nullptr)
                m_LineEdit->clear();
        } else {
        qDebug() << "Invalid input number";
        }

    } else {
        qDebug() << "Append input number: " << number;
    }
    if (m_LineEdit != nullptr) {
        if (QString::compare(m_LineEdit->text(), "0") == 0)
        {
            m_LineEdit->clear();
            m_LineEdit->setText(text);
        }
        else {
            m_LineEdit->setText(m_LineEdit->text() + text);
        }
        m_LineEdit->setFocus();
    }

}

void cRowErrorWidget::setCheckBoxText(const QString &text)
{
    QString tmpText = "";
    if (m_CheckBox != nullptr)
    {
        QStringList setOfWords = text.split(' ');
        if (setOfWords.count() <= 4) {
            m_CheckBox->setText(text);
        } else {
            for (int i = 0; i < setOfWords.count(); i++) {
                if (i > 0 && ( i % 4 == 0)) {
                    tmpText.append("\n" + setOfWords.at(i));
                } else {
                    tmpText.append(setOfWords.at(i) + " ");
                }
            }
            m_CheckBox->setText(tmpText.trimmed());
        }
    }
}

bool cRowErrorWidget::isCheckBoxChecked()
{
    if (m_CheckBox != nullptr)
        return m_CheckBox->isChecked();
    return false;
}

QString cRowErrorWidget::getLineEditText()
{
    if (m_LineEdit != nullptr)
        return m_LineEdit->text();
    return "";
}

void cRowErrorWidget::showLineEdit()
{
    if (m_LineEdit != nullptr) {
        m_LineEdit->setStyleSheet("color: black; background-color: white; border: none;");
        m_LineEdit->setText("1");
        m_LineEdit->selectAll();
        m_LineEdit->setFocus();
        m_LineEdit->setVisible(true);
    }
}


void cRowErrorWidget::setCheckBoxFont(const QFont &font)
{
    if (m_CheckBox != nullptr)
        m_CheckBox->setFont(font);
}

void cRowErrorWidget::setLineEditFont(const QFont &font)
{
    if (m_LineEdit != nullptr)
        m_LineEdit->setFont(font);
}

void cRowErrorWidget::hideLineEdit()
{
    if (m_LineEdit != nullptr) {
        m_LineEdit->setStyleSheet("color: white; background-color: white; border: none;");
        m_LineEdit->setVisible(false);
    }
}

void cRowErrorWidget::setCheckBoxSize(QSize size)
{
    if (m_CheckBox != nullptr) {
        m_CheckBox->setStyleSheet(QString("QCheckBox::indicator { width:%1px; height: %2px;};").arg(size.width()).arg(size.height()));
        m_LineEdit->setFixedSize(QSize(size.width() + 10, size.height()));
    }
}

void cRowErrorWidget::clearCheckBoxChecked()
{
    m_CheckBox->setCheckState(Qt::Unchecked);

    onCheckBoxStateChanged(Qt::Unchecked);
}

void cRowErrorWidget::onCheckBoxStateChanged(int status)
{
    switch (status) {
        case Qt::Unchecked:
            hideLineEdit();
            break;
        case Qt::Checked:
            showLineEdit();
            emit clicked();
            break;
        case Qt::PartiallyChecked:
            qDebug() << "Future used";
            break;
        default:
        break;
//            qWarning() << "Unknow status of checkbox";
    }
}

void cRowErrorWidget::onLineEditClicked()
{
    if (m_LineEdit != nullptr)
    {
        m_LineEdit->selectAll();
        emit clicked();
    }
}

void cRowErrorWidget::onLineEdirFocusInEvent()
{
    if (m_LineEdit != nullptr) {
        m_LineEdit->setStyleSheet("color: white; background-color: blue; border: none;");
    }
}

void cRowErrorWidget::onLineEditFocusOutEvent()
{
    if (m_LineEdit != nullptr) {
        m_LineEdit->setStyleSheet("color: black; background-color: rgb(255,255,255); border: none;");
    }
}

