#ifndef  _Object__H__
#define  _Object__H__

namespace ma
{
	class Object
	{
	public:
		Object(const char* pName);
		
		virtual ~Object();
	
		const char* GetName() {m_sName.c_str();}

		void SetName(const char* pName) {m_sName = pName;}

		void AddChild(const Object* pObject);

		UINT GetChildNumber();

		Object* GetChildByIndex(UINT uIndex);

		virtual void Render();

		virtual void Update();

		virtual void ParalleUpdate(float fTimeElapsed);

		virtual void FixedUpdate(float fTimeElapsed);

		virtual void LateUpdate(float fTimeElapsed);

		virtual void Start();

		virtual void Stop();

	private:
		std::string m_sName;
		std::vector<Object*> m_vChilds;
	};
}


#endif