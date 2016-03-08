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
			m_matViewProjInv = Matrix4::IDENTITY;
			m_bViewProjDirty = true;
		}

		const Matrix4&	GetMatView() const
		{
			return m_matView;
		}

		const Matrix4& GetMatViewInv() const
		{
			return m_matViewInv;
		}

		void				SetMatView(const Matrix4& matView) 
		{
			m_matView = matView; 
			m_matViewInv = matView.inverse();
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

		const Matrix4&	GetMatViewProj() const 
		{
			if (m_bViewProjDirty)
			{
				m_matViewProj = m_matProj * m_matView;
				m_bViewProjDirty = false;
			}

			return m_matViewProj;
		}

		const Matrix4&	GetMatViewProjInv()
		{
			if (m_bViewProjDirty)
			{
				m_matViewProj = m_matProj * m_matView;
				m_matViewProjInv = m_matViewProj.inverse();
				m_bViewProjDirty = false;
			}
			return m_matViewProjInv;
		}

	private:
		Matrix4				m_matView;
		mutable Matrix4		m_matViewInv;
		Matrix4				m_matProj;
		mutable Matrix4		m_matViewProj;
		mutable Matrix4		m_matViewProjInv;

		mutable bool		m_bViewProjDirty;

	};
}

#endif