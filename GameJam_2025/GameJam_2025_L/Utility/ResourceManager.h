#pragma once

#include <map>
#include <string>
#include <vector>

// リソース管理クラス
class ResourceManager
{
private:
	static ResourceManager* instance;

	std::map<std::string, std::vector<int>> images_container;		// 画像コンテナ

private:
	// 他のところからオブジェクト化できないようにコンストラクタはprivate
	ResourceManager() = default;

	// コピーガード
	// コピーコンストラクタの削除
	ResourceManager(const ResourceManager&) = delete;
	// コピー代入演算子の削除
	ResourceManager& operator = (const ResourceManager&) = delete;

public:
	~ResourceManager() = default;

public:
	// インスタンスを取得
	static ResourceManager* GetInstance();
	// インスタンスの削除
	static void DeleteInstance();

public:
	// 画像コンテナから画像を取得
	const std::vector<int>& GetImages(const char* file_name, int div = 1, int dx = 1, int dy = 1, int px = 1, int py = 1);
	const std::vector<int>& GetImages(const std::string& file_name, int div = 1, int dx = 1, int dy = 1, int px = 1, int py = 1);

public:
	// 読み込んだ画像の削除処理
	void UnloadImages();

private:
	// 画像読込処理
	void CreateImages(const std::string& file_name);
	void CreateImages(const std::string& file_name, int div, int dx, int dy, int px, int py);
};