#pragma once

class Vector2
{
public:
	Vector2();                             
	Vector2(float X, float Y);             
	~Vector2();												   
															   
	union													   
	{														   
		struct { float x; float y; };
		struct { float Float[2]; };
	};														   
															   
															   
	Vector2& operator + (const Vector2 R);
	Vector2& operator - (const Vector2 R);
	Vector2& operator * (const float R);
	Vector2& operator / (const float R);
	friend Vector2& operator * (const float L, const Vector2 R);
	friend Vector2& operator / (const float L, const Vector2 R);

	Vector2& operator = (const Vector2 R);
	Vector2& operator += (const Vector2 R);
	Vector2& operator -= (const Vector2 R);
	Vector2& operator *= (const float R);
	Vector2& operator /= (const float R);
	friend Vector2& operator *= (const float L, Vector2 R);
	friend Vector2& operator /= (const float L, Vector2 R);

	friend bool operator == (const Vector2 L, const Vector2 R);
	friend bool operator != (const Vector2 L, const Vector2 R);

	const Vector2 operator-() const;
	float& operator[] (int Index) { return Float[Index]; }
	explicit operator float*() { return &x; };

	void SetVector(float X, float Y);
};