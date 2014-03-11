#ifndef  _ActionData__H__
#define  _ActionData__H__

namespace ma
{
// 	DeclareRefPtr(AnimNodeData)
// 	DeclareRefPtr(AnimClipNodeData)
// 	DeclareRefPtr(AnimLayerNodeData)
// 	DeclareRefPtr(AnimBlendNodData)
// 	DeclareRefPtr(ActionData)

	struct ANIMATION_API AnimNodeData : public Object
	{
		DECL_OBJECT(AnimNodeData)
		
	public:	

		virtual void	Serialize(Serializer& sl, const char* pszLable = "AnimNode" ) ;
	};

	struct ANIMATION_API AnimClipNodeData : public AnimNodeData
	{
		DECL_OBJECT(AnimClipNodeData)

	public:	

		std::string		m_sClipPath;

		std::string		m_sBoneSetName;

		virtual void	Serialize(Serializer& sl, const char* pszLable = "AnimClipNode" ) ;
	};

	struct ANIMATION_API AnimLayerNodeData : public AnimNodeData
	{
		DECL_OBJECT(AnimLayerNodeData)
		
	public:	

		~AnimLayerNodeData()
		{
			for (UINT i = 0; i < m_arrAnimNodeData.size(); ++i)
			{
				SAFE_DELETE(m_arrAnimNodeData[i]);
			}
			m_arrAnimNodeData.clear();
		}

		std::vector<AnimNodeData*>	m_arrAnimNodeData;

		virtual void Serialize(Serializer& sl, const char* pszLable = "AnimLayerNode" );
	};

	struct ANIMATION_API AnimBlendNodData : public AnimNodeData
	{
		DECL_OBJECT(AnimBlendNodData)

	public:

		AnimBlendNodData()
		{
			m_pSrcAnimNodeData = NULL;
			m_pDestAnimNodeData = NULL;
			m_fWeight = 0;
		}

		~AnimBlendNodData()
		{
			SAFE_DELETE(m_pSrcAnimNodeData);
			SAFE_DELETE(m_pDestAnimNodeData);
		}

		AnimNodeData*		m_pSrcAnimNodeData;
		
		AnimNodeData*		m_pDestAnimNodeData;

		float				m_fWeight;

		virtual void		Serialize(Serializer& sl, const char* pszLable = "AnimClipNode" ) ;
	};

	struct ANIMATION_API ActionData 
	{
		ActionData()
		{
			m_pAnimNodeData = NULL;
		}

		~ActionData()
		{
			SAFE_DELETE(m_pAnimNodeData);
		}

		std::string					m_sAnimName;

		std::vector<std::string>	m_arrPMName;

		AnimNodeData*				m_pAnimNodeData;

		virtual void				Serialize(Serializer& sl, const char* pszLable = "ActionData");
	};


}

#endif

