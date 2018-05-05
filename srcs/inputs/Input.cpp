//
// Created by Marc on 12/04/2018.
//

#include <Core.hpp>
#include "Input.hpp"

Input::Input(const Display &display)
	: m_display(display)
{
}

Input::~Input()
{

}

void Input::setupCallbacks()
{
	glfwSetKeyCallback(m_display.getWindow(), &keyCallback);
	glfwSetCharCallback(m_display.getWindow(), &characterCallback);
	glfwSetCursorPosCallback(m_display.getWindow(), &cursorPositionCallback);
	glfwSetMouseButtonCallback(m_display.getWindow(), &mouseButtonCallback);
}

void Input::update()
{
	static Vec2<float> s_lastPosition = m_mousePosition;

	if (m_focused)
	{
		glfwSetInputMode(m_display.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		m_mouseVelocity = (m_mousePosition - s_lastPosition);
		s_lastPosition = m_mousePosition;
	}
	else
	{
		glfwSetInputMode(m_display.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		s_lastPosition = m_mousePosition;
	}
}

void Input::reset()
{
	m_upKeys.clear();
	m_downKeys.clear();
	m_upButtons.clear();
	m_downButtons.clear();
}

void Input::keyEvent(int key, int scancode, int action, int mods)
{
	(void) scancode;
	(void) mods;

	if (action == GLFW_PRESS)
	{
		if (std::find(m_keys.begin(), m_keys.end(), key) == m_keys.end())
			m_keys.push_back(key);
		if (std::find(m_downKeys.begin(), m_downKeys.end(), key) == m_downKeys.end())
			m_downKeys.push_back(key);
	}
	else if (action == GLFW_RELEASE)
	{
		if (std::find(m_upKeys.begin(), m_upKeys.end(), key) == m_upKeys.end())
			m_upKeys.push_back(key);
		auto removeIndex = std::find(m_keys.begin(), m_keys.end(), key);
		if (removeIndex != m_keys.end())
			m_keys.erase(removeIndex);
	}
}

void Input::characterEvent(unsigned int codepoint)
{
	(void) codepoint;
}

void Input::cursorPositionEvent(double xpos, double ypos)
{
	m_mousePosition.setX(xpos);
	m_mousePosition.setY(ypos);
}

void Input::mouseButtonEvent(int button, int action, int mods)
{
	(void) mods;

	if (action == GLFW_PRESS)
	{
		if (std::find(m_buttons.begin(), m_buttons.end(), button) == m_buttons.end())
			m_buttons.push_back(button);
		if (std::find(m_downButtons.begin(), m_downButtons.end(), button) == m_downButtons.end())
			m_downButtons.push_back(button);
	}
	else if (action == GLFW_RELEASE)
	{
		if (std::find(m_upButtons.begin(), m_upButtons.end(), button) == m_upButtons.end())
			m_upButtons.push_back(button);
		auto removeIndex = std::find(m_buttons.begin(), m_buttons.end(), button);
		if (removeIndex != m_buttons.end())
			m_buttons.erase(removeIndex);
	}
}

bool Input::getKey(const int &key) const { return std::find(m_keys.begin(), m_keys.end(), key) != m_keys.end(); }
bool Input::getKeyUp(const int &key) const { return std::find(m_upKeys.begin(), m_upKeys.end(), key) != m_upKeys.end(); }
bool Input::getKeyDown(const int &key) const { return std::find(m_downKeys.begin(), m_downKeys.end(), key) != m_downKeys.end(); }

bool Input::getButton(const int &btn) const { return std::find(m_buttons.begin(), m_buttons.end(), btn) != m_buttons.end(); }
bool Input::getButtonUp(const int &btn) const { return std::find(m_upButtons.begin(), m_upButtons.end(), btn) != m_upButtons.end(); }
bool Input::getButtonDown(const int &btn) const { return std::find(m_downButtons.begin(), m_downButtons.end(), btn) != m_downButtons.end(); }

const Vec2<float> &Input::getMousePosition() const { return m_mousePosition; }
const Vec2<float> &Input::getMouseVelocity() const { return m_mouseVelocity; }

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) { (void) window; Core::getInstance().getInput().keyEvent(key, scancode, action, mods); }
void Input::characterCallback(GLFWwindow* window, unsigned int codepoint) { (void) window;Core::getInstance().getInput().characterEvent(codepoint); }
void Input::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) { (void) window;Core::getInstance().getInput().cursorPositionEvent(xpos, ypos); }
void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) { (void) window;Core::getInstance().getInput().mouseButtonEvent(button, action, mods); }

bool Input::isFocused() const { return m_focused; }
void Input::setFocused(bool focused) { m_focused = focused; }
