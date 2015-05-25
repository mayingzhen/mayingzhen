#ifndef __CPoissonDiskGen_H__
#define __CPoissonDiskGen_H__

#include "ShadowMapFrustum.h"

namespace ma
{
	class PoissonDiskGen
	{
	public:
		PoissonDiskGen();
		~PoissonDiskGen();

		static void SetKernelSize(int num);
		static Vector2& GetSample(int ind);

		static void Realse();

	private:
		static void RandomPoint(Vector2& p);
		static void InitSamples();
	
	private:
		static bool		m_bCached;
		static int		m_numSamples;
		static Vector2*	m_pvSamples;

	};
}

#endif