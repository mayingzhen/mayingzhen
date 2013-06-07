namespace ma
{

	const static D3DVERTEXELEMENT9 gs_primtiveVBElem[] = 
	{
		{ 0, 0,     D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
		{ 0, 3 * 4, D3DDECLTYPE_FLOAT4,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,		0 },
		D3DDECL_END()
	};
	

	void RGBToColor(D3DXCOLOR* col,Uint32 dwARGB)
	{
		col->a = ((dwARGB>>24)&0xff)/255.0f;
		col->r = ((dwARGB>>16)&0xff)/255.0f;
		col->g = ((dwARGB>> 8)&0xff)/255.0f;
		col->b = ((dwARGB>> 0)&0xff)/255.0f;
	}


	LineRender::LineRender()
	{
		m_pLineShader = NULL;
		m_pPrimitiveVBDesc = NULL;
		m_LineBatchSize = 0;
		m_pDxDevice = NULL;
	}

	void LineRender::Init(LPDIRECT3DDEVICE9 pDxDevice)
	{
		m_pDxDevice = pDxDevice;

		DWORD dwShaderFlags = 0;
#ifdef _DEBUG
		dwShaderFlags |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif // _DEBUG

		const char* pszLineShader = "../Data/shader/ColorVertex.fx";
		HRESULT hr = D3DXCreateEffectFromFile( pDxDevice, 
			pszLineShader, NULL, NULL, dwShaderFlags, NULL, &m_pLineShader, NULL);

		m_pDxDevice->CreateVertexDeclaration(gs_primtiveVBElem,&m_pPrimitiveVBDesc);
	}


	void LineRender::DrawLine(const Vector3& p0,const Vector3& p1,Uint32 dwColor)
	{
		if (m_LineBatchSize < LINE_BATCH_MAX)
		{
			LineVertex& v0 = m_LineBatch[m_LineBatchSize * 2];
			LineVertex& v1 = m_LineBatch[m_LineBatchSize * 2 + 1];

			RGBToColor(&v0.col,dwColor);
			RGBToColor(&v1.col,dwColor);
			v0.pos = p0;
			v1.pos = p1;
			++m_LineBatchSize;
		}
	}

	void LineRender::FlushLine()
	{
		if (GetRenderDevice() == NULL || GetRenderDevice()->GetCamera() == NULL)
			return;

		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"FlushLine");


		Matrix4x4 matViewProj = GetRenderDevice()->GetCamera()->GetViewProjMatrix();

		m_pDxDevice->SetVertexDeclaration( m_pPrimitiveVBDesc );

		m_pLineShader->SetTechnique("LineShader");
		m_pLineShader->SetMatrix( "gmVP", (const D3DXMATRIX*)&matViewProj);
		
		m_pDxDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS);

		UINT cPasses = 0; 
		m_pLineShader->Begin(&cPasses, 0 );
		for (UINT i = 0; i < cPasses; ++i)
		{
			m_pLineShader->BeginPass(i);
			m_pLineShader->CommitChanges();
			m_pDxDevice->DrawPrimitiveUP(D3DPT_LINELIST,m_LineBatchSize,(void*)&m_LineBatch,sizeof(LineVertex));
			m_pLineShader->EndPass();
		}	
		m_pLineShader->End();

		m_pDxDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESS);

		m_LineBatchSize = 0;

		D3DPERF_EndEvent();
	}
	
}

