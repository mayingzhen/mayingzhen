#ifndef  _SCENE_NODE__H__
#define  _SCENE_NODE__H__


namespace ma
{
	class Scene;
	class SceneVisiter;
	class GameObject;

	class ENGINE_API SceneNode : public Object
	{
		DECL_OBJECT(SceneNode)

	public:
		SceneNode(GameObject* pGameObj);

		~SceneNode();

		// Transform
		void					SetTransform(const NodeTransform& tsf);	
		
		const NodeTransform&	GetTransform();
		
		const Matrix4x4&		GetWorldMatrix();

		void					LookAt(const Vector3& vPos, const Vector3& vTarget,const Vector3& vUp);

		void					LookAt(const Vector3& vTarget,const Vector3& vUp);
		
		void					Translate(const Vector3& vDir);

		void					Rotate(float xDegree,float yDegree,float zDegree);
		
		void					Rotate(const Quaternion& qRot);

		void					RotateYAxisLS(float fDegree);
		
		void					RotateZAxisLS(float fDegree);
		
		void					RotateXAxisLS(float fDegree);

		Vector3					GetForward();
		
		Vector3					GetRight();
		
		Vector3					GetUp();

		void                    Forward(float fValue);
		
		void                    Up(float fValue);
		
		void                    Right(float fValue);

		void					Scale(float fScale);

		void					WorldToLocal(NodeTransform* pOutLocal,const NodeTransform* pWorld);

		GameObject*				GetGameObject() {return m_pGameObject;}

		void					Serialize(Serializer& sl, const char* pszLable = "SceneNode");
		
		bool					IsMatrixWSDirty() {return m_bmatWSDirty;}

	private:
		void					UpdateMatWorld();

	protected:

		GameObject*				m_pGameObject;
		
		Matrix4x4				m_matWorld;	
		
		NodeTransform			m_tsfWS;
		
		bool					m_bmatWSDirty;

		//AABB					m_aabbWS;	
	};

}


#endif