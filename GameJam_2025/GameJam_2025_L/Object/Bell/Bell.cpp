#include "Bell.h"
#include "../Player/Player.h"
#include "../../Problem/ProblemGenerator.h"
#include "../../Utility/ResourceManager.h"
#include "Dxlib.h"

// コンストラクタ
Bell::Bell()
	: player(nullptr)
	, problem(nullptr)
	, now_target_num(0)
	, correct_answer_flag(false)
	, question()
	, problem_num(0)
	, problem_presentation_flag(true)
	, presentation_count(0)
	, next_presentation_wait_time(0)
	, one_note_presentation_count(0)
	, animation()
	, stand_image(NULL)
	, create_note_flag(false)
	, shadow_flag(false)
	, game_end_flag(false)
{

}

// デストラクタ
Bell::~Bell()
{

}

// 初期化処理
void Bell::Initialize()
{
	presentation_count = 60;
	one_note_presentation_count = presentation_count;

	// リソース管理のインスタンスを取得
	ResourceManager* rm = ResourceManager::GetInstance();
	// 鐘アニメーション画像の読み込み
	animation = rm->GetImages("Resource/Images/Object/Bell/Bell.png", 6, 6, 1, 100, 452);
	stand_image = LoadGraph("Resource/Images/Object/Bell/Stand.png");
	// 不正解と正解の音を読み込み
	False_Sounds = LoadSoundMem("Resource/Sounds/SE/False.mp3");  // 不正解
	True_Sounds = LoadSoundMem("Resource/Sounds/SE/True.mp3");    // 正解

	// 鐘の音を読み込む
	bell_sounds[0] = LoadSoundMem("Resource/Sounds/SE/BellSounds/do.mp3");
	bell_sounds[1] = LoadSoundMem("Resource/Sounds/SE/BellSounds/re.mp3");
	bell_sounds[2] = LoadSoundMem("Resource/Sounds/SE/BellSounds/mi.mp3");
	bell_sounds[3] = LoadSoundMem("Resource/Sounds/SE/BellSounds/fa.mp3");
	bell_sounds[4] = LoadSoundMem("Resource/Sounds/SE/BellSounds/so.mp3");
	bell_sounds[5] = LoadSoundMem("Resource/Sounds/SE/BellSounds/ra.mp3");
}

// 更新処理
void Bell::Update()
{
	// ゲーム終了なら関数を抜ける
	if (game_end_flag) return;

	create_note_flag = false;

	if (problem_presentation_flag)
	{
		// お題発表中の場合

		// 次のお題発表までのカウントダウン
		next_presentation_wait_time--;

		if (next_presentation_wait_time <= 30 && correct_answer_flag)
		{
			// 全問正解ならレベルを上げて次の問題を生成する
			problem->LevelUp();
			// 現在のお題を取得
			question = problem->GetCurrentProblem();
			// 次の対象の鐘を設定する
			now_target_num = problem->GetCurrentTarget();

			correct_answer_flag = false;
		}

		if (next_presentation_wait_time <= 0)
		{
			one_note_presentation_count--;

			if (one_note_presentation_count <= 0)
			{
				one_note_presentation_count = presentation_count;
				// お題発表処理
				Question();
			}
		}
	}
	else
	{
		// お題発表後の場合

		// プレイヤーが鐘を打った場合
		if (player->GetRingFlag() == true)
		{
			// 音符生成
			create_note_flag = true;

			// プレイヤーの入力と打つ対象の鐘が同じなら
			if (problem->CheckAnswer(player->GetMoveLaneNum()) == true)
			{
				// 鐘の音を再生
				if (player->GetMoveLaneNum() >= 0 && player->GetMoveLaneNum() < 6)
				{
					PlaySoundMem(bell_sounds[player->GetMoveLaneNum()], DX_PLAYTYPE_BACK);
				}
				// 正解
				correct_answer_flag = true;

				if (problem->CheckAllCorrect() == true)
				{
					// 正解の音鳴らす
					PlaySoundMem(True_Sounds, DX_PLAYTYPE_BACK);

					if (problem->GetLevel() <= 4)
					{
						// レベルが4以下の場合

						if (problem->GetLevel() % 2 == 0)
						{
							// レベルが2の倍数だったら

							// 発表時間を速くする
							presentation_count = presentation_count / 1.5f;
						}
					}

					// 発表する
					problem_presentation_flag = true;
					// 発表までの待ち時間を設定する
					next_presentation_wait_time = 60;
					// プレイヤーにお題発表フラグを設定する
					player->SetPresentationFlag(problem_presentation_flag);
				}
			}
			else
			{
				// 不正解
				correct_answer_flag = false;
				// 不正解の音を鳴らす
				PlaySoundMem(False_Sounds, DX_PLAYTYPE_BACK);
				// 新しい問題を生成する
				problem->CreateProblem();
				// 現在のお題を取得
				question = problem->GetCurrentProblem();
				// 発表する
				problem_presentation_flag = true;
				// 発表までの待ち時間を設定する
				next_presentation_wait_time = 60;
				// プレイヤーにお題発表フラグを設定する
				player->SetPresentationFlag(problem_presentation_flag);
				// 次の対象の鐘を設定する
				now_target_num = problem->GetCurrentTarget();
			}
		}
	}
}

// 描画処理
void Bell::Draw() const
{
	// 親クラスの描画関数呼び出し
	//__super::Draw();

	if (shadow_flag)
	{
		// 描画ブレンドモードをアルファブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		// スタンドの足元に影が来るように楕円描画
		DrawOvalAA(location.x, 570.0f, 300.0f, 30.0f, 32, 0x000000, TRUE);
		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// スタンド画像描画
	DrawRotaGraphF(location.x, location.y, 0.95, 0.0, stand_image, TRUE);

	// 鐘の1つ当たりの幅を求める
	int bell_width = 600 / 6;
	for (int i = 0; i < 6; i++)
	{
		// 本数確認用（中心座標から-250.0fした位置から描画していく）
		DrawRotaGraphF(location.x + (-250.0f + i * 100.0f), location.y, 1.0, 0.0, animation[i], TRUE);
	}

	if (problem_presentation_flag)
	{
		// お題発表中の場合

		if (one_note_presentation_count <= presentation_count / 2)
		{
			// 対象の鐘を順番に光らせる
			// 打つ対象の鐘

			//色をかぶせる
			SetDrawBright(255, 0, 0);
			DrawRotaGraphF(location.x + (-250.0f + now_target_num * 100.0f), location.y, 1.0, 0.0, animation[now_target_num], TRUE);
			SetDrawBright(255, 255, 255);
		}
	}

#if _DEBUG

	//DrawFormatString(10, 200, 0xffffff, "now_target_num: %d", now_target_num);
	//DrawFormatString(10, 220, 0xffffff, "problem_num: %d", problem_num);
	//DrawFormatString(10, 20, 0x000000, "problem_presentation_flag: %d", problem_presentation_flag);
	//DrawFormatString(10, 50, 0x000000, "next_presentation_wait_time: %d", next_presentation_wait_time);

	//DrawFormatString(600, 150, 0x000000, "problem");
	//for (int i = 0; i < question.size(); i++)
	//{
	//	DrawFormatString(600, 170 + (i * 20), 0x000000, "target: %d", question[i]);
	//}

#endif // _DEBUG

}

// 終了処理
void Bell::Finalize()
{
}

// アニメーション制御処理
void Bell::AnimationControl()
{

}

// プレイヤーポインタを設定する
void Bell::SetPlayer(Player* p)
{
	player = p;
}

// 問題ジェネレータを設定する
void Bell::SetProblemGenerator(ProblemGenerator* p)
{
	problem = p;

	// 問題生成処理
	problem->CreateProblem();
	// 現在のお題を取得
	question = problem->GetCurrentProblem();

	// 打つ対象の鐘を設定する
	now_target_num = problem->GetCurrentTarget();
}

// 正解フラグを取得する
bool Bell::GetCorrectFlag() const
{
	return correct_answer_flag;
}

// 音符生成フラグを取得する
bool Bell::GetCreateNoteFlag() const
{
	return create_note_flag;
}

// 影付けフラグをtrueに設定する
void Bell::SetShadowFlag()
{
	// 影をつける
	shadow_flag = true;
}

// チュートリアルフラグをtrueに設定する
void Bell::SetGameEndFlag()
{
	game_end_flag = true;
}

// お題発表中フラグを取得する
bool Bell::GetPresentationFlag() const
{
	return problem_presentation_flag;
}

// 出題処理
void Bell::Question()
{
	// 次のお題の鐘
	problem_num++;

	if (problem_num < question.size())
	{
		// 光らせる鐘を更新する
		now_target_num = question[problem_num];
		
	}
	else
	{
		problem_num = 0;

		// 発表終了
		problem_presentation_flag = false;
		// プレイヤーにお題発表フラグを設定する
		player->SetPresentationFlag(problem_presentation_flag);
	}
}