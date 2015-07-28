#ifndef POLEWIDGET_H
#define POLEWIDGET_H
//
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
//

class poleWidget : public QWidget
{
Q_OBJECT
public:
	poleWidget(QWidget *parent = 0,int width=21,int height=21,int needs_for_win=5);
	~poleWidget();
	void newGame(bool is_i_krest,bool is_i_make_furst_turn);
	void makeTurn(bool is_i,int x,int y);
	int getWidth();
	int getHeight();
	int getNeedsForWin();
	void exitGame();
signals:
	void playerNewTurn(int x,int y);
	void weGetWinner(bool is_i);
	void weGetNicya();
protected:
	void initPole();
	void paintEvent(QPaintEvent *event);
	void mouseReleaseEvent(QMouseEvent * event);
	enum elem
	{
		NONE,KREST,NUL
	}
	**pl;
	elem i_am,opponent;
	bool is_my_turn,is_we_get_winner,is_play;
	int WIDTH;
	int HEIGHT;
	int NEEDS_FOR_WIN;
	struct winner_enum
	{
		elem who;
		enum
		{
			GOR,VERT,NIZN_DIAG,VERHN_DIAG
		} where;
		int x,y;
	} winner;
};
#endif
