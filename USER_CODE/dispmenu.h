#ifndef __DISPMENU_H 
#define __DISPMENU_H
#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

typedef struct
 {
 	unsigned short int KeyTab_MenuIndex;  		//当前状态索引号
 	unsigned short int KeyTab_MaxItems;  		//本级菜单最大条目数
    unsigned short int KeyTab_PressOk;   	 	//按下"回车"键时转向的状态索引号
    unsigned short int KeyTab_PressEsc;  		//按下"返回"键时转向的状态索引号
    unsigned short int KeyTab_PressDown;  		//按下"向下"键时转向的状态索引号
    unsigned short int KeyTab_PressUp;    		//按下"向上"键时转向的状态索引号
    unsigned short int KeyTab_Visible;    		//当前的状态索引号在用户模式下是否可见
 	uint8_t    	(*CurrentOperate)(void);   		//当前状态应该执行的功能操作
	char * 		(*InitKeyTab)(void);			//当前状态初始化函数
 }KeyTabStruct;									//菜单项结构



#define		ROOTNO			8
#define		CHANNELNO		5
#define		DRIVERNO		18
#define		BUSINESSNO		27
#define  	CHANGEVALUENO	3
#define  	SETNOTE			4
#define  	SETREADER		2//读卡器菜单;by gzz 20121120
#define  	NOTEVALUENO		8
#define  	COINVALUENO		8
#define  	HOLDNOTENO		2
#define  	TRANSACTIONSNO	6
#define		PAYMENTSNO		8
#define		SYSTEMNO		8
#define		LANGUAGENO		4
//121105 by cq TenChannels
//#define		SETCHANNELNO	11
#define		SETCHANNELNO	23

#define 	MAX_KEYTABSTRUCT_NUM      	198

extern  KeyTabStruct  KeyTab[MAX_KEYTABSTRUCT_NUM] ;

#ifdef __cplusplus
    }
#endif                                                                  /*  __cplusplus                 */

#endif
