#pragma once

#include <stdio.h>
#define _USE_MATH_DEFINES 
#include <math.h>

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

struct Vector2i
{
	int x;
	int y;
};

struct Vector2f
{
	float x;
	float y;

	Vector2f()
	{
	}

	Vector2f(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
};

struct Vector3f
{
	float x;
	float y;
	float z;

	Vector3f()
	{
	}

	Vector3f(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f(float f)
	{
		x = y = z = f;
	}

	Vector3f& operator+=(const Vector3f& r)
	{
		x += r.x;
		y += r.y;
		z += r.z;

		return *this;
	}

	Vector3f& operator-=(const Vector3f& r)
	{
		x -= r.x;
		y -= r.y;
		z -= r.z;

		return *this;
	}

	Vector3f& operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	operator const float*() const
	{
		return &(x);
	}


	Vector3f Cross(const Vector3f& v) const;

	Vector3f& Normalize();

	void Rotate(float Angle, const Vector3f& Axis);

	void Print() const
	{
		printf("(%.02f, %.02f, %.02f)", x, y, z);
	}
};

struct Vector4f
{
	float x;
	float y;
	float z;
	float w;

	Vector4f()
	{
	}

	Vector4f(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	void Print() const
	{
		printf("(%.02f, %.02f, %.02f, %.02f)", x, y, z, w);
	}

	Vector3f to3f() const
	{
		Vector3f v(x, y, z);
		return v;
	}
};

inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
{
	Vector3f Ret(l.x + r.x,
		l.y + r.y,
		l.z + r.z);

	return Ret;
}

inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
{
	Vector3f Ret(l.x - r.x,
		l.y - r.y,
		l.z - r.z);

	return Ret;
}

inline Vector3f operator*(const Vector3f& l, float f)
{
	Vector3f Ret(l.x * f,
		l.y * f,
		l.z * f);

	return Ret;
}

inline Vector4f operator/(const Vector4f& l, float f)
{
	Vector4f Ret(l.x / f,
		l.y / f,
		l.z / f,
		l.w / f);

	return Ret;
}

struct PersProjInfo
{
	float FOV;
	float Width;
	float Height;
	float zNear;
	float zFar;
};

class Matrix4f
{
public:
	float m[4][4];

	Matrix4f()
	{
	}

	inline Matrix4f operator*(const Matrix4f& Right) const
	{
		Matrix4f Ret;
		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++) {
				Ret.m[i][j] = m[i][0] * Right.m[0][j] +
					m[i][1] * Right.m[1][j] +
					m[i][2] * Right.m[2][j] +
					m[i][3] * Right.m[3][j];
			}
		}

		return Ret;
	}

	void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
	void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
	void InitTranslationTransform(float x, float y, float z);
    void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);
	void InitPersProjTransform(const PersProjInfo& p);
	void InitOrthoProjTransform(const PersProjInfo& p);
};

struct Quaternion
{
	float x, y, z, w;

	Quaternion(float _x, float _y, float _z, float _w);

	void Normalize();

	Quaternion Conjugate();
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const Vector3f& v);