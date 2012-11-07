#include "Framework/Object.h"

void Object::AddChild(const Object* pObject)
{
	if (pObject == NULL)
		return;

	std::vector<Object*>::iterator it = std::find(m_vChilds.begin(), m_vChilds.end(), pObject);
	if (it ! = m_vChilds.end())
		return;

	m_vChilds.push_back(pObject);
}

UINT Object::GetChildNumber()
{
	return m_vChilds.size();
}

Object* Object::GetChildByIndex(UINT uIndex)
{
	assert(uIndex >=0 && uIndex < m_vChilds.size());
	if (uIndex < 0 || uIndex >= m_vChilds.size())
		return NULL;

	return m_vChilds[uIndex];
}

void Object::Render()
{
	for(UINT i = 0; i < m_vChilds.size(); ++i)
	{
		m_vChilds[i].Render();
	}
}

void Object::Update()
{
	for(UINT i = 0; i < m_vChilds.size(); ++i)
	{
		m_vChilds[i].Update();
	}
}

void Object::ParalleUpdate(float fTimeElapsed)
{
	for(UINT i = 0; i < m_vChilds.size(); ++i)
	{
		m_vChilds[i].ParalleUpdate();
	}
}

void Object::FixedUpdate(float fTimeElapsed)
{
	for(UINT i = 0; i < m_vChilds.size(); ++i)
	{
		m_vChilds[i].FixedUpdate();
	}
}

void Object::LateUpdate(float fTimeElapsed)
{
	for(UINT i = 0; i < m_vChilds.size(); ++i)
	{
		m_vChilds[i].LateUpdate();
	}
}

void Object::Start()
{
	for(UINT i = 0; i < m_vChilds.size(); ++i)
	{
		m_vChilds[i].Start();
	}
}

void Object::Stop()
{
	for(UINT i = 0; i < m_vChilds.size(); ++i)
	{
		m_vChilds[i].Stop();
	}
}
