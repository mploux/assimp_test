//
// Created by Marc on 10/04/2018.
//
#pragma once

#include <graphics/Camera.hpp>
#include <inputs/Input.hpp>
#include <graphics/textures/Texture.hpp>
#include <graphics/Shader.hpp>
#include <graphics/Model.hpp>
#include "graphics/Display.hpp"

class Core
{
private:
	static Core	m_instance;
	Display		m_display;
	bool		m_running;
	Shader		m_shader;
	Camera		m_camera;
	Input		m_input;
	Texture		m_texture;
	Model		m_model;

public:
	Core();
	virtual ~Core();

	void update();
	void render();

	void start();
	void stop();
	void terminate();
	void loop();

	Display &getDisplay();
	Input &getInput();
	Camera &getCamera();

	static Core &getInstance();

	int getRenderMode() const;
};


