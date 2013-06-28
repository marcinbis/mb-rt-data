#ifndef TLASERMSG_H
#define TLASERMSG_H

#include <QObject>
#include <mqueue.h>
#include "../common/control.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class TLaserMsg : public QObject
{
Q_OBJECT
public:
    explicit TLaserMsg(QObject *parent = 0);

signals:
    void messageReceived(QString);

public slots:
    void doWork(void);

private:
        mqd_t mq;
};

#endif // TLASERMSG_H
