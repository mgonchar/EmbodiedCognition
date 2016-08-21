#include "ExperimentController.h"

#include <time.h>

ExpirementController::ExpirementController(QApplication* a, QString id)
	: experiment_block(this, id)
{
	//formed_for_experiment.reserve(3*8*10);

	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");

	common_words.reserve(10);
	common_words.push_back(CategorialWord(codec->toUnicode("��������"), Common));
	common_words.push_back(CategorialWord(codec->toUnicode("�������"), Common));
	common_words.push_back(CategorialWord(codec->toUnicode("�������"), Common));
	common_words.push_back(CategorialWord(codec->toUnicode("������"), Common));
	common_words.push_back(CategorialWord(codec->toUnicode("�������"), Common));
	common_words.push_back(CategorialWord(codec->toUnicode("������"), Common));
	common_words.push_back(CategorialWord(codec->toUnicode("�������"), Common));
	common_words.push_back(CategorialWord(codec->toUnicode("������"), Common));
	common_words.push_back(CategorialWord(codec->toUnicode("�������"), Common));
	common_words.push_back(CategorialWord(codec->toUnicode("���������"), Common));

	hand_words.reserve(10);
	hand_words.push_back(CategorialWord(codec->toUnicode("�������"), Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("������"), Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("�������"), Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("������"), Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("�������"), Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("������"), Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("�������"), Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("������"), Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("���������"), Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("��������"), Hand));

	leg_words.reserve(10);
	leg_words.push_back(CategorialWord(codec->toUnicode("������"), Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("��������"), Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("������"), Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("���������"), Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("�������"), Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("������"), Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("������"), Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("������"), Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("�������"), Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("�������"), Leg));

	FormExperimentSet();

	connect(&experiment_block, SIGNAL(CloseAll()), a, SLOT(quit()));

	qsrand(time(0));

	if ((static_cast<double>(qrand())) / RAND_MAX > 0.5) {
		std::swap(hk_design[0], hk_design[1]);
	}
	experiment_block.SetHandKind(hk_design[0]);

	/*
	if ((static_cast<double>(qrand())) / RAND_MAX > 0.5) {
		std::swap(mvngtm_design[0], mvngtm_design[1]);
	}
	experiment_block.SetInterval(mvngtm_design[0]);
	*/
}

void ExpirementController::FormExperimentSet()
{
	formed_for_experiment.clear();

	QList<CategorialWord> tmp;
	tmp.reserve(30);

	std::srand(time(0));
	for (int i = 0; i < 8; ++i) 
	{
		tmp.clear();

		std::random_shuffle(hand_words.begin(), hand_words.end());
		std::random_shuffle(leg_words.begin(), leg_words.end());
		std::random_shuffle(common_words.begin(), common_words.end());

		tmp += hand_words;
		tmp += leg_words;
		tmp += common_words;

		std::random_shuffle(tmp.begin(), tmp.end());
		
		formed_for_experiment += tmp;
	}


	/*
	formed_for_experiment.clear();
	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("��������"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("�������"), Hand));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("�������"), Leg));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("������"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("�������"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("������"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("�������"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("������"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("�������"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("���������"), Common));
	*/
	show_idx = 0;
}

void ExpirementController::EvaluateExperiment()
{
	experiment_block.SetText(formed_for_experiment[show_idx].word, formed_for_experiment[show_idx].category);
	show_idx++;
	experiment_block.show();
}

void ExpirementController::NextTrial()
{
	if (show_idx != formed_for_experiment.size())
	{
		experiment_block.SetText(formed_for_experiment[show_idx].word, formed_for_experiment[show_idx].category);
		show_idx++;
		experiment_block.SetBlockState(ShowingCentralCircle);
		experiment_block.update();
	}
	else 
	{
		if (hk_idx == 1 /*&& mvngtm_idx == 1*/) {
			experiment_block.SetBlockState(FinishExperiment);
			return;
		}
		else
		{
			/*
			if (hk_idx == 0 && mvngtm_idx == 0)
			{
				experiment_block.SetInterval(mvngtm_design[++mvngtm_idx]);
			}
			else if (hk_idx == 0 && mvngtm_idx == 1) 
			{
				experiment_block.SetHandKind(hk_design[++hk_idx]);
				experiment_block.SetInterval(mvngtm_design[--mvngtm_idx]);
			}
			else if (hk_idx == 1 && mvngtm_idx == 0)
			{
				experiment_block.SetInterval(mvngtm_design[++mvngtm_idx]);
			}*/

			experiment_block.SetHandKind(hk_design[++hk_idx]);
		}
		FormExperimentSet();
		experiment_block.SetBlockState(DisplayTextMessage);
		experiment_block.update();
	}
}
