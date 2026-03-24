#include "Player.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/PadInputManager.h"
#include "../../Utility/ResourceManager.h"
#include "Dxlib.h"

// コンストラクタ
Player::Player()
	: velocity(0.0f)
	, animation()
	, animation_num(0)
	, animation_count(0)
	, attack_anim_flag(false)
	, move_lane_num(0)
	, ring_flag(false)
	, problem_presentation_flag(true)
	, shadow_radius_x(0.0f)
	, shadow_flag(false)
	, move_wait_time(0)
	, game_end_flag(false)
	, operable_flag(false)
{

}

// デストラクタ
Player::~Player()
{

}

// 初期化処理
void Player::Initialize()
{
	// リソース管理のインスタンスを取得
	ResourceManager* rm = ResourceManager::GetInstance();
	// プレイヤーアニメーション画像の読み込み
	animation = rm->GetImages("Resource/Images/Object/Player/Player.png", 6, 6, 1,200, 200);
	// ボタンが押されたときのseの読み込み
	push_se = LoadSoundMem("Resource/Sounds/SE/Chara_move_sounds.mp3");

	graphic_images = animation[0];

	// 足元の影の半径xの設定
	shadow_radius_x = 80.0f;
}

// 更新処理
void Player::Update()
{
	if (operable_flag)
	{
		if (game_end_flag == false)
		{
			// 鐘を打つ処理
			RingBell();
		}
	}

	// アニメーション制御処理
	AnimationControl();

	if (operable_flag)
	{
		if (attack_anim_flag == false)
		{
			// 十字ボタンでの移動処理
			CrossButtonMovement();

			if (move_wait_time > 0)
			{
				// 移動待ち時間が0より大きい場合
				move_wait_time--;
				// ここで関数を抜ける
				return;
			}
			// 左スティックでの移動処理
			LStickMovement();
		}
	}
}

// 描画処理
void Player::Draw() const
{
	// 親クラスの描画関数呼び出し
	//__super::Draw();

	if (shadow_flag)
	{
		// 描画ブレンドモードをアルファブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		// プレイヤーの足元に影が来るように楕円描画
		DrawOvalAA(location.x - 15.0f, 650.0f, shadow_radius_x, 10.0f, 32, 0x000000, TRUE);
		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// 画像描画
	DrawRotaGraphF(location.x, location.y, 1.0, 0.0, graphic_images, TRUE, flip_flag);

#if _DEBUG

#endif // _DEBUG
}

// 終了処理
void Player::Finalize()
{
	// コンテナが空か調べる
	if (!animation.empty())
	{
		// 動的配列の要素を削除
		animation.clear();
	}
}

// 左スティックでの移動処理
void Player::LStickMovement()
{
	// パッド入力制御のインスタンスを取得
	PadInputManager* pad_input = PadInputManager::GetInstance();

	if (pad_input->GetLeftStickTiltPercentage().x <= -0.5f)
	{
		// 左スティックが5割以上左に傾いている場合


		if (move_lane_num > 0)
		{
			// レーンを1つ左にする
			move_lane_num--;

			// 待ち時間の設定
			move_wait_time = 10;

			// 左移動
			velocity.x = -100.0f;

			// 移動したときのSEを鳴らす
			PlaySoundMem(push_se, DX_PLAYTYPE_BACK);
		}
	}
	else if (pad_input->GetLeftStickTiltPercentage().x >= 0.5f)
	{
		// 左スティックが5割以上右に傾いている場合


		if (move_lane_num < 5)
		{
			// レーンを1つ右にする
			move_lane_num++;

			// 待ち時間の設定
			move_wait_time = 10;

			// 右移動
			velocity.x = 100.0f;

			// 移動したときのSEを鳴らす
			PlaySoundMem(push_se, DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		// 移動速度を0に戻す
		velocity.x = 0.0f;
	}

	// プレイヤー移動
	location += velocity;
}

// 十字ボタンでの移動処理
void Player::CrossButtonMovement()
{
	// パッド入力制御のインスタンスを取得
	PadInputManager* pad_input = PadInputManager::GetInstance();

	if (pad_input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == ePadInputState::ePress)
	{
		// 十字ボタンの左を押した瞬間

		if (move_lane_num > 0)
		{
			// レーンを1つ左にする
			move_lane_num--;

			// 左移動
			velocity.x = -100.0f;

			// 移動したときのSEを鳴らす
			PlaySoundMem(push_se, DX_PLAYTYPE_BACK);
		}
	}
	else if (pad_input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == ePadInputState::ePress)
	{
		// 十字ボタンの右を押した瞬間


		if (move_lane_num < 5)
		{
			// レーンを1つ右にする
			move_lane_num++;

			// 右移動
			velocity.x = 100.0f;

			// 移動したときのSEを鳴らす
			PlaySoundMem(push_se, DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		// 移動速度を0に戻す
		velocity.x = 0.0f;
	}

	// プレイヤー移動
	location += velocity;



	// 後で、キーボード処理は削除する
	// キーボード入力制御のインスタンスを取得
	InputManager* input = InputManager::GetInstance();

	if (input->GetKeyInputState(KEY_INPUT_RIGHT) == eInputState::ePress)
	{
		// 右矢印キーを押した瞬間

		if (move_lane_num < 5)
		{
			// レーンを1つ右にする
			move_lane_num++;

			// 右移動
			velocity.x = 100.0f;

			// 移動したときのSEを鳴らす
			PlaySoundMem(push_se, DX_PLAYTYPE_BACK);
		}
	}
	else if (input->GetKeyInputState(KEY_INPUT_LEFT) == eInputState::ePress)
	{
		// 左矢印キーを押した瞬間

		if (move_lane_num > 0)
		{
			// レーンを1つ左にする
			move_lane_num--;

			// 左移動
			velocity.x = -100.0f;
			// 移動したときのSEを鳴らす
			PlaySoundMem(push_se, DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		// 移動速度を0に戻す
		velocity.x = 0.0f;
	}

	// プレイヤー移動
	location += velocity;
}

// 鐘を打つ処理
void Player::RingBell()
{
	// パッド入力制御のインスタンスを取得
	PadInputManager* pad_input = PadInputManager::GetInstance();

	// 後で、キーボード処理は削除する
	// キーボード入力制御のインスタンスを取得
	InputManager* input = InputManager::GetInstance();

	if (pad_input->GetButtonInputState(XINPUT_BUTTON_A) == ePadInputState::ePress
		|| input->GetKeyInputState(KEY_INPUT_A) == eInputState::ePress)
	{
		// Aボタンを押した瞬間
		if (attack_anim_flag == false && problem_presentation_flag == false)
		{
			// 鐘を打つアニメーションを行っていないかつ、
			// お題発表中でないとき

			// 鐘を打ったフラグを立てる
			ring_flag = true;

			// 打つアニメーション開始
			attack_anim_flag = true;
			// 画像変更
			animation_num = 2;
			graphic_images = animation[animation_num];
			// 鐘の上部に来るようy座標を変更する
			location.y -= 250.0f;
			// 足元の影の半径xの設定
			shadow_radius_x = 40.0f;
		}
	}
	else
	{
		ring_flag = false;
	}
}

// アニメーション制御処理
void Player::AnimationControl()
{
	// アニメーションカウント加算
	animation_count++;

	if (attack_anim_flag == false)
	{
		if (animation_count >= 10)
		{
			animation_count = 0;
			// 次のアニメーション画像にする
			animation_num++;

			// アニメーション画像番号が読み込んだ画像枚数より多い場合
			if (animation_num > 1)
			{
				// 最初のアニメーション画像にする
				animation_num = 0;
			}

			// プレイヤーの描画画像を変更
			graphic_images = animation[animation_num];
		}
	}
	else
	{
		if (animation_count >= 5)
		{
			animation_count = 0;
			// 次のアニメーション画像にする
			animation_num++;

			// アニメーション画像番号が読み込んだ画像枚数より多い場合
			if (animation_num > 5)
			{
				// 最初のアニメーション画像にする
				animation_num = 0;
				// 打つアニメーション終了
				attack_anim_flag = false;
				// 鐘の下部に来るようy座標を設定する
				location.y += 250.0f;
				// 足元の影の半径xの設定
				shadow_radius_x = 90.0f;
			}

			// プレイヤーの描画画像を変更
			graphic_images = animation[animation_num];
		}
	}
}

// プレイヤーがいるレーンを取得する
int Player::GetMoveLaneNum() const
{
	return move_lane_num;
}

// プレイヤーが鐘を打ったフラグを取得する
bool Player::GetRingFlag() const
{
	return ring_flag;
}

// お題発表フラグを設定する
void Player::SetPresentationFlag(bool set_flag)
{
	problem_presentation_flag = set_flag;
}

// 影付けフラグをtrueに設定する
void Player::SetShadowFlag()
{
	// 影をつける
	shadow_flag = true;
}

// ゲーム終了フラグをtrueに設定する
void Player::SetGameEndFlag()
{
	game_end_flag = true;
}

void Player::SetOperableFlag(bool set_flag)
{
	operable_flag = set_flag;
}
