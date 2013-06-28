#include "tlasermsg.h"
#include <QDateTime>
#include <iostream>

TLaserMsg::TLaserMsg(QObject *parent) :
    QObject(parent)
{
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MQ_MAXMSGS;
    attr.mq_msgsize = sizeof(message_t);

    mq = mq_open(MQ_FILE, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    if (mq == -1)
            std::cout << "Opening MessageQueue failed" << std::endl;
    else {
            struct mq_attr a;
            mq_getattr(mq, &a);
            std::cout << "MQ: mq_flags: " << a.mq_flags
                      << " mq_maxmsg: " << a.mq_maxmsg
                      << " mq_msgsize: " << a.mq_msgsize
                      << " mq_curmsgs: " << a.mq_curmsgs
                      << std::endl;
    }
}

void TLaserMsg::doWork(void)
{
    unsigned prio;

    message_t msg;

    QDateTime msg_date;

    QString tmp;

    while (1)
    {
       if (mq_receive(mq, (char *)&msg, sizeof(message_t), &prio) != -1)
       {
            msg_date.setTime_t(msg.msg_timestamp.tv_sec);
            switch(msg.msg_id)
            {
                    case WAIT_FOR_TRIGGER:
                                tmp = tr("WAIT_FOR_TRIGGER");
                                break;
                    case PWM_ON:
                                tmp = tr("PWM ON");
                                break;
                    case PWM_OFF:
                                tmp = tr("PWM OFF");
                                break;
                    default:
                                tmp = tr("UNKNOWN ERROR");
                                break;
            }
            emit messageReceived(tmp + " " + msg_date.toString("yyyy-MM-dd hh:mm:ss"));
       }
    }
}
