#pragma once

namespace ma
{

	/** The parent class of commands stored in the rendering command queue. */
	class RenderCommand
	{
	public:
		virtual UINT Execute() = 0;
		virtual const char* DescribeCommand() = 0;
	};


	/**
	* Enqueues a rendering command with the given type and parameters.
	*/
#define ENQUEUE_RENDER_COMMAND(TypeName,Params) \
{ \
	MILK_ASSERT(!RenderingThread::s_is_threaded_rendering || IsInGameThread); \
	if(RenderingThread::s_is_threaded_rendering) \
	{ \
		FRingBuffer::AllocationContext AllocationContext(RenderingThread::s_render_command_buffer,sizeof(TypeName)); \
		if(AllocationContext.GetAllocatedSize() < sizeof(TypeName)) \
		{ \
			MILK_ASSERT(AllocationContext.GetAllocatedSize() >= sizeof(FSkipRenderCommand)); \
			new(AllocationContext) FSkipRenderCommand(AllocationContext.GetAllocatedSize()); \
			AllocationContext.Commit(); \
			new(FRingBuffer::AllocationContext(RenderingThread::s_render_command_buffer,sizeof(TypeName))) TypeName Params; \
		} \
		else \
		{ \
			new(AllocationContext) TypeName Params; \
		} \
	} \
	else \
	{ \
		TypeName TypeName##Command Params; \
		TypeName##Command.Execute(); \
	} \
}

	/* ::QueryPerformanceCounter( (LARGE_INTEGER*)&RenderingThread::s_end_time );\
	++RenderingThread::s_count;\
	RenderingThread::s_total += RenderingThread::s_end_time-RenderingThread::s_begin_time;\
	RenderingThread::s_average = (double)RenderingThread::s_total/RenderingThread::s_count;\
	*/
	/**
	* Declares a rendering command type with 0 parameters.
	*/
#define ENQUEUE_UNIQUE_RENDER_COMMAND(TypeName,Code) \
class TypeName : public FRenderCommand \
{ \
public: \
	virtual nfd::Uint Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return ( #TypeName ); \
	} \
}; \
ENQUEUE_RENDER_COMMAND(TypeName,);

	/**
	* Declares a rendering command type with 1 parameters.
	*/
#define ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,ParamValue1,Code) \
class TypeName : public FRenderCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	TypeName(const _ParamType1& In##ParamName1): \
	ParamName1(In##ParamName1) \
	{} \
	virtual nfd::Uint Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return ( #TypeName ); \
	} \
private: \
	ParamType1 ParamName1; \
}; \
ENQUEUE_RENDER_COMMAND(TypeName,(ParamValue1));

	/**
	* Declares a rendering command type which is expressed by lambda.
	*/
#define ENQUEUE_LAMBDA_RENDER_COMMAND(TypeName, lambda) \
do \
{ \
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(TypeName, decltype(lambda), lambda, lambda, { lambda(); }); \
} while(0)

	/**
	* Declares a rendering command type which is expressed by lambda.
	*/
#define EXECUTE_LOADER_TASK_ON_RENDER_THREAD_ASYNC(TypeName, Lambda) \
do { \
	class TypeName : public ILoaderGpuTask \
	{ \
	public: \
		TypeName(decltype(Lambda) _Lambda): Code(_Lambda) {} \
		virtual void Execute() \
		{ \
			Code(); \
		} \
	private: \
		decltype(Lambda) Code; \
	}; \
	TypeName *task = new TypeName(Lambda); \
	RenderingThread::AddLoaderGpuResTask(task); \
	task->Release(); \
} while(0)

	/**
	* Declares a rendering command type which is expressed by lambda.
	*/
#define EXECUTE_LOADER_TASK_ON_RENDER_THREAD_SYNC(TypeName, Lambda) \
do { \
	class TypeName : public ILoaderGpuTask \
	{ \
	public: \
		TypeName(decltype(Lambda) __Lambda): _Lambda(__Lambda) {} \
		virtual void Execute() \
		{ \
			_Lambda(); \
			_Event.Signal(); \
		} \
		void Wait() \
		{ \
			_Event.Wait(); \
		} \
	private: \
		decltype(Lambda) _Lambda; \
		neox::Event _Event; \
	}; \
	TypeName *task = new TypeName(Lambda); \
	RenderingThread::AddLoaderGpuResTask(task); \
	task->Wait(); \
	task->Release(); \
} while(0)

	/**
	* Declares a rendering command type which is expressed by lambda.
	* This macro could be used in game thread or rendering thread.
	*/
#define EXECUTE_LAMBDA_ON_RENDER_THREAD(TypeName, Lambda, Blocking) \
do \
{ \
	MILK_ASSERT(!RenderingThread::s_is_threaded_rendering || IsInGameThread || IsInRenderingThread || IsInLoaderThread); \
	if (RenderingThread::s_is_threaded_rendering) \
	{ \
		if (IsInGameThread) \
		{ \
			ENQUEUE_LAMBDA_RENDER_COMMAND(TypeName, Lambda); \
			if (Blocking) \
			{ \
				RenderingThread::FlushRenderingCommands(); \
			} \
		} \
		else if (IsInRenderingThread) \
		{ \
			Lambda(); \
		} \
		else if (IsInLoaderThread) \
		{ \
			if (Blocking) \
			{ \
				EXECUTE_LOADER_TASK_ON_RENDER_THREAD_SYNC(TypeName, Lambda); \
			} \
			else \
			{ \
				EXECUTE_LOADER_TASK_ON_RENDER_THREAD_ASYNC(TypeName, Lambda); \
			} \
		} \
	} \
	else \
	{ \
		Lambda(); \
	} \
} while(0)

	/**
	* Declares a rendering command type with 2 parameters.
	*/
#define ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,ParamValue1,ParamType2,ParamName2,ParamValue2,Code) \
class TypeName : public FRenderCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	typedef ParamType2 _ParamType2; \
	TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2): \
	ParamName1(In##ParamName1), \
	ParamName2(In##ParamName2) \
	{} \
	virtual nfd::Uint Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return ( #TypeName ); \
	} \
private: \
	ParamType1 ParamName1; \
	ParamType2 ParamName2; \
}; \
ENQUEUE_RENDER_COMMAND(TypeName,(ParamValue1,ParamValue2));

	/**
	* Declares a rendering command type with 3 parameters.
	*/
#define ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,ParamValue1,ParamType2,ParamName2,ParamValue2,ParamType3,ParamName3,ParamValue3,Code) \
class TypeName : public FRenderCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	typedef ParamType2 _ParamType2; \
	typedef ParamType3 _ParamType3; \
	TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2,const _ParamType3& In##ParamName3): \
	ParamName1(In##ParamName1), \
	ParamName2(In##ParamName2), \
	ParamName3(In##ParamName3) \
	{} \
	virtual nfd::Uint Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return ( #TypeName ); \
	} \
private: \
	ParamType1 ParamName1; \
	ParamType2 ParamName2; \
	ParamType3 ParamName3; \
}; \
ENQUEUE_RENDER_COMMAND(TypeName,(ParamValue1,ParamValue2,ParamValue3));

	/**
	* Declares a rendering command type with 4 parameters.
	*/
#define ENQUEUE_UNIQUE_RENDER_COMMAND_FOURPARAMETER(TypeName,ParamType1,ParamName1,ParamValue1,ParamType2,ParamName2,ParamValue2,ParamType3,ParamName3,ParamValue3,ParamType4,ParamName4,ParamValue4,Code) \
class TypeName : public FRenderCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	typedef ParamType2 _ParamType2; \
	typedef ParamType3 _ParamType3; \
	typedef ParamType4 _ParamType4; \
	TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2,const _ParamType3& In##ParamName3,const _ParamType4& In##ParamName4): \
	ParamName1(In##ParamName1), \
	ParamName2(In##ParamName2), \
	ParamName3(In##ParamName3), \
	ParamName4(In##ParamName4) \
	{} \
	virtual nfd::Uint Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return ( #TypeName ); \
	} \
private: \
	ParamType1 ParamName1; \
	ParamType2 ParamName2; \
	ParamType3 ParamName3; \
	ParamType4 ParamName4; \
}; \
ENQUEUE_RENDER_COMMAND(TypeName,(ParamValue1,ParamValue2,ParamValue3,ParamValue4));

	/**
	* Declares a rendering command type with 5 parameters.
	*/
#define ENQUEUE_UNIQUE_RENDER_COMMAND_FIVEPARAMETER(TypeName,ParamType1,ParamName1,ParamValue1,ParamType2,ParamName2,ParamValue2,ParamType3,ParamName3,ParamValue3,ParamType4,ParamName4,ParamValue4,ParamType5,ParamName5,ParamValue5,Code) \
class TypeName : public FRenderCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	typedef ParamType2 _ParamType2; \
	typedef ParamType3 _ParamType3; \
	typedef ParamType4 _ParamType4; \
	typedef ParamType5 _ParamType5; \
	TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2,const _ParamType3& In##ParamName3,const _ParamType4& In##ParamName4,const _ParamType5& In##ParamName5): \
	ParamName1(In##ParamName1), \
	ParamName2(In##ParamName2), \
	ParamName3(In##ParamName3), \
	ParamName4(In##ParamName4), \
	ParamName5(In##ParamName5) \
	{} \
	virtual nfd::Uint Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return ( #TypeName ); \
	} \
private: \
	ParamType1 ParamName1; \
	ParamType2 ParamName2; \
	ParamType3 ParamName3; \
	ParamType4 ParamName4; \
	ParamType5 ParamName5; \
}; \
ENQUEUE_RENDER_COMMAND(TypeName,(ParamValue1,ParamValue2,ParamValue3,ParamValue4,ParamValue5));

	/**
	* Declares a rendering command type with 6 parameters.
	*/
#define ENQUEUE_UNIQUE_RENDER_COMMAND_SIXPARAMETER(TypeName,ParamType1,ParamName1,ParamValue1,ParamType2,ParamName2,ParamValue2,ParamType3,ParamName3,ParamValue3,ParamType4,ParamName4,ParamValue4,ParamType5,ParamName5,ParamValue5,ParamType6,ParamName6,ParamValue6,Code) \
class TypeName : public FRenderCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	typedef ParamType2 _ParamType2; \
	typedef ParamType3 _ParamType3; \
	typedef ParamType4 _ParamType4; \
	typedef ParamType5 _ParamType5; \
	typedef ParamType6 _ParamType6; \
	TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2,const _ParamType3& In##ParamName3,const _ParamType4& In##ParamName4,const _ParamType5& In##ParamName5,const _ParamType6& In##ParamName6): \
	ParamName1(In##ParamName1), \
	ParamName2(In##ParamName2), \
	ParamName3(In##ParamName3), \
	ParamName4(In##ParamName4), \
	ParamName5(In##ParamName5), \
	ParamName6(In##ParamName6) \
	{} \
	virtual nfd::Uint Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return ( #TypeName ); \
	} \
private: \
	ParamType1 ParamName1; \
	ParamType2 ParamName2; \
	ParamType3 ParamName3; \
	ParamType4 ParamName4; \
	ParamType5 ParamName5; \
	ParamType6 ParamName6; \
}; \
ENQUEUE_RENDER_COMMAND(TypeName,(ParamValue1,ParamValue2,ParamValue3,ParamValue4,ParamValue5,ParamValue6));

	/**
	* Declares a rendering command type with 7 parameters.
	*/
#define ENQUEUE_UNIQUE_RENDER_COMMAND_SEVENPARAMETER(TypeName,ParamType1,ParamName1,ParamValue1,ParamType2,ParamName2,ParamValue2,ParamType3,ParamName3,ParamValue3,ParamType4,ParamName4,ParamValue4,ParamType5,ParamName5,ParamValue5,ParamType6,ParamName6,ParamValue6,ParamType7,ParamName7,ParamValue7,Code) \
class TypeName : public FRenderCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	typedef ParamType2 _ParamType2; \
	typedef ParamType3 _ParamType3; \
	typedef ParamType4 _ParamType4; \
	typedef ParamType5 _ParamType5; \
	typedef ParamType6 _ParamType6; \
	typedef ParamType7 _ParamType7; \
	TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2,const _ParamType3& In##ParamName3,const _ParamType4& In##ParamName4,const _ParamType5& In##ParamName5,const _ParamType6& In##ParamName6,const _ParamType7& In##ParamName7): \
	ParamName1(In##ParamName1), \
	ParamName2(In##ParamName2), \
	ParamName3(In##ParamName3), \
	ParamName4(In##ParamName4), \
	ParamName5(In##ParamName5), \
	ParamName6(In##ParamName6), \
	ParamName7(In##ParamName7) \
	{} \
	virtual nfd::Uint Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return ( #TypeName ); \
	} \
private: \
	ParamType1 ParamName1; \
	ParamType2 ParamName2; \
	ParamType3 ParamName3; \
	ParamType4 ParamName4; \
	ParamType5 ParamName5; \
	ParamType6 ParamName6; \
	ParamType7 ParamName7; \
}; \
ENQUEUE_RENDER_COMMAND(TypeName,(ParamValue1,ParamValue2,ParamValue3,ParamValue4,ParamValue5,ParamValue6,ParamValue7));

	/**
	* Declares a rendering command type with 8 parameters.
	*/
#define ENQUEUE_UNIQUE_RENDER_COMMAND_EIGHTPARAMETER(TypeName,ParamType1,ParamName1,ParamValue1,ParamType2,ParamName2,ParamValue2,ParamType3,ParamName3,ParamValue3,ParamType4,ParamName4,ParamValue4,ParamType5,ParamName5,ParamValue5,ParamType6,ParamName6,ParamValue6,ParamType7,ParamName7,ParamValue7,ParamType8,ParamName8,ParamValue8,Code) \
class TypeName : public FRenderCommand \
{ \
public: \
	typedef ParamType1 _ParamType1; \
	typedef ParamType2 _ParamType2; \
	typedef ParamType3 _ParamType3; \
	typedef ParamType4 _ParamType4; \
	typedef ParamType5 _ParamType5; \
	typedef ParamType6 _ParamType6; \
	typedef ParamType7 _ParamType7; \
	typedef ParamType8 _ParamType8; \
	TypeName(const _ParamType1& In##ParamName1,const _ParamType2& In##ParamName2,const _ParamType3& In##ParamName3,const _ParamType4& In##ParamName4,const _ParamType5& In##ParamName5,const _ParamType6& In##ParamName6,const _ParamType7& In##ParamName7,const _ParamType8& In##ParamName8): \
	ParamName1(In##ParamName1), \
	ParamName2(In##ParamName2), \
	ParamName3(In##ParamName3), \
	ParamName4(In##ParamName4), \
	ParamName5(In##ParamName5), \
	ParamName6(In##ParamName6), \
	ParamName7(In##ParamName7), \
	ParamName8(In##ParamName8) \
	{} \
	virtual nfd::Uint Execute() \
	{ \
		Code; \
		return sizeof(*this); \
	} \
	virtual const char* DescribeCommand() \
	{ \
		return ( #TypeName ); \
	} \
private: \
	ParamType1 ParamName1; \
	ParamType2 ParamName2; \
	ParamType3 ParamName3; \
	ParamType4 ParamName4; \
	ParamType5 ParamName5; \
	ParamType6 ParamName6; \
	ParamType7 ParamName7; \
	ParamType8 ParamName8; \
}; \
ENQUEUE_RENDER_COMMAND(TypeName,(ParamValue1,ParamValue2,ParamValue3,ParamValue4,ParamValue5,ParamValue6,ParamValue7,ParamValue8));

}