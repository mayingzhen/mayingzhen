#ifndef  _Object__H__
#define  _Object__H__

namespace ma
{
	class Object
	{
	public:
		Object(const Object* pParent = NULL,const char* pName = NULL);
		
		virtual ~Object();
	
		const char* GetName() {m_sName.c_str();}

		void SetName(const char* pName) {m_sName = pName;}

		void AddChild(const Object* pObject);

		UINT GetChildNumber();

		Object* GetChildByIndex(UINT uIndex);

		Object* GetParent() {return m_pParent;}

		virtual void Render();

		virtual void Update();

		virtual void ParalleUpdate(float fTimeElapsed);

		virtual void FixedUpdate(float fTimeElapsed);

		virtual void LateUpdate(float fTimeElapsed);

		virtual void Start();

		virtual void Stop();

	protected:
		std::string m_sName;
		std::vector<Object*> m_vChilds;
		Object* m_pParent;
	};
}


#endif