#pragma once


namespace ma
{
	class RasterizerState : public Serializable
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

	class DepthStencilState : public Serializable
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
		uint32				m_nStencilRefValue;
		uint32              m_nStencilMask;
		uint32				m_nStencilWriteMask;

		bool				m_bDepthWrite;
		CompareFunction		m_eDepthCheckMode;
	};

	RefPtr<DepthStencilState> CreateDepthStencilState();

	class BlendState : public Serializable
	{
	public:
		BlendState();

		DECL_OBJECT(BlendState);

		static void	RegisterAttribute();

		virtual void	RT_StreamComplete() { ASSERT(false); }

	public:
		BLEND_MODE			m_eBlendMode;

		bool				m_bColorWrite;
	};

	RefPtr<BlendState> CreateBlendState();
}

