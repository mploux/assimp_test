//
// Created by Marc on 12/04/2018.
//

#include <Core.hpp>
#include "Camera.hpp"
#include "Display.hpp"

Camera::Camera()
	: m_position(Vec3<float>(0.0f, 0.0f, 0.0f)), m_rotation(Vec2<float>(0.0f, 0.0f))
{

}

Camera::Camera(const Vec3<float> &position)
	: m_position(position), m_rotation(Vec2<float>(0.0f, 0.0f))
{

}

Camera::~Camera()
{

}

void Camera::input(const Display &display)
{
	if (!Core::getInstance().getInput().isFocused())
		return;

	(void) display;

	Vec2<float> mouseVelocity;
	mouseVelocity.setX(Core::getInstance().getInput().getMouseVelocity().getY());
	mouseVelocity.setY(Core::getInstance().getInput().getMouseVelocity().getX());

	m_rotation += mouseVelocity * 0.25f;

	if (m_rotation.getX() > 89)
		m_rotation.setX(89);
	if (m_rotation.getX() < -89)
		m_rotation.setX(-89);

	if (Core::getInstance().getInput().getKey(GLFW_KEY_W))
	{
		m_position.setZ(m_position.getZ() - cos(DTR(m_rotation.getY())) * 0.1f);
		m_position.setX(m_position.getX() - sin(DTR(m_rotation.getY())) * 0.1f);
	}
	if (Core::getInstance().getInput().getKey(GLFW_KEY_S))
	{
		m_position.setZ(m_position.getZ() + cos(DTR(m_rotation.getY())) * 0.1f);
		m_position.setX(m_position.getX() + sin(DTR(m_rotation.getY())) * 0.1f);
	}
	if (Core::getInstance().getInput().getKey(GLFW_KEY_A))
	{
		m_position.setZ(m_position.getZ() - sin(DTR(m_rotation.getY())) * 0.1f);
		m_position.setX(m_position.getX() + cos(DTR(m_rotation.getY())) * 0.1f);
	}
	if (Core::getInstance().getInput().getKey(GLFW_KEY_D))
	{
		m_position.setZ(m_position.getZ() + sin(DTR(m_rotation.getY())) * 0.1f);
		m_position.setX(m_position.getX() - cos(DTR(m_rotation.getY())) * 0.1f);
	}
	if (Core::getInstance().getInput().getKey(GLFW_KEY_SPACE))
		m_position.setY(m_position.getY() - 0.1f);
	if (Core::getInstance().getInput().getKey(GLFW_KEY_LEFT_SHIFT))
		m_position.setY(m_position.getY() + 0.1f);
}

void Camera::update()
{
}

Mat4<float> Camera::getTransformation() const
{
	Mat4<float> perspective = Mat4<float>::perspective(80.0f, 1280.0f / 720.0f, 0.1f, 500.0f);
	Mat4<float> translation = Mat4<float>::translate(m_position);
	Mat4<float> rotation_x = Mat4<float>::rotate(Vec3<float>(1, 0, 0), m_rotation.getX());
	Mat4<float> rotation_y = Mat4<float>::rotate(Vec3<float>(0, 1, 0), m_rotation.getY());

	return perspective * rotation_x * rotation_y * translation;
}

const Vec3<float> &Camera::getPosition() const
{
	return m_position;
}

const Vec2<float> &Camera::getRotation() const
{
	return m_rotation;
}
