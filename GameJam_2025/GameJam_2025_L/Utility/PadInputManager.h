#pragma once
#include "Vector2D.h"

#define		PAD_BUTTON_MAX	(16)						// ゲームパッドのボタンの最大数
#define		PAD_STICK_MAX	(32767.0f)					// スティックを傾けたときの最大値

enum class ePadInputState : unsigned char
{
	eNone,					// 入力無し
	ePress,					// ボタンを押した瞬間
	eRelease,				// ボタンを離した瞬間
	eHeld,					// ボタンを押し続けている
};

class PadInputManager
{
private:
	static PadInputManager* instance;

	unsigned char now_button[PAD_BUTTON_MAX] = {};		// 現在のボタンの入力
	unsigned char old_button[PAD_BUTTON_MAX] = {};		// 前回のボタンの入力

	Vector2D left_stick = 0.0f;							// 左スティック
	Vector2D right_stick = 0.0f;						// 右スティック

	int left_trigger = 0;								// 左トリガー
	int right_trigger = 0;								// 右トリガー

private:
	// 他のところからオブジェクト化できないようにコンストラクタはprivate
	PadInputManager() = default;
	// コピーガード
	// コピーコンストラクタの削除
	PadInputManager(const PadInputManager&) = delete;
	// コピー代入演算子の削除
	PadInputManager& operator = (const PadInputManager&) = delete;

public:
	~PadInputManager() = default;

public:
	// インスタンスを取得
	static PadInputManager* GetInstance();
	// インスタンスの削除
	static void DeleteInstance();

public:
	void Update();

public:
	// ボタンの入力状態を取得
	ePadInputState GetButtonInputState(int button);

	// 左スティックの値を取得
	Vector2D GetLeftStick() const;
	// 右スティックの値を取得
	Vector2D GetRightStick() const;

	// 左トリガーの値を取得
	int GetLeftTrigger() const;
	// 右トリガーの値を取得
	int GetRightTrigger() const;

	// 左スティックの傾きの割合を取得
	Vector2D GetLeftStickTiltPercentage();
	// 右スティックの傾きの割合を取得
	Vector2D GetRightStickTiltPercentage();

private:
	// 入力が有効な範囲かチェック
	bool CheckButtonRange(int button);

};

