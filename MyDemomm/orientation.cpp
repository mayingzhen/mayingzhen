#include "orientation.h"

void cOrientation::setIdentity()
{
	if (m_typeFlags.value != 0)
	{
		m_position = D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_right = D3DXVECTOR3(1.0f,0.0f,0.0f);
		m_up = D3DXVECTOR3(0.0f,1.0f,0.0f);
		m_forward = D3DXVECTOR3(0.0f,0.0f,1.0f);
		m_scale = D3DXVECTOR3(0.0f,0.0f,0.0f);
		
		m_dirtyFlags.value = 0;

		if (TEST_BIT(m_typeFlags,k_translation))
		{
			m_dirtyFlags.setBit(k_translation);
		}

		if (TEST_BIT(m_typeFlags,k_rotation))
		{
			m_dirtyFlags.setBit(k_rotation);
		}

		if (TEST_BIT(m_typeFlags,k_scale))
		{
			m_dirtyFlags.setBit(k_scale);
		}

		m_typeFlags.value = 0;
	}
}

void cOrientation::clearPosition()
{
	if (TEST_BIT(m_typeFlags,k_translation))
	{
		m_position = D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_dirtyFlags.setBit(k_translation);
		m_typeFlags.clearBit(k_translation);
	}
}

void cOrientation::clearRotation()
{
	if (TEST_BIT(m_typeFlags,k_rotation))
	{
		m_right = D3DXVECTOR3(1.0f,0.0f,0.0f);
		m_up = D3DXVECTOR3(0.0f,1.0f,0.0f);
		m_forward = D3DXVECTOR3(0.0f,0.0f,1.0f);
		m_dirtyFlags.setBit(k_rotation);
		m_typeFlags.clearBit(k_rotation);
	}
}

void cOrientation::clearScale()
{
	if (TEST_BIT(m_typeFlags,k_scale))
	{
		m_scale = D3DXVECTOR3(1.0f,0.0f,0.0f);
		m_dirtyFlags.setBit(k_scale);
		m_typeFlags.clearBit(k_scale);
	}
}

void cOrientation::setPosition(const D3DXVECTOR3& pos)
{
	m_position = pos;
	m_dirtyFlags.setBit(k_translation);
	m_typeFlags.setBit(k_translation);
}

void cOrientation::setRelPosition(const D3DXVECTOR3& pos)
{
	m_position = 
		(m_right*pos.x)
		+(m_up*pos.y)
		+(m_forward*pos.z);
	m_dirtyFlags.setBit(k_translation);
	m_typeFlags.setBit(k_translation);
}

void cOrientation::addRelPosition(const D3DXVECTOR3& pos)
{
	m_position += 
		(m_right*pos.x)
		+(m_up*pos.y)
		+(m_forward*pos.z);
	m_dirtyFlags.setBit(k_translation);
	m_typeFlags.setBit(k_translation);
}

void cOrientation::setRotation(const D3DXMATRIX& rotation)
{
	m_right = D3DXVECTOR3(rotation._11, rotation._12, rotation._13);
	m_up = D3DXVECTOR3(rotation._21, rotation._22, rotation._23);
	m_forward = D3DXVECTOR3(rotation._31, rotation._32, rotation._33);

	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);

}

void cOrientation::setRelRotation(const D3DXMATRIX& rotation)
{
	m_right = D3DXVECTOR3(rotation._11, rotation._21, rotation._31);
	m_up = D3DXVECTOR3(rotation._12, rotation._22, rotation._32);
	m_forward = D3DXVECTOR3(rotation._13, rotation._23, rotation._33);

	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);

}

void cOrientation::setRotation(const D3DXVECTOR3& forward, const D3DXVECTOR3& up)
{
	m_forward = forward;
	m_up = up;
	m_right.crossProduct(m_up, forward);

	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);
}

void cOrientation::setRotation(const D3DXVECTOR3& axis, float angle)
{
	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &axis, angle);
	setRotation(rotation);
}

void cOrientation::setRotation(float yaw, float pitch, float roll)
{
	D3DXMATRIX rotation;

	D3DXMatrixRotationYawPitchRoll(&rotation, yaw, pitch, roll);
	setRotation(rotation);
}

void cOrientation::addRelRotation(float yaw, float pitch, float roll)
{
	D3DXMATRIX rotation;

	D3DXMatrixRotationYawPitchRoll(&rotation, yaw, pitch, roll);
	addRelRotation(rotation);
}

void cOrientation::setScale(const D3DXVECTOR3& scale)
{
	m_scale = scale;
	m_dirtyFlags.setBit(k_scale);
	m_typeFlags.setBit(k_scale);
}

void cOrientation::setScale(float scale)
{
	m_scale = D3DXVECTOR3(scale, scale, scale);
	m_dirtyFlags.setBit(k_scale);
	m_typeFlags.setBit(k_scale);
}

void cOrientation::addPosition(const D3DXVECTOR3& pos)
{
	m_position += pos;
	m_dirtyFlags.setBit(k_translation);
	m_typeFlags.setBit(k_translation);
}

void cOrientation::addRotation(const D3DXMATRIX& rotation)
{
	m_right = D3DXVECTOR3(
		D3DXVec3Dot(m_right, D3DXVECTOR3(rotation._11, rotation._21, rotation._31)),
		D3DXVec3Dot(m_right, D3DXVECTOR3(rotation._12, rotation._22, rotation._32)),
		D3DXVec3Dot(m_right, D3DXVECTOR3(rotation._13, rotation._23, rotation._33)));

	m_forward = D3DXVECTOR3(
		D3DXVec3Dot(m_forward, D3DXVECTOR3(rotation._11, rotation._21, rotation._31)),
		D3DXVec3Dot(m_forward, D3DXVECTOR3(rotation._12, rotation._22, rotation._32)),
		D3DXVec3Dot(m_forward, D3DXVECTOR3(rotation._13, rotation._23, rotation._33)));

	m_up = D3DXVECTOR3(
		D3DXVec3Dot(m_up, D3DXVECTOR3(rotation._11, rotation._21, rotation._31)),
		D3DXVec3Dot(m_up, D3DXVECTOR3(rotation._12, rotation._22, rotation._32)),
		D3DXVec3Dot(m_up, D3DXVECTOR3(rotation._13, rotation._23, rotation._33)));

	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);

}


void cOrientation::addRotation(const D3DXVECTOR3& forward, const D3DXVECTOR3& up)
{
	D3DXVECTOR3 right;
	//right.crossProduct(forward, up);
	D3DXVec3Cross(&right,forward,up);

	m_right = D3DXVECTOR3(
		D3DXVec3Dot(m_right, D3DXVECTOR3(right.x, up.x, forward.x)),
		D3DXVec3Dot(m_right, D3DXVECTOR3(right.y, up.y, forward.y)),
		D3DXVec3Dot(m_right, D3DXVECTOR3(right.z, up.z, forward.z)));

	m_forward = D3DXVECTOR3(
		D3DXVec3Dot(m_forward, D3DXVECTOR3(right.x, up.x, forward.x)),
		D3DXVec3Dot(m_forward, D3DXVECTOR3(right.y, up.y, forward.y)),
		D3DXVec3Dot(m_forward, D3DXVECTOR3(right.z, up.z, forward.z)));

	m_up = D3DXVECTOR3(
		D3DXVec3Dot(m_up, D3DXVECTOR3(right.x, up.x, forward.x)),
		D3DXVec3Dot(m_up, D3DXVECTOR3(right.y, up.y, forward.y)),
		D3DXVec3Dot(m_up, D3DXVECTOR3(right.z, up.z, forward.z)));

	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);
}

void cOrientation::addRelRotation(const D3DXMATRIX& rotation)
{
	D3DXVECTOR3 col0(m_right.x, m_up.x, m_forward.x);
	D3DXVECTOR3 col1(m_right.y, m_up.y, m_forward.y);
	D3DXVECTOR3 col2(m_right.z, m_up.z, m_forward.z);

	m_right = D3DXVECTOR3(
		D3DXVec3Dot(col0, D3DXVECTOR3(rotation._11, rotation._12, rotation._13)),
		D3DXVec3Dot(col1, D3DXVECTOR3(rotation._11, rotation._12, rotation._13)),
		D3DXVec3Dot(col2, D3DXVECTOR3(rotation._11, rotation._12, rotation._13)));

	m_up = D3DXVECTOR3(
		D3DXVec3Dot(col0, D3DXVECTOR3(rotation._21, rotation._22, rotation._23)),
		D3DXVec3Dot(col1, D3DXVECTOR3(rotation._21, rotation._22, rotation._23)),
		D3DXVec3Dot(col2, D3DXVECTOR3(rotation._21, rotation._22, rotation._23)));

	m_forward = D3DXVECTOR3(
		D3DXVec3Dot(col0, D3DXVECTOR3(rotation._31, rotation._32, rotation._33)),
		D3DXVec3Dot(col1, D3DXVECTOR3(rotation._31, rotation._32, rotation._33)),
		D3DXVec3Dot(col2, D3DXVECTOR3(rotation._31, rotation._32, rotation._33)));


	m_dirtyFlags.setBit(k_rotation);
	m_typeFlags.setBit(k_rotation);

}

void cOrientation::addRotation(const D3DXVECTOR3& axis, float angle)
{
	D3DXMATRIX rotation;

	D3DXMatrixRotationAxis(&rotation, &axis, angle);
	addRotation(rotation);
}

void cOrientation::addRotation(float yaw, float pitch, float roll)
{
	D3DXMATRIX rotation;

	D3DXMatrixRotationYawPitchRoll(&rotation, yaw, pitch, roll);
	addRotation(rotation);
}

void cOrientation::addScale(const D3DXVECTOR3& scale)
{
	m_scale += scale;
	m_dirtyFlags.setBit(k_scale);
	m_typeFlags.setBit(k_scale);
}

void cOrientation::addScale(float scale)
{
	m_scale.x += scale;
	m_scale.y += scale;
	m_scale.z += scale;
	m_dirtyFlags.setBit(k_scale);
	m_typeFlags.setBit(k_scale);
}

void cOrientation::buildMatrix(D3DXMATRIX& matrix)
{
	matrix._11 = m_right.x * m_scale.x;
	matrix._12 = m_right.y;
	matrix._13 = m_right.z;
	matrix._14 = 0.0f;

	matrix._21 = m_up.x;
	matrix._22 = m_up.y * m_scale.y;
	matrix._23 = m_up.z;
	matrix._24 = 0.0f;

	matrix._31 = m_forward.x;
	matrix._32 = m_forward.y;
	matrix._33 = m_forward.z * m_scale.z;
	matrix._34 = 0.0f;

	matrix._41 = m_position.x;
	matrix._42 = m_position.y;
	matrix._43 = m_position.z;
	matrix._44 = 1.0f;
}

bool cOrientation::updateMatrix(D3DXMATRIX& matrix, bool clearDirtyFlag)
{
	if (m_dirtyFlags.value != 0)
	{
		buildMatrix(matrix);

		if (clearDirtyFlag)
		{
			m_dirtyFlags.value = 0;
		}
		return true;
	}
	return false;
}

