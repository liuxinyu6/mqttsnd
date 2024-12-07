#include <QCoreApplication>
#include "framesend.h"
#include <thread>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
/*
    for(int n = 0; n < 9; n++)
    {
        std::thread f([&]()
            {
                char str[1024]{0};
                sprintf(str, "TOPIC_%d", n);
                int index = -1;
                if(n == 0)
                {
                    index = 0;
                }
                FrameSend fs("127.0.0.1", 1883 + n, "abc", "123", str, index, "D:\\program\\downloada\\2.mp4");
                fs.send();
            }
        );
        f.detach();
    }
*/

    int port = atoi(argv[1]);
    int index = atoi(argv[3]);

    qDebug()<<port<<" "<<argv[2] <<" "<<index;
    FrameSend fs("127.0.0.1", port, "abc", "123", argv[2], index, "D:\\program\\downloada\\2.mp4");
    fs.send();
    return a.exec();
}
