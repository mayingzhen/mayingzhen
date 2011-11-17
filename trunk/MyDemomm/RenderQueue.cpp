
#include "RenderQueue.h"
#include "CObject.h"


void CRenderQueue::clear()
{
	m_NormalRQ.clear();
	m_TransRQ.clear();
	m_ShadowSrcRQ.clear();
	m_ShadowDestRQ.clear();

}

void CRenderQueue::Render()
{
	// ��ͨ����
	for (int i = 0; i < m_NormalRQ.size(); ++i)
	{
		m_NormalRQ[i]->Render();
	}

	// ��͸������
	for (int i = 0; i < m_TransRQ.size(); ++i)
	{
		m_TransRQ[i]->Render();
	}
}