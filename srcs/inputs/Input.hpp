//
// Created by Marc on 12/04/2018.
//

#pragma once


#include <graphics/Display.hpp>
#include <maths/Vec2.hpp>
#include <vector>
#include <algorithm>

class Input
{
private:
	const Display		&m_display;
	Vec2<float>		m_mousePosition;
	Vec2<float>		m_mouseVelocity;

	std::vector<int>	m_keys;
	std::vector<int>	m_upKeys;
	std::vector<int>	m_downKeys;

	std::vector<int>	m_buttons;
	std::vector<int>	m_upButtons;
	std::vector<int>	m_downButtons;

	bool				m_focused;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void characterCallback(GLFWwindow* window, unsigned int codepoint);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

public:
	Input(const Display &display);
	virtual ~Input();

	void setupCallbacks();
	void update();
	void reset();

	bool getKey(const int &key) const;
	bool getKeyUp(const int &key) const;
	bool getKeyDown(const int &key) const;

	bool getButton(const int &btn) const;
	bool getButtonUp(const int &btn) const;
	bool getButtonDown(const int &btn) const;

	void keyEvent(int key, int scancode, int action, int mods);
	void characterEvent(unsigned int codepoint);
	void cursorPositionEvent(double xpos, double ypos);
	void mouseButtonEvent(int button, int action, int mods);

	const Vec2<float> &getMousePosition() const;
	const Vec2<float> &getMouseVelocity() const;

	bool isFocused() const;
	void setFocused(bool focused);
};

