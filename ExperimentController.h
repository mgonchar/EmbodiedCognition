#pragma once

#include "ExperimentBlock.h"

#include <QApplication>
#include <QWidget>

struct CategorialWord
{
	CategorialWord(QString wrd, uint categ){
		word = wrd;
		category = categ;
	};

	QString word;
	uint    category;
};

class ExpirementController: public QWidget
{
	Q_OBJECT

public:
	explicit ExpirementController(QApplication* a, QString id = "", bool colored_series = false, double dist = 500.0, bool is_hand = true, 
		double circle_angle = 3.2, double text_angle = 5.0, double shift_angle = 9.0);

	void EvaluateExperiment();

	void FormExperimentSet();

	ExperimentBlock experiment_block;

	QList<CategorialWord> formed_for_experiment;

	int show_idx = 0;

	QList<CategorialWord> hand_words;
	QList<CategorialWord> leg_words;
	QList<CategorialWord> common_words;

	HandKind hk_design[2] = { LEFT, RIGHT };
	unsigned int mvngtm_design[2] = { 53, 332 };

	int hk_idx = 0;
	int mvngtm_idx = 0;

	bool is_colored = false;

public slots:
	void NextTrial(bool);
};