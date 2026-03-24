#pragma once
#include "../SceneBase.h"

// エンド画面クラス
class EndScene : public SceneBase
{
private:
	class Fade* fade;        // フェード
	int all_end_time;        // エンド画面終わるまでの時間変数

	int background_image;    // 背景画像用
	int ty_text_image;       // 「thanks you for playing」のテキスト画像用

	int End_bgm;             // エンド用BGM


	/*以下の変数は使用するか不明*/
	int sound_site_image;    // BGMやSEの使用サイト表記用画像用
	int picture_site_image;  // 画像使用サイト表記用画像用

public:
	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

public:
	virtual eSceneType GetNowSceneType() const override;

};
