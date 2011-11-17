#ifndef	_TABLE_OPERATOR__
#define	_TABLE_OPERATOR__

//
// 读表管理器
///////////////////////////////////////////////////
// 客户端的很多数据都放在由excel导出的txt文本表中
// 这些表主要由策划填写，里面大都是物件的属性和规则之类
// 程序需要读取这些表，然后计算
// (*)使用的时候先Open，然后SetLine，最后GetLine即可
// (*)在UIFunc中有一些封装好的读特定表的函数，到时候可以参考
//
//

enum TBL_ITEM_SHOW{		// 道具表
	ITEM_TYPE				= 0,					// 道具类型			
	ITEM_SUIT_ID,									// 套装ID
	ITEM_TYPE_NAME,									// 类型名称
	ITEM_SUIT_NAME,									// 套装名称
	ITEM_INTRO,										// 说明
	ITEM_ALIAS,										// 别名
	ITEM_TYPE_NAME_ID,								// 类型名称ID
	ITEM_SUIT_NAME_ID,								// 套装名称ID
	ITEM_INTRO_ID,									// 说明信息ID
	ITEM_LEVEL,										// 品质
	ITEM_LEVEL_NEED,								// 等级需求
	ITEM_LEVEL_NEED_INC,							// 等级需求增量
	ITEM_SPECIAL_NEED,								// 特殊要求 lvlong
	ITEM_PRESTIGE_CAMP_NEED,						// 购买声望要求
	ITEM_PRESTIGE_LEV_NEED,							// 声望数值
	ITEM_FE,										// 五行
	ITEM_DURATION,									// 耐久
	ITEM_DURATION_ADD,								// 耐久增量
	ITEM_PROPERTY_1,								// 属性值1
	ITEM_PROPERTY_1_NUM,							// 属性值1数值
	ITEM_PROPERTY_1_NUM_INC,						// 属性值1增量
	ITEM_PROPERTY_2,								// 属性值2
	ITEM_PROPERTY_2_NUM,							// 属性值2数值
	ITEM_PROPERTY_2_NUM_INC,						// 属性值2增量
	ITEM_NORMAL_PRICE,								// 基本价格
	ITEM_BUY_TYPE,									// 购买方式
	ITEM_SELL_PRICE,								// 出售价格
	ITEM_SELL_TYPE,									// 出售方式
	ITEM_HIDDEN_ATTR,								// 隐藏属性
	ITEM_FIXUP_ATTR,								// 固定抽取属性
	ITEM_USEITEM_NEED_FLAGE,						// 状态索引
	ITEM_WHOLE_PROBABILITY_ADD,						// 总概率增量 new added by zjt 2005.12.05
    ITEM_RADOM_COMPUTE_RULE,						// 随机算法规则 new added by zjt 2005.12.05
	ITEM_RAND_ATTRIB_01,							// 随机属性1
	ITEM_RAND_APPEAR_01,							// 出现几率1
	ITEM_RAND_NONE_01,								// 空
	ITEM_RAND_ATTRIB_02,							// 随机属性2
	ITEM_RAND_APPEAR_02,							// 出现几率2
	ITEM_RAND_NONE_02,								// 空
	ITEM_RAND_ATTRIB_03,							// 随机属性3
	ITEM_RAND_APPEAR_03,							// 出现几率3
	ITEM_RAND_NONE_03,								// 空
	ITEM_RAND_ATTRIB_04,							// 随机属性4
	ITEM_RAND_APPEAR_04,							// 出现几率4
	ITEM_RAND_NONE_04,								// 空
	ITEM_RAND_ATTRIB_05,							// 随机属性5
	ITEM_RAND_APPEAR_05,							// 出现几率5
	ITEM_RAND_NONE_05,								// 空
	ITEM_PROP_PIC,									// 道具图片
	ITEM_EQUIP_STRING_MALE,							// 装备串男
	ITEM_EQUIP_STRING_FEMALE,						// 装备串女
	ITEM_PROP_EFFECT_01,							// 装备效果1
	ITEM_PROP_EFFECT_02,							// 装备特效2
	ITEM_PROP_EFFECT_03,							// 装备特效3
	ITEM_PROP_EFFECT_04,							// 装备特效4
	ITEM_PROP_EFFECT_05,							// 装备特效5
	ITEM_PROP_EFFECT_06,							// 装备特效6
	ITEM_FECTH_TYPE,								// 携带状态
	ITEM_EFFECT_TYPE,								// 作用方式
	ITEM_MODE_HOLE_01,								// 出孔方式 new added by zjt 2005.12.05
	ITEM_OWN_TYPE,									// 认主方式
	ITEM_OWN_LEVEL,									// 可否摧毁////
	ITEM_SKILL_INDEX_01,							// 道具技能索引01
	ITEM_RAND_APPEAR_07,							// 出现几率	
	ITEM_RAND_APPEAR_ADD_07,						// 出现几率增量
	ITEM_SKILL_INDEX_02,							// 道具技能索引02
	ITEM_RAND_APPEAR_08,							// 出现几率
	ITEM_RAND_APPEAR_ADD_08,						// 出现几率增量	
	ITEM_EQUIP_STRING_MALE_2,						// 使用次数
	ITEM_EQUIP_STRING_FEMALE_2,						// 离开场景删除
	ITEM_WEAPON_TYPE,								// 武器类型
	ITEM_STRENGTH_NEED_INC,							// 贵重等级
	ITEM_AGILITY_NEED,								// 道具唯一
	ITEM_AGILITY_NEED_INC,							// 有效期
	ITEM_STAMINA_NEED,								// 道具CD
	ITEM_STAMINA_NEED_INC,							// 组CD
	ITEM_INTELLIGENCE_NEED,							// 功能索引
	ITEM_CANNOTUSE_FLAGE,							// 禁用的状态索引(有这个状态就不能使用该道具)
	ITEM_NATIVE_NEED_TYPE,							// 天赋需求类别
	ITEM_PARAM1,									// 使用参数1(作用对象类型)
	ITEM_PARAM2,									// 使用参数2

	ITEM_USE_SCENE,									// 使用场景
	ITEM_USE_POS,									// 使用坐标
	
	// 客户端用不到
	COL_ITEM_CREATERNPC				= 83,	//道具刷新怪物
	COL_ITEM_CREATERRES				= 84,	//道具刷新资源
	COL_ITEM_CREATERNUM				= 85,	//道具刷新出东西的数量

	ITEM_EFFECT,									// 道具特效（背包）
};

enum TBL_SKILL_SHOW{		// 技能表
	SKILL_ALIAS	= 0,								// 索引名
	SKILL_NAME,										// 说明
	SKILL_NAME_ID,									// 名称ID
	SKILL_INTRO_ID,									// 技能介绍ID
	SKILL_PIC_ID,									// 技能图标
	SKILL_SHORTKET_TYPE,							// 快捷键类型
	SKILL_TYPE,										// 技能类型
	SKILL_ATTACK_CALC_EXP,							// 攻击计算公式
	SKILL_MALE_METHOD_ID,							// 招事编辑ID
	SKILL_FEMALE_METHOD_ID,							// 女招事
	SKILL_LEVEL_UP,									// 等级上限
	SKILL_COOL_DOWN,
	SKILL_ACTION_RANGE,
	SKILL_MP_DELETE,
	SKILL_AP_DELETE,
	SKILL_FE_REQUIRE,								// 五行要求
	SKILL_RELASKILL,								// 关联技能
	//{------------------------------------------------------------------
	// [2006-06-26 15:23] 边陲
	SKILL_ATTACK_TYPE = 32,		// 攻击类型 0是不良技能，只能对敌对目标用，1是只能对友好目标用
	//------------------------------------------------------------------}
	//...
};

enum TBL_SUB_SKILL_SHOW{		// 子技能表
	SUB_SKILL_ALIAS		= 0,
	SUB_MAIN_SKILL,									// 主技能
	SUB_SKILL_INTRO,								// 说明
	SUB_NAME_ID,										// 名称ID
	SUB_INTRO_ID,									// 简单介绍ID
	SUB_PIC_ID,										//图标ID
	SUB_SKILL_STORY		= 33 ,						//子技能故事ID
	// ...
};

enum TBL_SKILLLEARN_SHOW{		// 技能学习表
	LEARN_ALIAS		= 0,					// 索引
	LEARN_INTRO,							// 说明
	LEARN_TYPE,								// 技能类型
	LEARN_SKILL_CLASS_INDEX,				// 技能索引类别		（这个是左栏）
	LEARN_SKILL_INDEX,						// 技能索引			（右栏）
	LEARN_SKILL_LEVEL,						// 技能级别
	LEARN_INTRO_ID,							// 介绍ID
	LEARN_NAME_ID,							// 名称ID
	LEARN_CAN_BUY,							// 技能是否可以购买
	LEARN_JOB,								// 技能职业要求
	LEARN_MAIN_FE,							// 五行要求
	LEARN_MINOR_FE,							// 服五行要求
	LEARN_ROLE_LEVEL,						// 角色等级要求
	LEARN_MONEY,							// 需要多少钱
	LEARN_SKILL_POINT,						// 技能点消耗
	LEARN_SKILL_LEVEL_NEED,					// 要求技能等级
	LEARN_ATTR_01,							// 附属子技能1
	LEARN_ATTR_02,							// 附属子技能2
	LEARN_ATTR_03,							// 附属子技能3
	LEARN_FEP_1,//灵性需求 : 金
	LEARN_FEP_2,//m
	LEARN_FEP_3,//s
	LEARN_FEP_4,//h
	LEARN_FEP_5,//t
	LEARN_STORY,							// 子技能的故事说明
	LEARN_MINOR_FE_CLIENT	= 24,			// 客户端的辅五行
};

enum TBL_ROLEMDL_SHOW{		// 人物模型表
	CLASS_ID			= 0,							// 类别
	ROLE_NAME,										// 人物名称
	ROLE_ID,											// 角色ID
	GRAPHICS_ID,										// 图形ID
};

enum TBL_MAKE_SHOW{				// 打造表
	MAKE_INDEX		= 0,				// 打造配方索引
//	MAKE_SUBSKILL_TYPE,					// 子配方分类
	MAKE_ROLE,							// 打造角色
	MAKE_SKILL,							// 制造技能
//	MAKE_SUB_SKILL,						// 制造子技能
	
	MAKE_SPECIALSKILL_REQ,				// 专修技能需求	new added by zjt 2005.12.05	
	MAKE_SPECIALSKILL_ID,				// 专修技能ID   
	MAKE_SPECIALMODE_REQ,				// 专修技能方式 new added by zjt 2005.12.05



	MAKE_SKILL_LEVEL,					// 技能等级
	MAKE_PRODUCED_TYPE,					// 成品类型
	MAKE_PRODUCED_ALIAS,				// 成品别名
	MAKE_PRODUCED_METHOD,				// 制造方法
	MAKE_MAT_1,							// 材料1
	MAKE_MAT_2,							// 材料2
	MAKE_MAT_3,							// 材料3
	MAKE_MAT_4,							// 材料4
	MAKE_MAT_5,							// 材料5
	MAKE_MAT_6,							// 材料6
	MAKE_MAT_7,							// 材料7
	MAKE_MAT_8,							// 材料8
	MAKE_MAT_9,							// 材料9

	// 2005-03-02 modified by Victor.
	// 这两列已经被指定为“相生属性”和“价格系数”
	// Begin VS specific
	// MAKE_EMPTY_0,
	// MAKE_EMPTY_1,
	MAKE_FEBIRTH_ATTR01,				// 相生属性1

	MAKE_APPEAR_PROBABILITY01,			// 出现概率1 new added by zjt 2005.12.05
	MAKE_PROBABILITY_ADD01,				// 概率增量1 new added by zjt 2005.12.05
	MAKE_FEBIRTH_ATTR02,				// 相生属性2 new added by zjt 2005.12.05
	MAKE_APPEAR_PROBABILITY02,			// 出现概率2 new added by zjt 2005.12.05
	MAKE_PROBABILITY_ADD02,				// 概率增量2 new added by zjt 2005.12.05
	MAKE_FEBIRTH_ATTR03,				// 相生属性3 new added by zjt 2005.12.05
	MAKE_APPEAR_PROBABILITY03,			// 出现概率3 new added by zjt 2005.12.05
	MAKE_PROBABILITY_ADD03,				// 概率增量3 new added by zjt 2005.12.05

	MAKE_PRICE_FACTOR,					// 价格系数
	// End VS specific

	MAKE_PRACTICE_GAIN,					// 熟练度获得  new added by zjt 2005.12.05
	MAKE_PHYSIC_EXPAND,					// 体力消耗	   new added by zjt 2005.12.05

	MAKE_ITEM_NAME_ID,					// 配方名称ID
	MAKE_ITEM_INTRO_ID,					// 配方说明ID
};

enum TBL_MAKE_TYPE_SHOW{		// 打造子技能索引表
	MAKE_ROLE_TYPE	= 0,				// 打造人物
	MAKE_TYPE_TYPE,						// 打造类型
	MAKE_TYPE_SKILL_ALIAS,				// 打造主技能索引
	MAKE_TYPE_SUB_SKILL_ALIAS,			// 打造子索引
	MAKE_TYPE_SUB_SKILL_NAMEID,			// 打造子技能名称ID
	MAKE_TYPE,							// 打造类型
	MAKE_TYPE_ID,						// 打造类型名称ID
	MAKE_TYPE_PICID,					// 打造类型图片ID
	MAKE_TYPE_PICID_ZJ,					// 打造专精图片ID
	MAKE_EXTRACT_TYPE,					// 这个打造类型是否需要精修,如果需要那么需要的精修类型是什么?
};

enum TBL_MAKE_WND_TYPE_SHOW{	// 打造主技能索引表
	MAKE_WND_TYPE	= 0,				// 打造窗体类型
	MAKE_SKILL_ALIAS,					// 打造主技能
};

enum TBL_MISSION_SHOW{			// 任务表
	MISSION_ALIAS	= 0,
	MISSION_NAME_ID,
	MISSION_DESCRIBE_ID,
	MISSION_COMPLETE_ID,
	MISSION_FAILED_ID,
	MISSION_IS_SHOW,
};

enum TBL_MISSION_TARGET_SHOW{		// 任务目标表
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

enum TBL_TITLE_SHOW{		// 称号列表
	TITLE_ALIAS	= 0,
	TITLE_INTRO,
	TITLE_NAME_ID = 3 ,
	TITLE_INTRO_ID,
	TITLE_LEVEL,
	TITLE_STATUS,
};

enum TBL_SCENE_SHOW{		// 场景出生地点
	SCENE_BORNSITE	= 0,
	SCENE_BORNSITE_ID,
	SCENE_BORNSITE_NAME,
};

enum TBL_PRES_SHOW{			// 声望列表
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


	BOOL		OpenPropTable				();				// 打开表
	BOOL		OpenItemPartTable			();
	BOOL        OpenFaceTable				();				// 打开表情表
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
	BOOL		OpenSkillStatusTable		();				// 打开技能状态表，add by maoyongbin[2008-12-15]
	BOOL		OpenChangeLifeTable			();				// 打开变身表，add by czg[2008-12-16]

	BOOL        OpenPrestigeTable			();				//added by zjt for 声望界面
	BOOL		OpenProduceAbiTable			();				//added by zjt for 打造界面
	BOOL		OpenProduceIconTable		();				//added by wangxi for 打造技能界面
	BOOL		OpenFCTaskTable				();				//帮派任务收集表

	BOOL        OpenSecretaryTable          ();             //add by xsm for 小秘书提示功能
	BOOL        OpenProduce_HTable          ();             //add by xsm for 打造界面
	BOOL        OPenItem_PicTable           ();             //add by xsm for 获得道具图片ID
	BOOL        OPenEffortTable             ();             //add by xsm for 获得成就ToolTip
	BOOL		OPenGambling_HTable			();				//老虎机 add by guochenshu_April
	BOOL		OpenRoleTable				();				//role.txt
	BOOL		OpenNPCTable				();				//npc_x.txt

	int			GetLineFromAlias			( CString const& strAlias );
	int			GetLineFromAttr				( int attr, int value );
	int			GetLineFromAttr				( int attr,CString const& strValue );

	int			GetCurTableTotalLine		();						// 得到表属性
	int			GetCurTableTotalRow			();

	void		ResetLine					();						// 一些行操作
	void		SetLine						( int line );
	int			GetLine						();
	int			NextLine					();
	int			PrevLine					();

	int			GetIntFromCurLine			( int row );			// 得到数据
	float		GetFloatFromCurLine			( int row );
	CString		GetStrFromCurLine			( int row );

	int			GetIntFromLine				( int line, int row );
	float		GetFloatFromLine			( int line, int row );
	CString		GetStringFromLine			( int line, int row );

};






using namespace ExcelNameSpace;		// 高手教导不要using在.h中


#endif









//
// Jun 18th , 2004	EL.AHong.F	<. money ! money .>
// 开始添加读取ExcelTable的函数
// 因为使全局功能函数，个人的爱好是加上名字空间来管理
// 某些属性的全局函数

//
// Jun 18th , 2004	EL.AHong.F
// 由于MFC的CMap在这里没有弹出提示。一怒之下使用了STL.
// 害怕stl中的东西和mfc不兼容。所以这里的字符串业使用了标准的string..
// 还有列队。如果列队使用MFC会让人感觉声明不一致。
// 也就是说这里没有用MFC太多的东西

//
// Jun 18th , 2004	EL.AHong.F
// 设计思想，又一个Excel表列队和一个名字树(map.)
// 读取的时候首先从名字树中得到相应名字的索引
// 这个索引其实就是Excel表列队中的下标
// 由于STL中的map是一个字串名字树，在子串匹配的时候速度会快。

//
// Jun 20th , 2004	EL.AHong.F	<. Wish E .>
// 今天有了代码，总算找到了星期五eax老是=0的错误
// 原来是Om SDK里面的虽然不是模版，但是界限还是初始设置
// 在构造里面，没有文档或者维护的类是多么令人沮丧阿。
// 那么如果说构造的时候还要初始化大小，那么非模版有什么意义呢？

//
// Jun 20th , 2004	EL.AHong.F
// 增加tb表类型。使用一个类型标示符比直接访问列看起来要舒服

//
// Jun 20th , 2004	EL.AHong.F
// 由于返回接口是CString,为了对外保持一致，没有办法，
// 还是混杂了这一点，可能归根到底也就是CString比std::string要强大
// 这点不承认不行！

//
// Jun 20th , 2004	EL.AHong.F
// 将excelTable放入DataCenter。理论上他应该属于这里
// 而且想到一个优化那就是DataCenter只是简单的存一个这里的映射
// 如果需要的就到这里来找。反正这里是读入到内存的。
// 第一次是找，第2次就是到这里某个位置的映射……
// 等最后有时间再作吧，这个不是很关键

//
// Jun 20th , 2004	EL.AHong.F
// 因为表就初始化一次，所以在没有退出游戏之前，我们没有必要析构
// 那么最好的主意大概就是这个数据控制也交给DataMng来管理！

//
// Jun 23th , 2004	EL.AHong.F
// 添加了技能SHOW，BTW:机器的速度越来越慢了！！

//
// Jun 28th , 2004	EL.AHong.F
// 发现CloseAllExcelTable命名太不明智了。
// 关闭不等于释放啊。改成ClearAllExcelTable..

// 
// Jun 28th , 2004	EL.AHong.F
// 增加技能学习列表。

//
// Jul 05th , 2004	EL.AHong.F
// 想法，把DlgText的操作也制作成一个这样的表来进行。
// 但是现在的表耦合比较多，可以考虑再全部代码都制作后进行。

//
// Jul 07th , 2004	EL.AHong.F	<. money ! money .>
// 添加 RoleMdl 表

//
// Jul 16th , 2004	EL.AHong.F	<. W E .>
// 这个类已经很类似一个控管器了。
// 因为一开始我使用的是名空间管理，当然如果你说要做成控管器，
// 我暂时没有意见

//
// Jul 16th , 2004	EL.AHong.F	<. W E .>
// 要注意程序里面的行号和文本的行号不一样，因为文本里面标行号
// 从来没有标第0行，而程序是从第0行开始

//
// Sep 09th , 2004	EL.AHong.F	<. money ! money .>
// 把脏话屏蔽系统和命名系统都加入了~。
// 经过一小段时间的观察发现用一个控管器有的时候
// 确实会比全局好用。

//
// Sep 09th , 2004	EL.AHong.F	<. M E .>
// 改用卫语句（ＧＵＡＲＤ　ＣＬＡＵＳＥＳ）判断指针。

//
// Dec 09th , 2004	EL.AHong.F	<. M E .>
// 底层修改了excel表的读取方式
// 所以去掉了静态分配的空间。

//
// Dec 12th , 2004	EL.AHong.F	<. M E .>
// 添加了Om里面的非法字符串屏蔽函数
