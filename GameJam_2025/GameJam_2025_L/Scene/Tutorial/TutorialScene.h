#pragma once
#include "../SceneBase.h"

// チュートリアル画面クラス
class TutorialScene : public SceneBase
{
private:
	class ProblemGenerator* problem;		// 問題ジェネレータ
	class Fade* fade;						// フェード
	class Player* player;					// プレイヤー
	class Bell* bell;						// 鐘
	class Answer* answer;					// 回答結果

	bool change_scene_flag;					// シーン変更フラグ	true: 変更可	false: 変更不可

	int page_num;							// 説明ページ番号

	std::vector<int> text_image;			// テキスト画像
	int text_alpha;							// テキストの透明度
	bool text_fade_in_flag;					// テキストフェードインフラグ	true: フェードイン	false: フェードアウト
	int change_text_count;					// テキスト変更カウント（0で変更）

	enum class eTutoriaState
	{
		eStart,				// チュートリアル開始
		ePlay,				// プレイ中
		eEnd				// チュートリアル終了
	};

	eTutoriaState state;

	std::vector<int> ui_image;				// UI画像
	int ui_alpha;							// UI画像の透明度

	bool obj_update_flag;					// オブジェクト更新フラグ

	int Tutorial_main_bgm;                  // メインbgm
	int Tutorial_cursol_move_se;            // カーソル動かしたときの音
	int Tutorial_cursol_push_se;            // 決定ボタン押したときの音
public:
	// コンストラクタ
	TutorialScene();
	// デストラクタ
	virtual ~TutorialScene();

public:
	// 初期化処理
	virtual void Initialize() override;
	// 更新処理
	virtual eSceneType Update() override;
	// 描画処理
	virtual void Draw() const override;
	// 終了処理
	virtual void Finalize() override;

public:
	// 現在のシーンを取得する
	virtual eSceneType GetNowSceneType() const override;

public:
	// テキストのフェードイン処理
	void TextFadeIn();
	// テキストのフェードアウト処理
	void TextFadeAut();
	// エフェクト生成処理
	void CreateEffect();

};