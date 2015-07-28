#ifndef KNOGAMEIMPL_H
#define KNOGAMEIMPL_H
//
#include <QDialog>
#include <QVector>
#include <QPair>
#include <QtNetwork>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include "ui_knoGame.h"
#include "aboutknodialogimpl.h"
//
class knoGameImpl : public QDialog, public Ui::knoGameDialog
{
Q_OBJECT
public:
	knoGameImpl(QString tNick , bool tis_server, QString tServerAddress, QString tServerPort, QWidget * parent = 0, Qt::WFlags f = 0 );
	~knoGameImpl();
private:
	QString nick;
	bool is_server;
	bool is_con;
	QString serverAddress;
	QString serverPort;
	QTcpServer *server;
	QTcpSocket *con;
	QTextStream *in;
	bool is_server_krest;
	bool is_server_next_turn;
private slots:
	void on_helpButton_clicked();
	void on_aboutGameButton_clicked();
	void on_poleView_playerNewTurn(int x, int y);
	void on_poleView_weGetWinner(bool is_i);
	void on_poleView_weGetNicya();
	void on_newGameButton_clicked();
	void on_sayEdit_returnPressed();
	void server_newConnection();
	void client_connected();
	void server_clientDisconected();
	void con_dataGet();
	void clientSocketError(QAbstractSocket::SocketError error);
};
#endif






