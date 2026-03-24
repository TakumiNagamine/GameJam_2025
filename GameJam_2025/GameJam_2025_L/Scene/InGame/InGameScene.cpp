#include "InGameScene.h"
#include "../../Utility/InputManager.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Bell/Bell.h"
#include "../../Object/Note/Note.h"
#include "../../Object/Answer/Answer.h"
#include "../../Problem/ProblemGenerator.h"
#include "../../Effect/Fade.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"

InGameScene::InGameScene()
	: problem(nullptr)
	, fade(nullptr)
	, player(nullptr)
	, bell(nullptr)
	, answer(nullptr)
	, change_scene_flag(false)
	, ui_image()
	, text_image()
	, timer_num_image()
	, timer_num(3)
	, count_down_timer(60)
	, state(eInGameState::eCountDown)
	, obj_update_flag(false)
	, curtain_x(50.0f)
	, curtain_y(500.0f)
	, change_scene_count(120)
	, ui_alpha(0)
{

}

InGameScene::~InGameScene()
{

}

void InGameScene::Initialize()
{
	// 背景画像読み込み
	background_image = LoadGraph("Resource/Images/Scene/InGame/Background01.png");
	// リソース管理のインスタンスを取得
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int> tmp;
	// UI画像の読み込み
	ui_image = rm->GetImages("Resource/Images/Scene/Tutorial/Text/Operation.png");
	tmp = rm->GetImages("Resource/Images/Scene/InGame/Level1.png");
	ui_image.push_back(tmp[0]);
	// テキスト画像の読み込み
	text_image = rm->GetImages("Resource/Images/Scene/InGame/Text/Start.png");
	tmp = rm->GetImages("Resource/Images/Scene/InGame/Text/Level.png");
	text_image.push_back(tmp[0]);
	tmp = rm->GetImages("Resource/Images/Scene/InGame/Text/End.png");
	text_image.push_back(tmp[0]);
	tmp = rm->GetImages("Resource/Images/Scene/InGame/Text/Theme.png");
	text_image.push_back(tmp[0]);
	// 数字画像の読み込み
	timer_num_image = rm->GetImages("Resource/Images/Scene/InGame/Text/Num.png", 10, 10, 1, 49, 80);
	
	background_img = rm->GetImages("Resource/Images/Scene/InGame/Background01.png");
	tmp = rm->GetImages("Resource/Images/Scene/Tutorial/Background.png");
	background_img.push_back(tmp[0]);

	// 時間の画像読み込み（秒）
	LoadDivGraph("Resource/Images/Scene/InGame/Text/NumWhite.png", 10, 10, 1, 49, 81, second_images);

	// 時間の画像読み込み（分）
	LoadDivGraph("Resource/Images/Scene/InGame/Text/NumWhite.png", 10, 10, 1, 49, 81, min_images);

	// メインbgm読み込み
	InGame_main_bgm = LoadSoundMem("Resource/Sounds/BGM/Tutorial_main.mp3");

	// カウントの心拍音の読み込み
	sinpaku = LoadSoundMem("Resource/Sounds/SE/sinpaku.mp3");

	// ライトの音読み込み
	light_sound = LoadSoundMem("Resource/Sounds/SE/light.mp3");

	// 問題ジェネレータをインスタンス化
	problem = new ProblemGenerator();
	// フェードをインスタンス化
	fade = new Fade();
	// フェードの初期化処理（フェードイン）
	fade->Initialize(true);

	// 鐘の生成、生成座標の設定
	bell = CreateObject<Bell>(Vector2D(640.0f, 350.0f));
	// プレイヤーの生成、生成座標の設定
	player = CreateObject<Player>(Vector2D(390.0f, 550.0f));
	// 回答結果の生成、生成座標の設定
	answer = CreateObject<Answer>(Vector2D(390.0f, 550.0f));
	
	// 鐘にプレイヤーのポインタを渡す
	bell->SetPlayer(player);
	// 鐘に問題ジェネレータのポインタを渡す
	bell->SetProblemGenerator(problem);

	// プレイヤーを操作可能にする
	player->SetOperableFlag(true);

	// InGameのメインbgmを流す
	PlaySoundMem(InGame_main_bgm, DX_PLAYTYPE_BACK);

	light_flag = false;
}

eSceneType InGameScene::Update()
{
	if (fade->GetEndFlag() == true)
	{
		// フェード処理が終了しているなら

		switch (state)
		{
		case InGameScene::eInGameState::eCountDown:
			// カウントダウン処理
			CountDown();
			// 舞台幕を上げていく
			curtain_y -= 3.0f;

			if (ui_alpha < 255)
			{
				// UI画像を徐々に濃く
				ui_alpha += 5;
			}
			else
			{
				ui_alpha = 255;
			}

			if (timer_num == 0)
			{
				// 鐘とプレイヤーに影を設定する
				bell->SetShadowFlag();
				player->SetShadowFlag();
			}
			if (timer_num < 0)
			{
				state = eInGameState::ePlay;
				timer_num = 30;
			}
			break;

		case InGameScene::eInGameState::ePlay:

			// カウントダウン処理
			CountDown();

			// オブジェクトの更新を行えるように
			obj_update_flag = true;

			// プレイヤーを操作可能にする
			player->SetOperableFlag(true);

			answer->SetNowLevel(problem->GetLevel());

			// 制限時間が0になった場合
			if (timer_num <= 0)
			{
				// プレイヤーを操作不可にする
				player->SetOperableFlag(false);

				// ゲーム終了をオブジェクトに知らせる
				bell->SetGameEndFlag();
				player->SetGameEndFlag();

				// メインサウンドを止める
				StopSoundMem(InGame_main_bgm);

				state = eInGameState::eEnd;
			}

			// プレイヤーの回答があったなら
			if (bell->GetCreateNoteFlag() == true)
			{
				answer->AnimStart();

				answer->SetCorrectAnswerFlag(bell->GetCorrectFlag());

				// 音符エフェクト生成処理
				CreateEffect();

				if (bell->GetPresentationFlag())
				{
					// お題を新しく発表するなら回答結果をリセット
					answer->SetResetFlag();
				}
			}
			break;

		case InGameScene::eInGameState::eEnd:

			// 舞台幕を下げていく
			curtain_y += 4.0f;

			if (ui_alpha > 0)
			{
				// UI画像を徐々に薄く
				ui_alpha -= 5;
			}
			else
			{
				ui_alpha = 0;
			}

			if (change_scene_count > 0)
			{
				change_scene_count--;
			}
			else
			{
				if (change_scene_flag)
				{
					// ゲーム結果をファイルに保存する
					problem->SaveGameResult();
					// メインサウンドを止める
					StopSoundMem(InGame_main_bgm);
					// リザルト画面に遷移する
					return eSceneType::eResult;
				}
				else
				{
					// フェードアウト
					fade->Initialize(false);
					// シーン変更可能
					change_scene_flag = true;
				}
			}
			break;

		default:
			break;
		}


		// ↓、後で削除する

		// キーボード入力制御のインスタンスを取得
		InputManager* input = InputManager::GetInstance();

		if (input->GetKeyInputState(KEY_INPUT_Z) == eInputState::ePress)
		{
			// ゲーム結果をファイルに保存する
			problem->SaveGameResult();
			// メインサウンドを止める
			StopSoundMem(InGame_main_bgm);
			// Zキーが押されたらリザルト画面に遷移
			return eSceneType::eResult;
		}

		if (obj_update_flag)
		{
			// 基底クラスのUpdateを呼び出し、その戻り値を返す
			return __super::Update();
		}
		else
		{
			return GetNowSceneType();
		}
	}
	else
	{
		// フェード更新
		fade->Update();

		if (change_scene_flag)
		{
			// 舞台幕を下げていく
			curtain_y += 4.0f;
		}

		return GetNowSceneType();
	}
}

void InGameScene::Draw() const
{
	if (state == eInGameState::eCountDown && timer_num >= 1)
	{
		DrawGraph(0, 0, background_img[1], TRUE);
	}
	else
	{
		DrawGraph(0, 0, background_img[0], TRUE);
	}

	__super::Draw();

	//DrawString(10, 10, "InGame", GetColor(255, 255, 255));
	//DrawString(10, 26, "Z : Result", GetColor(255, 255, 255));

	// 残り時間の描画
	DrawRotaGraph(650.0f, 50.0f, 1.0, 0, second_images[timer_num % 10], TRUE);
	DrawRotaGraph(600.0f, 50.0f, 1.0, 0, second_images[timer_num / 10], TRUE);

	DrawBoxAA(24.0f, 5.0f, 216.0f, 135.0f, 0xaaaaff, TRUE);
	//DrawCircleAA(120.0f, 80.0f, 80.0f, 32, 0xaaaaff, FALSE, 10.0f);

	// レベル描画
	DrawRotaGraphF(120.0f, 70.0f, 1.0, 0.0, ui_image[1], TRUE);
	DrawRotaGraphF(120.0f, 30.0f, 0.65, 0.0, text_image[1], TRUE);
	DrawRotaGraphF(120.0f, 90.0f, 1.0, 0.0, timer_num_image[problem->GetLevel() + 1], TRUE);

	switch (state)
	{
	case InGameScene::eInGameState::eCountDown:

		// 描画ブレンドモードをアルファブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ui_alpha);
		// ボタン案内表示
		DrawRotaGraphF(1150.0f, 650.0f, 1.0, 0.0, ui_image[0], TRUE);
		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		if (state == eInGameState::eCountDown && timer_num >= 1)
		{
			// 画面全体を暗くする
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
			DrawBoxAA(0.0f, 0.0f, 1280.0f, 720.0f, 0x000000, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			PlaySoundMem(light_sound, DX_PLAYTYPE_BACK);

		}

		// 舞台幕っぽく図形描画
		// 四角
		DrawBoxAA(0.0f, 0.0f, 1280.0f, curtain_y, 0x440000, TRUE);
		for (int i = 0; i < 10; i++)
		{
			// 丸
			DrawCircleAA(curtain_x + (i * 200.0f), curtain_y, 200.0f, 32, 0x440000, TRUE);
		}

		// フェードインが終了してから
		if (fade->GetEndFlag() == true)
		{
			if (timer_num > 0)
			{
				// 数字を描画
				DrawRotaGraphF(640.0f, 360.0f, 2.0, 0.0, timer_num_image[timer_num], TRUE);
			}
			else
			{
				// "演奏開始"テキスト描画
				DrawRotaGraphF(640.0f, 360.0f, 1.5, 0.0, text_image[0], TRUE);
			}
		}
		break;

	case InGameScene::eInGameState::ePlay:
		// ボタン案内表示
		DrawRotaGraphF(1150.0f, 650.0f, 1.0, 0.0, ui_image[0], TRUE);
		break;

	case InGameScene::eInGameState::eEnd:
		// 描画ブレンドモードをアルファブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ui_alpha);
		// ボタン案内表示
		DrawRotaGraphF(1150.0f, 650.0f, 1.0, 0.0, ui_image[0], TRUE);
		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 舞台幕っぽく図形描画
		// 四角
		DrawBoxAA(0.0f, 0.0f, 1280.0f, curtain_y, 0x660000, TRUE);
		for (int i = 0; i < 10; i++)
		{
			// 丸
			DrawCircleAA(curtain_x + (i * 200.0f), curtain_y, 200.0f, 32, 0x660000, TRUE);
		}

		// "演奏終了"テキスト描画
		DrawRotaGraphF(640.0f, 360.0f, 1.5, 0.0, text_image[2], TRUE);
		break;

	default:
		break;
	}

	//DrawFormatString(120, 120, 0x000000, "timer%d", timer_num);

	// フェード描画
	fade->Draw();
}

void InGameScene::Finalize()
{
	__super::Finalize();

	// メモリ解放
	delete problem;
	delete fade;

	if (background_image != NULL)
	{
		DeleteGraph(background_image);
	}
}

eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::eInGame;
}

// カウントダウン処理
void InGameScene::CountDown()
{
	if (count_down_timer > 0)
	{
		count_down_timer--;
	}
	else
	{
		timer_num--;
		count_down_timer = 60;
	}
}

// エフェクト生成処理
void InGameScene::CreateEffect()
{
	// プレイヤーがいるレーンの鐘の座標を求める
	float lane = 640.0f + (-250.0f + player->GetMoveLaneNum() * 100.0f);
	// 音符生成
	Note* note = CreateObject<Note>(Vector2D(lane, 200.0f));
	// 音符の色を設定する
	note->SetNoteColor(bell->GetCorrectFlag());
}
