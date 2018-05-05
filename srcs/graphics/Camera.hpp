//
// Created by Marc on 12/04/2018.
//

#pragma once


#include <maths/Vec3.hpp>
#include <maths/Mat4.hpp>
#include "Display.hpp"

class Camera
{
private:
	Vec3<float>	m_position;
	Vec2<float>	m_rotation;

public:
	Camera();
	Camera(const Vec3<float> &position);
	virtual ~Camera();

	void input(const Display &display);
	void update();

	Mat4<float> getTransformation() const;

	const Vec3<float> &getPosition() const;
	const Vec2<float> &getRotation() const;
};

