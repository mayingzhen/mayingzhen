#pragma once

namespace ma
{

class WwiseListener : public Component
{
public:
	WwiseListener(int nIndex);
	~WwiseListener(void);

	virtual void Update();

	void SetAutoUpdate(bool bAuto){m_bAutoUpdate = bAuto;}
	bool GetAutoUpdate() const{return m_bAutoUpdate;}

	void ManualUpdate(const Vector3& vPos, const Vector3& vForward, const Vector3& vUp);

private:
	int m_nIndex;
	bool m_bAutoUpdate;
};

}