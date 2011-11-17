#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <basetsd.h>
#include <stdio.h>
#include <d3dx9.h>
#include <algorithm>

#include "TextureAtlas.h"

#include "TATypes.h"
#include "CmdLineOptions.h"
#include "TextureObject.h"
#include "AtlasContainer.h"

CTextureAtlas::CTextureAtlas() 
{
	m_pAtlas = NULL;
}

CTextureAtlas::~CTextureAtlas() 
{ 
	if (m_pAtlas)
	{
		delete m_pAtlas; 
		m_pAtlas = NULL;
	} 
}


bool CreateTAIFile(CmdLineOptionCollection const &options, 
				   TNewFormatMap const &formatMap)
{
	// write the tai header 
	char     fname[kFilenameLength];
	sprintf( fname, "%s.tai", options.GetArgument(CLO_OUTFILE, 0));

	FILE   *fp = fopen( fname, "w" );
	if (fp == NULL)
	{
		fprintf( stderr, "*** Error: Unable to open file \"%s\" for writing.\n", fname );
		return false;
	}
	fprintf( stderr, "Saving file: %s\n", fname );

	fprintf( fp, "# %s\n", fname );
	// echo the cmd line used to invoke this
	fprintf( fp, "# AtlasCreationTool.exe");
	for (int i = CLO_NOMIPMAP; i < CLO_NUM; ++i)
		if (options.IsSet(static_cast<eCmdLineOptionType>(i)))
		{
			fprintf( fp, " %s", kParseString[i]);
			for (int j = 0; j < kNumArguments[i]; ++j) 
				fprintf( fp, " %s", options.GetArgument(static_cast<eCmdLineOptionType>(i), j));    
		}
		fprintf( fp, "\n#\n");
		fprintf( fp, "# <filename>\t\t<atlas filename>, <atlas idx>, <atlas type>, <woffset>, <hoffset>, <depth offset>, <width>, <height>\n#\n" );
		fprintf( fp, "# Texture <filename> can be found in texture atlas <atlas filename>, i.e., \n");
		fprintf( fp, "# %s<idx>.dds of <atlas type> type with texture coordinates boundary given by:\n", options.GetArgument(CLO_OUTFILE, 0));
		fprintf( fp, "#   A = ( <woffset>, <hoffset> )\n" );
		fprintf( fp, "#   B = ( <woffset> + <width>, <hoffset> + <height> )\n#\n" );
		fprintf( fp, "# where coordinates (0,0) and (1,1) of the original texture map correspond\n" );
		fprintf( fp, "# to coordinates A and B, respectively, in the texture atlas.\n" );
		fprintf( fp, "# If the atlas is a volume texture then <depth offset> is the w-coordinate\n" );
		fprintf( fp, "# to use the access the appropriate slice in the volume atlas.\n" );
		fprintf( fp, "\n" );

		// go through each texture and convert coordinates and write out the data
		TNewFormatMap::const_iterator           fmIter;
		TTexture2DPtrVector::const_iterator     texIter;
		for (fmIter = formatMap.begin(); fmIter != formatMap.end(); ++fmIter)
			for (texIter = (fmIter->second).begin(); texIter != (fmIter->second).end(); ++texIter)  
				(*texIter)->WriteTAILine(options, fp);

		fclose( fp );
		return true;
}


//-----------------------------------------------------------------------------
// Name: CreateTextureAtlases()
// Desc: Creates atlases for the given textures.
//       All options/info is stored in the options parameter.
//       Returns false if errors occur.
//-----------------------------------------------------------------------------
bool CTextureAtlas::CreateTextureAtlases(CmdLineOptionCollection const &options) 
{
	// Create an array of texture objects and load the filenames/textures into that array.
	int const   kNumTextures = options.GetNumFilenames();
	char const *pFilename = NULL;
	int         i;

	Texture2D * pSourceTex = new Texture2D[kNumTextures];

	for (i = 0; i < kNumTextures; ++i)
	{
		options.GetFilename(i, &pFilename);
		pSourceTex[i].Init(m_pd3dDevice, pFilename);
		if (FAILED(pSourceTex[i].LoadTexture(options)))
		{
			delete [] pSourceTex;
			return false;
		}
	}

	// Bin these textures into format groups (maps of vectors)
	TNewFormatMap   formatMap;
	for (i = 0; i < kNumTextures; ++i)
		formatMap[pSourceTex[i].GetFormat()].push_back( &(pSourceTex[i]) );

	// We do not do format conversions, so all these different formats 
	// require their own atlases.  Each format may have multiple atlases, e.g., 
	// there is not enough space in a single atlas for all textures of the 
	// same format.  An atlas container contains all these concepts.
	m_pAtlas = new AtlasContainer( options, formatMap.size() );

	// For each format-vector of textures, Sort textures by size (width*height, then height
	TNewFormatMap::iterator fmSort;
	for ( fmSort = formatMap.begin(); fmSort != formatMap.end(); ++fmSort)
		std::sort( (*fmSort).second.begin(), (*fmSort).second.end(), Texture2DGreater() );

	// For each format-vector of textures, insert them into their respective 
	// atlas vector: 
	TNewFormatMap::const_iterator fmIter;
	for (i = 0, fmIter = formatMap.begin(); fmIter != formatMap.end(); ++fmIter, ++i)
	{
		m_pAtlas->Insert(i, fmIter->second);
	}

	// Done inserting data: shrink all atlases to minimum size
	m_pAtlas->Shrink();

	// Write all atlases to disk w/ the filenames they have stored
	m_pAtlas->WriteToDisk();

	// Save the Texture Atlas Info (tai) file.
	// For each original texture read-out where it landed up
	// and write that into the tai file.
	bool const kSuccess = CreateTAIFile(options, formatMap);

	for(int i = 0; i < kNumTextures; ++i)
	{
		m_atlas.push_back(pSourceTex[i].mInAlta);
	}

	// free all memory: clear all vectors of everything
	delete [] pSourceTex;
	return true;
}


bool CTextureAtlas::Create(LPDIRECT3DDEVICE9 pd3dDevice,const std::vector<std::string>& fileNames)
{
	m_pd3dDevice = pd3dDevice;

	if ( fileNames.empty() )
		return false;

	std::vector<std::string> outFileNme,widthMax; 
	outFileNme.push_back("TerainTextAltas");
	widthMax.push_back("1024");

 	CmdLineOptionCollection options;
	options.SetOption(CLO_HALFTEXEL);
	options.SetOption(CLO_OUTFILE,outFileNme);
	options.SetOption(CLO_WIDTH,widthMax);
	options.SetFileNames(fileNames);
	return CreateTextureAtlases(options);
}

bool CTextureAtlas::GetInTextureAtlasUV(std::string fileName,
										float& texU, float& texV,
										float& texW, float& texH)
{
	for (int i = 0; i < m_atlas.size(); ++i)
	{
		if (m_atlas[i].mpFilename == fileName)
		{
			texU = m_atlas[i].uOffset;
			texV = m_atlas[i].vOffset;
			texW = m_atlas[i].uWidth;
			texH = m_atlas[i].vHeight;
			return true;
		}
	}
	return false;
}

LPDIRECT3DTEXTURE9 CTextureAtlas::GetAtlasTexture()
{
	Atlas2D* altas2D = (Atlas2D*)(m_pAtlas->mpAtlasVectorArray[0][0]);
	return altas2D->GetD3DTexture();
}


