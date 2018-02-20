#pragma once

class Vector3
{
public:
	Vector3();                               
	Vector3(float X, float Y, float Z);		 
	~Vector3();												        
															        
	union													        
	{														        
		struct { float x; float y; float z; };
		struct { float Float[3]; };
	};														        
		

	Vector3& operator + (const Vector3 R);
	Vector3& operator - (const Vector3 R);
	Vector3& operator * (const float R);
	Vector3& operator / (const float R);
	friend Vector3& operator * (const float L, const Vector3 R);
	friend Vector3& operator / (const float L, const Vector3 R);

	Vector3& operator = (const Vector3 R);
	Vector3& operator += (const Vector3 R);
	Vector3& operator -= (const Vector3 R);
	Vector3& operator *= (const float R);
	Vector3& operator /= (const float R);
	friend Vector3& operator *= (const float L, Vector3 R);
	friend Vector3& operator /= (const float L, Vector3 R);

	friend bool operator == (const Vector3 L, const Vector3 R);
	friend bool operator != (const Vector3 L, const Vector3 R);

	const Vector3 operator-() const;
	float& operator[] (int Index) { return Float[Index]; }
	explicit operator float*() { return &x; };
															   
	void SetVector(float X, float Y, float Z);
};