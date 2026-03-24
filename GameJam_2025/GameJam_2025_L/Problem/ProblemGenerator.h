#pragma once
#include <vector>

// お題生成クラス
class ProblemGenerator
{
private:
	std::vector<int> problem;			// お題
	int level;							// レベル
	int current_step;					// 現在の進行状況
	int miss;							// ミス数

	// ゲーム結果保存用（1位～3位 + 自分の結果）
	int level_num[4];
	int score[4];
	int miss_num[4];

public:
	// コンストラクタ
	ProblemGenerator();
	// デストラクタ
	~ProblemGenerator();

public:
	// 問題作成処理
	void CreateProblem();
	// レベルアップ処理
	void LevelUp();
	// プレイヤーの入力を調べる
	bool CheckAnswer(int bell_num);
	// 全問正解か調べる
	bool CheckAllCorrect();
	// ランキングデータ読み込み
	void LoadRankingData();

	// デバッグ用描画処理
	void Draw() const;

public:
	// 現在、打つ対象の鐘の番号を取得
	int GetCurrentTarget() const;
	// 現在の問題を取得する
	const std::vector<int>& GetCurrentProblem() const;
	// レベル数を取得する
	int GetLevel() const;
	// 現在の進行状況を取得する
	int GetCurrentStep() const;
	// ゲーム結果をファイルに保存する処理
	void SaveGameResult();
};

