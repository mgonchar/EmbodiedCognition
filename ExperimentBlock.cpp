#include "ExperimentBlock.h"

double angle2px(double angle, double dist, double size_px, bool is_width)
{
	return std::tan(angle*pi / 360.0)*2.0*dist*size_px / (is_width ? laptop_screen_width_mm : laptop_screen_height_mm);
}

ExperimentBlock::ExperimentBlock(QWidget * parent,
	QString subject_id,
	bool colored_series,
	double dist,
	bool is_hand,
	double circle_angle, 
	double text_angle, 
	double shift_angle,
	unsigned int wait_before_circle_moving_time,
	HandKind hand_to_test)
	: controller(parent)//QWidget(parent)
	, player(new QMediaPlayer(this, QMediaPlayer::LowLatency))
	, logging_file(QDir::currentPath() + "/subjects/" + subject_id + QString(colored_series ? "_colored_" : "_ordinal_") + (is_hand ? "hand_" : "leg_") + QDate::currentDate().toString("dd.MM.yyyy") + ".log")
	, logging_stream(&logging_file)
	, wait_before_circle_moving_time(wait_before_circle_moving_time)
	, hand_to_test(hand_to_test)
	//, interstimulus_time(500)
	//, center_helding_time(400)	  // use random here ?
	//, perifiric_helding_time(400) // use random here ?
	, current_state(ShowingCentralCircle)
	//, circle_bounds_rect(-55, -55, 110, 110)
	//, text_bounds_rect(-50,-100,100,50)
	//, cross_half_size(10)
	, colored(colored_series)
	, distance_to_display(dist)
	, is_hand(is_hand)
	, holding_center(false)
	, holding_perifiric(false)
	, font_size(50)
	, circle_angle(circle_angle)
	, text_angle(text_angle)
	, shift_angle(shift_angle)
	//, hold_center_timer(new QTimer(this))
	//, draw_perifiric_timer(new QTimer(this))
	//, hold_perifiric_timer(new QTimer(this))
	//, painter(new QPainter(this))
{
	connect(this, SIGNAL(GetNextWord(bool)), controller, SLOT(NextTrial(bool)));

	// Not overwrite the log-file, if exists
	int idx_to_append = 1;
	while (logging_file.exists())
	{
		int idx = logging_file.fileName().lastIndexOf("___");

		QString tmp_nm;
		if (idx != -1)
		{
			tmp_nm = logging_file.fileName().replace(idx, logging_file.fileName().length() - idx, "___" + QString("%1").arg(idx_to_append) + ".log");
		}
		else
		{
			tmp_nm = logging_file.fileName().replace(logging_file.fileName().length() - 4, 4, "___" + QString("%1").arg(idx_to_append) + ".log");
		}

		logging_file.setFileName(tmp_nm);

		++idx_to_append;
	}

	QDir().mkpath(QDir::currentPath() + "/subjects/");
	logging_file.open(QIODevice::ReadWrite | QIODevice::Truncate);
	logging_stream.setCodec("UTF-16");
	logging_stream << ("Experiment started: " + QDate::currentDate().toString("dd.MM.yyyy") + "\nSubject id: "+ subject_id + "\n\n");
	logging_stream << "Series type: " << (is_hand? "TESTING_HANDS\n" : "TESTING_LEGS\n");
	logging_stream << "Series type: " << (colored_series ? "colored\n\n" : "ordinal\n\n");

	int wdt = QGuiApplication::primaryScreen()->geometry().width(), hgt = QGuiApplication::primaryScreen()->geometry().height();
	int circle_size = angle2px(circle_angle, distance_to_display, wdt, true);
	int delta = angle2px(shift_angle, distance_to_display, wdt, true);

	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
	QFontMetrics fm = QFontMetrics(QFont("Arial", font_size));
	int px_w = fm.width(codec->toUnicode("возмутить"));
	double tmp = angle2px(text_angle, distance_to_display, wdt, true);
	font_size *= tmp / px_w;

	circle_bounds_rect = QRect(-circle_size/2, -circle_size/2, circle_size, circle_size);
	text_bounds_rect = QRect(-wdt/2, -hgt/2, wdt, hgt / 2 - circle_size / 2);
	perifiric_circle_bounds_rect = QRect( (hand_to_test&RIGHT) ? delta : -(delta + circle_size), -circle_size/2, circle_size, circle_size);

	hold_center_timer.setTimerType(Qt::PreciseTimer);
	connect(&hold_center_timer, SIGNAL(timeout()), this, SLOT(AddText()));
	hold_center_timer.setSingleShot(true);

	draw_perifiric_timer.setTimerType(Qt::PreciseTimer);
	connect(&draw_perifiric_timer, SIGNAL(timeout()), this, SLOT(ChangeToPerifiric()));
	draw_perifiric_timer.setSingleShot(true);

	hold_perifiric_timer.setTimerType(Qt::PreciseTimer);
	connect(&hold_perifiric_timer, SIGNAL(timeout()), this, SLOT(FinishTheBlock()));
	hold_perifiric_timer.setSingleShot(true);

	wait_during_common_word.setTimerType(Qt::PreciseTimer);
	connect(&wait_during_common_word, SIGNAL(timeout()), this, SLOT(FinishTheBlock()));
	wait_during_common_word.setSingleShot(true);

	setAutoFillBackground(true);
	setStyleSheet("background-color: black");
	setWindowState(Qt::WindowFullScreen);

	playlist.addMedia(QUrl::fromLocalFile(QDir::currentPath() + "/sounds/bad_signal-1.wav"));
	playlist.addMedia(QUrl::fromLocalFile(QDir::currentPath() + "/sounds/good_signal-4-1.wav"));
	playlist.setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
	playlist.setCurrentIndex(1);
	player->setPlaylist(&playlist);
	player->setAudioRole(QAudio::NotificationRole/*AlarmRole*/);
	player->setVolume(100);

	setAttribute(Qt::WA_AcceptTouchEvents);

	//installEventFilter(this);
}

ExperimentBlock::~ExperimentBlock() 
{
	StopAllTimers();

	logging_stream << "Experiment finished.";
	logging_stream.flush(); 
	logging_file.close(); 
}

void ExperimentBlock::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);

	QStyleOption opt;
	opt.init(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

	QPen pen(Qt::white);
	pen.setWidth(2);

	QBrush brush(colored ? QColor(130,128,128) : Qt::red);

	painter.translate(width() / 2, height() / 2);
	//int side = qMin(width(), height());
	//painter.scale(side / 200.0, side / 200.0);

	unsigned int current_state = GetBlockState();

	if (current_state & ShowingCentralCircle)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(brush);

		painter.drawEllipse(GetCircleBounds());
	}

	if (current_state & ShowingText)
	{
		pen.setColor(colored ? ((GetCategory() & Red) ? Qt::red : Qt::green) : Qt::white);
		painter.setPen(pen);
		painter.setFont(QFont("Arial", font_size));

		logging_stream << ("Displaying text: " + text_to_show + " of category: " + GetCategoryString() +"\n");

		if (!(GetCategory() == Common) && !(GetCategory() & Red))
			elapsed_timer.start();

		painter.drawText(text_bounds_rect, Qt::AlignBottom | Qt::AlignHCenter, text_to_show);
	}

	if (current_state & ShowingPerifiricCircle)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(brush);

		painter.drawEllipse(GetPerifiricCircleBounds());

		if ((GetCategory() == Common) || (GetCategory() & Red))
		{
			uint timer_time = (dis(gen) * 300 + 500); /* random 500-800 ms */
			logging_stream << "Started timer to wait during " << GetCategoryString() << " word: " << QString("%1").arg(timer_time) << " ms\n";
			logging_stream.flush();
			wait_during_common_word.start(timer_time);
		}
	}

	if (current_state & DisplayTextMessage)
	{
		painter.setPen(pen);
		painter.setFont(QFont("Arial", 40));

		painter.drawText(text_bounds_rect, Qt::AlignBottom | Qt::AlignHCenter, condition_change);
	}

	if (current_state & FinishExperiment)
	{
		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		painter.setPen(pen);
		painter.setFont(QFont("Arial", font_size));

		painter.drawText(text_bounds_rect, Qt::AlignBottom | Qt::AlignHCenter, codec->toUnicode("Эксперимент окончен!"));
	}

}

/*
bool ExperimentBlock::eventFilter(QObject *obj, QEvent *ev) {
	auto tp = ev->type();

	QList<QEvent::Type> ignore = { 
		QEvent::Type::Move, 
		QEvent::Type::Resize, 
		QEvent::Type::Show, 
		QEvent::Type::CursorChange, 
		QEvent::Type::ShowToParent, 
		QEvent::Type::PolishRequest, 
		QEvent::Type::UpdateLater, 
		QEvent::Type::UpdateRequest, 
		QEvent::Type::WindowActivate, 
		QEvent::Type::ActivationChange, 
		QEvent::Type::InputMethodQuery,  
		QEvent::Type::Paint, 
		QEvent::Type::Enter, 
		QEvent::Type::WindowDeactivate, 
		QEvent::Type::Leave, 
		QEvent::Type::WindowActivate,
		QEvent::Type::ToolTip};

	if (!ignore.contains(tp))
	{
		int ttt = 0;
	}

	return true;
}*/

/*
void ExperimentBlock::mousePressEvent(QMouseEvent *event)
{
	logging_stream << "EVENT: MOUSE PRESS\n";

	QPoint pos = event->pos() - QPoint(width() / 2, height() / 2);

	Press(pos);
}

void ExperimentBlock::mouseReleaseEvent(QMouseEvent *)
{
	logging_stream << "EVENT: MOUSE RELEASE\n";

	Release();
}

void ExperimentBlock::mouseMoveEvent(QMouseEvent *event)
{
	logging_stream << "EVENT: MOUSE MOVE\n";

	QPoint pos = event->pos() - QPoint(width() / 2, height() / 2);

	Move(pos);
}*/

bool ExperimentBlock::event(QEvent *event)
{
	//auto tp = event->type();
	switch (event->type())
	{
	case QEvent::MouseButtonPress:
		//Press((dynamic_cast<QMouseEvent *>(event))->pos() - QPoint(width() / 2, height() / 2));
		break;
    case QEvent::TouchBegin:
	{
		Press(((dynamic_cast<QTouchEvent *>(event))->touchPoints())[0].pos().toPoint() - QPoint(width() / 2, height() / 2));
	}
		break;

	case QEvent::MouseButtonRelease:
		//Release();
		break;
	case QEvent::TouchEnd:
		Release();
		break;

	case QEvent::MouseMove:
		//Move((dynamic_cast<QMouseEvent *>(event))->pos() - QPoint(width() / 2, height() / 2));
		break;
	case QEvent::TouchUpdate:
	{
		Move(((dynamic_cast<QTouchEvent *>(event))->touchPoints())[0].pos().toPoint() - QPoint(width() / 2, height() / 2));
	}
		break;

	case QEvent::KeyPress:
		keyPressEvent(dynamic_cast<QKeyEvent *>(event));
		break;

	case QEvent::Paint:
	case QEvent::UpdateRequest:
	case QEvent::UpdateLater:
		paintEvent(dynamic_cast<QPaintEvent*>(event));
		return QWidget::event(event);
		break;

	default:
		break;
	}

	return event->isAccepted();
}

void ExperimentBlock::Press(const QPoint & pos)
{
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
			uint timer_time = (dis(gen) * 300 + 400); /* random 400-700 ms */
			logging_stream << "Started timer to wait while central circle is held: " << QString("%1").arg(timer_time) << " ms\n";
			logging_stream.flush();
			hold_center_timer.start(timer_time);
		}
		else
		{
			StopAllTimers();
			FailedTrial(pos);
			return;
		}
		break;

	case ShowingPerifiricCircle:
		if (GetPerifiricCircleBounds().contains(pos))
		{
			holding_perifiric = true;
			int n_milliseconds = elapsed_timer.elapsed();
			logging_stream << ("Finger movement taken: " + QString("%1").arg(n_milliseconds) + " ms\n");
			logging_stream.flush();
			uint timer_time = (dis(gen) * 100 + 300); /* random 300-400 ms */
			logging_stream << "Started timer to wait while perifiric circle is held: " << QString("%1").arg(timer_time) << " ms\n";
			logging_stream.flush();
			hold_perifiric_timer.start(timer_time);
		}
		else
		{
			StopAllTimers();
			FailedTrial(pos);
			return;
		}
		break;

	default:
		StopAllTimers();
		FailedTrial(pos);
		return;
		break;
	}
}

void ExperimentBlock::Release()
{
	if (!holding_center && !holding_perifiric)
	{
		return;
	}

	switch (GetBlockState())
	{
	case DisplayTextMessage:
	case FinishExperiment:
		return;
		break;
	}

	bool wrong_decision_common_word = ((GetCategory() == Common) || (GetCategory() & Red)) && holding_center && wait_during_common_word.isActive();
	if (wrong_decision_common_word || hold_perifiric_timer.isActive() || hold_center_timer.isActive())
	{
		if (wrong_decision_common_word)
			logging_stream << "Finger released while word of category " << (colored ? "RED" : "COMMON") << " is displayed!!\n";
		else
			logging_stream << "Release event occured!\n";

		StopAllTimers();
		FailedTrial();
		return;
	}
	else
	{
		if (holding_center && !((GetCategory() == Common) || (GetCategory() & Red)))
		{
			int n_milliseconds = elapsed_timer.elapsed();
			logging_stream << ("Thinked before action: " + QString("%1").arg(n_milliseconds) + " ms\n");
			logging_stream.flush();
			elapsed_timer.start();
		}
	}

	holding_center = holding_perifiric = false;
}

void ExperimentBlock::Move(const QPoint & pos)
{
	if (!holding_center && !holding_perifiric)
	{
		return;
	}

	switch (GetBlockState())
	{
	case DisplayTextMessage:
	case FinishExperiment:
		return;
		break;
	}

	if (holding_center && !GetCircleBounds().contains(pos))
	{
		logging_stream << ("Pointer moved outside target circle\n");
		logging_stream.flush();

		StopAllTimers();
		FailedTrial(pos);
		return;
	}

	if (holding_perifiric && !GetPerifiricCircleBounds().contains(pos))
	{
		logging_stream << ("\n");
		logging_stream.flush();

		StopAllTimers();
		FailedTrial(pos);
		return;
	}
}

void ExperimentBlock::keyPressEvent(QKeyEvent *event)
{
	logging_stream << "EVENT: KEY PRESS\n";

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
		GetNextWord(false);
		break;
	}
}

void ExperimentBlock::StopAllTimers()
{
	if (wait_during_common_word.isActive())
		wait_during_common_word.stop();

	if (hold_center_timer.isActive())
		hold_center_timer.stop();

	if (draw_perifiric_timer.isActive())
		draw_perifiric_timer.stop();

	if (hold_perifiric_timer.isActive())
		hold_perifiric_timer.stop();
}


void ExperimentBlock::FailedTrial(QPoint point)
{
	holding_center = holding_perifiric = false;

	playlist.setCurrentIndex(0);
	player->play();

	logging_stream << "Trial failed in reason of occasional or incorrect screen touching\n";

	if (point.x() != DEFAULT_POS)
	{
		logging_stream << "Current state is: " << GetStateString();
		logging_stream << "Touched at x: " << point.x() << " y: " << point.y();

		switch (GetBlockState() & CircleOnScreen)
		{
			case ShowingCentralCircle:
			{
				QRect rect = GetCircleBounds();
				logging_stream << " While bounds are x: " << rect.x() << " y: " << rect.y() << " w: " << rect.width() << " h: " << rect.height() << "\n";
			}
			break;

			case ShowingPerifiricCircle:
			{
				QRect rect = GetPerifiricCircleBounds();
				logging_stream << " While bounds are x: " << rect.x() << " y: " << rect.y() << " w: " << rect.width() << " h: " << rect.height() << "\n";
			}
			break;
		}
	}

	logging_stream.flush();
	GetNextWord(true);
	update();
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
	playlist.setCurrentIndex(1);
	player->play();

	GetNextWord(false);
	//QTimer::singleShot(2000, this, SLOT(update()));
	update();
}

QString ExperimentBlock::GetStateString()
{
	unsigned int state = GetBlockState();
	QString ret = "";
	
	if (state & ShowingCentralCircle)
	{
		ret += "CentralCircle ";
	}

	if (state & ShowingPerifiricCircle)
	{
		ret += "PerifiricCircle ";
	}

	if (state & ShowingText)
	{
		ret += "ShowWord ";
	}

	if (state & DisplayTextMessage)
	{
		ret += "InterSerialMessage ";
	}

	if (state & FinishExperiment)
	{
		ret += "ExperimentFinished ";
	}

	ret += "\n";

	return ret;
}
