#include "InputManager.h"
#include "DxLib.h"

// 静的メンバ変数の初期化
InputManager* InputManager::instance = nullptr;

InputManager* InputManager::GetInstance()
{
	if (instance == nullptr)
	{
		// 最初の1回だけオブジェクトを生成する
		instance = new InputManager();
	}

	// 実体を返す
	return instance;
}

void InputManager::DeleteInstance()
{
	if (instance != nullptr)
	{
		// メモリの開放
		delete instance;
		instance = nullptr;
	}
}

void InputManager::Update()
{
	// メモリ領域をコピー
	// now_keyの先頭256バイトをold_keyにコピー
	memcpy(old_key, now_key, (sizeof(char) * D_KEYCODE_MAX));
	// 全てのキーの入力状態を取得
	GetHitKeyStateAll(now_key);
}

// 入力状態を取得
eInputState InputManager::GetKeyInputState(int keycode)
{
	if (CheckKeycodeRange(keycode))
	{
		if (old_key[keycode] == TRUE)
		{
			if (now_key[keycode] == TRUE)
			{
				// 押し続けている
				return eInputState::eHeld;
			}
			else
			{
				// 離した瞬間
				return eInputState::eRelease;
			}
		}
		else
		{
			if (now_key[keycode] == TRUE)
			{
				// 押した瞬間
				return eInputState::ePress;
			}
			else
			{
				// 入力無し
				return eInputState::eNone;
			}
		}
	}

	return eInputState::eNone;
}

// 入力が有効な範囲かチェック
bool InputManager::CheckKeycodeRange(int keycode)
{
	return (0 <= keycode && keycode < D_KEYCODE_MAX);
}