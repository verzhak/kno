#include "knoFirstImpl.h"
//
dFirstImpl::dFirstImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	ipEdit->setValidator(new QRegExpValidator(QRegExp("[0-2][0-5][0-9]\\.[0-2][0-5][0-9]\\.[0-2][0-5][0-9]\\.[0-2][0-5][0-9]"),this));
	portEdit->setValidator(new QRegExpValidator(QRegExp("[0-5]{5}"),this));
}
//

void dFirstImpl::on_knoAbout_clicked()
{
	aboutKNODialogImpl *aboutD=new aboutKNODialogImpl;
	aboutD->setModal(true);
	aboutD->exec();
	delete aboutD;
}


void dFirstImpl::on_clientButton_toggled(bool checked)
{
	ipEdit->setEnabled(checked);
}

void dFirstImpl::on_joinButton_clicked()
{
	if(nickEdit->text()!="")
	{
		knoGameImpl *gameDialog=new knoGameImpl(nickEdit->text(),serverButton->isChecked(),ipEdit->text(),portEdit->text(),this);
		gameDialog->exec();
		delete gameDialog;
	}
	else
		QMessageBox::information(this,tr("KNO"),"----->"+tr("Enter nick")+"<-----");
}
