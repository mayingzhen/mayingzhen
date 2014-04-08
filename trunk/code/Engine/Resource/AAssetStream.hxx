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

	UINT AAssetStream::Read( IN OUT void* pBuffer, UINT nCount )
	{
		int result = AAsset_read(m_pAAsset, pBuffer, nCount);
		return result;
	}

	UINT AAssetStream::Write( const void* pBuffer, UINT nCount )
	{
		ASSERT(false);
		return 0;
	}

	void AAssetStream::Skip( int nCount )
	{
		AAsset_seek(m_pAAsset, nCount, SEEK_CUR);
	}

	void AAssetStream::Seek( UINT nPos )
	{
		AAsset_seek(m_pAAsset, nPos, SEEK_SET);
	}

	UINT AAssetStream::Tell() const
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