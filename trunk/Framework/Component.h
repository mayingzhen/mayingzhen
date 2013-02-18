#ifndef  _COMPONENT__H__
#define  _COMPONENT__H__

namespace ma
{
	class FRAMEWORK_API Component : public Object
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

	protected:
		GameObject* m_pGameObject;
	};
}



#endif