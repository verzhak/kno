#ifndef ABOUTKNODIALOGIMPL_H
#define ABOUTKNODIALOGIMPL_H
//
#include <QDialog>
#include <QMessageBox>
#include "ui_aboutKNO.h"
//
class aboutKNODialogImpl : public QDialog, public Ui::aboutKNODialog
{
Q_OBJECT
public:
	aboutKNODialogImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
private slots:
	void on_aboutQTButton_clicked();
};
#endif





