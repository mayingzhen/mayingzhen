#include "PoissonDiskGen.h"


namespace ma
{

	bool CPoissonDiskGen::m_bCached = false;
	int CPoissonDiskGen::m_numSamples = 0;
	Vector2* CPoissonDiskGen::m_pvSamples = NULL;

	CPoissonDiskGen::CPoissonDiskGen()
	{

	}

	CPoissonDiskGen::~CPoissonDiskGen()
	{
	}

	void CPoissonDiskGen::Realse()
	{
		SAFE_DELETE_ARRAY(m_pvSamples);
	}

	Vector2& CPoissonDiskGen::GetSample(int ind)
	{
		assert(ind<m_numSamples && ind>=0);

		return m_pvSamples[ind];
	}

	void CPoissonDiskGen::SetKernelSize(int num)
	{
		if (m_numSamples != num && num > 0)
		{
			m_numSamples = num;

			SAFE_DELETE_ARRAY(m_pvSamples);

			m_pvSamples = new Vector2[m_numSamples];

			InitSamples();
		}

		return;
	}

	void CPoissonDiskGen::RandomPoint(Vector2& p)
	{
		//generate random point inside circle
		do 
		{
			p.x = Math::UnitRandom()- 0.5f;
			p.y = Math::UnitRandom()- 0.5f;
		} 
		while (p.x*p.x + p.y*p.y > 0.25f);

		return;
	}


	void CPoissonDiskGen::InitSamples()
	{
		const int nQ = 1000;

		RandomPoint(m_pvSamples[0]);

		for (int i = 1; i < m_numSamples; i++)
		{
			float dmax = -1.0;

			for (int c = 0; c < i * nQ; c++)
			{
				Vector2 curSample;
				RandomPoint (curSample);

				float dc = 2.0;
				for (int j = 0; j < i; j++)
				{
					float dj =
						(m_pvSamples[j].x - curSample.x) * (m_pvSamples[j].x - curSample.x) +
						(m_pvSamples[j].y - curSample.y) * (m_pvSamples[j].y - curSample.y);
					if (dc > dj)
						dc = dj;
				}

				if (dc > dmax) 
				{
					m_pvSamples[i] = curSample;
					dmax = dc;
				}
			}
		}

		for (int i = 0; i < m_numSamples; i++)
		{
			m_pvSamples[i] *=2.0f;
			//CryLogAlways("Sample %i: (%.6f, %.6f)\n", i, m_pvSamples[i].x, m_pvSamples[i].y);
		}

		return;
	}
}
