#ifndef  _SkinMeshRendererable__H__
#define  _SkinMeshRendererable__H__

#include "MeshRenderable.h"

namespace ma
{
	class SkinMeshRenderable : public MeshRenderable
	{
	public:
		virtual void					PreRender(Technique* pTech);

		void							SetSkinMatrix(const Matrix3x4* arrMatrixs, uint32_t nCount);

		const DualQuaternion*			GetSkinDQ();
		uint32_t							GetSkinDQCount() const;
		
		uint32_t							GetSkinMatrixCount() const;
		const Matrix3x4*				GetSkinMatrix();

	private:
		typedef std::vector<DualQuaternion>	VEC_DQ;
		typedef std::vector<Matrix3x4> VEC_MATRIX;
		VEC_DQ							m_arrSkinDQ[2];
		VEC_MATRIX						m_arrSkinMatrix[2];
	};

}

#endif

