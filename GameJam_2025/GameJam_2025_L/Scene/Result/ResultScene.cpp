#include "ResultScene.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"
#include <stdio.h>

#define UI_BUTTON_BASE_WIDTH (220)
#define UI_BUTTON_BASE_HEIGHT (70)

void ResultScene::Initialize()
{
    // カーソルの初期位置
	cursor_pos = { 600.0f, 300.0f };
    cursorOnButton = false;
    // カーソル画像
    cursor_blank_Graph = LoadGraph("Resource/Images/UI_Elements/cursor_blank.png");
    cursor_on_Graph = LoadGraph("Resource/Images/UI_Elements/cursor_on.png");
    cursor_shadow_Graph = LoadGraph("Resource/Images/UI_Elements/cursor_shadow.png");

    cursor_se_move = LoadSoundMem("Resource/Sounds/SE/cursol_move_se.mp3");
    cursor_se_push = LoadSoundMem("Resource/Sounds/SE/cursol_push_se.mp3");

	// コントローラーの入力インスタンス取得
	pad_input = PadInputManager::GetInstance();

    // リソース管理のインスタンスを取得
    ResourceManager* rm = ResourceManager::GetInstance();

    // 背景画像
    BackgroundGraph = LoadGraph("Resource/Images/Scene/InGame/Background01.png");

    // リザルトシーン画像ハンドル
    ResultTitle_Graph = LoadGraph("Resource/Images/Scene/Result/Result_Title.png");
    rank1_Graph = LoadGraph("Resource/Images/Scene/Result/Result_rank1.png");
    rank2_Graph = LoadGraph("Resource/Images/Scene/Result/Result_rank2.png");
    rank3_Graph = LoadGraph("Resource/Images/Scene/Result/Result_rank3.png");
    YourScoreGraph = LoadGraph("Resource/Images/Scene/Result/Your_Score.png");
    HighScoreHistory = LoadGraph("Resource/Images/Scene/Result/HighScore_History.png");
    ReplayButton_Graph = LoadGraph("Resource/Images/Scene/Result/Result_Replay.png");
    TitleButton_Graph = LoadGraph("Resource/Images/Scene/Result/Result_returnTitle.png");

    // 背景画像
    backgroundWaves[0] = LoadGraph("Resource/Images/Scene/Result/Backgrounds/BackgroundCloud_1.png");
    backgroundWaves[1] = LoadGraph("Resource/Images/Scene/Result/Backgrounds/BackgroundCloud_2.png");
    backgroundWaves[2] = LoadGraph("Resource/Images/Scene/Result/Backgrounds/BackgroundCloud_3.png");
    backgroundWaves[3] = LoadGraph("Resource/Images/Scene/Result/Backgrounds/BackgroundCloud_4.png");
    // 背景画像の座標
    backgroundY[0] = 0;
    backgroundY[1] = -screenHeight;
    backgroundY[2] = -screenHeight * 2;
    backgroundY[3] = -screenHeight * 3;

    // リザルトメインbgm読み込み
    result_bgm = LoadSoundMem("Resource/Sounds/BGM/Result.mp3");

    // ハイスコアデータを取得
    LoadHighScores();

    // ハイスコアSEの読み込み
    HighScore_SE = LoadSoundMem("Resource/Sounds/BGM/HighScore.mp3");

    // 数字画像（0〜9）の読み込み
    num_image = rm->GetImages("Resource/Images/Scene/InGame/Text/NumWhite.png");

    /************ UIボタン ************/
    // リトライボタン
    Button RetryButton;
    RetryButton.GraphicHandle = ReplayButton_Graph;
    RetryButton.x = 1030;
    RetryButton.y = 520;
    RetryButton.Width = UI_BUTTON_BASE_WIDTH;
    RetryButton.Height = UI_BUTTON_BASE_HEIGHT;
    RetryButton.TargetScene = eSceneType::eInGame;
    buttons.push_back(RetryButton);

    // タイトルボタン
    Button TitleButton;
    TitleButton.GraphicHandle = TitleButton_Graph;
    TitleButton.x = 1030;
    TitleButton.y = 620;
    TitleButton.Width = UI_BUTTON_BASE_WIDTH;
    TitleButton.Height = UI_BUTTON_BASE_HEIGHT;
    TitleButton.TargetScene = eSceneType::eTitle;
    buttons.push_back(TitleButton);
    /***********************************/

    // リザルトメインbgm再生
    PlaySoundMem(result_bgm, DX_PLAYTYPE_BACK);
}


eSceneType ResultScene::Update()
{
    cursorOnButton = false;

    // 背景画像の座標更新
    for (int i = 0; i < 4; ++i) 
    {
        backgroundY[i] -= scrollSpeedY;

        // もし画像が画面外に行けば座標を初期化
        if (backgroundY[i] < 0) 
        {
            backgroundY[i] += screenHeight * 4;
        }
    }

    HandleNewHighScore();

    // ボタンが押されたかチェック
    for (size_t i = 0; i < buttons.size(); i++)
    {
        Button& button = buttons[i];
        bool cursorOverButton = (cursor_pos.x >= button.x && cursor_pos.x <= button.x + button.Width &&
            cursor_pos.y >= button.y && cursor_pos.y <= button.y + button.Height);

        if (cursorOverButton)
        {
            cursorOnButton = true;
        }

        if (pad_input->GetButtonInputState(XINPUT_BUTTON_A) == ePadInputState::eRelease && cursorOverButton)
        {

            // ボタンが押されたときのSE
            PlaySoundMem(cursor_se_push, DX_PLAYTYPE_BACK);
            // メインBGMを止める
            StopSoundMem(result_bgm);
            return button.TargetScene; 

        }
    }

    // キーボード入力制御のインスタンスを取得
    InputManager* key_input = InputManager::GetInstance();
	if (key_input->GetKeyInputState(KEY_INPUT_Z) == eInputState::ePress)
	{
        // メインBGMを止める
        StopSoundMem(result_bgm);
		// Zキーが押されたらインゲーム画面に遷移
		return eSceneType::eInGame;
	}
	if (key_input->GetKeyInputState(KEY_INPUT_X) == eInputState::ePress)
	{
        // メインBGMを止める
        StopSoundMem(result_bgm);
		// Xキーが押されたらタイトル画面に遷移
		return eSceneType::eTitle;
	}
    // 左スティック入力値取得
    LeftStickTilt = pad_input->GetLeftStickTiltPercentage();

    // 左スティック入力があればカーソル位置を更新
    if (LeftStickTilt.x != 0.0f || LeftStickTilt.y != 0.0f)
    {
        cursor_pos.x += LeftStickTilt.x * 10;
        cursor_pos.y -= LeftStickTilt.y * 10;
    }

    return GetNowSceneType();
}


void ResultScene::Draw() const
{
    ////// 背景描画
    //for (int i = 0; i < 4; ++i) 
    //{
    //    DrawExtendGraph(0, backgroundY[i], 1280, backgroundY[i] + 720, backgroundWaves[i], TRUE);
    //}

    DrawGraph(0, 0, BackgroundGraph, TRUE);

	DrawString(10, 10, "Result", GetColor(255, 255, 255));
	DrawString(10, 26, "Z : InGame\nX : Title", GetColor(255, 255, 255));

    // リザルトシーン画像 (1280, 720 \ 460, 90)
    DrawExtendGraph(410, 20, 870, 110, ResultTitle_Graph, TRUE);

    // あなたのスコア画像描画
    DrawExtendGraph(100, 130, 444, 181, YourScoreGraph, TRUE);
    // ハイスコア歴画像描画
    DrawExtendGraph(100, 300, 381, 348, HighScoreHistory, TRUE);

	// 今回のスコアとレベルを描画
    DisplayCurrentRunScores();
    DrawFormatString(10, 58, GetColor(255, 255, 255), "Level: %d", LevelReached);
	DrawFormatString(10, 74, GetColor(255, 255, 255), "Score: %d", FinalScore);
    DrawFormatString(10, 90, GetColor(255, 255, 255), "Misinputs: %d", Misinputs);

	// ハイスコアを描画
	DisplayHighScores();

    // UIボタンを描画
    for (size_t i = 0; i < buttons.size(); ++i) 
    {
        DrawExtendGraph(buttons[i].x, buttons[i].y, buttons[i].x + buttons[i].Width, buttons[i].y + buttons[i].Height, buttons[i].GraphicHandle, TRUE);
    }

    // カーソル描画
    DrawCursor();
}

void ResultScene::Finalize()
{
    // スコアを保存
    SaveHighScores();

    // 画像を解放する
    /*for (size_t i = 0; buttons.size(); ++i)
    {
        if (buttons[i].GraphicHandle != -1)
        {
            DeleteGraph(buttons[i].GraphicHandle);
        }
    }*/
}

eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::eResult;
}


// ハイスコアをソートする
void ResultScene::DataSortDescending(std::vector<HighScoreEntry>& arr, int n)
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


// ハイスコアのデータを取得
void ResultScene::LoadHighScores()
{
    // ファイルを開ける
    FILE* inputFile;
    fopen_s(&inputFile, HighScoreFileName, "r");

    //エラーチェック
    if (inputFile != NULL) 
    {
        HighScoreEntry entry;
        int i = 0;
        int lineNumber = 1;

        // ファイルからレベル、スコア、ミス数の順番で読み込む
        while (fscanf_s(inputFile, "%d %d %d", &entry.level, &entry.score, &entry.misinputs) == 3)
        {
            if (lineNumber != 4)
            {
                HighScores.push_back(entry);
            }
            else
            {
                FinalScore = entry.score;
                LevelReached = entry.level;
                Misinputs = entry.misinputs;
            }

            i++;
            lineNumber++;
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
void ResultScene::SaveHighScores()
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
void ResultScene::HandleNewHighScore()
{
    HighScoreEntry newEntry;
    newEntry.level = LevelReached;
    newEntry.score = FinalScore;
    newEntry.misinputs = Misinputs;

    HighScores.push_back(newEntry);

    // ランキングソート
    DataSortDescending(HighScores, HighScores.size());

    // ランキングの数が多くないかチェック
    if (HighScores.size() > MaxHighScores)
    {
        // もし多かったら、一番下のハイスコアを削除
        while (HighScores.size() > MaxHighScores)
        {
            HighScores.pop_back();
        }
    }
}

// ハイスコアの描画
void ResultScene::DisplayCurrentRunScores() const
{
    int yOffset = 180;      // y軸オフセット
    int levelX = 350;       // レベルのX座標
    int scoreX = 500;       // スコアのX座標
    int missX = 800;        // ミスのX座標
    int rowSpacing = 100;    // 行間のスペース
    int digitWidth = 32;    // 1桁の幅（使用するフォント画像に合わせる）

    // レベルを描画
    DrawNumber(levelX, yOffset, LevelReached);

    // スコアを描画
    DrawNumber(scoreX, yOffset, FinalScore);

    // ミスの回数を描画
    DrawNumber(missX, yOffset, Misinputs);
}

// ハイスコアの描画
void ResultScene::DisplayHighScores() const
{
    int yOffset = 350;      // y軸オフセット
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
void ResultScene::DrawNumber(int x, int y, int number) const
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

// カーソル描画処理
void ResultScene::DrawCursor() const
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);  // 画像を半透明にする
    DrawExtendGraph(cursor_pos.x - 9, cursor_pos.y - 2, cursor_pos.x + 25, cursor_pos.y + 30, cursor_shadow_Graph, true);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);  // 透明度を元に戻す

    if (cursorOnButton)
    {
        DrawExtendGraph(cursor_pos.x - 20, cursor_pos.y - 20, cursor_pos.x + 20, cursor_pos.y + 20, cursor_on_Graph, true);
    }
    else
    {
        DrawExtendGraph(cursor_pos.x - 20, cursor_pos.y - 20, cursor_pos.x + 20, cursor_pos.y + 20, cursor_blank_Graph, true);
    }
}

void ResultScene::DrawBackground()
{
    DrawExtendGraph(cursor_pos.x - 20, cursor_pos.y - 20, cursor_pos.x + 20, cursor_pos.y + 20, cursor_blank_Graph, true);
}