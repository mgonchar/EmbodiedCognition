//#pragma once

#ifndef __EXPERIMENT_BLOCK
#define __EXPERIMENT_BLOCK

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QTime>

typedef enum
{
	LEFT = 1,
	RIGHT
} HandKind;

typedef enum 
{
	//ShowingInterstimulus   = 1,
	ShowingCentralCircle   = 2,
	ShowingText			   = 4,
	ShowingPerifiricCircle = 8,
	FinishBlock            = 16
} BlockState;

typedef enum
{
	Hand   = 1,
	Leg    = 2,
	Common = 4
} WordCategory;

#define CircleOnScreen (ShowingCentralCircle|ShowingPerifiricCircle)

class ExperimentBlock : public QWidget
{
	Q_OBJECT

public:
	explicit ExperimentBlock(QWidget* parent = nullptr,
								QString subject_id = "",
								unsigned int wait_before_circle_moving_time = 53, 
								HandKind hand_to_test = RIGHT);
	~ExperimentBlock();

	const QString & GetText() { return text_to_show; }
	void SetText(const QString& text, WordCategory category) { text_to_show = text; word_category = category; };
	QString GetCategoryString()
	{
		switch (word_category)
		{
		case Hand:
			return QString("HAND");
		case Leg:
			return QString("LEG");
		case Common:
			return QString("COMMON");
		default:
			return QString("Error!!!");
		}
	}

	HandKind GetHandKind() { return hand_to_test; }
	void SetHandKind(HandKind hand) 
	{ 
		hand_to_test = hand; 
		logging_stream << ("Active hand: " + static_cast<QString>(GetHandKind() == LEFT ? "LEFT" : "RIGHT") + "\n\n");
	}

	void SetInterval(unsigned int interval)
	{
		wait_before_circle_moving_time = interval;
		logging_stream << ("Interval before circle change: " + QString("%1").arg(wait_before_circle_moving_time) + "\n\n");
	}

	unsigned int GetBlockState() { return current_state; }
	void SetBlockState(unsigned int state) { current_state = state; }

	QRect GetCircleBounds()          { return circle_bounds_rect; }
	QRect GetPerifiricCircleBounds() { return perifiric_circle_bounds_rect; }

	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	void StopAllTimers();
	void FailedTrial();

	QFile logging_file;
	QTextStream logging_stream;
	QTime elapsed_timer;

	QWidget* controller;

private slots:
	void AddText();
	void ChangeToPerifiric();
	void FinishTheBlock();

signals:
	void GetNextWord();

private:
	unsigned int interstimulus_time;
	unsigned int center_helding_time;
	unsigned int perifiric_helding_time;
	unsigned int wait_before_circle_moving_time;
	HandKind     hand_to_test;
	unsigned int current_state;
	QString		 text_to_show;
	WordCategory word_category;
	QRect        circle_bounds_rect;
	QRect        text_bounds_rect;
	QRect        perifiric_circle_bounds_rect;
	QTimer       hold_center_timer;
	QTimer       draw_perifiric_timer;
	QTimer 		 hold_perifiric_timer; 
	int          cross_half_size;

	bool holding_center;
	bool holding_perifiric;
};

#endif // !__EXPERIMENT_BLOCK
