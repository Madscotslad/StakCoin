#include "marketbrowser.h"
#include "ui_marketbrowser.h"
#include "main.h"
#include "wallet.h"
#include "base58.h"
#include "clientmodel.h"
#include "bitcoinrpc.h"
#include <QDesktopServices>

#include <sstream>
#include <string>

using namespace json_spirit;

const QString kBaseUrl = "http://denarius.io/STAKusd.php";
const QString kBaseUrl1 = "http://blockchain.info/tobtc?currency=USD&value=1";
const QString kBaseUrl2 = "http://denarius.io/STAKmc.php";
const QString kBaseUrl3 = "http://denarius.io/STAKbtc.php";

QString bitcoinp = "";
QString denariusp = "";
QString STAKmcp = "";
QString STAKbtcp = "";
double bitcoin2;
double denarius2;
double STAKmc2;
double STAKbtc2;
QString bitcoing;
QString STAKmarket;
QString dollarg;
int mode=1;
int o = 0;


MarketBrowser::MarketBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarketBrowser)
{
    ui->setupUi(this);
    setFixedSize(400, 420);


requests();
QObject::connect(&m_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponse(QNetworkReply*)));
connect(ui->startButton, SIGNAL(pressed()), this, SLOT( requests()));
connect(ui->egal, SIGNAL(pressed()), this, SLOT( update()));

}

void MarketBrowser::update()
{
    QString temps = ui->egals->text();
    double totald = dollarg.toDouble() * temps.toDouble();
    double totaldq = bitcoing.toDouble() * temps.toDouble();
    ui->egald->setText("$ "+QString::number(totald)+" USD or "+QString::number(totaldq)+" BTC");

}

void MarketBrowser::requests()
{
	getRequest(kBaseUrl);
    getRequest(kBaseUrl1);
	getRequest(kBaseUrl2);
	getRequest(kBaseUrl3);
}

void MarketBrowser::getRequest( const QString &urlString )
{
    QUrl url ( urlString );
    QNetworkRequest req ( url );
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    m_nam.get(req);
}

void MarketBrowser::parseNetworkResponse(QNetworkReply *finished )
{

    QUrl what = finished->url();

    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        emit networkError( finished->error() );
        return;
    }
	
if (what == kBaseUrl) // Denarius Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString denarius = finished->readAll();
    denarius2 = (denarius.toDouble());
    denarius = QString::number(denarius2, 'f', 2);
	
    if(denarius > denariusp)
    {
        ui->denarius->setText("<font color=\"yellow\">$" + denarius + "</font>");
    } else if (denarius < denariusp) {
        ui->denarius->setText("<font color=\"red\">$" + denarius + "</font>");
        } else {
    ui->denarius->setText("$"+denarius+" USD");
    }

    denariusp = denarius;
	dollarg = denarius;
}

if (what == kBaseUrl1) // Bitcoin Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString bitcoin = finished->readAll();
    bitcoin2 = (1 / bitcoin.toDouble());
    bitcoin = QString::number(bitcoin2, 'f', 2);
    if(bitcoin > bitcoinp)
    {
        ui->bitcoin->setText("<font color=\"yellow\">$" + bitcoin + " USD</font>");
    } else if (bitcoin < bitcoinp) {
        ui->bitcoin->setText("<font color=\"red\">$" + bitcoin + " USD</font>");
        } else {
    ui->bitcoin->setText("$"+bitcoin+" USD");
    }

    bitcoinp = bitcoin;
}

if (what == kBaseUrl2) // Denarius Market Cap
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString STAKmc = finished->readAll();
    STAKmc2 = (STAKmc.toDouble());
    STAKmc = QString::number(STAKmc2, 'f', 2);
	
    if(STAKmc > STAKmcp)
    {
        ui->STAKmc->setText("<font color=\"yellow\">$" + STAKmc + "</font>");
    } else if (STAKmc < STAKmcp) {
        ui->STAKmc->setText("<font color=\"red\">$" + STAKmc + "</font>");
        } else {
    ui->STAKmc->setText("$"+STAKmc+" USD");
    }

    STAKmcp = STAKmc;
	STAKmarket = STAKmc;
}

if (what == kBaseUrl3) // Denarius BTC Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString STAKbtc = finished->readAll();
    STAKbtc2 = (STAKbtc.toDouble());
    STAKbtc = QString::number(STAKbtc2, 'f', 8);
	
    if(STAKbtc > STAKbtcp)
    {
        ui->STAKbtc->setText("<font color=\"yellow\">" + STAKbtc + " BTC</font>");
    } else if (STAKbtc < STAKbtcp) {
        ui->STAKbtc->setText("<font color=\"red\">" + STAKbtc + " BTC</font>");
        } else {
    ui->STAKbtc->setText(STAKbtc+" BTC");
    }

    STAKbtcp = STAKbtc;
	bitcoing = STAKbtc;
}

finished->deleteLater();
}


void MarketBrowser::setModel(ClientModel *model)
{
    this->model = model;
}

MarketBrowser::~MarketBrowser()
{
    delete ui;
}
