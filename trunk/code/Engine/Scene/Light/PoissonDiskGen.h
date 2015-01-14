#pragma once

#include "ShadowMapFrustum.h"

namespace ma
{

class CPoissonDiskGen
{
	static bool		m_bCached;
	static int		m_numSamples;
	static Vector2*	m_pvSamples;

private:
	static void RandomPoint(Vector2& p);
	static void InitSamples();

public:
	static void SetKernelSize(int num);
	static Vector2& GetSample(int ind);

	static void Realse();

	CPoissonDiskGen();
	~CPoissonDiskGen();
};

}