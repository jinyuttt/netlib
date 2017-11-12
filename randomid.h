#ifndef RANDOMID_H
#define RANDOMID_H



#include <QTime>
class RandomStaticID
{
public:
    RandomStaticID() {}

    static int getSeqNo()
    {
        int randn;
        QTime time=QTime::currentTime();
        qsrand(time.msec()*qrand()*qrand()*qrand()*qrand()*qrand()*qrand());
          randn   = qrand()%nImg;
          return randn;
    }

private:
    static int nImg;
};
int RandomStaticID::nImg=85;
#endif // RANDOMID_H
