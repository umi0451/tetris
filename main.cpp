#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QWidget>
#include <QtDebug>

#include "main.h"
#include "tetris.h"

MainWindow::MainWindow(QWidget * parent)
	: QWidget(parent), tetris(QSize(10, 15))
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	timer.start(500);
}


void MainWindow::onTimer()
{
	int oldLevel = tetris.level();

	if(!tetris.tick()) {
		timer.stop();
		QMessageBox::information(this, tr("Fail"), tr("Game over"));
		close();
	}

	if(tetris.level() != oldLevel) {
		timer.setInterval(0.8 * timer.interval());
	}

	update();
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
	switch(event->key()) {
		case Qt::Key_Up   : tetris.rotate(); break;
		case Qt::Key_Down : tetris.drop();   break;
		case Qt::Key_Left : tetris.left();   break;
		case Qt::Key_Right: tetris.right();  break;
		case Qt::Key_Q    : close();         break;
		case Qt::Key_P: {
			if(timer.isActive()) {
				timer.stop();
			} else {
				timer.start();
			}
			break;
		}
		default: QWidget::keyPressEvent(event);
	}
	update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
	int cellWidth = qMin(width() / tetris.cupSize().width(),
			height() / tetris.cupSize().height());

	QPainter painter(this);
	painter.fillRect(rect(), Qt::black);

	painter.setPen(Qt::white);
	painter.setBrush(Qt::black);
	painter.drawRect(0, 0, cellWidth * tetris.cupSize().width(),
			cellWidth * tetris.cupSize().height());

	painter.setPen(Qt::black);
	painter.setBrush(Qt::white);
	QRect cell(0, 0, cellWidth, cellWidth);
	for(int x = 0; x < tetris.cupSize().width(); ++x) {
		for(int y = 0; y < tetris.cupSize().height(); ++y) {
			cell.moveTopLeft(QPoint(x * cellWidth, y * cellWidth));
			if(tetris.cell(QPoint(x, y))) {
				painter.drawRect(cell);
			}
		}
	}

	painter.setPen(Qt::white);
	painter.setBrush(Qt::black);

	painter.drawText(cell.right() + cell.width(), cell.height(),
			tr("Level: %1").arg(tetris.level()));

	if(!timer.isActive()) {
		painter.drawText(cell.right() + cell.width(), cell.height() * 2, tr("Paused"));
	}
}



int main(int argc, char ** argv)
{
	qsrand(time(NULL));
	QApplication app(argc, argv);
	MainWindow wnd;
	wnd.show();
	return app.exec();
}