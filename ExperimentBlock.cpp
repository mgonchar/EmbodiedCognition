#include "ExperimentBlock.h"

ExperimentBlock::ExperimentBlock(QWidget * parent,
	QString subject_id,
	unsigned int wait_before_circle_moving_time,
	HandKind hand_to_test)
	: controller(parent)//QWidget(parent)
	, logging_file(QDir::currentPath() + "/subjects/" + subject_id +"_"+ QDate::currentDate().toString("dd.MM.yyyy") + ".log")
	, logging_stream(&logging_file)
	, wait_before_circle_moving_time(wait_before_circle_moving_time)
	, hand_to_test(hand_to_test)
	, interstimulus_time(500)
	, center_helding_time(400)	  // use random here ?
	, perifiric_helding_time(400) // use random here ?
	, current_state(ShowingCentralCircle)
	, circle_bounds_rect(-50, -50, 100, 100)
	//, text_bounds_rect(-50,-100,100,50)
	, cross_half_size(10)
	, holding_center(false)
	, holding_perifiric(false)
	//, hold_center_timer(new QTimer(this))
	//, draw_perifiric_timer(new QTimer(this))
	//, hold_perifiric_timer(new QTimer(this))
	//, painter(new QPainter(this))
{

	//painter->setPen(Qt::white);
	//painter->translate(width() / 2, height() / 2);
	//int side = qMin(width(), height());
	//painter->scale(side / 200.0, side / 200.0);
	//painter->setRenderHint(QPainter::Antialiasing);
	//painter->setRenderHint(QPainter::TextAntialiasing);

	connect(this, SIGNAL(GetNextWord()), controller, SLOT(NextTrial()));

	QDir().mkpath(logging_file.fileName().remove(subject_id + QDate::currentDate().toString("dd.MM.yyyy") + ".log"));
	logging_file.open(QIODevice::ReadWrite | QIODevice::Truncate);

	logging_stream.setCodec("UTF-16");

	logging_stream << ("Experiment started: " + QDate::currentDate().toString("dd.MM.yyyy") + "\nSubject id: "+ subject_id + "\n\n");

	//logging_stream << ("Interval before circle change: " + QString("%1").arg(wait_before_circle_moving_time) + "\n\n");

	//logging_stream << ("Active hand: " + static_cast<QString>(GetHandKind()==LEFT ? "LEFT" : "RIGHT") + "\n\n");

	text_bounds_rect = QRect(-width()/2, -height()/2, width(), height() / 2 - 50);
	perifiric_circle_bounds_rect = QRect( (hand_to_test&RIGHT) ? width()/2 - 100 : 0, -50, 100, 100);

	hold_center_timer.setTimerType(Qt::PreciseTimer);
	connect(&hold_center_timer, SIGNAL(timeout()), this, SLOT(AddText()));
	hold_center_timer.setSingleShot(true);

	draw_perifiric_timer.setTimerType(Qt::PreciseTimer);
	connect(&draw_perifiric_timer, SIGNAL(timeout()), this, SLOT(ChangeToPerifiric()));
	draw_perifiric_timer.setSingleShot(true);

	hold_perifiric_timer.setTimerType(Qt::PreciseTimer);
	connect(&hold_perifiric_timer, SIGNAL(timeout()), this, SLOT(FinishTheBlock()));
	hold_perifiric_timer.setSingleShot(true);

	setAutoFillBackground(true);
	setStyleSheet("background-color: black");
	setWindowState(Qt::WindowFullScreen); // uncomment to get fullscreen mode
}

ExperimentBlock::~ExperimentBlock() 
{
	logging_stream << "Experiment finished.";
	logging_stream.flush(); 
	logging_file.close(); 
}

void ExperimentBlock::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);

	QPen pen(Qt::white);
	pen.setWidth(2);

	QBrush brush(Qt::red);

	painter.translate(width() / 2, height() / 2);
	//int side = qMin(width(), height());
	//painter.scale(side / 200.0, side / 200.0);

	unsigned int current_state = GetBlockState();
	
	/*
	if (current_state & ShowingInterstimulus)
	{
		painter.setPen(pen);

		painter.drawLine(-cross_half_size, 0, cross_half_size, 0);
		painter.drawLine(0, -cross_half_size, 0, cross_half_size);

		//QTimer *timer = new QTimer(this);
		//timer->setTimerType(Qt::PreciseTimer);
		//connect(timer, SIGNAL(timeout()), this, SLOT(update()));
		//timer->start(1000);

		SetBlockState(ShowingCentralCircle);
		QTimer::singleShot(interstimulus_time, Qt::PreciseTimer, this, SLOT(update()));

		//painter.setFont(QFont("Arial", 50));
		//painter.drawText(rect(), Qt::AlignCenter, "Nastichka Zayka :*");
	}*/

	if (current_state & ShowingCentralCircle)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(brush);

		painter.drawEllipse(GetCircleBounds());
	}

	if (current_state & ShowingText)
	{
		painter.setPen(pen);
		painter.setFont(QFont("Arial", 50));

		logging_stream << ("Displaying text: " + text_to_show + " of category: " + GetCategoryString() +"\n");

		elapsed_timer.start();
		painter.drawText(text_bounds_rect, Qt::AlignBottom | Qt::AlignHCenter, text_to_show);
	}

	if (current_state & ShowingPerifiricCircle)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(brush);

		painter.drawEllipse(GetPerifiricCircleBounds());
	}

	if (current_state & DisplayTextMessage)
	{
		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		painter.setPen(pen);
		painter.setFont(QFont("Arial", 50));

		painter.drawText(text_bounds_rect, Qt::AlignBottom | Qt::AlignHCenter, codec->toUnicode("Серия окончена\nСмена условий эксперимента"));
	}

	if (current_state & FinishExperiment)
	{
		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		painter.setPen(pen);
		painter.setFont(QFont("Arial", 50));

		painter.drawText(text_bounds_rect, Qt::AlignBottom | Qt::AlignHCenter, codec->toUnicode("Эксперимент окончен!"));
	}

}

void ExperimentBlock::mousePressEvent(QMouseEvent *event)
{
	QPoint pos = event->pos() - QPoint(width() / 2, height() / 2);

	unsigned int current_state = GetBlockState();

	switch (current_state)
	{
	case DisplayTextMessage:
	case FinishExperiment:
		return;
		break;
	}

	switch (current_state & CircleOnScreen)
	{
		case ShowingCentralCircle:
			if (!(current_state & ShowingText) && GetCircleBounds().contains(pos))
			{
				holding_center = true;
				hold_center_timer.start(center_helding_time);
			}
			else
			{
				StopAllTimers();
				FailedTrial();
			}
			break;

		case ShowingPerifiricCircle:
			if (GetPerifiricCircleBounds().contains(pos))
			{
				holding_perifiric = true;
				int n_milliseconds = elapsed_timer.elapsed();
				logging_stream << ("Finger movement taken: " + QString("%1").arg(n_milliseconds) + " ms\n");
				logging_stream.flush();
				hold_perifiric_timer.start(perifiric_helding_time);
			}
			else
			{
				StopAllTimers();
				FailedTrial();
			}
			break;

		default:
			StopAllTimers();
			FailedTrial();
			break;
	}

}

void ExperimentBlock::mouseReleaseEvent(QMouseEvent *)
{
	if (hold_perifiric_timer.isActive() || hold_center_timer.isActive())
	{
		StopAllTimers();
		FailedTrial();
	} 
	else
	{
		if (holding_center)
		{
			int n_milliseconds = elapsed_timer.elapsed();
			logging_stream << ("Thinked before action: " + QString("%1").arg(n_milliseconds) + " ms\n");
			logging_stream.flush();
			elapsed_timer.start();
		}
	}

	holding_center = holding_perifiric = false;
}

void ExperimentBlock::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key::Key_Escape:
		CloseAll();
		break;
	}

	switch (GetBlockState()) {
	case FinishExperiment:
		CloseAll();
		break;
	case DisplayTextMessage:
		GetNextWord();
		break;
	}
}

void ExperimentBlock::StopAllTimers()
{
	if (hold_center_timer.isActive())
		hold_center_timer.stop();

	if (draw_perifiric_timer.isActive())
		draw_perifiric_timer.stop();

	if (hold_perifiric_timer.isActive())
		hold_perifiric_timer.stop();
}


void ExperimentBlock::FailedTrial()
{
	logging_stream << "Trial failed in reason of occasional or incorrect screen touching\n";
	logging_stream.flush();
	GetNextWord();
}


void ExperimentBlock::AddText()
{
	SetBlockState(ShowingCentralCircle | ShowingText); 
	draw_perifiric_timer.start(wait_before_circle_moving_time); 
	update(); 
}

void ExperimentBlock::ChangeToPerifiric()
{ 
	SetBlockState(ShowingText | ShowingPerifiricCircle); 
	update(); 
}

void ExperimentBlock::FinishTheBlock()
{ 
	GetNextWord();
	update(); 
}
