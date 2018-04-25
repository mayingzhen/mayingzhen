#include "AAssetStream.h"
#include <android/asset_manager.h>


namespace ma
{
	AAssetStream::AAssetStream(AAsset* pAsset)
	{
		m_pAAsset = pAsset;
		m_nSize = AAsset_getLength(m_pAAsset);
	}

	AAssetStream::~AAssetStream(void)
	{
		this->Close();
	}

	uint32_t AAssetStream::Read( IN OUT void* pBuffer, uint32_t nCount )
	{
		int result = AAsset_read(m_pAAsset, pBuffer, nCount);
		return result;
	}

	uint32_t AAssetStream::Write( const void* pBuffer, uint32_t nCount )
	{
		ASSERT(false);
		return 0;
	}

	void AAssetStream::Skip( int nCount )
	{
		AAsset_seek(m_pAAsset, nCount, SEEK_CUR);
	}

	void AAssetStream::Seek( uint32_t nPos )
	{
		AAsset_seek(m_pAAsset, nPos, SEEK_SET);
	}

	uint32_t AAssetStream::Tell() const
	{
		return AAsset_getLength(m_pAAsset) - AAsset_getRemainingLength(m_pAAsset);
	}

	bool AAssetStream::Eof() const
	{
		return AAsset_getRemainingLength(m_pAAsset) <= 0;
	}

	void AAssetStream::Close()
	{
		if (m_pAAsset)
		{
			AAsset_close(m_pAAsset);
			m_pAAsset = NULL;
		}
	}

}