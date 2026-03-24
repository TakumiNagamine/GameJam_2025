#pragma once
#include "../GameObject.h"
#include <vector>

// 音符クラス
class Note : public GameObject
{
private:
	bool correct_answer_flag;				// 正解チェックフラグ　true: 正解　false: 不正解
	bool problem_presentation_flag;			// お題発表フラグ　true: 発表中

	std::vector<int> note_image;			// 音符画像の動的配列
	std::vector<int> noise_image;			// ノイズ画像の動的配列

	int red[4] = { 0,255,240,125 };			// 音符色データ
	int green[4] = { 0,225,125,230 };		// 音符色データ
	int bule[4] = { 0,95,115,235 };			// 音符色データ
	int color_num;							// 音符色番号

	int life_time;							// 音符が削除されるまでの時間
	float degree;							// sin用のデグリー（度）
	int move_direction;						// 移動方向	0: 左に移動	1: 右に移動

public:
	// コンストラクタ
	Note();
	// デストラクタ
	virtual ~Note();

public:
	// 初期化処理
	virtual void Initialize();
	// 更新処理
	virtual void Update();
	// 描画処理
	virtual void Draw() const;
	// 終了処理
	virtual void Finalize();

public:
	// 音符の色を設定する
	void SetNoteColor(bool set_flag);
	// お題発表中フラグを設定する
	void SetPressentationFlag(bool set_flag);
};