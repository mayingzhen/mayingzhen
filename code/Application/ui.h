#pragma once

struct ImDrawData;


namespace ma
{
	class UIRenderable;
	
	class APP_API UI
	{

	public:
		UI();

		~UI();

		void Init();

		void Shutdown();

		void Update();

		void Render();

		/// UI 
		bool Begin(const char* name, bool* p_open = NULL, int flags = 0);
		void End();
		void SameLine(float pos_x = 0.0f, float spacing_w = -1.0f);
		bool Header(const char* caption);
		bool Checkbox(const char* caption, bool* value);
		bool InputFloat(const char* caption, float* value, float step, uint32_t precision);
		bool SliderFloat(const char* caption, float* value, float min, float max);
		bool SliderInt(const char* caption, int32_t* value, int32_t min, int32_t max);
		bool ComboBox(const char* caption, int32_t* itemindex, std::vector<std::string> items);
		bool Button(const char* caption);
		void Text(const char* formatstr, ...);
		bool ColorEdit4(const char* label, float col[4], int flags = 0);
		float GetFramerate();

	private:
		void RenderDrawData(ImDrawData* draw_dat);

		void RT_Render();

		UIRenderable* GetRenderable(uint32_t nIndex);

	private:
		RefPtr<Technique> m_pTechUI;

		RefPtr<SamplerState> m_pFontSampler;

		std::vector< RefPtr<UIRenderable> > m_vecRendeable[2];
		uint32_t m_nUsedCount[2] = { 0,0 };

		RefPtr<VertexBuffer> m_pVertexBuffer[3];
		RefPtr<IndexBuffer> m_pIndexBuffer[3];
		uint32_t m_nBufferIndex = 0;

		RefPtr<RenderStep> m_pRenderStep;

		RefPtr<RenderCommand> m_pUICommand;

		bool m_bInit = false;
	};

	extern UI* g_pUI;
	APP_API UI* GetUI();
}

