//
// Created by Marc on 11/04/2018.
//

#pragma once

#include <exception>
#include <string>

class VoxException : public std::exception
{
protected:
	std::string		m_name;
	std::string		m_description;

public:
	VoxException();
	virtual ~VoxException() throw() ;

	const char *what() const throw();

	void setName(const std::string &name);
	void setDescription(const std::string &description);
};
