#ifndef _SpriteBatch_H_
#define _SpriteBatch_H_


namespace ma
{
	class Material;
	class SamplerState;
 
	class RENDER_API SpriteBatch
	{
		friend class Bundle;
		friend class Font;

	public:

		SpriteBatch(Texture* texture, Technique* pTech = NULL, UINT initialCapacity = 0);

		virtual ~SpriteBatch();

		void start();

		void draw(const Rectangle& dst, const Rectangle& src, const Vector4& color = Vec4One());

		void draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Vector4& color = Vec4One());

		void draw(const Vector3& dst, const Rectangle& src, const Vector2& scale, const Vector4& color,
				  const Vector2& rotationPoint, float rotationAngle);

		void draw(const Vector3& dst, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color,
				  const Vector2& rotationPoint, float rotationAngle, bool positionIsCenter = false);
	    
		void draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color,
				  const Vector2& rotationPoint, float rotationAngle, bool positionIsCenter = false);

		void draw(const Vector3& position, const Vector3& right, const Vector3& forward, float width, float height, 
				  float u1, float v1, float u2, float v2, const Vector4& color, const Vector2& rotationPoint, float rotationAngle);

		void draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color);

		void draw(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, const Rectangle& clip);

		void draw(float x, float y, float z, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, bool positionIsCenter = false);

		void finish(Technique* pTech = NULL);


		SamplerState*		getSampler() const;

		RenderState&		getStateBlock() const;

		Technique*			GetTechnique() {return m_pTechnique;}

		void				setProjectionMatrix(const Matrix4x4& matrix);

		const Matrix4x4&	getProjectionMatrix() const;


		MeshBatch*			GetMeshBatch() {return m_pMeshBatch;}

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


		void addSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, SpriteBatch::SpriteVertex* vertices);

		void addSprite(float x, float y, float width, float height, float u1, float v1, float u2, float v2, const Vector4& color, const Rectangle& clip, SpriteBatch::SpriteVertex* vertices);

		void draw(SpriteBatch::SpriteVertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount);

		bool clipSprite(const Rectangle& clip, float& x, float& y, float& width, float& height, float& u1, float& v1, float& u2, float& v2);

	private:
		MeshBatch*		m_pMeshBatch;

		Technique*		m_pTechnique;

		SamplerState*	m_pSampler;
		
		float			m_fTextureWidthRatio;
		
		float			m_fTextureHeightRatio;
		
		Matrix4x4		m_projectionMatrix;
	};

}

#endif
