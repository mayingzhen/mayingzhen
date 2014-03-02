#ifndef  _IAnimationSet__H__
#define  _IAnimationSet__H__


namespace ma
{
	class IAction;

	class ENGINE_API IAnimationSet 
	{
	public:
		virtual	IAction*	CreateAction(const char* actionName) = 0;

		virtual	void		AddAnimClip(const char* pszSkaPath, const char* actionName) = 0;

		virtual	IAction*	GetActionByName(const char* pszName) = 0;

		virtual	UINT		GetActionNumber() = 0;

		virtual	IAction*	GetActionByIndex(UINT index) = 0;

		virtual	void		RemoveAction(IAction* pAction) = 0;
	};
}

#endif // _IAnimationSet__H__
