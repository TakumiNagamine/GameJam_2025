#include "Note.h"
#include "../../Utility/ResourceManager.h"
#include "Dxlib.h"

//#include <stdio.h>
//#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

// コンストラクタ
Note::Note()
	: correct_answer_flag(false)
	, problem_presentation_flag(false)
	, note_image()
	, noise_image()
	, color_num(0)
	, life_time(0)
	, degree(0.0f)
	, move_direction(0)
{

}

// デストラクタ
Note::~Note()
{

}

// 初期化処理
void Note::Initialize()
{
	// リソース管理のインスタンスを取得
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int> tmp;

	// 音符画像読み込み
	note_image = rm->GetImages("Resource/Images/Object/Note/Note.png");
	// ノイズ画像読み込み
	noise_image = rm->GetImages("Resource/Images/Object/Note/Noise.png");

	// 音符寿命は3秒程度
	life_time = 180;

	// 移動方向を指定する（0～1）
	move_direction = rand() % 2;
}

// 更新処理
void Note::Update()
{
	life_time--;

	if (move_direction == 0)
	{
		// 左に移動する
		location.x -= 3.0f;
	}
	else
	{
		// 右に移動する
		location.x += 3.0f;
	}
	location.y -= 2.0f;

	if (life_time > 0)
	{
		float result = 0.0f;
		float radian = 0.0f;

		degree += 10.0f;
		if (degree > 360.0f)
		{
			// 360度より大きくなったら0に戻す
			degree = 0.0f;
		}

		// デグリーからラジアンへ角度変換
		radian = degree * (float)M_PI / 180.0f;
		// sinで-1～1を求める
		result = sinf(radian);
		// 移動
		location.y += result;
	}
	else
	{
		// 削除する
		delete_flag = true;
	}
}

// 描画処理
void Note::Draw() const
{
	if (correct_answer_flag == true)
	{
		// 正解だった場合

		// 色をかぶせる
		SetDrawBright(red[color_num], green[color_num], bule[color_num]);
		// 音符画像描画
		DrawRotaGraphF(location.x, location.y, 1.0, 0.0, note_image[0], TRUE);
		// 色を戻す
		SetDrawBright(255, 255, 255);
	}
	else
	{
		// 不正解だった場合
		// 
		// ノイズ画像描画
		DrawRotaGraphF(location.x, location.y, 1.0, 0.0, noise_image[0], TRUE);
	}
}

// 終了処理
void Note::Finalize()
{

}

// 音符の色を設定する
void Note::SetNoteColor(bool set_flag)
{
	correct_answer_flag = set_flag;

	if (correct_answer_flag == true)
	{
		// 正解した場合

		// 音符の色を設定する
		color_num = rand() % 3 + 1;
	}
}

// お題発表中フラグを設定する
void Note::SetPressentationFlag(bool set_flag)
{
	problem_presentation_flag = set_flag;
}
