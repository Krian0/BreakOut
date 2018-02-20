#pragma once

class Vector4
{
public:
	Vector4();
	Vector4(float X, float Y, float Z, float W);
	~Vector4();

	union 
	{
		struct { float x; float y; float z; float w; };
		struct { float Float[4]; };
	};


	Vector4& operator + (const Vector4 R);
	Vector4& operator - (const Vector4 R);
	Vector4& operator * (const float R);
	Vector4& operator / (const float R);
	friend Vector4& operator * (const float L, const Vector4 R);
	friend Vector4& operator / (const float L, const Vector4 R);

	Vector4& operator = (const Vector4 R);
	Vector4& operator += (const Vector4 R);
	Vector4& operator -= (const Vector4 R);
	Vector4& operator *= (const float R);
	Vector4& operator /= (const float R);
	friend Vector4& operator *= (const float L, Vector4 R);
	friend Vector4& operator /= (const float L, Vector4 R);

	friend bool operator == (const Vector4 L, const Vector4 R);
	friend bool operator != (const Vector4 L, const Vector4 R);

	const Vector4 operator-() const;
	float& operator[] (int Index) { return Float[Index]; }
	explicit operator float*() { return &x; };
															   
	void SetVector(float X, float Y, float Z, float W);
};