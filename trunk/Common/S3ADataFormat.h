#ifndef __S3ADataFormat_H__
#define __S3ADataFormat_H__

struct S3AReferencePoseData
{
	std::vector<xmVector3>	m_arrScaleOS;
	std::vector<xmQuaternion> m_arrRotOS;
	std::vector<xmVector3>	m_arrPosOS;

	std::vector<xmVector3>	m_arrScalePS;
	std::vector<xmQuaternion> m_arrRotPS;
	std::vector<xmVector3>	m_arrPosPS;


};

struct S3ASockeData
{

	std::string		m_strName;
	xmUint			m_nBoneID;
	xmMatrix4x4	m_matTransformBS;//socket's transform related to bone
};


//Skeleton Data
struct S3ASkeletonData
{
	S3AGUID						m_nGlobalSkeletonID;//used to verify if skeleton/animation skeleton/mesh match
	xmUint						m_nBoneNum;	
	S3AReferencePoseData		m_refPose;

	S3AArray<xmMatrix4x4>		m_arrRefPosePS;
	S3AArray<xmMatrix4x4>		m_arrRefPoseOS;
	S3AArray<xmMatrix4x4>		m_arrRefPoseOSInv; // in object space
	std::vector<xmUint>			m_vecParentIndice;
	std::vector<std::string>	m_boneName;
	std::vector<S3ASocket*>		m_vecSocket;
}

//Animation Data
struct xmVector3TrackData
{
	std::vector<xmVector3>	m_key;
	std::vector<int>			m_frame; //frame number of each key
};

struct xmQuaternionTrackData
{
	std::vector<xmQuaternion>	m_key;
	std::vector<int>			m_frame; //frame number of each key
};

struct S3AAnimationData
{
	std::string								m_name;
	S3AGUID									m_nGlobalSkeletonID;
	std::vector<xmVector3TrackData*>		m_scale;
	std::vector<xmQuaternionTrackData*>	m_rot;
	std::vector<xmVector3TrackData*>		m_pos;
	int										m_nFrameNumber;
	bool									m_nFrameNumberDirty;
};


#endif// __S3ADataFormat_H__