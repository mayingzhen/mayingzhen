#ifndef  _VertexDeclaration__H__
#define  _VertexDeclaration__H__

#include "RenderDefine.h"

namespace ma
{
	class ShaderProgram;

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

		void					AddElement(const VertexElement& element);

        UINT					GetStreanmStride(UINT nStream) const;
		
        UINT					GetElementCount(UINT nStream);
		
        const VertexElement&	GetElement(UINT nStream, UINT index);
		
		uint64					GetHash();

		bool					Import(rapidxml::xml_node<>* pXmlElem);
		bool					Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc);

	private:
		int						GetDeclTypeSize(DECL_TYPE type);

	protected:
		typedef std::vector<VertexElement> StreamElement;
		std::vector<StreamElement> 	m_arrStreamElement;
		
		std::vector<UINT>			m_arrStreamStride;
	};

	RefPtr<VertexDeclaration> CreateVertexDeclaration();

}

#endif
