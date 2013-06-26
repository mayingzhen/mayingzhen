#ifndef  _UnitSphere__H__
#define  _UnitSphere__H__

namespace ma
{
	class UnitSphere
	{
	public:
		void Init();

		void Render(Material* pMaterial);

	private:
		void CreateUnitSphere(int rec, std::vector<Uint16>& indBuff, std::vector<Vector3>& vertBuff );

		void SphereTess(Vector3& v0, Vector3& v1, Vector3& v2, 
			std::vector<Uint16>& indBuff, std::vector<Vector3>& vertBuff);

		void SphereTessR(Vector3& v0, Vector3& v1, Vector3& v2, int depth, 
			std::vector<Uint16>& indBuff, std::vector<Vector3>& vertBuff);

	private:

		Renderable*		m_pRenderable;
	};
}


#endif

