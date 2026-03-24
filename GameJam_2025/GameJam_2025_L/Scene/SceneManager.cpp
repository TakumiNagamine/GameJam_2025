#include "SceneManager.h"

#include "Title/TitleScene.h"
#include "Tutorial/TutorialScene.h"
#include "InGame/InGameScene.h"
#include "Ranking/RankingScene.h"
#include "Result/ResultScene.h"
#include "End/EndScene.h"

SceneManager::SceneManager()
	: current_scene(nullptr)
	, loop_flag(true)
{

}

SceneManager::~SceneManager()
{
	// 自身の終了処理を呼ぶ
	this->Finalize();
}

void SceneManager::Initialize()
{
	// SceneManagerが生成されたときのScene（ゲーム開始時のScene）
	ChangeScene(eSceneType::eTitle);
}

void SceneManager::Update()
{
	// シーンの更新処理
	// current_sceneのUpdateで返されたシーンタイプを代入
	// （次に処理を行うシーンタイプを保持）
	eSceneType next_scene_type = current_scene->Update();

	// シーンの描画処理
	current_scene->Draw();

	if (next_scene_type != current_scene->GetNowSceneType())
	{
		// シーン変更
		ChangeScene(next_scene_type);
	}
}

void SceneManager::Finalize()
{
	if (current_scene != nullptr)
	{
		// current_sceneの終了処理
		current_scene->Finalize();

		// current_sceneが確保したメモリを解放
		delete current_scene;
		// current_sceneのポインタを無効化
		current_scene = nullptr;
	}
}

bool SceneManager::LoopCheck() const
{
	return loop_flag;
}

// シーン切り替え処理
void SceneManager::ChangeScene(eSceneType new_scene_type)
{
	/*if (new_scene_type == eSceneType::eEnd)
	{
		// ゲームループを終了させる
		loop_flag = false;
		return;
	}*/

	// シーンの生成で返ってきたポインタをnew_sceneに代入
	SceneBase* new_scene = CreateScene(new_scene_type);

	// 上のCreateScene(new_scene_type)でキャストが失敗した場合
	if (new_scene == nullptr)
	{
		throw("\n新しいシーンの生成ができませんでした\n");
	}

	if (current_scene != nullptr)
	{
		// current_sceneの終了処理
		current_scene->Finalize();

		// current_sceneが確保したメモリを解放
		delete current_scene;
	}

	// 新しいSceneの初期化関数
	new_scene->Initialize();

	current_scene = new_scene;
}

// シーンの生成
SceneBase* SceneManager::CreateScene(eSceneType new_scene_type)
{
	// dynamic_castで、生成したSceneがSceneBaseを継承しているかチェック
	// 生成したSceneのポインタを返す
	switch (new_scene_type)
	{
		case eSceneType::eTitle:
			return dynamic_cast<SceneBase*>(new TitleScene());
		case eSceneType::eInGame:
			return dynamic_cast<SceneBase*>(new InGameScene());
		case eSceneType::eTutorial:
			return dynamic_cast<SceneBase*>(new TutorialScene());
		case eSceneType::eRanking:
			return dynamic_cast<SceneBase*>(new RankingScene());
		case eSceneType::eResult:
			return dynamic_cast<SceneBase*>(new ResultScene());
		case eSceneType::eEnd:
			return dynamic_cast<SceneBase*>(new EndScene());
		default:
			return nullptr;
	}
}
