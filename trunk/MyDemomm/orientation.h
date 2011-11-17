#ifndef CORIENTATION_H
#define CORIENTATION_H
#pragma once

#include <d3dx9.h>
#include "bit_flags.h"

class cOrientation
{
public:

	// Data Types & Constants...
	enum e_flag_bits
	{
		k_translation,
		k_rotation,
		k_scale,
	};

	// Public Data...

	// Creators...
    cOrientation();
    cOrientation(const cOrientation& Src);
    ~cOrientation(){};

	// Operators...
    cOrientation& operator=(const cOrientation& Src);

	// Mutators...
	void setIdentity();

	void clearPosition();
	void clearRotation();
	void clearScale();

	void setRelRotation(const D3DXMATRIX& matrix);
	void addRelRotation(float yaw, float pitch, float roll);
	void addRelRotation(const D3DXMATRIX& matrix);

	void setRelPosition(const D3DXVECTOR3& pos);
	void addRelPosition(const D3DXVECTOR3& pos);

	void setPosition(const D3DXVECTOR3& pos);
	void setRotation(const D3DXVECTOR3& forward, const D3DXVECTOR3& up);
	void setRotation(const D3DXVECTOR3& axis, float angle);
	void setRotation(float yaw, float pitch, float roll);
	void setRotation(const D3DXMATRIX& matrix);
	void setScale(const D3DXVECTOR3& scale);
	void setScale(float scale);

	void addPosition(const D3DXVECTOR3& pos);
	void addRotation(const D3DXVECTOR3& forward, const D3DXVECTOR3& up);
	void addRotation(const D3DXVECTOR3& axis, float angle);
	void addRotation(float yaw, float pitch, float roll);
	void addRotation(const D3DXMATRIX& matrix);
	void addScale(const D3DXVECTOR3& scale);
	void addScale(float scale);

	const D3DXVECTOR3& getPosition()const;
	const D3DXVECTOR3& getScale()const;
	const D3DXVECTOR3& getForward()const;
	const D3DXVECTOR3& getRight()const;
	const D3DXVECTOR3& getUp()const;
	const u16Flags& typeFlags()const;
	const u16Flags& dirtyFlags()const;

	void buildMatrix(D3DXMATRIX& matrix);
	bool updateMatrix(D3DXMATRIX& matrix, bool clearDirtyFlag = true);

private:

	// Private Data...
	D3DXVECTOR3	m_right;
	D3DXVECTOR3	m_up;
	D3DXVECTOR3	m_forward;
	D3DXVECTOR3	m_position;
	D3DXVECTOR3	m_scale;
	u16Flags	m_typeFlags;
	u16Flags	m_dirtyFlags;

};


inline cOrientation::cOrientation()
:m_right(1.0f,0.0f,0.0f)
,m_up(0.0f,1.0f,0.0f)
,m_forward(0.0f,0.0f,1.0f)
,m_position(0.0f,0.0f,0.0f)
,m_scale(1.0f,1.0f,1.0f)
,m_typeFlags(0)
,m_dirtyFlags(0)
{
}


inline cOrientation::cOrientation(const cOrientation& Src)
:m_right(Src.m_right)
,m_up(Src.m_up)
,m_forward(Src.m_forward)
,m_position(Src.m_position)
,m_scale(Src.m_scale)
,m_typeFlags(Src.m_typeFlags)
,m_dirtyFlags(Src.m_dirtyFlags)
{
}


inline cOrientation& cOrientation::operator=(const cOrientation& Src)
{
	m_position = Src.m_position;
	m_right = Src.m_right;
	m_up = Src.m_up;
	m_forward = Src.m_forward;
	m_scale = Src.m_scale;
	m_typeFlags = Src.m_typeFlags;
	m_dirtyFlags = Src.m_dirtyFlags;

    return(*this);
}


inline const D3DXVECTOR3& cOrientation::getPosition()const
{
	return m_position;
}

inline const D3DXVECTOR3& cOrientation::getScale()const
{
	return m_scale;
}

inline const D3DXVECTOR3& cOrientation::getForward()const
{
	return m_forward;
}

inline const D3DXVECTOR3& cOrientation::getRight()const
{
	return m_right;
}

inline const D3DXVECTOR3& cOrientation::getUp()const
{
	return m_up;
}


inline const u16Flags& cOrientation::typeFlags()const
{
	return m_typeFlags;
}

inline const u16Flags& cOrientation::dirtyFlags()const
{
	return m_dirtyFlags;
}


#endif  
