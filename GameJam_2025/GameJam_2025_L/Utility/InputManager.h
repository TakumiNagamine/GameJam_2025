#pragma once

#define		D_KEYCODE_MAX	(256)

// 入力状態
enum class eInputState : unsigned char
{
	eNone,					// 入力無し
	ePress,					// 押した瞬間
	eRelease,				// 離した瞬間
	eHeld,					// 押し続けている
};

// 入力制御クラス
// Singleton（生成に関するデザインパターン）
class InputManager
{
private:
	static InputManager* instance;

	char now_key[D_KEYCODE_MAX] = {};		// 現在の入力キー
	char old_key[D_KEYCODE_MAX] = {};		// 前回の入力キー

private:
	// 他のところからオブジェクト化できないようにコンストラクタはprivate
	InputManager() = default;

	// コピーガード
	// コピーコンストラクタの削除
	InputManager(const InputManager&) = delete;
	// コピー代入演算子の削除
	InputManager& operator = (const InputManager&) = delete;

public:
	~InputManager() = default;

public:
	// インスタンスを取得
	static InputManager* GetInstance();
	// インスタンスの削除
	static void DeleteInstance();

public:
	// 更新処理
	void Update();

public:
	// 入力状態を取得
	eInputState GetKeyInputState(int keycode);

private:
	// 入力が有効な範囲かチェック
	bool CheckKeycodeRange(int keycode);
};