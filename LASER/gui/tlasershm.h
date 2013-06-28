#ifndef TLASERSHM_H
#define TLASERSHM_H

#include <QObject>
#include "../common/control.h"

class TLaserShm : public QObject
{
Q_OBJECT
public:
    explicit TLaserShm(QObject *parent = 0);

signals:

public slots:
    void ustawCzasImpulsu(int);

private:
    int shm_fd;
    volatile shm_content_t * shm_content;
};

#endif // TLASERSHM_H
