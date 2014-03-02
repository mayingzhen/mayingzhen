#ifndef __FBXImporterVertexH__
#define __FBXImporterVertexH__


namespace ma
{

	// VertexType
	struct V_3P_3N_2UV
	{
		Vector3  m_position;			// position
		Vector3	 m_normal;				// normal
		Vector2	 m_uv;					// Texture Coordinate

		bool operator == ( const V_3P_3N_2UV& other) const
		{
			if (m_position == other.m_position && m_uv == other.m_uv)
				return true;
			else
				return false;
		}
	};

	struct V_3P_3N_2UV_S
	{
		Vector3  m_position;			// position
		Vector3	 m_normal;				// normal
		Vector2	 m_uv;					// Texture Coordinate
		Vector3	 m_tan;
		UINT	 m_boneID;				// bones
		UINT	 m_weight;				// Weights	
		UINT	 m_iVC;
		UINT	 m_iVL;

		V_3P_3N_2UV_S()
		{
			m_iVL = -1;
			m_iVC = -1;
		}

		bool operator == ( const V_3P_3N_2UV_S& other) const
		{
			if (m_position == other.m_position && m_uv == other.m_uv)
				return true;
			else
				return false;
		}
	};
}


#endif
