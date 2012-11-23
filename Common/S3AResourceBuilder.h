//----------------------------------------------------------------------
// Copyright(c) TQ Digital Entertainment, All Rights Reserved
// Author:  Xu Kaijie
// Created: 2010/3/3
// Describe:S3 Animation
//----------------------------------------------------------------------

#ifndef __S3AResourceBuilder_H__
#define __S3AResourceBuilder_H__



class S3ASerializeListener;
class S3ASkeleton;
class IS3ASkeleton;
class S3AAnimation;
class S3AMeshData;
class IS3AMeshData;

//Sub for resource of different version loading
namespace S3AResourceBuilder
{

	//S3ASkeleton*	LoadSkeleton(S3ASerializeListener* pSL);

	//S3AAnimation*	LoadAnimation(S3ASerializeListener* pSL);

	//S3AAnimation*	LoadLegacyAnimation(S3ASerializeListener* pSL,IS3ASkeleton* pSkel);

	COMMON_API bool			LoadMesh(S3AMeshData* pMesh,S3ASerializeListener* pSL);

	COMMON_API bool			SaveMesh(S3AMeshData* pMesh,S3ASerializeListener* pSL);

	COMMON_API IS3AMeshData* LoadMeshFromBinaryFile(const char* filename);

};

#endif// __S3AResourceBuilder_H__