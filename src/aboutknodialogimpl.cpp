#include "aboutknodialogimpl.h"
//
aboutKNODialogImpl::aboutKNODialogImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	label->setText(
	"<html><body>"
	"<center>" + tr("KNO - Krestiki-NOliki") + "<br>"
	+ tr("Version") + ": 1.0<br>23.03.2008<hr>"
	+ tr("Maxim") + " \"amv\" " + tr("Akinin")+ "<br><a href=\"mailto:amv_post@mail.ru?subject=About KNO&Body=KNO\">amv_post@mail.ru</a><br>"
	"ICQ - 410976216<hr>"
	"<a href=\"http://masimo-amv.livejournal.com\">masimo-amv.livejournal.com</a><br>"
	"<a href=\"http://group642.by.ru\">group642.by.ru</a></center>"
	"</body></html>"
	);
}
//

void aboutKNODialogImpl::on_aboutQTButton_clicked()
{
	QMessageBox::aboutQt(this,tr("About QT"));
}
