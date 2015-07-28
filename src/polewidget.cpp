#include "polewidget.h"
//

poleWidget::poleWidget(QWidget *parent,int width,int height,int needs_for_win) : QWidget(parent)
{
	int x;
	WIDTH=width;
	HEIGHT=height;
	NEEDS_FOR_WIN=needs_for_win;
	pl=new elem*[WIDTH];
	for(x=0;x<WIDTH;x++)
		pl[x]=new elem[HEIGHT];
	setBackgroundRole(QPalette::HighlightedText);
	setAutoFillBackground(true);
	exitGame();
}

poleWidget::~poleWidget()
{
	delete [] pl;
}

void poleWidget::initPole()
{
	int x,y;
	for(x=0;x<WIDTH;x++)
		for(y=0;y<HEIGHT;y++)
			pl[x][y]=NONE;
	update();
}

void poleWidget::exitGame()
{
	initPole();
	winner.who=NONE;
	is_play=false;
	update();
}

void poleWidget::newGame(bool is_i_krest,bool is_i_make_furst_turn)
{
	initPole();
	if(is_i_krest)
	{
		i_am=KREST;
		opponent=NUL;
	}
	else
	{
		i_am=NUL;
		opponent=KREST;
	}
	is_my_turn=is_i_make_furst_turn;
	winner.who=NONE;
	is_play=true;
	update();
}

void poleWidget::makeTurn(bool is_i,int x,int y)
{
	if(winner.who==NONE && is_play)
	{
		if(pl[x][y]==NONE)
		{
			if(is_my_turn && is_i)
			{
				pl[x][y]=i_am;
				is_my_turn=false;
				playerNewTurn(x,y);
			}
			else if(!is_i && !is_my_turn)
			{
				pl[x][y]=opponent;
				is_my_turn=true;
			}
		}
		int tx,ty,scx,scy,sc_kl;
		elem cur;
		bool flag[3];
		sc_kl=0;
		for(tx=0;tx<WIDTH;tx++)
			for(ty=0;ty<HEIGHT;ty++)
			{
				if(pl[tx][ty]!=NONE)
					sc_kl++;
				if(tx<WIDTH-NEEDS_FOR_WIN+1 && pl[tx][ty]!=NONE)
				{
					if(ty<HEIGHT-NEEDS_FOR_WIN+1)
					{
						cur=pl[tx][ty];
						flag[0]=flag[1]=flag[2]=true;
						for(scx=tx+1,scy=ty+1;scx<tx+NEEDS_FOR_WIN && scy<ty+NEEDS_FOR_WIN;scx++,scy++)
						{
							if(cur!=pl[scx][ty])
								flag[0]=false;
							if(cur!=pl[tx][scy])
								flag[1]=false;
							if(cur!=pl[scx][scy])
								flag[2]=false;
							if(!(flag[0] || flag[1] || flag[2]))
								break;
						}
						if(flag[0] || flag[1] || flag[2])
						{
							winner.who=pl[tx][ty];
							if(flag[0])
								winner.where=winner_enum::GOR;
							else if(flag[1])
								winner.where=winner_enum::VERT;
							else if(flag[2])
								winner.where=winner_enum::NIZN_DIAG;
							winner.x=tx;
							winner.y=ty;
						}
					}
					if(ty-NEEDS_FOR_WIN+1>-1 && winner.who==NONE)
					{
						cur=pl[tx][ty];
						flag[0]=true;
						for(scx=tx+1,scy=ty-1;scx<tx+NEEDS_FOR_WIN && scy>ty-NEEDS_FOR_WIN;scx++,scy--)
							if(cur!=pl[scx][scy])
							{
								flag[0]=false;
								break;
							}
						if(flag[0])
						{
							winner.who=pl[tx][ty];
							winner.where=winner_enum::VERHN_DIAG;
							winner.x=tx;
							winner.y=ty;
						}
					}
				}
				if(winner.who!=NONE)
					break;
			}
		if(winner.who!=NONE || sc_kl==WIDTH*HEIGHT)
			is_play=false;
		update();
		if(winner.who!=NONE)
			if(winner.who==i_am)
				weGetWinner(true);
			else
				weGetWinner(false);
		else if(sc_kl==WIDTH*HEIGHT)
			weGetNicya();
	}
}

int poleWidget::getWidth()
{
	return WIDTH;
}

int poleWidget::getHeight()
{
	return HEIGHT;
}

int poleWidget::getNeedsForWin()
{
	return NEEDS_FOR_WIN;
}

void poleWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	int x,y,real_x=0,real_y=0;
	const QPen krest_pen(QBrush(Qt::darkRed),2);
	const QPen nul_pen(QBrush(Qt::darkBlue),2);
	const QPen winner_pen(QBrush(Qt::black),4);
	y=10;
	for(x=10;x<WIDTH*20+20;x+=20)
			painter.drawLine(x,y,x,HEIGHT*20+20-10);
	x=10;
	for(y=10;y<HEIGHT*20+20;y+=20)
		painter.drawLine(x,y,WIDTH*20+20-10,y);
	for(x=0;x<WIDTH;x++)
		for(y=0;y<HEIGHT;y++)
		{
			real_x=10+x*20;
			real_y=10+y*20;
			if(pl[x][y]==KREST)
			{
				painter.setPen(krest_pen);
				painter.drawLine(real_x,real_y,real_x+20,real_y+20);
				painter.drawLine(real_x,real_y+20,real_x+20,real_y);
			}
			else if (pl[x][y]==NUL)
			{
				painter.setPen(nul_pen);
				painter.drawEllipse(real_x,real_y,20,20);
			}
		}
	if(winner.who!=NONE)
	{
		painter.setPen(winner_pen);
		x=winner.x*20+10;
		y=winner.y*20+10;
		if(winner.where==winner_enum::GOR)
		{
			y+=10;
			real_x=x+NEEDS_FOR_WIN*20;
			real_y=y;
		}
		else if(winner.where==winner_enum::VERT)
		{
			x+=10;
			real_x=x;
			real_y=y+NEEDS_FOR_WIN*20;
		}
		else if(winner.where==winner_enum::VERHN_DIAG)
		{
			y+=20;
			real_x=x+NEEDS_FOR_WIN*20;
			real_y=y-NEEDS_FOR_WIN*20;
		}
		else if(winner.where==winner_enum::NIZN_DIAG)
		{
			real_x=x+NEEDS_FOR_WIN*20;
			real_y=y+NEEDS_FOR_WIN*20;
		}
		painter.drawLine(x,y,real_x,real_y);
	}
}

void poleWidget::mouseReleaseEvent(QMouseEvent *event)
{
	int x,y;
	x=(event->x()-10)/20;
	y=(event->y()-10)/20;
	if(x>=0 && x<WIDTH && y>=0 && y<HEIGHT)
		makeTurn(true,x,y);
	this->update();
}
//
