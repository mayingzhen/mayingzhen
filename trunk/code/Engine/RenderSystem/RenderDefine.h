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
	struct ENGINE_API LockedRect
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
	struct ENGINE_API LockedBox
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
	enum USAGE
	{
		USAGE_STATIC,
		USAGE_DYNAMIC,
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
	};


	/* enum: FORMAT
	----------------------------------------------------------
		@Remark:
			格式类型(像素格式, 索引格式).        
	----------------------------------------------------------
	*/

	enum FORMAT
	{
		FMT_UNKNOWN,

		FMT_R8G8B8,
		FMT_A8R8G8B8,
		FMT_X8R8G8B8,
		FMT_R5G6B5,
		FMT_X1R5G5B5,
		FMT_A1R5G5B5,
		FMT_A4R4G4B4,
		FMT_R3G3B2,
		FMT_A8,
		FMT_A8R3G3B2,
		FMT_X4R4G4B4,
		FMT_A2B10G10R10,
		FMT_A8B8G8R8,
		FMT_X8B8G8R8,
		FMT_G16R16,
		FMT_A2R10G10B10,
		FMT_A16B16G16R16,

		FMT_A8P8,
		FMT_P8,

		FMT_L8,
		FMT_A8L8,
		FMT_A4L4,

		FMT_V8U8,
		FMT_L6V5U5,
		FMT_X8L8V8U8,
		FMT_Q8W8V8U8,
		FMT_V16U16,
		FMT_A2W10V10U10,

		FMT_UYVY,
		FMT_R8G8_B8G8,
		FMT_YUY2,
		FMT_G8R8_G8B8,
		FMT_DXT1,
		FMT_DXT2,
		FMT_DXT3,
		FMT_DXT4,
		FMT_DXT5,

		FMT_D16_LOCKABLE,
		FMT_D32,
		FMT_D15S1,
		FMT_D24S8,
		FMT_D24X8,
		FMT_D24X4S4,
		FMT_D16,

		FMT_D32F_LOCKABLE,
		FMT_D24FS8,

		FMT_L16,

		FMT_VERTEXDATA,

		FMT_INDEX16,
		FMT_INDEX32,

		FMT_Q16W16V16U16,

		FMT_MULTI2_ARGB8,

		// Floating point surface formats(s10e5 formats <16-bits per channel>)
		FMT_R16F,
		FMT_G16R16F,
		FMT_A16B16G16R16F,

		// IEEE s23e8 formats (32-bits per channel)
		FMT_R32F,
		FMT_G32R32F,
		FMT_A32B32G32R32F,

		//D3D9 Ex
		FMT_D32_LOCKABLE,
		FMT_S8_LOCKABLE,
		FMT_CxV8U8,
		FMT_A1,
		FMT_BINARYBUFFER,

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
		TEXTYPE_RENDERTARGET,       // render target
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
		DT_SHORT4         = 8,

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
		DU_TEXCOORD0     = 1,
		DU_BLENDINDICES  = 2,
		DU_BLENDWEIGHT   = 3,
		DU_NORMAL        = 4,
		DU_TANGENT       = 5,
		DU_COLOR         = 6,
		DU_TEXCOORD1     = 7,
	    
		DU_UNKNOWN       = 255
	};


	enum DECL_USAGEMASK
	{
		DUM_POSITION      = 1 << 0,
		DUM_TEXCOORD      = 1 << 1,
		DUM_BLENDWEIGHT   = 1 << 2,
		DUM_BLENDINDICES  = 1 << 3,
		DUM_NORMAL        = 1 << 4,
		DUM_TANGENT       = 1 << 5,
		DUM_COLOR         = 1 << 6,

		DUM_UNKNOWN       = 255
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

	struct ENGINE_API VertexElement
	{
		short           Stream;
		short           Offset;
		DECL_TYPE       Type;
		unsigned char   unused;
		DECL_USAGE      Usage;
		unsigned char   UsageIndex;

		VertexElement()
		: Stream(DECL_UNUSED),
		  Offset(0),
		  Type(DT_UNKNOWN),
		  unused(0),
		  Usage(DU_UNKNOWN),
		  UsageIndex(0)
		{
		}
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


	/* enum: FILL_MODE
	----------------------------------------------------------
		@Remark:
			填充模式.        
	----------------------------------------------------------
	*/
	enum FILL_MODE
	{
		FILL_POINT                  = 1,                //点填充
		FILL_FRAME                  = 2,                //框架填充
		FILL_SOLID                  = 3,                //实填充
	};

	enum BLEND_MODE
	{
		BM_OPATICY,
		BM_TRANSPARENT,
		BM_ADD,
		BM_MULTIPLY,
	};

	enum DEPTH_CHECK_MODE
	{
		DCM_LESS_EQUAL,
		DCM_LESS,
		DCM_GREATER_EQUAL,
		DCM_GREATER,
		DCM_EQUAL,
		DCM_ALWAYS,
		DCM_NONE,
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
}

#endif