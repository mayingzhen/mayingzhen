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

class ENGINE_API DeviceCapabilitie
{
public:
	DeviceCapabilitie(void);
	~DeviceCapabilitie(void);

	GPUVendor getVendor() const
	{
		return mVendor;
	}

	void setVendor(GPUVendor v)
	{
		mVendor = v;
	}

	DriverVersion getDriverVersion() const
	{
		return mDriverVersion;
	}

	/** Set the driver version. */
	void setDriverVersion(const DriverVersion& version)
	{
		mDriverVersion = version;
	}

	/// sets the device name for Render system
	void setDeviceName(const char* name);

	/// gets the device name for render system
	const char* getDeviceName() const
	{
		return mDeviceName.c_str();
	}

	/// Get the identifier of the rendersystem from which these capabilities were generated
	const char* getRenderSystemName(void) const
	{
		return mRenderSystemName.c_str();
	}
	///  Set the identifier of the rendersystem from which these capabilities were generated
	void setRenderSystemName(const char* rs);

	/// The number of simultaneous render targets supported
	uint8 getNumMultiRenderTargets(void) const{	return mNumMultiRenderTargets;}

	/// The number of simultaneous render targets supported
	void setNumMultiRenderTargets(uint8 num)
	{
		mNumMultiRenderTargets = num;
	}

	/// Maximum point screen size in pixels
	void setMaxPointSize(Real s)
	{
		mMaxPointSize = s;
	}
	/// Maximum point screen size in pixels
	Real getMaxPointSize(void) const
	{
		return mMaxPointSize;
	}

	void setFloatTexturesSupported(bool b){mFloatTexturesSupported = b;}
	bool getFloatTexturesSupported() const{return mFloatTexturesSupported;}

	/** Write the capabilities to the pass in Log */
	void log();

	void setMRTIndependentBitDepths(bool b){m_bMRTIndependentBitDepth = b;}
	bool getMRTIndependentBitDepths() const{return m_bMRTIndependentBitDepth;}

	void SetVSTextureSupported(bool b){m_bIsVSTextureSupported = b;}
	bool IsVSTextureSupported() const{return m_bIsVSTextureSupported;}

	void SetTextureDXTSupported(bool b){m_bDXT = b;}
	void SetTexturePVRTCSupported(bool b){m_bPVRTC = b;}
	void SetTextureETC1Supported(bool b){m_bETC1 = b;}
	bool GetTextureDXTSupported() const{return m_bDXT;}
	bool GetTexturePVRTCSupported() const{return m_bPVRTC;}
	bool GetTextureETC1Supported() const{return m_bETC1;}
	bool GetDepthTextureSupported() const {return m_bDepthTexture;}

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

ENGINE_API void SetDeviceCapabilities(DeviceCapabilitie* pDevicCap);
ENGINE_API DeviceCapabilitie* GetDeviceCapabilities();

}