//
// Created by Marc on 12/04/2018.
//

#pragma once

#include <ostream>

template <typename T>
class Vec2
{
private:
	T	m_x, m_y;

public:
	Vec2() : Vec2(0, 0) {}
	Vec2(const T &v) : Vec2(v, v) {}
	Vec2(const T &x, const T &y) : m_x(x), m_y(y) {}
	Vec2(const Vec2 &v) : m_x(v.m_x), m_y(v.m_y) {}
	virtual ~Vec2() = default;

	Vec2 &add(const T &x, const T &y) { m_x += x; m_y += y; return *this; }
	Vec2 &sub(const T &x, const T &y) { m_x -= x; m_y -= y; return *this; }
	Vec2 &mul(const T &x, const T &y) { m_x *= x; m_y *= y; return *this; }
	Vec2 &div(const T &x, const T &y) { m_x /= x; m_y /= y; return *this; }

	Vec2 &add(const Vec2 &v) { return add(v.m_x, v.m_y); }
	Vec2 &sub(const Vec2 &v) { return sub(v.m_x, v.m_y); }
	Vec2 &mul(const Vec2 &v) { return mul(v.m_x, v.m_y); }
	Vec2 &div(const Vec2 &v) { return div(v.m_x, v.m_y); }

	Vec2 &operator+=(const Vec2 &v) { return add(v); }
	Vec2 &operator-=(const Vec2 &v) { return sub(v); }
	Vec2 &operator*=(const Vec2 &v) { return mul(v); }
	Vec2 &operator/=(const Vec2 &v) { return div(v); }

	Vec2 operator+(const Vec2 &v2) { return Vec2(m_x + v2.m_x, m_y + v2.m_y); }
	Vec2 operator-(const Vec2 &v2) { return Vec2(m_x - v2.m_x, m_y - v2.m_y); }
	Vec2 operator*(const Vec2 &v2) { return Vec2(m_x * v2.m_x, m_y * v2.m_y); }
	Vec2 operator/(const Vec2 &v2) { return Vec2(m_x / v2.m_x, m_y / v2.m_y); }

	Vec2 operator+(const T &v2) { return Vec2(m_x + v2, m_y + v2); }
	Vec2 operator-(const T &v2) { return Vec2(m_x - v2, m_y - v2); }
	Vec2 operator*(const T &v2) { return Vec2(m_x * v2, m_y * v2); }
	Vec2 operator/(const T &v2) { return Vec2(m_x / v2, m_y / v2); }

	Vec2 &operator=(const Vec2 &v) { m_x = v.m_x; m_y = v.m_y; return *this; }
	Vec2 &operator=(const T &v) { m_x = v; m_y = v; return *this; }

	bool operator==(const Vec2 &v) { return m_x == v.m_x && m_y == v.m_y; }

	T getX() const { return m_x; }

	T getY() const { return m_y; }
	void setX(T x) { m_x = x; }

	void setY(T y) { m_y = y; }
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Vec2<T> &v) { os << v.getX() << " " << v.getY(); return os; }
