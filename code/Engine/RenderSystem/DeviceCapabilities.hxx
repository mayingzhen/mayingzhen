#include "DeviceCapabilities.h"

namespace ma
{
DeviceCapabilitie* g_pDeviceCapabilities = NULL;

DeviceCapabilitie* GetDeviceCapabilities()
{
	return g_pDeviceCapabilities;
}

DeviceCapabilitie::DeviceCapabilitie(void)
: mVendor(GPU_UNKNOWN)
, m_bDXT(true)
, m_bPVRTC(false)
, m_bETC1(false)
, m_bAnisotropy(true)
, m_ShadowMapDepthFormat(PF_D16F)
{

}

DeviceCapabilitie::~DeviceCapabilitie(void)
{
}

string DeviceCapabilitie::vendorToString(GPUVendor v)
{
	switch (v)
	{
	case GPU_UNKNOWN:
		return "unknown";
		break;
	case GPU_NVIDIA:
		return "nvidia";
		break;
	case GPU_ATI:
		return "ati";
		break;
	case GPU_INTEL:
		return "intel";
		break;
	case GPU_3DLABS:
		return "3dlabs";
		break;
	case GPU_S3:
		return "s3";
		break;
	case GPU_MATROX:
		return "matrox";
		break;
	case GPU_SIS:
		return "sis";
		break;
	case GPU_IMAGINATION_TECHNOLOGIES:
		return "imagination technologies";
		break;
	case GPU_APPLE:
		return  "apple";    // iPhone Simulator
    default:
        break;
	}

	ASSERT(false);
	return "";
}

void DeviceCapabilitie::log()
{
	LogInfo("RenderSystem capabilities");
	LogInfo("-------------------------");
	LogInfo(("RenderSystem Name: " + mRenderSystemName).c_str());
	LogInfo(("GPU Vendor: " + vendorToString(getVendor())).c_str());
	LogInfo(("Device Name: " + mDeviceName).c_str());
	LogInfo(("Driver Version: " + getDriverVersion().toString()).c_str());
	LogInfo((" * DDS: " + StringConverter::toString(m_bDXT)).c_str());
	LogInfo((" * PVRTC: " + StringConverter::toString(m_bPVRTC)).c_str());
	LogInfo((" * ETC1: " + StringConverter::toString(m_bETC1)).c_str());
}

void DeviceCapabilitie::SetDeviceName( const char* name )
{
    mDeviceName = name;
}

const char* DeviceCapabilitie::GetDeviceName() const
{
	return mDeviceName.c_str();
}

void DeviceCapabilitie::SetRenderSystemName( const char* rs )
{
    mRenderSystemName = rs;
}

const char* DeviceCapabilitie::GetRenderSystemName(void) const
{
	return mRenderSystemName.c_str();
}



}
