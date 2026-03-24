#pragma once

#include "SceneBase.h"

// シーン管理クラス
class SceneManager
{
private:
	// 現在のシーン
	SceneBase* current_scene;
	// ゲームループを行うかのフラグ
	bool loop_flag;

public:
	SceneManager();
	~SceneManager();

public:
	void Initialize();
	void Update();
	void Finalize();

public:
	// ゲームループを行うか調べる処理
	bool LoopCheck() const;

private:
	// シーン切り替え処理
	void ChangeScene(eSceneType new_scene_type);
	// シーン生成処理
	SceneBase* CreateScene(eSceneType new_scene_type);
};