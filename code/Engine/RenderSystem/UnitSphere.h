#ifndef  _UnitSphere__H__
#define  _UnitSphere__H__

namespace ma
{
	class UnitSphere
	{
	public:
		static void Init();

		static void Render(Material* pMaterial,const Vector3& vPos,float fRadius);

	};
}


#endif

