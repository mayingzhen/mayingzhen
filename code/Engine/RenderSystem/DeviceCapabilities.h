#ifndef __DeviceCapabilitie_H__
#define __DeviceCapabilitie_H__

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

	/** Write the capabilities to the pass in Log */
	void log();

	void SetTextureDXTSupported(bool b){m_bDXT = b;}
	void SetTexturePVRTCSupported(bool b){m_bPVRTC = b;}
	void SetTextureETC1Supported(bool b){m_bETC1 = b;}
	bool GetTextureDXTSupported() const{return m_bDXT;}
	bool GetTexturePVRTCSupported() const{return m_bPVRTC;}
	bool GetTextureETC1Supported() const{return m_bETC1;}

	void SetShadowMapDepthFormat(PixelFormat format) {m_ShadowMapDepthFormat = format;}
	PixelFormat GetShadowMapDepthFormat() const {return m_ShadowMapDepthFormat;}

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

	bool m_bDXT = true;
	bool m_bPVRTC = true;
	bool m_bETC1 = true;

	bool m_bAnisotropy;

	PixelFormat m_ShadowMapDepthFormat;
};

extern DeviceCapabilitie* g_pDeviceCapabilities;
DeviceCapabilitie* GetDeviceCapabilities();

}

#endif