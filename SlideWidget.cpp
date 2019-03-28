#include "SlideWidget.h"
#include "ui_SlideWidget.h"
#include <QMouseEvent>
#include <QDebug>

SlideWidget::SlideWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SlideWidget),
	m_index(-1),
	m_startMoveFlag(false), m_prevMouseX(0)
{
	ui->setupUi(this);

	m_slideTimer = new QTimer();
	connect(m_slideTimer, SIGNAL(timeout()), this, SLOT(slideSlot()));
}

SlideWidget::~SlideWidget()
{
	delete ui;
	if (NULL != m_slideTimer) {
		m_slideTimer->stop();
		delete m_slideTimer;
	}
}

void SlideWidget::appendWidget(QWidget *w)
{
	w->setParent(this);	//要实现滑动的话，就必须是本窗口的子窗口
	w->resize(this->width(), this->height());	//强行改变子窗口的大小
	w->move(width(), 0);	//初始进来都放在本窗口界面外
	w->show();	//显示出来

	m_widgetList.append(w);
	if (m_widgetList.size() == 1)
	{
		showWidgetByIndex(0, _NO_SLIDE);
	}
}

void SlideWidget::removeWidget(QWidget *w)
{
	if (NULL == w)
		return;

	for (int i = 0; i < m_widgetList.size(); i++)
	{
		if (w == m_widgetList.at(i)) {
			m_widgetList.removeAt(i);
			delete w;

			if (m_index >= i) {
				//显示上一个窗口
				m_index --;
				showWidgetByIndex(m_index, _NO_SLIDE);
			}
		}
	}
}

void SlideWidget::removeWidget(int index)
{
	if (index < 0 || index >= m_widgetList.size())
		return;

	QWidget *w = m_widgetList.at(index);
	removeWidget(w);
}

void SlideWidget::showWidgetByIndex(int index, _SLIDE_EFFECT effect)
{
	if (index < 0 || index >= m_widgetList.size())
		return;

	QWidget *prevWidget = NULL;
	if (m_index >= 0)
		prevWidget = m_widgetList.at(m_index);

	QWidget *curWidget = m_widgetList.at(index);
	m_index = index;

	switch (effect) {
		default:
		case _NO_SLIDE: {
			if (NULL != prevWidget)
				prevWidget->move(width(), 0);

			curWidget->move(0, 0);

			break;
		}
		case _LEFT_SLIDE: {
			startSlide(_LEFT_SLIDE);
			break;
		}

		case _RIGHT_SLIDE: {
			startSlide( _RIGHT_SLIDE);
			break;
		}
	}
}

void SlideWidget::startSlide(SlideWidget::_SLIDE_EFFECT effect)
{
	m_effect = effect;

	if (NULL == m_slideTimer)
		return;
	m_slideTimer->start(SLIDE_TIMEOUT);
}

void SlideWidget::stopSlide()
{
	if (NULL != m_slideTimer)
		m_slideTimer->stop();
}

void SlideWidget::leftShiftPixel(int pix)
{
	if (m_index < 0)
		return;

	QWidget *curWidget = m_widgetList.at(m_index);
	QWidget *leftWidget = (m_index > 0) ? m_widgetList.at(m_index - 1) : NULL;
	QWidget *rightWidget = (m_index < m_widgetList.size() - 1) ? m_widgetList.at(m_index + 1) : NULL;

	int curWidgetX = curWidget->x() - pix;
	int leftWidgetX = -width() + curWidgetX;
	int rightWidgetX = width() + curWidgetX;

	if (NULL != curWidget)
		curWidget->move(curWidgetX, 0);
	if (NULL != leftWidget)
		leftWidget->move(leftWidgetX, 0);
	if (NULL != rightWidget)
		rightWidget->move(rightWidgetX, 0);
}

void SlideWidget::rigthShiftPixel(int pix)
{
	if (m_index < 0)
		return;

	QWidget *curWidget = m_widgetList.at(m_index);
	QWidget *leftWidget = (m_index > 0) ? m_widgetList.at(m_index - 1) : NULL;
	QWidget *rightWidget = (m_index < m_widgetList.size() - 1) ? m_widgetList.at(m_index + 1) : NULL;

	int curWidgetX = curWidget->x() + pix;
	int leftWidgetX = -width() + curWidgetX;
	int rightWidgetX = width() + curWidgetX;

	if (NULL != curWidget)
		curWidget->move(curWidgetX, 0);
	if (NULL != leftWidget)
		leftWidget->move(leftWidgetX, 0);
	if (NULL != rightWidget)
		rightWidget->move(rightWidgetX, 0);
}

void SlideWidget::slideSlot()
{
	if (m_index < 0)
		return;

	QWidget *curWidget = m_widgetList.at(m_index);
	if (NULL == curWidget)
		return;

	int curWidgetX = curWidget->x();
	if (m_effect == _LEFT_SLIDE)
	{
		if (curWidgetX > 0) {
			int pix = (curWidgetX > SLIDE_STEP) ? SLIDE_STEP : curWidgetX;
			leftShiftPixel(pix);
		} else {
			stopSlide();
		}
	} else if (m_effect == _RIGHT_SLIDE)
	{
		if (curWidgetX < 0) {
			int pix = (curWidgetX < -SLIDE_STEP) ? SLIDE_STEP : -curWidgetX;
			rigthShiftPixel(pix);
		} else {
			stopSlide();
		}
	}

}

void SlideWidget::showPrevWidget()
{
	if (NULL != m_slideTimer && m_slideTimer->isActive())
		return;

	showWidgetByIndex(m_index - 1, _RIGHT_SLIDE);
}

void SlideWidget::showNextWidget()
{
	if (NULL != m_slideTimer && m_slideTimer->isActive())
		return;

	showWidgetByIndex(m_index + 1, _LEFT_SLIDE);
}

void SlideWidget::mousePressEvent(QMouseEvent *e)
{
//	qDebug() << "mouse pressed" << e->x();
	stopSlide();
	m_startMoveFlag = true;
	m_prevMouseX = e->x();
}

void SlideWidget::mouseReleaseEvent(QMouseEvent *)
{
//	qDebug() << "mouse release" << e->x();
	m_startMoveFlag = false;

	QWidget *curWidget = m_widgetList.at(m_index);
	if (NULL == curWidget)
		return;

	if (_LEFT_SLIDE == m_effect)
	{
		if (curWidget->x() < 0)
			showWidgetByIndex(m_index + 1, _LEFT_SLIDE);
		else
			showWidgetByIndex(m_index, _LEFT_SLIDE);
	} else if (_RIGHT_SLIDE == m_effect)
	{
		if (curWidget->x() > 0)
			showWidgetByIndex(m_index - 1, _RIGHT_SLIDE);
		else
			showWidgetByIndex(m_index, _RIGHT_SLIDE);
	} else
	{
		qDebug() << "start slide";
//		startSlide(m_prevWidget, m_curWidget, m_effect);
	}
}

void SlideWidget::mouseMoveEvent(QMouseEvent *e)
{
//	qDebug() << "mouse move" << e->x();
	if (!m_startMoveFlag || m_index < 0)
		return;

	if (e->x() < 0)
		return;

	int mouseDiffX = e->x() - m_prevMouseX;
	QWidget *curWidget = m_widgetList.at(m_index);
	if (mouseDiffX < 0)
	{
		m_effect = _LEFT_SLIDE;

		if (curWidget == m_widgetList.last()) {
			if (curWidget->x() > 0) {
				leftShiftPixel((-mouseDiffX < curWidget->x()) ? (-mouseDiffX) : curWidget->x());
			}
		} else {
			leftShiftPixel(-mouseDiffX);
		}
	} else if (mouseDiffX > 0)
	{
		m_effect = _RIGHT_SLIDE;

		if (curWidget == m_widgetList.first()) {
			if (curWidget->x() < 0) {
				rigthShiftPixel((mouseDiffX < -curWidget->x()) ? mouseDiffX : (-curWidget->x()));
			}
		} else {
			rigthShiftPixel(mouseDiffX);
		}
	}

	m_prevMouseX = e->x();
}
