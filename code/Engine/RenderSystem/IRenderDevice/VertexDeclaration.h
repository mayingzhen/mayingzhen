#ifndef  _VertexDeclaration__H__
#define  _VertexDeclaration__H__

#include "RenderDefine.h"

namespace ma
{
	class ShaderProgram;

#define MAX_ELEMENT             16

#define DECL_UNUSED             255

	struct  VertexElement : public Object
	{
	public:
		DECL_OBJECT(VertexElement);

		static void	RegisterAttribute();

		short           Stream;
		short           Offset;
		DECL_TYPE       Type;
		DECL_USAGE      Usage;
		short			UsageIndex;

		uint32 mHash;

		VertexElement();

		VertexElement(short stream, short offset, DECL_TYPE type, DECL_USAGE use, unsigned char index);

		void BuildHash();

		uint32 GetHash() const { return mHash; }
	};

	class VertexDeclaration : public Referenced
	{
	public: 
		VertexDeclaration();

		virtual ~VertexDeclaration();
			
		virtual void			RT_StreamComplete() = 0;

		void					AddElement(short number, short Offset, DECL_TYPE DeclType, DECL_USAGE DeclUsage, unsigned char index);
		
		void					AddElement(const VertexElement& element);

		int						GetStreanmStride() const {return m_nStreamStride;} 
		
		int						GetElementCount() { return m_ElementCount; }
		
		const VertexElement&	GetElement(int index) { ASSERT (index < m_ElementCount); return m_Elements[index]; }
		
		uint64					GetHash();

		void					SetShaderProgram(ShaderProgram* pShader) { m_pShader = pShader; }


		bool					Import(rapidxml::xml_node<>* pXmlElem);
		bool					Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc);

	private:
		int						GetDeclTypeSize(DECL_TYPE type);

	protected:
		VertexElement			m_Elements[MAX_ELEMENT];

		int						m_ElementCount;
		
		int						m_nStreamStride;

		RefPtr<ShaderProgram>	m_pShader;
	};

	RefPtr<VertexDeclaration> CreateVertexDeclaration();

}

#endif