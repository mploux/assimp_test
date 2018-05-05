//
// Created by Marc on 12/04/2018.
//

#pragma once

#include <ostream>

template <typename T>
class Vec3
{
private:
	T	m_x, m_y, m_z;

public:
	Vec3() : Vec3(0, 0, 0) {}
	Vec3(const T &x, const T &y, const T &z) : m_x(x), m_y(y), m_z(z) {}
	Vec3(const Vec3 &v) : m_x(v.m_x), m_y(v.m_y), m_z(v.m_z) {}
	virtual ~Vec3() = default;

	Vec3 &add(const T &x, const T &y, const T &z) { m_x += x; m_y += y; m_z += z; return *this; }
	Vec3 &sub(const T &x, const T &y, const T &z) { m_x -= x; m_y -= y; m_z -= z; return *this; }
	Vec3 &mul(const T &x, const T &y, const T &z) { m_x *= x; m_y *= y; m_z *= z; return *this; }
	Vec3 &div(const T &x, const T &y, const T &z) { m_x /= x; m_y /= y; m_z /= z; return *this; }

	Vec3 &add(const Vec3 &v) { return add(v.m_x, v.m_y, v.m_z); }
	Vec3 &sub(const Vec3 &v) { return sub(v.m_x, v.m_y, v.m_z); }
	Vec3 &mul(const Vec3 &v) { return mul(v.m_x, v.m_y, v.m_z); }
	Vec3 &div(const Vec3 &v) { return div(v.m_x, v.m_y, v.m_z); }

	Vec3 &operator+=(const Vec3 &v) { return add(v); }
	Vec3 &operator-=(const Vec3 &v) { return sub(v); }
	Vec3 &operator*=(const Vec3 &v) { return mul(v); }
	Vec3 &operator/=(const Vec3 &v) { return div(v); }

	Vec3 operator+(const Vec3 &v2) { return Vec3(m_x + v2.m_x, m_y + v2.m_y, m_z + v2.m_z); }
	Vec3 operator-(const Vec3 &v2) { return Vec3(m_x - v2.m_x, m_y - v2.m_y, m_z - v2.m_z); }
	Vec3 operator*(const Vec3 &v2) { return Vec3(m_x * v2.m_x, m_y * v2.m_y, m_z * v2.m_z); }
	Vec3 operator/(const Vec3 &v2) { return Vec3(m_x / v2.m_x, m_y / v2.m_y, m_z / v2.m_z); }

	Vec3 &operator=(const Vec3 &v) { m_x = v.m_x; m_y = v.m_y; m_z = v.m_z; return *this; }
	Vec3 &operator=(const T &v) { m_x = v; m_y = v; m_z = v; return *this; }

	bool operator==(const Vec3 &v) { return m_x == v.m_x && m_y == v.m_y && m_z == v.m_z; }

	T getX() const { return m_x; }
	T getY() const { return m_y; }
	T getZ() const { return m_z; }

	void setX(T x) { m_x = x; }
	void setY(T y) { m_y = y; }
	void setZ(T z) { m_z = z; }
};

static Vec3<int> ZERO = Vec3<int>(0, 0, 0);

template <typename T>
std::ostream &operator<<(std::ostream &os, const Vec3<T> &v) { os << v.getX() << " " << v.getY() << " " << v.getZ(); return os; }
