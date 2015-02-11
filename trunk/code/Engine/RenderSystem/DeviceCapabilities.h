#pragma once

namespace ma
{

/// DriverVersion is used by RenderSystemCapabilities and both GL and D3D9
/// to store the version of the current GPU driver
struct  DriverVersion 
{
	int major;
	int minor;
	int release;
	int build;

	DriverVersion() 
	{
		major = minor = release = build = 0;
	}

	string toString() const 
	{
		char sz[128];
		sprintf(sz, "%d.%d.%d.%04d", major, minor, release, build);
		return sz;
	}

	void fromString(const char* versionString)
	{
		vector<string> tokens = StringUtil::split(versionString, ".");
		if(!tokens.empty())
		{
			major = StringConverter::parseInt(tokens[0]);
			if (tokens.size() > 1)
				minor = StringConverter::parseInt(tokens[1]);
			if (tokens.size() > 2)
				release = StringConverter::parseInt(tokens[2]);
			if (tokens.size() > 3)
				build = StringConverter::parseInt(tokens[3]);
		}

	}
};

/** Enumeration of GPU vendors. */
enum GPUVendor
{
	GPU_UNKNOWN = 0,
	GPU_NVIDIA = 1,
	GPU_ATI = 2, 
	GPU_INTEL = 3,
	GPU_S3 = 4,
	GPU_MATROX = 5,
	GPU_3DLABS = 6,
	GPU_SIS = 7,
	GPU_IMAGINATION_TECHNOLOGIES = 8,
	GPU_APPLE = 9,  // Apple Software Renderer
	GPU_NOKIA = 10,
	GPU_MS_SOFTWARE = 11, // Microsoft software device
	GPU_MS_WARP = 12, // Microsoft WARP (Windows Advanced Rasterization Platform) software device - http://msdn.microsoft.com/en-us/library/dd285359.aspx

	/// placeholder
	GPU_VENDOR_COUNT = 13
};

class DeviceCapabilitie
{
public:
	DeviceCapabilitie(void);
	~DeviceCapabilitie(void);

	GPUVendor getVendor() const {return mVendor;}
	void SetVendor(GPUVendor v){mVendor = v;}

	DriverVersion getDriverVersion() const{return mDriverVersion;}
	void SetDriverVersion(const DriverVersion& version){mDriverVersion = version;}

	/// device name for Render system
	void SetDeviceName(const char* name);
	const char* GetDeviceName() const;

	/// identifier of the Rendersystem from which these capabilities were generated
	const char* GetRenderSystemName(void) const;
	void SetRenderSystemName(const char* rs);

	/// The number of simultaneous render targets supported
	uint8 GetNumMultiRenderTargets(void) const{	return mNumMultiRenderTargets;}
	void SetNumMultiRenderTargets(uint8 num){mNumMultiRenderTargets = num;}

	/// Maximum point screen size in pixels
	void SetMaxPointSize(Real s){mMaxPointSize = s;} 
	Real GetMaxPointSize(void) const{return mMaxPointSize;}

	void SetFloatTexturesSupported(bool b){mFloatTexturesSupported = b;}
	bool GetFloatTexturesSupported() const{return mFloatTexturesSupported;}

	/** Write the capabilities to the pass in Log */
	void log();

	void SetMRTIndependentBitDepths(bool b){m_bMRTIndependentBitDepth = b;}
	bool GetMRTIndependentBitDepths() const{return m_bMRTIndependentBitDepth;}

	void SetVSTextureSupported(bool b){m_bIsVSTextureSupported = b;}
	bool GetVSTextureSupported() const{return m_bIsVSTextureSupported;}

	void SetTextureDXTSupported(bool b){m_bDXT = b;}
	void SetTexturePVRTCSupported(bool b){m_bPVRTC = b;}
	void SetTextureETC1Supported(bool b){m_bETC1 = b;}
	bool GetTextureDXTSupported() const{return m_bDXT;}
	bool GetTexturePVRTCSupported() const{return m_bPVRTC;}
	bool GetTextureETC1Supported() const{return m_bETC1;}
	
	bool GetDepthTextureSupported() const {return m_bDepthTexture;}
	void SetDepthTextureSupported(bool b) {m_bDepthTexture = b;}

	void SetTextureAnisotropy(bool b){m_bAnisotropy = b;}
	bool GetTextureAnisotropy() const{return m_bAnisotropy;}
private:
	string vendorToString(GPUVendor v);

private:
	/// This is used to build a database of RSC's
	/// if a RSC with same name, but newer version is introduced, the older one 
	/// will be removed
	DriverVersion mDriverVersion;
	/// GPU Vendor
	GPUVendor mVendor;
	string mDeviceName;
	string mRenderSystemName;

	uint8 mNumMultiRenderTargets;
	Real mMaxPointSize;
	bool mFloatTexturesSupported;
	bool m_bMRTIndependentBitDepth;

	bool m_bIsVSTextureSupported;
	bool m_bDXT;
	bool m_bPVRTC;
	bool m_bETC1;
	bool m_bAnisotropy;

	bool m_bDepthTexture;
};

 void SetDeviceCapabilities(DeviceCapabilitie* pDevicCap);
 DeviceCapabilitie* GetDeviceCapabilities();

}