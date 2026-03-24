#pragma once

#include "../SceneBase.h"

// インゲーム画面クラス
class InGameScene : public SceneBase
{
private:
	class ProblemGenerator* problem;		// 問題ジェネレータ
	class Fade* fade;						// フェード
	class Player* player;					// プレイヤー
	class Bell* bell;						// 鐘
	class Answer* answer;					// 回答結果

	int InGame_main_bgm;                    // インゲームのメインbgm
	bool light_flag;

	bool change_scene_flag;					// シーン変更フラグ	true: 変更可	false: 変更不可
	
	std::vector<int> ui_image;				// UI画像
	std::vector<int> text_image;			// テキスト画像
	std::vector<int> timer_num_image;		// タイマー数字画像
	std::vector<int> background_img;		// 背景画像
	int timer_num;							// 数字画像番号

	int	count_down_timer;					// カウントダウン時間
	int sinpaku;                            // 心拍音

	enum class eInGameState
	{
		eCountDown,							// ゲーム開始のカウントダウン中
		ePlay,								// プレイ中
		eEnd								// ゲーム終了
	};

	eInGameState state;						// インゲーム画面の状態

	bool obj_update_flag;					// オブジェクト更新フラグ
	float curtain_x;						// 舞台幕画像x座標
	float curtain_y;						// 舞台幕画像y座標
	int change_scene_count;					// シーン切り替え待ちカウント

	int second_images[10];                  // 秒のイメージ
	int min_images[10];                     // 分のイメージ

	int ui_alpha;							// UI画像の透明度

	int light_sound;                        // ライトの音

public:
	InGameScene();
	virtual ~InGameScene();

public:
	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

public:
	virtual eSceneType GetNowSceneType() const override;

private:
	// カウントダウン処理
	void CountDown();
	// エフェクト生成処理
	void CreateEffect();

};