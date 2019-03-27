#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void					showPicture(QImage *image);

public slots:
	void					prev();
	void					next();

private:
	Ui::MainWindow *ui;

	quint32					m_index;
	QList<QImage *>			m_imageList;
};

#endif // MAINWINDOW_H
