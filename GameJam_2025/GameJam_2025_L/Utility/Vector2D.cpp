#include "Vector2D.h"
#include "UserTemplate.h"

// コンストラクタ
Vector2D::Vector2D() : x(0.0f), y(0.0f)
{
}
Vector2D::Vector2D(float scalar) : x(scalar), y(scalar)
{
}
Vector2D::Vector2D(float mx, float my) : x(mx), y(my)
{
}

// デストラクタ
Vector2D::~Vector2D()
{
}

// x、yにlocationを代入
Vector2D& Vector2D::operator=(const Vector2D& location)
{
	this->x = location.x;
	this->y = location.y;

	return *this;
}

// x、yにlocationを加算した値を返す
const Vector2D Vector2D::operator+(const Vector2D& location) const
{
	Vector2D result = Vector2D(0.0f);

	result.x = this->x + location.x;
	result.y = this->y + location.y;

	return result;
}

// x、yにlocationを加算代入
Vector2D& Vector2D::operator+=(const Vector2D& location)
{
	this->x += location.x;
	this->y += location.y;

	return *this;
}

// x、yからlocationを減算した値を返す
const Vector2D Vector2D::operator-(const Vector2D& location) const
{
	Vector2D result = Vector2D(0.0f);

	result.x = this->x - location.x;
	result.y = this->y - location.y;

	return result;
}

// x、yからlocationを減算代入
Vector2D& Vector2D::operator-=(const Vector2D& location)
{
	this->x -= location.x;
	this->y -= location.y;

	return *this;
}

// x、yをscalar倍にした値を返す
const Vector2D Vector2D::operator*(const float& scalar) const
{
	Vector2D result = Vector2D(0.0f);

	result.x = this->x * scalar;
	result.y = this->y * scalar;

	return result;
}

// x、yにlocationを掛けた値を返す
const Vector2D Vector2D::operator*(const Vector2D& location) const
{
	Vector2D result = Vector2D(0.0f);

	result.x = this->x * location.x;
	result.y = this->y * location.y;

	return result;
}

// x、yをscalar倍にする
Vector2D& Vector2D::operator*=(const float& scalar)
{
	this->x *= scalar;
	this->y *= scalar;

	return *this;
}

// x、yにlocationを掛ける
Vector2D& Vector2D::operator*=(const Vector2D& location)
{
	this->x *= location.x;
	this->y *= location.y;

	return *this;
}

// x、yをscalarで割った値を返す
const Vector2D Vector2D::operator/(const float& scalar) const
{
	// 1e-6f・・・しきい値
	if (Abs(scalar) < 1e-6f)
	{
		return Vector2D(0.0f);
	}

	return Vector2D(this->x / scalar, this->y / scalar);
}

// x、yをlocationで割った値を返す
const Vector2D Vector2D::operator/(const Vector2D& location) const
{
	// 1e-6f・・・しきい値
	if ((Abs(location.x) < 1e-6f) || (Abs(location.y) < 1e-6f))
	{
		return Vector2D(0.0f);
	}

	return Vector2D(this->x / location.x, this->y / location.y);
}

// x、yをscalarで割る
Vector2D& Vector2D::operator/=(const float& scalar)
{
	if (Abs(scalar) < 1e-6f)
	{
		this->x = 0.0f;
		this->y = 0.0f;
	}
	else
	{
		this->x /= scalar;
		this->y /= scalar;
	}
	return *this;
}

// x、yをlocationで割る
Vector2D& Vector2D::operator/=(const Vector2D& location)
{
	if ((Abs(location.x) < 1e-6f) || (Abs(location.y) < 1e-6f))
	{
		this->x = 0.0f;
		this->y = 0.0f;
	}
	else
	{
		this->x /= location.x;
		this->y /= location.y;
	}
	return *this;
}

// int型に変換
void Vector2D::ToInt(int* x, int* y)
{
	*x = static_cast<int>(this->x);
	*y = static_cast<int>(this->y);
}
