#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTime>
#include <iostream>
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;

    parser.process(a);

    QStringList args = parser.positionalArguments();

    for (int s = 0; s < 20; s++, QThread::sleep(1)) {
//        QString tmp = args.at(s % args.length());
//        std::cout << QString("%1 %2").arg(QTime::currentTime().toString(), tmp).toStdString() << std::endl;
        std::cout << QTime::currentTime().toString().toStdString() << std::endl;
        std::cerr << args.at(s % args.length()).toStdString() << std::endl;
    }
    QCoreApplication::exit(0);
    std::cout << "EXIT" << std::endl;
    return 0;
//    return a.exec();

}
