#ifndef	_TABLE_OPERATOR__
#define	_TABLE_OPERATOR__

//
// ���������
///////////////////////////////////////////////////
// �ͻ��˵ĺܶ����ݶ�������excel������txt�ı�����
// ��Щ����Ҫ�ɲ߻���д�����������������Ժ͹���֮��
// ������Ҫ��ȡ��Щ��Ȼ�����
// (*)ʹ�õ�ʱ����Open��Ȼ��SetLine�����GetLine����
// (*)��UIFunc����һЩ��װ�õĶ��ض���ĺ�������ʱ����Բο�
//
//

enum TBL_ITEM_SHOW{		// ���߱�
	ITEM_TYPE				= 0,					// ��������			
	ITEM_SUIT_ID,									// ��װID
	ITEM_TYPE_NAME,									// ��������
	ITEM_SUIT_NAME,									// ��װ����
	ITEM_INTRO,										// ˵��
	ITEM_ALIAS,										// ����
	ITEM_TYPE_NAME_ID,								// ��������ID
	ITEM_SUIT_NAME_ID,								// ��װ����ID
	ITEM_INTRO_ID,									// ˵����ϢID
	ITEM_LEVEL,										// Ʒ��
	ITEM_LEVEL_NEED,								// �ȼ�����
	ITEM_LEVEL_NEED_INC,							// �ȼ���������
	ITEM_SPECIAL_NEED,								// ����Ҫ�� lvlong
	ITEM_PRESTIGE_CAMP_NEED,						// ��������Ҫ��
	ITEM_PRESTIGE_LEV_NEED,							// ������ֵ
	ITEM_FE,										// ����
	ITEM_DURATION,									// �;�
	ITEM_DURATION_ADD,								// �;�����
	ITEM_PROPERTY_1,								// ����ֵ1
	ITEM_PROPERTY_1_NUM,							// ����ֵ1��ֵ
	ITEM_PROPERTY_1_NUM_INC,						// ����ֵ1����
	ITEM_PROPERTY_2,								// ����ֵ2
	ITEM_PROPERTY_2_NUM,							// ����ֵ2��ֵ
	ITEM_PROPERTY_2_NUM_INC,						// ����ֵ2����
	ITEM_NORMAL_PRICE,								// �����۸�
	ITEM_BUY_TYPE,									// ����ʽ
	ITEM_SELL_PRICE,								// ���ۼ۸�
	ITEM_SELL_TYPE,									// ���۷�ʽ
	ITEM_HIDDEN_ATTR,								// ��������
	ITEM_FIXUP_ATTR,								// �̶���ȡ����
	ITEM_USEITEM_NEED_FLAGE,						// ״̬����
	ITEM_WHOLE_PROBABILITY_ADD,						// �ܸ������� new added by zjt 2005.12.05
    ITEM_RADOM_COMPUTE_RULE,						// ����㷨���� new added by zjt 2005.12.05
	ITEM_RAND_ATTRIB_01,							// �������1
	ITEM_RAND_APPEAR_01,							// ���ּ���1
	ITEM_RAND_NONE_01,								// ��
	ITEM_RAND_ATTRIB_02,							// �������2
	ITEM_RAND_APPEAR_02,							// ���ּ���2
	ITEM_RAND_NONE_02,								// ��
	ITEM_RAND_ATTRIB_03,							// �������3
	ITEM_RAND_APPEAR_03,							// ���ּ���3
	ITEM_RAND_NONE_03,								// ��
	ITEM_RAND_ATTRIB_04,							// �������4
	ITEM_RAND_APPEAR_04,							// ���ּ���4
	ITEM_RAND_NONE_04,								// ��
	ITEM_RAND_ATTRIB_05,							// �������5
	ITEM_RAND_APPEAR_05,							// ���ּ���5
	ITEM_RAND_NONE_05,								// ��
	ITEM_PROP_PIC,									// ����ͼƬ
	ITEM_EQUIP_STRING_MALE,							// װ������
	ITEM_EQUIP_STRING_FEMALE,						// װ����Ů
	ITEM_PROP_EFFECT_01,							// װ��Ч��1
	ITEM_PROP_EFFECT_02,							// װ����Ч2
	ITEM_PROP_EFFECT_03,							// װ����Ч3
	ITEM_PROP_EFFECT_04,							// װ����Ч4
	ITEM_PROP_EFFECT_05,							// װ����Ч5
	ITEM_PROP_EFFECT_06,							// װ����Ч6
	ITEM_FECTH_TYPE,								// Я��״̬
	ITEM_EFFECT_TYPE,								// ���÷�ʽ
	ITEM_MODE_HOLE_01,								// ���׷�ʽ new added by zjt 2005.12.05
	ITEM_OWN_TYPE,									// ������ʽ
	ITEM_OWN_LEVEL,									// �ɷ�ݻ�////
	ITEM_SKILL_INDEX_01,							// ���߼�������01
	ITEM_RAND_APPEAR_07,							// ���ּ���	
	ITEM_RAND_APPEAR_ADD_07,						// ���ּ�������
	ITEM_SKILL_INDEX_02,							// ���߼�������02
	ITEM_RAND_APPEAR_08,							// ���ּ���
	ITEM_RAND_APPEAR_ADD_08,						// ���ּ�������	
	ITEM_EQUIP_STRING_MALE_2,						// ʹ�ô���
	ITEM_EQUIP_STRING_FEMALE_2,						// �뿪����ɾ��
	ITEM_WEAPON_TYPE,								// ��������
	ITEM_STRENGTH_NEED_INC,							// ���صȼ�
	ITEM_AGILITY_NEED,								// ����Ψһ
	ITEM_AGILITY_NEED_INC,							// ��Ч��
	ITEM_STAMINA_NEED,								// ����CD
	ITEM_STAMINA_NEED_INC,							// ��CD
	ITEM_INTELLIGENCE_NEED,							// ��������
	ITEM_CANNOTUSE_FLAGE,							// ���õ�״̬����(�����״̬�Ͳ���ʹ�øõ���)
	ITEM_NATIVE_NEED_TYPE,							// �츳�������
	ITEM_PARAM1,									// ʹ�ò���1(���ö�������)
	ITEM_PARAM2,									// ʹ�ò���2

	ITEM_USE_SCENE,									// ʹ�ó���
	ITEM_USE_POS,									// ʹ������
	
	// �ͻ����ò���
	COL_ITEM_CREATERNPC				= 83,	//����ˢ�¹���
	COL_ITEM_CREATERRES				= 84,	//����ˢ����Դ
	COL_ITEM_CREATERNUM				= 85,	//����ˢ�³�����������

	ITEM_EFFECT,									// ������Ч��������
};

enum TBL_SKILL_SHOW{		// ���ܱ�
	SKILL_ALIAS	= 0,								// ������
	SKILL_NAME,										// ˵��
	SKILL_NAME_ID,									// ����ID
	SKILL_INTRO_ID,									// ���ܽ���ID
	SKILL_PIC_ID,									// ����ͼ��
	SKILL_SHORTKET_TYPE,							// ��ݼ�����
	SKILL_TYPE,										// ��������
	SKILL_ATTACK_CALC_EXP,							// �������㹫ʽ
	SKILL_MALE_METHOD_ID,							// ���±༭ID
	SKILL_FEMALE_METHOD_ID,							// Ů����
	SKILL_LEVEL_UP,									// �ȼ�����
	SKILL_COOL_DOWN,
	SKILL_ACTION_RANGE,
	SKILL_MP_DELETE,
	SKILL_AP_DELETE,
	SKILL_FE_REQUIRE,								// ����Ҫ��
	SKILL_RELASKILL,								// ��������
	//{------------------------------------------------------------------
	// [2006-06-26 15:23] ����
	SKILL_ATTACK_TYPE = 32,		// �������� 0�ǲ������ܣ�ֻ�ܶԵж�Ŀ���ã�1��ֻ�ܶ��Ѻ�Ŀ����
	//------------------------------------------------------------------}
	//...
};

enum TBL_SUB_SKILL_SHOW{		// �Ӽ��ܱ�
	SUB_SKILL_ALIAS		= 0,
	SUB_MAIN_SKILL,									// ������
	SUB_SKILL_INTRO,								// ˵��
	SUB_NAME_ID,										// ����ID
	SUB_INTRO_ID,									// �򵥽���ID
	SUB_PIC_ID,										//ͼ��ID
	SUB_SKILL_STORY		= 33 ,						//�Ӽ��ܹ���ID
	// ...
};

enum TBL_SKILLLEARN_SHOW{		// ����ѧϰ��
	LEARN_ALIAS		= 0,					// ����
	LEARN_INTRO,							// ˵��
	LEARN_TYPE,								// ��������
	LEARN_SKILL_CLASS_INDEX,				// �����������		�������������
	LEARN_SKILL_INDEX,						// ��������			��������
	LEARN_SKILL_LEVEL,						// ���ܼ���
	LEARN_INTRO_ID,							// ����ID
	LEARN_NAME_ID,							// ����ID
	LEARN_CAN_BUY,							// �����Ƿ���Թ���
	LEARN_JOB,								// ����ְҵҪ��
	LEARN_MAIN_FE,							// ����Ҫ��
	LEARN_MINOR_FE,							// ������Ҫ��
	LEARN_ROLE_LEVEL,						// ��ɫ�ȼ�Ҫ��
	LEARN_MONEY,							// ��Ҫ����Ǯ
	LEARN_SKILL_POINT,						// ���ܵ�����
	LEARN_SKILL_LEVEL_NEED,					// Ҫ���ܵȼ�
	LEARN_ATTR_01,							// �����Ӽ���1
	LEARN_ATTR_02,							// �����Ӽ���2
	LEARN_ATTR_03,							// �����Ӽ���3
	LEARN_FEP_1,//�������� : ��
	LEARN_FEP_2,//m
	LEARN_FEP_3,//s
	LEARN_FEP_4,//h
	LEARN_FEP_5,//t
	LEARN_STORY,							// �Ӽ��ܵĹ���˵��
	LEARN_MINOR_FE_CLIENT	= 24,			// �ͻ��˵ĸ�����
};

enum TBL_ROLEMDL_SHOW{		// ����ģ�ͱ�
	CLASS_ID			= 0,							// ���
	ROLE_NAME,										// ��������
	ROLE_ID,											// ��ɫID
	GRAPHICS_ID,										// ͼ��ID
};

enum TBL_MAKE_SHOW{				// �����
	MAKE_INDEX		= 0,				// �����䷽����
//	MAKE_SUBSKILL_TYPE,					// ���䷽����
	MAKE_ROLE,							// �����ɫ
	MAKE_SKILL,							// ���켼��
//	MAKE_SUB_SKILL,						// �����Ӽ���
	
	MAKE_SPECIALSKILL_REQ,				// ר�޼�������	new added by zjt 2005.12.05	
	MAKE_SPECIALSKILL_ID,				// ר�޼���ID   
	MAKE_SPECIALMODE_REQ,				// ר�޼��ܷ�ʽ new added by zjt 2005.12.05



	MAKE_SKILL_LEVEL,					// ���ܵȼ�
	MAKE_PRODUCED_TYPE,					// ��Ʒ����
	MAKE_PRODUCED_ALIAS,				// ��Ʒ����
	MAKE_PRODUCED_METHOD,				// ���췽��
	MAKE_MAT_1,							// ����1
	MAKE_MAT_2,							// ����2
	MAKE_MAT_3,							// ����3
	MAKE_MAT_4,							// ����4
	MAKE_MAT_5,							// ����5
	MAKE_MAT_6,							// ����6
	MAKE_MAT_7,							// ����7
	MAKE_MAT_8,							// ����8
	MAKE_MAT_9,							// ����9

	// 2005-03-02 modified by Victor.
	// �������Ѿ���ָ��Ϊ���������ԡ��͡��۸�ϵ����
	// Begin VS specific
	// MAKE_EMPTY_0,
	// MAKE_EMPTY_1,
	MAKE_FEBIRTH_ATTR01,				// ��������1

	MAKE_APPEAR_PROBABILITY01,			// ���ָ���1 new added by zjt 2005.12.05
	MAKE_PROBABILITY_ADD01,				// ��������1 new added by zjt 2005.12.05
	MAKE_FEBIRTH_ATTR02,				// ��������2 new added by zjt 2005.12.05
	MAKE_APPEAR_PROBABILITY02,			// ���ָ���2 new added by zjt 2005.12.05
	MAKE_PROBABILITY_ADD02,				// ��������2 new added by zjt 2005.12.05
	MAKE_FEBIRTH_ATTR03,				// ��������3 new added by zjt 2005.12.05
	MAKE_APPEAR_PROBABILITY03,			// ���ָ���3 new added by zjt 2005.12.05
	MAKE_PROBABILITY_ADD03,				// ��������3 new added by zjt 2005.12.05

	MAKE_PRICE_FACTOR,					// �۸�ϵ��
	// End VS specific

	MAKE_PRACTICE_GAIN,					// �����Ȼ��  new added by zjt 2005.12.05
	MAKE_PHYSIC_EXPAND,					// ��������	   new added by zjt 2005.12.05

	MAKE_ITEM_NAME_ID,					// �䷽����ID
	MAKE_ITEM_INTRO_ID,					// �䷽˵��ID
};

enum TBL_MAKE_TYPE_SHOW{		// �����Ӽ���������
	MAKE_ROLE_TYPE	= 0,				// ��������
	MAKE_TYPE_TYPE,						// ��������
	MAKE_TYPE_SKILL_ALIAS,				// ��������������
	MAKE_TYPE_SUB_SKILL_ALIAS,			// ����������
	MAKE_TYPE_SUB_SKILL_NAMEID,			// �����Ӽ�������ID
	MAKE_TYPE,							// ��������
	MAKE_TYPE_ID,						// ������������ID
	MAKE_TYPE_PICID,					// ��������ͼƬID
	MAKE_TYPE_PICID_ZJ,					// ����ר��ͼƬID
	MAKE_EXTRACT_TYPE,					// ������������Ƿ���Ҫ����,�����Ҫ��ô��Ҫ�ľ���������ʲô?
};

enum TBL_MAKE_WND_TYPE_SHOW{	// ����������������
	MAKE_WND_TYPE	= 0,				// ���촰������
	MAKE_SKILL_ALIAS,					// ����������
};

enum TBL_MISSION_SHOW{			// �����
	MISSION_ALIAS	= 0,
	MISSION_NAME_ID,
	MISSION_DESCRIBE_ID,
	MISSION_COMPLETE_ID,
	MISSION_FAILED_ID,
	MISSION_IS_SHOW,
};

enum TBL_MISSION_TARGET_SHOW{		// ����Ŀ���
	TARGET_ALIAS			= 0,
	TARGET_DESCRIBE_ID,
	TARGET_PROMPT_ID,
	TARGET_COMPLETE_ID,
	TARGET_IS_SHOW,
		
	// ...

	TARGET_TYPE			= 7,
	TARGET_CONTENT_NAME_ID,
	TARGET_CONTENT_SUIT_ID,
	TARGET_ALIAS_ID,
	TARGET_REQUIRE_COUNT,
	TARGET_REQUIRE_LEVEL,
	TARGET_REQUIRE_TIMES,
	TARGET_NAME_ID		= 25,
	TARGET_NPC			= 26,
	TARGET_NPC_SCENCE   = 27,
};

enum TBL_PROPMDL_SHOW{
	MDL_PROP_NAME	= 0,
	MDL_PROP_ID,
	MDL_PROP_GRAPH_ID,
	MDL_GROUND_SOUND,
	MDL_DROPING_SHOUND,
	MDL_USE_SOUND,
};

enum TBL_TITLE_SHOW{		// �ƺ��б�
	TITLE_ALIAS	= 0,
	TITLE_INTRO,
	TITLE_NAME_ID = 3 ,
	TITLE_INTRO_ID,
	TITLE_LEVEL,
	TITLE_STATUS,
};

enum TBL_SCENE_SHOW{		// ���������ص�
	SCENE_BORNSITE	= 0,
	SCENE_BORNSITE_ID,
	SCENE_BORNSITE_NAME,
};

enum TBL_PRES_SHOW{			// �����б�
	PRES_LEVEL = 0,
	PRES_DOWN,
	PRES_UP,
	PRES_TITLE,
	PRES_PERTITLE1,
	PRES_PERTITLE2,
	PRES_PERTITLE3,
	PRES_PERTITLE4,
	PRES_PERTITLE5,
	PRES_ICON = 9,
	PRES_EXPRETION = 10,
};

enum TBL_PRODUCE_ABILITY{
	MAKE_LEVEL = 0,
	MAKE_PROP_LEVEL,
	MAKE_NEEDLEVEL,
	MAKE_NEEDMONEY,
	MAKE_APPRENTICEDEGREE,
	MAKE_APPRENTICEGETQEO,
	MAKE_APPRENTICESTRENGTHQEO,
	MAKE_ARTISANDEGREE,
	MAKE_ARTISANGETQEO,
	MAKE_ARTISANSTRENGTHQEO,
	MAKE_EXPERTDEGREE,
	MAKE_EXPERTGETQEO,
	MAKE_EXPERTSTRENGTHQEO
};

enum TBL_PRODUCE_ICON{
	PRODUCE_SUB_SKILL_NAME = 0,
	PRODUCE_SKILL_NAME,
	PRODUCE_INFO,
	PRODUCE_ID,
	PRODUCE_INTRODUCTION,
	PRODUCE_ICON1,
	PRODUCE_ICON2,
};


enum TBL_FCTASK{
	FCTASK_INDEX = 0,
	FCTASK_DESC,
	FCTASK_TYPE,
	FCTASK_PROP_QUALITY,
	FCTASK_PROP_LEVEL,
	FCTASK_PROP_COUNT,
	FCTASK_TIME,
	FCTASK_ACTIVITY_ADD,
	FCTASK_ACTIVITY_REDUCE
};
namespace ExcelNameSpace{

	BOOL		InitAllExcelTable			();
	BOOL		ClearAllExcelTable			();

	BOOL		OpenExcelTable				( CString const& strName );
	VOID		CloseExcelTable				();


	BOOL		OpenPropTable				();				// �򿪱�
	BOOL		OpenItemPartTable			();
	BOOL        OpenFaceTable				();				// �򿪱����
	BOOL		OpenSkillTable				();
	BOOL		OpenSubSkillTable			();
	BOOL		OpenLearnSkillTable			();
	BOOL		OpenProduceWndTypeTable		();
	BOOL		OpenProduceTypeTable		();				// produce_type
	BOOL		OpenProduceTable			();				// produce..
	BOOL		OpenMissionTable			();
	BOOL		OpenMissionTargetTable		();	
	BOOL		OpenTitleTable				();
	BOOL		OpenBornTable				();
	BOOL	    OpenSceneTable				();
	BOOL		OpenStatusTable				();
	BOOL		OpenSkillStatusTable		();				// �򿪼���״̬��add by maoyongbin[2008-12-15]
	BOOL		OpenChangeLifeTable			();				// �򿪱����add by czg[2008-12-16]

	BOOL        OpenPrestigeTable			();				//added by zjt for ��������
	BOOL		OpenProduceAbiTable			();				//added by zjt for �������
	BOOL		OpenProduceIconTable		();				//added by wangxi for ���켼�ܽ���
	BOOL		OpenFCTaskTable				();				//���������ռ���

	BOOL        OpenSecretaryTable          ();             //add by xsm for С������ʾ����
	BOOL        OpenProduce_HTable          ();             //add by xsm for �������
	BOOL        OPenItem_PicTable           ();             //add by xsm for ��õ���ͼƬID
	BOOL        OPenEffortTable             ();             //add by xsm for ��óɾ�ToolTip
	BOOL		OPenGambling_HTable			();				//�ϻ��� add by guochenshu_April
	BOOL		OpenRoleTable				();				//role.txt
	BOOL		OpenNPCTable				();				//npc_x.txt

	int			GetLineFromAlias			( CString const& strAlias );
	int			GetLineFromAttr				( int attr, int value );
	int			GetLineFromAttr				( int attr,CString const& strValue );

	int			GetCurTableTotalLine		();						// �õ�������
	int			GetCurTableTotalRow			();

	void		ResetLine					();						// һЩ�в���
	void		SetLine						( int line );
	int			GetLine						();
	int			NextLine					();
	int			PrevLine					();

	int			GetIntFromCurLine			( int row );			// �õ�����
	float		GetFloatFromCurLine			( int row );
	CString		GetStrFromCurLine			( int row );

	int			GetIntFromLine				( int line, int row );
	float		GetFloatFromLine			( int line, int row );
	CString		GetStringFromLine			( int line, int row );

};






using namespace ExcelNameSpace;		// ���̵ֽ���Ҫusing��.h��


#endif









//
// Jun 18th , 2004	EL.AHong.F	<. money ! money .>
// ��ʼ��Ӷ�ȡExcelTable�ĺ���
// ��Ϊʹȫ�ֹ��ܺ��������˵İ����Ǽ������ֿռ�������
// ĳЩ���Ե�ȫ�ֺ���

//
// Jun 18th , 2004	EL.AHong.F
// ����MFC��CMap������û�е�����ʾ��һŭ֮��ʹ����STL.
// ����stl�еĶ�����mfc�����ݡ�����������ַ���ҵʹ���˱�׼��string..
// �����жӡ�����ж�ʹ��MFC�����˸о�������һ�¡�
// Ҳ����˵����û����MFC̫��Ķ���

//
// Jun 18th , 2004	EL.AHong.F
// ���˼�룬��һ��Excel���жӺ�һ��������(map.)
// ��ȡ��ʱ�����ȴ��������еõ���Ӧ���ֵ�����
// ���������ʵ����Excel���ж��е��±�
// ����STL�е�map��һ���ִ������������Ӵ�ƥ���ʱ���ٶȻ�졣

//
// Jun 20th , 2004	EL.AHong.F	<. Wish E .>
// �������˴��룬�����ҵ���������eax����=0�Ĵ���
// ԭ����Om SDK�������Ȼ����ģ�棬���ǽ��޻��ǳ�ʼ����
// �ڹ������棬û���ĵ�����ά�������Ƕ�ô���˾�ɥ����
// ��ô���˵�����ʱ��Ҫ��ʼ����С����ô��ģ����ʲô�����أ�

//
// Jun 20th , 2004	EL.AHong.F
// ����tb�����͡�ʹ��һ�����ͱ�ʾ����ֱ�ӷ����п�����Ҫ���

//
// Jun 20th , 2004	EL.AHong.F
// ���ڷ��ؽӿ���CString,Ϊ�˶��Ᵽ��һ�£�û�а취��
// ���ǻ�������һ�㣬���ܹ������Ҳ����CString��std::stringҪǿ��
// ��㲻���ϲ��У�

//
// Jun 20th , 2004	EL.AHong.F
// ��excelTable����DataCenter����������Ӧ����������
// �����뵽һ���Ż��Ǿ���DataCenterֻ�Ǽ򵥵Ĵ�һ�������ӳ��
// �����Ҫ�ľ͵��������ҡ����������Ƕ��뵽�ڴ�ġ�
// ��һ�����ң���2�ξ��ǵ�����ĳ��λ�õ�ӳ�䡭��
// �������ʱ�������ɣ�������Ǻܹؼ�

//
// Jun 20th , 2004	EL.AHong.F
// ��Ϊ��ͳ�ʼ��һ�Σ�������û���˳���Ϸ֮ǰ������û�б�Ҫ����
// ��ô��õ������ž���������ݿ���Ҳ����DataMng������

//
// Jun 23th , 2004	EL.AHong.F
// ����˼���SHOW��BTW:�������ٶ�Խ��Խ���ˣ���

//
// Jun 28th , 2004	EL.AHong.F
// ����CloseAllExcelTable����̫�������ˡ�
// �رղ������ͷŰ����ĳ�ClearAllExcelTable..

// 
// Jun 28th , 2004	EL.AHong.F
// ���Ӽ���ѧϰ�б�

//
// Jul 05th , 2004	EL.AHong.F
// �뷨����DlgText�Ĳ���Ҳ������һ�������ı������С�
// �������ڵı���ϱȽ϶࣬���Կ�����ȫ�����붼��������С�

//
// Jul 07th , 2004	EL.AHong.F	<. money ! money .>
// ��� RoleMdl ��

//
// Jul 16th , 2004	EL.AHong.F	<. W E .>
// ������Ѿ�������һ���ع����ˡ�
// ��Ϊһ��ʼ��ʹ�õ������ռ������Ȼ�����˵Ҫ���ɿع�����
// ����ʱû�����

//
// Jul 16th , 2004	EL.AHong.F	<. W E .>
// Ҫע�����������кź��ı����кŲ�һ������Ϊ�ı�������к�
// ����û�б��0�У��������Ǵӵ�0�п�ʼ

//
// Sep 09th , 2004	EL.AHong.F	<. money ! money .>
// ���໰����ϵͳ������ϵͳ��������~��
// ����һС��ʱ��Ĺ۲췢����һ���ع����е�ʱ��
// ȷʵ���ȫ�ֺ��á�

//
// Sep 09th , 2004	EL.AHong.F	<. M E .>
// ��������䣨�ǣգ��ңġ��ạ̃��գӣţӣ��ж�ָ�롣

//
// Dec 09th , 2004	EL.AHong.F	<. M E .>
// �ײ��޸���excel��Ķ�ȡ��ʽ
// ����ȥ���˾�̬����Ŀռ䡣

//
// Dec 12th , 2004	EL.AHong.F	<. M E .>
// �����Om����ķǷ��ַ������κ���
