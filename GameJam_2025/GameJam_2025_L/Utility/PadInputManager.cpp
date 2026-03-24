#include "PadInputManager.h"
#include "DxLib.h"

// 静的メンバ変数の初期化
PadInputManager* PadInputManager::instance = nullptr;

// インスタンスを取得
PadInputManager* PadInputManager::GetInstance()
{
	if (instance == nullptr)
	{
		// 最初の1回だけオブジェクトを生成する
		instance = new PadInputManager();
	}

	// 実体を返す
	return instance;
}

// インスタンスの削除
void PadInputManager::DeleteInstance()
{
	if (instance != nullptr)
	{
		// メモリの開放
		delete instance;
		instance = nullptr;
	}
}

void PadInputManager::Update()
{
	// ゲームパッドの情報
	XINPUT_STATE input;

	// ゲームパッドの状態を取得
	GetJoypadXInputState(DX_INPUT_PAD1, &input);

	// メモリ領域をコピー
	memcpy(old_button, now_button, (sizeof(char) * PAD_BUTTON_MAX));
	for (int i = 0; i < PAD_BUTTON_MAX; i++)
	{
		// 現在押しているボタンの更新
		now_button[i] = input.Buttons[i];
	}

	// 左スティックの更新
	left_stick.x = input.ThumbLX;
	left_stick.y = input.ThumbLY;

	// 右スティックの更新
	right_stick.x = input.ThumbRX;
	right_stick.y = input.ThumbRY;

	// 左トリガーの更新
	left_trigger = (int)input.LeftTrigger;

	// 右トリガーの更新
	right_trigger = (int)input.RightTrigger;
}

// ボタンの入力状態を取得
ePadInputState PadInputManager::GetButtonInputState(int button)
{
	// ボタン入力が有効な範囲だったら処理を行う
	if (CheckButtonRange(button))
	{
		if (old_button[button] == TRUE)
		{
			if (now_button[button] == TRUE)
			{
				// 押し続けている
				return ePadInputState::eHeld;
			}
			else
			{
				// 離した瞬間
				return ePadInputState::eRelease;
			}
		}
		else
		{
			if (now_button[button] == TRUE)
			{
				// 押した瞬間
				return ePadInputState::ePress;
			}
			else
			{
				// 入力無し
				return ePadInputState::eNone;
			}
		}
	}

	return ePadInputState::eNone;
}

// 左スティックの値を取得
Vector2D PadInputManager::GetLeftStick() const
{
	return left_stick;
}

// 右スティックの値を取得
Vector2D PadInputManager::GetRightStick() const
{
	return right_stick;
}

// 左トリガーの値を取得
int PadInputManager::GetLeftTrigger() const
{
	return left_trigger;
}

// 右トリガーの値を取得
int PadInputManager::GetRightTrigger() const
{
	return right_trigger;
}

// 左スティックの傾きの割合を取得
Vector2D PadInputManager::GetLeftStickTiltPercentage()
{
	// 左スティックの傾き
	Vector2D left_stick_tilt;

	// 左スティックの傾きを-1.0f ～ 1.0fの間に変換
	// x軸値の変換
	left_stick_tilt.x = left_stick.x / PAD_STICK_MAX;
	if (left_stick_tilt.x < -1.0f)
	{
		left_stick_tilt.x = -1.0f;
	}
	// y軸値の変換
	left_stick_tilt.y = left_stick.y / PAD_STICK_MAX;
	if (left_stick_tilt.y < -1.0f)
	{
		left_stick_tilt.y = -1.0f;
	}

	return left_stick_tilt;
}

// 右スティックの傾きの割合を取得
Vector2D PadInputManager::GetRightStickTiltPercentage()
{
	// 右スティックの傾き
	Vector2D right_stick_tilt;

	// 右スティックの傾きを-1.0f ～ 1.0fの間に変換
	// x軸値の変換
	right_stick_tilt.x = right_stick.x / PAD_STICK_MAX;
	if (right_stick_tilt.x < -1.0f)
	{
		right_stick_tilt.x = -1.0f;
	}
	// y軸値の変換
	right_stick_tilt.y = right_stick.y / PAD_STICK_MAX;
	if (right_stick_tilt.y < -1.0f)
	{
		right_stick_tilt.y = -1.0f;
	}

	return right_stick_tilt;
}

// 入力が有効な範囲かチェック
bool PadInputManager::CheckButtonRange(int button)
{
	// 現在の入力値がtrueかfalseか返す
	return (0 <= button && button < PAD_BUTTON_MAX);
}
