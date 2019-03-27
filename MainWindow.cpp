#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_index(0)
{
	ui->setupUi(this);

	Q_INIT_RESOURCE(pictures);

#ifdef ARM
	this->setWindowFlags(Qt::FramelessWindowHint);      //设置窗口标志为无边框
#endif

	for (int i = 0; i < 3; i ++)
	{
		QString path = QString(":/pictures/%1.png").arg(i+1);
		qDebug() << "path: " << path;
		QImage *image = new QImage(path);
		m_imageList.append(image);
	}

	showPicture(m_imageList.first());

	connect(ui->leftButton, SIGNAL(clicked(bool)), this, SLOT(prev()));
	connect(ui->rigthButton, SIGNAL(clicked(bool)), this, SLOT(next()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::showPicture(QImage *image)
{
	ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void MainWindow::prev()
{
	if (0 == m_index)
		m_index = 2;
	else
		m_index --;

	showPicture(m_imageList.at(m_index));
}

void MainWindow::next()
{
	m_index ++;
	m_index %= 3;

	showPicture(m_imageList.at(m_index));
}
