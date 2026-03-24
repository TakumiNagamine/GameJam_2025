#include "Answer.h"
#include "../../Utility/ResourceManager.h"
#include "Dxlib.h"

Answer::Answer()
	: book_image()
	, answer_image()
	, num_image()
	, text_image()
	, answer_image_y(250.0f)
	, answer_result()
	, answer_reset_flag(false)
	, answer_anim_flag(false)
	, answer_anim_count(30)
	, display_answer_flag(false)
	, correct_answer_flag(false)
	, now_level(0)
{

}

Answer::~Answer()
{
}

void Answer::Initialize()
{
	// リソース管理のインスタンスを取得
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int> tmp;
	// 本画像の読み込み
	book_image = rm->GetImages("Resource/Images/Scene/InGame/Book.png", 2, 1, 2, 160, 75);
	// ○×画像の読み込み
	answer_image = rm->GetImages("Resource/Images/Object/Mark/Circle.png");
	tmp = rm->GetImages("Resource/Images/Object/Mark/Cross.png");
	answer_image.push_back(tmp[0]);
	// 数字画像の読み込み
	num_image = rm->GetImages("Resource/Images/Scene/InGame/Text/Num.png", 10, 10, 1, 49, 80);
	// "お題"テキスト読み込み
	text_image = rm->GetImages("Resource/Images/Scene/InGame/Text/Theme.png");

}

void Answer::Update()
{
	if (answer_anim_flag)
	{
		answer_anim_count--;
		if (answer_anim_count <= 0)
		{
			answer_anim_count = 30;
			answer_anim_flag = false;
		}
	}

	if (answer_reset_flag && answer_anim_flag == false)
	{
		answer_reset_flag = false;
		// 回答結果をリセット
		answer_result.clear();
	}
}

void Answer::Draw() const
{
	float book_start_y = 180.0f;
	float book_end_y = book_start_y + 40.0f + (now_level + 1) * 50.0f;
	// お題の進行状況描画
	DrawRotaGraphF(120.0f, book_start_y, 1.2, 0.0, book_image[0], TRUE);
	DrawBoxAA(24.0f, book_start_y, 216.0f, book_end_y, 0xfafafa, TRUE);
	DrawRotaGraphF(120.0f, book_end_y, 1.2, 0.0, book_image[1], TRUE);

	DrawRotaGraphF(120.0f, 190.0f, 0.9, 0.0, text_image[0], TRUE);

	double default_scale = 0.6;
	double highlight_scale = 0.8;

	for (int i = 0; i <= now_level; i++)
	{
		double scale = default_scale;

		if (answer_anim_flag && i == answer_result.size() - 1) // 回答直後
		{
			scale = highlight_scale;
		}
		else if (answer_anim_flag == false && i == answer_result.size() && answer_reset_flag == false)  // 次に回答する問題
		{
			scale = highlight_scale;
		}

		DrawRotaGraphF(70.0f, answer_image_y + (i * 50.0f), scale, 0.0, num_image[i + 1], TRUE);
	}

	DrawAnswer();

}

void Answer::Finalize()
{
}

void Answer::DrawAnswer() const
{
	double default_scale = 0.8;
	double animation_scale = 1.2;

	for (int i = 0; i < answer_result.size(); i++)
	{
		double scale = default_scale;
		if (answer_anim_flag && i == answer_result.size() - 1)
		{
			// 拡大する
			scale = animation_scale;
		}

		if (answer_result[i] == 1)
		{
			// ○画像描画
			DrawRotaGraphF(130.0f, answer_image_y + (i * 50.0f), scale, 0.0, answer_image[0], TRUE);  // ○
		}
		else
		{
			// ×画像描画
			DrawRotaGraphF(130.0f, answer_image_y + (i * 50.0f), scale, 0.0, answer_image[1], TRUE);  // ×
		}
	}
}

void Answer::AnimStart()
{
	// 回答アニメーションを再生する
	answer_anim_flag = true;
	answer_anim_count = 30;
}

void Answer::SetCorrectAnswerFlag(bool set_flag)
{
	if (set_flag)
	{
		// 結果に"正解"を格納
		answer_result.push_back(1);
	}
	else
	{
		// 結果に"不正解"を格納
		answer_result.push_back(0);
	}
}

void Answer::SetResetFlag()
{
	answer_reset_flag = true;
}

void Answer::SetNowLevel(int set_level)
{
	now_level = set_level;
}
