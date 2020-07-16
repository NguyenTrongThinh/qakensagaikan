#include "wCameraStream.h"
#include "ui_wCameraStream.h"

wCameraStream::wCameraStream(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wCameraStream)
{
    ui->setupUi(this);
    ui->vlCamera->setSpacing(0);
    ui->vlCamera->setMargin(0);
    m_CameraViewer = new cCameraViewer();
    m_CameraViewer->setText("");
    connect(m_CameraViewer, SIGNAL(sigMouseReleaseEvent()), this, SIGNAL(sigCaptureImage()));
    ui->vlCamera->addWidget(m_CameraViewer);
    m_CameraControllerWidget = new cCameraControllerWidget();
    m_CameraControllerWidget->setFixedWidth(81);
    connect(m_CameraControllerWidget, SIGNAL(sigTrashButtonClicked()), this, SIGNAL(sigStreamImage()));
    connect(m_CameraControllerWidget, SIGNAL(sigSaveButtonClicked()), this, SIGNAL(sigSaveImage()));
    ui->hzLayout->addWidget(m_CameraControllerWidget);
    setImage(QPixmap(":/images/resources/camera_blank.png"));
}

wCameraStream::~wCameraStream()
{
    delete m_CameraViewer;
    delete m_CameraControllerWidget;
    delete ui;
}

void wCameraStream::setImage(QPixmap pixmap)
{
    if (m_CameraViewer != nullptr) {
        m_CameraViewer->setPixmap(pixmap);
        m_CameraViewer->setScaledContents( true );
        m_CameraViewer->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    }
}
