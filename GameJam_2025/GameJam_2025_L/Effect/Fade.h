#pragma once

class Fade
{
private:
	int alpha;				// 透明度
	bool is_fade_in;		// フェードインか？　true: フェードイン	false: フェードアウト
	bool end_flag;			// フェード終了フラグ	true: 終了した	false: 終了していない

public:
	// コンストラクタ
	Fade();
	// デストラクタ
	~Fade();

public:
	// 初期化処理
	void Initialize(bool set_fade_in_flag);
	// 更新処理
	void Update();
	// 描画処理
	void Draw() const;
	// 終了処理
	void Finalize();

public:
	// フェード終了フラグを取得する
	bool GetEndFlag() const;
};

