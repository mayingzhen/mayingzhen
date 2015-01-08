#include "SpriteBatch.h"
#include "Engine/Material/Technqiue.h"
#include "Engine/Material/RenderState.h"
#include "Engine/Material/SamplerState.h"

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
// 	SpriteBatch::SpriteBatch()
// 		::MeshBatch(NULL,PRIM_TRIANGLESTRIP, true, SPRITE_BATCH_DEFAULT_SIZE)
// 	{
// 
// 	}

	SpriteBatch::SpriteBatch(RefPtr<Texture> texture, Technique* pTech, UINT initialCapacity/* = 0*/)
		:MeshBatch(NULL,PRIM_TRIANGLESTRIP, true, initialCapacity > 0 ? initialCapacity : SPRITE_BATCH_DEFAULT_SIZE)
	{
		m_pSampler = NULL;
		
		m_pMaterial = CreateSubMaterial();

		m_pTechnique = pTech;
		if (pTech == NULL)
		{
			m_pTechnique= m_pMaterial->AddTechnique("Shading","default","DIFFUSE;COLOR");
		}
		m_pMaterial->AddTechnique(m_pTechnique.get());

		m_pTechnique->GetRenderState().m_eBlendMode = BM_TRANSPARENT;

		if (texture != NULL)
		{
			SetTexture(texture);
		}

		VertexElement element[3];
		element[0] = VertexElement(0,0,DT_FLOAT3,DU_POSITION,0);
		element[1] = VertexElement(0,12,DT_FLOAT2,DU_TEXCOORD,0);
		element[2] = VertexElement(0,20,DT_FLOAT4,DU_COLOR,0);
		m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,3);
		
		Resize(m_nCapacity);

		// Bind an ortho projection to the material by default (user can override with setProjectionMatrix)
		Rectangle rect = GetRenderSystem()->GetViewPort();
		GetRenderDevice()->MakeOrthoMatrixOffCenter(m_projectionMatrix, 0, rect.width, rect.height, 0, 0.0f, 1.0f);
		m_pTechnique->GetParameter("u_worldViewProjectionMatrix")->BindMethod(this, &SpriteBatch::GetProjectionMatrix);
	}

	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::SetTexture(TexturePtr pTexture)
	{
		ASSERT(pTexture != NULL);

		m_fTextureWidthRatio = 1.0f / (float)pTexture->GetWidth();
		m_fTextureHeightRatio = 1.0f / (float)pTexture->GetHeight();

		m_pTechnique->GetRenderState().m_eBlendMode = BM_TRANSPARENT;

		if (m_pSampler == NULL)
			m_pSampler = new SamplerState();
		m_pSampler->SetTexture(pTexture);

		m_pTechnique->SetParameter("u_texture",Any(m_pSampler));
	}

	void SpriteBatch::Render(Technique* pTech)
	{
		if (pTech == NULL)
			MeshBatch::Render(m_pTechnique.get());
		else
			MeshBatch::Render(pTech);
	}

	void SpriteBatch::Draw(const Rectangle& dst, const Rectangle& src, const ColourValue& color)
	{
		// Calculate uvs.
		float u1 = m_fTextureWidthRatio * src.x;
		float v1 = 1.0f - m_fTextureHeightRatio * src.y;
		float u2 = u1 + m_fTextureWidthRatio * src.width;
		float v2 = v1 - m_fTextureHeightRatio * src.height;

		Draw(dst.x, dst.y, dst.width, dst.height, u1, v1, u2, v2, color);
	}

	void SpriteBatch::Draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const ColourValue& color)
	{
		// Calculate uvs.
		float u1 = m_fTextureWidthRatio * src.x;
		float v1 = 1.0f - m_fTextureHeightRatio * src.y;
		float u2 = u1 + m_fTextureWidthRatio * src.width;
		float v2 = v1 - m_fTextureHeightRatio * src.height;

		Draw(dst.x, dst.y, dst.z, scale.x, scale.y, u1, v1, u2, v2, color);
	}

	void SpriteBatch::Draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const ColourValue& color,
						   const Vector2& rotationPoint, float rotationAngle)
	{
		// Calculate uvs.
		float u1 = m_fTextureWidthRatio * src.x;
		float v1 = 1.0f - m_fTextureHeightRatio * src.y;
		float u2 = u1 + m_fTextureWidthRatio * src.width;
		float v2 = v1 - m_fTextureHeightRatio * src.height;

		Draw(dst, scale.x, scale.y, u1, v1, u2, v2, color, rotationPoint, rotationAngle);
	}

	void SpriteBatch::Draw(const Vector3& dst, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color,
						   const Vector2& rotationPoint, float rotationAngle, bool positionIsCenter)
	{
		Draw(dst.x, dst.y, dst.z, width, height, u1, v1, u2, v2, color, rotationPoint, rotationAngle, positionIsCenter);
	}

	void SpriteBatch::Draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color,
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
		upLeft.rotate(Radian(rotationAngle), pivotPoint);
		upRight.rotate(Radian(rotationAngle), pivotPoint);
		downLeft.rotate(Radian(rotationAngle), pivotPoint);
		downRight.rotate(Radian(rotationAngle), pivotPoint);

		// Write sprite vertex data.
		static SpriteVertex v[4];
		SPRITE_ADD_VERTEX(v[0], downLeft.x, downLeft.y, z, u1, v1, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(v[1], upLeft.x, upLeft.y, z, u1, v2, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(v[2], downRight.x, downRight.y, z, u2, v1, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(v[3], upRight.x, upRight.y, z, u2, v2, color.r, color.g, color.b, color.a);
	    
		//static unsigned short indices[4] = { 0, 1, 2, 3 };
		static const unsigned short indices[4] = { 0, 2, 1, 3 };

		Add(v, 4, indices, 4);
	}

	void SpriteBatch::Draw(const Vector3& position, const Vector3& right, const Vector3& forward, float width, float height,
		float u1, float v1, float u2, float v2, const ColourValue& color, const Vector2& rotationPoint, float rotationAngle)
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
		//Vec3Cross(&u, &right, &forward);
		u = right.crossProduct(forward);
		static Matrix3 rotation;
		//Matrix4::createRotation(u, rotationAngle, &rotation);
		//MatrixRotationAxis(&rotation, &u, rotationAngle);
		rotation.FromAxisAngle(u,Radian(rotationAngle));

		p0 -= rp;
		p0 = rotation * p0;
		//Vec3TransformNormal(&p0,&p0,&rotation);
		p0 += rp;

		p1 -= rp;
		p1 = rotation * p1;
		//Vec3TransformNormal(&p1,&p1,&rotation);
		p1 += rp;

		p2 -= rp;
		p2 = rotation * p2;
		//Vec3TransformNormal(&p2,&p2,&rotation);
		p2 += rp;

		p3 -= rp;
		p3 = rotation * p3;
		//Vec3TransformNormal(&p3,&p3,&rotation);
		p3 += rp;

		// Add the sprite vertex data to the batch.
		static SpriteVertex v[4];
		SPRITE_ADD_VERTEX(v[0], p0.x, p0.y, p0.z, u1, v1, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(v[1], p1.x, p1.y, p1.z, u2, v1, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(v[2], p2.x, p2.y, p2.z, u1, v2, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(v[3], p3.x, p3.y, p3.z, u2, v2, color.r, color.g, color.b, color.a);

		// 2 ------- 3
		//   |	   |
		//   |	   |
	    // 0 ------- 1
		static const unsigned short indices[4] = { 0, 2, 1, 3 };
		Add(v, 4, const_cast<unsigned short*>(indices), 4);
	}

	void SpriteBatch::Draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color)
	{
		Draw(x, y, 0, width, height, u1, v1, u2, v2, color);
	}

	void SpriteBatch::Draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color, const Rectangle& clip)
	{
		// Only draw if at least part of the sprite is within the clip region.
		if (ClipSprite(clip, x, y, width, height, u1, v1, u2, v2))
			Draw(x, y, 0, width, height, u1, v1, u2, v2, color);
	}

	void SpriteBatch::AddSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color, SpriteBatch::SpriteVertex* vertices)
	{
		ASSERT(vertices);

		const float x2 = x + width;
		const float y2 = y + height;
		SPRITE_ADD_VERTEX(vertices[0], x, y, 0, u1, v1, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(vertices[1], x, y2, 0, u1, v2, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(vertices[2], x2, y, 0, u2, v1, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(vertices[3], x2, y2, 0, u2, v2, color.r, color.g, color.b, color.a);
	}

	void SpriteBatch::AddSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color, const Rectangle& clip, SpriteBatch::SpriteVertex* vertices)
	{
		ASSERT(vertices);

		// Only add a sprite if at least part of the sprite is within the clip region.
		if (ClipSprite(clip, x, y, width, height, u1, v1, u2, v2))
		{
			const float x2 = x + width;
			const float y2 = y + height;
			SPRITE_ADD_VERTEX(vertices[0], x, y, 0, u1, v1, color.r, color.g, color.b, color.a);
			SPRITE_ADD_VERTEX(vertices[1], x, y2, 0, u1, v2, color.r, color.g, color.b, color.a);
			SPRITE_ADD_VERTEX(vertices[2], x2, y, 0, u2, v1, color.r, color.g, color.b, color.a);
			SPRITE_ADD_VERTEX(vertices[3], x2, y2, 0, u2, v2, color.r, color.g, color.b, color.a);
		}
	}

	void SpriteBatch::Draw(SpriteBatch::SpriteVertex* vertices, UINT vertexCount, unsigned short* indices, UINT indexCount)
	{
		ASSERT(vertices);
		ASSERT(indices);

		Add(vertices, vertexCount, indices, indexCount);
	}

	void SpriteBatch::Draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color, bool positionIsCenter)
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
		SPRITE_ADD_VERTEX(v[0], x, y, z, u1, v1, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(v[1], x, y2, z, u1, v2, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(v[2], x2, y, z, u2, v1, color.r, color.g, color.b, color.a);
		SPRITE_ADD_VERTEX(v[3], x2, y2, z, u2, v2, color.r, color.g, color.b, color.a);

		// 0 ------- 2
		//   |	   |
		//   |	   |
		// 1 ------- 3

		//static unsigned short indices[4] = { 0, 1, 2, 3 };
		static const unsigned short indices[4] = { 0, 2, 1, 3 };

		Add(v, 4, indices, 4);
	}

	RenderState& SpriteBatch::GetStateBlock() const
	{
		return m_pTechnique->GetRenderState();
	}

	void SpriteBatch::SetProjectionMatrix(const Matrix4& matrix)
	{
		m_projectionMatrix = matrix;
	}

	const Matrix4& SpriteBatch::GetProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

	bool SpriteBatch::ClipSprite(const Rectangle& clip, float& x, float& y, float& width, float& height, float& u1, float& v1, float& u2, float& v2)
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
