#include "Animation/Animation.h"
#include "Animation/Track.h"
#include "Animation/Skeleton.h"
#include "AnimDataManager.h"

namespace ma
{
	Animation::Animation()
	{
		m_nFrameNumber = 0;	
		m_bCompress = false;
	}

	Animation::~Animation()
	{
	}

	void Animation::SampleSingleTrackByFrame(Transform* pTSF, BoneIndex nTrackID,float fFrame) const
	{
		profile_code();
		
		if (m_bCompress)
		{
			m_arrScaleTrack[nTrackID].SampleFrame(fFrame,pTSF->m_vScale);
			m_arrRotTrack[nTrackID].SampleFrame(fFrame,pTSF->m_qRot);
			m_arrPosTrack[nTrackID].SampleFrame(fFrame,pTSF->m_vPos);
		}
		else
		{
			uint32_t nFrame = (uint32_t)fFrame;
			uint32_t nNextFrame = nFrame <= m_nFrameNumber - 2 ? nFrame + 1 : nFrame;
			float fFactor =  fFrame - nFrame;

			pTSF->m_vScale = Math::Lerp(m_arrScaleTrack[nTrackID].m_arrValue[nFrame], m_arrScaleTrack[nTrackID].m_arrValue[nNextFrame],fFactor);
			pTSF->m_qRot = Quaternion::Slerp(fFactor, m_arrRotTrack[nTrackID].m_arrValue[nFrame], m_arrRotTrack[nTrackID].m_arrValue[nNextFrame],true);
			pTSF->m_vPos = Math::Lerp(m_arrPosTrack[nTrackID].m_arrValue[nFrame], m_arrPosTrack[nTrackID].m_arrValue[nNextFrame],fFactor);
		}

	}

	uint32_t  Animation::GetTrackNumber() const
	{
		return m_arrTrackName.size();
	}

	const char*	 Animation::GetTrackNameByIndex(uint32_t index) const
	{
		return m_arrTrackName[index].c_str();
	}

	uint32_t Animation::GetTrackIndexByName(const char* pszName) const
	{
		if (pszName == NULL)
			return Math::InvalidID<BoneIndex>();

		for (uint32_t i = 0; i < m_arrTrackName.size(); ++i)
		{
			const char* pszTrackName = m_arrTrackName[i].c_str();
			if ( _stricmp(pszTrackName,pszName) == 0)
			{
				return i;
			}
		}

		return Math::InvalidID<BoneIndex>();
	}

	void Animation::AddTrack(const char* pName,const Vector3Track* pScale,const QuaternionTrack* pRot,const Vector3Track* pPos)
	{
		m_arrTrackName.push_back(pName);

		m_arrScaleTrack.push_back(*pScale);
		m_arrRotTrack.push_back(*pRot);
		m_arrPosTrack.push_back(*pPos);

		uint32_t nFrame = pPos->GetFrameNumber();
		m_nFrameNumber = m_nFrameNumber < nFrame ? nFrame : m_nFrameNumber;
	}

	bool Animation::Load(const char* pszFile, const char* pszSkeleton)
	{
		if (pszSkeleton && strlen(pszSkeleton) > 0)
		{
			m_pSkeleton = CreateSkeleton(pszSkeleton);
		}

		return Resource::Load(pszFile);
	}

	bool Animation::SaveToFile(const char* pszFile)
	{
		RefPtr<FileStream> pSaveStream = CreateFileStream(pszFile);

		uint32_t nIden = 'MANM';
		uint32_t nVersion = 0;

		pSaveStream->WriteUInt(nIden);
		pSaveStream->WriteUInt(nVersion);

		pSaveStream->WriteUInt(m_nFrameNumber);

		pSaveStream->WriteUInt(m_arrTrackName.size());
		for (uint32_t i = 0; i < m_arrTrackName.size(); ++i)
		{
			pSaveStream->WriteString(m_arrTrackName[i]);
		}

		for (uint32_t i = 0; i < m_arrTrackName.size(); ++i)
		{
			uint32_t nScaleFrame = m_arrScaleTrack[i].m_arrFrame.size();
			pSaveStream->WriteUInt(nScaleFrame);
			pSaveStream->Write(&m_arrScaleTrack[i].m_arrFrame[0],sizeof(uint32_t) * nScaleFrame);
			pSaveStream->Write(&m_arrScaleTrack[i].m_arrValue[0],sizeof(Vector3) * nScaleFrame);

			uint32_t nRotFrame = m_arrRotTrack[i].m_arrFrame.size();
			pSaveStream->WriteUInt(nRotFrame);
			pSaveStream->Write(&m_arrRotTrack[i].m_arrFrame[0],sizeof(uint32_t) * nRotFrame);
			pSaveStream->Write(&m_arrRotTrack[i].m_arrValue[0],sizeof(Quaternion) * nRotFrame);

			uint32_t nPosFrame = m_arrPosTrack[i].m_arrFrame.size();
			pSaveStream->WriteUInt(nPosFrame);
			pSaveStream->Write(&m_arrPosTrack[i].m_arrFrame[0],sizeof(uint32_t) * nPosFrame);
			pSaveStream->Write(&m_arrPosTrack[i].m_arrValue[0],sizeof(Vector3) * nPosFrame);
		}

		return true;
	}

	bool Animation::InitRes()
	{
		uint32_t nIden = m_pDataStream->ReadUInt();

		if (nIden == 'SANM')
		{
			ReadDataV0();
		}
		else if (nIden == 'MANM')
		{
			ReadDataV1();
		}

		return true;
	}

	void Animation::ReadDataV1()
	{
		//uint32_t nIden = m_pDataStream->ReadUInt();
		uint32_t nVersion = m_pDataStream->ReadUInt();
		
		m_nFrameNumber = m_pDataStream->ReadUInt();

		uint32_t nTrackNameNum = m_pDataStream->ReadUInt();
		m_arrTrackName.resize(nTrackNameNum);
		m_arrPosTrack.resize(nTrackNameNum);
		m_arrRotTrack.resize(nTrackNameNum);
		m_arrScaleTrack.resize(nTrackNameNum);

		for (uint32_t i = 0; i < nTrackNameNum; ++i)
		{
			m_arrTrackName[i] = m_pDataStream->ReadString();
		}
		 
		m_bCompress = false;
		for (uint32_t i = 0; i < nTrackNameNum; ++i)
		{
			uint32_t nScaleFrame = m_pDataStream->ReadUInt();	
			m_arrScaleTrack[i].m_arrFrame.resize(nScaleFrame);
			m_arrScaleTrack[i].m_arrValue.resize(nScaleFrame);

			m_bCompress = nScaleFrame != m_nFrameNumber;

			m_pDataStream->Read(&m_arrScaleTrack[i].m_arrFrame[0],sizeof(uint32_t) * nScaleFrame);
			m_pDataStream->Read(&m_arrScaleTrack[i].m_arrValue[0],sizeof(Vector3) * nScaleFrame);

			uint32_t nRotFrame = m_pDataStream->ReadUInt();
			m_arrRotTrack[i].m_arrFrame.resize(nRotFrame);
			m_arrRotTrack[i].m_arrValue.resize(nRotFrame);

			m_bCompress = nRotFrame != m_nFrameNumber;

			m_pDataStream->Read(&m_arrRotTrack[i].m_arrFrame[0],sizeof(uint32_t) * nRotFrame);
			m_pDataStream->Read(&m_arrRotTrack[i].m_arrValue[0],sizeof(Quaternion) * nRotFrame);

			uint32_t nPosFrame = m_pDataStream->ReadUInt();
			m_arrPosTrack[i].m_arrFrame.resize(nPosFrame);
			m_arrPosTrack[i].m_arrValue.resize(nPosFrame);

			m_bCompress = nPosFrame != m_nFrameNumber;

			m_pDataStream->Read(&m_arrPosTrack[i].m_arrFrame[0],sizeof(uint32_t) * nPosFrame);
			m_pDataStream->Read(&m_arrPosTrack[i].m_arrValue[0],sizeof(Vector3) * nPosFrame);
		}

		ConverteAnimDataLocalToParentSpace(m_pSkeleton.get());
	}

	void Animation::ReadDataV0()
	{
		//uint32_t nIden = m_pDataStream->ReadUInt();
		uint32_t nVersion = m_pDataStream->ReadUInt();
		uint32_t nStringLen = m_pDataStream->ReadUInt();
		vector<char> vecChar;
		vecChar.resize(nStringLen);
		m_pDataStream->Read(&vecChar[0],nStringLen);	
		string strMaxFile = &vecChar[0];
		//if (nIden != 'S3MD')
		//	return ;

		char GID[16];
		m_pDataStream->Read(GID,16);

		m_nFrameNumber = m_pDataStream->ReadUInt();
		uint32_t nBoneNum = m_pDataStream->ReadUInt();
		uint32_t nSocketNum = m_pDataStream->ReadUInt();
		
		if (nVersion == 2)
		{
			uint32_t nParentIDNum = m_pDataStream->ReadUInt();
			vector<BoneIndex> arrParentIndice;
			arrParentIndice.resize(nParentIDNum);
			for (uint32_t i = 0; i < nParentIDNum; ++i)
			{
				arrParentIndice[i] = m_pDataStream->ReadUShort();
			}
		}

		if (nVersion >= 3)
		{
			bool bCompressed = m_pDataStream->ReadBool();
			float fCompDiffThreshold = m_pDataStream->ReadFloat();
			float fCompVarianceThreshold = m_pDataStream->ReadFloat();
		}

		uint32_t nSacleTrackNum = m_pDataStream->ReadUInt();
		m_arrScaleTrack.resize(nSacleTrackNum);
		for (uint32_t i = 0; i < nSacleTrackNum; ++i)
		{
			uint32_t nFrame = m_pDataStream->ReadUInt();
			if (nFrame != m_nFrameNumber)
				m_bCompress = true;

			m_arrScaleTrack[i].m_arrFrame.resize(nFrame);
			for (uint32_t iFrame = 0; iFrame < nFrame; ++iFrame)
			{
				m_arrScaleTrack[i].m_arrFrame[iFrame] = m_pDataStream->ReadUInt();
			}

			uint32_t nKey = m_pDataStream->ReadUInt();
			m_arrScaleTrack[i].m_arrValue.resize(nKey);
			for (uint32_t iKey = 0; iKey < nKey; ++iKey)
			{
				m_arrScaleTrack[i].m_arrValue[iKey] = m_pDataStream->ReadVector3();
			}
		}

		uint32_t nRotTrackNum = m_pDataStream->ReadUInt();
		m_arrRotTrack.resize(nRotTrackNum);
		for (uint32_t i = 0; i < nRotTrackNum; ++i)
		{
			uint32_t nFrame = m_pDataStream->ReadUInt();
			if (nFrame != m_nFrameNumber)
				m_bCompress = true;

			m_arrRotTrack[i].m_arrFrame.resize(nFrame);
			for (uint32_t iFrame = 0; iFrame < nFrame; ++iFrame)
			{
				m_arrRotTrack[i].m_arrFrame[iFrame] = m_pDataStream->ReadUInt();
			}

			uint32_t nKey = m_pDataStream->ReadUInt();
			m_arrRotTrack[i].m_arrValue.resize(nKey);
			for (uint32_t iKey = 0; iKey < nKey; ++iKey)
			{
				m_arrRotTrack[i].m_arrValue[iKey] = m_pDataStream->ReadQuaternion();
			}
		}
	

		uint32_t nPosTrackNum = m_pDataStream->ReadUInt();
		m_arrPosTrack.resize(nPosTrackNum);
		for (uint32_t i = 0; i < nPosTrackNum; ++i)
		{
			uint32_t nFrame = m_pDataStream->ReadUInt();
			if (nFrame != m_nFrameNumber)
				m_bCompress = true;

			m_arrPosTrack[i].m_arrFrame.resize(nFrame);
			for (uint32_t iFrame = 0; iFrame < nFrame; ++iFrame)
			{
				m_arrPosTrack[i].m_arrFrame[iFrame] = m_pDataStream->ReadUInt();
			}

			uint32_t nKey = m_pDataStream->ReadUInt();
			m_arrPosTrack[i].m_arrValue.resize(nKey);
			for (uint32_t iKey = 0; iKey < nKey; ++iKey)
			{
				m_arrPosTrack[i].m_arrValue[iKey] = m_pDataStream->ReadVector3();
			}
		}

		uint32_t nTrackNameNum = m_pDataStream->ReadUInt();
		m_arrTrackName.resize(nTrackNameNum);
		for (uint32_t i = 0; i < nTrackNameNum; ++i)
		{
			uint32_t nStringLen = m_pDataStream->ReadUInt();
			vector<char> vecChar;
			vecChar.resize(nStringLen);
			m_pDataStream->Read(&vecChar[0],nStringLen);
			m_arrTrackName[i] = &vecChar[0];
		}

		if (nVersion == 3)
		{
			ConverteAnimDataLocalToParentSpace(m_pSkeleton.get());
		}
	}

	RefPtr<Animation> CreateAnimation()
	{
		return new Animation;
	}

	RefPtr<Animation> CreateAnimation(const char* pszFile,const char* pszSkeletonFile)
	{
		return g_pAnimDataManager->Open(pszFile,pszSkeletonFile);
	}
}
