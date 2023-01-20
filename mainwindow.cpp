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

QString MainWindow::encryptxor(QString test,QString key)
{
//code taken from silve_smuggler xor utility
    // Read the file into a QByteArray
    QByteArray clearBlob = test.toLatin1();
    qDebug() << "File contains " << clearBlob.length() << " bytes.";

    // Convert the key into a QByteArray
    QByteArray keyBlob = key.toLatin1();

    qDebug() << "Key is " << keyBlob.length() << " bytes.";

    // XOR the file with the key
    QByteArray cryptBlob;
    // Give ourselves the memory before hand, so we don't call append() in the loop. It's slow.
    cryptBlob.resize(clearBlob.size());
    int blobPos;
    for (blobPos = 0; blobPos < clearBlob.length(); blobPos++)
    {
        // XOR each byte of the clearblob with the key, wrapping the key position around
        cryptBlob[blobPos] = clearBlob[blobPos] ^ keyBlob[blobPos % keyBlob.size()];
        //qDebug() << blobPos << ": " << clearBlob[blobPos] << " XOR " << keyBlob[blobPos % keyBlob.size()] << " (@ " << blobPos % keyBlob.size() << ")";
    }

    // Base64Encode the resulting encrypted QByteArray
    QString output = cryptBlob.toBase64(QByteArray::Base64Encoding | QByteArray::KeepTrailingEquals);

return output;
}

QString MainWindow::decryptxor(QString string,QString key)
{
    // Read the input into a QByteArray (integral Base64Decode)
    QByteArray cryptBlob;
    cryptBlob = QByteArray::fromBase64(string.toUtf8(), QByteArray::Base64Encoding | QByteArray::KeepTrailingEquals);
    qDebug() << "Input contains " << cryptBlob.length() << " bytes.";

    // Convert the key into a QByteArray
    QByteArray keyBlob = key.toLatin1();

    // XOR the key with the decoded input
    QByteArray clearBlob;
    // Give ourselves the memory before hand, so we don't call append() in the loop. It's slow.
    clearBlob.resize(cryptBlob.size());
    int blobPos;
    for (blobPos = 0; blobPos < cryptBlob.length(); blobPos++)
    {
        // XOR each byte of the clearblob with the key, wrapping the key position around
        clearBlob[blobPos] = cryptBlob[blobPos] ^ keyBlob[blobPos % keyBlob.size()];
        //qDebug() << blobPos << ": " << cryptBlob[blobPos] << " XOR " << keyBlob[blobPos % keyBlob.size()] << " (@ " << blobPos % keyBlob.size() << ")";
    }

    QTextCodec *codec = QTextCodec::codecForName("KOI8-R");

    return codec->toUnicode(clearBlob);
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
//        ui->serialtxt->setText(nums.at(0).toLatin1());//email/registercode
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

