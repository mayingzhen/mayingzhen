#include "AAssetArchive.h"
#include "AAssetStream.h"

#include <android/log.h>
#include <android/asset_manager.h>


namespace ma
{
	extern AAssetManager* __assetManager;

	AAssetArchive::AAssetArchive(const char* pszName)
	: Archive(pszName)
	{
	}

	AAssetArchive::~AAssetArchive(void)
	{
	}

	bool AAssetArchive::isCaseSensitive( void ) const
	{
		return true;
	}

	Stream* AAssetArchive::open(const char* pszFile, bool readOnly /*= true*/ ) const
	{
		std::string full_path = pszFile;
		full_path = StringUtil::replaceAll(full_path, "\\", "/");
		full_path = mName + "/" + pszFile;
		StringUtil::toLowerCase(full_path);

		AAsset* asset = AAssetManager_open(__assetManager, full_path.c_str(), AASSET_MODE_RANDOM);
		if (asset)
		{
			//LogInfo("@@@open file:%s,%d", full_path.c_str(), AAsset_getLength(asset));
			RefPtr<AAssetStream> pAssettream = new AAssetStream(asset);
			return pAssettream;
		}
		else
		{
			ASSERT(false && "AAssetArchive::open");
			//LogError("@Connot open file: %s", full_path.c_str());
		}
		return NULL;
	}

	Stream* AAssetArchive::create(const char* pszFile) const
	{
		return NULL;
	}

	void AAssetArchive::remove(const char* pszFile) const
	{

	}

	void AAssetArchive::list( OUT VEC_STR& ret, bool recursive /*= true*/, bool dirs /*= false*/ )
	{

	}

	void AAssetArchive::listFileInfo( OUT VEC_FileInfo& vecFileInfo, bool recursive /*= true*/, bool dirs /*= false*/ )
	{

	}

	void AAssetArchive::find( OUT VEC_STR& ret, const std::string& pattern, bool recursive /*= true*/, bool dirs /*= false*/ )
	{

	}

	bool AAssetArchive::exists( const std::string& filename )
	{
		AAsset* asset = AAssetManager_open(__assetManager, filename.c_str(), AASSET_MODE_RANDOM);
		if (asset)
		{
			int length = AAsset_getLength(asset);
			AAsset_close(asset);
			return length > 0;
		}
	}

	time_t AAssetArchive::getModifiedTime( const std::string& filename )
	{
		return 0;
	}

	void AAssetArchive::findFileInfo( OUT VEC_FileInfo& vecFileInfo, const std::string& pattern, bool recursive /*= true*/, bool dirs /*= false*/ )
	{

	}

}
