#include "Fade.h"
#include "DxLib.h"

// コンストラクタ
Fade::Fade()
	: alpha(0)
	, is_fade_in(true)
	, end_flag(false)
{

}

// デストラクタ
Fade::~Fade()
{

}

// 初期化処理
void Fade::Initialize(bool set_fade_in_flag)
{
	is_fade_in = set_fade_in_flag;
	end_flag = false;

	if (is_fade_in)
	{
		alpha = 255;
	}
	else
	{
		alpha = 0;
	}
}

// 更新処理
void Fade::Update()
{
	if (is_fade_in == true)
	{
		// フェードイン（明るくなる）
		if (alpha > 0)
		{
			alpha -= 5;

			if (alpha <= 0)
			{
				alpha = 0;
				// フェード終了
				end_flag = true;
			}
		}
	}
	else
	{
		// フェードアウト（暗くなる）
		if (alpha < 255)
		{
			alpha += 5;

			if (alpha >= 255)
			{
				alpha = 255;
				// フェード終了
				end_flag = true;
			}
		}
	}
}

// 描画処理
void Fade::Draw() const
{
	// 描画ブレンドモードをアルファブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	// 画面サイズの黒色四角
	DrawBox(0, 0, 1280, 720, 0x000000, TRUE);
	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 終了処理
void Fade::Finalize()
{

}

// フェード終了フラグを取得する
bool Fade::GetEndFlag() const
{
	return end_flag;
}