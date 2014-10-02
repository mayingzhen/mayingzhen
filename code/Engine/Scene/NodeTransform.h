#ifndef  _SCENE_NODE__H__
#define  _SCENE_NODE__H__


namespace ma
{
	class Scene;
	class SceneVisiter;
	class SceneNode;

	class ENGINE_API NodeTransform : public Object
	{
		DECL_OBJECT(NodeTransform)

	public:
		NodeTransform();

		~NodeTransform();

		void					SetTransform(const Transform& tsf);	
		
		const Transform&		GetTransform();
		
		const Matrix4&			GetWorldMatrix();

		void					LookAt(const Vector3& vPos, const Vector3& vTarget);

		void					LookAt(const Vector3& vTarget);
		
		void					Translate(const Vector3& vDir);

		void					RotateLS(float xDegree,float yDegree,float zDegree);

		void					RotateWS(float xDegree,float yDegree,float zDegree);

		void					RotateAround(const Vector3& vPoint, Vector3 vAxis,float angle); 

		Vector3					GetForward();
		
		Vector3					GetRight();
		
		Vector3					GetUp();

		Vector3					GetPos();

		Quaternion				GetRotate();

		void                    Forward(float fValue);
		
		void                    Up(float fValue);
		
		void                    Right(float fValue);

		void					Scale(float fScale);

		void					Serialize(Serializer& sl, const char* pszLable = "SceneNode");
		
		bool					IsMatrixWSDirty() {return m_bmatWSDirty;}

		virtual void			OnTransformChange() {}

	private:
		void					UpdateMatWorld();

	protected:
		Matrix4					m_matWorld;	
		
		Transform				m_tsfWS;
		
		bool					m_bmatWSDirty;
	};

}


#endif
