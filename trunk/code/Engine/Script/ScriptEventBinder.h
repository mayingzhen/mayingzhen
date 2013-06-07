#ifndef __ScriptEventBinder_h__
#define __ScriptEventBinder_h__

namespace ma
{
	typedef void* ScriptObjectHandle;
	struct ScriptSignalKey;
	struct ScriptEventBinderKey;
	class ScriptEventBinder;
	class ScriptEventFactory;
	struct ScriptEventAgent;

	struct ENGINE_API ScriptSignalKey
	{
		std::string m_className;
		std::string m_signal;

		bool operator<(const ScriptSignalKey& rhs) const;
	};

	struct ENGINE_API ScriptEventBinderKey
	{
		int		m_sender;
		std::string m_signal;
		ScriptObjectHandle m_reciver;
		std::string m_slot;

		bool operator<(const ScriptEventBinderKey& rhs) const;

		bool operator==(const ScriptEventBinderKey& rhs) const;

	};

	class ENGINE_API ScriptEventBinder
	{
	protected:
		ScriptEventFactory*			m_pFactory;
		ScriptEventBinderKey		m_key;

	public:
		ScriptEventBinder(ScriptEventFactory* pFactory);

		virtual void						Release();

		virtual const ScriptEventBinderKey& GetKey() const;

		virtual bool BindScriptEvent(GameObject* pSender,const char* signal,ScriptObjectHandle hReciver,const char* slot);

		virtual void UnBindScriptEvent() = 0;

	protected:

		template<class T>
		T* GetSender()
		{
			return static_cast<T*>(m_key.m_sender);
		}

		virtual bool BindScriptEventImpl(GameObject* pSender,const char* signal,ScriptObjectHandle hReciver,const char* slot) = 0;
	};

	class ENGINE_API ScriptEventFactory
	{
	public:

		virtual ScriptEventBinder*	CreateScriptEventBinder(ScriptSignalKey key) = 0;

		virtual void				ReleaseScriptEventBinder(ScriptEventBinder* pBinder) = 0;
	};

	typedef ScriptEventBinder* (*FunCreateScriptEventBinder)(ScriptEventFactory* pFactory);

	class ENGINE_API DefaultScriptEventFactory : public ScriptEventFactory
	{
		typedef std::map<ScriptSignalKey,FunCreateScriptEventBinder> BinderCreateMap;

		BinderCreateMap m_createMap;

	public:

		virtual void RegisterScriptEvent(ScriptSignalKey signiture,FunCreateScriptEventBinder fCreate);

		virtual void UnregisterScriptEvent(ScriptSignalKey signiture,FunCreateScriptEventBinder fCreate);

		ScriptEventBinder*	CreateScriptEventBinder(ScriptSignalKey key);

		void				ReleaseScriptEventBinder(ScriptEventBinder* pBinder);
	};

	struct ENGINE_API ScriptEventAgent
	{
		typedef std::vector<ScriptEventBinder*> VecBinder;
		VecBinder m_arrBinder;

		void				AddBinder(ScriptEventBinder* pBinder);

		ScriptEventBinder*	TakeBinder(const ScriptEventBinderKey& key);
	};

	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------

	inline bool ScriptSignalKey::operator<(const ScriptSignalKey& rhs) const
	{
		return m_className < rhs.m_className ? true :
			m_className > rhs.m_className ? false :
			m_signal < rhs.m_signal;
	}

	inline bool ScriptEventBinderKey::operator<(const ScriptEventBinderKey& rhs) const
	{
		return	m_sender < rhs.m_sender ? true :
				m_sender > rhs.m_sender ? false :
			m_signal < rhs.m_signal ? true :
			m_signal > rhs.m_signal ? false :
			m_reciver < rhs.m_reciver ? true :
			m_reciver > rhs.m_reciver ? false :
			m_slot < rhs.m_slot;
	}

	inline bool ScriptEventBinderKey::operator==(const ScriptEventBinderKey& rhs) const
	{
		return m_sender == rhs.m_sender && m_signal == rhs.m_signal && m_reciver == rhs.m_reciver && m_slot == rhs.m_slot;
	}

}

#endif //__ScriptEventBinder_h__
