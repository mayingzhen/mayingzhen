#pragma once

#include "RenderDefine.h"

namespace ma
{
	class ShaderProgram;

	struct  VertexElement 
	{
	public:
		short           Stream;
		short           Offset;
		DECL_TYPE       Type;
		DECL_USAGE      Usage;
		short			UsageIndex;

		uint32_t mHash;

		VertexElement();

		VertexElement(short stream, short offset, DECL_TYPE type, DECL_USAGE use, unsigned char index);

		void BuildHash();

		uint32_t GetHash() const { return mHash; }
	};

	class VertexDeclaration : public Referenced
	{
	public: 
		VertexDeclaration();

		virtual ~VertexDeclaration();
			
		virtual void			RT_StreamComplete() = 0;

		void					AddElement(const VertexElement& element);

        uint32_t				GetStreanmStride(uint32_t nStream) const;
		
        uint32_t				GetElementCount(uint32_t nStream);
		
        const VertexElement&	GetElement(uint32_t nStream, uint32_t index);
		
		uint64_t				GetHash();

		bool					Import(rapidxml::xml_node<>* pXmlElem);
		bool					Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc);

	private:
		int						GetDeclTypeSize(DECL_TYPE type);

	protected:
		typedef std::vector<VertexElement> StreamElement;
		std::vector<StreamElement> 	m_arrStreamElement;
		
		std::vector<uint32_t>			m_arrStreamStride;
	};

	RefPtr<VertexDeclaration> CreateVertexDeclaration();

}