#include "ExperimentController.h"

#include <time.h>

ExpirementController::ExpirementController(QApplication* a, QString id, bool colored_series, double dist, bool is_hand, double circle_angle, double text_angle, double shift_angle)
	: experiment_block(this, id, colored_series, dist, is_hand, circle_angle, text_angle, shift_angle)
	, is_colored(colored_series)
{
	formed_for_experiment.reserve(3*8*10);

	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");

	common_words.reserve(10);
	common_words.push_back(CategorialWord(codec->toUnicode("��������"),  colored_series ? (Red   | Common): Common));
	common_words.push_back(CategorialWord(codec->toUnicode("�������"),   colored_series ? (Red   | Common): Common));
	common_words.push_back(CategorialWord(codec->toUnicode("�������"),   colored_series ? (Red   | Common): Common));
	common_words.push_back(CategorialWord(codec->toUnicode("������"),    colored_series ? (Red   | Common): Common));
	common_words.push_back(CategorialWord(codec->toUnicode("�������"),   colored_series ? (Red   | Common): Common));
	common_words.push_back(CategorialWord(codec->toUnicode("������"),    colored_series ? (Green | Common): Common));
	common_words.push_back(CategorialWord(codec->toUnicode("�������"),   colored_series ? (Green | Common): Common));
	common_words.push_back(CategorialWord(codec->toUnicode("������"),    colored_series ? (Green | Common): Common));
	common_words.push_back(CategorialWord(codec->toUnicode("�������"),   colored_series ? (Green | Common): Common));
	common_words.push_back(CategorialWord(codec->toUnicode("���������"), colored_series ? (Green | Common): Common));

	hand_words.reserve(10);
	hand_words.push_back(CategorialWord(codec->toUnicode("�������"),   colored_series ? (Red   | Hand) : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("������"),    colored_series ? (Red   | Hand) : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("�������"),   colored_series ? (Red   | Hand) : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("������"),    colored_series ? (Red   | Hand) : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("�������"),   colored_series ? (Red   | Hand) : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("������"),    colored_series ? (Green | Hand) : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("�������"),   colored_series ? (Green | Hand) : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("������"),    colored_series ? (Green | Hand) : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("���������"), colored_series ? (Green | Hand) : Hand));
	hand_words.push_back(CategorialWord(codec->toUnicode("��������"),  colored_series ? (Green | Hand) : Hand));

	leg_words.reserve(10);
	leg_words.push_back(CategorialWord(codec->toUnicode("������"),     colored_series ? (Red   | Leg) : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("��������"),   colored_series ? (Red   | Leg) : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("������"),     colored_series ? (Red   | Leg) : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("���������"),  colored_series ? (Red   | Leg) : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("�������"),    colored_series ? (Red   | Leg) : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("������"),     colored_series ? (Green | Leg) : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("������"),     colored_series ? (Green | Leg) : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("������"),     colored_series ? (Green | Leg) : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("�������"),    colored_series ? (Green | Leg) : Leg));
	leg_words.push_back(CategorialWord(codec->toUnicode("�������"),    colored_series ? (Green | Leg) : Leg));

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

void ChangeColor(QList<CategorialWord> & list)
{
	uint tmp;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		tmp = (*it).category;
		(*it).category = (tmp & Red) ? (tmp & 7 | Green) : (tmp & 7 | Red);
	}
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

		if (is_colored && (i & 1))
		{
			ChangeColor(hand_words);
			ChangeColor(leg_words);
			ChangeColor(common_words);
		}

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

void ExpirementController::NextTrial(bool failed)
{
	if (failed)
	{
		// backup failed trial
		int idx = ((static_cast<double>(qrand())) / RAND_MAX) * (formed_for_experiment.size() - show_idx - 1) + show_idx + 1;
		formed_for_experiment.insert(idx, formed_for_experiment[show_idx]);
	}

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
