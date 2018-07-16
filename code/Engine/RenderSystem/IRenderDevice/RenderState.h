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

	class BlendState : public Object
	{
	public:
		BlendState();

		DECL_OBJECT(BlendState);

		static void	RegisterAttribute();

		virtual void	RT_StreamComplete() { ASSERT(false); }

	public:
		BLEND_MODE			m_eBlendMode;

		bool				m_bColorWrite;
        
        bool operator < (const BlendState& other) const
        {
#define CMPVAR(x) if (x != other.x) return x < other.x;
            CMPVAR(m_eBlendMode);
            CMPVAR(m_bColorWrite);
#undef CMPVAR
            
            return false;
        }
	};

	RefPtr<BlendState> CreateBlendState();
}

