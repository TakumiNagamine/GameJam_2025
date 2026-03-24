#pragma once

#include "../Utility/Vector2D.h"

class GameObject
{
protected:
	Vector2D location;			// 座標
	int graphic_images;			// 描画する画像
	int flip_flag;				// 画像反転フラグ
	bool delete_flag;			// 削除フラグ

public:
	GameObject();
	virtual ~GameObject();

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Draw() const;
	virtual void Finalize();

public:
	// 座標を取得
	const Vector2D& GetLocation() const
	{
		return location;
	}
	// 座標を設定
	void SetLocation(const Vector2D& location)
	{
		this->location = location;
	}
	// 削除フラグを取得
	const bool GetDeleteFlag() const
	{
		return delete_flag;
	}
};