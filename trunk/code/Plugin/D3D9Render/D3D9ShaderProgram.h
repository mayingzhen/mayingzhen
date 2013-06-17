#ifndef D3D9EFFECT_H_
#define D3D9EFFECT_H_



namespace ma
{

	class D3D9ShaderProgram : public ShaderProgram
	{
	public:

		D3D9ShaderProgram();

		~D3D9ShaderProgram();

		virtual void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);

		virtual void ParseUniform();

		/**
		 * Sets a float uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param value The float value to set.
		 */
		void SetValue(Uniform* uniform, float value);

		/**
		 * Sets a float array uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param values The array to set.
		 * @param count The number of elements in the array.
		 */
		void SetValue(Uniform* uniform, const float* values, unsigned int count = 1);

		/**
		 * Sets an integer uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param value The value to set.
		 */
		void SetValue(Uniform* uniform, int value);

		/**
		 * Sets an integer array uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param values The array to set.
		 * @param count The number of elements in the array.
		 */
		void SetValue(Uniform* uniform, const int* values, unsigned int count = 1);

		/**
		 * Sets a matrix uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param value The value to set.
		 */
		void SetValue(Uniform* uniform, const Matrix4x4& value);

		/**
		 * Sets a matrix array uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param values The array to set.
		 * @param count The number of elements in the array.
		 */
		void SetValue(Uniform* uniform, const Matrix4x4* values, unsigned int count = 1);

		/**
		 * Sets a vector uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param value The value to set.
		 */
		void SetValue(Uniform* uniform, const Vector2& value);

		/**
		 * Sets a vector array uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param values The array to set.
		 * @param count The number of elements in the array.
		 */
		void SetValue(Uniform* uniform, const Vector2* values, unsigned int count = 1);

		/**
		 * Sets a vector uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param value The value to set.
		 */
		void SetValue(Uniform* uniform, const Vector3& value);

		/**
		 * Sets a vector array uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param values The array to set.
		 * @param count The number of elements in the array.
		 */
		void SetValue(Uniform* uniform, const Vector3* values, unsigned int count = 1);

		/**
		 * Sets a vector uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param value The value to set.
		 */
		void SetValue(Uniform* uniform, const Vector4& value);

		/**
		 * Sets a vector array uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param values The array to set.
		 * @param count The number of elements in the array.
		 */
		void SetValue(Uniform* uniform, const Vector4* values, unsigned int count = 1);

		/**
		 * Sets a sampler uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param sampler The sampler to set.
		 */
		void SetValue(Uniform* uniform, const Sampler* sampler);

		/**
		 * Sets a sampler array uniform value.
		 *
		 * @param uniform The uniform to set.
		 * @param values The sampler array to set.
		 * @param count The number of elements in the array.
		 *
		 * @script{ignore}
		 */
		void SetValue(Uniform* uniform, const Sampler** values, unsigned int count);

		/**
		 * Binds this effect to make it the currently active effect for the rendering system.
		 */
		void Bind();

		/**
		 * Returns the currently bound effect for the rendering system.
		 *
		 * @return The currently bound effect, or NULL if no effect is currently bound.
		 */
		//static ShaderProgram* GetCurrentEffect();

	private:
		void BindSampler(const Sampler* pSampler);

	private:

		IDirect3DVertexShader9*		m_pVertexShader;
		IDirect3DPixelShader9*		m_pPiexelShader;

		LPD3DXCONSTANTTABLE			m_pVShConstantTable;
		LPD3DXCONSTANTTABLE			m_pPShConstantTable;
	};


}

#endif
