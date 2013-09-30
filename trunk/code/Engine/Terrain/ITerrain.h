#ifndef _ITerrain_H__
#define _ITerrain_H__


namespace ma
{
	class ENGINE_API ITerrain
	{
	public:
		virtual void	Create(const char* pPath) = 0;
		
		virtual void	Update() = 0;
	};

	ENGINE_API ITerrain*	GetTerrain();

	ENGINE_API	void		SetTerrain(ITerrain* pTerrain);
}



#endif // _ITerrainSystem_H__


