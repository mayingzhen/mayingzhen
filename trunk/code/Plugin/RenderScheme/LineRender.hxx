namespace ma
{
	struct LineVertex
	{
	 	Vector3 pos;
	 	Color   col;
 	};

	LineRender::LineRender()
	{
		m_pMeshBatch = NULL;
	}

	void LineRender::Init()
	{
		ShaderProgram* pShaderProram = GetRenderDevice()->CreateShaderProgram();
		pShaderProram->CreateFromShaderName("default","COLOR");
		
		Material* pMaterial = Material::create(pShaderProram);
		//pMaterial->GetParameter("u_worldViewProjectionMatrix")->bindValue(this, &LineRender::GetViewProjMatrix);
		
		VertexDeclaration* pVertexDec = GetRenderDevice()->CreateVertexDeclaration(); //(vertexElements, 3);
		pVertexDec->AddElement(0,0,DT_FLOAT3,DU_POSITION,0);
		pVertexDec->AddElement(0,12,DT_FLOAT4,DU_COLOR,0);
		pVertexDec->Active();
		m_pMeshBatch = MeshBatch::create(pVertexDec, PRIM_LINELIST, pMaterial, false, 1024);
	}


	void LineRender::DrawLine(const Vector3& p0,const Vector3& p1,Color dwColor)
	{
		LineVertex v[2];
		v[0].pos = p0;
		v[0].col = dwColor;
		v[1].pos = p1;
		v[2].col = dwColor;
		
		m_pMeshBatch->add(v,2,NULL,0);
	}


	void LineRender::DrawBox(const Matrix4x4& wordMat,const Vector3& size, Color dwcolor)
	{
		Vector3 vScale;
		Quaternion qRot;
		Vector3 vPos;
		MatrixDecompose(&vScale,&qRot,&vPos,&wordMat);

		float fHalfWidth = 0.5f * vScale.x * size.x;
		float fHalfHeight = 0.5f * vScale.y * size.y;
		float fHalfDepth = 0.5f * vScale.z * size.z;

		Vector3 arrVec[]=
		{
			Vector3(-fHalfWidth,-fHalfHeight,fHalfDepth),
			Vector3(fHalfWidth,-fHalfHeight,fHalfDepth),
			Vector3(fHalfWidth,fHalfHeight,fHalfDepth),
			Vector3(-fHalfWidth,fHalfHeight,fHalfDepth),
			Vector3(-fHalfWidth,-fHalfHeight,-fHalfDepth),
			Vector3(fHalfWidth,-fHalfHeight,-fHalfDepth),
			Vector3(fHalfWidth,fHalfHeight,-fHalfDepth),
			Vector3(-fHalfWidth,fHalfHeight,-fHalfDepth),
		};

		for (UINT nCnt = 0; nCnt < 8; ++nCnt)
		{
			QuaternionTransformVector(&arrVec[nCnt],&arrVec[nCnt],&qRot);
			arrVec[nCnt] += vPos;
		}

		for (UINT nCnt = 0; nCnt < 4; ++nCnt)
		{
			UINT nID2 = (nCnt + 1) % 4;
			DrawLine(arrVec[nCnt],arrVec[nID2],dwcolor);
			DrawLine(arrVec[nCnt+4],arrVec[nID2+4],dwcolor);
			DrawLine(arrVec[nCnt],arrVec[nCnt+4],dwcolor);
		}
	}

	void LineRender::DrawCircle(UINT nbSegments, const Matrix4x4& world, 
		Color dwColor, float radius, bool semicircle)
	{
		static const float NxTwoPiF32	= 6.28318530717958647692f;

		float step = NxTwoPiF32/float(nbSegments);
		float segs = float(nbSegments);
		if(semicircle)
		{
			segs /= 2;
		}

		for(int i = 0; i < segs; i++)
		{
			int j = i+1;
			if(j == nbSegments)	
				j=0;

			float angle0 = float(i) * step;
			float angle1 = float(j) * step;

			Vector3 p0,p1;
			Vector3 pTemp0(radius * sinf(angle0), radius * cosf(angle0), 0.0f);
			Vector3 pTemp1(radius * sinf(angle1), radius * cosf(angle1), 0.0f);
			Vec3TransformCoord(&p0, &pTemp0, &world);
			Vec3TransformCoord(&p1, &pTemp1, &world);

			DrawLine(p0, p1, dwColor);
		}
	}

	void LineRender::DrawWireSphere(const Matrix4x4& world,float fRadius, Color dwColor)
	{
		Vector3 c0 = *(Vector3*)&world._11;
		Vector3 c1 = *(Vector3*)&world._21;
		Vector3 c2 = *(Vector3*)&world._31;	

		DrawCircle(20, world, dwColor, fRadius);

		Matrix4x4 matrTemp = world;
		matrTemp._11 = c1.x; matrTemp._12 = c1.y; matrTemp._13 = c1.z;
		matrTemp._21 = c2.x; matrTemp._22 = c2.y; matrTemp._23 = c2.z;
		matrTemp._31 = c0.x; matrTemp._32 = c0.y; matrTemp._33 = c0.z;
		DrawCircle(20, matrTemp, dwColor, fRadius);

		matrTemp._11 = c2.x; matrTemp._12 = c2.y; matrTemp._13 = c2.z;
		matrTemp._21 = c0.x; matrTemp._22 = c0.y; matrTemp._23 = c0.z;
		matrTemp._31 = c1.x; matrTemp._32 = c1.y; matrTemp._33 = c1.z;
		DrawCircle(20, matrTemp, dwColor, fRadius);
	}


	void LineRender::Start()
	{
		m_pMeshBatch->start();
	}

	void LineRender::Finish()
	{
		m_pMeshBatch->finish();

		m_pMeshBatch->draw();
	}
	
}

