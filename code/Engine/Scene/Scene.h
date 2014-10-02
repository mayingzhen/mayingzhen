#ifndef  _Scene__H__
#define  _Scene__H__


namespace ma
{
	class SceneNode;
	class IPhysicsScene;
	class Camera;
	class MeshComponent;
	class CullTree;


	class ENGINE_API Scene  : public Referenced
	{
	
	public:
		Scene(const char* pszName = NULL);

		SceneNode*				CreateNode(const char* pName);

		void					Reset();

		void					Serialize(Serializer& sl, const char* pszLable = "Scene");

		CullTree*				GetCullTree() {return m_pCullTree.get();}

		SceneNode*				GetRootNode() {return m_pRootNode.get();}
		
	private:
		RefPtr<SceneNode>		m_pRootNode;	

		RefPtr<CullTree>		m_pCullTree;

		std::string				m_sName;
	};

	DeclareRefPtr(Scene);
}



#endif // _Scene__H__
