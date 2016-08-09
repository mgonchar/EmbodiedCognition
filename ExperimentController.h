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
	QStringList common = { 
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
};