#ifndef  _FbxImportParm__H__
#define  _FbxImportParm__H__

namespace ma
{
	enum NormalParm
	{
		Import,
		Calculate,
		None,
	};

	struct ImportParm
	{
		//float			m_fScaleFactor;
		NormalParm		m_eImportNormal;
		NormalParm		m_eImportTangents;

		ImportParm()
		{
			//m_fScaleFactor = 1.0f;
			m_eImportNormal = Import;
			m_eImportTangents = None;
		}
	};
}

#endif

