#ifndef __MENU_H 
#define __MENU_H

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

void menu(void);
#define		 KEYTEST 1
#define		 KEYLOOP 2





	//存储标志
//
#define S_S   	0x80//上      =1000,0000=0x80,
#define S_X   	0x40//下      =0100,0000=0x40,
#define S_Z  	0x20//左      =0010,0000=0x20,
#define S_Y  	0x10//右      =0001,0000=0x10,
#define S_ZS  	0xa0//左上    =1010,0000=0xa0,
#define S_ZX  	0x60//左下    =0110,0000=0x60,
#define S_YS  	0x90//右上    =1001,0000=0x90,
#define S_YX  	0x50//右下    =0101,0000=0x50,
#define S_SXZY  0xf0//上下左右=1111,0000=0xf0
  








//




									//
									//************************************************************************
									//* Menu Struct:                                                         *
									//************************************************************************
#define  MENU_PASSWORD			0	//* First Bmp--------------------->(MenuID=0)                            *
									//*  ---+                                                                *
#define  MENU_CHANNEL			1	//*     |货道参数----------------->(MenuID=1)                            *
									//*     +---+                                                            *
#define  MENU_INPUTNUM			2	//*     |   |请输入编号----------->(MenuID=2)                            *
									//*     |   +--------------                                              *
#define  MENU_CHANNELNO			3	//*     |   	|货道号----------------->(MenuID=3)                      *
									//*     |   +--------------                                              *
#define  MENU_PRICE				4	//*     |   	|单价------------------->(MenuID=4)                      *
									//*     |   +--------------                                              *
#define  MENU_CHANNELLEFT		5	//*     |   	|货道存货--------------->(MenuID=5)                      *
									//*     |   +--------------                                              *									
#define  MENU_CHANNELSTAT		6	//*     |   	|货道状态--------------->(MenuID=6)                      *
									//*     |   +--------------                                              *
#define  MENU_CHANNELGOODS		7	//*     |   	|商品编号--------------->(MenuID=7)                      *
									//*     |   +--------------                                              *									
#define  MENU_ADDGOODS 			8   //*		|添货--------------------->(MenuID=7)                            *
									//*     +---+                                                            *
#define  MENU_ADDINPUTLAY		9	//*     |   |请输入编号----------->(MenuID=8)                        *
									//*     |   +--------------                                              *
#define  MENU_INPUTADDGOODS		10	//*     |   	|你确定要添货吗----------->(MenuID=8)                        *
									//*     |   	+--------------                                              *
#define  MENU_DRIVER			11	//*     |设备管理----------------->(MenuID=9)                            *
									//*     +---+                                                            *
#define  MENU_NOTESTAT			12	//*     |   |纸币器状态------------->(MenuID=10)                         *
									//*     |   +--------------                                              *
#define  MENU_COINSSTAT			13	//*     |   |硬币器状态------------->(MenuID=11)                         *
									//*     |   +--------------                                              *
#define  MENU_HOPPERSTAT1		14	//*     |   |#1找零器状态----------->(MenuID=12)                         *
									//*     |   +--------------                                              *
#define  MENU_HOPPERSTAT2		15	//*     |   |#2找零器状态----------->(MenuID=13)                         *
									//*     |   +--------------                                              *
#define  MENU_HOPPERSTAT3		16	//*     |   |#3找零器状态----------->(MenuID=14)                         *
									//*     |   +--------------                                              *
#define  MENU_GPRSTEST			17	//*     |   |手机模块状态测试------->(MenuID=20)                         *
									//*     |   +--------------                                              *
#define  MENU_SMSTEST			18	//*     |   |短消息检测------------->(MenuID=21)                         *
									//*     |   +--------------                                              *
#define  MENU_KEYTEST			19	//*     |   |按钮检测--------------->(MenuID=15)                         *
									//*     |   +--------------                                              *
#define  MENU_DOKEYTEST			20	//*     |   |	|按钮检测动作--------->(MenuID=16)                       *
									//*     |   +--------------                                              *
#define  MENU_ALLCHANNELSTAT 	21	//*     |   |货道状态--------------->(MenuID=17)                         *
									//*     |   +--------------                                              *
#define  MENU_CHANNELTEST		22	//*     |   |货道检测--------------->(MenuID=18)                         *
									//*     |   +--------------                                              *
#define  MENU_DRINPUTNUM		23  //*     |   	|请输入编号----------->(MenuID=19)                       *
									//*     |   +--------------                                              *
#define  MENU_CHTESTRESULT		24	//*     |   |货道测试结果------->(MenuID=20)                         *
									//*     |   +--------------                                              *
//#define  MENU_TRANSINPUTNUM		56	//*     |	|   |请输入编号---------->(MenuID=48)                    *
									//*     |	|   +--------------                                      *
//#define  MENU_TRANSCHANNELNO 	57	//*     |   | 		|货道编号-------->(MenuID=49)          		     *
									//*     |   |   |	+--------------                                  *
//#define  MENU_TRANSSUCCESS		58	//*     |   |   | 	|货道正常次数---->(MenuID=50)                    *
									//*     |   |   |	+--------------                                  *
//#define  MENU_TRANSDOUBT		59	//*     |   |   | 	|货道故障次数---->(MenuID=51)                    *
									//*     |   |   |	+--------------                                  *
//#define  MENU_TRANSDOUBT		59	//*     |   |   | 	|货道无货次数---->(MenuID=51)                    *
									//*     |   |   |	+--------------                                  *
#define  MENU_IRTEST			25	//*     |   |出货确认测试------->(MenuID=20)                         *
									//*     |   +--------------                                              *
#define  MENU_LCDTEST			26	//*     |   |LCD测试------->(MenuID=20)                         *
                                    //*     |   +--------------                                              *
#define  MENU_DOLCDTEST			27	//*     |       |LCD测试操作------->(MenuID=20)                         *
									//* 	|	+--------------
#define  MENU_TEMPRATURETEST	28	//*     |   |温控器测试------->(MenuID=20)                         *
									//* 	|	+--------------									
#define  MENU_DOTEMPTEST		29	//*     |       |温控器测试操作------->(MenuID=20)                         *
									//* 	|	+--------------	
#define  MENU_SELECTKEYTEST		30	//*     |   |选货按键测试------->(MenuID=20)                         *
									//* 	|	+--------------									
#define  MENU_DOSELECTKEYTEST	31	//*     |       |选货按键测试操作------->(MenuID=20)                         *									
									//*     |   +--------------                                              *	
#define  MENU_TEMPSTEST			32	//*     |   |压缩机测试------->(MenuID=20)                         *//添加压缩机控制;by gzz 20121224
                                    //*     |   +--------------                                              *//添加压缩机控制;by gzz 20121224
#define  MENU_DOTEMPSTEST		33	//*     |       |压缩机测试操作------->(MenuID=20)                         *
									//* 	|	+--------------
#define  MENU_LEDTEST			34	//*     |   |展示灯测试------->(MenuID=20)                         *//添加展示灯控制;by gzz 20121229
                                    //*     |   +--------------                                              *//添加展示灯控制;by gzz 20121229
#define  MENU_DOLEDTEST			35	//*     |       |展示灯测试操作------->(MenuID=20)                         *									
									//*     |   +--------------                                              *									
#define  MENU_FLASHLEFT			36	//*     |   |FLASH存储剩余空间------------->(MenuID=52)                           *
									//*     |   +---+                                                        *
#define  MENU_BUSINESS			37	//*     |业务参数----------------->(MenuID=22)                           *
									//*     +---+                                                            *
#define  MENU_COINVALUE			38	//*     |   |硬币器面值：--------------->(MenuID=23)                   *
									//*     |   +--------------                                              *
#define  MENU_NOTEVALUE			39	//*     |   |纸币器面值：--------------->(MenuID=24)                   *
									//*     |   +--------------                                              *
#define  MENU_CHANGEVALUE		40	//*     |   |找零器面值：--------------->(MenuID=25)                   *
									//*     |   +--------------                                              *
#define  MENU_OPENCOIN			41	//*		|	|开启硬币器：----------------->(MenuID=28)                   *
									//*     |   +--------------                                              *
#define  MENU_OPENNOTE			42	//*		|	|纸币器类型：----------------->(MenuID=28)                   *
									//*     |   +--------------                                              *
#define  MENU_OPENCHANGE		43	//*		|	|开启找零器：----------------->(MenuID=28)                   *
									//*     |   +--------------                                              *
#define  MENU_OPENREADER		44	//*		|	|读卡器类型：----------------->(MenuID=28)                   *
									//*     |   +--------------                                              *
#define  MENU_READEROFF			45	//*     |       |关闭读卡器------->(MenuID=20)                         *									
									//*     |   +--------------    
#define  MENU_READERMDB			46	//*     |       |MDB读卡器------->(MenuID=20)                         *									
									//*     |   +--------------  
#define  MENU_READERCOST		47	//*		|	|读卡器是否真正扣款----------------->(MenuID=28)                   *
									//*     |   +-------------- 									
#define  MENU_CHANGESERVER		48	//*		|	|无零币是否服务：------------->(MenuID=33)                   *	
									//*     |   +--------------   											 *
#define  MENU_NOLIMITEDBUY		49	//*     |   |允许多次购买----------------->(MenuID=26)                   *
									//*     |   +--------------                                              *
#define  MENU_RETURNCOINS		50	//*		|	|不买货退币：----------------->(MenuID=27)                   *
									//*     |   +--------------                                              *
#define  MENU_BALANCEMIN		51	//*     |   |余额下限--------------------->(MenuID=29)                   *
									//*     |   +--------------   											 *
#define  MENU_MONEYMAXIN		52	//*     |   |收币上限--------------------->(MenuID=30)                   *
									//*     |   +--------------   											 *
#define  MENU_IOU				53	//*     |   |欠条：----------------------->(MenuID=32)                   *
									//*     |   +--------------                                              *
#define  MENU_DOUBTMONEY		54	//*     |   |疑问金额--------------------->(MenuID=31)                   *
									//*     |   +--------------                                              *
#define  MENU_DECPOINTNUM		55	//*		|	|小数点位数：------------->(MenuID=33)                   *
									//*     |   +--------------                                              *
#define  MENU_SALECONFIRM		56	//*		|	|收货确认：------------->(MenuID=33)                   *
									//*     |   +--------------                                              *
#define  MENU_POSENABLE			57	//*		|	|开启POS机：------------->(MenuID=33)                   *
									//*     |   +--------------                                              *
#define  MENU_VMCPC             58  //*     |	使用pc通讯--------------->(MenuID=)                         *
                                    //*     |   +--------------                                              *
#define  MENU_LARGELCD          59  //*     |	屏幕类型 --------------->(MenuID=)                         *	
                                    //*     |   +--------------                                              *
#define  MENU_SELECTKEY         60  //*     |	使用选货按键模式--------------->(MenuID=)                         *	
                                    //*     |   +--------------                                              *   
#define  MENU_LOGOSEL           61  //*     |	使用Logo--------类型------->(MenuID=)                         *	//拼图屏幕样式;by gzz 20121106
                                    //*     |   +--------------                                              *  
#define  MENU_ADDCOLUMN         62  //*     |	自动补货--------类型------->(MenuID=)                         *	//不用手工补货功能;by gzz 20130122
                                    //*     |   +--------------                                              *   
#define  MENU_TENCOLUMN         63  //*     |	是否使用十货道的机器--------类型------->(MenuID=)              *	//是否使用十货道的机器;by gzz 20130226
                                    //*     |   +--------------                                              *                                     
#define  MENU_HOLDNOTE			64	//*		|	|是否开启暂存：------------->(MenuID=33)                   *
									//*     |   +--------------                                              *
#define  MENU_HOLDNOTE5 		65	//*     |	5元纸币暂存--------------->(MenuID=17)                         *
									//*     |   +--------------                                              *
#define  MENU_HOLDNOTE10 		66	//*     |	10元纸币暂存--------------->(MenuID=17)                         *
									//*     |   +--------------                                              *
#define  MENU_HOLDNOTE20 		67	//*     |	20元纸币暂存--------------->(MenuID=17)                         *
                                    //*     |	+--------------                                              *
									//*     |   +--------------                                              *
#define  MENU_TRANSACTIONS		68	//*     |交易记录----------------->(MenuID=34)                           *
									//*     +---+                                                            *
#define  MENU_PAYMENTS			69	//*     |   |日常货币收支------------->(MenuID=35)                           *
									//*     |   +---+                                                        *
#define  MENU_INCOME			70	//*     |   |   |货币总收入--------->(MenuID=36)                         *
									//*     |   |   +--------------                                          *
#define  MENU_NOTEINCOME		71	//*     |   |   |纸币收入----------->(MenuID=37)                         *
									//*     |   |   +--------------                                          *
#define  MENU_COINSINCOME		72	//*     |   |   |硬币收入----------->(MenuID=38)                         *
									//*     |   |   +--------------                                          *
#define  MENU_TOTALTRANS		73	//*     |   |   |交易总金额--------->(MenuID=39)                         *
									//*     |   |   +--------------                                          *
#define  MENU_TOTALCHANGE		74  //*     |   |   |找零总金额--------->(MenuID=40)                         *
									//*     |   |   +--------------                                          *
#define  MENU_COINSCHANGE1		75	//*     |   |   |#1出币枚数--------->(MenuID=41)                         *
									//*     |   |   +--------------                                          *
#define  MENU_COINSCHANGE2		76	//*     |   |   |#2出币枚数--------->(MenuID=42)                         *
									//*     |   |   +--------------                                          *
#define  MENU_COINSCHANGE3		77	//*     |   |   |#3出币枚数--------->(MenuID=43)                         *
									//*     |   |   +--------------                                          *
#define  MENU_GOODS				78	//*     |   |日常商品记录----------------->(MenuID=44)                           *
									//*     |   +---+                                                        *
#define  MENU_SUCCESSNUM		79	//*     |   |   |成功交易次数----->(MenuID=45)                           *
									//*     |   |   +--------------                                          *
#define  MENU_DOUBTNUM			80	//*     |   |   |疑问交易次数----->(MenuID=46)                           *
									//*     |   |   +--------------                                          *
#define  MENU_CHANNELTRANS		81	//*     |   |   |单货道交易统计--->(MenuID=47)                           *
									//*     |   |   +--------------                                          *
#define  MENU_TRANSINPUTNUM		82	//*     |	|   	|请输入编号---------->(MenuID=48)                    *
									//*     |	|   	+--------------                                      *
#define  MENU_TRANSCHANNELNO 	83	//*     |   | 			|货道编号-------->(MenuID=49)          		     *
									//*     |   |   |		+--------------                                  *
#define  MENU_TRANSSUCCESS		84	//*     |   |   | 		|成功交易统计---->(MenuID=50)                    *
									//*     |   |   |		+--------------                                  *
#define  MENU_TRANMONEY			85	//*     |   |   | 		|商品金额统计---->(MenuID=50)                    *
									//*     |   |   |		+--------------                                  *									
#define  MENU_TRANSDOUBT		86	//*     |   |   | 		|疑问交易统计---->(MenuID=51)                    *
									//*     |   |   |		+--------------                                  *
#define  MENU_CLEARTRANS		87	//*     |   |清除日常记录------------->(MenuID=52)                           *
									//*     |   +---+                                                        *
#define  MENU_CONFIRMATION		88	//*     |   |   |你确定吗？------->(MenuID=53)                           *
									//*     |   |   +--------------                                          *
#define  MENU_PAYMENTSJIDU		89	//*     |   |季度货币收支------------->(MenuID=35)                           *
									//*     |   +---+                                                        *
#define  MENU_INCOMEJIDU		90	//*     |   |   |货币总收入--------->(MenuID=36)                         *
									//*     |   |   +--------------                                          *
#define  MENU_NOTEINCOMEJIDU	91	//*     |   |   |纸币收入----------->(MenuID=37)                         *
									//*     |   |   +--------------                                          *
#define  MENU_COINSINCOMEJIDU	92	//*     |   |   |硬币收入----------->(MenuID=38)                         *
									//*     |   |   +--------------                                          *
#define  MENU_TOTALTRANSJIDU	93	//*     |   |   |交易总金额--------->(MenuID=39)                         *
									//*     |   |   +--------------                                          *
#define  MENU_TOTALCHANGEJIDU	94  //*     |   |   |找零总金额--------->(MenuID=40)                         *
									//*     |   |   +--------------                                          *
#define  MENU_COINSCHANGE1JIDU	95	//*     |   |   |#1出币枚数--------->(MenuID=41)                         *
									//*     |   |   +--------------                                          *
#define  MENU_COINSCHANGE2JIDU	96	//*     |   |   |#2出币枚数--------->(MenuID=42)                         *
									//*     |   |   +--------------                                          *
#define  MENU_COINSCHANGE3JIDU	97	//*     |   |   |#3出币枚数--------->(MenuID=43)                         *
									//*     |   |   +--------------                                          *
#define  MENU_GOODSJIDU			98	//*     |   |季度商品记录----------------->(MenuID=44)                           *
									//*     |   |   +--------------                                          *
#define  MENU_CHANNELTRANSJIDU	99	//*     |   |   |单货道交易统计--->(MenuID=47)                           *
									//*     |   |   +--------------                                          *
#define  MENU_TRANSINPUTNUMJIDU	100	//*     |	|   	|请输入编号---------->(MenuID=48)                    *
									//*     |	|   	+--------------                                      *
#define  MENU_TRANSCHANNELNOJIDU 101//*     |   | 			|货道编号-------->(MenuID=49)          		     *
									//*     |   |   |		+--------------                                  *
#define  MENU_TRANSSUCCESSJIDU	102	//*     |   |   | 		|成功交易统计---->(MenuID=50)                    *
									//*     |   |   |		+--------------                                  *
#define  MENU_TRANMONEYJIDU		103	//*     |   |   | 		|商品金额统计---->(MenuID=50)                    *
									//*     |   |   |		+--------------                                  *	
#define  MENU_CLEARTRANSJIDU	104	//*     |   |清除季度记录------------->(MenuID=52)                           *
									//*     |   +---+                                                        *
#define  MENU_CONFIRMATIONJIDU	105	//*     |   |   |你确定吗？------->(MenuID=53)                           *
									//*     |   |   +--------------                                          *									
#define  MENU_LANGUAGE			106	//* 	Language------------------>(MenuID=54)                           *
									//* 	---+                                                             *
#define  MENU_ENGLISH			107	//* 		|英文----------------->(MenuID=55)                           *
									//*   		+---+                                                        *
#define  MENU_CHINESE			108	//*   		|中文----------------->(MenuID=56)                           *
									//*   		+---+                                                        *
#define  MENU_RUSSIAN			109	//*   		|俄罗斯文----------------->(MenuID=56)                       *//俄罗斯文;by gzz 20121012
									//*   		+---+                                                        *														
#define  MENU_SYSTEM			110	//*     |系统参数----------------->(MenuID=57)                           *
									//*     +---+                                                            *
#define  MENU_TIMESET			111	//* 		时间设置----------------->(MenuID=58)                        *
									//* 		---+                                                         *
#define  MENU_SETYEAR			112	//* 			|年--------------------->(MenuID=59)                     *
									//*   			+---+                                                    *
#define  MENU_SETMONTH			113	//*   			|月设置----------------->(MenuID=60)                     *
									//*   			+---+                                                    *
#define  MENU_SETDATE			114	//* 			|日--------------------->(MenuID=61)                     *
									//*   			+---+                                                    *
#define  MENU_SETHOUR			115	//*   			|时设置----------------->(MenuID=62)                     *
									//*   			+---+                                                    *
#define  MENU_SETMINUTE			116	//*   			|分设置----------------->(MenuID=63)                     *
									//*   			+---+                                                    *																																
#define  MENU_CGPASSWORD		117	//*     |   |更改密码：--------->(MenuID=64)                             *
									//*     |   +--------------                                              *
#define  MENU_NEWPW				118	//*   			|新密码----------------->(MenuID=65)                     *
									//*   			+---+                                                    *
#define  MENU_NEWPWREPEAT		119	//*   			|重新输入--------------->(MenuID=66)                     *
									//*   			+---+                                                    *			
#define  MENU_GPRSONOFF			120	//*     |   |开启手机模块：------->(MenuID=67)                           *
									//*     |   +--------------                                              *
#define  MENU_SERIALNO			121	//*     |   |机器编号------------->(MenuID=68)                           *
									//*     |   +--------------                                              *
#define  MENU_GSMNO				122	//*		|	|GSM NO：------------->(MenuID=69)                       	 *
									//*     |   +--------------                                              *
#define  MENU_TEMPERATURE		123	//*		|	|温度设定：------------->(MenuID=69)                       	 *
									//*     |   +--------------                                              *	
#define  MENU_SYSLANGUAGE		124	//* 	|	Language------------------>(MenuID=54)                           *
																		//* 	---+															 *
#define  MENU_SYSENGLISH		125	//* 			|英文----------------->(MenuID=55)                           *
									//*				|					//* 		+---+														 *
#define  MENU_SYSCHINESE		126	//*   			|中文----------------->(MenuID=56)                           *
									//*				|					//* 		+---+														 *
#define  MENU_SYSRUSSIAN		127	//*   			|俄罗斯文----------------->(MenuID=56)                       *//俄罗斯文;by gzz 20121012														//* 		+---+														 *							
									//*				|					//* 		+---+								
#define  MENU_SYSTURKEY			128	//*   			|土耳其文----------------->(MenuID=56)                           *//土耳其语;by gzz 20121115
									//* 			|					//* 		+---+
									//*     |   +--------------                                              *
#define  MENU_TEMPSET			129	//*		|	|压缩机开关控制：------------->(MenuID=69)                       	 *//添加压缩机控制;by gzz 20121224
									//*     |   ---+                                              *	
#define  MENU_TEMPON			130	//*		|		|使用压缩机开关：------------->(MenuID=69)                       	 *
									//*     |   	+---+                                               *										
#define  MENU_TEMPHOURSTR		131	//*		|		|压缩机开始时：------------->(MenuID=69)                       	 *
									//*     |   	+---+                                               *	
#define  MENU_TEMPMINUTESTR		132	//*		|		|压缩机开始分：------------->(MenuID=69)                       	 *
                                    //*     |   	+---+ 
#define  MENU_TEMPHOUREND		133	//*		|		|压缩机结束时：------------->(MenuID=69)                       	 *
									//* 	|		+---+												*	
#define  MENU_TEMPMINUTEEND		134	//*		|		|压缩机结束分：------------->(MenuID=69)                       	 *
									//*     |   +--------------                                              *	
#define  MENU_LEDSET			135	//*		|	|展示灯开关控制：------------->(MenuID=69)                       	 *//添加展示灯控制;by gzz 20121229
									//*     |   ---+                                              *	
#define  MENU_LEDON			    136	//*		|		|使用展示灯开关：------------->(MenuID=69)                       	 *
									//*     |   	+---+                                               *										
#define  MENU_LEDHOURSTR		137	//*		|		|展示灯开始时：------------->(MenuID=69)                       	 *
									//*     |   	+---+                                               *	
#define  MENU_LEDMINUTESTR		138	//*		|		|展示灯开始分：------------->(MenuID=69)                       	 *
                                    //*     |   	+---+ 
#define  MENU_LEDHOUREND		139	//*		|		|展示灯结束时：------------->(MenuID=69)                       	 *
									//* 	|		+---+												*	
#define  MENU_LEDMINUTEEND		140	//*		|		|展示灯结束分：------------->(MenuID=69)                       	 *
									//*     |   +--------------                                              *										
#define  MENU_SETCHANNEL		141	//*     |货道配置----------------->(MenuID=70)                           *
									//*     +---+                                                            *
#define  MENU_INPUTLAYER		142  //*     |   |请输入编号----------->(MenuID=71)                           *
									//*     |   +--------------                                              *
#define  MENU_LAYERNO			143  //*     |   	|层架编号----------------->(MenuID=72)                   *
									//*     |   +--------------                                              *
#define  MENU_LAYENABLE			144 //*     |   	|是否启用----------------->(MenuID=73)                   *
									//*     |   +--------------                                              *
#define  MENU_MAXGOODS			145  //*     |   	|最大存货量--------------->(MenuID=74)                   *
									//*     |   +--------------                                              *
#define  MENU_CHANNEL1			146  //*     |   	|第一货道----------------->(MenuID=75)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNELNUM1		147  //*     |   	|第一货道存量----------------->(MenuID=75)                   *
									 //*     |   +--------------                                              *									
#define  MENU_CHANNEL2			148  //*     |   	|第二货道----------------->(MenuID=76)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNELNUM2		149  //*     |   	|第二货道存量----------------->(MenuID=75)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNEL3			150  //*     |   	|第三货道----------------->(MenuID=77)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNELNUM3		151  //*     |   	|第三货道存量----------------->(MenuID=75)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNEL4			152  //*     |   	|第四货道----------------->(MenuID=78)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNELNUM4		153  //*     |   	|第四货道存量----------------->(MenuID=75)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNEL5			154  //*     |   	|第五货道----------------->(MenuID=79)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNELNUM5		155  //*     |   	|第五货道存量----------------->(MenuID=75)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNEL6			156  //*     |   	|第六货道----------------->(MenuID=80)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNELNUM6		157  //*     |   	|第六货道存量----------------->(MenuID=75)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNEL7			158  //*     |   	|第七货道----------------->(MenuID=81)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNELNUM7		159  //*     |   	|第七货道存量----------------->(MenuID=75)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNEL8			160  //*     |   	|第八货道----------------->(MenuID=82)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNELNUM8		161  //*     |   	|第八货道存量----------------->(MenuID=75)                   *
									 //*     |   +--------------                                              *

//121105 by cq TenChannels
#define  MENU_CHANNEL9			162  //*     |   	|第九货道----------------->(MenuID=82)                   *
									 //*     |   +-------------- 
#define  MENU_CHANNELNUM9		163  //*     |   	|第九货道存量----------------->(MenuID=75)                   *
									 //*     |   +--------------                                              *
#define  MENU_CHANNEL0			164  //*     |   	|第零货道----------------->(MenuID=82)                   *
									 //*     |   +-------------- 
#define  MENU_CHANNELNUM0		165  //*     |   	|第零货道存量----------------->(MenuID=75)                   *
									 //*     |   +--------------                                              *



#define  MENU_NOTEVALUE1		166	//*     |   			|纸币器通道1：----->(MenuID=23)                  *
									//*     |   +--------------                                              *
#define  MENU_NOTEVALUE2		167 //*     |   			|纸币器通道2：----->(MenuID=24)                  *
									//*     |   +--------------                                              *
#define  MENU_NOTEVALUE3		168	//*     |   			|纸币器通道3：----->(MenuID=23)                  *
									//*     |   +--------------                                              *
#define  MENU_NOTEVALUE4		169	//*     |   			|纸币器通道4：----->(MenuID=24)                  *
									//*     |   +--------------                                              *
#define  MENU_NOTEVALUE5		170	//*     |   			|纸币器通道5：----->(MenuID=23)                  *
									//*     |   +--------------                                              *
#define  MENU_NOTEVALUE6		171	//*     |   			|纸币器通道6：----->(MenuID=24)                  *
									//*     |   +--------------                                              *
#define  MENU_NOTEVALUE7		172	//*     |   			|纸币器通道7：----->(MenuID=23)                  *
									//*     |   +--------------                                              *
#define  MENU_NOTEVALUE8		173	//*     |   			|纸币器通道8：----->(MenuID=24)                  *
									//*     |   +--------------                                              *
#define  MENU_COINVALUE1		174	//*     |   			|硬币器通道1：----->(MenuID=23)                  *
									//*     |   +--------------                                              *
#define  MENU_COINVALUE2		175	//*     |   			|硬币器通道2：----->(MenuID=24)                  *
									//*     |   +--------------                                              *
#define  MENU_COINVALUE3		176	//*     |   			|硬币器通道3：----->(MenuID=23)                  *
									//*     |   +--------------                                              *
#define  MENU_COINVALUE4		177	//*     |   			|硬币器通道4：----->(MenuID=24)                  *
									//*     |   +--------------                                              *
#define  MENU_COINVALUE5		178 //*     |   			|硬币器通道5：----->(MenuID=23)                  *
									//*     |   +--------------                                              *
#define  MENU_COINVALUE6		179	//*     |   			|硬币器通道6：----->(MenuID=24)                  *
									//*     |   +--------------                                              *
#define  MENU_COINVALUE7		180 //*     |   			|硬币器通道7：----->(MenuID=23)                  *
									//*     |   +--------------                                              *
#define  MENU_COINVALUE8		181	//*     |   			|硬币器通道8：----->(MenuID=24)                  *
									//*     |   +--------------                                              *
#define  MENU_CHANGEVALUE1		182	//*     |   |#1找零器面值：--------------->(MenuID=23)                   *
									//*     |   +--------------                                              *
#define  MENU_CHANGEVALUE2		183	//*     |   |#2找零器面值：--------------->(MenuID=24)                   *
									//*     |   +--------------                                              *
#define  MENU_CHANGEVALUE3		184	//*     |   |#3找零器面值：--------------->(MenuID=25)                   *
									//*     |   +--------------                                              *
#define  MENU_DEFAULTSET		185	//* 	|出厂默认设置----------->(MenuID=8)                        *
									//*     |   +--------------                                              *
#define  MENU_CONFIRMSET		186	//*     |   |   |你确定吗？------->(MenuID=53)                           *
									//*     |   |   +--------------                                          *																	
//#define  MENU_SYNGOODSCOL		126	//* 	|全系统同步----------->(MenuID=8)                        *		
//#define  MENU_ADDCOIN   		127	//* 	|补硬币完成----------->(MenuID=8)                        *	
//#define  MENU_ADDBILL	    	128	//* 	|取纸币完成----------->(MenuID=8)                        *	



//************************************************************************************************************************//
#define  MENU_NOTETESTCONFIRM	187	//* 	|纸币器正常吗----------->(MenuID=8)                        *
									//*     |   +--------------                                              *
#define  MENU_COINSTESTCONFIRM	188	//* 	|投币器正常吗----------->(MenuID=8)                        *
									//*     |   +--------------                                              *
#define  MENU_SUBALLCHANNELSTAT 189	//*     |货道状态子菜单--------------->(MenuID=17)                         *
									//*     |   +--------------                                              *
#define  MENU_SUBCHTESTRESULT 	190	//*     |货道测试结果子菜单--------------->(MenuID=17)                         *
									//*     |   +--------------                                              *
#define  MENU_SELECTMEI 		191	//*     |MEI菜单选择项--------------->(MenuID=17)                         *
									//*     |   +--------------                                              *
#define  MENU_SELECTGBA 		192 //*     |MEI菜单选择项--------------->(MenuID=17)                         *
                                    //*     |
#define  MENU_SELECTITL         193 //*     |MEI菜单选择项--------------->(MenuID=17)                         *
                                    //*     |   +--------------                                              *
#define  MENU_SELECTCLOSE 		194	//*     |MEI菜单选择项--------------->(MenuID=17)                         *
									//*     |   +--------------                                              *

#define MENU_PASSWORD_2			195 //120724 by cq RecordPassword /**/
#define MENU_PASSWORD_3			196 
#define MENU_PASSWORD_ADMIN     197


#define  MENU_NULL				225
//用户模式菜单;by gzz 20121130
#define  MENU_VISIBLE_FAUSE		1	
#define  MENU_VISIBLE_TRUE		255

	
char * MenuInit0(void);
char * MenuInitPWIN(void);
char * MenuInitCHIN(void);
char * MenuInitLAYIN(void);
char * MenuInitAddGoods(void);
char * MenuInitCHNO(void);
char * MenuInitCHPR(void);
char * MenuInitCHST(void);
char * MenuInitCHGOODS(void);
char * MenuInitCHSU(void);
char * MenuInitCHMONEY(void);
char * MenuInitCHSUJIDU(void);
char * MenuInitCHMONEYJIDU(void);
char * MenuInitCHDO(void);
char * MenuInitCHLF(void);
char * MenuInitNOST(void);
char * MenuInitCOST(void);
char * MenuInitTOCH(void);
char * MenuInitCOCH1(void);
char * MenuInitHOST1(void);
char * MenuInitHOST2(void);
char * MenuInitHOST3(void);
//121113 by cq Cashless
char * MenuInitHOST2_ReaderCost(void);
char * MenuInitHOST3_ReaderEnable(void);
char * MenuInitIOU(void);

char * MenuInitCGSV(void) ;
char * MenuInitPONO(void) ;
char * MenuInitSACON(void) ;
char * MenuInitPOSEN(void) ;
char * MenuInitPCEN(void) ;
char * MenuInitLCDSEL(void) ;
char * MenuInitKeySel(void)	;	//选货按键;by gzz 20121025
char * MenuInitLogoSel(void);//拼图屏幕样式;by gzz 20121106
char * MenuInitAutoColumn(void);//不用手工补货功能;by gzz 20130122
char * MenuInitTenColumn(void);//是否使用十货道的机器;by gzz 20130226
char * MenuInitHOLDNOTE(void) ;
char * MenuInitHOLDNOTE5(void) ;
char * MenuInitHOLDNOTE10(void) ;
char * MenuInitNBUY(void);
char * MenuInitRCOIN(void);
char * MenuInitOCOIN(void);
char * MenuInitONOTE(void);
char *MenuInitONREADER(void);//读卡器菜单;by gzz 20121120
char *MenuInitOREADERCOST(void);//读卡器菜单;by gzz 20121120
char * MenuInitOCHANGE(void);
char * MenuInitBMIN(void);
char * MenuInitMMAX(void);
char * MenuInitGPRS(void);
char * MenuInitSENO(void);
char * MenuInitGSMNO(void);
char * MenuInitTemerature(void);
char * MenuInitDBMN(void);
char * MenuInitINCOME(void);
char * MenuInitNOIN(void);
char * MenuInitCOIN(void);
char * MenuInitTOTR(void);
char * MenuInitTOCH(void);
char * MenuInitCOVA1(void);
char * MenuInitCOVA2(void);
char * MenuInitCOVA3(void);
//用于季度结账用;by gzz 20121009
char * MenuInitINCOMEJIDU(void);
char * MenuInitNOINJIDU(void);
char * MenuInitCOINJIDU(void);
char * MenuInitTOTRJIDU(void);
char * MenuInitTOCHJIDU(void);
char * MenuInitCOVA1JIDU(void);
char * MenuInitCOVA2JIDU(void);
char * MenuInitCOVA3JIDU(void);

char * MenuInitSUNUM(void);
char * MenuInitDBNUM(void);
char * MenuInitFLASH(void);
char * MenuInitYear(void);
char * MenuInitMonth(void);
char * MenuInitDate(void);
char * MenuInitHour(void);
char * MenuInitMinute(void);
//添加压缩机控制;by gzz 20121224
char * MenuInitCompressOn(void);				  //压缩机时间控制
char * MenuInitCompressHourOn(void); 			  //压缩机打开时
char * MenuInitCompressMinuteOn(void);				  //压缩机打开分
char * MenuInitCompressHourOff(void);				  //压缩机关闭时
char * MenuInitCompressMinuteOff(void);				  //压缩机关闭分
//添加展示灯控制;by gzz 20121229
char * MenuInitLEDOn(void);				  //展示灯时间控制
char * MenuInitLEDHourOn(void);				  //展示灯打开时
char * MenuInitLEDMinuteOn(void);				  //展示灯打开分
char * MenuInitLEDHourOff(void); 			  //展示灯关闭时
char * MenuInitLEDMinuteOff(void);				  //展示灯关闭分
char * MenuInitLAYNO(void);
char * MenuInitLAYENABLE(void);
char * MenuInitMAXGOODS(void);
char * MenuInitCH1(void);
char * MenuInitCH2(void);
char * MenuInitCH3(void);
char * MenuInitCH4(void);
char * MenuInitCH5(void);
char * MenuInitCH6(void);
char * MenuInitCH7(void);
char * MenuInitCH8(void);
//121105 by cq TenChannels
char * MenuInitCH9(void);
char * MenuInitCH0(void);

char * MenuInitCHNum(uint8_t ID);//按货道添存货量;by gzz 20121120
char * MenuInitCH1Num(void);
char * MenuInitCH2Num(void);
char * MenuInitCH3Num(void);
char * MenuInitCH4Num(void);
char * MenuInitCH5Num(void);
char * MenuInitCH6Num(void);
char * MenuInitCH7Num(void);
char * MenuInitCH8Num(void);
//121105 by cq TenChannels
char * MenuInitCH9Num(void);
char * MenuInitCH0Num(void);

		
char *	MenuInitNOCH1(void);
char *	MenuInitNOCH2(void);
char *	MenuInitNOCH3(void);
char *	MenuInitNOCH4(void);
char *	MenuInitNOCH5(void);
char *	MenuInitNOCH6(void);
char *	MenuInitNOCH7(void);
char *	MenuInitNOCH8(void);
char *	MenuInitCOCH1(void);
char *	MenuInitCOCH2(void);
char *	MenuInitCOCH3(void);
char *	MenuInitCOCH4(void);
char *	MenuInitCOCH5(void);
char *	MenuInitCOCH6(void);
char *	MenuInitCOCH7(void);
char *	MenuInitCOCH8(void);
char *  MenuInitCGVA1(void);
char *  MenuInitCGVA2(void);
char *  MenuInitCGVA3(void);	
void 	HoldNoteInit(void);
void KeySystemInit(uint8_t mode);//配置用户菜单可见;by gzz 20121204


uint8_t  MenuFunctionNull(void);
uint8_t  MenuFunctionPW2(void);//120729 by cq RecordPassword /**/
uint8_t  MenuFunctionPW3(void);//用于季度结账用;by gzz 20121009
uint8_t  MenuFunctionPWAdmin(void);//用户模式菜单;by gzz 20121130
uint8_t  MenuFunctionPW(void);
uint8_t  MenuFunctionENG(void);
uint8_t  MenuFunctionCHN(void);
uint8_t  MenuFunctionRussian(void);	//俄罗斯文;by gzz 20121012
uint8_t  MenuFunctionTurkey(void);//土耳其语;by gzz 20121115
uint8_t  MenuFunctionDemo1(void);
uint8_t  MenuFunctionCHIN(void);
uint8_t  MenuFunctionCHLF(void);//
//uint8_t  MenuFunctionDemo3(void);
uint8_t  MenuFunctionCHPR(void);
uint8_t  MenuFunctionCHST(void);
uint8_t  MenuFunctionCHGOODS(void);
uint8_t  MenuFunctionAddGoods(void);
uint8_t  MenuFunctionADDLAYIN(void);	
uint8_t  MenuFunctionSynGoodsCol(void);	
uint8_t  MenuFunctionAddCoinChanges(void);
uint8_t  MenuFunctionAddBillChanges(void);
uint8_t  MenuFunctionDemo6(void);
uint8_t  MenuFunctionDemo7(void);
uint8_t  MenuFunctionDemo8(void);
uint8_t  MenuFunctionNOTETEST(void);
uint8_t  MenuFunctionCOINSTEST(void);
uint8_t  Hopper1Test(void);
uint8_t  Hopper2Test(void);
uint8_t  Hopper3Test(void);
//121113 by cq Cashless
uint8_t  Hopper2Test_ReaderCost(void);
uint8_t  Hopper3Test_ReaderEnable(void);
uint8_t  MenuFunctionKeyTest(void);
uint8_t  MenuFunctionAllChannelStat(void);
uint8_t  MenuFunctionAllCHResult(void);	//所有货道测试结果
uint8_t  MenuFunctionChannelTest(void);
uint8_t  MenuFunctionTESTCHIN(void);
uint8_t  MenuFunctionIRTest(void);
uint8_t  MenuFunctionLCDTest(void);
uint8_t  MenuFunctionDOLCDTest(void);
uint8_t  MenuFunctionTemperatureTest(void);
uint8_t  MenuFunctionDoTempeTest(void);
uint8_t  MenuFunctionXuanhuoTest(void);	//选货按键测试
uint8_t  MenuFunctionDoXuanhuoTest(void);	//真正做选货按键测试
uint8_t  MenuFunctionTempTest(void);	//压缩机测试//添加压缩机控制;by gzz 20121224
uint8_t  MenuFunctionDoTempsTest(void);	//真正做压缩机测试//添加压缩机控制;by gzz 20121224
uint8_t  MenuFunctionLEDTest(void);	//展示灯测试//添加展示灯控制;by gzz 20121229
uint8_t  MenuFunctionDoLEDTest(void);	//真正做展示灯测试
uint8_t  MenuFunctionDemo12(void);
uint8_t  MenuFunctionDemo13(void);
uint8_t  MenuFunctionDemo14(void);
uint8_t  MenuFunctionDemo15(void);
uint8_t  FunctionCGVA(uint8_t id);

uint8_t  MenuFunctionNBUY(void);
uint8_t  MenuFunctionRCOIN(void);
uint8_t  MenuFunctionOCOIN(void);
uint8_t  MenuFunctionONOTE(void);
uint8_t  MenuFunctionONREADER(void);//读卡器菜单;by gzz 20121120
uint8_t  MenuFunctionREADERSetOFF(void);//读卡器菜单;by gzz 20121120
uint8_t  MenuFunctionREADERSetMDB(void);//读卡器菜单;by gzz 20121120
uint8_t  MenuFunctionONREADERCOST(void);//读卡器菜单;by gzz 20121120
uint8_t  MenuFunctionSetMEI(void);
uint8_t  MenuFunctionSetGBA(void);
uint8_t  MenuFunctionSetITL(void);
uint8_t  MenuFunctionSetClose(void);
uint8_t  MenuFunctionOCHANGE(void);
uint8_t  MenuFunctionBMIN(void);
uint8_t  MenuFunctionCGSV(void);
uint8_t  MenuFunctionPONO(void);
uint8_t  MenuFunctionSACON(void);
uint8_t  MenuFunctionPOSEN(void);
uint8_t  MenuFunctionPCEN(void);
uint8_t  MenuFunctionLCDSEL(void);
uint8_t  MenuFunctionKeySel(void);//选货按键;by gzz 20121025
uint8_t  MenuFunctionLogoSel(void);//拼图屏幕样式;by gzz 20121106
uint8_t  MenuFunctionAutoColumn(void);//不用手工补货功能;by gzz 20130122
uint8_t  MenuFunctionTenColumn(void);//是否使用十货道的机器;by gzz 20130226
uint8_t  MenuFunctionHOLDNOTE(void);
uint8_t  MenuFunctionHOLDNOTE5(void);
uint8_t  MenuFunctionHOLDNOTE10(void);

uint8_t  MenuFunctionMMAX(void);
uint8_t  MenuFunctionGPRS(void);
uint8_t  MenuFunctionGSMNO(void);
uint8_t  MenuFunctionTemprature(void);
uint8_t  MenuFunctionSENO(void);
uint8_t  MenuFunctionDBMN(void);
uint8_t  MenuFunctionDemo23(void);
uint8_t  MenuFunctionDemo24(void);
uint8_t  MenuFunctionDemo25(void);
uint8_t  MenuFunctionDemo26(void);
uint8_t  MenuFunctionDemo27(void);
uint8_t  MenuFunctionDemo28(void);
uint8_t  MenuFunctionDemo29(void);
uint8_t  MenuFunctionDemo30(void);
uint8_t  MenuFunctionDemo31(void);
uint8_t  MenuFunctionDemo32(void);
uint8_t  MenuFunctionDemo33(void);
uint8_t  MenuFunctionDemo34(void);
uint8_t  MenuFunctionDemo35(void);
uint8_t  MenuFunctionDemo36(void);
uint8_t  MenuFunctionDemo37(void);
uint8_t  MenuFunctionDemo38(void);
uint8_t  MenuFunctionCONF(void);
uint8_t  MenuFunctionCONFJIDU(void);
uint8_t  MenuFunctionYear(void);
uint8_t  MenuFunctionMonth(void);
uint8_t  MenuFunctionDate(void);
uint8_t  MenuFunctionHour(void);
uint8_t  MenuFunctionMinute(void);
uint8_t  MenuFunctionCompressOn(void);//压缩机时间控制//添加压缩机控制;by gzz 20121224
uint8_t  MenuFunctionCompressHourOn(void);//压缩机打开时//添加压缩机控制;by gzz 20121224
uint8_t  MenuFunctionCompressMinuteOn(void);//压缩机打开分//添加压缩机控制;by gzz 20121224
uint8_t  MenuFunctionCompressHourOff(void);//压缩机关闭时//添加压缩机控制;by gzz 20121224
uint8_t  MenuFunctionCompressMinuteOff(void);//压缩机关闭分//添加压缩机控制;by gzz 20121224
uint8_t  MenuFunctionLEDOn(void);//展示灯时间控制//添加展示灯控制;by gzz 20121229
uint8_t  MenuFunctionLEDHourOn(void);//展示灯打开时//添加展示灯控制;by gzz 20121229
uint8_t  MenuFunctionLEDMinuteOn(void);//展示灯打开分//添加展示灯控制;by gzz 20121229
uint8_t  MenuFunctionLEDHourOff(void);//展示灯关闭时//添加展示灯控制;by gzz 20121229
uint8_t  MenuFunctionLEDMinuteOff(void);//展示灯关闭分//添加展示灯控制;by gzz 20121229
uint8_t  MenuFunctionNEWPW(void);
uint8_t  MenuFunctionNEWPW2(void);
uint8_t  MenuFunctionLAYIN(void);
uint8_t  MenuFunctionLAYENABLE(void);
uint8_t  MenuFunctionMAXGOODS(void);
uint8_t  MenuFunctionCH1(void);
uint8_t  MenuFunctionCH2(void);
uint8_t  MenuFunctionCH3(void);
uint8_t  MenuFunctionCH4(void);
uint8_t  MenuFunctionCH5(void);
uint8_t  MenuFunctionCH6(void);
uint8_t  MenuFunctionCH7(void);
uint8_t  MenuFunctionCH8(void);
//121105 by cq TenChannels
uint8_t  MenuFunctionCH9(void);
uint8_t  MenuFunctionCH0(void);

uint8_t  MenuFunctionCHNum(uint8_t ID);//按货道添存货量;by gzz 20121120
uint8_t  MenuFunctionCH1Num(void);
uint8_t  MenuFunctionCH2Num(void);
uint8_t  MenuFunctionCH3Num(void);
uint8_t  MenuFunctionCH4Num(void);
uint8_t  MenuFunctionCH5Num(void);
uint8_t  MenuFunctionCH6Num(void);
uint8_t  MenuFunctionCH7Num(void);
uint8_t  MenuFunctionCH8Num(void);
//121105 by cq TenChannels
uint8_t  MenuFunctionCH9Num(void);
uint8_t  MenuFunctionCH0Num(void);


uint8_t  MenuFunctionNOCH1(void);
uint8_t  MenuFunctionNOCH2(void);
uint8_t  MenuFunctionNOCH3(void);
uint8_t  MenuFunctionNOCH4(void);
uint8_t  MenuFunctionNOCH5(void);
uint8_t  MenuFunctionNOCH6(void);
uint8_t  MenuFunctionNOCH7(void);
uint8_t  MenuFunctionNOCH8(void);
uint8_t  MenuFunctionCOCH1(void);
uint8_t  MenuFunctionCOCH2(void);
uint8_t  MenuFunctionCOCH3(void);
uint8_t  MenuFunctionCOCH4(void);
uint8_t  MenuFunctionCOCH5(void);
uint8_t  MenuFunctionCOCH6(void);
uint8_t  MenuFunctionCOCH7(void);
uint8_t  MenuFunctionCOCH8(void);
uint8_t  MenuFunctionCGVA1(void);
uint8_t  MenuFunctionCGVA2(void);
uint8_t  MenuFunctionCGVA3(void);
uint8_t  MenuFunctionCONFSET(void);
uint8_t  MenuFunctionADMINSET(void);//用户模式菜单;by gzz 20121130


 //
void lcd_write_d(char c1);
void lcd_write_c(char c1);
void LcdInit(void);
void MyInit(void);
//
void DispOneBoxTextChar(unsigned char Row,unsigned char Col,unsigned char c1,unsigned char DispAttrib);
void DispTwoBoxHexChar(unsigned char Row,unsigned char Col,unsigned char c1,unsigned char DispAttrib);
//void DispGraphChar(unsigned char Row,unsigned char Col,unsigned char xdata *cArray,unsigned char DispAttrib);
//void DispTextLine(unsigned char Row,unsigned char code *MenuText,unsigned char DispAttrib);
void DispTextLine(unsigned char Row, const char  *MenuText,unsigned char DispAttrib)	;
//void ReadyForKey(void);
void DelayMs(unsigned int iMs);


//
//void DispFaceBmp(void);
void DispMenu_Root(uint8_t udkey);
void DispMenu_View(uint8_t udkey);
void DispMenu_Operation(uint8_t udkey);
void DispMenu_Test(uint8_t udkey);
void DispMENU_BUSINESSSet(uint8_t udkey);
void DispMenu_Debug(void);
//
void JudgeHLight(uint8_t udkey);

char  *DecimalPoint( uint32_t money);		  //价格小数点显示
unsigned char ReadTime(void);
void 		ShowDesktopTime(void);
uint8_t  	AllChannelStat(uint8_t	layno);	 //当前层号的所有货道状态
	
extern	uint8_t    	(*KeyFuncPtr)(void);    		//按键功能指针
extern  char *  	(*KeyInitPtr)(void);    		//按键初始化指针
extern  unsigned int  	iMenuID   ;
extern  unsigned int   	iMenuNextID ;
extern  unsigned char 	cMenuTemp1;
extern  uint8_t		CursorLine;	   
extern  uint8_t  	InputSite ;			//数字显示位置

extern 	 uint8_t		BusinessNo;
extern 	 uint8_t		InLayerNO;
extern   uint8_t		InCHNO;
extern   uint8_t		NowChannel;	 	//当前货道行列号
extern   uint8_t		NowLayerNo;	 	//当前层号
extern   uint8_t		NowLayerAll;
extern   uint8_t   		NowCHNO;		//active channel
extern   uint8_t  	InputMode;
extern   uint8_t		PASSWORD;
extern   uint8_t		DOT;		//小数点使能
extern   uint8_t		FDot;		//小数点标志位
extern   uint8_t		FDot1;
extern   uint8_t		FDot2;
extern   uint8_t		FSave;	
extern   uint8_t  	offset;
extern   uint32_t  	InputNum;	
extern   uint8_t		InputCol;
extern   uint8_t  	InputDone;			//已进入输入状态  20110823
extern   char  		StrInit[22]; 



#define		SLNOENOUGH		0	// 		"【      不足      】" 
#define		SLFAULT			1	//		"【      故障      】" 
#define		SLNORMAL		2 	//		"【      正常      】" 
#define		SLTESTING		3 	//		"正在检测，请等待 ..."
#define		SLCHANNELOK		4 	//		"【  货道驱动正常  】" 
#define		SLCHANNELBAD	5	//		"【  货道驱动异常  】" 
#define		SLSAVEOK 		6 	//		"【    保存成功    】" 
#define		SNORMAL			7 	//		"正常" 
#define		SFAULT			8 	//		"异常" 
#define		SSNORMAL		9 	//		"对" 
#define		SSFAULT			10 	//		"缺" 
#define		SSDISABLE		11 	//		"禁" 
#define		CHBAD			12 	// 		"货道:%d故障" ,
#define		CHOK			13 	// 	 	"货道:%d正常"  ,
#define		CHEMPTY			14 	// 		"货道:%d无货"  ,
#define		INSERTCOIN		15 	// 		"请投入硬币"
#define		INSERTBILL		16 	// 		"请投入纸币"
#define		BALANCE			17 	// 		"投币金额："
#define		SSNOGOODS		18 	// 		"无"
#define		SCHKERR		    19 	// 		"出货确认模块故障"
#define		SCHKOK		    20 	// 		"出货确认模块正常"
#define		SCHKOFF		    21 	// 		"出货确认模块关闭"
#define		ADDLAYNO	    22 	// 		"将第n层填满"
#define		ADDFULLLAY	    23 	// 		"将全部层填满"

#ifdef __cplusplus
    }
#endif                                                                  /*  __cplusplus                 */

#endif
