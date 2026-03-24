#include "TitleScene.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/PadInputManager.h"
#include "../../Effect/Fade.h"
#include "DxLib.h"

void TitleScene::Initialize()
{
	title_cursol_system = 0;
	title_stick_time = 15.0f;
	next_scene = eSceneType::eTitle;

	// 画像
	background_image = LoadGraph("Resource/Images/Scene/Title/background.png");
	button_guid_image = LoadGraph("Resource/Images/Scene/Title/Operation.png");
	cursol_image = LoadGraph("Resource/Images/Scene/Title/cursol.png");
	cursol_push_image = LoadGraph("Resource/Images/Scene/Title/push.png");
	title_image = LoadGraph("Resource/Images/Scene/Title/title.png");
	subtitle_image = LoadGraph("Resource/Images/Scene/Title/subtitle.png");
	title_play_image = LoadGraph("Resource/Images/Scene/Title/play.png");
	title_ranking_image = LoadGraph("Resource/Images/Scene/Title/ranking.png");
	title_end_image = LoadGraph("Resource/Images/Scene/Title/end.png");

	// bgmの読み込み
	title_bgm = LoadSoundMem("Resource/Sounds/BGM/Title_plan1.mp3");
	// SEの読み込み
	cursol_move_se = LoadSoundMem("Resource/Sounds/SE/cursol_move_se.mp3");
	cursol_push_se = LoadSoundMem("Resource/Sounds/SE/cursol_push_se.mp3");

	// ずっとメインbgmを流す
	PlaySoundMem(title_bgm, DX_PLAYTYPE_LOOP);

	// フェードをインスタンス化
	fade = new Fade();
	// フェードの初期化処理（フェードイン）
	fade->Initialize(true);
}

eSceneType TitleScene::Update()
{
	// パッド入力制御のインスタンスを取得
	PadInputManager* pad_input = PadInputManager::GetInstance();

	// 1. スティック用タイマー更新
	title_stick_time--;
	if (title_stick_time <= 0.0f) { title_stick_time = 15.0f; }

	// 2. フェードの更新（常に実行）
	fade->Update();

	// 3. シーン遷移の予約がある場合（フェードアウト中）
	if (next_scene != eSceneType::eTitle)
	{
		// 完全に暗くなったら（フェードアウト完了）次のシーンへ
		if (fade->GetEndFlag())
		{
			return next_scene;
		}
		// 暗くしている最中なので、入力を受け付けずタイトルを維持
		return eSceneType::eTitle;
	}

	// 4. 通常時の入力処理（フェードイン完了後のみ操作可能にする）
	// ※フェードイン中も操作させたいなら、この if は外してOK
	if (fade->GetEndFlag())
	{
		// --- 十字キー・スティック操作 (中身はそのまま) ---
		if (pad_input->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == ePadInputState::ePress ||
			(pad_input->GetLeftStickTiltPercentage().y > 0.5f && title_stick_time == 15.0f))
		{
			title_cursol_system = (title_cursol_system + 2) % 3; // 2 -> 1 -> 0 -> 2
			PlaySoundMem(cursol_move_se, DX_PLAYTYPE_BACK);
		}

		if (pad_input->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == ePadInputState::ePress ||
			(pad_input->GetLeftStickTiltPercentage().y < -0.5f && title_stick_time == 15.0f))
		{
			title_cursol_system = (title_cursol_system + 1) % 3; // 0 -> 1 -> 2 -> 0
			PlaySoundMem(cursol_move_se, DX_PLAYTYPE_BACK);
		}

		// --- 決定ボタン処理 ---
		if (pad_input->GetButtonInputState(XINPUT_BUTTON_A) == ePadInputState::ePress)
		{
			PlaySoundMem(cursol_push_se, DX_PLAYTYPE_BACK);
			StopSoundMem(title_bgm);

			// フェードアウト開始の設定
			fade->Initialize(false); // 暗くするモードへ

			// 行き先を予約
			if (title_cursol_system == 0) next_scene = eSceneType::eTutorial;
			else if (title_cursol_system == 1) next_scene = eSceneType::eRanking;
			else if (title_cursol_system == 2) next_scene = eSceneType::eEnd;
		}
	}

	return eSceneType::eTitle;
}

void TitleScene::Draw() const
{

	// パッド入力制御のインスタンスを取得
	PadInputManager* pad_input = PadInputManager::GetInstance();

	// 背景画像
	DrawGraph(0, 0, background_image, FALSE);

	// タイトルロゴ
	DrawRotaGraph(630, 40, 1.0f, DX_PI / 0.5, title_image, TRUE);

	// サブタイトルロゴ
	DrawRotaGraph(650, 90, 0.8f, DX_PI / 0.5, subtitle_image, TRUE);

	// プレイロゴ
	DrawRotaGraph(620, 500, 1.0f, DX_PI / 0.5, title_play_image, TRUE);

	// ランキングロゴ
	DrawRotaGraph(670, 580, 1.0f, DX_PI / 0.5, title_ranking_image, TRUE);

	// エンドロゴ
	DrawRotaGraph(620, 660, 1.0f, DX_PI / 0.5, title_end_image, TRUE);

	// ボタン操作説明
	DrawRotaGraph(1150, 650, 1.0f, DX_PI / 0.5, button_guid_image, TRUE);

	// カーソル画像
	DrawRotaGraph(500, 500 + title_cursol_system * 80, 0.2, DX_PI / 0.1, cursol_image, TRUE);

	// フェード描画
	fade->Draw();
}

void TitleScene::Finalize()
{
	if (background_image != NULL)
	{
		DeleteGraph(background_image);
	}
}

eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::eTitle;
}
