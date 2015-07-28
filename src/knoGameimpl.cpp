#include "knoGameimpl.h"
//
#define SERVER_MARK (is_server_krest ? "X" : "0")
#define NOT_SERVER_MARK (!is_server_krest ? "X" : "0")

knoGameImpl::knoGameImpl(QString tNick , bool tis_server, QString tServerAddress, QString tServerPort, QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	nick=tNick;
	is_server=tis_server;
	is_con=false;
	serverAddress=tServerAddress;
	serverPort=tServerPort;
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
	chatEdit->append("--->"+nick+"<---");
	if(is_server)
	{
		server = new QTcpServer(this);
		if (!server->listen(QHostAddress::Any,serverPort.toInt()))
		{
			QMessageBox::critical(this,tr("KNO"),tr("Unable to start the server: %1.").arg(server->errorString()));
			close();
			return;
		}
		connect(server, SIGNAL(newConnection()), this, SLOT(server_newConnection()));
		chatEdit->append("--->" + tr("server address (you): ")+serverAddress+":"+serverPort+"<---");
		newGameButton->setEnabled(true);
		is_server_krest=rand()%2;
		is_server_next_turn=rand()%2;
	}
	else
	{
		con = new QTcpSocket(this);
		in = new QTextStream(con);
		con->connectToHost(serverAddress,serverPort.toInt());
		connect(con, SIGNAL(readyRead()), this, SLOT(con_dataGet()));
		connect(con,SIGNAL(connected()),this,SLOT(client_connected()));
		connect(con, SIGNAL(disconnected()),con, SLOT(deleteLater()));
		connect(con, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(clientSocketError(QAbstractSocket::SocketError)));
	}
	poleView->update();
}

knoGameImpl::~knoGameImpl()
{
	if(is_server)
		delete server;
	else
	{
		delete con;
		delete in;
	}
}

void knoGameImpl::server_newConnection()
{
	if(!is_con)
	{
		con=server->nextPendingConnection();
		in = new QTextStream(con);
		connect(con, SIGNAL(readyRead()), this, SLOT(con_dataGet()));
		connect(con, SIGNAL(disconnected()),this, SLOT(server_clientDisconected()));
		is_con=true;
		con->write(("--->"+nick+tr(" connected")+"<---").toUtf8()+"\n");
		on_newGameButton_clicked();
	}
}

void knoGameImpl::client_connected()
{
	con->write(("--->"+nick+tr(" connected")+"<---").toUtf8()+"\n");
}

void knoGameImpl::server_clientDisconected()
{
	con->deleteLater();
	is_con=false;
	youAreEdit->setText("");
	nextTurnEdit->setText("");
	chatEdit->append("--->" + tr("Opponent disconnected") + "<---");
	poleView->exitGame();
}

void knoGameImpl::clientSocketError(QAbstractSocket::SocketError error)
{
	switch (error)
	{
		case QAbstractSocket::RemoteHostClosedError:
		{
			chatEdit->append("--->"+tr("Server close connection")+"<---");
			QMessageBox::information(this, tr("KNO"),tr("Server close connection"));
			break;
		}
		case QAbstractSocket::HostNotFoundError:
		{
			QMessageBox::information(this, tr("KNO"),tr("The host was not found. Please check the host name and port settings."));
			break;
		}
		case QAbstractSocket::ConnectionRefusedError:
		{
			QMessageBox::information(this, tr("KNO"),tr("The connection was refused by the peer. Make sure the KNO server is running, and check that the host name and port settings are correct."));
			break;
		}
		default:
		{
			QMessageBox::information(this, tr("KNO"),tr("The following error occurred: %1.").arg(con->errorString()));
		}
	}
	this->done(1);
}

void knoGameImpl::con_dataGet()
{
	//Получение данных
	QString read_string;
	while(!in->atEnd())
	{
		read_string=in->readLine();
		read_string.remove('\n');
		if(read_string[0]!='#')
			{
				//TODO cp1251
				chatEdit->append(read_string);
			}
		else
		{
			QString com=read_string.section('#',1,1);
			if(com=="new_game" && !is_server)
			{
				bool is_i_krest,is_i_make_first_turn;
				youAreEdit->setText(read_string.section('#',2,2));
				nextTurnEdit->setText(read_string.section('#',3,3));
				if(nextTurnEdit->text()==youAreEdit->text())
					nextTurnEdit->setEnabled(true);
				else
					nextTurnEdit->setEnabled(false);
				if(youAreEdit->text()=="X")
					is_i_krest=true;
				else
					is_i_krest=false;
				if(nextTurnEdit->text()==youAreEdit->text())
					is_i_make_first_turn=true;
				else
					is_i_make_first_turn=false;
				poleView->newGame(is_i_krest,is_i_make_first_turn);
			}
			else if(com=="new_turn")
			{
				int x,y;
				x=read_string.section('#',2,2).toInt();
				y=read_string.section('#',3,3).toInt();
				nextTurnEdit->setText(nextTurnEdit->text()=="X" ? "0" : "X");
				if(nextTurnEdit->text()==youAreEdit->text())
					nextTurnEdit->setEnabled(true);
				else
					nextTurnEdit->setEnabled(false);
				poleView->makeTurn(false,x,y);
			}
			else
				chatEdit->append(tr("Error get_bad_command: %1.").arg(com));
		}
	}
}
//

void knoGameImpl::on_sayEdit_returnPressed()
{
	//Отправка данных
	QString outS;
	if(!is_server || is_con)
	{
		outS=nick+": "+sayEdit->text();
		//TODO cp1251
		con->write(QString(outS+"\n").toUtf8());
	}
	else
	{
		outS=tr("Error: ")+sayEdit->text();
	}
	chatEdit->append(outS);
	sayEdit->clear();
}

void knoGameImpl::on_newGameButton_clicked()
{
	if(is_con)
	{
		is_server_krest=!is_server_krest;
		is_server_next_turn=!is_server_next_turn;
		youAreEdit->setText(SERVER_MARK);
		nextTurnEdit->setText(is_server_next_turn ? SERVER_MARK : NOT_SERVER_MARK);
		if(nextTurnEdit->text()==youAreEdit->text())
			nextTurnEdit->setEnabled(true);
		else
			nextTurnEdit->setEnabled(false);
		con->write(QString("#new_game#"+QString(NOT_SERVER_MARK)+"#"+nextTurnEdit->text()).toUtf8()+"\n");
		poleView->newGame(is_server_krest,is_server_next_turn);
	}
}

void knoGameImpl::on_poleView_playerNewTurn(int x, int y)
{
	con->write(QString("#new_turn#"+QString().setNum(x)+"#"+QString().setNum(y)).toUtf8()+"\n");
	nextTurnEdit->setText(nextTurnEdit->text()=="X" ? "0" : "X");
	if(nextTurnEdit->text()==youAreEdit->text())
		nextTurnEdit->setEnabled(true);
	else
		nextTurnEdit->setEnabled(false);
}

void knoGameImpl::on_poleView_weGetWinner(bool is_i)
{
	if(is_i)
		youEdit->setText(QString().setNum(youEdit->text().toInt()+1));
	else
		enemyEdit->setText(QString().setNum(enemyEdit->text().toInt()+1));
	QMessageBox::information(this,tr("We get winner!"),"############################\n"+tr("And winner is: ")+QString(is_i ? tr("you") : tr("opponent")) + "\n############################");
}

void knoGameImpl::on_poleView_weGetNicya()
{
	QMessageBox::information(this,tr("We get nicya!"),"############################\n"+tr("Nicya")+"\n############################");
}

void knoGameImpl::on_helpButton_clicked()
{
	//TODO
	QMessageBox::information(this,tr("You need some help?"),tr("Thinking... LOL. I think... I don't think. Play!"));
}

void knoGameImpl::on_aboutGameButton_clicked()
{
	aboutKNODialogImpl *aboutD=new aboutKNODialogImpl;
	aboutD->setModal(true);
	aboutD->exec();
	delete aboutD;
}
