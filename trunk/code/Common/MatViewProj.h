#ifndef  _MatViewProj__H__
#define  _MatViewProj__H__

#include "Common/Math/Matrix4.h"

namespace ma
{
	class MatViewProj
	{

	public:
		MatViewProj()
		{
			m_matView = Matrix4::IDENTITY;
			m_matProj = Matrix4::IDENTITY;
			m_matViewProj = Matrix4::IDENTITY;
			m_bViewProjDirty = true;
		}

		const Matrix4&	GetMatView() const
		{
			return m_matView;
		}

		void				SetMatView(const Matrix4& matView) 
		{
			m_matView = matView; 
			m_bViewProjDirty = true;
		}

		const Matrix4&	GetMatProj()  const
		{
			return m_matProj;
		}

		void				SetMatProj(const Matrix4& matProj) 
		{
			m_matProj = matProj; 
			m_bViewProjDirty = true;
		}

		const Matrix4&	GetMatViewProj() 
		{
			if (m_bViewProjDirty)
			{
				m_matViewProj = m_matProj * m_matView;
				m_bViewProjDirty = false;
			}

			return m_matViewProj;
		}

	private:
		Matrix4				m_matView;

		Matrix4				m_matProj;

		Matrix4				m_matViewProj;

		bool				m_bViewProjDirty;

	};
}

#endif