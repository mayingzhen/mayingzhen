#include "DeviceCapabilities.h"

namespace ma
{
static DeviceCapabilitie* g_pDeviceCapabilities = NULL;

void SetDeviceCapabilities(DeviceCapabilitie* pDevicCap)
{
	g_pDeviceCapabilities = pDevicCap;
}

DeviceCapabilitie* GetDeviceCapabilities()
{
	return g_pDeviceCapabilities;
}

DeviceCapabilitie::DeviceCapabilitie(void)
: mVendor(GPU_UNKNOWN)
, mNumMultiRenderTargets(1)
, mMaxPointSize(2.19902e+012f)
, mFloatTexturesSupported(false)
, m_bMRTIndependentBitDepth(false)
, m_bIsVSTextureSupported(false)
, m_bDXT(true)
, m_bPVRTC(false)
, m_bETC1(false)
, m_bAnisotropy(true)
,m_bDepthTexture(false)
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

	LogInfo((string(" * Floating point textures: ")	+ (mFloatTexturesSupported?"supported":"not supported")).c_str());
	LogInfo((" * Multiple Render Targets: "+ StringConverter::toString(mNumMultiRenderTargets)).c_str());
	LogInfo((" * Max Point Size: "+ StringConverter::toString(mMaxPointSize)).c_str());
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