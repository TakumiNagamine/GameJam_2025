#pragma once
#include "../GameObject.h"
#include <vector>

// プレイヤークラス
class Player :  public GameObject
{
private:
	Vector2D velocity;					// 移動方向、移動速度
	std::vector<int> animation;			// アニメーション画像の動的配列
	int animation_num;					// アニメーション番号
	int animation_count;				// アニメーションカウント
	bool attack_anim_flag;				// 打つアニメーションフラグ　true: 打っている　false: 打っていない

	int move_lane_num;					// 移動のレーン 0～5まで（鐘の本数と同じ）
	bool ring_flag;						// 鐘を打ったか	true: 打った　false: 打っていない

	bool problem_presentation_flag;		// お題発表フラグ　true: 発表中

	float shadow_radius_x;				// プレイヤーの足元の影の半径x
	bool shadow_flag;					// 影をつけるか	true: つける	false: 付けない

	int move_wait_time;					// スティックで移動するときの待ち時間

	bool game_end_flag;					// ゲーム終了フラグ
	bool operable_flag;					// 操作可能フラグ

	int push_se;                        // 押したときのSE
public:
	// コンストラクタ
	Player();
	// デストラクタ
	virtual ~Player();

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
	// 左スティックでの移動処理
	void LStickMovement();
	// 十字ボタンでの移動処理
	void CrossButtonMovement();

	// 鐘を打つ処理
	void RingBell();
	// アニメーション制御処理
	void AnimationControl();

public:
	// プレイヤーがいるレーンを取得する
	int GetMoveLaneNum() const;
	// プレイヤーが鐘を打ったフラグを取得する
	bool GetRingFlag() const;
	// お題発表フラグを設定する
	void SetPresentationFlag(bool set_flag);
	// 影付けフラグをtrueに設定する
	void SetShadowFlag();
	// ゲーム終了フラグをtrueに設定する
	void SetGameEndFlag();
	// 操作可能フラグを設定する
	void SetOperableFlag(bool set_flag);
};