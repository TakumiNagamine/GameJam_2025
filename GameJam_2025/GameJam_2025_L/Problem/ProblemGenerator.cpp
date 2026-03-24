#include "ProblemGenerator.h"
#include "Dxlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// コンストラクタ
ProblemGenerator::ProblemGenerator()
	: problem()
	, level(0)
	, current_step(0)
	, miss(0)
    , level_num()
	, score()
	, miss_num()
{
    // 乱数の初期化
    srand((unsigned int)time(NULL));
}

// デストラクタ
ProblemGenerator::~ProblemGenerator()
{

}

// 問題生成処理
void ProblemGenerator::CreateProblem()
{
    // 前回の問題をリセット
    problem.clear();
    // 進行状況リセット
    current_step = 0; 
    // レベルごとに問題の長さを増やす
    int length = level + 1;

    for (int i = 0; i < length; i++)
    {
        // 0～5のランダムな鐘を選ぶ
        problem.push_back(rand() % 6);      
    }
}

// レベルアップ処理
void ProblemGenerator::LevelUp()
{
    if (level < 8)
    {
        // レベルアップ
        level++;
    }

    // 問題生成処理
    CreateProblem();
}

// プレイヤーの入力を調べる
bool ProblemGenerator::CheckAnswer(int bell_num)
{
    // プレイヤーが打った鐘と問題の鐘が同じ場合
    if (bell_num == problem[current_step])
    {
        // 進行状況を進める
        current_step++;

        // 正解
        return true;
    }
    else
    {
        // プレイヤーが失敗した場合は最初から
        current_step = 0;
        miss++;

        // 不正解
        return false;
    }
}

// 全問正解か調べる
bool ProblemGenerator::CheckAllCorrect()
{
    if (current_step >= problem.size())
    {
        // 全問正解した
        return true;
    }
    else
    {
        // 全問正解していない
        return false;
    }
}

// ランキングデータ読み込み
void ProblemGenerator::LoadRankingData()
{
    FILE* fp = nullptr;

    // ファイルを開く
    errno_t result = fopen_s(&fp, "Resource/ScoreData/HighScores.txt", "r");

    if (result == -1 || fp == nullptr)
    {
        // ファイルが開けなかったらthrowする
        throw("Resource/ScoreData/HighScores.txtが開けませんでした\n");
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        // ランキングデータ読み込み
        fscanf_s(fp, "%d %d %d", &level_num[i], &score[i], &miss_num[i]);
    }

    // ファイルを閉じる
    fclose(fp);
}

// デバッグ用描画処理
void ProblemGenerator::Draw() const
{
    DrawFormatString(400, 240, 0x00ffff, "lv: %d",level);
    DrawFormatString(400, 260, 0x00ffff, "miss: %d",miss);
    DrawFormatString(400, 280, 0x00ffff, "current_step: %d", current_step);

    DrawFormatString(400, 300, 0x00ffff, "problem");
    for (int i = 0; i < problem.size(); i++)
    {
        DrawFormatString(400, 320 + (i * 20), 0x00ffff, "target: %d", problem[i]);
    }
}

// 現在、打つ対象の鐘の番号を取得
int ProblemGenerator::GetCurrentTarget() const
{
    return problem[current_step];
}

// 現在の問題を取得する
const std::vector<int>& ProblemGenerator::GetCurrentProblem() const
{
    return problem;
}

// レベルを取得する
int ProblemGenerator::GetLevel() const
{
    return level;
}

// 現在の進行状況を取得する
int ProblemGenerator::GetCurrentStep() const
{
    return current_step;
}

// ゲーム結果をファイルに保存する処理
void ProblemGenerator::SaveGameResult()
{
    // ランキングデータ読み込み
    LoadRankingData();

    // 4行目に自分のゲームデータを追加
    level_num[3] = level;
    score[3] = level * 150 - miss * 10;
    miss_num[3] = miss;

    FILE* fp = nullptr;

    // ファイルを開く
    errno_t result = fopen_s(&fp, "Resource/ScoreData/Scores.txt", "w");

    if (result == -1 || fp == nullptr)
    {
        // ファイルが開けなかったらthrowする
        throw("Resource/ScoreData/Scores.txtが開けませんでした\n");
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        // ランキングデータ + 自分のゲームデータ書き込み
        fprintf(fp, "%d %d %d\n", level_num[i], score[i], miss_num[i]);
    }

    // ファイルを閉じる
    fclose(fp);
}