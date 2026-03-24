#include "ResourceManager.h"

#include "DxLib.h"

// 静的メンバ変数の初期化
ResourceManager* ResourceManager::instance = nullptr;

// インスタンスを取得
ResourceManager* ResourceManager::GetInstance()
{
	if (instance == nullptr)
	{
		// 最初の1回だけオブジェクトを生成する
		instance = new ResourceManager();
	}

	// 実体を返す
	return instance;
}

// インスタンスの削除
void ResourceManager::DeleteInstance()
{
	if (instance != nullptr)
	{
		// 読み込んだ画像の削除処理
		instance->UnloadImages();
		delete instance;
	}
}

// 読み込んだ画像の取得
const std::vector<int>& ResourceManager::GetImages(const char* file_name, int div, int dx, int dy, int px, int py)
{
	return GetImages(std::string(file_name), div, dx, dy, px, py);
}

// 読み込んだ画像の取得
const std::vector<int>& ResourceManager::GetImages(const std::string& file_name, int div, int dx, int dy, int px, int py)
{
	// 画像が無かったら新しく画像を読み込む
	if (images_container.count(file_name) == NULL)
	{
		if (div == 1)
		{
			// 画像読込
			CreateImages(file_name);
		}
		else
		{
			// 画像分割読込
			CreateImages(file_name, div, dx, dy, px, py);
		}
	}

	return images_container[file_name];
}

// 画像の削除処理
void ResourceManager::UnloadImages()
{
	// 要素が空なら関数を終了する
	if (images_container.empty())
	{
		return;
	}

	for (std::pair<std::string, std::vector<int>> data : images_container)
	{
		// 指定したグラフィックハンドルと派生しているグラフィックハンドルの削除
		// LoadDivGraphで読み込んで作成された複数のハンドルも削除される
		DeleteSharingGraph(data.second[0]);
		data.second.clear();
	}

	// 要素を削除
	images_container.clear();
}

// 画像読込処理
void ResourceManager::CreateImages(const std::string& file_name)
{
	// 画像読込
	int handle = LoadGraph(file_name.c_str());

	if (handle == -1)
	{
		// 画像が読み込めなかったらthrowする
		throw(file_name + "の画像が見つかりませんでした\n");
		return;
	}

	// 読み込んだ画像をimages_containerの末尾に格納
	images_container[file_name].push_back(handle);
}

// 画像読込処理
void ResourceManager::CreateImages(const std::string& file_name, int div, int dx, int dy, int px, int py)
{
	// [div]個の動的配列の確保
	// 先頭アドレスがhandleに格納される
	int* handle = new int[div];

	// 画像分割読込
	int result = LoadDivGraph(file_name.c_str(), div, dx, dy, px, py, handle);

	if (result == -1)
	{
		// メモリの開放
		delete[] handle;
		// 画像が読み込めなかったらthrowする
		throw(file_name + "の画像が見つかりませんでした\n");
		return;
	}

	for (int i = 0; i < div; i++)
	{
		// 読み込んだ分割画像を一枚ずつimages_containerの末尾に格納
		images_container[file_name].push_back(handle[i]);
	}

	// メモリの開放
	delete[] handle;
}