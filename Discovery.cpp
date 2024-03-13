#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QNetworkInterface>

QString getNetworkCIDR(QHostAddress ipAddress, QHostAddress subnetMask){
    
    // Convert the IP Address into integer.
    quint32 subnetAddrInt = subnetMask.toIPv4Address();
    
    // Initialize the CIDR notation
    int cidr = 0;
    
    // Count the number of "1" in the subnet mask
    while(subnetAddrInt){
        cidr = cidr + (subnetAddrInt & 1);
        subnetAddrInt = subnetAddrInt >> 1;
    }

    QStringList ipAddrList = ipAddress.toString().split(".");
    ipAddrList[ipAddrList.count() - 1] = "0";
    return ipAddrList.join(".").append("/").append(QString::number(cidr));
}

int startProcess(QString cmdStr){
    QProcess *cmd = new QProcess();
    QObject::connect(
        cmd,
        &QProcess::readyReadStandardOutput,
        [=](){
            qDebug() << "Command execution output received.";
            QString output(cmd->readAllStandardOutput());
            QStringList op = output.split("\n");
            foreach (QString line, op){
                qDebug() << line;
            }
        });
    cmd->start(cmdStr);

    if (!cmd->waitForStarted()){
        qDebug() << "Failed to start command execution";
        return -1;
    }

    if (!cmd->waitForFinished()){
        qDebug() << "Command execution timed out";
        return -1;
    }

    qDebug() << "Command execution completed";

    delete cmd;
    return 0;
}

int main(int argc, char *argv[]){
    

    struct NetworkInterface{
        QString networkCIDR;
        QNetworkAddressEntry networkInfo;
    };
    
    // Iterate through networks.
    for(const QNetworkInterface &interface : QNetworkInterface::allInterfaces()){
        for(QNetworkAddressEntry entry : interface.addressEntries()){
            if( interface.flags() & QNetworkInterface::IsRunning;
                entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                entry.ip() != QHostAddress(QHostAddress::LocalHost) &&
                !entry.ip().isLoopback()
                ){
                
                qDebug() << "IP Address " << entry.ip().toString()
                << "\tNetwork CIDR " << getNetworkCIDR(entry.ip(), entry.netmask());
            }
        }
    }
    
    // QCoreApplication a(argc, argv);
    // return a.exec();
    return 0;
}