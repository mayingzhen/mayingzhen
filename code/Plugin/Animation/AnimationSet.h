#ifndef  _AnimationSet__H__
#define  _AnimationSet__H__

namespace ma
{
	class SkelAnimtion;
	class Skeleton;
	class AnimTreeNode;

	class AnimationSet : public Serializable
	{

	public:
		AnimationSet();

		~AnimationSet();

		DECL_OBJECT(AnimationSet)	

		AnimTreeNode*		GetSkelAnimByName(const char* pszName);

		UINT				GetSkelAnimNumber() {return m_arrSkelAnim.size();}

		AnimTreeNode*		GetSkelAnimByIndex(UINT index);

		void				AddSkelAnim(AnimTreeNode* pAction); 

		void				RemoveSkelAnim(AnimTreeNode* pAction);

		virtual bool		Improt(rapidxml::xml_node<>* pXmlElem);
		virtual bool		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);		

	private:
		typedef std::vector< RefPtr<AnimTreeNode> >	 VEC_SkELANIM;
		VEC_SkELANIM				m_arrSkelAnim;
	};

	RefPtr<AnimationSet> CreateAnimationSet();
	RefPtr<AnimationSet> CreateAnimationSet(const char* pszFile);
}


#endif
