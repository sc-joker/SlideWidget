#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QLabel>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	enum _DIRECTION {
		LEFT,
		RIGHT
	};

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void					showPicture(QLabel *image);

public slots:
	void					prev();
	void					next();

	void					movePicture();

private:
	Ui::MainWindow *ui;

	QLabel					*m_imageLabel;
	quint32					m_index;
	QList<QLabel *>			m_labelList;
	QTimer					m_timer;
	bool					m_flag;
	_DIRECTION				m_direction;
};

#endif // MAINWINDOW_H
