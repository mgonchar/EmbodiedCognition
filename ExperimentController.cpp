#include "ExperimentController.h"

ExpirementController::ExpirementController()
	: experiment_block(this)
{
	//formed_for_experiment.reserve(3*8*10);

	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");

	formed_for_experiment.push_back(codec->toUnicode("обладать"));
	formed_for_experiment.push_back(codec->toUnicode("усилить"));
	formed_for_experiment.push_back(codec->toUnicode("выучить"));
	formed_for_experiment.push_back(codec->toUnicode("увлечь"));
	formed_for_experiment.push_back(codec->toUnicode("уделить"));
	formed_for_experiment.push_back(codec->toUnicode("верить"));
	formed_for_experiment.push_back(codec->toUnicode("развить"));
	formed_for_experiment.push_back(codec->toUnicode("желать"));
	formed_for_experiment.push_back(codec->toUnicode("обещать"));
	formed_for_experiment.push_back(codec->toUnicode("возмутить"));

}

void ExpirementController::EvaluateExperiment()
{
	experiment_block.SetText(formed_for_experiment[show_idx++], Common);
	experiment_block.show();
	//show();
}

void ExpirementController::NextTrial()
{
	experiment_block.SetText(formed_for_experiment[show_idx++], Common);
	experiment_block.SetBlockState(ShowingCentralCircle);
	experiment_block.update();
}
