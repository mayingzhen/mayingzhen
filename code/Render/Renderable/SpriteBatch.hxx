#include "SpriteBatch.h"

// Default size of a newly created sprite batch
#define SPRITE_BATCH_DEFAULT_SIZE 128

// Factor to grow a sprite batch by when its size is exceeded
#define SPRITE_BATCH_GROW_FACTOR 2.0f

// Macro for adding a sprite to the batch
#define SPRITE_ADD_VERTEX(vtx, vx, vy, vz, vu, vv, vr, vg, vb, va) \
    vtx.x = vx; vtx.y = vy; vtx.z = vz; \
    vtx.u = vu; vtx.v = vv; \
    vtx.r = vr; vtx.g = vg; vtx.b = vb; vtx.a = va


namespace ma
{

	SpriteBatch::SpriteBatch(Texture* texture, Technique* pTech, UINT initialCapacity/* = 0*/)
		: m_pMeshBatch(NULL), m_pSampler(NULL), m_fTextureWidthRatio(0.0f), m_fTextureHeightRatio(0.0f)
	{
		ASSERT(texture != NULL);

		m_fTextureWidthRatio = 1.0f / (float)texture->getWidth();
		m_fTextureHeightRatio = 1.0f / (float)texture->getHeight();
		
		m_pTechnique = pTech;
		if (pTech == NULL)
		{
			m_pTechnique = new Technique("Shading","default","default","DIFFUSE;COLOR");
		}
		
		m_pTechnique->GetRenderState().m_eBlendMode = BM_TRANSPARENT;

		m_pSampler = new SamplerState();
		m_pSampler->SetTexture(texture);

		m_pTechnique->GetParameter("u_texture")->setSampler(m_pSampler);

		VertexDeclaration* vertexFormat = GetRenderDevice()->CreateVertexDeclaration(); //(vertexElements, 3);
		vertexFormat->AddElement(0,0,DT_FLOAT3,DU_POSITION,0);
		vertexFormat->AddElement(0,12,DT_FLOAT2,DU_TEXCOORD0,0);
		vertexFormat->AddElement(0,20,DT_FLOAT4,DU_COLOR,0);

		m_pMeshBatch = new MeshBatch(vertexFormat, PRIM_TRIANGLESTRIP, /*pEffect,*/ true, initialCapacity > 0 ? initialCapacity : SPRITE_BATCH_DEFAULT_SIZE);


		// Bind an ortho projection to the material by default (user can override with setProjectionMatrix)
		Platform& platform = Platform::GetInstance();
		int w,h;
		platform.GetWindowSize(w, h);
		GetRenderDevice()->MakeOrthoMatrixOffCenter(&m_projectionMatrix, 0, (float)w, (float)h, 0, 0.0f, 1.0f);
		m_pTechnique->GetParameter("u_worldViewProjectionMatrix")->bindValue(this, &SpriteBatch::getProjectionMatrix);
	}

	SpriteBatch::~SpriteBatch()
	{
		SAFE_DELETE(m_pMeshBatch);
	}

	void SpriteBatch::start()
	{
		m_pMeshBatch->start();
	}

	void SpriteBatch::draw(const Rectangle& dst, const Rectangle& src, const Vector4& color)
	{
		// Calculate uvs.
		float u1 = m_fTextureWidthRatio * src.x;
		float v1 = 1.0f - m_fTextureHeightRatio * src.y;
		float u2 = u1 + m_fTextureWidthRatio * src.width;
		float v2 = v1 - m_fTextureHeightRatio * src.height;

		draw(dst.x, dst.y, dst.width, dst.height, u1, v1, u2, v2, color);
	}

	void SpriteBatch::draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Vector4& color)
	{
		// Calculate uvs.
		float u1 = m_fTextureWidthRatio * src.x;
		float v1 = 1.0f - m_fTextureHeightRatio * src.y;
		float u2 = u1 + m_fTextureWidthRatio * src.width;
		float v2 = v1 - m_fTextureHeightRatio * src.height;

		draw(dst.x, dst.y, dst.z, scale.x, scale.y, u1, v1, u2, v2, color);
	}

	void SpriteBatch::draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Vector4& color,
						   const Vector2& rotationPoint, float rotationAngle)
	{
		// Calculate uvs.
		float u1 = m_fTextureWidthRatio * src.x;
		float v1 = 1.0f - m_fTextureHeightRatio * src.y;
		float u2 = u1 + m_fTextureWidthRatio * src.width;
		float v2 = v1 - m_fTextureHeightRatio * src.height;

		draw(dst, scale.x, scale.y, u1, v1, u2, v2, color, rotationPoint, rotationAngle);
	}

	void SpriteBatch::draw(const Vector3& dst, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color,
						   const Vector2& rotationPoint, float rotationAngle, bool positionIsCenter)
	{
		draw(dst.x, dst.y, dst.z, width, height, u1, v1, u2, v2, color, rotationPoint, rotationAngle, positionIsCenter);
	}

	void SpriteBatch::draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color,
			  const Vector2& rotationPoint, float rotationAngle, bool positionIsCenter)
	{
		// Treat the given position as the center if the user specified it as such.
		if (positionIsCenter)
		{
			x -= 0.5f * width;
			y -= 0.5f * height;
		}

		// Expand the destination position by scale into 4 points.
		float x2 = x + width;
		float y2 = y + height;
	    
		Vector2 upLeft(x, y);
		Vector2 upRight(x2, y);
		Vector2 downLeft(x, y2);
		Vector2 downRight(x2, y2);

		// Rotate points around rotationAxis by rotationAngle.
		Vector2 pivotPoint(rotationPoint);
		pivotPoint.x *= width;
		pivotPoint.y *= height;
		pivotPoint.x += x;
		pivotPoint.y += y;
		upLeft.rotate(pivotPoint, rotationAngle);
		upRight.rotate(pivotPoint, rotationAngle);
		downLeft.rotate(pivotPoint, rotationAngle);
		downRight.rotate(pivotPoint, rotationAngle);

		// Write sprite vertex data.
		static SpriteVertex v[4];
		SPRITE_ADD_VERTEX(v[0], downLeft.x, downLeft.y, z, u1, v1, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(v[1], upLeft.x, upLeft.y, z, u1, v2, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(v[2], downRight.x, downRight.y, z, u2, v1, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(v[3], upRight.x, upRight.y, z, u2, v2, color.x, color.y, color.z, color.w);
	    
		//static unsigned short indices[4] = { 0, 1, 2, 3 };
		static const unsigned short indices[4] = { 0, 2, 1, 3 };

		m_pMeshBatch->add(v, 4, indices, 4);
	}

	void SpriteBatch::draw(const Vector3& position, const Vector3& right, const Vector3& forward, float width, float height,
		float u1, float v1, float u2, float v2, const Vector4& color, const Vector2& rotationPoint, float rotationAngle)
	{
		// Calculate the vertex positions.
		Vector3 tRight(right);
		tRight *= width * 0.5f;
		Vector3 tForward(forward);
		tForward *= height * 0.5f;
	    
		Vector3 p0 = position;
		p0 -= tRight;
		p0 -= tForward;

		Vector3 p1 = position;
		p1 += tRight;
		p1 -= tForward;

		tForward = forward;
		tForward *= height;
		Vector3 p2 = p0;
		p2 += tForward;
		Vector3 p3 = p1;
		p3 += tForward;

		// Calculate the rotation point.
		Vector3 rp = p0;
		tRight = right;
		tRight *= width * rotationPoint.x;
		tForward *= rotationPoint.y;
		rp += tRight;
		rp += tForward;

		// Rotate all points the specified amount about the given point (about the up vector).
		static Vector3 u;
		//Vector3::cross(right, forward, &u);
		Vec3Cross(&u, &right, &forward);
		static Matrix4x4 rotation;
		//Matrix4x4::createRotation(u, rotationAngle, &rotation);
		MatrixRotationAxis(&rotation, &u, rotationAngle);

		p0 -= rp;
		//p0 *= rotation;
		Vec3TransformNormal(&p0,&p0,&rotation);
		p0 += rp;

		p1 -= rp;
		//p1 *= rotation;
		Vec3TransformNormal(&p1,&p1,&rotation);
		p1 += rp;

		p2 -= rp;
		//p2 *= rotation;
		Vec3TransformNormal(&p2,&p2,&rotation);
		p2 += rp;

		p3 -= rp;
		//p3 *= rotation;
		Vec3TransformNormal(&p3,&p3,&rotation);
		p3 += rp;

		// Add the sprite vertex data to the batch.
		static SpriteVertex v[4];
		SPRITE_ADD_VERTEX(v[0], p0.x, p0.y, p0.z, u1, v1, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(v[1], p1.x, p1.y, p1.z, u2, v1, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(v[2], p2.x, p2.y, p2.z, u1, v2, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(v[3], p3.x, p3.y, p3.z, u2, v2, color.x, color.y, color.z, color.w);

		// 2 ------- 3
		//   |	   |
		//   |	   |
	    // 0 ------- 1
		static const unsigned short indices[4] = { 0, 2, 1, 3 };
		m_pMeshBatch->add(v, 4, const_cast<unsigned short*>(indices), 4);
	}

	void SpriteBatch::draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color)
	{
		draw(x, y, 0, width, height, u1, v1, u2, v2, color);
	}

	void SpriteBatch::draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, const Rectangle& clip)
	{
		// Only draw if at least part of the sprite is within the clip region.
		if (clipSprite(clip, x, y, width, height, u1, v1, u2, v2))
			draw(x, y, 0, width, height, u1, v1, u2, v2, color);
	}

	void SpriteBatch::addSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, SpriteBatch::SpriteVertex* vertices)
	{
		ASSERT(vertices);

		const float x2 = x + width;
		const float y2 = y + height;
		SPRITE_ADD_VERTEX(vertices[0], x, y, 0, u1, v1, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(vertices[1], x, y2, 0, u1, v2, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(vertices[2], x2, y, 0, u2, v1, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(vertices[3], x2, y2, 0, u2, v2, color.x, color.y, color.z, color.w);
	}

	void SpriteBatch::addSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, const Rectangle& clip, SpriteBatch::SpriteVertex* vertices)
	{
		ASSERT(vertices);

		// Only add a sprite if at least part of the sprite is within the clip region.
		if (clipSprite(clip, x, y, width, height, u1, v1, u2, v2))
		{
			const float x2 = x + width;
			const float y2 = y + height;
			SPRITE_ADD_VERTEX(vertices[0], x, y, 0, u1, v1, color.x, color.y, color.z, color.w);
			SPRITE_ADD_VERTEX(vertices[1], x, y2, 0, u1, v2, color.x, color.y, color.z, color.w);
			SPRITE_ADD_VERTEX(vertices[2], x2, y, 0, u2, v1, color.x, color.y, color.z, color.w);
			SPRITE_ADD_VERTEX(vertices[3], x2, y2, 0, u2, v2, color.x, color.y, color.z, color.w);
		}
	}

	void SpriteBatch::draw(SpriteBatch::SpriteVertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount)
	{
		ASSERT(vertices);
		ASSERT(indices);

		m_pMeshBatch->add(vertices, vertexCount, indices, indexCount);
	}

	void SpriteBatch::draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, bool positionIsCenter)
	{
		// Treat the given position as the center if the user specified it as such.
		if (positionIsCenter)
		{
			x -= 0.5f * width;
			y -= 0.5f * height;
		}

		// Write sprite vertex data.
		const float x2 = x + width;
		const float y2 = y + height;
		static SpriteVertex v[4];
		SPRITE_ADD_VERTEX(v[0], x, y, z, u1, v1, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(v[1], x, y2, z, u1, v2, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(v[2], x2, y, z, u2, v1, color.x, color.y, color.z, color.w);
		SPRITE_ADD_VERTEX(v[3], x2, y2, z, u2, v2, color.x, color.y, color.z, color.w);

		// 0 ------- 2
		//   |	   |
		//   |	   |
		// 1 ------- 3

		//static unsigned short indices[4] = { 0, 1, 2, 3 };
		static const unsigned short indices[4] = { 0, 2, 1, 3 };

		m_pMeshBatch->add(v, 4, indices, 4);
	}

	void SpriteBatch::finish(Technique* pTech)
	{
		if (pTech == NULL)
		{
			pTech = m_pTechnique;
		}

		m_pMeshBatch->finish(pTech);
	}

	RenderState& SpriteBatch::getStateBlock() const
	{
		//ASSERT(m_pMeshBatch);
		//ASSERT(m_pMeshBatch->m_pMaterial->GetEffect());
		//ASSERT(m_pMeshBatch->m_pMaterial->GetEffect()->GetCurTechnqiue());
		//if (m_pMeshBatch || m_pMeshBatch->getMaterial() || m_pMeshBatch->getMaterial()->GetCurTechnqiue())
		//	return 

		//return m_pMeshBatch->m_pMaterial->GetEffect()->GetCurTechnqiue()->GetRenderState();
		return m_pTechnique->GetRenderState();
	}

	SamplerState* SpriteBatch::getSampler() const
	{
		return m_pSampler;
	}

// 	Material* SpriteBatch::getMaterial() const
// 	{
// 		return m_pMeshBatch->m_pMaterial;
// 	}

	void SpriteBatch::setProjectionMatrix(const Matrix4x4& matrix)
	{
		m_projectionMatrix = matrix;
	}

	const Matrix4x4& SpriteBatch::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

	bool SpriteBatch::clipSprite(const Rectangle& clip, float& x, float& y, float& width, float& height, float& u1, float& v1, float& u2, float& v2)
	{
		// Clip the rectangle given by { x, y, width, height } into clip.
		// We need to scale the uvs accordingly as we do this.

		// First check to see if we need to draw at all.
		if (x + width < clip.x || x > clip.x + clip.width ||
			y + height < clip.y || y > clip.y + clip.height)
		{
			return false;
		}

		const float uvWidth = u2 - u1;
		const float uvHeight = v2 - v1;

		// Moving x to the right.
		if (x < clip.x)
		{
			const float percent = (clip.x - x) / width;
			const float dx = clip.x - x;
			x = clip.x;
			width -= dx;
			u1 += uvWidth * percent;
		}

		// Moving y down.
		if (y < clip.y)
		{
			const float percent = (clip.y - y) / height;
			const float dy = clip.y - y;
			y = clip.y;
			height -= dy;
			v1 += uvHeight * percent;
		}

		// Moving width to the left.
		const float clipX2 = clip.x + clip.width;
		float x2 = x + width;
		if (x2 > clipX2)
		{
			const float percent = (x2 - clipX2) / width;
			width = clipX2 - x;
			u2 -= uvWidth * percent;
		}

		// Moving height up.
		const float clipY2 = clip.y + clip.height;
		float y2 = y + height;
		if (y2 > clipY2)
		{
			const float percent = (y2 - clipY2) / height;
			height = clipY2 - y;
			v2 -= uvHeight * percent;
		}

		return true;
	}

}
