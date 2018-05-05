//
// Created by Marc on 11/04/2018.
//

#pragma once

class ABlock
{
protected:
	unsigned char	m_texture[6];
	char			m_type;
	bool			m_solid;
	bool			m_opaque;

public:
	ABlock(const unsigned char &texture);
	ABlock(const unsigned char &top, const unsigned char &bottom,
			const unsigned char &left, const unsigned char &right,
			const unsigned char &front, const unsigned char &back);

	virtual ~ABlock();

	virtual char getType() const;
	virtual bool isSolid() const;
	virtual bool isOpaque() const;
	virtual bool isRendered() const;

	const unsigned char *getTextureData() const;
};
