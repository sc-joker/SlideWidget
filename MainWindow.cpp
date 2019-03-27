#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_index(0), m_flag(false), m_direction(LEFT)
{
	ui->setupUi(this);

	m_imageLabel = new QLabel(ui->pictureWidget);
	m_imageLabel->setGeometry(0, 0, 420, 300);

	Q_INIT_RESOURCE(pictures);

#ifdef ARM
	this->setWindowFlags(Qt::FramelessWindowHint);      //设置窗口标志为无边框
#endif

	for (int i = 0; i < 3; i ++)
	{
		/*
		QString path = QString(":/pictures/%1.png").arg(i+1);
		qDebug() << "path: " << path;
		QImage *image = new QImage(path);
		m_imageList.append(image);
		*/

		QLabel *label = new QLabel(ui->pictureWidget);
		m_imageLabel->setGeometry(0, 0, 420, 300);
		QImage image(QString(":/pictures/%1.png").arg(i + 1));
		label->setPixmap(QPixmap::fromImage(image));

		m_labelList.append(label);
	}

	showPicture(m_labelList.first());

	connect(ui->leftButton, SIGNAL(clicked(bool)), this, SLOT(prev()));
	connect(ui->rigthButton, SIGNAL(clicked(bool)), this, SLOT(next()));

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(movePicture()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::showPicture(QLabel *image)
{
	for (int i = 0; i < m_labelList.size(); i++)
	{
		if (image == m_labelList.at(i))
			image->move(0, 0);
		else
			m_labelList.at(i)->move(420, 0);
	}
}

void MainWindow::prev()
{
	if (m_flag)
		return;

	m_flag = true;
	m_direction = LEFT;
	if (0 == m_index)
		m_index = 2;
	else
		m_index --;

//	showPicture(m_imageList.at(m_index));
//	m_imageLabel->move(0, 0);

	m_timer.start(20);
}

void MainWindow::next()
{
	if (m_flag)
		return;

	m_flag = true;
	m_direction = RIGHT;
	m_index ++;
	m_index %= 3;

//	showPicture(m_imageList.at(m_index));

	m_timer.start(20);
}

void MainWindow::movePicture()
{
	QLabel *prevLabel;
	if (RIGHT == m_direction)
		prevLabel = m_labelList.at(m_index > 0 ? (m_index - 1) : 2);
	else
		prevLabel = m_labelList.at((m_index == 2) ? 0 : (m_index + 1));

	int prevLabelX = prevLabel->x();
	QLabel *curLabel = m_labelList.at(m_index);

	if (RIGHT == m_direction)
	{
		if (prevLabelX < 420)
		{
			prevLabelX += 10;
			int curLabelX = -420 + prevLabelX;
			prevLabel->move(prevLabelX, 0);
			curLabel->move(curLabelX, 0);
		} else
		{
			m_timer.stop();
			m_flag = false;
		}
	} else
	{
		if (prevLabelX > -420)
		{
			prevLabelX -= 10;
			int curLabelX = 420 - (-prevLabelX);
			prevLabel->move(prevLabelX, 0);
			curLabel->move(curLabelX, 0);
		} else
		{
			m_timer.stop();
			m_flag = false;
		}
	}
}
