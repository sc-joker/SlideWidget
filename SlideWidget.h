#ifndef SLIDEWIDGET_H
#define SLIDEWIDGET_H

#include <QWidget>
#include <QList>
#include <QTimer>

namespace Ui {
class SlideWidget;
}

class SlideWidget : public QWidget
{
	Q_OBJECT
#define SLIDE_TIMEOUT		15	//每20ms滑动一下
#define SLIDE_STEP			20	//每滑动一下步进10个像素

	enum _SLIDE_EFFECT
	{
		_NO_SLIDE,
		_LEFT_SLIDE,
		_RIGHT_SLIDE
	};

public:
	explicit SlideWidget(QWidget *parent = 0);
	~SlideWidget();

	//往滑动窗口添加窗口
	void						appendWidget(QWidget *w);

	//从滑动窗口去掉窗口
	void						removeWidget(QWidget *w);
	void						removeWidget(int index);

private:
	void						showWidgetByIndex(int index, _SLIDE_EFFECT effect);
	void						startSlide(_SLIDE_EFFECT effect);
	void						stopSlide();

	void						leftShiftPixel(int pix);
	void						rigthShiftPixel(int pix);

public slots:
	void						slideSlot();

	void						showPrevWidget();
	void						showNextWidget();

private:
	Ui::SlideWidget *ui;

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *e);

private:
	QList<QWidget *>			m_widgetList;
	int							m_index;
	QTimer						*m_slideTimer;

	_SLIDE_EFFECT				m_effect;

	bool						m_startMoveFlag;
	int							m_prevMouseX;
};

#endif // SLIDEWIDGET_H
