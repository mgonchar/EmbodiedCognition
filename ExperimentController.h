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
		"�������",
		"������",
		"�������",
		"������",
		"�������",
		"������",
		"�������",
		"������",
		"���������",
		"��������" 
	};
	QStringList leg_words = { 
		"������",
		"��������",
		"������",
		"���������",
		"�������",
		"������",
		"������",
		"������",
		"�������",
		"�������"
	};
	QStringList common_words = {
		"��������",
		"�������",
		"�������",
		"������",
		"�������",
		"������",
		"�������",
		"������",
		"�������",
		"���������"
	};

public slots:
	void NextTrial();
};