//#pragma once

#ifndef __EXPERIMENT_BLOCK
#define __EXPERIMENT_BLOCK

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QTextStream>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QTime>
#include <QMediaPlayer> 
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QScreen>
#include <QGuiApplication>

#include <cmath>
const double pi = std::acos(-1);

/*
const double CIRCLE_ANGLE     = 3.2;
const double TEXT_ANGLE       = 5.0;
const double SHIFT_ANGLE      = 9.0;

const double CIRCLE_ANGLE_LEG = 3.2;
const double TEXT_ANGLE_LEG   = 5.0;
const double SHIFT_ANGLE_LEG  = 9.0; */

const double laptop_screen_width_mm  = 352.0;
const double laptop_screen_height_mm = 197.0;

double angle2px(double angle, double dist, double size_px, bool is_width);

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
	DisplayTextMessage	   = 16,
	FinishExperiment       = 32
} BlockState;

enum
{
	Hand   = 1,
	Leg    = 2,
	Common = 4,
	Red	   = 8,
	Green  = 16
};

#define CircleOnScreen (ShowingCentralCircle|ShowingPerifiricCircle)

class ExperimentBlock : public QWidget
{
	Q_OBJECT

public:
	explicit ExperimentBlock(QWidget* parent = nullptr,
								QString subject_id = "",
								bool colored_series = false,
								double dist = 500,
								bool is_hand = true,
								double circle_angle = 3.2, 
								double text_angle = 5.0, 
								double shift_angle = 9.0,
								unsigned int wait_before_circle_moving_time = 332, 
								HandKind hand_to_test = RIGHT);
	~ExperimentBlock();

	const QString & GetText() { return text_to_show; }
	void SetText(const QString& text, uint category) { text_to_show = text; word_category = category; }
	
	inline uint GetCategory() { return word_category; }
	QString GetCategoryString()
	{
		bool complicate = word_category > 7;
		QString tmp = complicate ? "(" : "";

		switch (word_category&7)
		{
		case Hand:
			tmp +="HAND";
			break;
		case Leg:
			tmp += "LEG";
			break;
		case Common:
			tmp += "COMMON";
			break;
		default:
			return QString("Error!!!");
		}

		if (complicate)
		{
			switch (word_category >> 3)
			{
			case Red:
				tmp += " RED)";
				break;
			case Green:
				tmp += " GREEN)";
				break;
			default:
				return QString("Error!!!");
			}
		}

		return tmp;
	}

	HandKind GetHandKind() { return hand_to_test; }
	void SetHandKind(HandKind hand) 
	{ 
		hand_to_test = hand; 
		logging_stream << ("\nActive hand: " + static_cast<QString>(GetHandKind() == LEFT ? "LEFT" : "RIGHT") + "\n\n");

		int wdt = QGuiApplication::primaryScreen()->geometry().width();

		int delta = angle2px(shift_angle, distance_to_display, wdt, true),
			circle_size = circle_bounds_rect.width();
		perifiric_circle_bounds_rect = QRect((hand_to_test&RIGHT) ? delta : -(delta + circle_size), -circle_size / 2, circle_size, circle_size);

		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		condition_change = codec->toUnicode("Серия окончена\nНовая серия будет рисовать круг ") + codec->toUnicode(GetHandKind() == LEFT ? "слева" : "справа");
	}

	void SetInterval(unsigned int interval)
	{
		wait_before_circle_moving_time = interval;
		logging_stream << ("\nInterval before circle change: " + QString("%1").arg(wait_before_circle_moving_time) + "\n\n");

		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		condition_change = codec->toUnicode("Серия окончена\nНовая серия будет выводить текст с другим интервалом");
	}

	unsigned int GetBlockState() { return current_state; }
	void SetBlockState(unsigned int state) { current_state = state; }

	QRect GetCircleBounds()          { return circle_bounds_rect; }
	QRect GetPerifiricCircleBounds() { return perifiric_circle_bounds_rect; }

	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent * event);
	void keyPressEvent(QKeyEvent *event);

	void StopAllTimers();
	void FailedTrial();

	QFile logging_file;
	QTextStream logging_stream;
	QTime elapsed_timer;

	QWidget* controller;

	QString condition_change;

	QMediaPlayer* player;
	QMediaPlaylist playlist;

	double distance_to_display;
	bool   is_hand;
	int    font_size;
	double circle_angle;
	double text_angle;
	double shift_angle;

private slots:
	void AddText();
	void ChangeToPerifiric();
	void FinishTheBlock();

signals:
	void GetNextWord(bool);
	void CloseAll();

private:
	//unsigned int interstimulus_time;
	//unsigned int center_helding_time;
	//unsigned int perifiric_helding_time;
	unsigned int wait_before_circle_moving_time;
	HandKind     hand_to_test;
	unsigned int current_state;
	QString		 text_to_show;
	unsigned int word_category;
	QRect        circle_bounds_rect;
	QRect        text_bounds_rect;
	QRect        perifiric_circle_bounds_rect;
	QTimer       hold_center_timer;
	QTimer       draw_perifiric_timer;
	QTimer 		 hold_perifiric_timer; 
	QTimer		 wait_during_common_word;
	//int          cross_half_size;
	bool         colored;

	bool holding_center;
	bool holding_perifiric;
};

#endif // !__EXPERIMENT_BLOCK
