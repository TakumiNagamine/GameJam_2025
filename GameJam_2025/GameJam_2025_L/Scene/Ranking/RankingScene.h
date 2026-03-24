#pragma once
#include "../SceneBase.h"

// ランキング画面クラス
class RankingScene : public SceneBase
{
private:
	class Fade* fade;
	eSceneType ranking_next_scene; // 遷移先のシーン
	bool push_button_flag;
	int a_button_push;
	int ranking_main_bgm;

	std::vector<int> text_image;
	std::vector<int> num_image;
	std::vector<int> certificate_image;

	int ranking_text_image;     // 「順位」画像     
	int level_text_image;       // 「レベル」画像
	int score_text_image;       // 「スコア」画像
	int miss_text_image;        // 「ミス数」画像

	// ゲームデータ
	int FinalScore;		// 最終スコア
	int LevelReached;	// 最終レベル
	int Misinputs;	    // ミス数

	struct HighScoreEntry
	{
		int score;
		int misinputs;
		int level;
	};

	// ハイスコア
	std::vector<HighScoreEntry> HighScores;		// ハイスコアを格納するヴェクター
	const int MaxHighScores = 3;		// ハイスコアを保存する数
	const char* HighScoreFileName = "Resource/ScoreData/Scores.txt";	// ハイスコアデータを保存しているファイルの名前

	void LoadHighScores();			                        // ハイスコアデータをファイルから取得する
	void SaveHighScores();			                        // ハイスコアデータをファイルに保存する
	void HandleNewHighScore();		                        // 今回のスコアが過去のスコアより高いか確認する
	void DisplayHighScores() const;	                        // ハイスコアを描画する
	void DrawNumber(int x, int y, int number) const;        // スコアを画像で描画

	// データを並べ替えるセッター
	void DataSortDescending(std::vector<HighScoreEntry>& arr, int n);


public:
	// 他のシーンからデータを受け取る（InGameシーンで使う）
	void SetGameData(int score, int level, int misinputs)
	{
		this->FinalScore = score;
		this->LevelReached = level;
		this->Misinputs = misinputs;
	}

public:
	RankingScene();
	~RankingScene();

public:
	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

public:
	virtual eSceneType GetNowSceneType() const override;

};
