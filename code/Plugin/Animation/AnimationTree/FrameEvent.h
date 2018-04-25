#pragma once



namespace ma
{

	class AnimTreeNode;


	class FrameEvent : public Serializable
	{
	public:
		FrameEvent();
		virtual ~FrameEvent(){};

		DECL_OBJECT(FrameEvent);

		float GetFrame() const {return m_fFrame;}
		void SetFrame(float fFrame) {m_fFrame = fFrame;}
		
		uint32_t GetEventID() const {return m_nEventID;}
		void SetEvnetID(uint32_t nEnventID) {m_nEventID = nEnventID;} 
		
		virtual void DoEvent(AnimTreeNode* pNode) = 0;

	protected:
		float	m_fFrame;
		uint32_t	m_nEventID;
	};

}
