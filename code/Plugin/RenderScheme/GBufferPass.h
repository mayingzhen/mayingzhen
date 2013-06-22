#ifndef  _GBufferPass__H__
#define  _GBufferPass__H__




class RENDERSCHEME_API GBufferPass
{
public:
	GBufferPass();

	void Init();

	void Shutdown();

	void DoRender();

private:
	ShaderProgram*				m_pGBufferTech;
	ShaderProgram*				m_pGBufferSkinTech;

	RenderTarget*				m_pDepthTex;
	RenderTarget*				m_pNormalTex;

	std::vector<Renderable*>	m_SolidEntry;

	Camera*						m_pMainCamera;

};

#endif