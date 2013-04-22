#ifndef  _VertexDeclaration__H__
#define  _VertexDeclaration__H__

#include "RenderDefine.h"

namespace ma
{

	class ENGINE_API VertexDeclaration : public RefCountObject
	{
		static int GetDeclTypeSize(DECL_TYPE type);

	public: 
		VertexDeclaration();
		virtual ~VertexDeclaration();

		virtual void Init() = 0;

		void AddElement(short number, short Offset, 
			DECL_TYPE DeclType, DECL_USAGE DeclUsage, unsigned char index);

		int GetStreamSize(short number);
		void GetStreamItem(short number, VertexElement * ve, int & count);
		void GetAllStreamItem(VertexElement * ve, int & count);
		void GetElement(VertexElement & ve);
		const VertexElement * GetElementByUsage(DECL_USAGE usage);

		int GetElementCount() { return mElementCount; }
		const VertexElement & GetElement(int index) { assert (index < mElementCount); return mElements[index]; }

	protected:
		VertexElement mElements[MAX_ELEMENT];
		int mElementCount;

	};

	DeclareSmartPtr(VertexDeclaration);

}

#endif