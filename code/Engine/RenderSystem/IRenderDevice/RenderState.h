#pragma once



namespace ma
{
	class RasterizerState : public Object
	{
	public:
		RasterizerState();

		DECL_OBJECT(RasterizerState);

		static void	RegisterAttribute();

		virtual void	RT_StreamComplete() { ASSERT(false); }

	public:
		float				m_fConstantBias;
		float				m_fSlopeScaleBias;

		bool				m_bScissorTest;
		FillMode			m_eFillMode;
		CULL_MODE			m_eCullMode;
	};

	RefPtr<RasterizerState> CreateRasterizerState();

	class DepthStencilState : public Object
	{
	public:
		DepthStencilState();

		DECL_OBJECT(DepthStencilState);

		static void	RegisterAttribute();

		virtual void	RT_StreamComplete() { ASSERT(false); };

	public:
		bool				m_bStencil;
		CompareFunction		m_eStencilfunc;
		StencilOperation	m_eStencilPass;
		StencilOperation	m_eStencilFail;
		StencilOperation	m_eDepthFailOp;
		uint32_t            m_nStencilMask;
		uint32_t			m_nStencilWriteMask;

		bool				m_bDepthWrite;
		CompareFunction		m_eDepthCheckMode;
        
        bool operator < (const DepthStencilState& other) const
        {
#define CMPVAR(x) if (x != other.x) return x < other.x;
            CMPVAR(m_bStencil);
            CMPVAR(m_eStencilfunc);
            CMPVAR(m_eStencilPass);
            CMPVAR(m_eDepthFailOp);
            CMPVAR(m_eDepthFailOp);
            CMPVAR(m_nStencilMask);
            CMPVAR(m_nStencilWriteMask);
            
            CMPVAR(m_bDepthWrite);
            CMPVAR(m_eDepthCheckMode);
#undef CMPVAR
            
            return false;
        }
	};

	RefPtr<DepthStencilState> CreateDepthStencilState();

	enum BLEND
	{
		BLEND_ZERO = 1,
		BLEND_ONE = 2,
		BLEND_SRC_COLOR = 3,
		BLEND_INV_SRC_COLOR = 4,
		BLEND_SRC_ALPHA = 5,
		BLEND_INV_SRC_ALPHA = 6,
		BLEND_DEST_ALPHA = 7,
		BLEND_INV_DEST_ALPHA = 8,
		BLEND_DEST_COLOR = 9,
		BLEND_INV_DEST_COLOR = 10,
		BLEND_SRC_ALPHA_SAT = 11,
		BLEND_BLEND_FACTOR = 14,
		BLEND_INV_BLEND_FACTOR = 15,
		BLEND_SRC1_COLOR = 16,
		BLEND_INV_SRC1_COLOR = 17,
		BLEND_SRC1_ALPHA = 18,
		BLEND_INV_SRC1_ALPHA = 19
	};

	enum BLEND_OP
	{
		BLEND_OP_ADD = 1,
		BLEND_OP_SUBTRACT = 2,
		BLEND_OP_REV_SUBTRACT = 3,
		BLEND_OP_MIN = 4,
		BLEND_OP_MAX = 5
	};

	struct RENDER_TARGET_BLEND_DESC
	{
		bool BlendEnable = false;
		BLEND SrcBlend = BLEND_SRC_ALPHA;
		BLEND DestBlend = BLEND_INV_SRC_ALPHA;
		BLEND_OP BlendOp = BLEND_OP_ADD;
		BLEND SrcBlendAlpha = BLEND_SRC_ALPHA;
		BLEND DestBlendAlpha = BLEND_INV_SRC_ALPHA;
		BLEND_OP BlendOpAlpha = BLEND_OP_ADD;
		uint8_t nColorWrite = 0xF;
	};



	class BlendState : public Object
	{
	public:
		BlendState();

		DECL_OBJECT(BlendState);

		virtual bool	Import(rapidxml::xml_node<>* pXmlObject);
		virtual bool	Export(rapidxml::xml_node<>* pXmlObject, rapidxml::xml_document<>& doc);

		virtual void	RT_StreamComplete() { ASSERT(false); }

	public:

		RENDER_TARGET_BLEND_DESC m_blendDesc[MAX_RENDERTARGETS];
 
        bool operator < (const BlendState& other) const
        {
			for (uint32_t i = 0; i < MAX_RENDERTARGETS; ++i)
			{
#define CMPVAR(x) if (x != other.x) return x < other.x;
				CMPVAR(m_blendDesc[i].BlendEnable);
				CMPVAR(m_blendDesc[i].SrcBlend);
				CMPVAR(m_blendDesc[i].DestBlend);
				CMPVAR(m_blendDesc[i].BlendOp);
				CMPVAR(m_blendDesc[i].SrcBlendAlpha);
				CMPVAR(m_blendDesc[i].DestBlendAlpha);
				CMPVAR(m_blendDesc[i].BlendOpAlpha);
				CMPVAR(m_blendDesc[i].nColorWrite);
#undef CMPVAR
			}

			return false;
        }
	};

	RefPtr<BlendState> CreateBlendState();
}

