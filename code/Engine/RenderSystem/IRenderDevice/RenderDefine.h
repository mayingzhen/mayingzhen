#ifndef  _RenderDefine__H__
#define  _RenderDefine__H__

namespace ma
{
	/* LockedRect
	----------------------------------------------------------
		@Remark:
			锁定的矩形(锁纹理用的).
			某些纹理一行的实际大小可能和定义时不同.
	----------------------------------------------------------
	*/
	struct LockedRect
	{
		int  Pitch;
		void * pData;
	};


	/* LockedBox
	----------------------------------------------------------
		@Remark:
			锁定的长方体(锁纹理用的).
			某些纹理一行的实际大小可能和定义时不同.       
	----------------------------------------------------------
	*/
	struct LockedBox
	{
		int  RowPitch;
		int  SlicePitch;
		void * pData;
	};



	/* USAGE
	----------------------------------------------------------
		@Remark:
			用法(硬件缓存使用)      
	----------------------------------------------------------
	*/
	enum TEXTURE_USAGE
	{
		USAGE_STATIC,
		USAGE_RENDERTARGET,
		USAGE_DEPTHSTENCIL,
		USAGE_DYNAMIC,
	};

	enum BUFFER_USAGE
	{

	};

	/* enum: LOCK
	----------------------------------------------------------
		@Remark:
			锁定方式.        
	----------------------------------------------------------
	*/
	enum LOCK
	{
		LOCK_NORMAL         = 0,
		LOCK_DISCARD        = 1,
		LOCK_NOOVERWRITE    = 2,
		LOCK_READONLY       = 4,
		LOCK_WRITE          = 5,
		LOCK_READWRITE      = 6,
	};



	/* enum: TEXTURE_TYPE
	----------------------------------------------------------
		@Remark:
			纹理类型.        
	----------------------------------------------------------
	*/
	enum TEXTURE_TYPE
	{
		TEXTYPE_2D,                 // normal texture
		TEXTYPE_3D,                 // volume texture
		TEXTYPE_CUBE,               // cube texture
		//TEXTYPE_RENDERTARGET,       // render target
	};

	/* enum: DECL_TYPE
	----------------------------------------------------------
		@Remark:
			定点元素类型.        
	----------------------------------------------------------
	*/
	enum DECL_TYPE
	{
		DT_FLOAT1         = 0,
		DT_FLOAT2         = 1,
		DT_FLOAT3         = 2,
		DT_FLOAT4         = 3,
		DT_COLOR          = 4,
		DT_UBYTE4         = 5,
		DT_UBYTE4N        = 6,
		DT_SHORT2         = 7,
		DT_SHORT2N		  = 8,
		DT_SHORT4         = 9,
		DT_SHORT4N		  = 10,


		DT_UNKNOWN        = 255
	};


	/* enum: DECL_USAGE
	----------------------------------------------------------
		@Remark:
			定点元素语义.        
	----------------------------------------------------------
	*/
	enum DECL_USAGE
	{
		DU_POSITION      = 0,
		DU_TEXCOORD		 = 1,
		DU_BLENDINDICES  = 2,
		DU_BLENDWEIGHT   = 3,
		DU_NORMAL        = 4,
		DU_TANGENT       = 5,
		DU_COLOR         = 6,
		
		MAX_DECL_USAGE   = 7,

		DU_UNKNOWN       = 255
	};


	enum INDEX_TYPE
	{
		INDEX_TYPE_U16,
		INDEX_TYPE_U32,
	};


	/* struct: VertexElement
	--------------------------------------------------------------------------------------
		@Remark:
			顶点元素声明.
	--------------------------------------------------------------------------------------
	*/
	#define MAX_ELEMENT             16

	#define DECL_UNUSED             255

	struct  VertexElement
	{
		short           Stream;
		short           Offset;
		DECL_TYPE       Type;
		DECL_USAGE      Usage;
		unsigned char   UsageIndex;

		uint32 mHash;
	
		VertexElement()
		: Stream(DECL_UNUSED),
		  Offset(0),
		  Type(DT_UNKNOWN),
		  Usage(DU_UNKNOWN),
		  UsageIndex(0),
		  mHash(0)
		{
		}

		VertexElement(short stream,short offset,DECL_TYPE type,DECL_USAGE use,unsigned char index)
			: Stream(stream),
			Offset(offset),
			Type(type),
			Usage(use),
			UsageIndex(index),
			mHash(0)
		{
			BuildHash();
		}

		// ---------------------------------------------------------------------
		//  stream  offset  type  semantic index GLLocate
		//   4        12     4      4        4	    4
		//  0~15    0~4096-1 0~15   0~15    0~15   0~15
		// ---------------------------------------------------------------------
		void BuildHash()
		{
			mHash = 0;

			//ASSERT(mGLLocate <= 15);
			//UINT32 n = mGLLocate;
			//mHash += n;

			ASSERT(UsageIndex<= 15);
			UINT32 n = UsageIndex;
			mHash += n << 4;

			ASSERT(Usage<= 15);
			n = Usage;
			mHash += n << 8;

			ASSERT(Type<= 15);
			n = Type;
			mHash += n << 12;

			ASSERT(Offset<= 4096 - 1);
			n = Offset;
			mHash += n << 16;

			ASSERT(Stream<= 15);
			n = Stream;
			mHash += n << 28;
		}
		uint32 GetHash() const{return mHash;}
	};

	enum PRIMITIVE_TYPE
	{
		PRIM_UNKNOW                 = 0,
		PRIM_POINTLIST              = 1,
		PRIM_LINELIST               = 2,
		PRIM_LINESTRIP              = 3,
		PRIM_TRIANGLELIST           = 4,
		PRIM_TRIANGLESTRIP          = 5,
		PRIM_TRIANGLEFAN            = 6,
	};


	/* enum: CULL_MODE
	----------------------------------------------------------
		@Remark:
//			背面裁剪模式.        
	----------------------------------------------------------
	*/
	enum CULL_MODE
	{
		CULL_FACE_SIDE_NONE,
		CULL_FACE_SIDE_BACK,
		CULL_FACE_SIDE_FRONT,
		CULL_FACE_SIDE_FRONT_AND_BACK,
	};

	static const char* strCullMode[] =
	{
		"CULL_FACE_SIDE_NONE",
		"CULL_FACE_SIDE_BACK",
		"CULL_FACE_SIDE_FRONT",
		"CULL_FACE_SIDE_FRONT_AND_BACK",
		0
	};

	enum BLEND_MODE
	{
		BM_OPATICY,
		BM_TRANSPARENT,
		BM_ADD,
		BM_MULTIPLY,
	};

	static const char* strBlendMode[] =
	{
		"BM_OPATICY",
		"BM_TRANSPARENT",
		"BM_ADD",
		"BM_MULTIPLY",
		0
	};

	/// Fill mode.
	enum FillMode
	{
		FILL_SOLID = 0,
		FILL_WIREFRAME,
		FILL_POINT
	};


	enum CompareFunction
	{
		CMPF_ALWAYS_FAIL,
		CMPF_ALWAYS_PASS,
		CMPF_LESS,
		CMPF_LESS_EQUAL,
		CMPF_EQUAL,
		CMPF_NOT_EQUAL,
		CMPF_GREATER_EQUAL,
		CMPF_GREATER
	};

	static const char* strCompareFunction[] =
	{
 		"CMPF_ALWAYS_FAIL",
 		"CMPF_ALWAYS_PASS",
 		"CMPF_LESS",
 		"CMPF_LESS_EQUAL",
 		"CMPF_EQUAL",
 		"CMPF_NOT_EQUAL",
 		"CMPF_GREATER_EQUAL",
		"CMPF_GREATER",
 		0
	 };


	/// Enum describing the various actions which can be taken onthe stencil buffer
	enum StencilOperation
	{
		/// Leave the stencil buffer unchanged
		SOP_KEEP,
		/// Set the stencil value to zero
		SOP_ZERO,
		/// Set the stencil value to the reference value
		SOP_REPLACE,
		/// Increase the stencil value by 1, clamping at the maximum value
		SOP_INCREMENT,
		/// Decrease the stencil value by 1, clamping at 0
		SOP_DECREMENT,
		/// Increase the stencil value by 1, wrapping back to 0 when incrementing the maximum value
		SOP_INCREMENT_WRAP,
		/// Decrease the stencil value by 1, wrapping when decrementing 0
		SOP_DECREMENT_WRAP,
		/// Invert the bits of the stencil buffer
		SOP_INVERT
	};

	static const char* strStencilOperation[] =
	{
		/// Leave the stencil buffer unchanged
		"SOP_KEEP",
		/// Set the stencil value to zero
		"SOP_ZERO",
		/// Set the stencil value to the reference value
		"SOP_REPLACE",
		/// Increase the stencil value by 1, clamping at the maximum value
		"SOP_INCREMENT",
		/// Decrease the stencil value by 1, clamping at 0
		"SOP_DECREMENT",
		/// Increase the stencil value by 1, wrapping back to 0 when incrementing the maximum value
		"SOP_INCREMENT_WRAP",
		/// Decrease the stencil value by 1, wrapping when decrementing 0
		"SOP_DECREMENT_WRAP",
		/// Invert the bits of the stencil buffer
		"SOP_INVERT",
		0
	};

	/// Shader types.
	enum ShaderType
	{
		VS = 0,
		PS,
	};



	/* enum: COLOR_WRITE_ENABLE
	----------------------------------------------------------
		@Remark:
			颜色写入通道.        
	----------------------------------------------------------
	*/
	enum COLOR_WRITE_ENABLE
	{
		CWE_RED    = 1 << 0,
		CWE_BLUE   = 1 << 1,
		CWE_GREEN  = 1 << 2,
		CWE_ALPHA  = 1 << 3,
		CWE_ALL    = CWE_RED | CWE_BLUE | CWE_GREEN | CWE_ALPHA,
	};

	enum LIGHT_TYPE
	{
		LT_POINT                 = 1,
		LT_SPOT                  = 2,
		LT_DIRECTIONAL           = 3,
	};

	enum TRANSFORM_TYPE
	{
		TS_LOCAL,
		TS_PARENT,
		TS_WORLD,
	};

	enum PROJECTION_TYPE
	{
		PROJTYPE_ORTHO,
		PROJTYPE_PROJECTION,
	};

	enum Filter
	{
		/// Equal to: min=FO_POINT, mag=FO_POINT, mip=FO_NONE
		TFO_POINT,
		/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_POINT
		TFO_BILINEAR,
		/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_LINEAR
		TFO_TRILINEAR,
		/// Equal to: min=FO_ANISOTROPIC, max=FO_ANISOTROPIC, mip=FO_LINEAR
		TFO_ANISOTROPIC,

		TFO_SHADOWCOMPARE,
	};

	static const char* strDescFilter[] =
	{
		"TFO_POINT",
		"TFO_BILINEAR",
		"TFO_TRILINEAR",
		"TFO_ANISOTROPIC",
		"TFO_SHADOWCOMPARE",
		0
	};


	enum Wrap
	{
		REPEAT,
		CLAMP 
	};

	static const char* strDescWrap[] =
	{
		"REPEAT",
		"CLAMP", 
		0
	};

	enum HBU_USAGE 
    {
        /** Static buffer which the application rarely modifies once created. Modifying 
        the contents of this buffer will involve a performance hit.
        */
        HBU_STATIC = 1,

	    /** Indicates the application would like to modify this buffer with the CPU
	    fairly often. 
	    Buffers created with this flag will typically end up in AGP memory rather 
	    than video memory.
	    */
	    HBU_DYNAMIC = 2,

	    /** Indicates the application will never read the contents of the buffer back, 
	    it will only ever write data. Locking a buffer with this flag will ALWAYS 
	    return a pointer to new, blank memory rather than the memory associated 
	    with the contents of the buffer; this avoids DMA stalls because you can 
	    write to a new memory area while the previous one is being used. 
	    */
	    HBU_WRITE_ONLY = 4,

        /** Indicates that the application will be refilling the contents
        of the buffer regularly (not just updating, but generating the
        contents from scratch), and therefore does not mind if the contents 
        of the buffer are lost somehow and need to be recreated. This
        allows and additional level of optimisation on the buffer.
        This option only really makes sense when combined with 
        HBU_DYNAMIC_WRITE_ONLY.
        */
        HBU_DISCARDABLE = 8,

		/// Combination of HBU_STATIC and HBU_WRITE_ONLY
		HBU_STATIC_WRITE_ONLY = 5, 

		/** Combination of HBU_DYNAMIC and HBU_WRITE_ONLY. If you use 
        this, strongly consider using HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE
        instead if you update the entire contents of the buffer very 
        regularly. 
        */
		HBU_DYNAMIC_WRITE_ONLY = 6,

        /// Combination of HBU_DYNAMIC, HBU_WRITE_ONLY and HBU_DISCARDABLE
        HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE = 14

    };
	

	enum ShaderParameterGroup
	{
		SP_FRAME = 0,
		SP_CAMERA,
		SP_ZONE,
		SP_LIGHT,
		SP_MATERIAL,
		SP_OBJECT,
		SP_CUSTOM,
		MAX_SHADER_PARAMETER_GROUPS
	};

	static const int MAX_RENDERTARGETS = 4;
	static const int MAX_VERTEX_STREAMS = 4;
	static const int MAX_CONSTANT_REGISTERS = 256;
	static const int MAX_TEXTURE_UNITS = 16;

}

#endif