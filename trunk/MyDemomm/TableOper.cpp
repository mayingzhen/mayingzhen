//#include "stdafx.h"
#include "TableOper.h"
//#include "uidebug.h"				
//#include "GlobalMng.h"
#include "ExcelTable.h"			// for CExcelTextTable in OM SDK..
//#include "DataMng.h"
//#include "MemCheck.h"

#include <map>
#include <string>
#include <vector>

using namespace std;

//#ifdef _DEBUG
//#define new DEBUG_NEW
// #endif

// string GetUIRunDir( BOOL na )
// {
// 	char	tmpDir[256]		= { 0 };
// 	//GetCurrentDirectory( 256,tmpDir );
// 	strcpy( tmpDir, GetModulePath( ) );
// 	if ( !na )
// 	{
// 		tmpDir[ strlen( tmpDir ) - 1 ]	= 0;
// 		//strcat( tmpDir,"\\");
// 	}
// 	return tmpDir;
// }


namespace ExcelNameSpace{

	// ���־����
	enum { 
		TBL_ITEM				= 0,
		TBL_SKILL,
		TBL_SUBSKILL,
		TBL_LEARNSKILL,
		TBL_PRODUCE,
		TBL_PRODUCE_TYPE,
		TBL_MAKE_WND_TYPE,
		TBL_MISSION,
		TBL_MISSION_TARGET,
		TBL_TITLE,
		TBL_BORN,
		TBL_FACE,
		TBL_SCENE,
		TBL_STATUS,
		TBL_SKILLSTATUS,				// add by maoyongbin[2008-12-15]
		TBL_CHANGE_PARTY,				// add by czg[2008-12-16]
		TBL_PRESTIGE,
		TBL_PRODUCEABILITY,
		TBL_PRODUCE_ICON,
		TBL_FCTASK,
		TBL_SECRETARY,
		TBL_PRODUCE_H,
		TBL_ITEM_PIC,
		TBL_EFFORT,
		TBL_GAMBLE_MACHINE,
		TBL_ROLE,						//role.txt,modify by LiuJianduo
		TBL_NPC,							//npc.txt,modify by LiuJianduo
		TBL_ITEM_PART
	};

	vector<string>						arTableList;		// ��������б�
	map<string,UINT>					mapExcelTable;		// Excel��Ĳ���ӳ��
	map<string,UINT>::const_iterator	itExcelTable;			// Excel��ĵ�����
	vector<CExcelTextTable*>			arExcelTableList;		// Excel����ж�
	vector<CExcelTextTable*>::iterator	itExcelTableList;		// Excel����жӵĵ�����
	CExcelTextTable*					cpCurExcelTable;		// ��ǰ�򿪵�excelTable;
	string								strCurExcelTbName;	// ��ǰ�򿪵ı�����
	int									nCurLine;				// ��ǰ���ҵ�����
	int									nTblName;				// ��ǰ�򿪵ı������

	int			AddMap					( string const& strFileName );
	int			GetNameIndex			( string const& strFileName );

	// const group..
	static const char szInvalidName[]		= "\\/|:*?\"<> &[](),";	// ��Ч�������ִ�
	static const char szExcelTableDir[]		= "Data\\";				// DlgTextĿ¼
	static const char szExcelTableSuffix[]	= ".txt";				// ��׷��
};

int ExcelNameSpace::AddMap( string const& strFileName )
{
	int nCount		= arExcelTableList.size();
	pair<string,UINT> value( strFileName,nCount );

	mapExcelTable.insert( value );
	return nCount;
}

int ExcelNameSpace::GetNameIndex( string const& strFileName )
{
	itExcelTable	= mapExcelTable.find( strFileName );
	if ( itExcelTable == mapExcelTable.end() )
		return -1;
	return (*itExcelTable).second;	//mapExcelTable[ strFileName ];
}

BOOL ExcelNameSpace::InitAllExcelTable()
{
	//
	// ͻȻ����������Ǿ�̬����Ļ���sizeof(array)/sizeof(array[0])
	// ���ڱ����ʱ���������ֵ��

	// er���ݱ������
	// er���ݱ������
	// liugang.May.29.2008, �Ǵ��롣���ߵ�ͼƬ��Դ������������������ʱ����ȡ����ĵ��ߡ�
	static const char itemNameTable[][64]			= { "item_1","item_2","item_3","item_4","item_5","item_6","item_7","item_8","item_9","item_10" };
	static const char skillNameTable[][64]			= { "skill_role"/*,"skill_npc"*/ };
	static const char subSkillNameTable[][64]		= { "skill_enhance" };
	static const char learnSkillNameTable[][64]		= { "skill_learn" };
	static const char produceNameTable[][64]		= { "produce_c" };
	static const char produceSkillNameTable[][64]	= { "produce_type_c" };
	static const char produceSkillWndTable[][64]	= { "produce_wnd_type" };
	static const char missionTable[][64]			= { "quest_1","quest_2","quest_3","quest_4","quest_5","quest_6","quest_7","quest_8","quest_9" };
	static const char missionTargetTable[][64]		= { "quest_objective_1","quest_objective_2","quest_objective_3","quest_objective_4","quest_objective_5","quest_objective_6","quest_objective_7","quest_objective_8","quest_objective_9" };
	static const char titleTable[][64]				= { "title" };
	static const char bornTable[][64]				= { "scene_start" };
	static const char faceTable[][64]			    = { "face" };
	static const char sceneTable[][64]				= { "scene" };
	//static const char statusTable[][64]				= { "status" };
	static const char skillStatusTable[][64]		= { "skill_status" };	// add by maoyongbin[2008-12-15]
	static const char changePartyTable[][64]		= { "ChangeParty" };	// add by maoyongbin[2008-12-15]
	static const char prestigeTable[][64]           = { "prestige" };
	static const char produceAbiTable[][64]			= { "Produce_Ability"};
	static const char produceIconTable[][64]		= { "produce_icon"};
	static const char factionCollectionTaskTable[][64]		= { "bpcollect"};
	
	static const char Secretary[][64]		        = { "Secretary"};               // add by xsm[2009.02.16]

	static const char Produce_H[][64]               = { "Produce_H"};              // add by xsm[2009.09.16]

	static const char Effort[][64]                  = { "effort"};
	
	static const char Item_Pic[][64]                = { "Item_Pic"};
	static const char GambleMachine[][64]			= { "Gambling_H"};		//�ϻ��� guochenshu_April
	static const char RoleTable[][64]				= { "role" };			//role��
	static const char NPCTable[][64]				= { "npc_1", "npc_2" };			//NPC��
	static const char itemPartTable[][64]			= { "item_part" };				// װ������
	//CString	strPhyDir	= (LPSTR)(LPCSTR)GetUIRunDir();		// Ϊ�˷�ֹһЩ���⣬��������·��

	int nErr	= 0;
	//-------------------------------------------------------------
	// ��� item ��
	AddMap( "item" );
	CExcelTextTable* cEttItem	= new CExcelTextTable;	//( 10000, 50 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttItem ) );
	arTableList.push_back( "Item_xx.txt" );
	for ( int i=0;i<sizeof(itemNameTable)/sizeof(itemNameTable[0]);i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= itemNameTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttItem->ReadExcelTable( tmpString ) )
		{
			nErr = 1;
			goto _ErrEnd;
		}
	}
	//-------------------------------------------------------------

	//-------------------------------------------------------------
	// ��� skill ��
	AddMap( "skill" );
	CExcelTextTable* cEttSkill		= new CExcelTextTable;	//( 800, 120 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttSkill ) );
	arTableList.push_back( "skill_role.txt"/*"skill_role.txt or skill_npc.txt"*/ );
	for ( int i=0;i<sizeof(skillNameTable)/sizeof(skillNameTable[0]);i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= skillNameTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttSkill->ReadExcelTable( tmpString ) )
		{
			nErr |= ( 1 << 1 );
			goto _ErrEnd;
		}
	}
	//-------------------------------------------------------------
	
	//-------------------------------------------------------------
	// ��� subskill ��
	AddMap( "subskill" );
	CExcelTextTable* cEttSubSkill	= new CExcelTextTable;	//( 500, 50 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttSubSkill ) );
	arTableList.push_back( "skill_enhance.txt" );
	for ( int i=0;i<sizeof(subSkillNameTable)/sizeof(subSkillNameTable[0]);i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= subSkillNameTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttSubSkill->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 2 );
			goto _ErrEnd;
		}
	}
	//-------------------------------------------------------------
	
	//-------------------------------------------------------------
	// ��� learnskill ��
	AddMap( "learnskill" );
	CExcelTextTable* cEttLearnSkill	= new CExcelTextTable;	//( 500, 50 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttLearnSkill ) );
	arTableList.push_back( "skill_learn.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= learnSkillNameTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttLearnSkill->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 3 );
			goto _ErrEnd;
		}
	}
	//-------------------------------------------------------------

	//-------------------------------------------------------------
	// ��� produce ��
	AddMap( "produce_c" );
	CExcelTextTable* cEttProduce	= new CExcelTextTable;	//( 800, 30 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttProduce ) );
	arTableList.push_back( "produce_c.txt" );
	for ( int i=0;i<sizeof(produceNameTable)/sizeof(produceNameTable[0]);i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= produceNameTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttProduce->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 4 );
			goto _ErrEnd;
		}
	}
	//-------------------------------------------------------------

	//-------------------------------------------------------------
	// ��� produce ���ͱ�
	AddMap( "produce_type_c" );
	CExcelTextTable* cEttProduceSkill	= new CExcelTextTable;	//( 100, 5 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttProduceSkill ) );
	arTableList.push_back( "produce_type_c.txt" );
	for ( int i=0;i<sizeof( produceSkillNameTable )/sizeof( produceSkillNameTable[0] );i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= produceSkillNameTable[i];
		tmpString			+= szExcelTableSuffix;
		// produce_type
		if ( !cEttProduceSkill->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 5 );
			goto _ErrEnd;
		}
	}
	//-------------------------------------------------------------

	//-------------------------------------------------------------
	// ��� produce �������ͱ�
	AddMap( "produce_wnd_type" );
	CExcelTextTable* cEttProduceWndType	= new CExcelTextTable;	//( 50, 5 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttProduceWndType ) );
	arTableList.push_back( "produce_wnd_type.txt" );
	for ( int i=0;i<sizeof( produceSkillWndTable )/sizeof( produceSkillWndTable[0] );i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= produceSkillWndTable[i];
		tmpString			+= szExcelTableSuffix;
		// produce_wnd_type
		if ( !cEttProduceWndType->ReadExcelTable( tmpString ) )
		{
			nErr |= ( 1 << 6 );
			goto _ErrEnd;
		}
	}
	//-------------------------------------------------------------


#if 0
	//-------------------------------------------------------------
	// ��� ���� ��
	AddMap( "mission" );
	CExcelTextTable* cMissionTable	= new CExcelTextTable;	//( 3000, 30 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cMissionTable ) );
	arTableList.push_back( "quest.txt" );
	for ( int i=0;i<sizeof( missionTable )/sizeof( missionTable[0] );i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= "\\Quest\\";
		tmpString			+= missionTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cMissionTable->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 7 );
			goto _ErrEnd;
		}
	}
	//-------------------------------------------------------------

	//-------------------------------------------------------------
	// ��� ����Ŀ�� ��
	AddMap( "target" );
	CExcelTextTable* cTargetTable	= new CExcelTextTable;	//( 5000, 30 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cTargetTable ) );
	arTableList.push_back( "quest_objective.txt" );
	for ( int i=0;i<sizeof( missionTargetTable )/sizeof( missionTargetTable[0] );i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= "\\Quest\\";
		tmpString			+= missionTargetTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cTargetTable->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 8 );
			goto _ErrEnd;
		}
	}
	//-------------------------------------------------------------
#endif

	//-------------------------------------------------------------
	// ��� title ��
	AddMap( "title" );
	CExcelTextTable* cTitleTbl	= new CExcelTextTable;	//( 300, 50 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cTitleTbl ) );
	arTableList.push_back( "title.txt" );
	for ( int i=0;i<sizeof( titleTable )/sizeof( titleTable[0] );i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= titleTable[i];
		tmpString			+= szExcelTableSuffix;
		// produce_wnd_type
		if ( !cTitleTbl->ReadExcelTable( tmpString ) )
		{
			nErr |= ( 1 << 9 );
			goto _ErrEnd;
		}
	}
	//-------------------------------------------------------------

	//-------------------------------------------------------------
	// ��� born ��
	AddMap( "born" );
	CExcelTextTable* cBornTbl	= new CExcelTextTable;	//( 300, 50 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cBornTbl ) );
	arTableList.push_back( "scene_start.txt" );
	for ( int i=0;i<sizeof( bornTable )/sizeof( bornTable[0] );i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= bornTable[i];
		tmpString			+= szExcelTableSuffix;
		// produce_wnd_type
		if ( !cBornTbl->ReadExcelTable( tmpString ) )
		{
			nErr |= ( 1 << 10 );
			goto _ErrEnd;
		}
	}
	//-------------------------------------------------------------
	// ��� face ��
	AddMap( "face" );
	CExcelTextTable* cEttFace	= new CExcelTextTable;	//( 500, 50 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttFace) );
	arTableList.push_back( "face.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= faceTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttFace->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 11 );
			goto _ErrEnd;
		}
	}

	//-------------------------------------------------------------
	// ��� scene ��
	AddMap( "scene" );
	CExcelTextTable* cEttScene	= new CExcelTextTable;	//( 500, 50 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttScene ) );
	arTableList.push_back( "scene.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= sceneTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttScene->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 12 );
			goto _ErrEnd;
		}
	}

	////-------------------------------------------------------------
	//// ��� stauts ��
	//AddMap( "status" );
	//CExcelTextTable* cEttStatus	= new CExcelTextTable;	//( 500, 50 ) ;
	//arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttStatus ) );
	//arTableList.push_back( "status.txt" );
	//for ( int i=0;i<1;i++ )
	//{	
	//	CString	tmpString	= strPhyDir;
	//	tmpString			+= szExcelTableDir;
	//	tmpString			+= statusTable[i];
	//	tmpString			+= szExcelTableSuffix;
	//	if ( !cEttStatus->ReadExcelTable( tmpString ) )	
	//	{
	//		nErr |= ( 1 << 13 );
	//		goto _ErrEnd;
	//	}
	//}
	// ��� skill_status ��add by maoyongbin[2008-12-15]
	//----------------------------------------------------------------
	AddMap( "skill_status" );
	CExcelTextTable* cEttSkillStatus = new CExcelTextTable; // (6,5)
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttSkillStatus ) );
	arTableList.push_back( "skill_status.txt" );
	for( int i = 0; i < 1; i++ )
	{
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= skillStatusTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttSkillStatus->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 14 );
			goto _ErrEnd;
		}	
	}
	//-----------------------------------------------------------------

	// ��� changePartyTable ��add by czg[2008-12-16]
	//----------------------------------------------------------------
	AddMap( "ChangeParty" );
	CExcelTextTable* cEttChangePartyStatus = new CExcelTextTable; // (6,5)
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttChangePartyStatus ) );
	arTableList.push_back( "ChangeParty.txt" );
	for( int i = 0; i < 1; i++ )
	{
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= changePartyTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttChangePartyStatus->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 15 );
			goto _ErrEnd;
		}	
	}
	//-----------------------------------------------------------------
//-------------------------------------------------------------------------------
//���prestige ��
	AddMap( "prestige" );
	CExcelTextTable* cEttPrestige	= new CExcelTextTable;	//( 500, 50 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttPrestige ) );
	arTableList.push_back( "prestige.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= prestigeTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttPrestige->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 16 );
			goto _ErrEnd;
		}
	}

//���Produce_Ability��
	AddMap( "Produce_Ability" );
	CExcelTextTable* cEttProduceAbility	= new CExcelTextTable;	//( 500, 50 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttProduceAbility ) );
	arTableList.push_back( "Produce_Ability.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= produceAbiTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttProduceAbility->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 17 );
			goto _ErrEnd;
		}
	}

	//���produce_icon��
	AddMap( "produce_icon" );
	CExcelTextTable* cEttProduceIcon	= new CExcelTextTable;	//( 500, 50 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cEttProduceIcon ) );
	arTableList.push_back( "produce_icon.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= produceIconTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cEttProduceIcon->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 18 );
			goto _ErrEnd;
		}
	}


	//���bpcollect��
	AddMap( "bpcollect" );
	CExcelTextTable* cbpcollect	= new CExcelTextTable;	//( 500, 50 ) ;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( cbpcollect ) );
	arTableList.push_back( "bpcollect.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= factionCollectionTaskTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !cbpcollect->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 19 );
			goto _ErrEnd;
		}
	}

	//���produce_icon�� add by xsm for С���� [2009.02.16]
	AddMap( "Secretary" );
	CExcelTextTable* Secretary_Excel = new CExcelTextTable;	
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( Secretary_Excel ) );
	arTableList.push_back( "Secretary.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= Secretary[i];
		tmpString			+= szExcelTableSuffix;
		if ( !Secretary_Excel->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 20 );
			goto _ErrEnd;
		}
	}
//Produce_H  // add by xsm for ����
	AddMap( "Produce_H" );
	CExcelTextTable* Produce_H_Excel = new CExcelTextTable;	
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( Produce_H_Excel ) );
	arTableList.push_back( "Produce_H.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= Produce_H[i];
		tmpString			+= szExcelTableSuffix;
		if ( !Produce_H_Excel->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 21 );
			goto _ErrEnd;
		}
	}
//effort // add by xsm for �ɾ� ��ʾ tooltip
	AddMap( "effort" );
	CExcelTextTable* Effort_Excel = new CExcelTextTable;	
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( Effort_Excel ) );
	arTableList.push_back( "effort.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= Effort[i];
		tmpString			+= szExcelTableSuffix;
		if ( !Effort_Excel->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 22 );
			goto _ErrEnd;
		}
	}
// Item_Pic // add by xsm for ����
	AddMap( "Item_Pic" );
	CExcelTextTable* Item_Pic_Excel = new CExcelTextTable;	
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( Item_Pic_Excel ) );
	arTableList.push_back( "item_pic.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= Item_Pic[i];
		tmpString			+= szExcelTableSuffix;
		if ( !Item_Pic_Excel->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 23 );
			goto _ErrEnd;
		}
	}
// Gambling_H guochenshu_April �ϻ���
	AddMap( "Gambling_H" );
	CExcelTextTable* Gamble_Machine_Excel = new CExcelTextTable;	
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( Gamble_Machine_Excel ) );
	arTableList.push_back( "Gambling_H.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= GambleMachine[i];
		tmpString			+= szExcelTableSuffix;
		if ( !Gamble_Machine_Excel->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 24 );
			goto _ErrEnd;
		}
	}

	// ��� role ��
	AddMap( "Role" );
	CExcelTextTable* roleTbl	= new CExcelTextTable;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( roleTbl ) );
	arTableList.push_back( "role.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= RoleTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !roleTbl->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 26 );
			goto _ErrEnd;
		}
	}
	// ��� npc ��
	AddMap( "NPC" );
	CExcelTextTable* npcTbl	= new CExcelTextTable;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( npcTbl ) );
	arTableList.push_back( "npc_x.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= NPCTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !npcTbl->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 27 );
			goto _ErrEnd;
		}
	}

	// ��� װ���� ��
	AddMap( "item_part" );
	CExcelTextTable* item_part_table	= new CExcelTextTable;
	arExcelTableList.push_back( const_cast<CExcelTextTable*>( item_part_table ) );
	arTableList.push_back( "item_part.txt" );
	for ( int i=0;i<1;i++ )
	{	
		CString	tmpString	= strPhyDir;
		tmpString			+= szExcelTableDir;
		tmpString			+= itemPartTable[i];
		tmpString			+= szExcelTableSuffix;
		if ( !item_part_table->ReadExcelTable( tmpString ) )	
		{
			nErr |= ( 1 << 28 );
			goto _ErrEnd;
		}
	}
//------------------------------------------------------------------------------------
	arTableList.push_back( "zhanghua" );
	if ( !InitStringCheck( ) )
	{
		arTableList.push_back( "StringCheck" );
		nErr	|= ( 1 << 25 );
		goto _ErrEnd;
	}

	return TRUE;

_ErrEnd:

	WriteInfo( "�򿪱�ʧ�ܣ���ʧ������( %d, %s ).", nErr, arTableList.operator []( arTableList.size() - 1 ).c_str() );
	return FALSE;
}

BOOL ExcelNameSpace::ClearAllExcelTable()
{
	for ( itExcelTableList	= arExcelTableList.begin();
			itExcelTableList != arExcelTableList.end();
			++itExcelTableList)
	{
		(*itExcelTableList)->Reset();
		delete (*itExcelTableList);
	}

	arExcelTableList.clear();
	arTableList.clear();
	
	// �ͷ�
	UnInitStringCheck( );

	return TRUE;
}

BOOL ExcelNameSpace::OpenExcelTable( CString const& strName )
{
	if ( arExcelTableList.empty() )
	{
		DISABLE_ASSERT( "��Ϊ�գ���û�б����� ");
		return FALSE;
	}
	cpCurExcelTable		= arExcelTableList[ GetNameIndex( (LPSTR)(LPCSTR)strName ) ];
	strCurExcelTbName	= strName;
	
	/*
#ifdef _DEBUG
	if ( -1 == cpCurExcelTable->CheckDup( ITEM_ALIAS ) )
	{
		// UIASSERT( 0," �������ظ��У���ȷ��Ҫ������" );
	}
#endif
	*/

	return TRUE;
}

VOID ExcelNameSpace::CloseExcelTable()
{
	cpCurExcelTable		= NULL;
	strCurExcelTbName.clear();
	nCurLine					= -1;
}

BOOL ExcelNameSpace::OpenPropTable()
{
	nTblName			= TBL_ITEM;
	return OpenExcelTable( "item" );
}

BOOL ExcelNameSpace::OpenItemPartTable()
{
	nTblName			= TBL_ITEM_PART;
	return OpenExcelTable( "item_part" );
}

BOOL ExcelNameSpace::OpenSkillTable()
{
	nTblName			= TBL_SKILL;
	return OpenExcelTable( "skill" );
}

BOOL ExcelNameSpace::OpenSubSkillTable()
{
	nTblName			= TBL_SUBSKILL;
	return OpenExcelTable( "subskill" );
}

BOOL ExcelNameSpace::OpenLearnSkillTable()
{
	nTblName			= TBL_LEARNSKILL;
	return OpenExcelTable( "learnskill" );
}
BOOL ExcelNameSpace::OpenFaceTable()
{
	nTblName			= TBL_FACE;
	return OpenExcelTable( "face" );
}

BOOL ExcelNameSpace::OpenSceneTable()
{
	nTblName			= TBL_SCENE;
	return OpenExcelTable( "scene" );
}

BOOL ExcelNameSpace::OpenStatusTable( )
{
	nTblName			= TBL_STATUS;
	return OpenExcelTable( "status" );
}

// add by maoyongbin[2008-12-15]
BOOL ExcelNameSpace::OpenSkillStatusTable( )
{
	nTblName			= TBL_SKILLSTATUS;
	return OpenExcelTable( "skill_status" );
}
// add by xsm [2009.03.16]
BOOL ExcelNameSpace::OpenProduce_HTable ( )
{
	nTblName			= TBL_PRODUCE_H;
	return OpenExcelTable( "Produce_H" );

}
// add by xsm [2009.03.17]
BOOL ExcelNameSpace::OPenItem_PicTable  ( )
{
	nTblName			= TBL_ITEM_PIC;
	return OpenExcelTable( "Item_Pic" );
}
// add by xsm [2009.03.25]
BOOL ExcelNameSpace::OPenEffortTable    ( )
{
	nTblName			= TBL_EFFORT;
	return OpenExcelTable( "effort" );

}
//add by guochenshu [2009-3-26]
BOOL ExcelNameSpace::OPenGambling_HTable ( )
{
	nTblName			= TBL_GAMBLE_MACHINE;
	return OpenExcelTable( "Gambling_H" );
}
// add by czg[2008-12-16]
BOOL ExcelNameSpace::OpenChangeLifeTable( )
{
	nTblName			= TBL_CHANGE_PARTY;
	return OpenExcelTable( "ChangeParty" );
}

BOOL ExcelNameSpace::OpenProduceTable()
{
	nTblName			= TBL_PRODUCE;
	return OpenExcelTable( "produce_c" );
}

BOOL ExcelNameSpace::OpenProduceTypeTable()
{
	nTblName			= TBL_PRODUCE_TYPE;
	return OpenExcelTable( "produce_type_c" );
}

BOOL ExcelNameSpace::OpenProduceWndTypeTable()
{
	nTblName			= TBL_MAKE_WND_TYPE;
	return OpenExcelTable( "produce_wnd_type" );
}

BOOL ExcelNameSpace::OpenMissionTable()
{
	nTblName			= TBL_MISSION;
	return OpenExcelTable( "mission" );
}

BOOL ExcelNameSpace::OpenMissionTargetTable()
{
	nTblName			= TBL_MISSION_TARGET;
	return OpenExcelTable( "target" );
}

BOOL ExcelNameSpace::OpenTitleTable()
{
	nTblName			= TBL_TITLE;
	return OpenExcelTable( "title" );
}

BOOL ExcelNameSpace::OpenBornTable()
{
	nTblName			= TBL_BORN;
	return OpenExcelTable( "born" );
}

BOOL ExcelNameSpace::OpenPrestigeTable()
{
	nTblName			= TBL_PRESTIGE;
	return OpenExcelTable("prestige");
}

BOOL ExcelNameSpace::OpenProduceAbiTable()
{
	nTblName			=	TBL_PRODUCEABILITY;
	return OpenExcelTable( "Produce_Ability" );
}

BOOL ExcelNameSpace::OpenProduceIconTable()
{
	nTblName			=	TBL_PRODUCE_ICON;
	return OpenExcelTable( "produce_icon" );
}
BOOL ExcelNameSpace::OpenFCTaskTable()
{
	nTblName			=	TBL_FCTASK;
	return OpenExcelTable( "bpcollect" );
}
BOOL ExcelNameSpace::OpenSecretaryTable()
{
	nTblName            =   TBL_SECRETARY;
	return OpenExcelTable("Secretary");

}
BOOL ExcelNameSpace::OpenRoleTable()
{
	nTblName            =   TBL_ROLE;
	return OpenExcelTable("Role");

}
BOOL ExcelNameSpace::OpenNPCTable()
{
	nTblName            =   TBL_NPC;
	return OpenExcelTable("NPC");

}
int ExcelNameSpace::GetIntFromLine( int line ,int row )
{
	POINTER_ASSERT( cpCurExcelTable );
	if ( !cpCurExcelTable ||line > cpCurExcelTable->GetMaxLn() )
	{
		return 0;
	}

	int	nInt;	
	cpCurExcelTable->Get( line , row, nInt );
	return nInt;
}

float ExcelNameSpace::GetFloatFromLine( int line ,int row )
{
	POINTER_ASSERT( cpCurExcelTable );
	if ( !cpCurExcelTable ||line > cpCurExcelTable->GetMaxLn() )
	{
		return 0.0F;
	}

	float fFloat;
	cpCurExcelTable->Get( line , row, fFloat );
	return fFloat;
}

CString ExcelNameSpace::GetStringFromLine( int line ,int row )
{
	POINTER_ASSERT( cpCurExcelTable );
	if ( !cpCurExcelTable ||
		line > cpCurExcelTable->GetMaxLn() )	
	{
		return _TEXT("");
	}

	// 128 is enough,I thought..
	char	tmpStr[128];
	cpCurExcelTable->Get( line , row , tmpStr, 128 );
	return tmpStr;
}

int ExcelNameSpace::GetIntFromCurLine( int row )
{
	UIASSERT( nCurLine != -1 ,"  ��ǰû���С��� ");
	if ( nCurLine == -1 )	return 0;

	return GetIntFromLine( nCurLine, row );
}

float ExcelNameSpace::GetFloatFromCurLine( int row )
{
	UIASSERT( nCurLine != -1 ,"  ��ǰû���С��� ");
	if ( nCurLine == -1 )	return 0;

	return GetFloatFromLine( nCurLine, row );
}

CString ExcelNameSpace::GetStrFromCurLine	( int row )
{	
	UIASSERT( nCurLine != -1 ,"  ��ǰû���С��� ");
	if ( nCurLine == -1 )	return _TEXT("");

	return GetStringFromLine( nCurLine, row );
}

int ExcelNameSpace::GetLineFromAlias( CString const& strAlias )
{
	if ( !cpCurExcelTable )	
	{
		DISABLE_ASSERT( "��ǰû�б��򿪣�" );
		return -1;
	}

	// for test ..
	nCurLine		= -1;
	if ( !strAlias.IsEmpty() )
	{
		switch ( nTblName ){
			case TBL_ITEM:
				nCurLine		= cpCurExcelTable->FindLn( ITEM_ALIAS, strAlias );
				break;

			case TBL_SKILL:
				nCurLine		= cpCurExcelTable->FindLn( SKILL_ALIAS, strAlias );
				break;

			case TBL_SUBSKILL:
				nCurLine		= cpCurExcelTable->FindLn( SUB_SKILL_ALIAS, strAlias );
				break;

			case TBL_LEARNSKILL:
				nCurLine		= cpCurExcelTable->FindLn( LEARN_ALIAS, strAlias );
				break;

			case TBL_PRODUCE:
				nCurLine		= cpCurExcelTable->FindLn( MAKE_INDEX, strAlias );
				break;

			case TBL_PRODUCE_TYPE:
				nCurLine		= cpCurExcelTable->FindLn( MAKE_TYPE_SUB_SKILL_ALIAS, strAlias );
				break;

			case TBL_MAKE_WND_TYPE:
				nCurLine		= cpCurExcelTable->FindLn( MAKE_SKILL_ALIAS, strAlias );
				break;

			case TBL_MISSION:
				nCurLine		= cpCurExcelTable->FindLn( MISSION_ALIAS, strAlias );
				break;

			case TBL_MISSION_TARGET:
				nCurLine		= cpCurExcelTable->FindLn( TARGET_ALIAS, strAlias );
				break;
			case TBL_PRESTIGE:
				nCurLine		= cpCurExcelTable->FindLn( PRES_LEVEL, strAlias );
				break;
			case TBL_PRODUCEABILITY:
				nCurLine		= cpCurExcelTable->FindLn( MAKE_LEVEL, strAlias );
				break;

			case TBL_ITEM_PIC:
				nCurLine		= cpCurExcelTable->FindLn( 0, strAlias );
				break;

			case TBL_STATUS:
				nCurLine		= cpCurExcelTable->FindLn( 1, strAlias );
				break;

			case TBL_ITEM_PART :
				nCurLine		= cpCurExcelTable->FindLn(0, strAlias);
				break;

			default:
				DISABLE_ASSERT( "û��ָ�����б�!" );
		}
	}

	// ���û���ҵ��еĻ�
	if ( nCurLine == -1 )
	{

		DISABLE_ASSERT( "�ҵ���ʧ�ܣ���ϸ��Ϣ���Լ���LOG��");
		WriteInfo("�ҵ���ʧ�ܣ������֣�%d(%s)��Ѱ�ұ�����%s",nTblName,arTableList[ nTblName ].c_str(),strAlias );
		return -1;
	}

	return nCurLine;
}

int ExcelNameSpace::GetLineFromAttr( int attr, int value )
{
	POINTER_ASSERT( cpCurExcelTable );
	if ( !cpCurExcelTable )	return -1;

	nCurLine		= cpCurExcelTable->FindLn( attr, value );

	// ���û���ҵ��еĻ�д��LOG�ļ�
	//if ( nCurLine == -1 )
	//{
	//	DISABLE_ASSERT( "�ҵ���ʧ�ܣ���ϸ��Ϣ���Լ���LOG��");
	//	WriteInfo("�ҵ���ʧ�ܣ������֣�%d(%s)��Ѱ�ұ�����%d",nTblName,arTableList[ nTblName ].c_str(),value );
	//	return -1;
	//}
	return nCurLine;
}

int ExcelNameSpace::GetLineFromAttr( int attr,CString const& strValue )
{
	POINTER_ASSERT( cpCurExcelTable );
	if ( !cpCurExcelTable )	return -1;

	nCurLine		= cpCurExcelTable->FindLn( attr, strValue );

	//// ���û���ҵ��еĻ�д��LOG�ļ�
	//if ( nCurLine == -1 )
	//{
	//	DISABLE_ASSERT( "��Ҫ�������и�����û���ҵ���" );
	//	WriteInfo("�ҵ���ʧ�ܣ������֣�%d(%s)��Ѱ�ұ�����%s",nTblName,arTableList[ nTblName ].c_str(),strValue );
	//	return -1;
	//}
	return nCurLine;
}

int ExcelNameSpace::GetCurTableTotalLine()
{
	POINTER_ASSERT( cpCurExcelTable );
	if ( !cpCurExcelTable )	return 0;

	return cpCurExcelTable->GetMaxLn();
}

int ExcelNameSpace::GetCurTableTotalRow()
{
	POINTER_ASSERT( cpCurExcelTable );
	if ( !cpCurExcelTable )	return 0;

	return cpCurExcelTable->GetMaxCol();
}

void ExcelNameSpace::ResetLine()
{
	nCurLine	= 0;
}

void ExcelNameSpace::SetLine( int line )
{
	nCurLine	= line;
	if ( ( nCurLine < 0 ) || ( nCurLine >= GetCurTableTotalLine() ) )
	{
		DISABLE_ASSERT( " �����õķ�Χ�Ѿ����ڱ��� " );
		WriteInfo( "out of table(%s) range..line = (%d)", strCurExcelTbName.c_str( ), line );
		nCurLine	= 0;
	}
}

int ExcelNameSpace::GetLine()
{
	return nCurLine;
}

int	ExcelNameSpace::NextLine	()
{
	nCurLine++;
	if ( nCurLine >= GetCurTableTotalLine() )
		nCurLine	= GetCurTableTotalLine();

	return nCurLine;
}

int	ExcelNameSpace::PrevLine()
{
	if ( nCurLine > 0 )
		nCurLine--;

	return nCurLine;
}
