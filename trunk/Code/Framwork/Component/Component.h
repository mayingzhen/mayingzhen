#ifndef  _Component__H__
#define  _Component__H__

namespace ma
{
	class GameObject;

	class FRAMWORK_API Component : public Object
	{
		DECL_OBJECT(Component)

	public:
		Component();

		virtual ~Component();

		virtual void Render();

		virtual void Update();

		virtual void ParalleUpdate(float fTimeElapsed);

		virtual void FixedUpdate(float fTimeElapsed);

		virtual void LateUpdate(float fTimeElapsed);

		virtual void Start();

		virtual void Stop();

		GameObject* GetGameObject() {return m_pGameObject;}

		virtual void SetGameObject(GameObject* pGameObject) {m_pGameObject = pGameObject;}

		virtual void Serialize(SerializeListener& sl, const char* pszLable = "Component");

	protected:
		GameObject* m_pGameObject;
	};
}



#endif