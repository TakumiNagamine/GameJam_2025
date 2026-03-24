#pragma once
#include "../GameObject.h"
#include <vector>

// お題の進行状況UIクラス
class Answer : public GameObject
{
private:
	std::vector<int> book_image;			// お題画像
	std::vector<int> answer_image;			// 回答結果画像
	std::vector<int> num_image;				// 数字画像
	std::vector<int> text_image;			// テキスト画像

	float answer_image_y;					// 回答結果画像y座標
	std::vector<int> answer_result;			// 回答結果	1: 正解	0: 不正解
	bool answer_reset_flag;					// 回答リセットフラグ

	bool answer_anim_flag;					// 回答アニメーション再生フラグ	true: 再生する	false: 再生しない
	int answer_anim_count;					// 回答アニメーションカウント
	bool display_answer_flag;				// 回答表示フラグ	true: 表示	false: 非表示
	
	bool correct_answer_flag;				// 正解チェックフラグ　true: 正解　false: 不正解

	int now_level;							// 現在のレベル

public:
	// コンストラクタ
	Answer();
	// デストラクタ
	virtual ~Answer();

public:
	// 初期化処理
	virtual void Initialize();
	// 更新処理
	virtual void Update();
	// 描画処理
	virtual void Draw() const;
	// 終了処理
	virtual void Finalize();

private:
	// 回答描画処理
	void DrawAnswer() const;

public:
	// アニメーション開始処理
	void AnimStart();
	// 正解チェックフラグを設定する
	void SetCorrectAnswerFlag(bool set_flag);
	// 回答リセットフラグをtrueに設定する
	void SetResetFlag();
	// 現在のレベルを設定する
	void SetNowLevel(int set_level);
};

