#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>

#include <QCryptographicHash>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::rot13( const QString & input,int offset )
{//https://doc.qt.io/archives/3.3/rot-example.html
    QString r = input;
    int i = r.length();
    while( i-- ) {
        if ( r[i] >= QChar('A') && r[i] <= QChar('M') ||
             r[i] >= QChar('a') && r[i] <= QChar('m') )
            r[i] = (char)((int)QChar(r[i]).toLatin1() + offset);
        else if  ( r[i] >= QChar('N') && r[i] <= QChar('Z') ||
                   r[i] >= QChar('n') && r[i] <= QChar('z') )
            r[i] = (char)((int)QChar(r[i]).toLatin1() - offset);
    }
    return r;
}


QByteArray fileChecksum(const QString &fileName);

void zip(QString filename , QString zip_filename)
{
   QFile infile(filename);
   QFile outfile(zip_filename);
   infile.open(QIODevice::ReadOnly);
   outfile.open(QIODevice::WriteOnly);
   QByteArray uncompressed_data = infile.readAll();
   QByteArray compressed_data = qCompress(uncompressed_data, 9);
   outfile.write(compressed_data);
   infile.close();
   outfile.close();
}

void unZip(QString zip_filename , QString filename)
{
   QFile infile(zip_filename);
   QFile outfile(filename);
   infile.open(QIODevice::ReadOnly);
   outfile.open(QIODevice::WriteOnly);
   QByteArray uncompressed_data = infile.readAll();
   QByteArray compressed_data = qUncompress(uncompressed_data);
   outfile.write(compressed_data);
   infile.close();
   outfile.close();
}

QByteArray fileChecksum(const QString &fileName)
{
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Md5);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}


void MainWindow::on_generatebtn_clicked()
{
 // ui->serialtxt_3->setText(  rot13(ui->serialtxt_2->text() + ui->serialtxt->text(), ui->offset->text().toInt() ));
    QString test = ui->serialtxt_2->text() + ui->serialtxt->text();
 QByteArray uncompressed_data = test.toUtf8() ;
  //QByteArray compressed_data =
 ui->serialtxt_3->setText( qCompress(uncompressed_data, 9).toHex());


}


void MainWindow::on_decodebtn_clicked()
{
    QString test =ui->serialtxt_3->text();
 QByteArray uncompressed_data = QByteArray::fromHex(test.toUtf8()) ;
   QString test2 = qUncompress(uncompressed_data);
  ui->decodetxt->setText( test2.toLatin1() );


//    bool settingsexists=false;
//    QFile Fout("settings.txt");    if(Fout.exists())    {       settingsexists=true;    }    Fout.close();
//    if (settingsexists){
//        QString searchString(":");
//        QFile MyFile("settings.txt");
//        MyFile.open(QIODevice::ReadWrite);
//        QTextStream in (&MyFile);
//        QString line;
//        QStringList list;
//        QStringList nums;
//        do {
//            line = in.readLine();
//            searchString=":";
//            if (line.contains(searchString)) { //, Qt::CaseSensitive
//                QRegExp rx("[:]");// match a comma or a space
//                list = line.split(rx);
//                nums.append(list.at(1).toLatin1());
//            }
//        } while (!line.isNull());
//        ui->lattxt->setText(nums.at(0).toLatin1());
//        ui->longtxt->setText(nums.at(1).toLatin1());
//        ui->tztxt->setText(nums.at(2).toLatin1());
//        ui->serialtxt->setText(nums.at(3).toLatin1());//email/registercode
//    }

//    QString test =ui->serialtxt->text(); ui->serialtxt->text();
// QByteArray uncompressed_data = QByteArray::fromHex(test.toUtf8()) ;
//   QString test2 = qUncompress(uncompressed_data);
// // ui->decodetxt->setText( test2.toLatin1() );


//    //split line and verify serial to get email.
//    QString line;
//    QStringList list;
//    QStringList nums;
//    line =  test2.toLatin1();
//        QString searchString("|");
//    if (line.contains(searchString)) { //, Qt::CaseSensitive
//        QRegExp rx("[|]");// match a comma or a space
//        list = line.split(rx);
//        qDebug() << list.at(0).toLatin1();
//        if (list.at(0).toLatin1() == "testhre"){
//        ui->lblregister->setText("Registered To: " + list.at(1).toLatin1());
//        } else{
//               ui->lblregister->setText("Registered To: unregistered");
//        }
//        }
        
        
}

