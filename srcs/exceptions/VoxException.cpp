//
// Created by Marc on 11/04/2018.
//

#include "VoxException.hpp"

VoxException::VoxException()
	: exception(), m_name("VoxException"), m_description("Unknown vox exception")
{}

VoxException::~VoxException() throw()
{}

const char *VoxException::what() const throw()
{
	std::string msg = m_name + ": " + m_description + " !";
	return msg.c_str();
}

void VoxException::setName(const std::string &name)
{
	m_name = name;
}

void VoxException::setDescription(const std::string &description)
{
	m_description = description;
}
