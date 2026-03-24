#pragma once
#include "../GameObject.h"
#include <vector>

// 鐘クラス
class Bell : public GameObject
{
private:
	class Player* player;					// プレイヤー情報参照用
	class ProblemGenerator* problem;		// 問題ジェネレータ

	int now_target_num;						// 打つ対象の鐘　0～5まで

	bool correct_answer_flag;				// 正解チェックフラグ　true: 正解　false: 不正解

	int False_Sounds;                       // 不正解の音
	int True_Sounds;                        // 正解の音

	std::vector<int> question;				// お題

	int problem_num;						// お題番号
	bool problem_presentation_flag;			// お題発表フラグ　true: 発表中
	int presentation_count;					// お題発表の時間
	int next_presentation_wait_time;		// 次の発表までの待ち時間
	int one_note_presentation_count;		// 1音発表時間

	std::vector<int> animation;				// アニメーション画像の動的配列
	int stand_image;						// 鐘のスタンド部分画像

	int bell_sounds[6];                     // 鐘の音  ド から ラ まで
	
	bool create_note_flag;					// 音符生成フラグ	treu: 生成する	false: 生成しない

	bool shadow_flag;						// 影をつけるか	true: つける	false: 付けない

	bool game_end_flag;						// ゲーム終了フラグ

public:
	// コンストラクタ
	Bell();
	// デストラクタ
	virtual ~Bell();

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
	// アニメーション制御処理
	void AnimationControl();

public:
	// プレイヤーポインタを設定する
	void SetPlayer(class Player* p);
	// 問題ジェネレータポインタを設定する
	void SetProblemGenerator(class ProblemGenerator* p);
	// 正解フラグを取得する
	bool GetCorrectFlag() const;
	// 音符生成フラグを取得する
	bool GetCreateNoteFlag() const;
	// 影付けフラグをtrueに設定する
	void SetShadowFlag();
	// ゲーム終了フラグをtrueに設定する
	void SetGameEndFlag();
	// お題発表中フラグを取得する
	bool GetPresentationFlag() const;

private:
	// 出題処理
	void Question();
};

