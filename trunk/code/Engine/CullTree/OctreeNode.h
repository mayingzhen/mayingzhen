#ifndef  _OctreeNode__H__
#define  _OctreeNode__H__

#include "CullNode.h"

namespace ma
{

	class OctreeNode : public CullNode
	{
	public:
		OctreeNode(const AABB& box);

		~OctreeNode();

		// ---------------------------------------------------------------------
		// Inherite from CCullNode
		// ---------------------------------------------------------------------
	public:
		void AddObject(GameObject* pObject);
		
		void RemoveObject(GameObject* pObject);

		// ---------------------------------------------------------------------
		// Self
		// ---------------------------------------------------------------------
	public:
		// ��ȡ��Ҫ�ü��İ�Χ�У��˰�Χ�б�ʵ�ʵİ�Χ�и���
		void GetCullBoundingBox(AABB& box) const;

		// ʵ�ʰ�Χ��
		const AABB& GetBoundingBox() const{return m_AABB;}

		// ��node��������box��������
		bool IsTwiceSize(const AABB& box) const;

		// ��ȡ�ʺ�box��child
		void GetChildIndexByBox(const AABB& box, int& x, int& y, int& z);

		// object operation
		UINT GetObjectAmount() const;

		GameObject* GetObjectByIndex(UINT nIndex) const;

	private:
		AABB		m_AABB;

		Vector3		m_vHalfSize;

		typedef std::vector<GameObject*> VEC_OBJECT;
		VEC_OBJECT	m_vecObject;

	public:
		OctreeNode* m_rgChildren[2][2][2];
	};

}

#endif