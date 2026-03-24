#include "EndScene.h"
#include "../../Scene/SceneManager.h"
#include "../../Effect/Fade.h"
#include "DxLib.h"

void EndScene::Initialize()
{
	// フェードをインスタンス化
	fade = new Fade();
	// フェードの初期化処理（フェードイン）
	fade->Initialize(true);

	// 画像
	background_image = LoadGraph("Resource/Images/Scene/Tutorial/Background.png");
	ty_text_image = LoadGraph("Resource/Images/Scene/End/ty.png");
	sound_site_image = LoadGraph("Resource/Images/Scene/End/sound_site.png");
	picture_site_image = LoadGraph("Resource/Images/Scene/End/pic_site.png");

	// BGM
	End_bgm = LoadSoundMem("Resource/Sounds/BGM/End.mp3");
	PlaySoundMem(End_bgm, DX_PLAYTYPE_BACK);

	// エンド終了保有秒数の初期値
	all_end_time = 0;
}

eSceneType EndScene::Update()
{
	// 秒数加算
	all_end_time++;

	// 約5秒で終了
	if (all_end_time == 295.0f)
	{
		exit(0);
	}

	// フェード更新
	fade->Update();

	return GetNowSceneType();
}

void EndScene::Draw() const
{
	// 背景画像
	DrawGraph(0, 0, background_image, FALSE);

	// 「thanks you for playing」描写
	DrawRotaGraph(770, 150, 1.5f, DX_PI / 0.5, ty_text_image, TRUE);

	// 使用サウンドサイト
	DrawRotaGraph(350, 400, 1.0f, DX_PI / 0.5, sound_site_image, TRUE);

	// 使用画像サイト
	DrawRotaGraph(1100, 350, 1.0f, DX_PI / 0.5, picture_site_image, TRUE);

	// フェード描画
	fade->Draw();
}

void EndScene::Finalize()
{
	StopSoundMem(End_bgm);
}

eSceneType EndScene::GetNowSceneType() const
{
	return eSceneType::eEnd;
}
