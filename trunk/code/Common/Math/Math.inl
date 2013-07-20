#ifndef __S3Math_Inl__
#define __S3Math_Inl__



//------------------------------------------------------------------------------
//S3AGUID
//------------------------------------------------------------------------------
inline void maGUID::Clear()
{
	m_a = 0;
	m_b = 0;
}

inline bool maGUID::operator ==(const maGUID& rhs) const
{
	return (m_a == rhs.m_a) && (m_b == rhs.m_b);
}

inline bool maGUID::operator !=(const maGUID& rhs) const
{
	return !(*this == rhs);
}

inline bool maGUID::operator <(const maGUID& rhs) const
{
	return (m_a < rhs.m_a ? true 
		: (m_a == rhs.m_a ? m_b < rhs.m_b : false)
		);
}

inline void Vec3Mul(Vector3* pOut,const Vector3* pA,const Vector3* pB)
{
	pOut->x = pA->x * pB->x;
	pOut->y = pA->y * pB->y;
	pOut->z = pA->z * pB->z;
}

inline void QuaternionTransformVector(Vector3* pOut,const Vector3* pV,const Quaternion* pQuat)
{
	Vector3 uv,uuv;
	Vector3 qvec(pQuat->x,pQuat->y,pQuat->z);
	Vec3Cross(&uv,&qvec,pV);
	Vec3Cross(&uuv,&qvec,&uv);
	uv *= (2.0f * pQuat->w);
	uuv *= 2.0f;
	*pOut = *pV + uv + uuv;
}




inline void MatrixTransform(Matrix4x4* pOut,const Vector3* pScale,const Quaternion* pRot,const Vector3* pPos)
{
	Quaternion qRot;
	QuaternionNormalize(&qRot,pRot);
	MatrixTransformation(pOut,NULL,NULL
		,pScale,NULL
		,&qRot
		,pPos);
}

inline void BoneMatrixFromTransform( Matrix4x4* pMat,const NodeTransform* pTSF)
{
	Vector3 vScale(pTSF->m_fScale,pTSF->m_fScale,pTSF->m_fScale);
	MatrixTransform(pMat,&vScale,&pTSF->m_qRot,&pTSF->m_vPos);
}

inline void NodeMatrixFromTransform( Matrix4x4* pMat,const NodeTransform* pTSF)
{
	Vector3 vScale = pTSF->m_fScale * pTSF->m_vLocalScale;
	MatrixTransform(pMat,&vScale,&pTSF->m_qRot,&pTSF->m_vPos);
}

inline void TransformSetIdentity(NodeTransform* pTSF)
{
	memset(&pTSF->m_vPos,0,sizeof(Vector3));
	QuaternionIdentity(&pTSF->m_qRot);
	pTSF->m_vLocalScale = Vector3(1.0f,1.0f,1.0f);
	pTSF->m_fScale = 1.0f;
}

inline void TransformFromMatrix(NodeTransform* pTSF,const Matrix4x4* pMat)
{
	MatrixDecompose(&pTSF->m_vLocalScale,&pTSF->m_qRot,&pTSF->m_vPos,pMat);
	float fPropScale = (fabs(pTSF->m_vLocalScale.x) + fabs(pTSF->m_vLocalScale.y) + fabs(pTSF->m_vLocalScale.z))/3.0f;
	if (fPropScale > FEPS)
	{
		pTSF->m_fScale = fPropScale;
		pTSF->m_vLocalScale = pTSF->m_vLocalScale/fPropScale;
	}else{
		pTSF->m_fScale = 1.0f;
	}
}

inline void  TransformMul(NodeTransform* pOut,const NodeTransform* pTSFA,const NodeTransform* pTSFB)
{
	Vector3 vOPos = pTSFA->m_vPos*pTSFB->m_fScale;
	QuaternionTransformVector(&vOPos,&vOPos,&pTSFB->m_qRot);
	pOut->m_vPos = vOPos + pTSFB->m_vPos;

	QuaternionMultiply(&pOut->m_qRot,&pTSFA->m_qRot,&pTSFB->m_qRot);

	pOut->m_fScale = pTSFA->m_fScale * pTSFB->m_fScale; 

	//It is local scale use A
	pOut->m_vLocalScale = pTSFA->m_vLocalScale;

}

inline void  TransformMulLocalScale(NodeTransform* pOut,const NodeTransform* pTSFA,const NodeTransform* pTSFB)
{
	Vector3 vOPos = pTSFA->m_vPos*pTSFB->m_fScale;
	QuaternionTransformVector(&vOPos,&vOPos,&pTSFB->m_qRot);
	pOut->m_vPos = vOPos + pTSFB->m_vPos;

	QuaternionMultiply(&pOut->m_qRot,&pTSFA->m_qRot,&pTSFB->m_qRot);

	pOut->m_fScale = pTSFA->m_fScale * pTSFB->m_fScale; 

	Vec3Mul(&pOut->m_vLocalScale,&pTSFA->m_vLocalScale,&pTSFB->m_vLocalScale);

}

inline void  Vec3TransformNormal(Vector3* pOut,const Vector3* pV,const NodeTransform* pNode)
{
	QuaternionTransformVector(pOut,pV,&pNode->m_qRot);
}

inline void Lerp(Quaternion& out, const Quaternion& v0, const Quaternion& v1,float factor)
{
	//out = v0*(1.0f-factor)+v1*factor;
	QuaternionSlerp(&out,&v0,&v1,factor);
}

inline void Normalize(Vector3* pOut,const Vector3* pIn)
{
	Vec3Normalize(pOut,pIn);
}

inline void Normalize(Quaternion* pOut,const Quaternion* pIn)
{
	QuaternionNormalize(pOut,pIn);
}

inline void Normalize(float* pOut,const float* pIn)
{
	*pOut = *pIn > 0.0f ? 1.0f : -1.0f;
}

inline void  QuaternionLerp(Quaternion* out,const Quaternion* q0,const Quaternion* q1,float factor)
{
	if (QuaternionDot(q0,q1) > 0)
	{
		Lerp(out->x,q0->x,q1->x,factor);
		Lerp(out->y,q0->y,q1->y,factor);
		Lerp(out->z,q0->z,q1->z,factor);
		Lerp(out->w,q0->w,q1->w,factor);
	}else{
		Lerp(out->x,q0->x,-q1->x,factor);
		Lerp(out->y,q0->y,-q1->y,factor);
		Lerp(out->z,q0->z,-q1->z,factor);
		Lerp(out->w,q0->w,-q1->w,factor);
	}

}






inline bool IsNaN(Matrix4x4* pMat)
{
	for (Uint i = 0; i < 16; ++i)
	{
		if (IsNaN( ((float*)pMat)[i] ))
		{
			return true;
		}
	}
	return false;
}

inline void  Vec3Add(Vector3* pOut,const Vector3* pV,float fVal)
{
	pOut->x = pV->x + fVal;
	pOut->y = pV->y + fVal;
	pOut->z = pV->z + fVal;
}



inline Vector3 Vec3PositiveX()
{
	return Vector3(1.0f,0.0f,0.0f);
}


inline Vector3 Vec3PositiveY()
{
	return Vector3(0.0f,1.0f,0.0f);
}


inline Vector3 Vec3PositiveZ()
{
	return Vector3(0.0f,0.0f,1.0f);
}

inline void  Vec3Recip(Vector3* pOut,const Vector3* pIn)
{
	pOut->x = 1.0f/pIn->x;
	pOut->y = 1.0f/pIn->y;
	pOut->z = 1.0f/pIn->z;

}


inline Quaternion QuaternionIden()
{
	return Quaternion(0.0f,0.0f,0.0f,1.0f);
}


inline Matrix4x4 MatrixIdentity()
{
	Matrix4x4 mat;
	MatrixIdentity(&mat);
	return mat;
}


//#pragma message( __FILE__ "(" STRING(__LINE__) ") : TODO replace mad with pQ1*fWeight+pQ2")
inline void  QuaternionMad(Quaternion* pOut, const Quaternion* pQ1, const Quaternion* pQ2,float fWeight)
{
	if (QuaternionDot(pQ1,pQ2) >= 0.0f)
	{
		*pOut = *pQ1 + *pQ2*fWeight;
	}else{
		*pOut = *pQ1 - *pQ2*fWeight;
	}
}


inline void  QuaternionAdd(Quaternion* pOut, const Quaternion* pQ1, const Quaternion* pQ2)
{
	if (QuaternionDot(pQ1,pQ2) > 0.0f)
	{
		*pOut = *pQ1 + *pQ2;
	}else{
		*pOut = *pQ1 - *pQ2;
	}
}


inline void  TransformQuaternion(Quaternion* outQuat,const Quaternion* inQuat,const Matrix4x4* inMat)
{
	const Vector3 vAxisP(inQuat->x,inQuat->y,inQuat->z);
	Vector3 vAxisG;
	Vec3TransformNormal(&vAxisG,&vAxisP,inMat);
	outQuat->x = vAxisG.x;
	outQuat->y = vAxisG.y;
	outQuat->z = vAxisG.z;
	outQuat->w = inQuat->w;
}

inline void  MatrixTransform(Matrix4x4* outMat,const Quaternion* inRot,const Vector3* inPos)
{
	MatrixFromQuaternion(outMat,inRot);
	outMat->_41 = inPos->x;
	outMat->_42 = inPos->y;
	outMat->_43 = inPos->z;
}




inline void  MatrixFromFrame(Matrix4x4* pOut,const Frame* pFrame)
{
	Vector3 vScale(pFrame->m_fScale,pFrame->m_fScale,pFrame->m_fScale);
	MatrixTransformation(pOut,NULL,NULL
		,&vScale
		,NULL,&pFrame->m_qRot
		,&pFrame->m_vPos);
}

inline void S3AVecClampLength(Vector3* outVec,const Vector3* inVec,float fMaxLen)
{
	float fLen = Vec3Length(inVec);
	if (fLen > FEPS)
	{
		float fNewLen = fLen < fMaxLen ? fLen : fMaxLen;
		*outVec = *inVec * (fNewLen/fLen);
	}else{
		*outVec = *inVec;
	}

}


inline Vector3* MatrixAsTranslation3(Matrix4x4* pMat)
{
	return reinterpret_cast<Vector3*>(&pMat->_41);
}


inline const Vector3* MatrixAsTranslation3(const Matrix4x4* pMat)
{
	return reinterpret_cast<const Vector3*>(&pMat->_41);
}


inline void  Vec4SetPoint(Vector4* pVec4, const Vector3* pVec3)
{
	pVec4->x = pVec3->x;
	pVec4->y = pVec3->y;
	pVec4->z = pVec3->z;
	pVec4->w = 1.0f;
}

inline void  Vec4SetVector(Vector4* pVec4, const Vector3* pVec3)
{
	pVec4->x = pVec3->x;
	pVec4->y = pVec3->y;
	pVec4->z = pVec3->z;
	pVec4->w = 0.0f;
}

inline Vector3* MatrixAsVector3(Matrix4x4* pMat,Uint nCol)
{
	return reinterpret_cast<Vector3*>(&pMat->m[nCol][0]);
}

inline const Vector3* MatrixAsVector3(const Matrix4x4* pMat,Uint nCol)
{
	return reinterpret_cast<const Vector3*>(&pMat->m[nCol][0]);
}



inline Vector4* MatrixAsVector4(Matrix4x4* pMat,Uint nCol)
{
	return reinterpret_cast<Vector4*>(&pMat->m[nCol][0]);
}

inline const Vector4* MatrixAsVector4(const Matrix4x4* pMat,Uint nCol)
{
	return reinterpret_cast<const Vector4*>(&pMat->m[nCol][0]);
}



inline const Vector3* MatrixAsTranslation(const Matrix4x4* pMat)
{
	return reinterpret_cast<const Vector3*>(&pMat->_41);
}

inline Vector3* MatrixAsTranslation(Matrix4x4* pMat)
{
	return reinterpret_cast<Vector3*>(&pMat->_41);
}

inline bool FloatEqual(float fa,float fb,float eps)
{
	return fabs(fa-fb) < eps;
}

inline void  MatrixAxis(Matrix4x4* pMat,const Vector3* pX,const Vector3* pY, const Vector3* pZ)
{

	Vec4SetVector(MatrixAsVector4(pMat,0),pX);
	Vec4SetVector(MatrixAsVector4(pMat,1),pY);
	Vec4SetVector(MatrixAsVector4(pMat,2),pZ);
	*MatrixAsVector4(pMat,3) = Vector4(0.0f,0.0f,0.0f,1.0f);

}


//pAxisFrom, pAxisTo should be normalized
inline void  QuaternionFromAxisToAxis(Quaternion* pRot,const Vector3* pAxisFrom,const Vector3* pAxisTo)
{
	Vector3 vAxisRot;
	Vec3Cross(&vAxisRot,pAxisFrom,pAxisTo);
	float fSinAlpha = Vec3Length(&vAxisRot);
	float fCosAlpha = Vec3Dot(pAxisFrom,pAxisTo);
	float fAlpha = atan2f(fSinAlpha,fCosAlpha);
	if (FloatEqual(fSinAlpha,0.0f))
	{
		if (fCosAlpha < 0.0f)
		{
			Vector3 vAxisTangent(pAxisFrom->y,pAxisFrom->z,pAxisFrom->x);
			Vec3Cross(&vAxisRot,pAxisFrom,&vAxisTangent);
			fAlpha = PI;
		}else{
			*pRot = Quaternion(0.0f,0.0f,0.0f,1.0f);
			return;
		}
	}

	Vec3Normalize(&vAxisRot,&vAxisRot);
	QuaternionRotationAxis(pRot,&vAxisRot,fAlpha);

}

inline void  QuaternionFromAxisToAxis(Quaternion* pRot,const Vector3* pAxisFrom,const Vector3* pAxisTo,float fMaxAngle)
{
	Vector3 vAxisRot;
	Vec3Cross(&vAxisRot,pAxisFrom,pAxisTo);
	float fSinAlpha = Vec3Length(&vAxisRot);
	float fCosAlpha = Vec3Dot(pAxisFrom,pAxisTo);
	float fAlpha = atan2f(fSinAlpha,fCosAlpha);
	if (FloatEqual(fSinAlpha,0.0f))
	{
		if (fCosAlpha < 0.0f)
		{
			Vector3 vAxisTangent(pAxisFrom->y,pAxisFrom->z,pAxisFrom->x);
			Vec3Cross(&vAxisRot,pAxisFrom,&vAxisTangent);
			fAlpha = PI;
		}else{
			*pRot = Quaternion(0.0f,0.0f,0.0f,1.0f);
			return;
		}
	}

	Vec3Normalize(&vAxisRot,&vAxisRot);
	fAlpha = Clamp(fAlpha, -fMaxAngle,fMaxAngle);
	QuaternionRotationAxis(pRot,&vAxisRot,fAlpha);

}


inline void  QuaternionFromAxisToAxis(Quaternion* pRot,const Vector3* pAxisFrom,const Vector3* pAxisTo,const Vector3* pTangent)
{
	Vector3 vAxisRot;
	Vec3Cross(&vAxisRot,pAxisFrom,pAxisTo);
	float fSinAlpha = Vec3Length(&vAxisRot);
	float fCosAlpha = Vec3Dot(pAxisFrom,pAxisTo);
	float fAlpha = atan2f(fSinAlpha,fCosAlpha);
	if (FloatEqual(fSinAlpha,0.0f))
	{
		if (fCosAlpha < 0.0f)
		{
			vAxisRot=*pTangent;
			fAlpha = PI;
		}else{
			*pRot = Quaternion(0.0f,0.0f,0.0f,1.0f);
			return;
		}
	}

	Vec3Normalize(&vAxisRot,&vAxisRot);
	QuaternionRotationAxis(pRot,&vAxisRot,fAlpha);

}

inline void  QuaternionFromAxisToAxis(Vector3* pAxisRot,float* pAngle,const Vector3* pAxisFrom,const Vector3* pAxisTo)
{
	Vector3 vAxisRot;
	Vec3Cross(&vAxisRot,pAxisFrom,pAxisTo);
	float fSinAlpha = Vec3Length(&vAxisRot);
	float fCosAlpha = Vec3Dot(pAxisFrom,pAxisTo);
	float fAlpha = atan2f(fSinAlpha,fCosAlpha);
	if (FloatEqual(fSinAlpha,0.0f))
	{
		Vector3 vAxisTangent(pAxisFrom->y,pAxisFrom->z,pAxisFrom->x);
		Vec3Cross(&vAxisRot,pAxisFrom,&vAxisTangent);
		fAlpha = fCosAlpha < 0.0f ?  PI : 0.0f;
	}

	Vec3Normalize(pAxisRot,&vAxisRot);
	*pAngle = fAlpha;

}

inline void Frame::SetIdentity()
{
	m_vPos = Vec3Zero();
	QuaternionIdentity(&m_qRot);
	m_fScale = 1.0f;
}

inline void Frame::Zero()
{
	memset(&m_vPos,0,sizeof(m_vPos));
	memset(&m_qRot,0,sizeof(m_qRot));
	m_fScale = 0.0f;

}

inline void FrameTransformPoint(Vector3* pOut,const Vector3* pP,const Frame* pFrame)
{
	*pOut = *pP * pFrame->m_fScale;
	QuaternionTransformVector(pOut,pOut,& pFrame->m_qRot);
	*pOut = *pOut + pFrame->m_vPos;
}



inline void  Vec3Min(Vector3* pOut,const Vector3* pA,const Vector3* pB)
{
	pOut->x = pA->x < pB->x ? pA->x : pB->x;
	pOut->y = pA->y < pB->y ? pA->y : pB->y;
	pOut->z = pA->z < pB->z ? pA->z : pB->z;
}


inline void  Vec3Max(Vector3* pOut,const Vector3* pA,const Vector3* pB)
{
	pOut->x = pA->x > pB->x ? pA->x : pB->x;
	pOut->y = pA->y > pB->y ? pA->y : pB->y;
	pOut->z = pA->z > pB->z ? pA->z : pB->z;
}

inline void  Vec3Mad(Vector3* pV,const Vector3* pA,float f,const Vector3* pB)
{
	pV->x = pA->x *f + pB->x;
	pV->y = pA->y *f + pB->y;
	pV->z = pA->z *f + pB->z;
}




inline float NormalizeNodeScale(float fNodeScale,float fMinNodeScale = MIN_NODE_SCALE)
{
	return fNodeScale > 0.0f
		? (fNodeScale > MIN_NODE_SCALE ? fNodeScale : MIN_NODE_SCALE)
		: (fNodeScale < -MIN_NODE_SCALE ? fNodeScale : -MIN_NODE_SCALE)
		;
}


inline void  MatrixFromTransform(Matrix4x4* pMat,const RigidTransform* pTSF)
{
	MatrixTransform(pMat,NULL,&pTSF->m_qRot,&pTSF->m_vPos);
}

inline void  MatrixFromTransform(Matrix4x4* pMat,Vector3* pNodeScale,const RigidTransform* pTSF)
{
	MatrixTransform(pMat,pNodeScale,&pTSF->m_qRot,&pTSF->m_vPos);
}





inline void  TransformSetIdentity(RigidTransform* pRT)
{
	pRT->m_vPos = Vector3(0.0f,0.0f,0.0f);
	pRT->m_qRot = Quaternion(0.0f,0.0f,0.0f,1.0f);
}

inline void  TransformFromMatrix(RigidTransform* pRT,const Matrix4x4* pMat)
{
	Vector3 vScale;
	MatrixDecompose(&vScale,&pRT->m_qRot,&pRT->m_vPos,pMat);
}


inline void  TransformInverse(RigidTransform* pOut,const RigidTransform* pRT)
{
	Quaternion qRotInv;
	QuaternionInverse(&qRotInv,&pRT->m_qRot);
	QuaternionTransformVector(&pOut->m_vPos,&pRT->m_vPos,&qRotInv);
	pOut->m_vPos = - pOut->m_vPos;
	pOut->m_qRot = qRotInv;
}

inline void  TransformMul(RigidTransform* pRT,const RigidTransform* pRTA,const RigidTransform* pRTB)
{
	Vector3 vNewPos;

	QuaternionTransformVector(&vNewPos,&pRTA->m_vPos,&pRTB->m_qRot);
	pRT->m_vPos = vNewPos + pRTB->m_vPos;
	pRT->m_qRot = pRTA->m_qRot * pRTB->m_qRot;
}

inline void  TransformInvMul(RigidTransform* pOut,const RigidTransform* pTSFA,const RigidTransform* pTSFB)
{
	RigidTransform tsfAInv;
	TransformInverse(&tsfAInv,pTSFB);
	TransformMul(pOut,pTSFA,&tsfAInv);
}

inline void  Vec3TransformCoord(Vector3* pOut,const Vector3* pV,const RigidTransform* pRT)
{
	QuaternionTransformVector(pOut,pV,&pRT->m_qRot);
	*pOut = *pOut+pRT->m_vPos;
}

inline void  Vec3TransformCoord(Vector3* pOut,const Vector3* pV,const NodeTransform* pRT)
{
	Vector3 vPos;
	vPos = *pV * pRT->m_fScale;
	Vec3Mul(&vPos,&vPos,&pRT->m_vLocalScale);
	QuaternionTransformVector(pOut,&vPos,&pRT->m_qRot);
	*pOut = *pOut+pRT->m_vPos;
}

inline void  Vec3TransformNormal(Vector3* pOut,const Vector3* pV,const RigidTransform* pRT)
{
	QuaternionTransformVector(pOut,pV,&pRT->m_qRot);
}



inline void  TransformPoint(Vector3* pOut,const Vector3* pV,const RigidTransform* pTSF)
{
	QuaternionTransformVector(pOut,pV,&pTSF->m_qRot);
	*pOut += pTSF->m_vPos;
}

inline void  TransformVector(Vector3* pOut,const Vector3* pV,const RigidTransform* pTSF)
{
	QuaternionTransformVector(pOut,pV,&pTSF->m_qRot);
}

inline void Lerp(RigidTransform* pOut,const RigidTransform* pA,const RigidTransform* pB,float fFactor)
{
	Lerp(pOut->m_vPos,pA->m_vPos,pB->m_vPos,fFactor);
	Lerp(pOut->m_qRot,pA->m_qRot,pB->m_qRot,fFactor);
}






inline void  TransformMul(NodeTransform* pOut,const NodeTransform* pTSF,float fWeight)
{
	pOut->m_vPos = pTSF->m_vPos * fWeight;
	pOut->m_qRot = pTSF->m_qRot * fWeight;
	pOut->m_vLocalScale = pTSF->m_vLocalScale * fWeight;
	pOut->m_fScale = pTSF->m_fScale * fWeight;
}

inline void  TransformInverse(NodeTransform* pOut,const NodeTransform* pTSF)
{
	float fScaleInv;


	fScaleInv = pTSF->m_fScale > FEPS ? 1.0f / pTSF->m_fScale : 1.0f;
	QuaternionInverse(&pOut->m_qRot,&pTSF->m_qRot);

	pOut->m_vPos = pTSF->m_vPos * fScaleInv;
	QuaternionTransformVector(&pOut->m_vPos,&pOut->m_vPos,&pOut->m_qRot);
	pOut->m_vPos = - pOut->m_vPos;

	pOut->m_fScale = fScaleInv;

	Vec3Recip(&pOut->m_vLocalScale,&pTSF->m_vLocalScale);	

}

inline void  TransformInvMul(NodeTransform* pOut,const NodeTransform* pTSFA,const NodeTransform* pTSFB)
{
	NodeTransform tsfAInv;
	TransformInverse(&tsfAInv,pTSFB);
	TransformMul(pOut,pTSFA,&tsfAInv);
}

inline void  TransformMad(NodeTransform* pOut,const NodeTransform* pA,float f,const NodeTransform* pB)
{
	Vec3Mad(&pOut->m_vPos,&pA->m_vPos,f,&pB->m_vPos);
	QuaternionMad(&pOut->m_qRot,&pB->m_qRot,&pA->m_qRot,f);
	Vec3Mad(&pOut->m_vLocalScale,&pA->m_vLocalScale,f,&pB->m_vLocalScale);
	pOut->m_fScale = pA->m_fScale * f + pB->m_fScale;
}



inline void  MatrixFromTransform( Matrix4x4* pMat,const NodeTransform* pTSF)
{
	Vector3 vScale = pTSF->m_vLocalScale * pTSF->m_fScale;
	MatrixTransform(pMat,&vScale,&pTSF->m_qRot,&pTSF->m_vPos);
}


inline void NodeVec3TransformCoord(Vector3* pOut,const Vector3* pPos,const NodeTransform* pTSF)
{
	Vector3 vPos;
	vPos = *pPos * pTSF->m_fScale;
	Vec3Mul(&vPos,&vPos,&pTSF->m_vLocalScale);
	QuaternionTransformVector(&vPos,&vPos,&pTSF->m_qRot);
	*pOut = vPos + pTSF->m_vPos;

}

inline void BoneVec3TransformCoord(Vector3* pOut,const Vector3* pPos,const NodeTransform* pTSF)
{
	Vector3 vPos;
	vPos = *pPos * pTSF->m_fScale;
	QuaternionTransformVector(&vPos,&vPos,&pTSF->m_qRot);
	*pOut = vPos + pTSF->m_vPos;	
}

inline void RigidVec3TransformCoord(Vector3* pOut,const Vector3* pPos,const NodeTransform* pTSF)
{
	Vector3 vPos;
	QuaternionTransformVector(&vPos,pPos,&pTSF->m_qRot);
	*pOut = vPos + pTSF->m_vPos;	
}

inline void RigidTransformInverse(RigidTransform* pOut,const NodeTransform* pTSF)
{

	QuaternionInverse(&pOut->m_qRot,&pTSF->m_qRot);
	QuaternionTransformVector(&pOut->m_vPos,&pTSF->m_vPos,&pOut->m_qRot);
	pOut->m_vPos = - pOut->m_vPos;
}



#endif //__S3Math_Inl__