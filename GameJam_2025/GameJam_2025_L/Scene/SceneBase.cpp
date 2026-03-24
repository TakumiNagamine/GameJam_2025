#include "SceneBase.h"

#include "DxLib.h"

SceneBase::SceneBase() : background_image(NULL)
{

}

SceneBase::~SceneBase()
{
	//// 終了処理
	//this->Finalize();
}

void SceneBase::Initialize()
{

}

eSceneType SceneBase::Update()
{
	// 範囲for文でobjectsの要素を順番にobjに代入
	for (GameObject* obj : objects)
	{
		// オブジェクトの更新処理
		obj->Update();
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetDeleteFlag())
		{
			// 終了処理
			objects[i]->Finalize();
			// 削除
			objects.erase(objects.begin() + i);
		}
	}

	// 現在のシーンを返す
	return GetNowSceneType();
}

void SceneBase::Draw() const
{

	//DrawBox(1080, 550, 1250, 700, 0x000000, TRUE);

	// 範囲for文でobjectsの要素を順番にobjに代入
	for (GameObject* obj : objects)
	{
		// オブジェクトの描画処理
		obj->Draw();
	}
}

void SceneBase::Finalize()
{
	// objectsの要素が空だったら
	if (objects.empty())
	{
		return;
	}

	// 範囲for文でobjectsの要素を順番にobjに代入
	for (GameObject* obj : objects)
	{
		// オブジェクトの終了処理
		obj->Finalize();
		delete obj;
	}

	// objectsの要素をすべて削除
	objects.clear();
}

void SceneBase::DestroyObject(GameObject* target)
{
}
