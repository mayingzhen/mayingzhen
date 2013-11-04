#ifndef  _MatViewProj__H__
#define  _MatViewProj__H__


namespace ma
{
	class COMMON_API MatViewProj
	{

	public:
		MatViewProj()
		{
			MatrixIdentity(&m_matView);
			MatrixIdentity(&m_matProj);
			MatrixIdentity(&m_matViewProj);
			m_bViewProjDirty = true;
		}

		const Matrix4x4&	GetMatView() const
		{
			return m_matView;
		}

		void				SetMatView(const Matrix4x4& matView) 
		{
			m_matView = matView; 
			m_bViewProjDirty = true;
		}

		const Matrix4x4&	GetMatProj()  const
		{
			return m_matProj;
		}

		void				SetMatProj(const Matrix4x4& matProj) 
		{
			m_matProj = matProj; 
			m_bViewProjDirty = true;
		}

		const Matrix4x4&	GetMatViewProj() 
		{
			if (m_bViewProjDirty)
			{
				m_matViewProj = m_matView * m_matProj;
				m_bViewProjDirty = false;

			}

			return m_matViewProj;
		}

	private:
		Matrix4x4				m_matView;

		Matrix4x4				m_matProj;

		Matrix4x4				m_matViewProj;

		bool					m_bViewProjDirty;

	};
}

#endif