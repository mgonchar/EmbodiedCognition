#pragma once

#include "ExperimentBlock.h"

#include <QWidget>

class ExpirementController
{
public:
	ExpirementController() 
	{
		formed_for_experiment.reserve(3*8*10);
	}

	QStringList formed_for_experiment;

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
	QStringList common = { 
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
};