#include "RankingScene.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/PadInputManager.h"
#include "../../Utility/ResourceManager.h"
#include "../../Effect/Fade.h"
#include "DxLib.h"

RankingScene::RankingScene()
	: fade(nullptr)
	, push_button_flag(false)
	, text_image()
	, num_image()
	, certificate_image()
{

}

RankingScene::~RankingScene()
{

}

void RankingScene::Initialize()
{
	// ボタン押されたときのSE読み込み
	a_button_push = LoadSoundMem("Resource/Sounds/SE/cursol_push_se.mp3");
	// メインbgm
	ranking_main_bgm = LoadSoundMem("Resource/Sounds/BGM/main_sound1.mp3");
	// リソース管理のインスタンスを取得
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int> tmp;

	// 背景画像読み込み
	background_image = LoadGraph("Resource/Images/Scene/Tutorial/Background.png");
	// テキスト画像の読み込み
	text_image = rm->GetImages("Resource/Images/Scene/Ranking/Text/Ranking.png");
	tmp = rm->GetImages("Resource/Images/Scene/Ranking/Text/Back1.png");
	text_image.push_back(tmp[0]);

	certificate_image = rm->GetImages("Resource/Images/Scene/Ranking/Certificate.png");

	// 数字画像（0〜9）の読み込み
	num_image = rm->GetImages("Resource/Images/Scene/InGame/Text/Num.png");

	// 順位画像の読み込み
	ranking_text_image = LoadGraph("Resource/Images/Scene/Ranking/Text/number_ranking.png");
	// レベル数画像の読み込み
	level_text_image = LoadGraph("Resource/Images/Scene/Ranking/Text/level.png");
	// スコア画像の読み込み
	score_text_image = LoadGraph("Resource/Images/Scene/Ranking/Text/score.png");
	// ミス数画像の読み込み
	miss_text_image = LoadGraph("Resource/Images/Scene/Ranking/Text/miss.png");

	// フェードをインスタンス化
	fade = new Fade();
	// フェードの初期化処理（フェードイン）
	fade->Initialize(true);

	// ハイスコアデータを取得
	LoadHighScores();
	
	ChangeVolumeSoundMem(255 * 70 / 100, ranking_main_bgm);
	PlaySoundMem(ranking_main_bgm, DX_PLAYTYPE_BACK);

	ranking_next_scene = eSceneType::eRanking;
}

eSceneType RankingScene::Update()
{
	// フェードアウト中なら、フェード処理を続ける
	if (ranking_next_scene != eSceneType::eRanking) {
		if (fade->GetEndFlag()) {
			// フェードアウト完了後にシーン遷移
			return ranking_next_scene;
		}
		fade->Update();
		// フェードアウト中はタイトルシーンを維持
		return eSceneType::eRanking;
	}

	if (fade->GetEndFlag() == true)
	{

		// パッド入力制御のインスタンスを取得
		PadInputManager* pad_input = PadInputManager::GetInstance();

		if (pad_input->GetButtonInputState(XINPUT_BUTTON_A) == ePadInputState::ePress)
		{
			// フェードアウト処理
			fade->Initialize(false);
			// メインの音を止める
			StopSoundMem(ranking_main_bgm);
			// 押されたときのSE
			PlaySoundMem(a_button_push, DX_PLAYTYPE_BACK);
			// タイトル画面に遷移
			ranking_next_scene = eSceneType::eTitle;
		}
	}
	else
	{
		// フェード更新
		fade->Update();
	}

	return GetNowSceneType();
}

void RankingScene::Draw() const
{
	DrawString(10, 10, "Ranking", GetColor(255, 255, 255));
	DrawString(10, 26, "Z : Title", GetColor(255, 255, 255));

	DrawGraph(0, 0, background_image, TRUE);

	DrawRotaGraphF(640.0f, 390.0f, 1.0, 0.0, certificate_image[0], TRUE);

	// "ランキング"テキスト描画
	DrawRotaGraphF(640.0f, 70.0f, 1.0, 0.0, text_image[0], TRUE);
	// ボタン案内表示
	DrawRotaGraphF(1150.0f, 680.0f, 1.0, 0.0, text_image[1], TRUE);

	// ランキング
	DrawRotaGraph(280, 240, 0.7f, DX_PI / 0.5, ranking_text_image, TRUE);

	// レベル数
	DrawRotaGraph(400, 240, 0.7f, DX_PI / 0.5, level_text_image, TRUE);

	// スコア
	DrawRotaGraph(620, 240, 0.7f, DX_PI / 0.5, score_text_image, TRUE);

	// ミス数
	DrawRotaGraph(950, 240, 0.7f, DX_PI / 0.5, miss_text_image, TRUE);

	// ハイスコアを描画
	DisplayHighScores();

	// フェード描画
	fade->Draw();
}

void RankingScene::Finalize()
{
	// スコアを保存
	SaveHighScores();
}

eSceneType RankingScene::GetNowSceneType() const
{
	return eSceneType::eRanking;
}

// ハイスコアをソートする
void RankingScene::DataSortDescending(std::vector<HighScoreEntry>& arr, int n)
{
	int i, j;
	HighScoreEntry key;

	for (i = 1; i < n; i++)
	{
		key = arr[i];
		j = i - 1;

		while (j >= 0 && arr[j].score < key.score)  /***** 並べ替える条件 *****/
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}

		arr[j + 1] = key;
	}
}

// ハイスコアのデータを取得する
void RankingScene::LoadHighScores()
{
	// ファイルを開ける
	FILE* inputFile;
	fopen_s(&inputFile, HighScoreFileName, "r");

	//エラーチェック
	if (inputFile != NULL)
	{
		HighScoreEntry entry;
		int i = 0;

		// ファイルからレベル、スコア、ミス数の順番で読み込む
		while (fscanf_s(inputFile, "%d %d %d", &entry.level, &entry.score, &entry.misinputs) == 3 && i < MaxHighScores)
		{
			HighScores.push_back(entry);
			i++;
		}
		fclose(inputFile);

		// スコアをソートする
		if (HighScores.size() > 1)
		{
			DataSortDescending(HighScores, HighScores.size());
		}
	}
}

// ハイスコアを保存する
void RankingScene::SaveHighScores()
{
	// 今回のスコアがハイスコアより高いかチェック
	HandleNewHighScore();

	FILE* outputFile;
	fopen_s(&outputFile, HighScoreFileName, "w");

	// エラーチェック
	if (outputFile != NULL)
	{
		// レベル、スコア、ミス数の順番でファイルに書き込む
		for (size_t i = 0; i < HighScores.size(); ++i)
		{
			fprintf_s(outputFile, "%d %d %d\n", HighScores[i].level, HighScores[i].score, HighScores[i].misinputs);
		}

		fclose(outputFile);
	}
}


// スコアがハイスコアより高いか確認
void RankingScene::HandleNewHighScore()
{
	HighScoreEntry newEntry;
	newEntry.level = LevelReached;
	newEntry.score = FinalScore;
	newEntry.misinputs = Misinputs;

	if (HighScores.size() < MaxHighScores || FinalScore > HighScores.back().score)
	{
		HighScores.push_back(newEntry);

		// データを並べ替える
		if (HighScores.size() > 1)
		{
			DataSortDescending(HighScores, HighScores.size());
		}

		// データ数が保存できる数より多ければ、一番下のデータを消去する
		if (HighScores.size() > MaxHighScores)
		{
			HighScores.pop_back();
		}
	}
}

// ハイスコアの描画
void RankingScene::DisplayHighScores() const
{
	int yOffset = 290;      // y軸オフセット
	int rankX = 250;        // 順位のX座標
	int levelX = 350;       // レベルのX座標
	int scoreX = 500;       // スコアのX座標
	int missX = 800;        // ミスのX座標
	int rowSpacing = 100;    // 行間のスペース
	int digitWidth = 32;    // 1桁の幅（使用するフォント画像に合わせる）

	for (size_t i = 0; i < HighScores.size(); ++i)
	{
		// 順位を描画（画像で）
		DrawNumber(rankX, yOffset, i + 1);

		// レベルを描画
		DrawNumber(levelX, yOffset, HighScores[i].level);

		// スコアを描画
		DrawNumber(scoreX, yOffset, HighScores[i].score);

		// ミスの回数を描画
		DrawNumber(missX, yOffset, HighScores[i].misinputs);

		yOffset += rowSpacing; // 次の行へ
	}
}

// 指定位置に数値を画像で描画する
void RankingScene::DrawNumber(int x, int y, int number) const
{
	if (num_image.empty()) return;  // 画像が未ロードなら描画しない

	int image_width, image_height;
	GetGraphSize(num_image[0], &image_width, &image_height);  // 画像のサイズ取得

	int digit_width = image_width / 10;  // 各数字の幅（10桁に分割）
	int digit_height = image_height;  // 画像の高さ

	std::string numStr = std::to_string(number); // 数字を文字列に変換

	for (size_t i = 0; i < numStr.length(); ++i)
	{
		int digit = numStr[i] - '0';  // 文字から数値へ変換
		int srcX = digit * digit_width;  // 画像内のX座標
		int srcY = 0;  // Y座標（1行の画像なら0）

		// 画像の一部を切り取って描画
		DrawRectGraph(x + i * digit_width, y, srcX, srcY, digit_width, digit_height, num_image[0], TRUE, FALSE);
	}
}