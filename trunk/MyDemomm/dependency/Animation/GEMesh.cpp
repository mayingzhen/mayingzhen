#include "GEMesh.h"
#include "public.h"
#include "tinyxml.h"
//#include "GEStringParser.h"
#include "GEUtil.h"
#include "GameApp.h"

#define BIG_NUMBER		    (1.0e+10f)

GEMesh::~GEMesh(void)
{
	for(size_t i=0;i<mSubmeshes.size();++i)
	{
		SAFE_DELETE(mSubmeshes[i]);
	}
	mSubmeshes.clear();		

}

/*
bool GEMesh::parseXML()
{
	TiXmlDocument	*XMLDoc;
	TiXmlElement	*XMLRoot, *XMLElement;

	string file;
	GEUTIL::ReadWholeFile(mMeshFile,file);

	XMLDoc = new TiXmlDocument();
	XMLDoc->Parse(file.c_str());

	if( XMLDoc->Error() )
	{
		SAFE_DELETE(XMLDoc);
        //GEDEBUGINFO("XML parsing error!, Parsing XML Mesh faild.");
	}

	XMLRoot = XMLDoc->RootElement();
	if( string(XMLRoot->Value()) != "mesh" ) 
	{
		delete XMLDoc;
		//GEDEBUGINFO("XML parsing error!, Parsing XML Mesh faild.");
	}

	XMLElement = XMLRoot->FirstChildElement("submeshes");
	if(XMLElement)
	{
		loadSubMeshes(XMLElement);
	}

	XMLElement = XMLRoot->FirstChildElement("skeletonlink");
	if(XMLElement)
	{
		mSkeletonFileName=XMLElement->Attribute("name");	
	}

	delete XMLDoc;
	return true;
}

bool GEMesh::loadSubMeshes(TiXmlElement* element)
{
	GESubmesh* p;
	TiXmlElement* e=element->FirstChildElement("submesh");
	while(e)
	{
		p=loadSubmesh(e);
		if(p!=NULL) mSubmeshes.push_back(p);
		e=e->NextSiblingElement("submesh");
	}
	return true;
}

GESubmesh* GEMesh::loadSubmesh(TiXmlElement* subMeshElement)
{	
	const char* c;
	string s;
	GESubmesh* submesh=NULL;

	if(string(subMeshElement->Value()) != "submesh")
		return NULL;
	else
		submesh=new GESubmesh();


	c=subMeshElement->Attribute("material");
	if(c) 
	{
		//Peter: 用GEMaterialLoader装载该material，material的文件名为c

	}

	// 初始化VB
	TiXmlElement* geo=subMeshElement->FirstChildElement("geometry");
	bool hasPosition, hasNormal;
	if(geo!=NULL)
	{
		GEPrimitiveType priType;
		
		const char* c = subMeshElement->Attribute("operationtype");
		string primitive;
		if (c)	primitive = string(c);
		else primitive = "UNKNOWN";

		if(primitive=="point_list")          priType=GE_POINTLIST;
		else if(primitive=="line_list")      priType=GE_LINELIST;
		else if(primitive=="line_strip")     priType=GE_LINESTRIP;
		else if(primitive=="triangle_list")  priType=GE_TRIANGLELIST;
		else if(primitive=="triangle_strip") priType=GE_TRIANGLESTRIP;
		else if(primitive=="triangle_fan")   priType=GE_TRIANGLEFAN;
		else priType=GE_TRIANGLELIST;	// for default

		DWORD FVF=0x00;
		int stride=0;
		TiXmlElement* vb;
		vb=geo->FirstChildElement("vertexbuffer");
		while(vb)
		{
			const char* c;
			string s;
			c=vb->Attribute("positions");
			if(c!=NULL) s=string(c); else s=""; 
			if(s=="true") { FVF |= D3DFVF_XYZ;    hasPosition=true; }

			c=vb->Attribute("normals");
			if(c!=NULL) s=string(c); else s=""; 
			if(s=="true") { FVF |= D3DFVF_NORMAL; hasNormal=true;   }

			c=vb->Attribute("texture_coords");
			string texNum;
			if(c!=NULL) 
				texNum=string(c);
			else
				texNum="";
			if(texNum=="1")      FVF |= D3DFVF_TEX1;
			else if(texNum=="2") FVF |= D3DFVF_TEX2;
			else if(texNum=="3") FVF |= D3DFVF_TEX3;
			else if(texNum=="4") FVF |= D3DFVF_TEX4;
			else if(texNum=="5") FVF |= D3DFVF_TEX5;
			else if(texNum=="6") FVF |= D3DFVF_TEX6;
			else if(texNum=="7") FVF |= D3DFVF_TEX7;
			else if(texNum=="8") FVF |= D3DFVF_TEX8;
			
			for(short i=0;i<8;i++)
			{
				string tcd; // texture_coord_dimensions				
				switch(i)
				{
				case 0:
					c=vb->Attribute("texture_coords_dimensions_0");
					break;
				case 1:
					c=vb->Attribute("texture_coords_dimensions_1");
					break;
				case 2:
					c=vb->Attribute("texture_coords_dimensions_2");
					break;
				case 3:
					c=vb->Attribute("texture_coords_dimensions_3");
					break;
				case 4:
					c=vb->Attribute("texture_coords_dimensions_4");
					break;
				case 5:
					c=vb->Attribute("texture_coords_dimensions_5");
					break;
				case 6:
					c=vb->Attribute("texture_coords_dimensions_6");
					break;
				case 7:
					c=vb->Attribute("texture_coords_dimensions_7");
					break;
				}
				if(c!=NULL) 
					tcd=c; 
				else 
					tcd="";

				if(tcd=="1")      { FVF |= D3DFVF_TEXCOORDSIZE1(i);	stride+=sizeof(float)*1; }
				else if(tcd=="2") { FVF |= D3DFVF_TEXCOORDSIZE2(i);	stride+=sizeof(float)*2; }
				else if(tcd=="3") { FVF |= D3DFVF_TEXCOORDSIZE3(i);	stride+=sizeof(float)*3; }
				else if(tcd=="4") { FVF |= D3DFVF_TEXCOORDSIZE4(i);	stride+=sizeof(float)*4; }
			}
			vb=vb->NextSiblingElement("vertexbuffer");
		}
		if(FVF & D3DFVF_XYZ	)    stride+=sizeof(float)*3;
		if(FVF & D3DFVF_NORMAL ) stride+=sizeof(float)*3;

		string vertexcount=string(geo->Attribute("vertexcount"));
		if(vertexcount=="") vertexcount="0";
		int count=GEStringParser::parseInt(vertexcount);

		submesh->mVB = new GEVertexBuffer(count*stride, FVF, stride, priType);
        submesh->mVB->InitDeviceObjectsFromMesh();
		// loadSubmesh就是由GEMesh::InitDeviceObjects()调用的，在mVB的InitDeviceObjects()调用之前不能lock


		// 填充VB数据
		IDirect3DVertexBuffer9* pVB=submesh->mVB->m_DX9VB;
		void *p;
		unsigned char*tmp;
		if(SUCCEEDED(pVB->Lock(0,0,&p,0)))
		{
			vb=geo->FirstChildElement("vertexbuffer");
			while(vb)
			{
				tmp=reinterpret_cast<unsigned char*>(p);
                TiXmlElement* vertex=vb->FirstChildElement("vertex");
				while(vertex)
				{
					float x,y,z;
					TiXmlElement* pos=vertex->FirstChildElement("position");
					if(pos)
					{
						x=GEStringParser::parseFloat(pos->Attribute("x"));
						// begin peter: flip y and z
						z=GEStringParser::parseFloat(pos->Attribute("y"));
						y=GEStringParser::parseFloat(pos->Attribute("z"));	
						// end peter
						*(reinterpret_cast<float*>(tmp)+0)=x;
						*(reinterpret_cast<float*>(tmp)+1)=y;
						*(reinterpret_cast<float*>(tmp)+2)=z;
						// 记录包围盒的信息
						if(x<mLeft)   
							mLeft=x;
						if(x>mRight)  
							mRight=x;
						if(z>mFront)  
							mFront=z;
						if(z<mRear)   
							mRear=z;						
						if(y<mBottom) 
							mBottom=y;
						if(y>mTop)    
							mTop=y;
					}

					TiXmlElement* nor=vertex->FirstChildElement("normal");
					if(nor)
					{				
						x=GEStringParser::parseFloat(nor->Attribute("x"));
						// begin peter
						z=GEStringParser::parseFloat(nor->Attribute("y"));
						y=GEStringParser::parseFloat(nor->Attribute("z"));
						// end peter
						short offset=0;
						if(hasPosition) offset+=3; // 这里的offset是以float的宽度为单位，不是byte
						*(reinterpret_cast<float*>(tmp)+offset+0)=x;
						*(reinterpret_cast<float*>(tmp)+offset+1)=y;
						*(reinterpret_cast<float*>(tmp)+offset+2)=z;						
					}

					short offset=0;
					float* temptexp=reinterpret_cast<float*>(tmp);
					if(hasPosition) temptexp+=3;
					if(hasNormal)   temptexp+=3;
					TiXmlElement* texcoord=vertex->FirstChildElement("texcoord");
					while(texcoord)
					{
						float tc;
						string s;
						const char* c;
						
						c=texcoord->Attribute("u");						
						if(c!=NULL)	s=string(c); else s="";
						if(s!="")
						{						
							tc=GEStringParser::parseFloat(s);						
							*(temptexp++)=tc;						
						}
						c=texcoord->Attribute("v");
						if(c!=NULL)	s=string(c); else s="";
						if(s!="")
						{						
							tc=GEStringParser::parseFloat(s);						
							*(temptexp++)=tc;						
						}
						c=texcoord->Attribute("w");
						if(c!=NULL)	s=string(c); else s="";
						if(s!="")
						{						
							tc=GEStringParser::parseFloat(s);						
							*(temptexp++)=tc;						
						}
						c=texcoord->Attribute("k");	//! 4D的texcoord是u,v,w,k吗？
						if(c!=NULL)	s=string(c); else s="";
						if(s!="")
						{						
							tc=GEStringParser::parseFloat(s);						
							*(temptexp++)=tc;						
						}
						texcoord=texcoord->NextSiblingElement("texcoord");
					}

					tmp+=stride;
                    vertex=vertex->NextSiblingElement("vertex");
				}
            
				vb=vb->NextSiblingElement("vertexbuffer");
			}
			pVB->Unlock();
		}
	}

	// 初始化IB
	TiXmlElement* e;
	e=subMeshElement->FirstChildElement("faces");
	if(e!=NULL)
	{
		string counts;
		counts=e->Attribute("count");
		if(counts=="") counts="0";
		int count=GEStringParser::parseInt(counts);

		UINT IBsize=count*sizeof(unsigned short)*3;
		unsigned char* cbuf = new unsigned char[IBsize];
		unsigned short* buf=reinterpret_cast<unsigned short*>(cbuf);  // temp buffer
		unsigned short* p=buf;
		
		TiXmlElement* face=e->FirstChildElement("face");
		while(face)
		{
			unsigned short v1,v2,v3;
			// peter
			v3=static_cast<unsigned short>(GEStringParser::parseInt(face->Attribute("v1")));
			v2=static_cast<unsigned short>(GEStringParser::parseInt(face->Attribute("v2")));
			v1=static_cast<unsigned short>(GEStringParser::parseInt(face->Attribute("v3")));
			*(p++)=v1;
			*(p++)=v2;
			*(p++)=v3;
			face=face->NextSiblingElement("face");
		}

		submesh->mIB=new GEIndexBuffer(count*sizeof(unsigned short)*3, sizeof(unsigned short), submesh->mVB);
		submesh->mIB->InitDeviceObjects(buf,IBsize);
        delete [] cbuf;
	}

	// 初始化BoneAssignment
	e=subMeshElement->FirstChildElement("boneassignments");
	if(e!=NULL)
	{
		submesh->mBoneAssignment = new GEBoneAssignment();
        TiXmlElement* asg=e->FirstChildElement("vertexboneassignment");
		while(asg)
		{			
            unsigned short vertex, bone;
			float weight;
			vertex = static_cast<unsigned short>(GEStringParser::parseInt(asg->Attribute("vertexindex")));
			bone   = static_cast<unsigned short>(GEStringParser::parseInt(asg->Attribute("boneindex")));
			weight = GEStringParser::parseFloat(asg->Attribute("weight"));
			submesh->mBoneAssignment->AddAssignment(vertex,bone,weight);                        
			asg=asg->NextSiblingElement("vertexboneassignment");
		}
	}
	return submesh; 
	
}

bool GEMesh::parseFromData(std::string sFileName))
{
}

bool GEMesh::loadSubMeshesFromData(BYTE* pData);
{

}

GESubmesh* GEMesh::loadSubmeshFromData(BYTE* pData);
{

}
*/

HRESULT GEMesh::Render()
{
	for(unsigned int i=0;i<mSubmeshes.size();++i)
	{
		mSubmeshes[i]->Render();
	}
	return S_OK;
}

HRESULT GEMesh::InitDeviceObjects()
{
// 	if (mIsInited)
// 		return S_OK;
// 
// 	// Peter:  the next 4 lines can be deleted
// 	for(size_t i=0;i<mSubmeshes.size();++i)
// 	{
// 		SAFE_DELETE(mSubmeshes[i]);
// 	}
// 	mSubmeshes.clear();
// 
// 	// if ( all the meshes using this file hasn't been inited )
// 	GEMesh* pMeshLoaded = GEMeshLoadHelper::GetSingleton().GetMeshLoaded(mMeshFile);
// 	if (pMeshLoaded == NULL)
// 	{
// 		if (GEUTIL::ToLowerCase(mMeshFile.substr(mMeshFile.length()-2)) == ".x")
// 			this->parseX();
// 		else if (GEUTIL::ToLowerCase(mMeshFile.substr(mMeshFile.length()-4)) == ".xml")
// 			this->parseXML();
// 		else if (GEUTIL::ToLowerCase(mMeshFile.substr(mMeshFile.length()-4)) == ".gem")
// 			this->parseGEM();
// 	}
// 	else
// 	{
// 		// copy from the object which has been inited
// 		CopyDataFrom(pMeshLoaded);
// 	}
// 
// 	// tell MeshLoadHelper
// 	GEMeshLoadHelper::GetSingleton().TagMeshAsLoaded(mMeshFile, this);
// 
// 	mIsInited  = true;
// 
 	return S_OK;
}

HRESULT GEMesh::RestoreDeviceObjects()
{
// 	if (mIsRestored)
// 		return S_OK;
// 
// 	LPDIRECT3DDEVICE9 pdevice=GEGraphicLayer::GetSingleton()->GetD3DDevice();
// 
// 	for(unsigned int i=0;i<mSubmeshes.size();++i)
// 	{
// 		mSubmeshes[i]->RestoreDeviceObjects();
// 	}
// 
// 	mIsRestored = true;
// 
	return S_OK;
}

HRESULT GEMesh::InvalidateDeviceObjects()
{
// 	if (!mIsRestored)
// 		return S_OK;
// 
// 	for(unsigned int i=0;i<mSubmeshes.size();++i)
// 	{
// 		mSubmeshes[i]->InvalidateDeviceObjects();
// 	}
// 
// 	mIsRestored = false;
// 
 	return S_OK;
}

HRESULT GEMesh::DeleteDeviceObjects()
{
// 	if (!mIsInited)
// 		return S_OK;
// 
// 	for(size_t i=0;i<mSubmeshes.size();++i)
// 	{
// 		mSubmeshes[i]->DeleteDeviceObjects();
// 		SAFE_DELETE(mSubmeshes[i]);
// 	}
// 	mSubmeshes.clear();	
// 
// 	// tell MeshLoadHelper
// 	GEMeshLoadHelper::GetSingleton().TagMeshAsUnloaded(mMeshFile, this);
// 
// 	mIsInited = false;

	return S_OK;
}

void GEMesh::updateFromSkeleton(GESkeleton* skeleton)
{
	mLeft	= BIG_NUMBER;
	mRight	= -BIG_NUMBER;
	mBottom	= BIG_NUMBER;
	mTop	= -BIG_NUMBER;
	mRear	= BIG_NUMBER;
	mFront	= -BIG_NUMBER;

	for(unsigned int i=0; i<mSubmeshes.size(); ++i)
	{
		mSubmeshes[i]->updateFromSkeleton(skeleton);

// 		if (mLeft > mSubmeshes[i]->GetXMin())
// 			mLeft = mSubmeshes[i]->GetXMin();
// 		if (mRight < mSubmeshes[i]->GetXMax())
// 			mRight = mSubmeshes[i]->GetXMax();
// 		if (mBottom > mSubmeshes[i]->GetYMin())
// 			mBottom = mSubmeshes[i]->GetYMin();
// 		if (mTop < mSubmeshes[i]->GetYMax())
// 			mTop = mSubmeshes[i]->GetYMax();
// 		if (mRear > mSubmeshes[i]->GetZMin())
// 			mRear = mSubmeshes[i]->GetZMin();
// 		if (mFront < mSubmeshes[i]->GetZMax())
// 			mFront = mSubmeshes[i]->GetZMax();
	}

}


void GEMesh::updateFromMorphAnimation(GEMorphAnimationKey* currentPose)
{
	// 在 Morph Animation 中只有一个Submesh 
	mLeft	= BIG_NUMBER;
	mRight	= -BIG_NUMBER;
	mBottom	= BIG_NUMBER;
	mTop	= -BIG_NUMBER;
	mRear	= BIG_NUMBER;
	mFront	= -BIG_NUMBER;

	mSubmeshes[0]->updateFromMorphAnimation(currentPose);

// 	if (mLeft > mSubmeshes[0]->GetXMin())
// 		mLeft = mSubmeshes[0]->GetXMin();
// 	if (mRight < mSubmeshes[0]->GetXMax())
// 		mRight = mSubmeshes[0]->GetXMax();
// 	if (mBottom > mSubmeshes[0]->GetYMin())
// 		mBottom = mSubmeshes[0]->GetYMin();
// 	if (mTop < mSubmeshes[0]->GetYMax())
// 		mTop = mSubmeshes[0]->GetYMax();
// 	if (mRear > mSubmeshes[0]->GetZMin())
// 		mRear = mSubmeshes[0]->GetZMin();
// 	if (mFront < mSubmeshes[0]->GetZMax())
// 		mFront = mSubmeshes[0]->GetZMax();
}


bool GEMesh::CopyDataFrom(const GEMesh* mesh)
{
	if (mesh == NULL || !mesh->mIsInited || mIsInited)
		return false;

	// means it has been inited
	// get mesh params
    mLeft	= mesh->mLeft;
	mRight	= mesh->mRight;
	mTop	= mesh->mTop;
	mBottom = mesh->mBottom;
	mFront	= mesh->mFront;
	mRear	= mesh->mRear;

	// copy submeshes
	mSubmeshes.resize(mesh->mSubmeshes.size());

	for (unsigned int i = 0; i < mSubmeshes.size(); ++i)
	{
		/*
		GESubmesh* srcSubmesh = mesh->mSubmeshes[i];
		GEVertexBuffer* srcVB = srcSubmesh->mVB;
		GEIndexBuffer*  srcIB = srcSubmesh->mIB;

		mSubmeshes[i] = new GESubmesh();
		mSubmeshes[i]->mMaterial = srcSubmesh->mMaterial;
		mSubmeshes[i]->mParent = this;

		// copy vb
		mSubmeshes[i]->mVB = new GEVertexBuffer(srcVB->m_size, srcVB->m_FVF, srcVB->m_stride, srcVB->m_primitiveType);
		mSubmeshes[i]->mVB->InitDeviceObjectsFromMesh();
        
		void *p;
		void *srcP;
        mSubmeshes[i]->mVB->m_DX9VB->Lock(0,0,&p,0);
		srcVB->m_DX9VB->Lock(0,0,&srcP,0);
        memcpy(p, srcP, srcVB->m_dataSize);
		srcVB->m_DX9VB->Unlock();
		mSubmeshes[i]->mVB->m_DX9VB->Unlock();

		// copy ib
		mSubmeshes[i]->mIB = new GEIndexBuffer(srcIB->m_size, sizeof(unsigned short), mSubmeshes[i]->mVB);
		srcIB->m_DX9IB->Lock(0, 0, &srcP, 0);		
		mSubmeshes[i]->mIB->InitDeviceObjects(srcP,srcIB->m_dataSize);
		srcIB->m_DX9IB->Unlock();

		// copy BoneAssignment
		if (srcSubmesh->mBoneAssignment != NULL)
		{
			mSubmeshes[i]->mBoneAssignment = new GEBoneAssignment();
			mSubmeshes[i]->mBoneAssignment->mMap = srcSubmesh->mBoneAssignment->mMap;
		}
		else
			mSubmeshes[i]->mBoneAssignment = NULL;
			*/
	}

	// copy skeletonlink
	mSkeletonFileName = mesh->mSkeletonFileName;
	
	return true;
}


// bool GEMesh::parseX()
// {
// 	const string stopStr = ",; \t\r\n";
// 
// 	ifstream fin(mMeshFile.c_str());
// 	
// 	while (true)		// get "Mesh {"
// 	{
// 		if (GEUTIL::GetNextStrNotCmt(fin) == "Mesh")
// 		{
// 			GEUTIL::GetThisLineRest(fin);	// {
// 			break;
// 		}
// 	}
// 	
// 	GESubmesh* pSubmesh = new GESubmesh();
// 	this->mSubmeshes.push_back(pSubmesh);
// 
// 	// 顶点格式
// 	D3DPRIMITIVETYPE  priType =  D3DPT_TRIANGLELIST;//GE_TRIANGLELIST;
// 	DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(i);
// 	int stride = sizeof(float) * (3 + 3 + 2);
// 	int nVertexs = GEUTIL::StrToInt_(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 
// 	// 创建VB
// 	//pSubmesh->mVB = new GEVertexBuffer(nVertexs*stride, FVF, stride, priType);
// 	HRESULT hr = 0;
// 	g_pD3DDevice->CreateVertexBuffer( nVertexs * stride,
// 		D3DUSAGE_WRITEONLY,0,D3DPOOL_MANAGED,&pSubmesh->mVB,0);
// 	if( FAILED(hr) )
// 		return false;
// 
// 	//pSubmesh->mVB->InitDeviceObjectsFromMesh();
// 
// 	// 填充VB数据
// 	IDirect3DVertexBuffer9* pVB = pSubmesh->mVB;
// 	void *p;
// 	if(SUCCEEDED(pVB->Lock(0,0,&p,0)))
// 	{
// 		char* tmp = reinterpret_cast<char*>(p);
// 		for (int i = 0; i < nVertexs; ++i)
// 		{
// 			// 填充位置
// 			float x = GEUTIL::StrToFloat(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			float y = GEUTIL::StrToFloat(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			float z = GEUTIL::StrToFloat(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			*(reinterpret_cast<float*>(tmp)+0) = x;
// 			*(reinterpret_cast<float*>(tmp)+1) = y;
// 			*(reinterpret_cast<float*>(tmp)+2) = z;
// 			tmp += stride;
// 
// 			// 记录包围盒的信息
// 			if(x<mLeft)   
// 				mLeft=x;
// 			if(x>mRight)  
// 				mRight=x;
// 			if(z>mFront)  
// 				mFront=z;
// 			if(z<mRear)   
// 				mRear=z;						
// 			if(y<mBottom) 
// 				mBottom=y;
// 			if(y>mTop)    
// 				mTop=y;
// 		}
// 
// 		// 初始化IB
// 		int nFaces = GEUTIL::StrToInt_(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 		UINT iIBsize= nFaces*sizeof(unsigned short)*3;
// 		char* pcFaces = new char[iIBsize];
// 		unsigned short* pFaces = reinterpret_cast<unsigned short*>(pcFaces);
// 
// 		for (int i = 0; i < nFaces; ++i)
// 		{
// 			GEUTIL::GetNextStrStopBy(fin, stopStr);	// 3
// 			unsigned short v1 = GEUTIL::StrToInt_(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			unsigned short v2 = GEUTIL::StrToInt_(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			unsigned short v3 = GEUTIL::StrToInt_(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			*(pFaces++) = v1;
// 			*(pFaces++) = v2;
// 			*(pFaces++) = v3;
// 		}
// 		pFaces = reinterpret_cast<unsigned short*>(pcFaces);
// 
// 		// 关于法向
// 		while (true)		// get "MeshNormals {"
// 		{
// 			string s = GEUTIL::GetNextStrNotCmt(fin);
// 			if (s.length() >= 11 && s.substr(0, 11) == "MeshNormals")
// 			{
// 				GEUTIL::GetThisLineRest(fin);
// 				break;
// 			}
// 		}
// 		
// 		int nNormals = GEUTIL::StrToInt_(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 		float* pNormals = new float[nNormals*3];
// 		for (int i = 0; i < nNormals; ++i)
// 		{
// 			float x = GEUTIL::StrToFloat(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			float y = GEUTIL::StrToFloat(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			float z = GEUTIL::StrToFloat(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			*(pNormals++) = x;
// 			*(pNormals++) = y;
// 			*(pNormals++) = z;
// 		}
// 		pNormals -= nNormals*3;
// 
// 		GEUTIL::GetNextStrStopBy(fin, stopStr);		// m个面
// 		tmp = reinterpret_cast<char*>(p);
// 		for (int i = 0; i < nFaces; ++i)
// 		{
// 			GEUTIL::GetNextStrStopBy(fin, stopStr);	// 3
// 			int nindex1 = GEUTIL::StrToInt_(GEUTIL::GetNextStrStopBy(fin, stopStr));	// 面三个顶点对应的法向index
// 			int nindex2 = GEUTIL::StrToInt_(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			int nindex3 = GEUTIL::StrToInt_(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			
// 			// 该面对应的顶点号
// 			int iVertex1 = pFaces[i*3+0];
// 			int iVertex2 = pFaces[i*3+1];
// 			int iVertex3 = pFaces[i*3+2];
// 			
// 			// 填充法向
//             *(reinterpret_cast<float*>((tmp+stride*iVertex1+sizeof(float)*3))) = *(pNormals+nindex1*3+0);
// 			*(reinterpret_cast<float*>((tmp+stride*iVertex1+sizeof(float)*4))) = *(pNormals+nindex1*3+1);
// 			*(reinterpret_cast<float*>((tmp+stride*iVertex1+sizeof(float)*5))) = *(pNormals+nindex1*3+2);
// 
// 			*(reinterpret_cast<float*>((tmp+stride*iVertex2+sizeof(float)*3))) = *(pNormals+nindex2*3+0);
// 			*(reinterpret_cast<float*>((tmp+stride*iVertex2+sizeof(float)*4))) = *(pNormals+nindex2*3+1);
// 			*(reinterpret_cast<float*>((tmp+stride*iVertex2+sizeof(float)*5))) = *(pNormals+nindex2*3+2);
// 
// 			*(reinterpret_cast<float*>((tmp+stride*iVertex3+sizeof(float)*3))) = *(pNormals+nindex3*3+0);
// 			*(reinterpret_cast<float*>((tmp+stride*iVertex3+sizeof(float)*4))) = *(pNormals+nindex3*3+1);
// 			*(reinterpret_cast<float*>((tmp+stride*iVertex3+sizeof(float)*5))) = *(pNormals+nindex3*3+2);
// 		}
// 		delete [] pNormals;
// 
// 		// 关于纹理贴图
// 		while (true)		// get "MeshTextureCoords {"
// 		{
// 			string s = GEUTIL::GetNextStrNotCmt(fin);
// 			if (s.length() >= 17 && s.substr(0, 17) == "MeshTextureCoords")
// 			{
// 				GEUTIL::GetThisLineRest(fin);
// 				break;
// 			}
// 		}
// 
// 		GEUTIL::GetNextStrStopBy(fin, stopStr);	// n个顶点
// 		tmp = reinterpret_cast<char*>(p);
// 		for (int i = 0; i < nVertexs; ++i)
// 		{
// 			*(reinterpret_cast<float*>(tmp+stride*i+sizeof(float)*6)) = GEUTIL::StrToFloat(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 			*(reinterpret_cast<float*>(tmp+stride*i+sizeof(float)*7)) = GEUTIL::StrToFloat(GEUTIL::GetNextStrStopBy(fin, stopStr));
// 		}
// 		
// 		pVB->Unlock();
// 
// 		HRESULT hr = D3D_OK;	
// 		hr = g_pD3DDevice->CreateIndexBuffer( iIBsize * sizeof(WORD), 
// 			D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&pSubmesh->mIB,0);
// 		if( FAILED(hr) )
// 			return false;
// 		//pSubmesh->mIB = new GEIndexBuffer(iIBsize, sizeof(unsigned short), pSubmesh->mVB);
// 		//pSubmesh->mIB->InitDeviceObjects(pFaces,iIBsize);
// 		delete [] pcFaces;
// 	}
// 	
// 	return true;
// }


