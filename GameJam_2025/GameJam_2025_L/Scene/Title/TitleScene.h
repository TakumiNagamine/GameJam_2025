#pragma once

#include "../SceneBase.h"

class TitleScene : public SceneBase
{
private:
	class Fade* fade;				   // フェード

	int title_cursol_system;           // カーソル別遷移用変数
	float title_stick_time;            // スティック用所有時間

	int background_image;              // 背景画像
	int button_guid_image;             // ボタン説明画像     
	int cursol_image;                  // カーソル画像
	int cursol_push_image;             // カーソル決定後画像
	int title_image;                   // 「タイトル」画像
	int subtitle_image;                // 「サブタイトル」画像
	int title_play_image;              // 「プレイ」画像
	int title_ranking_image;           // 「ランキング」画像
	int title_end_image;               // 「エンド」画像

	int title_bgm;                     // タイトルBGM

	int cursol_move_se;			       // カーソル移動SE
	int cursol_push_se;			       // カーソル決定SE

private:
	eSceneType next_scene; // 遷移先のシーン


public:
	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

public:
	virtual eSceneType GetNowSceneType() const override;
};