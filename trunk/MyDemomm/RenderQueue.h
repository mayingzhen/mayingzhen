#pragma once
#include <vector>

#include "public.h"

class CObject;

class CRenderQueue /*: public CSingleton<CRenderQueue>*/
{
public:
	CRenderQueue() {}
	~CRenderQueue() {}

	void clear();
	void Render();

	size_t PushToTransRQ(CObject * obj) 
	{ m_TransRQ.push_back(obj); return m_TransRQ.size(); }

	size_t PushToNormalRQ(CObject * obj) 
	{ m_NormalRQ.push_back(obj); return m_NormalRQ.size(); }

	size_t PushToShadowSrcRQ(CObject * obj) 
	{ m_ShadowSrcRQ.push_back(obj); return m_ShadowSrcRQ.size(); }

	size_t PushToShadowDestRQ(CObject * obj) 
	{ m_ShadowDestRQ.push_back(obj); return m_ShadowDestRQ.size(); }

public:

	std::vector<CObject*> m_TransRQ;
	std::vector<CObject*> m_NormalRQ;
	std::vector<CObject*> m_ShadowSrcRQ;
	std::vector<CObject*> m_ShadowDestRQ;
};

