#pragma once



namespace ma
{
	class RasterizerState : public Object
	{
	public:
		RasterizerState();

		DECL_OBJECT(RasterizerState);

		static void		RegisterAttribute();

		virtual void	RT_StreamComplete() { ASSERT(false); }

	public:
		float				m_fConstantBias = 0;
		float				m_fSlopeScaleBias = 0;

		bool				m_bScissorTest = false;
		FillMode			m_eFillMode = FILL_SOLID;
		CULL_MODE			m_eCullMode = CULL_FACE_SIDE_BACK;

		bool operator < (const RasterizerState& other) const
		{
#define CMPVAR(x) if (x != other.x) return x < other.x;
			CMPVAR(m_fConstantBias);
			CMPVAR(m_fSlopeScaleBias);
			CMPVAR(m_bScissorTest);
			CMPVAR(m_eFillMode);
			CMPVAR(m_eCullMode);
#undef CMPVAR

			return false;
		}

		bool operator == (const RasterizerState& other) const
		{
			if (*this < other)
				return false;
			else if (other < *this)
				return false;
			else
				return true;
		}

		bool operator != (const RasterizerState& other) const
		{
			return !(*this == other);
		}
	};

	RefPtr<RasterizerState> CreateRasterizerState();

	class DepthStencilState : public Object
	{
	public:
		DepthStencilState();

		DECL_OBJECT(DepthStencilState);

		static void		RegisterAttribute();

		virtual void	RT_StreamComplete() { ASSERT(false); };

	public:
		bool				m_bStencil = false;
		CompareFunction		m_eStencilfunc = CMPF_ALWAYS_PASS;
		StencilOperation	m_eStencilPass = SOP_KEEP;
		StencilOperation	m_eStencilFail = SOP_KEEP;
		StencilOperation	m_eDepthFailOp = SOP_KEEP;
		uint32_t            m_nStencilMask = -1;
		uint32_t			m_nStencilWriteMask = -1;
		uint32_t			m_nStencilRef = 0;

		bool				m_bBackFaceStencil = false;
		CompareFunction		m_eBackStencilfunc = CMPF_ALWAYS_PASS;
		StencilOperation	m_eBackStencilPass = SOP_KEEP;
		StencilOperation	m_eBackStencilFail = SOP_KEEP;
		StencilOperation	m_eBackDepthFailOp = SOP_KEEP;


		bool				m_bDepthWrite = true;
		CompareFunction		m_eDepthCheckMode = CMPF_LESS_EQUAL;
		bool				m_bDepthBoundsTest = false;
        
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
			CMPVAR(m_bDepthBoundsTest);
#undef CMPVAR
            
            return false;
        }

		bool operator == (const DepthStencilState& other) const
		{
			if (*this < other)
				return false;
			else if (other < *this)
				return false;
			else
				return true;
		}

		bool operator != (const DepthStencilState& other) const
		{
			return !(*this == other);
		}
	};

	RefPtr<DepthStencilState> CreateDepthStencilState();

	enum BLEND
	{
		BLEND_ZERO,
		BLEND_ONE,
		BLEND_SRC_COLOR,
		BLEND_INV_SRC_COLOR,
		BLEND_SRC_ALPHA,
		BLEND_INV_SRC_ALPHA,
		BLEND_DEST_ALPHA ,
		BLEND_INV_DEST_ALPHA ,
		BLEND_DEST_COLOR,
		BLEND_INV_DEST_COLOR,
		BLEND_SRC_ALPHA_SAT,
		BLEND_BLEND_FACTOR,
		BLEND_INV_BLEND_FACTOR,
		BLEND_SRC1_COLOR,
		BLEND_INV_SRC1_COLOR,
		BLEND_SRC1_ALPHA ,
		BLEND_INV_SRC1_ALPHA
	};

	static const char* strBLEND[] = 
	{
		"BLEND_ZERO",
		"BLEND_ONE",
		"BLEND_SRC_COLOR",
		"BLEND_INV_SRC_COLOR",
		"BLEND_SRC_ALPHA",
		"BLEND_INV_SRC_ALPHA",
		"BLEND_DEST_ALPHA",
		"BLEND_INV_DEST_ALPHA",
		"BLEND_DEST_COLOR",
		"BLEND_INV_DEST_COLOR",
		"BLEND_SRC_ALPHA_SAT",
		"BLEND_BLEND_FACTOR",
		"BLEND_INV_BLEND_FACTOR",
		"BLEND_SRC1_COLOR",
		"BLEND_INV_SRC1_COLOR",
		"BLEND_SRC1_ALPHA",
		"BLEND_INV_SRC1_ALPHA",
		0
	};
	//static_assert(sizeof(strBLEND) == BLEND_INV_SRC1_ALPHA + 1);

	enum BLEND_OP
	{
		BLEND_OP_ADD,
		BLEND_OP_SUBTRACT,
		BLEND_OP_REV_SUBTRACT,
		BLEND_OP_MIN,
		BLEND_OP_MAX
	};

	static const char* strBLEND_OP[] =
	{
		"BLEND_OP_ADD",
		"BLEND_OP_SUBTRACT",
		"BLEND_OP_REV_SUBTRACT",
		"BLEND_OP_MIN",
		"BLEND_OP_MAX",
		0
	};

	class RENDER_TARGET_BLEND_DESC : public Object
	{
	public:
		RENDER_TARGET_BLEND_DESC();

		DECL_OBJECT(RENDER_TARGET_BLEND_DESC);

		static void		RegisterAttribute();

		bool BlendEnable = false;
		BLEND SrcBlend = BLEND_SRC_ALPHA;
		BLEND DestBlend = BLEND_INV_SRC_ALPHA;
		BLEND_OP BlendOp = BLEND_OP_ADD;
		BLEND SrcBlendAlpha = BLEND_SRC_ALPHA;
		BLEND DestBlendAlpha = BLEND_INV_SRC_ALPHA;
		BLEND_OP BlendOpAlpha = BLEND_OP_ADD;
		uint8_t nColorWrite = 0xF;

		bool operator < (const RENDER_TARGET_BLEND_DESC& other) const
		{
			#define CMPVAR(x) if (x != other.x) return x < other.x;
			CMPVAR(BlendEnable);
			CMPVAR(SrcBlend);
			CMPVAR(DestBlend);
			CMPVAR(BlendOp);
			CMPVAR(SrcBlendAlpha);
			CMPVAR(DestBlendAlpha);
			CMPVAR(BlendOpAlpha);
			CMPVAR(nColorWrite);
			#undef CMPVAR

			return false;
		}

		bool operator == (const RENDER_TARGET_BLEND_DESC& other) const
		{
			if (*this < other)
				return false;
			else if (other < *this)
				return false;
			else
				return true;
		}

		bool operator != (const RENDER_TARGET_BLEND_DESC& other) const
		{
			return !(*this == other);
		}
	};



	class BlendState : public Object
	{
	public:
		BlendState();

		DECL_OBJECT(BlendState);

		static void		RegisterAttribute();

		virtual bool	Import(rapidxml::xml_node<>* pXmlObject);
		virtual bool	Export(rapidxml::xml_node<>* pXmlObject, rapidxml::xml_document<>& doc);

		virtual void	RT_StreamComplete() { ASSERT(false); }

	public:

		RENDER_TARGET_BLEND_DESC m_blendDesc[MAX_RENDERTARGETS];
 
        bool operator < (const BlendState& other) const
        {
			for (size_t i = 0; i < MAX_RENDERTARGETS; ++i)
			{
				if (m_blendDesc[i] != other.m_blendDesc[i])
					return m_blendDesc[i] < other.m_blendDesc[i];
			}

			return false;
        }

		bool operator == (const BlendState& other) const
		{
			if (*this < other)
				return false;
			else if (other < *this)
				return false;
			else
				return true;
		}

		bool operator != (const BlendState& other) const
		{
			return !(*this == other);
		}
	};

	RefPtr<BlendState> CreateBlendState();
}

