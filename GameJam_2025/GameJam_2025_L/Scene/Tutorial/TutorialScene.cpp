#include "TutorialScene.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/PadInputManager.h"
#include "../../Utility/ResourceManager.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Bell/Bell.h"
#include "../../Object/Note/Note.h"
#include "../../Object/Answer/Answer.h"
#include "../../Problem/ProblemGenerator.h"
#include "../../Effect/Fade.h"
#include "DxLib.h"

// コンストラクタ
TutorialScene::TutorialScene()
	: problem(nullptr)
	, fade(nullptr)
	, player(nullptr)
	, bell(nullptr)
	, answer(nullptr)
	, change_scene_flag(false)
	, page_num(0)
	, text_image()
	, text_alpha(0)
	, text_fade_in_flag(true)
	, state(eTutoriaState::eStart)
	, change_text_count(120)
	, ui_image()
	, ui_alpha(0)
	, obj_update_flag(false)
{

}

// デストラクタ
TutorialScene::~TutorialScene()
{

}

// 初期化処理
void TutorialScene::Initialize()
{
	// 背景画像読み込み
	background_image = LoadGraph("Resource/Images/Scene/Tutorial/Background.png");
	// リソース管理のインスタンスを取得
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int> tmp;

	// 説明テキスト画像の読み込み
	text_image = rm->GetImages("Resource/Images/Scene/Tutorial/Text/Practice.png");
	tmp = rm->GetImages("Resource/Images/Scene/Tutorial/Text/Rule.png");
	text_image.push_back(tmp[0]);
	tmp = rm->GetImages("Resource/Images/Scene/Tutorial/Text/Good.png");
	text_image.push_back(tmp[0]);

	// UI画像の読み込み
	ui_image = rm->GetImages("Resource/Images/Scene/Tutorial/Text/Operation.png");
	tmp = rm->GetImages("Resource/Images/Scene/Tutorial/Text/Skip.png");
	ui_image.push_back(tmp[0]);

	// 問題ジェネレータをインスタンス化
	problem = new ProblemGenerator();
	// フェードをインスタンス化
	fade = new Fade();
	// フェードの初期化処理（フェードイン）
	fade->Initialize(true);

	// チュートリアルのメインbgm読み込み
	Tutorial_main_bgm = LoadSoundMem("Resource/Sounds/BGM/Title_plan1.mp3");
	// カーソルが動くときのSE
	Tutorial_cursol_move_se = LoadSoundMem("Resource/Sounds/SE/cursol_move_se.mp3");
	// 決定ボタンを押したときのSE
	Tutorial_cursol_push_se = LoadSoundMem("Resource/Sounds/SE/cursol_push_se.mp3");

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

	answer->SetNowLevel(problem->GetLevel());

	// メインbgmを流す
	PlaySoundMem(Tutorial_main_bgm, DX_PLAYTYPE_BACK);
}

// 更新処理
eSceneType TutorialScene::Update()
{
	if (fade->GetEndFlag() == true)
	{
		// パッド入力制御のインスタンスを取得
		PadInputManager* pad_input = PadInputManager::GetInstance();
		if (pad_input->GetButtonInputState(XINPUT_BUTTON_START) == ePadInputState::ePress)
		{
			// スタートボタンのSEを鳴らす
			PlaySoundMem(Tutorial_cursol_push_se, DX_PLAYTYPE_BACK);
			// スタートボタンを押された場合
			
			// メインサウンドを止める
			StopSoundMem(Tutorial_main_bgm);
			//チュートリアルを終了する
			state = eTutoriaState::eEnd;
		}

		// フェード処理が終了しているなら
		switch (state)
		{
		case TutorialScene::eTutoriaState::eStart:
			if (text_fade_in_flag)
			{
				// テキストのフェードイン処理
				TextFadeIn();
			}
			else
			{
				if (text_alpha > 0)
				{
					// テキストフェードアウト処理
					TextFadeAut();
				}
				else
				{
					// 次のテキストはフェードインから
					text_fade_in_flag = true;
					// プレイ状態に変更する
					state = eTutoriaState::ePlay;
				}
			}
			break;

		case TutorialScene::eTutoriaState::ePlay:
			if (text_fade_in_flag)
			{
				// テキストのフェードイン処理
				TextFadeIn();
				ui_alpha = text_alpha;
			}
			else
			{
				// オブジェクトの更新を行えるように
				obj_update_flag = true;

				// プレイヤーを操作可能にする
				player->SetOperableFlag(true);

				// 1問クリアした場合
				if (problem->GetLevel() > 0)
				{
					// チュートリアル終了をオブジェクトに知らせる
					bell->SetGameEndFlag();
					player->SetGameEndFlag();

					if (text_alpha > 0)
					{
						// テキストフェードアウト処理
						TextFadeAut();
					}
					else
					{
						// 次のテキストはフェードインから
						text_fade_in_flag = true;
						// チュートリアル終了状態に変更する
						state = eTutoriaState::eEnd;
					}
				}
			}
			if (bell->GetCreateNoteFlag() == true)
			{
				answer->AnimStart();

				answer->SetCorrectAnswerFlag(bell->GetCorrectFlag());

				if (bell->GetCorrectFlag() == false)
				{
					// ミスしたら回答結果をリセット
					answer->SetResetFlag();
				}

				// 音符エフェクト生成処理
				CreateEffect();
			}

			break;

		case TutorialScene::eTutoriaState::eEnd:
			if (text_fade_in_flag)
			{
				// テキストのフェードイン処理
				TextFadeIn();
			}
			else
			{
				if (text_alpha > 0)
				{
					// テキストフェードアウト処理
					TextFadeAut();
				}
				else
				{
					if (change_scene_flag)
					{
						// メインサウンドを止める
						StopSoundMem(Tutorial_main_bgm);
						// インゲーム画面に遷移する
						return eSceneType::eInGame;
					}
					else
					{
						// フェードアウト
						fade->Initialize(false);
						// シーン変更可能
						change_scene_flag = true;
					}
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
			// メインサウンドを止める
			StopSoundMem(Tutorial_main_bgm);
			// Zキーが押されたらインゲーム画面に遷移
			return eSceneType::eInGame;
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

		return GetNowSceneType();
	}
}

// 描画処理
void TutorialScene::Draw() const
{
	DrawGraph(0, 0, background_image, TRUE);

	// 親クラスのオブジェクト描画処理
	__super::Draw();

	//DrawString(10, 10, "Tutorial", GetColor(255, 255, 255));
	//DrawString(10, 26, "Z : InGame", GetColor(255, 255, 255));

	// 描画ブレンドモードをアルファブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, text_alpha);
	// 説明テキスト画像描画
	switch (state)
	{
	case eTutoriaState::eStart:
		// "チュートリアル"画像表示
		DrawRotaGraphF(640.0f, 50.0f, 0.8, 0.0, text_image[0], TRUE);
		break;

	case eTutoriaState::ePlay:
		// ゲーム説明テキスト画像表示
		DrawRotaGraphF(640.0f, 50.0f, 0.8, 0.0, text_image[1], TRUE);
		break;

	case eTutoriaState::eEnd:
		DrawRotaGraphF(640.0f, 50.0f, 0.8, 0.0, text_image[2], TRUE);
		break;

	default:
		break;
	}
	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//DrawFormatString(10, 100, 0x000000, "state: %d", state);

	// 描画ブレンドモードをアルファブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ui_alpha);
	// ボタン案内表示
	// スキップ
	DrawRotaGraphF(1130.0f, 570.0f, 1.0, 0.0, ui_image[1], TRUE);
	// 操作方法
	DrawRotaGraphF(1150.0f, 650.0f, 1.0, 0.0, ui_image[0], TRUE);
	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// フェード描画
	fade->Draw();
}

// 終了処理
void TutorialScene::Finalize()
{
	// 親クラスのオブジェクト解放処理
	__super::Finalize();

	// メモリ解放
	delete problem;
	delete fade;

	if (background_image != NULL)
	{
		DeleteGraph(background_image);
	}
}

// 現在のシーンを取得する
eSceneType TutorialScene::GetNowSceneType() const
{
	return eSceneType::eTutorial;
}

// テキストのフェードイン処理
void TutorialScene::TextFadeIn()
{
	if (text_alpha < 255)
	{
		// テキスト画像を徐々に濃く
		text_alpha += 20;
	}
	else
	{
		change_text_count--;

		if (change_text_count <= 0)
		{
			change_text_count = 120;
			// フェードイン終了
			// 次回はフェードアウト処理から開始
			text_fade_in_flag = false;
		}
	}
}

// テキストのフェードアウト処理
void TutorialScene::TextFadeAut()
{
	if (text_alpha > 0)
	{
		// テキスト画像を徐々に薄く
		text_alpha -= 20;
	}
}

// エフェクト生成処理
void TutorialScene::CreateEffect()
{
	// プレイヤーがいるレーンの鐘の座標を求める
	float lane = 640.0f + (-250.0f + player->GetMoveLaneNum() * 100.0f);
	// 音符生成
	Note* note = CreateObject<Note>(Vector2D(lane, 200.0f));
	// 音符の色を設定する
	note->SetNoteColor(bell->GetCorrectFlag());
}
