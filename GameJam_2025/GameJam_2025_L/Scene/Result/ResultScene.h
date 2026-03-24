#pragma once
#include "../SceneBase.h"
#include "../../Utility/PadInputManager.h"

// リザルト画面クラス
class ResultScene : public SceneBase
{
private:
	Vector2D cursor_pos;		// カーソル座標
	PadInputManager* pad_input;	// コントローラーの入力インスタンス
	Vector2D LeftStickTilt;		// 左スティック入力値
	bool cursorOnButton;	// カーソルがUIボタンの上にいるハンドル
	int cursor_blank_Graph;		// カーソル画像ハンドル
	int cursor_on_Graph;		// 選択カーソル画像ハンドル
	int cursor_shadow_Graph;	// カーソル影画像ハンドル

	int cursor_se_move;         // カーソルの移動SE
	int cursor_se_push;         // カーソルの決定SE

	int BackgroundGraph;	// 背景画像ハンドル

	std::vector<int> num_image;

	// リザルトシーン画像ハンドル
	int ResultTitle_Graph;		// リザルトのタイトル画像ハンドル
	int rank1_Graph;			// ランキング１位画像ハンドル
	int rank2_Graph;			// ランキング２位画像ハンドル
	int rank3_Graph;			// ランキング３位画像ハンドル
	int YourScoreGraph;			// あなたのスコアフォント画像ハンドル
	int HighScoreHistory;		// ハイスコア歴フォント画像ハンドル
	int ReplayButton_Graph;		// リプレイボタン画像ハンドル
	int TitleButton_Graph;		// タイトルボタン画像ハンドル

	// 背景画像ハンドル
	int backgroundWaves[4];
	float backgroundY[4];
	const int screenHeight = 720;
	float scrollSpeedY = 5;

	int result_bgm;             // リザルトメインbgm

	// ゲームデータ
	int FinalScore;		// 最終スコア
	int LevelReached;	// 最終レベル
	int Misinputs;	// ミス数

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

	void LoadHighScores();			// ハイスコアデータをファイルから取得する
	void SaveHighScores();			// ハイスコアデータをファイルに保存する
	void HandleNewHighScore();		// 今回のスコアが過去のスコアより高いか確認する
	void DisplayCurrentRunScores() const;	// 今回のスコアを描画
	void DisplayHighScores() const;	// ハイスコアを描画する
	void DrawNumber(int x, int y, int number) const;        // スコアを画像で描画

	// 動く背景描画
	void DrawBackground();
	int offset_plus;


	int HighScore_SE;               // ハイスコアのSE

	// データを並べ替えるセッター
	void DataSortDescending(std::vector<HighScoreEntry>& arr, int n);

	// カーソル描画
	void DrawCursor() const;


	// UIボタンメンバー
	struct Button 
	{
		int GraphicHandle;	// ボタンの画像ハンドル
		int x, y;			// ボタンのｘとｙ座標
		int Width, Height;	// ボタンの幅と高さ
		eSceneType TargetScene;	// ボタンが押されて行きたいシーン
	};

	std::vector<Button> buttons;

public:
	// 他のシーンからデータを受け取る（InGameシーンで使う）
	void SetGameData(int score, int level, int misinputs)
	{
		this->FinalScore = score;
		this->LevelReached = level;
		this->Misinputs = misinputs;
	}


public:
	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

public:
	virtual eSceneType GetNowSceneType() const override;
};