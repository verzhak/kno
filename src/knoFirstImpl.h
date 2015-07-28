#ifndef KNOFIRSTIMPL_H
#define KNOFIRSTIMPL_H
//
#include <QDialog>
#include <QRegExpValidator>
#include "ui_knoFirst.h"
#include "aboutknodialogimpl.h"
#include "knoGameimpl.h"
//
class dFirstImpl : public QDialog, public Ui::dFirst
{
Q_OBJECT
public:
	dFirstImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
private slots:
	void on_clientButton_toggled(bool checked);
	void on_joinButton_clicked();
	void on_knoAbout_clicked();
};
#endif






