#pragma once

#include "ExperimentBlock.h"
#include <QTextCodec>

#include <QWidget>

class ExpirementController: public QWidget
{
	Q_OBJECT

public:
	explicit ExpirementController();

	void EvaluateExperiment();

	ExperimentBlock experiment_block;

	QStringList formed_for_experiment;

	int show_idx = 0;

	QStringList hand_words = { 
		"бросать",
		"вязать",
		"держать",
		"кидать",
		"красить",
		"рубить",
		"хватать",
		"резать",
		"ощупывать",
		"касаться" 
	};
	QStringList leg_words = { 
		"бежать",
		"вставать",
		"пинать",
		"приседать",
		"прыгать",
		"топать",
		"ходить",
		"шагать",
		"топтать",
		"шаркать"
	};
	QStringList common_words = {
		"обладать",
		"усилить",
		"выучить",
		"увлечь",
		"уделить",
		"верить",
		"развить",
		"желать",
		"обещать",
		"возмутить"
	};

public slots:
	void NextTrial();
};