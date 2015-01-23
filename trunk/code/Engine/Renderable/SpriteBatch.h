#ifndef _SpriteBatch_H_
#define _SpriteBatch_H_


namespace ma
{
	class SubMaterial;
	class SamplerState;
	class Texture;
	class Technique;
 
	class RENDER_API SpriteBatch : public MeshBatch
	{
		friend class Bundle;
		friend class Font;

	public:

		SpriteBatch(RefPtr<Texture> texture, Technique* pTech = NULL, UINT initialCapacity = 0);

		virtual ~SpriteBatch();

		virtual void	Render(Technique* pTech);

		void Draw(const Rectangle& dst, const Rectangle& src, const ColourValue& color = ColourValue::White);

		void Draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const ColourValue& color = ColourValue::White);

		void Draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const ColourValue& color,
				  const Vector2& rotationPoint, float rotationAngle);

		void Draw(const Vector3& dst, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color,
				  const Vector2& rotationPoint, float rotationAngle, bool positionIsCenter = false);
	    
		void Draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color,
				  const Vector2& rotationPoint, float rotationAngle, bool positionIsCenter = false);

		void Draw(const Vector3& position, const Vector3& right, const Vector3& forward, float width, float height, 
				  float u1, float v1, float u2, float v2, const ColourValue& color, const Vector2& rotationPoint, float rotationAngle);

		void Draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color);

		void Draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color, const Rectangle& clip);

		void Draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color, bool positionIsCenter = false);
		
		void				SetTexture(RefPtr<Texture> pTexture);

		SamplerState*		GetSampler() {return m_pSampler.get();}
	
		RenderState&		GetStateBlock() const;

		Technique*			GetTechnique() {return m_pTechnique.get();}

		void				SetProjectionMatrix(const Matrix4& matrix);

		const Matrix4&		GetProjectionMatrix() const;

	private:

		/**
		 * Sprite vertex structure used for batching.
		 */
		struct SpriteVertex
		{
			float x;        
			float y;
			float z;
			float u;
			float v;
			float r;
			float g;
			float b;
			float a;
		};


		void AddSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color, SpriteBatch::SpriteVertex* vertices);

		void AddSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const ColourValue& color, const Rectangle& clip, SpriteBatch::SpriteVertex* vertices);

		void Draw(SpriteBatch::SpriteVertex* vertices, UINT vertexCount, uint16* indices, UINT indexCount);

		bool ClipSprite(const Rectangle& clip, float& x, float& y, float& width, float& height, float& u1, float& v1, float& u2, float& v2);

	private:
		RefPtr<Technique>		m_pTechnique;

		RefPtr<SamplerState>	m_pSampler;
		
		float					m_fTextureWidthRatio;
		
		float					m_fTextureHeightRatio;
		
		Matrix4					m_projectionMatrix;
	};

}

#endif
