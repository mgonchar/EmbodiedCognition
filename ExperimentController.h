#pragma once

#include "ExperimentBlock.h"

#include <QApplication>
#include <QWidget>

struct CategorialWord
{
	CategorialWord(QString wrd, WordCategory categ){
		word = wrd;
		category = categ;
	};

	QString word;
	WordCategory category;
};

class ExpirementController: public QWidget
{
	Q_OBJECT

public:
	explicit ExpirementController(QApplication* a, QString id = "");

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

public slots:
	void NextTrial();
};