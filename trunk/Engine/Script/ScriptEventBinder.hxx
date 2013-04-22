#include <Framework/Script/ScriptEventBinder.h>

namespace ma
{

	//------------------------------------------------------------------------------
	//ScriptEventBinder
	//------------------------------------------------------------------------------

	ScriptEventBinder::ScriptEventBinder(ScriptEventFactory* pFactory)
		:m_pFactory(pFactory)
	{

	}

	void ScriptEventBinder::Release()
	{
		m_pFactory->ReleaseScriptEventBinder(this);
	}

	const ScriptEventBinderKey& ScriptEventBinder::GetKey() const
	{
		return m_key;
	}

	bool ScriptEventBinder::BindScriptEvent(S3AObject* pSender,S3AName signal,ScriptObjectHandle hReciver,S3AName slot)
	{
		m_key.m_sender = (int)pSender;
		m_key.m_signal = signal;
		m_key.m_reciver = hReciver;
		m_key.m_slot	= slot;

		return BindScriptEventImpl(pSender,signal,hReciver,slot);
	}

	//------------------------------------------------------------------------------
	//ScriptEventFactory
	//------------------------------------------------------------------------------

	void DefaultScriptEventFactory::RegisterScriptEvent(ScriptSignalKey signiture,FunCreateScriptEventBinder fCreate)
	{
		m_createMap[signiture] = fCreate;
	}

	void DefaultScriptEventFactory::UnregisterScriptEvent(ScriptSignalKey signiture,FunCreateScriptEventBinder fCreate)
	{
		BinderCreateMap::iterator iter = m_createMap.find(signiture);
		if (iter != m_createMap.end() && iter->second == fCreate)
		{
			m_createMap.erase(iter);
		}
	}

	ScriptEventBinder*	DefaultScriptEventFactory::CreateScriptEventBinder(ScriptSignalKey key)
	{
		BinderCreateMap::iterator iter = m_createMap.find(key);
		if (iter != m_createMap.end())
		{
			return iter->second(this);
		}

		return NULL;
	}

	void				DefaultScriptEventFactory::ReleaseScriptEventBinder(ScriptEventBinder* pBinder)
	{
		pBinder->Release();
	}

	//------------------------------------------------------------------------------
	//ScriptEventAgent
	//------------------------------------------------------------------------------
	void				ScriptEventAgent::AddBinder(ScriptEventBinder* pBinder)
	{
		m_arrBinder.push_back(pBinder);
	}

	ScriptEventBinder*	ScriptEventAgent::TakeBinder(const ScriptEventBinderKey& key)
	{
		ScriptEventBinder* pBinder = NULL;
		for (Uint nCnt = 0; nCnt < m_arrBinder.size(); ++nCnt)
		{
			if (m_arrBinder[nCnt]->GetKey() == key)
			{
				pBinder = m_arrBinder[nCnt];
				m_arrBinder.erase(m_arrBinder.begin() + nCnt);
				break;
			}
		}

		return pBinder;
	}
}
