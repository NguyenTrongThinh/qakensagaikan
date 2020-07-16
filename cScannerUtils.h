#ifndef CSCANNERUTILS_H
#define CSCANNERUTILS_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class cScannerUtils : public QObject
{
    Q_OBJECT
public:
    explicit cScannerUtils(QObject *parent = nullptr);
    ~cScannerUtils();
    void abort();
    cScannerUtils (const cScannerUtils &);
    cScannerUtils& operator=(const cScannerUtils & );

    static cScannerUtils *instance();
    static void drop();

private:
    QMutex m_Mutex;
    bool m_Abort;
    static cScannerUtils *m_Instance;
signals:
    void finished();
    void onDataDetected(const QString &data);
public slots:
    void mainLoop();
};

#endif // CSCANNERUTILS_H
