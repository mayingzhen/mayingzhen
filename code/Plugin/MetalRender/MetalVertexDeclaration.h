#pragma once


namespace ma
{
	class MetalVertexDeclaration : public VertexDeclaration
	{

	public: 
		MetalVertexDeclaration();
		
		~MetalVertexDeclaration();

	public:

		virtual void RT_StreamComplete();

		static void Clear();

	public:
		MTLVertexDescriptor* m_descriptor;
	};

}

