#include "ExperimentController.h"

#include <time.h>

ExpirementController::ExpirementController(QApplication* a, QString id, bool colored_series)
	: experiment_block(this, id, colored_series)
{
	formed_for_experiment.reserve(3*8*10);

	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");

	common_words.reserve(10);
	common_words.push_back(CategorialWord(codec->toUnicode("обладать"),  colored_series ? Red   : Common));
	common_words.push_back(CategorialWord(codec->toUnicode("усилить"),   colored_series ? Red   : Common));
	common_words.push_back(CategorialWord(codec->toUnicode("выучить"),   colored_series ? Red   : Common));
	common_words.push_back(CategorialWord(codec->toUnicode("увлечь"),    colored_series ? Red   : Common));
	common_words.push_back(CategorialWord(codec->toUnicode("уделить"),   colored_series ? Red   : Common));
	common_words.push_back(CategorialWord(codec->toUnicode("верить"),    colored_series ? Green : Common));
	common_words.push_back(CategorialWord(codec->toUnicode("развить"),   colored_series ? Green : Common));
	common_words.push_back(CategorialWord(codec->toUnicode("желать"),    colored_series ? Green : Common));
	common_words.push_back(CategorialWord(codec->toUnicode("обещать"),   colored_series ? Green : Common));
	common_words.push_back(CategorialWord(codec->toUnicode("возмутить"), colored_series ? Green : Common));

	hand_words.reserve(10);
	hand_words.push_back(CategorialWord(codec->toUnicode("бросать"),   colored_series ? Red   : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("вязать"),    colored_series ? Red   : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("держать"),   colored_series ? Red   : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("кидать"),    colored_series ? Red   : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("красить"),   colored_series ? Red   : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("рубить"),    colored_series ? Green : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("хватать"),   colored_series ? Green : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("резать"),    colored_series ? Green : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("ощупывать"), colored_series ? Green : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("касаться"),  colored_series ? Green : Hand));

	leg_words.reserve(10);
	leg_words.push_back(CategorialWord(codec->toUnicode("бежать"),     colored_series ? Red   : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("вставать"),   colored_series ? Red   : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("пинать"),     colored_series ? Red   : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("приседать"),  colored_series ? Red   : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("прыгать"),    colored_series ? Red   : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("топать"),     colored_series ? Green : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("ходить"),     colored_series ? Green : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("шагать"),     colored_series ? Green : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("топтать"),    colored_series ? Green : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("шаркать"),    colored_series ? Green : Leg));

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
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("обладать"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("усилить"), Hand));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("выучить"), Leg));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("увлечь"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("уделить"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("верить"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("развить"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("желать"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("обещать"), Common));
	formed_for_experiment.push_back(CategorialWord(codec->toUnicode("возмутить"), Common));
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
