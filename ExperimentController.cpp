#include "ExperimentController.h"

ExpirementController::ExpirementController()
	: experiment_block(this)
{
	//formed_for_experiment.reserve(3*8*10);

	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");

	formed_for_experiment.push_back(codec->toUnicode("��������"));
	formed_for_experiment.push_back(codec->toUnicode("�������"));
	formed_for_experiment.push_back(codec->toUnicode("�������"));
	formed_for_experiment.push_back(codec->toUnicode("������"));
	formed_for_experiment.push_back(codec->toUnicode("�������"));
	formed_for_experiment.push_back(codec->toUnicode("������"));
	formed_for_experiment.push_back(codec->toUnicode("�������"));
	formed_for_experiment.push_back(codec->toUnicode("������"));
	formed_for_experiment.push_back(codec->toUnicode("�������"));
	formed_for_experiment.push_back(codec->toUnicode("���������"));

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
