/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           Dispmenu.c
** Last modified Date:  2012-03-01
** Last Version:         
** Descriptions:        显示维护模块中各菜单页面
**                      
**------------------------------------------------------------------------------------------------------
** Created by:           
** Created date:        2012-03-01
** Version:              
** Descriptions:        The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**
********************************************************************************************************/


//Last Modify Time:03/11/07 01:22
//ReadMe
//屏宽:112
//屏高:64

//#include <intrins.h>
#include <absacc.h>
#include "..\driver\lpc_types.h"
#include "global.h"
#include "pcf8563t.h"
#include "RealTime.h"
#include "function.h"
#include "gbahr1.h"
#include "hopper.h"
#include "lcd.h"
#include "menu.h"
#include "drv.h"
#include "dispmenu.h"
#include "DebugTrace.h"		   




  
const char *     Menu_Password[LANGUAGENO][1]=
{
 	{
 		"请输入密码：" 
	},
	{
 		"Input password："
 	},
	//change by liya 2012-10-16
	{
		"Введите пароль:"
	},
	//土耳其语;by gzz 20121115
	{
		"\xf1\x5eifreyi girin:"
	}
};
const unsigned int  Menu_PasswordID[1]=
{
    MENU_PASSWORD 
};
//俄罗斯文;by gzz 20121012
const char *     Menu_Language[2][LANGUAGENO]=
{
 	{
 		"ENGLISH" ,
		"中文 ",
		"RUSSIAN",
		"Turkish"//土耳其语;by gzz 20121115
	},
	{
 		"ENGLISH" ,
		"中文 ",
		"RUSSIAN",
		"Turkish"//土耳其语;by gzz 20121115
 	}
};

const unsigned int  Menu_LanguageID[LANGUAGENO]=
{
   MENU_ENGLISH ,
   MENU_CHINESE ,
   MENU_RUSSIAN,//俄罗斯文;by gzz 20121012
   MENU_SYSTURKEY	//土耳其语;by gzz 20121115
};
const  unsigned int  Menu_SysLanguageID[LANGUAGENO]=
 {
	MENU_SYSENGLISH ,
	MENU_SYSCHINESE ,
	MENU_SYSRUSSIAN,//俄罗斯文;by gzz 20121012
	MENU_SYSTURKEY	//土耳其语;by gzz 20121115
 };


 //

const char *     Menu_Root[LANGUAGENO][ROOTNO]=
 {
	 {
	 "1 货道参数",
	 "2 加满全部货道",
	 "3 设备管理",
	 "4 业务参数",
	 "5 交易记录",
	 "6 系统参数",
	 "7 货架配置",
	 "8 出厂默认设置"	 
	 //"9.全系统同步",
	 //"10. 补硬币完成",
	 //"11. 取纸币完成"
	 },
	 {
	 "1.Column",
	 "2.Add full goods",
	 "3.Device",
	 "4.Parameter",
	 "5.Record inquiry",
	 "6.System",
	 "7.Tray",
	 "8.Default"	 
	 //"9.Goods-Column",
	 //"10.Add coin",
	 //"11.Add Cash"
	 },
	 //change by liya 20121013
	 {
	 "1.Колонка",
	 "2.Добавить полную еду",
	 "3.Устройство",
	 "4.Параметер",
	 "5.Запись запроса",
	 "6.Система",
	 "7.Лоток",
	 "8.Умолчания"
	 //"9.Колонка еды",
	 //"10.Загрузка монет",
	 //"11.Загрузка наличных",
	 //"12.Это не доступен",
	 //"13.Это не доступен"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.Sutun parametre",
	 "2.\xf0\xdcr\xf0\xfcn girin",	
	 "3.Cihazlar",
	 "4.Cihaz parametreleri",
	 "5.Kay\xf1\x31t sorgulama",
	 "6.Sistem parametreleri",
	 "7.Tepsi",
	 "8.Varsay\xf1\x31lan ayar"
	 }
 };
const unsigned int  Menu_RootID[ROOTNO]=
{
	 MENU_CHANNEL,
	 MENU_ADDGOODS,
	 MENU_DRIVER,
	 MENU_BUSINESS,
	 MENU_TRANSACTIONS,
	 MENU_SYSTEM,
	 MENU_SETCHANNEL,
	 MENU_DEFAULTSET
	 //MENU_SYNGOODSCOL,
	 //MENU_ADDCOIN,
	 //MENU_ADDBILL
};

const char *     Menu_InputChannelNo[LANGUAGENO][1]=
{
	 {
	  "请输入编号: "
	 },
	 {
	  "Input column code:"
	 },
	 //change by liya 2012-10-16
	 {
	 	"Ввод кода столбца: "
	 },
	 //土耳其语;by gzz 20121115
	 {
	 	"S\xf0\xfctun kodunu girin:"
	 }
};

const unsigned int  Menu_InputChannelNoID[1]=
{
  	MENU_INPUTNUM
};
//

const char *     Menu_Channel[LANGUAGENO][CHANNELNO]=
{
	 {
	 "1.货道编号: ",
	 "2.单价:     ",
	 "3.商品余量: ",
	 "4.货道状态: ",
	 "5.商品编号: "	 
	 },
	 {
	 "1.Column code:",
	 "2.Price:",
	 "3.Goods remain:",
	 "4.Column status:",
	 "5.Goods NO:"
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Код колонки: ",
	 "2.Цена: ",
	 "3.Остатки еды: ",
	 "4.Статус колонки: ",
	 "5. Номер еды: "
	 },
	 //土耳其语;by gzz 20121115
	 {
	 	"1 S\xf0\xfctun kodu:",
		"2.Fiyat:",
		"3.Kalan \xf0\xfcr\xf0\xfcn:",
		"4 S\xf0\xfctun  durumu:",
		"5.\xf0\xdcr\xf0\xfcn numaras\xf1\x31:"
	 }
};
const unsigned int  Menu_ChannelID[CHANNELNO]=
{
	 MENU_CHANNELNO,
	 MENU_PRICE,
	 MENU_CHANNELLEFT,
	 MENU_CHANNELSTAT,
	 MENU_CHANNELGOODS
};
//

const char *     Menu_AddInputLayerNum[LANGUAGENO][1]=
{
	 {
	 "请输入层号: "
	 },
	 {
	  "Input tray code:"
	 },
	 //change by liya 2012-10-16
	 {
	  "Введите код лотка:"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "Katman numaras\xf1\x31n\xf1\x31 girin"
	 }
};

const unsigned int  Menu_AddInputLayerNumID[1]=
{
	  MENU_ADDINPUTLAY
};


const  char *     Menu_InputAddGoods[LANGUAGENO][2]=
{
	 {
	 "" ,
	 "确认ENTER 返回CANCEL"
	 },
	 {
	 "",
	 "ENTER or CANCEL"
	 },
	 //change by liya 2012-10-16
	 {
	 "",
	 "Ввод   или   Выход"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "",
	 "Giri\xf1\x5f yada iptal"
	 }
};
const unsigned int  Menu_InputAddGoodsID[2]=
{
	  MENU_INPUTADDGOODS,
	  MENU_NULL
};


const char *    Menu_Driver[LANGUAGENO][DRIVERNO]=
{
	 {
	 "1.纸币器状态: ",
	 "2.硬币器状态: ",
	 "3.#1找零器: ",
	 "4.#2找零器: ",
	 "5.#3找零器: ",
	 "6.手机模块状态测试",
	 "7.短消息检测  ",
	 "8.按钮检测  ",
	 "9.货道状态  ",
	 "10. 货道检测  ",
	 "11. 货道检测结果",
	 "12. 出货确认检测",
	 "13. 显示屏检测",
	 "14. 温控器检测",
	 "15. 选货按键检测",
	 "16. 压缩机检测",//添加压缩机控制;by gzz 20121224
	 "17. 展示灯检测",//添加展示灯控制;by gzz 20121229
	 "18. 存储剩余空间"
	 },
	 {
	 "1.BillAcceptor:",
	 "2.CoinAcceptor:",
	 "3.ChangeUnit#1:",
	 "4.ChangeUnit#2:",
	 "5.ChangeUnit#3:",
	 "6.GSM status test",
	 "7.SMS test  ",
	 "8.Key test  ",
	 "9.Column status",
	 "10. Column test",
	 "11. ColTest result",
	 "12. GOC test  ",
	 "13. LCD test  ",
	 "14. Temperature test",
	 "15. Choose test",
	 "16. Compressor test",
	 "17. LED test",
	 "18. Flash left"
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Приемнк купюр: ",
	 "2.Приемник монет: ",
	 "3.Изменение 1UINT: ",
	 "4.Изменение 2UINT: ",
	 "5.Изменение 3UINT: ",
	 "6.Тест статуса GSM",
	 "7.Тест SMS",
	 "8.Тест кнопок",
	 "9.Статус колонки",
	 "10.Тест колонки",
	 "11.Результат теста колонки",
	 "12.Тест SMS",
	 "13.Тест LCD",
	 "14.Тест температуры",
	 "15.Тест Взяв ключи",
	 "16.Компрессор тест",
	 "17.LED тест",
	 "18.Остаток Flash памяти"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.Ka\xf1\x1f\xf1\x31t para al\xf1\x31\c\xf1\x31: ",
	 "2.Bozuk para al\xf1\x31\c\xf1\x31: ",
	 "3.De\xf1\x1fi\xf1\x5fim #1: ",
	 "4.De\xf1\x1fi\xf1\x5fim #2: ",
	 "5.De\xf1\x1fi\xf1\x5fim #3: ",
	 "6.Tele test durumu",
	 "7.SMS testi  ",
	 "8.Klavye testi",
	 "9.S\xf0\xfctun Durumu  ",
	 "10.S\xf0\xfctun testi",
	 "11.S\xf0\xfctun test sonuc",
	 "12.Ba\xf1\x5f\ar\xf1\x31l\xf1\x31 i\xf1\x5flem adet testi",
	 "13.LCD testi",
	 "14.Termostat testi",
	 "15.D\xf0\xfc\xf1\x1fmesi Toplama testi",
	 "16.Kompres\xf0\xF6r testi",
	 "17.LED testi",
	 "18.Depolama alan\xf1\x31"
	 }
};
const unsigned int  Menu_DriverID[DRIVERNO]=
{
	 MENU_NOTESTAT,
	 MENU_COINSSTAT,
	 MENU_HOPPERSTAT1,
	 MENU_HOPPERSTAT2,
	 MENU_HOPPERSTAT3,
	 MENU_GPRSTEST,
	 MENU_SMSTEST,
	 MENU_KEYTEST,
	 MENU_ALLCHANNELSTAT,
	 MENU_CHANNELTEST,
	 MENU_CHTESTRESULT,
	 MENU_IRTEST,
	 MENU_LCDTEST,
	 MENU_TEMPRATURETEST,
	 MENU_SELECTKEYTEST,
	 MENU_TEMPSTEST,//添加压缩机控制;by gzz 20121224
	 MENU_LEDTEST,//添加展示灯控制;by gzz 20121229
	 MENU_FLASHLEFT
};
const char *     Menu_KeyTest[LANGUAGENO][1]=
{
	 {
	 "请按输入键"
	 },
	 {
	 "Pls press any key"
	 },
	 //change by liya 2012-10-16
	 {
	 "Введите код столбца:"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "D\xf0\xfc\xf1\x1fme giriniz"
	 }
};
const  char *     Menu_NoteTestConfirm[LANGUAGENO][2]=
{
	 {
	 "纸币器正常吗？" ,
	 "正确ENTER 错误CANCEL"
	 },
	 {
	 "Bill acceptor OK?",
	 "ENTER or CANCEL"
	 },
	 //change by liya 2012-10-16
	 {
	 "Приемник купюр впорядке?",
	 "Ввод   или   Выход"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "Ka\xf1\x1f\xf1\x31t para ok?" ,
	 "Giri\xf1\x5f yada iptal"
	 }
};
const unsigned int  Menu_NoteTestConfirmID[2]=
{
	  MENU_NOTETESTCONFIRM,
	  MENU_NULL
};
const char *     Menu_CoinsTestConfirm[LANGUAGENO ][2]=
{
	 {
	 "硬币器正常吗？" ,
	 "正确ENTER 错误CANCEL"
	 },
	 {
	 "Coin acceptor OK?",
	 "ENTER or CANCEL"
	 },
	 //change by liya 2012-10-16
	 {
	 "Приемник монет впорядке?",
	 "Ввод или Выход"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "Bozuk para ok?" ,
	 "Giri\xf1\x5f yada iptal"
	 }
};
const unsigned int  Menu_CoinsTestConfirmID[2]=
{
	  MENU_COINSTESTCONFIRM,
	  MENU_NULL
};
const char *     Menu_DriverInputNum[LANGUAGENO][1]=
{
	 {
	 "请输入编号: "
	 },
	 {
	  "Input column code:"
	 },
	 //change by liya 2012-10-16
	 {
	 "Введите код колонки: "
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "S\xf0\xfctun xx Hata num.:"
	 }
};

const unsigned int  Menu_DriverInputNumID[1]=
{
	  MENU_DRINPUTNUM
};
//
const unsigned int  Menu_KeyTestID[1]=
{
   	MENU_DOKEYTEST
};
 //

 const char * 	Menu_Business[LANGUAGENO][BUSINESSNO]=
 {
	  {
	  "1.硬币器面值  ",
	  "2.纸币器面值  ",
	  "3.找零器面值  ",
	  "4.开启硬币器: ",
	  "5.纸币器类型: ",
	  "6.开启找零器: ",
	  "7.读卡器类型:",
	  "8.读卡器扣钱:",
	  "9.无零币是否服务: ",
	  "10. 允许多次购买: ",
	  "11. 不买货退币: ",
	  "12. 余额下限: ",
	  "13. 收币上限: ",
	  "14. 欠条：  ",
	  "15. 疑问金额: ",
	  "16. 小数点位数: ",
	  "17. 开启出货确认: ", 
	  "18. 开启POS 机: ",
	  "19. 开启PC通讯:",
	  "20. 屏幕类型:",//拼图屏幕样式;by gzz 20121106
	  "21. 使用选货按键板:",
	  "22. 使用logo:",
	  "23. 自动添货:",
	  "24. 十货道:",
	  "25. 是否开启暂存：",
	  "26. 纸币 5元暂存: ",
	  "27. 纸币10元暂存: "//,
	  //"20. 纸币20元暂存: "
	  },
	  {
	  "1.Coins type ", 
	  "2.Note type ", 
	  "3.Change type ", 
	  "4.Coin on: ", 
	  "5.Bill on: ", 
	  "6.Change on: ",
	  "7.Reader Type:",
	  "8.Reader Cost:",
	  "9.Nochange SVR:",
	  "10.Multi-verd:",
	  "11.Refund:",
	  "12.ChangeValue:",
	  "13.BanknoteMax:",
	  "14. IOU:",
	  "15.Doubtful Amt:",
	  "16.Decimal num:",
	  "17.GOC_dev on:", 
	  "18. POS on:",
	  "19. PC on:",
	  "20. LCDType:",
	  "21. SelectKey:",
	  "22. Logo:",
	  "23. AutoAdd:",
	  "24. TenColumn:",
	  "25. Hold note:",
	  "26.HoldNote5$: ",
	  "27.HoldNote10$:"//,
	  //"20.HoldNote10$:" 
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Тип ионет",
	 "2.Тип купюр",
	 "3.Тип разменника",
	 "4.Монета вкл.:",
	 "5.Купюра вкл.:",
	 "6.Разменник вкл.:",
	 "7.Читатель типа:",
	 "8.Читатель, за вычетом денег:",
	 "9.Нет размена обслуживание:",
	 "10.Мулти решение:",
	 "11.переопределение:",
	 "12.Значение размена:",
	 "13.Макс. Загрузка:",
	 "14.IOU:",
	 "15.Сомнительные Amt:",
	 "16.Десятичный момер:",
	 "17.GOC-устройство вкл.:",
	 "18.POS вкл.:",
	 "19.PC вкл.:",
	 "20.Крупный экран:",
	 "21.Взяв ключи:",
	 "22. Logo:",
	 "23.автоматически добавляться:",
	 "24.десять",
	 "25.Отключение купюр:",
	 "26.Отключить купюру 5$:",
	 "27.Отключить купюру 10$:"
	 },
	 //土耳其语;by gzz 20121115
	 {
	  "1.Bozuk paran\xf1\x31n de\xf1\x1f\eri",
	  "2.Ka\xf1\x1f\xf1\x31t paran\xf1\x31n de\xf1\x1f\eri",
	  "3.Homing cihaz\xf1\x31n nominal de\xf1\x1feri",
	  "4.P.alici acik:",
	  "5.Ka\xf1\x1f\xf1\x31t para:",
	  "6.De\xf1\x1fi\xf1\x5fiklik t\xf0\xfcr\xf0\xfcne tutun: ",
	  "7.Okuyucu tipi:",
	  "8.Para d\xf0\xfc\xf1\x5f\xf0\xfclmesi Okuyucu,:",
	  "9.No degisim Modu:",
	  "10.\xf0\xc7oklu al\xf1\x31m:",
	  "11.Geri \xf0\xF6deme:",
	  "12.Bozuk p:",
	  "13.Banknot max.de\xf1\x1f\er:",
	  "14.Kalan bakiye:",
	  "15.\xf1\x5e\xf0\xfcpheli bakiye:",
	  "16.Ondal\xf1\x31k say\xf1\x31:",
	  "17.GOC acik:", 
	  "18.POS acik:",
	  "19.PC acik:",
	  "20.Ekran T\xf0\xfcr\xf0\xfc:",//拼图屏幕样式;by gzz 20121106
	  "21.Anahtar kurulu Toplama:",
	  "22.kullanmak logo:",
	  "23.otomatik eklendi:",
	  "24.on:",
	  "25.Ge\xf0\xe7ici a\xf0\xe7mak i\xf0\xe7in olsun:",
	  "26.5Ge\xf0\xe7ici: ",
	  "27.10Ge\xf0\xe7ici:"//,
	  //"20. 纸币20元暂存: "
	  }
 };

const unsigned int  Menu_BusinessID[BUSINESSNO]=
{
	 MENU_COINVALUE,
	 MENU_NOTEVALUE,
	 MENU_CHANGEVALUE,
	 MENU_OPENCOIN, 
	 MENU_OPENNOTE,
	 MENU_OPENCHANGE,
	 MENU_OPENREADER,
	 MENU_READERCOST,
	 MENU_CHANGESERVER,
	 MENU_NOLIMITEDBUY,
	 MENU_RETURNCOINS,
	 MENU_BALANCEMIN,
	 MENU_MONEYMAXIN,
	 MENU_IOU	,
	 MENU_DOUBTMONEY,
	 MENU_DECPOINTNUM,
	 MENU_SALECONFIRM,
	 MENU_POSENABLE,
	 MENU_VMCPC,
	 MENU_LARGELCD,
	 MENU_SELECTKEY,
	 MENU_LOGOSEL,
	 MENU_ADDCOLUMN,
	 MENU_TENCOLUMN,
	 MENU_HOLDNOTE,
	 MENU_HOLDNOTE5,
	 MENU_HOLDNOTE10//,
	 //MENU_HOLDNOTE20
};
//

const char *     Menu_ChangeValue[LANGUAGENO][CHANGEVALUENO]=
{
	 {
	 "1.#1找零器: ",
	 "2.#2找零器: ",
	 "3.#3找零器: "
	 },
	 {
	 "1.#1Change: ",
	 "2.#2Change: ",
	 "3.#3Change: "
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Размен 1: ",
	 "2.Размен 2: ",
	 "3.Размен 3: "
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "De\xf1\x1fi\xf1\x5fim #1:",
	 "De\xf1\x1fi\xf1\x5fim #2:",
	 "De\xf1\x1fi\xf1\x5fim #3:"
	 }
};
const unsigned int  Menu_ChangeValueID[CHANGEVALUENO]=
{
	 MENU_CHANGEVALUE1,
	 MENU_CHANGEVALUE2,
	 MENU_CHANGEVALUE3
};


const char *     Menu_SetNote[LANGUAGENO][SETNOTE]=
{
	 {
	 "1. MDB",
	 "2. GBA ",
	 "3. SSP ",
	 "4. OFF "
	 },
	 {
	 "1. MDB",
	 "2. GBA ",
	 "3. SSP ",
	 "4. OFF "
	 },
	 //change by liya 2012-10-16
	 {
	 "1. MDB",
	 "2. GBA ",
	 "3. SSP ",
	 "4. OFF "
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1. MDB",
	 "2. GBA ",
	 "3. SSP ",
	 "4. OFF "
	 }
};
const unsigned int  Menu_SetNoteID[SETNOTE]=
{
	 MENU_SELECTMEI,
	 MENU_SELECTGBA,
	 MENU_SELECTITL,
	 MENU_SELECTCLOSE
};

//读卡器菜单;by gzz 20121120
const char * 	Menu_SetReader[LANGUAGENO][SETREADER]=
{
	  {
	  "1. OFF",
	  "2. MDB "
	  },
	  {
	  "1. OFF",
	  "2. MDB "
	  },
	  //change by liya 2012-10-16
	  {
	  "1. OFF",
	  "2. MDB "
	  },
	  //土耳其语;by gzz 20121115
	  {
	  "1. OFF",
	  "2. MDB "
	  }
};
//读卡器菜单;by gzz 20121120
const unsigned int  Menu_SetReaderID[SETREADER]=
{
	  MENU_READEROFF,
	  MENU_READERMDB
};

  
 //

const char *     Menu_NoteValue[LANGUAGENO][NOTEVALUENO]=
{
	 {
	 "1.#1纸币: ",
	 "2.#2纸币: ",
	 "3.#3纸币: ",
	 "4.#4纸币: ",
	 "5.#5纸币: ",
	 "6.#6纸币: ",
	 "7.#7纸币: ",
	 "8.#8纸币: "
	 },
	 {
	 "1.#1 Note: ",
	 "2.#2 Note: ",
	 "3.#3 Note: ",
	 "4.#4 Note: ",
	 "5.#5 Note: ",
	 "6.#6 Note: ",
	 "7.#7 Note: ",
	 "8.#8 Note: "
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Купюра №1:",
	 "2.Купюра №2:",
	 "3.Купюра №3:",
	 "4.Купюра №4:",
	 "5.Купюра №5:",
	 "6.Купюра №6:",
	 "7.Купюра №7:",
	 "8.Купюра №8:"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.#1 dikkat: ",
	 "2.#2 dikkat: ",
	 "3.#3 dikkat: ",
	 "4.#4 dikkat: ",
	 "5.#5 dikkat: ",
	 "6.#6 dikkat: ",
	 "7.#7 dikkat: ",
	 "8.#8 dikkat: "
	 }
};
const unsigned int  Menu_NoteValueID[NOTEVALUENO]=
{
	 MENU_NOTEVALUE1,
	 MENU_NOTEVALUE2,
	 MENU_NOTEVALUE3,
	 MENU_NOTEVALUE4,
	 MENU_NOTEVALUE5,
	 MENU_NOTEVALUE6,
	 MENU_NOTEVALUE7,
	 MENU_NOTEVALUE8
};
 
 
 //

const char *     Menu_CoinValue[LANGUAGENO][COINVALUENO]=
{
	 {
	 "1.#1硬币: ",
	 "2.#2硬币: ",
	 "3.#3硬币: ",
	 "4.#4硬币: ",
	 "5.#5硬币: ",
	 "6.#6硬币: ",
	 "7.#7硬币: ",
	 "8.#8硬币: "
	 },
	 {
	 "1.#1 Coin: ",
	 "2.#2 Coin: ",
	 "3.#3 Coin: ",
	 "4.#4 Coin: ",
	 "5.#5 Coin: ",
	 "6.#6 Coin: ",
	 "7.#7 Coin: ",
	 "8.#8 Coin: "
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Монета №1:",
	 "2.Монета №2:",
	 "3.Монета №3:",
	 "4.Монета №4:",
	 "5.Монета №5:",
	 "6.Монета №6:",
	 "7.Монета №7:",
	 "8.Монета №8:"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.#1 sikke: ",
	 "2.#2 sikke: ",
	 "3.#3 sikke: ",
	 "4.#4 sikke: ",
	 "5.#5 sikke: ",
	 "6.#6 sikke: ",
	 "7.#7 sikke: ",
	 "8.#8 sikke: "
	 }
}; 
 
const unsigned int  Menu_CoinValueID[COINVALUENO]=
{
	 MENU_COINVALUE1,
	 MENU_COINVALUE2,
	 MENU_COINVALUE3,
	 MENU_COINVALUE4,
	 MENU_COINVALUE5,
	 MENU_COINVALUE6,
	 MENU_COINVALUE7,
	 MENU_COINVALUE8
};
   
 //

const char *     Menu_HoldNote[LANGUAGENO][HOLDNOTENO]=
{
	 {
	 "1.纸币 5元暂存: ",
	 "2.纸币10元暂存: "
	 },
	 {
	 "1.HoldNote5$: ",
	 "2.HoldNote10$: "
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Отключить купюру 5$: ",
	 "2.Отключить купюру 10$: "
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.5$ Ge\xf0\xe7ici: ",
	 "2.10$ Ge\xf0\xe7ici: "
	 }
};
const unsigned int  Menu_HoldNoteID[COINVALUENO]=
{
	 MENU_HOLDNOTE5,
	 MENU_HOLDNOTE10
};
  
 
 

const  char *     Menu_Transactions[LANGUAGENO][TRANSACTIONSNO]=
{
	 {
	 "1 日常货币收支    ",
	 "2 日常商品记录        ",
	 "3 清除日常记录    ",
	 "4 季度货币收支    ",
	 "5 季度商品记录        ",
	 "6 清除季度记录    "
	 },
	 {
	 "1.Payments    ",
	 "2.Goods       ",
	 "3.Clear record",
	 "4.quarter Payments    ",
	 "5.quarter Goods       ",
	 "6.Clear quarter record"
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Платежи",
	 "2.Еда",
	 "3.Очистить запись",
	 "4.квартал Платежи",
	 "5.квартал Еда",
	 "6.квартал Очиститьзапись"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.\xf0\xD6\demeler g\xf0\xfcng\xf0\xfcn para bakiyesi",
	 "2.G\xf0\xfcnl\xf0\xfck mallar\xf1\x31n kay\xf1\x31tlar\xf1\x31",
	 "3.G\xf0\xfcnl\xf0\xfck kay\xf1\x31t Temizle",
	 "4.\xf0\xc7eyrek parasal \xf0\xF6demeler",
	 "5.Ayl\xf1\x31k emtia kay\xf1\x31tlar\xf1\x31",
	 "6.Temizle ayl\xf1\x31k rekor"
	 }
};
const unsigned int  Menu_TransactionsID[TRANSACTIONSNO]=
{
	 MENU_PAYMENTS,
	 MENU_GOODS,
	 MENU_CLEARTRANS,
	 MENU_PAYMENTSJIDU,
	 MENU_GOODSJIDU,
	 MENU_CLEARTRANSJIDU
};
//



const  char *     Menu_Payments[LANGUAGENO][PAYMENTSNO]=
{
	 {
	 "1.货币总收入:",
	 "2.纸币收入  :",
	 "3.硬币收入  :",
	 "4.交易总金额:",
	 "5.找零总金额:",
	 "6.#1出币枚数:",
	 "7.#2出币枚数:",
	 "8.#3出币枚数:"
	 },
	 {
	 "1.TotalIncome:",
	 "2.Note income:",
	 "3.CoinsIncome:",
	 "4.Total trans:",
	 "5.TotalChange:",
	 "6.#1Coins num:",
	 "7.#2Coins num:",
	 "8.#3Coins num:"
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Суммарная выручка:",
	 "2.Принято купюр:",
	 "3.Принято монет:",
	 "4.Всего транзакций:",
	 "5.Всего разменов:",
	 "6.Число монет №1:",
	 "7.Число монет №2:",
	 "8.Число монет №3:"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.Toplam:",
	 "2.Ka\xf1\x1f\xf1\x31t para:",
	 "3.Madeni para:",
	 "4.Sat\xf1\x31ld\xf1\x31:",
	 "5.Toplam tutar\xf1\x31 i\xf0\xe7in de\xf1\x1fi\xf1\x5ftirme:",
	 "6.#1Sikke alt\xf1\x31n:",
	 "7.#2Sikke alt\xf1\x31n:",
	 "8.#3Sikke alt\xf1\x31n:"
	 }
};
const unsigned int  Menu_PaymentsID[PAYMENTSNO]=
{
	 MENU_INCOME,
	 MENU_NOTEINCOME,
	 MENU_COINSINCOME,
	 MENU_TOTALTRANS,
	 MENU_TOTALCHANGE,
	 MENU_COINSCHANGE1,
	 MENU_COINSCHANGE2,
	 MENU_COINSCHANGE3
};

//用于季度结账用;by gzz 20121009
const unsigned int  Menu_PaymentsIDJidu[PAYMENTSNO]=
{
	  MENU_INCOMEJIDU,
	  MENU_NOTEINCOMEJIDU,
	  MENU_COINSINCOMEJIDU,
	  MENU_TOTALTRANSJIDU,
	  MENU_TOTALCHANGEJIDU,
	  MENU_COINSCHANGE1JIDU,
	  MENU_COINSCHANGE2JIDU,
	  MENU_COINSCHANGE3JIDU
};

const  char *     Menu_Goods[LANGUAGENO][3]=
{
	 {
	 "1.成功交易次数:",
	 "2.疑问交易次数:",
	 "3.单货道交易统计"
	 },
	 {
	 "1.Success num:",
	 "2.Doubt num  :",
	 "3.Column tran "
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Кол-во удач:",
	 "2.Кол-во сомнений:",
	 "3.Столбец транзакций:"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.I\xf1\x5flem say\xf1\x31s\xf1\x31:",
	 "2.\xf1\x5e\xf0\xfcphe bu i\xf1\x5flem say\xf1\x31s\xf1\x31:",
	 "3.Tek koridorlu i\xf1\x5flemleri istatistikleri"
	 }
};

//用于季度结账用;by gzz 20121009
const char *     Menu_GoodsJidu[LANGUAGENO][1]=
{
	 {	
	 "1.单货道交易统计"
	 },
	 {	
	 "1.Column tran "
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Столбец транзакций:"
	 },
	 //土耳其语;by gzz 20121115
	 {	
	 "1.Tek koridorlu i\xf1\x5flemleri istatistikleri"
	 }
}; 
const unsigned int  Menu_GoodsID[3]=
{
	 MENU_SUCCESSNUM,
	 MENU_DOUBTNUM,
	 MENU_CHANNELTRANS
};

//用于季度结账用;by gzz 20121009
const unsigned int  Menu_GoodsIDJidu[1]=
{	
	 MENU_CHANNELTRANSJIDU
};


const char *     Menu_TransInputChannelNo[LANGUAGENO][1]=
{
	 {
	 "请输入编号: "
	 },
	 {
	 "Input column code:"
	 },
	 //change by liya 2012-10-16
	 {
	 "Ввод код колонки:"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 	"S\xf0\xfctun kodunu girin:"
	 }
};
const unsigned int  Menu_TransInputChannelNoID[1]=
{
	MENU_TRANSINPUTNUM
};

 //用于季度结账用;by gzz 20121009
const  unsigned int  Menu_TransInputChannelNoIDJIDU[1]=
 {
	 MENU_TRANSINPUTNUMJIDU
 };


const  char *     Menu_TransChannel[LANGUAGENO][4]=
{
	 {
	 "1.货道编号:     ",
	 "2.成功交易统计: ",
	 "3.商品金额统计: ",
	 "4.疑问交易统计: "
	 },
	 {
	 "1.Column No.:",
	 "2.Success num:",
	 "3.Success money:",
	 "4.Doubt num:"
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Колонка номер:",
	 "2.кол-во удач:",
	 "3.Деньги статистика:",
	 "4.кол-во сомнений:"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.Kargo Road No:     ",
	 "2.Ba\xf1\x5far\xf1\x31l\xf1\x31 bir i\xf1\x5flem say\xf1\x31s\xf1\x31: ",
	 "3.Mal istatistik miktar\xf1\x31: ",
	 "4.\xf1\x5e\xf0\xfcphe bu i\xf1\x5flem say\xf1\x31s\xf1\x31: "
	 }
};

 //用于季度结账用;by gzz 20121009
const   char *     Menu_TransChannelJidu[LANGUAGENO][3]=
{
	 {
	 "1.货道编号:     ",
	 "2.成功交易统计: ",
	 "3.商品金额统计: "
	 },
	 {
	 "1.Column NO:",
	 "2.Success NUM:",
	 "3.Success Money:"	 
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Колонка номер:",
	 "2.кол-во удач:",
	 "3.кол-во сомнений:"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.Kargo Road No:     ",
	 "2.Ba\xf1\x5far\xf1\x31l\xf1\x31 bir i\xf1\x5flem say\xf1\x31s\xf1\x31: ",
	 "3.Mal istatistik miktar\xf1\x31: "
	 }
};
 
const  unsigned int  Menu_TransChannelID[4]=
{
	 MENU_TRANSCHANNELNO,
	 MENU_TRANSSUCCESS,
	 MENU_TRANMONEY,
	 MENU_TRANSDOUBT
};

//用于季度结账用;by gzz 20121009
const unsigned int  Menu_TransChannelIDJIDU[3]=
{
	 MENU_TRANSCHANNELNOJIDU,
	 MENU_TRANSSUCCESSJIDU,
	 MENU_TRANMONEYJIDU
}; 
 

const char *     Menu_System[LANGUAGENO][SYSTEMNO]=
{
	 {
	 "1 时间设置    ",
	// "更改密码    ",
	 "2 开启手机模块: ",
	 "3 机器编号: ",
	 "4 GSM No",
	 "5 机内温度设定:",
	 "6 语言选择",
	 "7 温控器控制",//添加压缩机控制;by gzz 20121224
	 "8 展示灯控制"//添加展示灯控制;by gzz 20121229
	 },
	 {
	 "1.Set time",
	 //"CHANGEPASSWORD",
	 "2.GSM on:",
	 "3.VM ID:",
	 "4.GSM No",
	 "5.Temperature:",
	 "6.language",
	 "7.Temperature control",
	 "8.LED control"
	 },
	 //change by liya 2012-10-16
	 {
	 "1.установка времени",
	 "2.Включить GSM:",
	 "3.VM ID:",
	 "4.Номер GSM:",
	 "5.Температура:",
	 "6.язык",
	 "7.регулирование температуры",
	 "8.LED температуры"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.Zaman ayar\xf1\x31",
	 "2.Mobil:",
	 "3.VM Ki:",
	 "4.GSM mum",
	 "5.S\xf1\x31.cakl\xf1\x31k ayar\xf1\x31:",
	 "6.Dil",
	 "7.S\xf1\x31\cakl\xf1\x31k kontrol\xf0\xfc",
	 "8.LED kontrol\xf0\xfc"
	 }
};
const  unsigned int  Menu_SystemID[SYSTEMNO]=
{
	 MENU_TIMESET,
	// MENU_CGPASSWORD,
	 MENU_GPRSONOFF,
	 MENU_SERIALNO,
	 MENU_GSMNO,
	 MENU_TEMPERATURE,
	 MENU_SYSLANGUAGE,
	 MENU_TEMPSET,//添加压缩机控制;by gzz 20121224
	 MENU_LEDSET//添加展示灯控制;by gzz 20121229
};
 
const char *    Menu_TimeSet[LANGUAGENO][5]=
{
	 {
	 "1.年:         ",
	 "2.月:         ",
	 "3.日:         ",
	 "4.时:         ",
	 "5.分:         "
	 },
	 {
	 "1.YEAR:       ",
	 "2.MONTH:      ",
	 "3.DATE:       ",
	 "4.HOUR:       ",
	 "5.MINUTE:     "
	 },
	 //change by liya
	 {
	 "1.Год:        ",
	 "2.Месяц:      ",
	 "3.День:       ",
	 "4.Часы:       ",
	 "5.Минуты:     "
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.Y\xf1\x31l:         ",
	 "2.ay:         ",
	 "3.g\xf0\xfcn:         ",
	 "4.zaman:         ",
	 "5.dakika:         "
	 }
};
const  unsigned int  Menu_TimeSetID[5]=
{
	 MENU_SETYEAR,
	 MENU_SETMONTH,
	 MENU_SETDATE,
	 MENU_SETHOUR,
	 MENU_SETMINUTE
};

//添加压缩机控制;by gzz 20121224
const char *    Menu_TempSet[LANGUAGENO][5]=
{
	 {
	 "1.压缩机设置: ",	
	 "2.压缩机启动时: ",
	 "3.压缩机启动分: ",
	 "4.压缩机关闭时: ",
	 "5.压缩机关闭分: "
	 },
	 {
	 "1.Temp Set:",
	 "2.TempStartHour:",
	 "3.TempStartMinute:",
	 "4.TempEndHour:",
	 "5.TempEndMinute:"
	 },
	 //change by liya 2012-10-16
	 {
	 "1.регулирование температуры:",
	 "2.Начать часов:",
	 "3.Начало минут:",
	 "4.Закрыть час:",
	 "5.Закрыть минуту:"
	 },
	 {
	 "1.S\xf1\x31\cakl\xf1\x31k kontrol\xf0\xfc:",
	 "2.Saat ba\xf1\x5flay\xf1\x31n:",
	 "3.Dakikada ba\xf1\x5flay\xf1\x31n:",
	 "4.Kapat saat:",
	 "5.Kapat dakika:"
	 }
};

//添加压缩机控制;by gzz 20121224
const unsigned int  Menu_TempSetID[5]=
{
	 MENU_TEMPON,
	 MENU_TEMPHOURSTR,
	 MENU_TEMPMINUTESTR,
	 MENU_TEMPHOUREND,
	 MENU_TEMPMINUTEEND
}; 

//添加展示灯控制;by gzz 20121229
const char *    Menu_LEDSet[LANGUAGENO][5]=
{
	 {
	 "1.展示灯设置: ",	
	 "2.展示灯启动时: ",
	 "3.展示灯启动分: ",
	 "4.展示灯关闭时: ",
	 "5.展示灯关闭分: "
	 },
	 {
	 "1.LED Set:",
	 "2.LEDStartHour:",
	 "3.LEDStartMinute:",
	 "4.LEDEndHour:",
	 "5.LEDEndMinute:"
	 },
	 //change by liya 2012-10-16
	 {
	 "1.LED температуры:",
	 "2.Начать часов:",
	 "3.Начало минут:",
	 "4.Закрыть час:",
	 "5.Закрыть минуту:"
	 },
	 {
	 "1.LED kontrol\xf0\xfc:",
	 "2.Saat ba\xf1\x5flay\xf1\x31n:",
	 "3.Dakikada ba\xf1\x5flay\xf1\x31n:",
	 "4.Kapat saat:",
	 "5.Kapat dakika:"
	 }
};

//添加展示灯控制;by gzz 20121229
const unsigned int  Menu_LEDSetID[5]=
{
	 MENU_LEDON,
	 MENU_LEDHOURSTR,
	 MENU_LEDMINUTESTR,
	 MENU_LEDHOUREND,
	 MENU_LEDMINUTEEND
}; 


//
const char *     Menu_InputLayerNum[LANGUAGENO][1]=
{
	 {
	 "请输入层号: "
	 },
	 {
	  "Input tray code:"
	 },
	 //change by liya 2012-10-16
	 {
	 "Введите код лотка:"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "Katman numaras\xf1\x31n\xf1\x31 girin: "
	 }
};



const unsigned int  Menu_InputLayerNumID[1]=
{
	  MENU_INPUTLAYER
};
 

const char *     Menu_SetChannel[LANGUAGENO][SETCHANNELNO]=
{
	 {
	 "1.层架编号:   ",
	 "2.是否启用:   ",
	 "3.存货量设置: ",
	 "4.第一货道:   ",
	 "5.第一货道存量: ",
	 "6.第二货道:   ",
	 "7.第二货道存量: ",
	 "8.第三货道:   ",
	 "9.第三货道存量: ",
	 "10. 第四货道:   ",
	 "11. 第四货道存量: ",
	 "12. 第五货道:   ",
	 "13. 第五货道存量: ",
	 "14. 第六货道:   ",
	 "15. 第六货道存量: ",
	 "16. 第七货道: ",
	 "17. 第七货道存量: ",
	 "18. 第八货道: ",
	 "19. 第八货道存量: ",
	 "20. 第九货道: ",
	 "21. 第九货道存量: ",
	 "22. 第零货道: ",
	 "23. 第零货道存量: "
	 },
	 {
	 "1.Tray code:",
	 "2.Enabled:  ",
	 "3.Goods Max:",
	 "4.Column 1: ",
	 "5.Column 1Num: ",
	 "6.Column 2: ",
	 "7.Column 2Num: ",
	 "8.Column 3: ",
	 "9.Column 3Num: ",
	 "10.Column 4: ",
	 "11.Column 4Num: ",
	 "12.Column 5: ",
	 "13.Column 5Num: ",
	 "14.Column 6: ",
	 "15.Column 6Num: ",
	 "16.Column 7:",
	 "17.Column 7Num: ",
	 "18.Column 8:",
	 "19.Column 8Num: ",
	 "20.Column 9:",
	 "21.Column 9Num: ",
	 "22.Column 0:",
	 "23.Column 0Num: "
	 },
	 //change by liya 2012-10-16
	 {
	 "1.Код лотка:",
	 "2.Включен:  ",
	 "3.Макс. порций:",
	 "4.Колонка 1: ",
	 "5.Колонка 1количество: ",
	 "6.Колонка 2: ",
	 "7.Колонка 2количество: ",
	 "8.Колонка 3: ",
	 "9.Колонка 3количество: ",
	 "10.Колонка 4: ",
	 "11.Колонка 4количество: ",
	 "12.Колонка 5: ",
	 "13.Колонка 5количество: ",
	 "14.Колонка 6: ",
	 "15.Колонка 6количество: ",
	 "16.Колонка 7:",
	 "17.Колонка 7количество: ",
	 "18.Колонка 8:",
	 "19.Колонка 8количество: ",
	 "20.Колонка 9:",
	 "21.Колонка 9количество: ",
	 "22.Колонка 0:",
	 "23.Колонка 0количество: "
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "1.Tepsi kodu:   ",
	 "2.Etkin:   ",
	 "3.Max \xf0\xfcr\xf0\xfcn: ",
	 "4.S\xf0\xfctun  1:",
	 "5.S\xf0\xfctun  1miktar:",
	 "6.S\xf0\xfctun  2:",
	 "7.S\xf0\xfctun  2miktar:",
	 "8.S\xf0\xfctun  3:",
	 "9.S\xf0\xfctun  3miktar:",
	 "10.S\xf0\xfctun  4:",
	 "11.S\xf0\xfctun  4miktar:",
	 "12.S\xf0\xfctun  5:",
	 "13.S\xf0\xfctun  5miktar:",
	 "14.S\xf0\xfctun  6:",
	 "15.S\xf0\xfctun  6miktar:",
	 "16.S\xf0\xfctun  7:",
	 "17.S\xf0\xfctun  7miktar:",
	 "18.S\xf0\xfctun  8:",
	 "19.S\xf0\xfctun  8miktar:",
	 "20.S\xf0\xfctun  9:",
	 "21.S\xf0\xfctun  9miktar:",
	 "22.S\xf0\xfctun  0:",
	 "23.S\xf0\xfctun  0miktar:"
	 }
};
const  unsigned int  Menu_SetChannelID[SETCHANNELNO]=
{
	 MENU_LAYERNO,
	 MENU_LAYENABLE,
	 MENU_MAXGOODS,
	 MENU_CHANNEL1,
	 MENU_CHANNELNUM1,//按货道添存货量;by gzz 20121120
	 MENU_CHANNEL2,
	 MENU_CHANNELNUM2,
	 MENU_CHANNEL3,
	 MENU_CHANNELNUM3,
	 MENU_CHANNEL4,
	 MENU_CHANNELNUM4,
	 MENU_CHANNEL5,
	 MENU_CHANNELNUM5,
	 MENU_CHANNEL6,
	 MENU_CHANNELNUM6,
	 MENU_CHANNEL7,
	 MENU_CHANNELNUM7,
	 MENU_CHANNEL8,
	 MENU_CHANNELNUM8,
	 MENU_CHANNEL9,
	 MENU_CHANNELNUM9,
	 MENU_CHANNEL0,
	 MENU_CHANNELNUM0
};
 
const char *     Menu_NewPassword[LANGUAGENO][2]=
{
	 {
	 "输入新密码",
	 "再次输入  "
	 },
	 {
	 "NEW PW    ",
	 "REPEAT    "
	 },
	 //change by liya 2012-10-16
	 {
	 "Новый PW",
	 "Повторить"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "Yeni \xf1\x5fifreyi girin",
	 "yeniden girin  "
	 }
};
const  unsigned int  Menu_NewPasswordID[2]=
{
	 MENU_NEWPW,
	 MENU_NEWPWREPEAT
};
 
const  char *     Menu_Confirmation[LANGUAGENO][2]=
{
	 {
	 "你确定吗？" ,
	 "确认ENTER 返回CANCEL"
	 },
	 {
	 "Are you sure?",
	 "ENTER or CANCEL"
	 },
	 //change by liya 2012-10-16
	 {
	 "Вы уверенны?",
	 "Ввод   или   Выход"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "Emin misiniz?",
	 "Giri\xf1\x5f yada iptal"
	 }
};
const unsigned int  Menu_ConfirmationID[2]=
{
	MENU_CONFIRMATION ,
	MENU_NULL
};

//用于季度结账用;by gzz 20121009
const unsigned int  Menu_ConfirmationIDJidu[2]=
{
	MENU_CONFIRMATIONJIDU ,
	MENU_NULL
};


const char *     Menu_SetConfirmation[LANGUAGENO][2]=
{
	 {
	 "你确定吗？" ,
	 "确认ENTER 返回CANCEL"
	 },
	 {
	 "Are you sure?",
	 "ENTER or CANCEL"
	 },
	 //change by liya 2012-10-16
	 {
	 "Вы уверенны?",
	 "Ввод   или   Выход"
	 },
	 //土耳其语;by gzz 20121115
	 {
	 "Emin misiniz?",
	 "Giri\xf1\x5f yada iptal"
	 }
};




const unsigned int  Menu_SetConfirmationID[2]=
{
	MENU_CONFIRMSET ,
	MENU_NULL
};


//用户模式菜单;by gzz 20121130
const unsigned int  Menu_SetAdminID[2]=
{
	MENU_CONFIRMSET ,
	MENU_NULL
};


//
const char  MenuNULL[]={"              "};
//



/*********************************************************
数组，用于跳转操作页面

//CurMenuID=本菜单ID
//MaxMenuItem=同级菜单最大项数
//OkMenuID=子菜单层所对应的菜单ID,ID=999为菜单已经到底了
//EscMenuID=父菜单层所对应的菜单ID,ID=999为菜单已经到顶了
//DownMenuID=弟菜单层所对应的菜单ID,ID=999为菜单是独生子
//UpMenuID=兄菜单层所对应的菜单ID,ID=999为菜单是独生子
//CurFunction=本菜单所对应的菜单函数指针
typedef struct
 {
 	unsigned short int KeyTab_MenuIndex;  		//当前状态索引号
 	unsigned short int KeyTab_MaxItems;  		//本级菜单最大条目数
    unsigned short int KeyTab_PressOk;   	 	//按下"回车"键时转向的状态索引号
    unsigned short int KeyTab_PressEsc;  		//按下"返回"键时转向的状态索引号
    unsigned short int KeyTab_PressDown;  		//按下"向下"键时转向的状态索引号
    unsigned short int KeyTab_PressUp;    		//按下"向上"键时转向的状态索引号
 	uint8_t    	(*CurrentOperate)(void);   		//当前选项应该执行的功能操作
	char * 		(*InitKeyTab)(void);			//当前选项输入的参数
 }KeyTabStruct;									//菜单项结构

***********************************************************/

//
KeyTabStruct  KeyTab[MAX_KEYTABSTRUCT_NUM]=
	{		  
		//CurMenuID,	  MaxMenuItem,			OkMenuID,			EscMenuID,			DownMenuID,				UpMenuID,			IsVisible				CurFunction				  		InitKeyTab
		{MENU_PASSWORD,			1,			MENU_INPUTLAYER,		MENU_SETCHANNEL,	0,						999,				MENU_VISIBLE_TRUE,		*MenuFunctionPW,				*MenuInitPWIN	},//0
		{MENU_CHANNEL,			ROOTNO,		MENU_INPUTNUM,			999,				MENU_ADDGOODS,			MENU_DEFAULTSET,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo1,				*MenuInit0	},       
		{MENU_INPUTNUM,			1,			MENU_CHANNELNO,			MENU_CHANNEL,		999,					999,				MENU_VISIBLE_TRUE,		*MenuFunctionCHIN,				*MenuInitCHIN	},
		{MENU_CHANNELNO,		CHANNELNO,			999,			MENU_INPUTNUM,		MENU_PRICE,				MENU_CHANNELGOODS,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo1,				*MenuInitCHNO	},
		{MENU_PRICE,			CHANNELNO,			999,			MENU_INPUTNUM,		MENU_CHANNELLEFT,		MENU_CHANNELNO,		MENU_VISIBLE_TRUE,		*MenuFunctionCHPR,				*MenuInitCHPR	},
		{MENU_CHANNELLEFT,		CHANNELNO,			999,			MENU_INPUTNUM,		MENU_CHANNELSTAT,		MENU_PRICE,     	MENU_VISIBLE_TRUE,		*MenuFunctionCHLF,				*MenuInitCHLF	},//5
		{MENU_CHANNELSTAT,		CHANNELNO,			999,			MENU_INPUTNUM,		MENU_CHANNELGOODS,	    MENU_CHANNELLEFT,	MENU_VISIBLE_TRUE,		*MenuFunctionCHST,				*MenuInitCHST	},	
		{MENU_CHANNELGOODS,		CHANNELNO,			999,			MENU_INPUTNUM,		MENU_CHANNELNO,		    MENU_CHANNELSTAT,	MENU_VISIBLE_TRUE,		*MenuFunctionCHGOODS,			*MenuInitCHGOODS},		
		{MENU_ADDGOODS,			ROOTNO,		MENU_ADDINPUTLAY,		999,				MENU_DRIVER,			MENU_CHANNEL,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo1, 			*MenuInit0	},
		{MENU_ADDINPUTLAY,		1,			MENU_INPUTADDGOODS,		MENU_ADDGOODS,		999,					999,				MENU_VISIBLE_TRUE,		*MenuFunctionADDLAYIN, 			*MenuInitLAYIN	},
		{MENU_INPUTADDGOODS,	2,			MENU_ADDINPUTLAY,		MENU_ADDINPUTLAY,	999,					999,				MENU_VISIBLE_TRUE,		*MenuFunctionAddGoods, 			*MenuInitAddGoods	},
		{MENU_DRIVER,			ROOTNO,		MENU_NOTESTAT,			999,				MENU_BUSINESS,			MENU_ADDGOODS,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo6,				*MenuInit0	},   //10
		{MENU_NOTESTAT, 		DRIVERNO,	MENU_NOTETESTCONFIRM,	MENU_DRIVER,		MENU_COINSSTAT,         MENU_FLASHLEFT,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo7,				*MenuInitNOST	},
		{MENU_COINSSTAT,      	DRIVERNO,	MENU_COINSTESTCONFIRM,	MENU_DRIVER,		MENU_HOPPERSTAT1, 		MENU_NOTESTAT,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo8,				*MenuInitCOST	},
		{MENU_HOPPERSTAT1,      DRIVERNO,	MENU_HOPPERSTAT1,		MENU_DRIVER,		MENU_HOPPERSTAT2,    	MENU_COINSSTAT,		MENU_VISIBLE_TRUE,		*Hopper1Test,					*MenuInitHOST1	},
		{MENU_HOPPERSTAT2,      DRIVERNO,	MENU_HOPPERSTAT2,		MENU_DRIVER,		MENU_HOPPERSTAT3,    	MENU_HOPPERSTAT1,	MENU_VISIBLE_TRUE,		*Hopper2Test,					*MenuInitHOST2	},
		{MENU_HOPPERSTAT3,      DRIVERNO,	MENU_HOPPERSTAT3,		MENU_DRIVER,		MENU_GPRSTEST,    		MENU_HOPPERSTAT2,	MENU_VISIBLE_TRUE,		*Hopper3Test,					*MenuInitHOST3	},
		{MENU_GPRSTEST,         DRIVERNO,			999,			MENU_DRIVER,		MENU_SMSTEST,			MENU_HOPPERSTAT3, 	MENU_VISIBLE_TRUE,		*MenuFunctionDemo12,			*MenuInit0	},
		{MENU_SMSTEST,			DRIVERNO,			999,			MENU_DRIVER,		MENU_KEYTEST,     		MENU_GPRSTEST, 		MENU_VISIBLE_TRUE,		*MenuFunctionDemo13,			*MenuInit0	},
		{MENU_KEYTEST,          DRIVERNO,	MENU_DOKEYTEST,			MENU_DRIVER,		MENU_ALLCHANNELSTAT,	MENU_SMSTEST,		MENU_VISIBLE_TRUE,		*MenuFunctionKeyTest,			*MenuInit0	},
		{MENU_DOKEYTEST,        1,					999,			MENU_KEYTEST,		999, 					999,				MENU_VISIBLE_TRUE,		*MenuFunctionDemo1,				*MenuInit0	},
		{MENU_ALLCHANNELSTAT,   DRIVERNO,	MENU_SUBALLCHANNELSTAT,	MENU_DRIVER,		MENU_CHANNELTEST,		MENU_KEYTEST,  		MENU_VISIBLE_TRUE,		*MenuFunctionAllChannelStat,	*MenuInit0	},//20
		{MENU_CHANNELTEST,      DRIVERNO,	MENU_DRINPUTNUM,		MENU_DRIVER,		MENU_CHTESTRESULT,		MENU_ALLCHANNELSTAT,MENU_VISIBLE_TRUE,		*MenuFunctionChannelTest,		*MenuInit0	},
		{MENU_DRINPUTNUM,       1,			MENU_DRINPUTNUM,		MENU_CHANNELTEST,	999,					999,  				MENU_VISIBLE_TRUE,		*MenuFunctionTESTCHIN,			*MenuInitCHIN	},
		{MENU_CHTESTRESULT, 	DRIVERNO,	MENU_SUBCHTESTRESULT,	MENU_DRIVER,		MENU_IRTEST,			MENU_CHANNELTEST,  	MENU_VISIBLE_TRUE,		*MenuFunctionAllCHResult,		*MenuInit0	},
		{MENU_IRTEST,		   	DRIVERNO,	MENU_IRTEST,			MENU_DRIVER,		MENU_LCDTEST,			MENU_CHTESTRESULT,  MENU_VISIBLE_TRUE,		*MenuFunctionIRTest,			*MenuInit0	},
        {MENU_LCDTEST,		   	DRIVERNO,	MENU_DOLCDTEST,			MENU_DRIVER,		MENU_TEMPRATURETEST,	MENU_IRTEST,        MENU_VISIBLE_TRUE,		*MenuFunctionLCDTest,			*MenuInit0	},
        {MENU_DOLCDTEST,		1,      			999,			MENU_LCDTEST,		999,					999,                MENU_VISIBLE_TRUE,		*MenuFunctionDOLCDTest,			*MenuInit0	},
		{MENU_TEMPRATURETEST,	DRIVERNO,	MENU_DOTEMPTEST,		MENU_DRIVER,		MENU_SELECTKEYTEST,		MENU_LCDTEST,       MENU_VISIBLE_TRUE,		*MenuFunctionTemperatureTest,	*MenuInit0	},
		{MENU_DOTEMPTEST,		1,      			999,			MENU_TEMPRATURETEST,		999,			999,      			MENU_VISIBLE_TRUE,		*MenuFunctionDoTempeTest,		*MenuInit0	},
		{MENU_SELECTKEYTEST,	DRIVERNO,	MENU_DOSELECTKEYTEST,	MENU_DRIVER,		MENU_TEMPSTEST,			MENU_TEMPRATURETEST,MENU_VISIBLE_TRUE,		*MenuFunctionXuanhuoTest,		*MenuInit0	},
		{MENU_DOSELECTKEYTEST,	1,      			999,			MENU_SELECTKEYTEST,			999,			999,      			MENU_VISIBLE_TRUE,		*MenuFunctionDoXuanhuoTest,		*MenuInit0	},
		{MENU_TEMPSTEST,	   	DRIVERNO,	MENU_DOTEMPSTEST,		MENU_DRIVER,		MENU_LEDTEST,			MENU_SELECTKEYTEST, MENU_VISIBLE_TRUE,		*MenuFunctionTempTest,			*MenuInit0	},//添加压缩机控制;by gzz 20121224
        {MENU_DOTEMPSTEST,		1,      			999,			MENU_TEMPSTEST,		999,					999,                MENU_VISIBLE_TRUE,		*MenuFunctionDoTempsTest,		*MenuInit0	},//添加压缩机控制;by gzz 20121224
		{MENU_LEDTEST,	   		DRIVERNO,	MENU_DOLEDTEST,			MENU_DRIVER,		MENU_FLASHLEFT,			MENU_TEMPSTEST, 	MENU_VISIBLE_TRUE,		*MenuFunctionLEDTest,			*MenuInit0	},//添加压缩机控制;by gzz 20121224
        {MENU_DOLEDTEST,		1,      			999,			MENU_LEDTEST,		999,					999,                MENU_VISIBLE_TRUE,		*MenuFunctionDoLEDTest,			*MenuInit0	},//添加压缩机控制;by gzz 20121224
		{MENU_FLASHLEFT,		DRIVERNO,			999,			MENU_DRIVER,		MENU_NOTESTAT,			MENU_LEDTEST, 		MENU_VISIBLE_TRUE,		*MenuFunctionDemo38,			*MenuInitFLASH	},
		//CurMenuID,	  MaxMenuItem,			OkMenuID,			EscMenuID,			DownMenuID,				UpMenuID,			IsVisible				CurFunction			
		{MENU_BUSINESS,       	ROOTNO,		MENU_COINVALUE,			999,				MENU_TRANSACTIONS,		MENU_DRIVER,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo14,			*MenuInit0	},
		{MENU_COINVALUE,		BUSINESSNO,	MENU_COINVALUE1,		MENU_BUSINESS,		MENU_NOTEVALUE,     	MENU_HOLDNOTE10,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo14,			*MenuInit0	},
		{MENU_NOTEVALUE,		BUSINESSNO,	MENU_NOTEVALUE1,		MENU_BUSINESS,		MENU_CHANGEVALUE,      	MENU_COINVALUE,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo14,			*MenuInit0	},
		{MENU_CHANGEVALUE,		BUSINESSNO,	MENU_CHANGEVALUE1,		MENU_BUSINESS,		MENU_OPENCOIN,      	MENU_NOTEVALUE,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo14,			*MenuInit0	},
		{MENU_OPENCOIN,   		BUSINESSNO,			999,			MENU_BUSINESS,		MENU_OPENNOTE,			MENU_CHANGEVALUE,	MENU_VISIBLE_TRUE,		*MenuFunctionOCOIN,				*MenuInitOCOIN	},//30
		{MENU_OPENNOTE,   		BUSINESSNO,	MENU_SELECTMEI,			MENU_BUSINESS,		MENU_OPENCHANGE,		MENU_OPENCOIN,		MENU_VISIBLE_TRUE,		*MenuFunctionONOTE,				*MenuInitONOTE	},
		{MENU_OPENCHANGE,   	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_OPENREADER,		MENU_OPENNOTE,		MENU_VISIBLE_TRUE,		*MenuFunctionOCHANGE,			*MenuInitOCHANGE	},
		{MENU_OPENREADER,   	BUSINESSNO,	MENU_READEROFF,			MENU_BUSINESS,		MENU_READERCOST,		MENU_OPENCHANGE,	MENU_VISIBLE_TRUE,		*MenuFunctionONREADER,			*MenuInitONREADER	},
		{MENU_READEROFF,		SETREADER,	MENU_OPENREADER,		MENU_OPENREADER,	MENU_READERMDB,			MENU_READERMDB,		MENU_VISIBLE_TRUE,		*MenuFunctionREADERSetOFF,		*MenuInit0	},
		{MENU_READERMDB,		SETREADER,	MENU_OPENREADER,		MENU_OPENREADER,	MENU_READEROFF,	    	MENU_READEROFF,		MENU_VISIBLE_TRUE,		*MenuFunctionREADERSetMDB,		*MenuInit0	},
		{MENU_READERCOST,   	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_CHANGESERVER,		MENU_OPENREADER,	MENU_VISIBLE_TRUE,		*MenuFunctionONREADERCOST,		*MenuInitOREADERCOST},
		{MENU_CHANGESERVER,   	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_NOLIMITEDBUY,		MENU_READERCOST,	MENU_VISIBLE_TRUE,		*MenuFunctionCGSV,				*MenuInitCGSV	},
		{MENU_NOLIMITEDBUY,   	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_RETURNCOINS,		MENU_CHANGESERVER,	MENU_VISIBLE_TRUE,		*MenuFunctionNBUY,				*MenuInitNBUY	},
		{MENU_RETURNCOINS,   	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_BALANCEMIN,		MENU_NOLIMITEDBUY,	MENU_VISIBLE_TRUE,		*MenuFunctionRCOIN,				*MenuInitRCOIN	},
		{MENU_BALANCEMIN,		BUSINESSNO,			999,			MENU_BUSINESS,		MENU_MONEYMAXIN,		MENU_RETURNCOINS, 	MENU_VISIBLE_TRUE,		*MenuFunctionBMIN,				*MenuInitBMIN	},
		{MENU_MONEYMAXIN,		BUSINESSNO,			999,			MENU_BUSINESS,		MENU_IOU,				MENU_BALANCEMIN, 	MENU_VISIBLE_TRUE,		*MenuFunctionMMAX,				*MenuInitMMAX	},
		{MENU_IOU,				BUSINESSNO,			999,			MENU_BUSINESS,		MENU_DOUBTMONEY,		MENU_MONEYMAXIN,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo15,			*MenuInitIOU	},
		{MENU_DOUBTMONEY, 		BUSINESSNO,			999,			MENU_BUSINESS,		MENU_DECPOINTNUM,		MENU_IOU,			MENU_VISIBLE_TRUE,		*MenuFunctionDBMN,				*MenuInitDBMN	},
		{MENU_DECPOINTNUM,   	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_SALECONFIRM,		MENU_DOUBTMONEY,	MENU_VISIBLE_TRUE,		*MenuFunctionPONO,				*MenuInitPONO	},//40
		{MENU_SALECONFIRM,   	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_POSENABLE,			MENU_DECPOINTNUM,	MENU_VISIBLE_TRUE,		*MenuFunctionSACON,				*MenuInitSACON	},
		{MENU_POSENABLE, 	  	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_VMCPC,			    MENU_SALECONFIRM,	MENU_VISIBLE_TRUE,		*MenuFunctionPOSEN,				*MenuInitPOSEN	},
		{MENU_VMCPC, 	  	    BUSINESSNO,			999,			MENU_BUSINESS,		MENU_LARGELCD,			MENU_POSENABLE,		MENU_VISIBLE_TRUE,		*MenuFunctionPCEN,	    		*MenuInitPCEN	}, 
        {MENU_LARGELCD, 	  	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_SELECTKEY,			MENU_VMCPC,		    MENU_VISIBLE_TRUE,		*MenuFunctionLCDSEL,	    	*MenuInitLCDSEL	}, 
        {MENU_SELECTKEY, 	  	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_LOGOSEL,			MENU_LARGELCD,	    MENU_VISIBLE_TRUE,		*MenuFunctionKeySel,	    	*MenuInitKeySel	}, 
		{MENU_LOGOSEL, 	  		BUSINESSNO,			999,			MENU_BUSINESS,		MENU_ADDCOLUMN,			MENU_SELECTKEY,	    MENU_VISIBLE_TRUE,		*MenuFunctionLogoSel,	    	*MenuInitLogoSel	}, 
		{MENU_ADDCOLUMN,  		BUSINESSNO,			999,			MENU_BUSINESS,		MENU_TENCOLUMN,			MENU_LOGOSEL,	    MENU_VISIBLE_TRUE,		*MenuFunctionAutoColumn,    	*MenuInitAutoColumn	}, 
		{MENU_TENCOLUMN,  		BUSINESSNO,			999,			MENU_BUSINESS,		MENU_HOLDNOTE,			MENU_ADDCOLUMN,	    MENU_VISIBLE_TRUE,		*MenuFunctionTenColumn,    		*MenuInitTenColumn	}, 
		{MENU_HOLDNOTE, 	  	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_HOLDNOTE5,			MENU_TENCOLUMN,		MENU_VISIBLE_TRUE,		*MenuFunctionHOLDNOTE,			*MenuInitHOLDNOTE	},
		{MENU_HOLDNOTE5, 	  	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_HOLDNOTE10,		MENU_HOLDNOTE,		MENU_VISIBLE_TRUE,		*MenuFunctionHOLDNOTE5,			*MenuInitHOLDNOTE5	},
		{MENU_HOLDNOTE10, 	  	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_COINVALUE,			MENU_HOLDNOTE5,		MENU_VISIBLE_TRUE,		*MenuFunctionHOLDNOTE10,		*MenuInitHOLDNOTE10	},
		{MENU_HOLDNOTE20, 	  	BUSINESSNO,			999,			MENU_BUSINESS,		MENU_COINVALUE,			MENU_HOLDNOTE10,	MENU_VISIBLE_TRUE,		*MenuFunctionHOLDNOTE10,		*MenuInitHOLDNOTE10	},
		
		 //CurMenuID,	  		MaxMenuItem,		OkMenuID,			EscMenuID,			DownMenuID,				UpMenuID,			CurFunction				  InitKeyTab
		
		{MENU_TRANSACTIONS,		ROOTNO,			MENU_PAYMENTS,		999,				MENU_SYSTEM,			MENU_BUSINESS,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo23,			*MenuInit0	},
		{MENU_PAYMENTS,			TRANSACTIONSNO,	MENU_INCOME,		MENU_TRANSACTIONS,	MENU_GOODS,				MENU_CLEARTRANSJIDU,MENU_VISIBLE_TRUE,		*MenuFunctionDemo24,			*MenuInit0	},
		{MENU_INCOME,			PAYMENTSNO,			999,			MENU_PAYMENTS,		MENU_NOTEINCOME,		MENU_COINSCHANGE3,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo25,			*MenuInitINCOME	},
		{MENU_NOTEINCOME,		PAYMENTSNO,			999,			MENU_PAYMENTS,		MENU_COINSINCOME,		MENU_INCOME,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo26,			*MenuInitNOIN	},//50
		{MENU_COINSINCOME,		PAYMENTSNO,			999,			MENU_PAYMENTS,		MENU_TOTALTRANS,		MENU_NOTEINCOME,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo27,			*MenuInitCOIN	},
		{MENU_TOTALTRANS,		PAYMENTSNO,			999,			MENU_PAYMENTS,		MENU_TOTALCHANGE,		MENU_COINSINCOME,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo28,			*MenuInitTOTR	},
		{MENU_TOTALCHANGE,		PAYMENTSNO,			999,			MENU_PAYMENTS,		MENU_COINSCHANGE1,		MENU_TOTALTRANS,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo29,			*MenuInitTOCH	},
		{MENU_COINSCHANGE1,		PAYMENTSNO,			999,			MENU_PAYMENTS,		MENU_COINSCHANGE2,		MENU_TOTALCHANGE,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo30,			*MenuInitCOVA1	},
		{MENU_COINSCHANGE2,		PAYMENTSNO,			999,			MENU_PAYMENTS,		MENU_COINSCHANGE3,		MENU_COINSCHANGE1,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo30,			*MenuInitCOVA2	},
		{MENU_COINSCHANGE3,		PAYMENTSNO,			999,			MENU_PAYMENTS,		MENU_INCOME,			MENU_COINSCHANGE2,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo30,			*MenuInitCOVA3	},
		{MENU_GOODS,			TRANSACTIONSNO,	MENU_SUCCESSNUM,	MENU_TRANSACTIONS,	MENU_CLEARTRANS,		MENU_PAYMENTS,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo31,			*MenuInit0	},
		{MENU_SUCCESSNUM,		3,					999,			MENU_GOODS,			MENU_DOUBTNUM,			MENU_CHANNELTRANS,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo32,			*MenuInitSUNUM	},
		{MENU_DOUBTNUM,			3,					999,			MENU_GOODS,			MENU_CHANNELTRANS,		MENU_SUCCESSNUM,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo33,			*MenuInitDBNUM	},
		{MENU_CHANNELTRANS,		3,			MENU_TRANSINPUTNUM,		MENU_GOODS,			MENU_SUCCESSNUM,		MENU_DOUBTNUM,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo34,			*MenuInit0	},    //60
		{MENU_TRANSINPUTNUM,	1,			MENU_TRANSCHANNELNO,	MENU_CHANNELTRANS,	999,					999,				MENU_VISIBLE_TRUE,		*MenuFunctionCHIN,				*MenuInitCHIN	},
		{MENU_TRANSCHANNELNO,	4,					999,			MENU_CHANNELTRANS,	MENU_TRANSSUCCESS,		MENU_TRANSDOUBT,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo36,			*MenuInitCHNO	},
		{MENU_TRANSSUCCESS,		4,					999,			MENU_CHANNELTRANS,	MENU_TRANMONEY,			MENU_TRANSCHANNELNO,MENU_VISIBLE_TRUE,		*MenuFunctionDemo37,			*MenuInitCHSU	},
		{MENU_TRANMONEY,		4,					999,			MENU_CHANNELTRANS,	MENU_TRANSDOUBT,		MENU_TRANSSUCCESS,  MENU_VISIBLE_TRUE,		*MenuFunctionDemo37,			*MenuInitCHMONEY},
		{MENU_TRANSDOUBT,		4,	 				999,			MENU_CHANNELTRANS,	MENU_TRANSCHANNELNO,	MENU_TRANMONEY,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo38,			*MenuInitCHDO	},

		//120724 by cq RecordPassword /**/
		//{MENU_CLEARTRANS,		TRANSACTIONSNO,	MENU_CONFIRMATION,	MENU_TRANSACTIONS,	MENU_PAYMENTS,			MENU_GOODS,			*MenuFunctionDemo38,			*MenuInit0	},
		{MENU_CLEARTRANS,		TRANSACTIONSNO,	MENU_PASSWORD_2,	MENU_TRANSACTIONS,	MENU_PAYMENTSJIDU,		MENU_GOODS,			MENU_VISIBLE_TRUE,		*MenuFunctionDemo38,			*MenuInit0	},

		//{MENU_PASSWORD_2,		1,				MENU_CONFIRMATION,	MENU_CLEARTRANS,	0,						999,				*MenuFunctionPW,				*MenuInitPWIN	},//0

		//{MENU_CONFIRMATION,		2,				MENU_CLEARTRANS,	MENU_CLEARTRANS,	999,					999,				*MenuFunctionCONF,				*MenuInit0	},
		{MENU_CONFIRMATION, 	2,				MENU_CLEARTRANS,	MENU_CLEARTRANS,	999,					999,				MENU_VISIBLE_TRUE,		*MenuFunctionCONF,				*MenuInit0	},


		//{MENU_CLEARTRANS,		TRANSACTIONSNO,	MENU_CONFIRMATION,	MENU_TRANSACTIONS,	MENU_PAYMENTS,			MENU_GOODS,			*MenuFunctionDemo38,			*MenuInit0	},
		//{MENU_CONFIRMATION,		2,				MENU_CLEARTRANS,	MENU_CLEARTRANS,	999,					999,				*MenuFunctionCONF,				*MenuInit0	},

		
		{MENU_PAYMENTSJIDU, 	TRANSACTIONSNO, MENU_INCOMEJIDU,		MENU_TRANSACTIONS,		MENU_GOODSJIDU, 			MENU_CLEARTRANS,			MENU_VISIBLE_TRUE,		*MenuFunctionDemo24,			*MenuInit0	},
		{MENU_INCOMEJIDU,			PAYMENTSNO, 		999,			MENU_PAYMENTSJIDU,		MENU_NOTEINCOMEJIDU,		MENU_COINSCHANGE3JIDU,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo25,			*MenuInitINCOMEJIDU },
		{MENU_NOTEINCOMEJIDU,		PAYMENTSNO, 		999,			MENU_PAYMENTSJIDU,		MENU_COINSINCOMEJIDU,		MENU_INCOMEJIDU,			MENU_VISIBLE_TRUE,		*MenuFunctionDemo26,			*MenuInitNOINJIDU	},//50
		{MENU_COINSINCOMEJIDU,		PAYMENTSNO, 		999,			MENU_PAYMENTSJIDU,		MENU_TOTALTRANSJIDU,		MENU_NOTEINCOMEJIDU,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo27,			*MenuInitCOINJIDU	},
		{MENU_TOTALTRANSJIDU,		PAYMENTSNO, 		999,			MENU_PAYMENTSJIDU,		MENU_TOTALCHANGEJIDU,		MENU_COINSINCOMEJIDU,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo28,			*MenuInitTOTRJIDU	},
		{MENU_TOTALCHANGEJIDU,		PAYMENTSNO, 		999,			MENU_PAYMENTSJIDU,		MENU_COINSCHANGE1JIDU,		MENU_TOTALTRANSJIDU,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo29,			*MenuInitTOCHJIDU	},
		{MENU_COINSCHANGE1JIDU, 	PAYMENTSNO, 		999,			MENU_PAYMENTSJIDU,		MENU_COINSCHANGE2JIDU,		MENU_TOTALCHANGEJIDU,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo30,			*MenuInitCOVA1JIDU	},
		{MENU_COINSCHANGE2JIDU, 	PAYMENTSNO, 		999,			MENU_PAYMENTSJIDU,		MENU_COINSCHANGE3JIDU,		MENU_COINSCHANGE1JIDU,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo30,			*MenuInitCOVA2JIDU	},
		{MENU_COINSCHANGE3JIDU, 	PAYMENTSNO, 		999,			MENU_PAYMENTSJIDU,		MENU_INCOMEJIDU,			MENU_COINSCHANGE2JIDU,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo30,			*MenuInitCOVA3JIDU	},
		{MENU_GOODSJIDU,	    	TRANSACTIONSNO, MENU_CHANNELTRANSJIDU,	MENU_TRANSACTIONS,	MENU_CLEARTRANSJIDU,	    MENU_PAYMENTSJIDU,			MENU_VISIBLE_TRUE,		*MenuFunctionDemo31,			*MenuInit0	},
		{MENU_CHANNELTRANSJIDU, 	1,			MENU_TRANSINPUTNUMJIDU, 	MENU_GOODSJIDU, 		999,					999,						MENU_VISIBLE_TRUE,		*MenuFunctionDemo34,			*MenuInit0	},	  //60
		{MENU_TRANSINPUTNUMJIDU,	1,			MENU_TRANSCHANNELNOJIDU,	MENU_CHANNELTRANSJIDU,	999,					999,						MENU_VISIBLE_TRUE,		*MenuFunctionCHIN,				*MenuInitCHIN	},
		{MENU_TRANSCHANNELNOJIDU,	3,					999,			MENU_CHANNELTRANSJIDU,	MENU_TRANSSUCCESSJIDU,		MENU_TRANMONEYJIDU,			MENU_VISIBLE_TRUE,		*MenuFunctionDemo36,			*MenuInitCHNO	},
		{MENU_TRANSSUCCESSJIDU, 	3,					999,			MENU_CHANNELTRANSJIDU,	MENU_TRANMONEYJIDU, 		MENU_TRANSCHANNELNOJIDU,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo37,			*MenuInitCHSUJIDU	},
		{MENU_TRANMONEYJIDU,		3,					999,			MENU_CHANNELTRANSJIDU,	MENU_TRANSCHANNELNOJIDU,	MENU_TRANSSUCCESSJIDU,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo37,			*MenuInitCHMONEYJIDU},
		
		//120724 by cq RecordPassword /**/
		//{MENU_CLEARTRANS, 	TRANSACTIONSNO, MENU_CONFIRMATION,	MENU_TRANSACTIONS,	MENU_PAYMENTS,			MENU_GOODS, 		*MenuFunctionDemo38,			*MenuInit0	},
		{MENU_CLEARTRANSJIDU,	TRANSACTIONSNO, MENU_PASSWORD_3,	MENU_TRANSACTIONS,	MENU_PAYMENTS,			MENU_GOODSJIDU,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo38,			*MenuInit0	},

		//{MENU_PASSWORD_2, 	1,				MENU_CONFIRMATION,	MENU_CLEARTRANS,	0,						999,				*MenuFunctionPW,				*MenuInitPWIN	},//0

		//{MENU_CONFIRMATION,		2,				MENU_CLEARTRANS,	MENU_CLEARTRANS,	999,					999,				*MenuFunctionCONF,				*MenuInit0	},
		{MENU_CONFIRMATIONJIDU, 	2,				MENU_CLEARTRANSJIDU,MENU_CLEARTRANSJIDU,999,					999,			MENU_VISIBLE_TRUE,		*MenuFunctionCONFJIDU,			*MenuInit0	},

		//CurMenuID,	  		MaxMenuItem,		OkMenuID,			EscMenuID,			DownMenuID,				UpMenuID,			CurFunction				  InitKeyTab
		
		{MENU_LANGUAGE,			1,				MENU_ENGLISH,		999,				MENU_CHANNEL,			MENU_TRANSACTIONS,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo1,				*MenuInit0	},
	    {MENU_ENGLISH,			3,				MENU_CHANNEL,		999,				MENU_CHINESE,			MENU_RUSSIAN,		MENU_VISIBLE_TRUE,		*MenuFunctionENG,				*MenuInit0	},
		{MENU_CHINESE,			3,				MENU_CHANNEL,		999,				MENU_RUSSIAN,			MENU_ENGLISH,		MENU_VISIBLE_TRUE,		*MenuFunctionCHN,				*MenuInit0	},
		{MENU_RUSSIAN,			3,				MENU_CHANNEL,		999,				MENU_ENGLISH,			MENU_CHINESE,		MENU_VISIBLE_TRUE,		*MenuFunctionRussian,			*MenuInit0	},//俄罗斯文;by gzz 20121012
		{MENU_SYSTEM,			ROOTNO,			MENU_TIMESET,		999,				MENU_SETCHANNEL,		MENU_TRANSACTIONS,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo1,				*MenuInit0	},   //70
	    {MENU_TIMESET,			SYSTEMNO,		MENU_SETYEAR,		MENU_SYSTEM,		MENU_GPRSONOFF,			MENU_LEDSET,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo1,				*MenuInit0	},
	    {MENU_SETYEAR,			5,					999,			MENU_TIMESET,		MENU_SETMONTH,			MENU_SETMINUTE,		MENU_VISIBLE_TRUE,		*MenuFunctionYear,				*MenuInitYear	},
		{MENU_SETMONTH,			5,					999,			MENU_TIMESET,		MENU_SETDATE,			MENU_SETYEAR,		MENU_VISIBLE_TRUE,		*MenuFunctionMonth,				*MenuInitMonth	},
	   
	   //CurMenuID,	  		MaxMenuItem,		OkMenuID,			EscMenuID,			DownMenuID,				UpMenuID,			CurFunction				 		 InitKeyTab
		{MENU_SETDATE,			5,					999,			MENU_TIMESET,		MENU_SETHOUR,			MENU_SETMONTH,		MENU_VISIBLE_TRUE,		*MenuFunctionDate,				*MenuInitDate	},
		{MENU_SETHOUR,			5,					999,			MENU_TIMESET,		MENU_SETMINUTE,			MENU_SETDATE,		MENU_VISIBLE_TRUE,		*MenuFunctionHour,				*MenuInitHour	},
		{MENU_SETMINUTE,		5,					999,			MENU_TIMESET,		MENU_SETYEAR,			MENU_SETHOUR,		MENU_VISIBLE_TRUE,		*MenuFunctionMinute,			*MenuInitMinute	},
		{MENU_CGPASSWORD,		SYSTEMNO,			999,			MENU_SYSTEM,		MENU_GPRSONOFF,			MENU_TIMESET,		MENU_VISIBLE_FAUSE,		*MenuFunctionDemo1,				*MenuInit0	},
//		{MENU_CGPASSWORD,		SYSTEMNO,		MENU_NEWPW,			MENU_SYSTEM,		MENU_GPRSONOFF,			MENU_TIMESET,		*MenuFunctionDemo1,				*MenuInit0	},
		{MENU_NEWPW,			2,					999,			MENU_CGPASSWORD,	MENU_NEWPWREPEAT,		MENU_NEWPW,			MENU_VISIBLE_FAUSE,		*MenuFunctionNEWPW,				*MenuInit0	},
		{MENU_NEWPWREPEAT,		2,					999,			MENU_CGPASSWORD,	MENU_NEWPW,				MENU_NEWPWREPEAT,	MENU_VISIBLE_FAUSE,		*MenuFunctionNEWPW2,			*MenuInit0	},
		{MENU_GPRSONOFF,		SYSTEMNO,			999,			MENU_SYSTEM,		MENU_SERIALNO,			MENU_TIMESET,		MENU_VISIBLE_FAUSE,		*MenuFunctionGPRS,				*MenuInitGPRS	},//80
		{MENU_SERIALNO,			SYSTEMNO,			999,			MENU_SYSTEM,		MENU_GSMNO,				MENU_GPRSONOFF,		MENU_VISIBLE_FAUSE,		*MenuFunctionSENO,				*MenuInitSENO	},
		{MENU_GSMNO,			SYSTEMNO,			999,			MENU_SYSTEM,		MENU_TEMPERATURE,		MENU_SERIALNO,		MENU_VISIBLE_FAUSE,		*MenuFunctionGSMNO,				*MenuInitGSMNO	},
        {MENU_TEMPERATURE,		SYSTEMNO,			999,			MENU_SYSTEM,		MENU_SYSLANGUAGE,		MENU_GSMNO,			MENU_VISIBLE_FAUSE,		*MenuFunctionTemprature,		*MenuInitTemerature	},
		{MENU_SYSLANGUAGE,		SYSTEMNO,		MENU_SYSENGLISH,		MENU_SYSTEM,		MENU_TEMPSET,			MENU_TEMPERATURE,			MENU_VISIBLE_FAUSE,		*MenuFunctionDemo1,				*MenuInit0	},
	    {MENU_SYSENGLISH,		LANGUAGENO,		MENU_SYSLANGUAGE,		MENU_SYSLANGUAGE,	MENU_SYSCHINESE,		MENU_SYSTURKEY,				MENU_VISIBLE_FAUSE,		*MenuFunctionENG,				*MenuInit0	},
		{MENU_SYSCHINESE,		LANGUAGENO,		MENU_SYSLANGUAGE,		MENU_SYSLANGUAGE,	MENU_SYSRUSSIAN,		MENU_SYSENGLISH,			MENU_VISIBLE_FAUSE,		*MenuFunctionCHN,				*MenuInit0	},
		{MENU_SYSRUSSIAN,		LANGUAGENO,		MENU_SYSLANGUAGE,		MENU_SYSLANGUAGE,	MENU_SYSTURKEY,			MENU_SYSCHINESE,			MENU_VISIBLE_FAUSE,		*MenuFunctionRussian,			*MenuInit0	},//俄罗斯文;by gzz 20121012
		{MENU_SYSTURKEY	,		LANGUAGENO,		MENU_SYSLANGUAGE,		MENU_SYSLANGUAGE,	MENU_SYSENGLISH,		MENU_SYSRUSSIAN,			MENU_VISIBLE_FAUSE,		*MenuFunctionTurkey,			*MenuInit0	},//俄罗斯文;by gzz 20121012
		{MENU_TEMPSET,			SYSTEMNO,		MENU_TEMPON,			MENU_SYSTEM,		MENU_LEDSET,			MENU_SYSLANGUAGE,			MENU_VISIBLE_TRUE,		*MenuFunctionDemo1,				*MenuInit0	},//添加压缩机控制;by gzz 20121224
		{MENU_TEMPON,				5,					999,			MENU_TEMPSET,		MENU_TEMPHOURSTR,		MENU_TEMPMINUTEEND,			MENU_VISIBLE_TRUE,		*MenuFunctionCompressOn,		*MenuInitCompressOn	},
		{MENU_TEMPHOURSTR,			5,					999,			MENU_TEMPSET,		MENU_TEMPMINUTESTR,		MENU_TEMPON,				MENU_VISIBLE_TRUE,		*MenuFunctionCompressHourOn,	*MenuInitCompressHourOn	},
		{MENU_TEMPMINUTESTR,		5,					999,			MENU_TEMPSET,		MENU_TEMPHOUREND,		MENU_TEMPHOURSTR,			MENU_VISIBLE_TRUE,		*MenuFunctionCompressMinuteOn,	*MenuInitCompressMinuteOn	},
		{MENU_TEMPHOUREND,			5,					999,			MENU_TEMPSET,		MENU_TEMPMINUTEEND,		MENU_TEMPMINUTESTR,			MENU_VISIBLE_TRUE,		*MenuFunctionCompressHourOff,	*MenuInitCompressHourOff	},
		{MENU_TEMPMINUTEEND,		5,					999,			MENU_TEMPSET,		MENU_TEMPON,			MENU_TEMPHOUREND,			MENU_VISIBLE_TRUE,		*MenuFunctionCompressMinuteOff,	*MenuInitCompressMinuteOff	},
		{MENU_LEDSET,			SYSTEMNO,		MENU_LEDON,				MENU_SYSTEM,		MENU_TIMESET,			MENU_TEMPSET,				MENU_VISIBLE_TRUE,		*MenuFunctionDemo1,				*MenuInit0	},//添加展示灯控制;by gzz 20121229
		{MENU_LEDON,				5,					999,			MENU_LEDSET,		MENU_LEDHOURSTR,		MENU_LEDMINUTEEND,			MENU_VISIBLE_TRUE,		*MenuFunctionLEDOn,				*MenuInitLEDOn	},
		{MENU_LEDHOURSTR,			5,					999,			MENU_LEDSET,		MENU_LEDMINUTESTR,		MENU_LEDON,					MENU_VISIBLE_TRUE,		*MenuFunctionLEDHourOn,			*MenuInitLEDHourOn	},
		{MENU_LEDMINUTESTR,			5,					999,			MENU_LEDSET,		MENU_LEDHOUREND,		MENU_LEDHOURSTR,			MENU_VISIBLE_TRUE,		*MenuFunctionLEDMinuteOn,		*MenuInitLEDMinuteOn	},
		{MENU_LEDHOUREND,			5,					999,			MENU_LEDSET,		MENU_LEDMINUTEEND,		MENU_LEDMINUTESTR,			MENU_VISIBLE_TRUE,		*MenuFunctionLEDHourOff,		*MenuInitLEDHourOff	},
		{MENU_LEDMINUTEEND,			5,					999,			MENU_LEDSET,		MENU_LEDON,				MENU_LEDHOUREND,			MENU_VISIBLE_TRUE,		*MenuFunctionLEDMinuteOff,		*MenuInitLEDMinuteOff	},
		{MENU_SETCHANNEL,		ROOTNO,			MENU_PASSWORD,		999,				MENU_DEFAULTSET,		MENU_SYSTEM,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo23,			*MenuInit0	},
		{MENU_INPUTLAYER,		1,				MENU_LAYERNO,		MENU_SETCHANNEL,	999,					999,				MENU_VISIBLE_TRUE,		*MenuFunctionLAYIN,				*MenuInitLAYIN	},

		//121105 by cq TenChannels
		//{MENU_LAYERNO,		SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_LAYENABLE,			MENU_CHANNEL8,		*MenuFunctionDemo1,				*MenuInitLAYNO	},
		//CurMenuID,	  		MaxMenuItem,		OkMenuID,			EscMenuID,			DownMenuID,				UpMenuID,			CurFunction				 		 InitKeyTab
		{MENU_LAYERNO,		SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_LAYENABLE, 		MENU_CHANNELNUM0,		MENU_VISIBLE_TRUE,		*MenuFunctionDemo1, 			*MenuInitLAYNO	},
		{MENU_LAYENABLE,	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_MAXGOODS,			MENU_LAYERNO,			MENU_VISIBLE_TRUE,		*MenuFunctionLAYENABLE,			*MenuInitLAYENABLE	},
		{MENU_MAXGOODS,		SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNEL1,			MENU_LAYENABLE,			MENU_VISIBLE_TRUE,		*MenuFunctionMAXGOODS,			*MenuInitMAXGOODS	},
		{MENU_CHANNEL1,		SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNELNUM1,		MENU_MAXGOODS,			MENU_VISIBLE_TRUE,		*MenuFunctionCH1,				*MenuInitCH1	},
		{MENU_CHANNELNUM1,	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNEL2,			MENU_CHANNEL1,			MENU_VISIBLE_TRUE,		*MenuFunctionCH1Num,			*MenuInitCH1Num	},
		{MENU_CHANNEL2,		SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNELNUM2,		MENU_CHANNELNUM1,		MENU_VISIBLE_TRUE,		*MenuFunctionCH2,				*MenuInitCH2	},
		{MENU_CHANNELNUM2,	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNEL3,			MENU_CHANNEL2,			MENU_VISIBLE_TRUE,		*MenuFunctionCH2Num,			*MenuInitCH2Num	},
		{MENU_CHANNEL3,		SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNELNUM3,		MENU_CHANNELNUM2,		MENU_VISIBLE_TRUE,		*MenuFunctionCH3,				*MenuInitCH3	},//90
		{MENU_CHANNELNUM3,	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNEL4,			MENU_CHANNEL3,			MENU_VISIBLE_TRUE,		*MenuFunctionCH3Num,			*MenuInitCH3Num	},
		{MENU_CHANNEL4,		SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNELNUM4,		MENU_CHANNELNUM3,		MENU_VISIBLE_TRUE,		*MenuFunctionCH4,				*MenuInitCH4	},
		{MENU_CHANNELNUM4,	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNEL5,			MENU_CHANNEL4,			MENU_VISIBLE_TRUE,		*MenuFunctionCH4Num,			*MenuInitCH4Num	},
		{MENU_CHANNEL5,		SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNELNUM5,		MENU_CHANNELNUM4,		MENU_VISIBLE_TRUE,		*MenuFunctionCH5,				*MenuInitCH5	},
		{MENU_CHANNELNUM5,	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNEL6,			MENU_CHANNEL5,			MENU_VISIBLE_TRUE,		*MenuFunctionCH5Num,			*MenuInitCH5Num	},
		{MENU_CHANNEL6,		SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNELNUM6,		MENU_CHANNELNUM5,		MENU_VISIBLE_TRUE,		*MenuFunctionCH6,				*MenuInitCH6	},
		{MENU_CHANNELNUM6,	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNEL7,			MENU_CHANNEL6,			MENU_VISIBLE_TRUE,		*MenuFunctionCH6Num,			*MenuInitCH6Num	},
		{MENU_CHANNEL7,		SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNELNUM7,		MENU_CHANNELNUM6,		MENU_VISIBLE_TRUE,		*MenuFunctionCH7,				*MenuInitCH7	},
		{MENU_CHANNELNUM7,	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNEL8,			MENU_CHANNEL7,			MENU_VISIBLE_TRUE,		*MenuFunctionCH7Num,			*MenuInitCH7Num	},
		//{MENU_CHANNEL8,		SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_LAYERNO,			MENU_CHANNEL7,		*MenuFunctionCH8,				*MenuInitCH8	},
		{MENU_CHANNEL8, 	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNELNUM8,		MENU_CHANNELNUM7,		MENU_VISIBLE_TRUE,		*MenuFunctionCH8,				*MenuInitCH8	},
		{MENU_CHANNELNUM8,	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNEL9,			MENU_CHANNEL8,			MENU_VISIBLE_TRUE,		*MenuFunctionCH8Num,			*MenuInitCH8Num	},	
		{MENU_CHANNEL9, 	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNELNUM9,		MENU_CHANNELNUM8,		MENU_VISIBLE_TRUE,		*MenuFunctionCH9,				*MenuInitCH9	},
		{MENU_CHANNELNUM9,	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNEL0,			MENU_CHANNEL9,			MENU_VISIBLE_TRUE,		*MenuFunctionCH9Num,			*MenuInitCH9Num	},
		{MENU_CHANNEL0, 	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_CHANNELNUM0,		MENU_CHANNELNUM9,		MENU_VISIBLE_TRUE,		*MenuFunctionCH0,				*MenuInitCH0	},
		{MENU_CHANNELNUM0,	SETCHANNELNO,			999,			MENU_INPUTLAYER,	MENU_LAYERNO,			MENU_CHANNEL0,			MENU_VISIBLE_TRUE,		*MenuFunctionCH0Num,			*MenuInitCH0Num	},
		
		{MENU_NOTEVALUE1,		NOTEVALUENO,		999,			MENU_NOTEVALUE,		MENU_NOTEVALUE2,     	MENU_NOTEVALUE8,	MENU_VISIBLE_TRUE,		*MenuFunctionNOCH1,				*MenuInitNOCH1	},
		{MENU_NOTEVALUE2,		NOTEVALUENO,		999,			MENU_NOTEVALUE,		MENU_NOTEVALUE3,     	MENU_NOTEVALUE1,	MENU_VISIBLE_TRUE,		*MenuFunctionNOCH2,				*MenuInitNOCH2	},
		{MENU_NOTEVALUE3,		NOTEVALUENO,		999,			MENU_NOTEVALUE,		MENU_NOTEVALUE4,     	MENU_NOTEVALUE2,	MENU_VISIBLE_TRUE,		*MenuFunctionNOCH3,				*MenuInitNOCH3	},
		{MENU_NOTEVALUE4,		NOTEVALUENO,		999,			MENU_NOTEVALUE,		MENU_NOTEVALUE5,     	MENU_NOTEVALUE3,	MENU_VISIBLE_TRUE,		*MenuFunctionNOCH4,				*MenuInitNOCH4	},
		{MENU_NOTEVALUE5,		NOTEVALUENO,		999,			MENU_NOTEVALUE,		MENU_NOTEVALUE6,     	MENU_NOTEVALUE4,	MENU_VISIBLE_TRUE,		*MenuFunctionNOCH5,				*MenuInitNOCH5	},//100
		{MENU_NOTEVALUE6,		NOTEVALUENO,		999,			MENU_NOTEVALUE,		MENU_NOTEVALUE7,     	MENU_NOTEVALUE5,	MENU_VISIBLE_TRUE,		*MenuFunctionNOCH6,				*MenuInitNOCH6	},
		{MENU_NOTEVALUE7,		NOTEVALUENO,		999,			MENU_NOTEVALUE,		MENU_NOTEVALUE8,     	MENU_NOTEVALUE6,	MENU_VISIBLE_TRUE,		*MenuFunctionNOCH7,				*MenuInitNOCH7	},
		{MENU_NOTEVALUE8,		NOTEVALUENO,		999,			MENU_NOTEVALUE,		MENU_NOTEVALUE1,     	MENU_NOTEVALUE7,	MENU_VISIBLE_TRUE,		*MenuFunctionNOCH8,				*MenuInitNOCH8	},
		{MENU_COINVALUE1,		COINVALUENO,		999,			MENU_COINVALUE,		MENU_COINVALUE2,     	MENU_COINVALUE8,	MENU_VISIBLE_TRUE,		*MenuFunctionCOCH1,				*MenuInitCOCH1	},
		{MENU_COINVALUE2,		COINVALUENO,		999,			MENU_COINVALUE,		MENU_COINVALUE3,     	MENU_COINVALUE1,	MENU_VISIBLE_TRUE,		*MenuFunctionCOCH2,				*MenuInitCOCH2	},
		{MENU_COINVALUE3,		COINVALUENO,		999,			MENU_COINVALUE,		MENU_COINVALUE4,     	MENU_COINVALUE2,	MENU_VISIBLE_TRUE,		*MenuFunctionCOCH3,				*MenuInitCOCH3	},
		{MENU_COINVALUE4,		COINVALUENO,		999,			MENU_COINVALUE,		MENU_COINVALUE5,     	MENU_COINVALUE3,	MENU_VISIBLE_TRUE,		*MenuFunctionCOCH4,				*MenuInitCOCH4	},
		{MENU_COINVALUE5,		COINVALUENO,		999,			MENU_COINVALUE,		MENU_COINVALUE6,     	MENU_COINVALUE4,	MENU_VISIBLE_TRUE,		*MenuFunctionCOCH5,				*MenuInitCOCH5	},
		{MENU_COINVALUE6,		COINVALUENO,		999,			MENU_COINVALUE,		MENU_COINVALUE7,     	MENU_COINVALUE5,	MENU_VISIBLE_TRUE,		*MenuFunctionCOCH6,				*MenuInitCOCH6	},
		{MENU_COINVALUE7,		COINVALUENO,		999,			MENU_COINVALUE,		MENU_COINVALUE8,     	MENU_COINVALUE6,	MENU_VISIBLE_TRUE,		*MenuFunctionCOCH7,				*MenuInitCOCH7	},//110
		{MENU_COINVALUE8,		COINVALUENO,		999,			MENU_COINVALUE,		MENU_COINVALUE1,     	MENU_COINVALUE7,	MENU_VISIBLE_TRUE,		*MenuFunctionCOCH8,				*MenuInitCOCH8	},
		{MENU_CHANGEVALUE1,		CHANGEVALUENO,		999,			MENU_CHANGEVALUE,	MENU_CHANGEVALUE2,    	MENU_CHANGEVALUE3,	MENU_VISIBLE_TRUE,		*MenuFunctionCGVA1,				*MenuInitCGVA1	},
 	    {MENU_CHANGEVALUE2,		CHANGEVALUENO,		999,			MENU_CHANGEVALUE,	MENU_CHANGEVALUE3,    	MENU_CHANGEVALUE1,	MENU_VISIBLE_TRUE,		*MenuFunctionCGVA2,				*MenuInitCGVA2	},
 	    {MENU_CHANGEVALUE3,		CHANGEVALUENO,		999,			MENU_CHANGEVALUE,	MENU_CHANGEVALUE1,   	MENU_CHANGEVALUE2,	MENU_VISIBLE_TRUE,		*MenuFunctionCGVA3,				*MenuInitCGVA3	},
 		{MENU_DEFAULTSET,		ROOTNO,			MENU_CONFIRMSET,		999,			MENU_CHANNEL,			MENU_SETCHANNEL,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo23,			*MenuInit0	},
		{MENU_CONFIRMSET,		2,				MENU_DEFAULTSET,	MENU_DEFAULTSET,		999,					999,			MENU_VISIBLE_TRUE,		*MenuFunctionCONFSET,			*MenuInit0	},
		//{MENU_SYNGOODSCOL,		ROOTNO,			    999,			999,				MENU_ADDCOIN,			MENU_DEFAULTSET,	*MenuFunctionSynGoodsCol,		*MenuInit0	},
		//{MENU_ADDCOIN,		    ROOTNO,				999,			999,				MENU_ADDBILL,			MENU_SYNGOODSCOL,	*MenuFunctionAddCoinChanges,	*MenuInit0	},
		//{MENU_ADDBILL,		    ROOTNO,				999,			999,				MENU_CHANNEL,			MENU_ADDCOIN,	    *MenuFunctionAddBillChanges,	*MenuInit0	},
		
 	   //CurMenuID,	  			MaxMenuItem,		OkMenuID,			EscMenuID,			DownMenuID,				UpMenuID,			CurFunction				 	 InitKeyTab

		{MENU_NOTETESTCONFIRM,		2,			MENU_NOTESTAT,		MENU_NOTESTAT,			999,						999,		MENU_VISIBLE_TRUE,		*MenuFunctionNOTETEST,			*MenuInit0	},
		{MENU_COINSTESTCONFIRM,		2,			MENU_COINSSTAT,		MENU_COINSSTAT,			999,						999,		MENU_VISIBLE_TRUE,		*MenuFunctionCOINSTEST,			*MenuInit0	},
		{MENU_SUBALLCHANNELSTAT,	2,				999,			MENU_ALLCHANNELSTAT,MENU_SUBALLCHANNELSTAT,	MENU_SUBALLCHANNELSTAT,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo1,			*MenuInit0	},
		{MENU_SUBCHTESTRESULT,		2,				999,			MENU_CHTESTRESULT,	MENU_SUBCHTESTRESULT,	MENU_SUBCHTESTRESULT,	MENU_VISIBLE_TRUE,		*MenuFunctionDemo1,			*MenuInit0	},//120
		{MENU_SELECTMEI,		SETNOTE,		MENU_OPENNOTE,		MENU_OPENNOTE,		MENU_SELECTGBA,			MENU_SELECTCLOSE,	MENU_VISIBLE_TRUE,		*MenuFunctionSetMEI,				*MenuInit0	},
		{MENU_SELECTGBA,		SETNOTE,		MENU_OPENNOTE,		MENU_OPENNOTE,		MENU_SELECTITL,	    	MENU_SELECTMEI,		MENU_VISIBLE_TRUE,		*MenuFunctionSetGBA,				*MenuInit0	},
		{MENU_SELECTITL,        SETNOTE,		MENU_OPENNOTE,		MENU_OPENNOTE,		MENU_SELECTCLOSE,		MENU_SELECTGBA,		MENU_VISIBLE_TRUE,		*MenuFunctionSetITL,				*MenuInit0	},
		{MENU_SELECTCLOSE,		SETNOTE,		MENU_OPENNOTE,		MENU_OPENNOTE,		MENU_SELECTMEI,			MENU_SELECTITL,		MENU_VISIBLE_TRUE,		*MenuFunctionSetClose,				*MenuInit0	},
	//	{MENU_HOLDNOTE5,		HOLDNOTENO,		MENU_OPENNOTE,		MENU_OPENNOTE,		MENU_SELECTMEI,			MENU_SELECTGBA,		*MenuFunctionSetClose,				*MenuInit0	},
	//	{MENU_HOLDNOTE10,		HOLDNOTENO,		MENU_OPENNOTE,		MENU_OPENNOTE,		MENU_SELECTMEI,			MENU_SELECTGBA,		*MenuFunctionSetClose,				*MenuInit0	}
	

		//120724 by cq RecordPassword /**/
		//120729 by cq RecordPassword /**/
		{MENU_PASSWORD_2,		1,				MENU_CONFIRMATION,	MENU_CLEARTRANS,		0,						999,				MENU_VISIBLE_TRUE,		*MenuFunctionPW2,				*MenuInitPWIN	},//0
		{MENU_PASSWORD_3,		1,			MENU_CONFIRMATIONJIDU,	MENU_CLEARTRANSJIDU,	0,						999,				MENU_VISIBLE_TRUE,		*MenuFunctionPW3,				*MenuInitPWIN	},//0
		{MENU_PASSWORD_ADMIN,		1,		MENU_CONFIRMATIONJIDU,	MENU_CLEARTRANSJIDU,				0,						999,				MENU_VISIBLE_TRUE,		*MenuFunctionPWAdmin,			*MenuInitPWIN	},//0
};


 


/*****************************************************************************
** Function name:	DispTextLine	
**
** Descriptions:	显示菜单信息
**					
**
** parameters:		Row行，MenuText显示信息,DispAttrib没用
** Returned value:	无
** 
*****************************************************************************/
//change by liya 20121013
void DispTextLine(unsigned char Row, const char * 	MenuText,unsigned char DispAttrib)
{
//
//uint8_t  temp[16];
//change by liya 20121013
	/*
   if(SYSPara.Language == 3)//change by liya 20121013
   {
		 if (Row == 1)
		 {
			if(DispAttrib==1)
				GetScriptUnicode(1,1,MenuText,strlen(MenuText),1);
			else
				GetScriptUnicode(1,1,MenuText,strlen(MenuText),0);
		 }
		 if (Row == 2)
		 {
			if(DispAttrib==1)
				GetScriptUnicode(2,1,MenuText,strlen(MenuText),1);
			else
				GetScriptUnicode(2,1,MenuText,strlen(MenuText),0);
		 }
		 if(SYSPara.LCDSel==1)
		 {
			 if (Row == 3)
			 {
				if(DispAttrib==1)
					GetScriptUnicode(1,2,MenuText,strlen(MenuText),1);
				else
					GetScriptUnicode(1,2,MenuText,strlen(MenuText),0);
			 }
			 if (Row == 4)
			 {
				if(DispAttrib==1)
					GetScriptUnicode(2,2,MenuText,strlen(MenuText),1);
				else
					GetScriptUnicode(2,2,MenuText,strlen(MenuText),0);
			}
		 }
		 //else
		 //{
		//	GetScriptUnicode(1,2,"					   ",strlen("					  "),0);
		//	GetScriptUnicode(2,2,"					   ",strlen("					  "),0);
		 //}/
   }
   */
	//else
	//{
		 if (Row == 1)
		 {
			Write_LCD(LINE0,MenuText,strlen(MenuText));
			//添加品途屏幕的兼容;by gzz 20121025
			if(SYSPara.LCDSel==2)
			{
				 Pitu_LCDPrintf(LINE0,0,MenuText,strlen(MenuText),1,0);//拼图屏幕样式;by gzz 20121106
			}
			else
			{
				if(SYSPara.Language == 2)
				{
					if(SYSPara.LCDSel==1)
						GetScriptUnicode(1,1,MenuText,strlen(MenuText),1);
					else
						GetScriptUnicode(1,2,MenuText,strlen(MenuText),1);
				}
			}
		 }
		 if (Row == 2)
			Write_LCD(LINE1,MenuText,strlen(MenuText));
		 //添加品途屏幕的兼容;by gzz 20121025
		 if(IsLargeLCD())
		 {
			 if (Row == 3)
				Write_LCD(LINE2,MenuText,strlen(MenuText));
			 if (Row == 4)
				Write_LCD(LINE3,MenuText,strlen(MenuText));
		 }
	//}
}





/*****************************************************************************
** Function name:	DispMenu_Password	
**
** Descriptions:	跳转到调用显示密码函数 	
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
void DispMenu_Password(uint8_t udkey)				  
{
 //	ClearScreen();
//	KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
//	(*KeyInitPtr)();
//	DispTextLine(1,&Menu_Password[Language][0][0],0);
//	InputSite = 0x80+strlen(Menu_Password[Language][0])/2;
	//InputSite = 0x86;

	KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
	(*KeyInitPtr)();
	
	
	ClearScreen();

	Line_Reverse(0);
	

	DispTextLine(1,&Menu_Password[SYSPara.Language][0][0],0);
	//change by liya 2012-10-16
	DispTextLine(2,"                   ",20);
	InputSite = 0x80+strlen(Menu_Password[SYSPara.Language][0])/2;
	CursorLine = LINE0;
		
}


/*****************************************************************************
** Function name:	DispMenu_Language	
**
** Descriptions:	显示语言菜单选择
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
void DispMenu_Language(uint8_t udkey)				 //语言菜单//俄罗斯文;by gzz 20121012
{
	SYSPara.Language = 1;
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_LanguageID[cMenuTemp1])   
		{			
			//ClearScreen();
			/*
			if( cMenuTemp1	< (KeyTab[iMenuNextID].KeyTab_MaxItems -1))
			{
				Line_Reverse(0);
				DispTextLine(1,&Menu_Language[SYSPara.Language][cMenuTemp1][0],0);
				DispTextLine(2,&Menu_Language[SYSPara.Language][cMenuTemp1+1][0],1);
				CursorLine = LINE0;
			}
			else
			{
				Line_Reverse(1) ;
				DispTextLine(1,&Menu_Language[SYSPara.Language][cMenuTemp1-1][0],0);
				DispTextLine(2,&Menu_Language[SYSPara.Language][cMenuTemp1][0],1);
				CursorLine = LINE1;
			}
			*/
			/*
			Line_Reverse(0);
			DispTextLine(1,&Menu_Language[SYSPara.Language][cMenuTemp1][0],0);
			cMenuTemp1++;
			if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				 cMenuTemp1 = 0;
			DispTextLine(2,&Menu_Language[SYSPara.Language][cMenuTemp1+1][0],1);
			CursorLine = LINE0;
			break;
			*/
			ClearScreen();
			Line_Reverse(0);
			//strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
			//KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			//strcpy(strtemp1,(*KeyInitPtr)());
			//strcat(strtemp, strtemp1);
			//DispTextLine(1,strtemp,0);
			DispTextLine(1,&Menu_Language[SYSPara.Language][cMenuTemp1][0],0);
			//Line_Reverse(0);
			//InputSite = 0x80+strlen(Menu_Channel[SYSPara.Language][cMenuTemp1])/2;
			cMenuTemp1++;
			if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				 cMenuTemp1 = 0;
			
			//strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
			//KeyInitPtr=KeyTab[Menu_DriverID[cMenuTemp1]].InitKeyTab;
			//strcpy(strtemp1,(*KeyInitPtr)());
			//strcat(strtemp, strtemp1);
			//DispTextLine(2,strtemp,0);
			DispTextLine(2,&Menu_Language[SYSPara.Language][cMenuTemp1][0],0);
			CursorLine = LINE0;
				break;
		}
	}
}


/*****************************************************************************
** Function name:	DispMenu_Language	
**
** Descriptions:	显示语言菜单选择
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
void DispMenu_SysLanguage(uint8_t udkey)				 //语言菜单//俄罗斯文;by gzz 20121012
{
	SYSPara.Language = 1;
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_SysLanguageID[cMenuTemp1])   
		{			
			//ClearScreen();
			/*
			if( cMenuTemp1	< (KeyTab[iMenuNextID].KeyTab_MaxItems -1))
			{
				Line_Reverse(0);
				DispTextLine(1,&Menu_Language[SYSPara.Language][cMenuTemp1][0],0);
				DispTextLine(2,&Menu_Language[SYSPara.Language][cMenuTemp1+1][0],1);
				CursorLine = LINE0;
			}
			else
			{
				Line_Reverse(1) ;
				DispTextLine(1,&Menu_Language[SYSPara.Language][cMenuTemp1-1][0],0);
				DispTextLine(2,&Menu_Language[SYSPara.Language][cMenuTemp1][0],1);
				CursorLine = LINE1;
			}
			*/
			/*
			Line_Reverse(0);
			DispTextLine(1,&Menu_Language[SYSPara.Language][cMenuTemp1][0],0);
			cMenuTemp1++;
			if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				 cMenuTemp1 = 0;
			DispTextLine(2,&Menu_Language[SYSPara.Language][cMenuTemp1+1][0],1);
			CursorLine = LINE0;
			break;
			*/
			ClearScreen();
			Line_Reverse(0);
			//strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
			//KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			//strcpy(strtemp1,(*KeyInitPtr)());
			//strcat(strtemp, strtemp1);
			//DispTextLine(1,strtemp,0);
			DispTextLine(1,&Menu_Language[SYSPara.Language][cMenuTemp1][0],0);
			//Line_Reverse(0);
			//InputSite = 0x80+strlen(Menu_Channel[SYSPara.Language][cMenuTemp1])/2;
			cMenuTemp1++;
			if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				 cMenuTemp1 = 0;
			
			//strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
			//KeyInitPtr=KeyTab[Menu_DriverID[cMenuTemp1]].InitKeyTab;
			//strcpy(strtemp1,(*KeyInitPtr)());
			//strcat(strtemp, strtemp1);
			//DispTextLine(2,strtemp,0);
			DispTextLine(2,&Menu_Language[SYSPara.Language][cMenuTemp1][0],0);
			CursorLine = LINE0;
				break;
		}
	}
}



/*****************************************************************************
** Function name:	DispMenu_Root	
**
** Descriptions:	显示主菜单
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
//change by liya 20121013
void DispMenu_Root(uint8_t udkey)					 //根菜单
{
	uint8_t KeyTab_TempVisible;
	char strtemp[60];
	
	
	//SYSPara.LCDSel=1;
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_RootID[cMenuTemp1])   
		{
			ClearScreen();
		//	if( cMenuTemp1	< (KeyTab[iMenuNextID].KeyTab_MaxItems -1))
		//		{
//			Line_Reverse(0);
			//用户模式菜单;by gzz 20121130
			if(SYSPara1.AdminStat != 1)
			{
				//Trace("\r\n t1");
				//第一行
				DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
				Line_Reverse(0);
				//第二行
				do
				{
					cMenuTemp1++;
					KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
				}
				while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
				if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
				{
					cMenuTemp1 = 0;
					KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
					}	
				}			
				DispTextLine(2,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
				CursorLine = LINE0;

				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())
				{
					//第三行
					do
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
					}
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999) );
					if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
					{
						cMenuTemp1 = 0;
						KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
						while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
						{
							cMenuTemp1++;
							KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
						}	
					}	
					DispTextLine(3,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
					//第四行
					do
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
					}
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999) );
					if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
					{
						cMenuTemp1 = 0;
						KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
						while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
						{
							cMenuTemp1++;
							KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
						}	
					}	
					DispTextLine(4,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
					CursorLine = LINE0; 		
				}			
				break;
			}
			else
			{
				//Trace("\r\n t2");
				//第一行
				//配置用户菜单可见;by gzz 20121204
				//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
				if(SYSPara1.VisibleStat == 1)
				{
					//Trace("\r\n t3");
					KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
					if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
					{
						strcpy(strtemp,"×");
					}
					else
					{
						strcpy(strtemp,"√");
					}
					strcat(strtemp,&Menu_Root[SYSPara.Language][cMenuTemp1][0]);	
					DispTextLine(1,strtemp,1);
					//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
				}
				else
				{
					DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
				}
				Line_Reverse(0);
				
				//第二行
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				//配置用户菜单可见;by gzz 20121204
				//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
				if(SYSPara1.VisibleStat == 1)
				{
					KeyTab_TempVisible = Menu_RootID[cMenuTemp1];					
					if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
					{
						strcpy(strtemp,"×");	
					}
					else
					{
						strcpy(strtemp,"√");	
					}					
					strcat(strtemp,&Menu_Root[SYSPara.Language][cMenuTemp1][0]);	
					DispTextLine(2,strtemp,1);
					//DispTextLine(2,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
				}
				else
				{
					DispTextLine(2,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
				}
				CursorLine = LINE0;

				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())
				{
					//第三行
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					//配置用户菜单可见;by gzz 20121204
					//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
					if(SYSPara1.VisibleStat == 1)
					{
						KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
						if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
						{
							strcpy(strtemp,"×");	
						}
						else
						{
							strcpy(strtemp,"√");	
						}
						strcat(strtemp,&Menu_Root[SYSPara.Language][cMenuTemp1][0]);	
						DispTextLine(3,strtemp,1);
						//DispTextLine(3,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
					}
					else
					{
						DispTextLine(3,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
					}

					//第四行
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						  cMenuTemp1 = 0;
					//配置用户菜单可见;by gzz 20121204
					//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
					if(SYSPara1.VisibleStat == 1)
					{
						KeyTab_TempVisible = Menu_RootID[cMenuTemp1];
						if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
						{
							strcpy(strtemp,"×");	
						}
						else
						{
							strcpy(strtemp,"√");	
						}
						strcat(strtemp,&Menu_Root[SYSPara.Language][cMenuTemp1][0]);	
						DispTextLine(4,strtemp,1);
						//DispTextLine(4,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
					}
					else
					{
						DispTextLine(4,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
					}
					CursorLine = LINE0; 		
				}			
				break;
			}
		//		}
		/*
			else
			{
//				Line_Reverse(1) ;
				DispTextLine(1,&Menu_Root[Language][cMenuTemp1][0],0);
				DispTextLine(2,&Menu_Root[Language][0][0],1);
				CursorLine = LINE1;
			}
		*/
		}
	}
}



void DispMenu_InputLayerNum(uint8_t udkey)				//输入层号
{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_InputLayerNumID[cMenuTemp1])    
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();
			if( cMenuTemp1	< (KeyTab[iMenuNextID].KeyTab_MaxItems ))
			{
			
				ClearScreen();
			//	Line_Reverse(0)	;
				DispTextLine(1,&Menu_InputLayerNum[SYSPara.Language][cMenuTemp1][0],0);
				InputSite = 0x80+strlen(Menu_InputLayerNum[SYSPara.Language][cMenuTemp1])/2;
				CursorLine = LINE0;
			}
		
		}
	}
}


/*****************************************************************************
** Function name:	DispMenu_InputNum	
**
** Descriptions:	货道参数设置的菜单:显示和输入货道参数
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
void DispMenu_Channel(uint8_t udkey)					  
{
	char strtemp[48];
	char strtemp1[48];


	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{

		if(iMenuNextID==Menu_ChannelID[cMenuTemp1])
		{
				ClearScreen();
		 

				strcpy(strtemp,&Menu_Channel[SYSPara.Language][cMenuTemp1][0]);				   //写屏幕的第一行
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,0);
				Line_Reverse(0);
				
				InputSite = 0x80+strlen(Menu_Channel[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_Channel[SYSPara.Language][cMenuTemp1][0]);					//写屏幕的第二行
				KeyInitPtr=KeyTab[Menu_ChannelID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_Channel[SYSPara.Language][cMenuTemp1][0]);					//写屏幕的第二行
					KeyInitPtr=KeyTab[Menu_ChannelID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);

					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_Channel[SYSPara.Language][cMenuTemp1][0]);					//写屏幕的第二行
					KeyInitPtr=KeyTab[Menu_ChannelID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
				break;
		}	
		
	}



}

/*****************************************************************************
** Function name:	DispMenu_InputNum	
**
** Descriptions:	显示和输入货道编号
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
void DispMenu_InputNum(uint8_t udkey)				 //输入数值
{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_InputChannelNoID[cMenuTemp1])   
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();									//初始化函数
			ClearScreen();
			
			DispTextLine(1,&Menu_InputChannelNo[SYSPara.Language][cMenuTemp1][0],0);
		//	Line_Reverse(0);
			InputSite = 0x80+(strlen(Menu_InputChannelNo[SYSPara.Language][cMenuTemp1])+1)/2;
			CursorLine = LINE0;
				
		 }
	}
}



/*****************************************************************************
** Function name:	DispMenu_AddGoods	
**
** Descriptions:	添货菜单:添满全部货物
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
void DispMenu_AddGoods(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];


	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_InputAddGoodsID[cMenuTemp1])   
		{
			
				ClearScreen();
				strcpy(strtemp,&Menu_InputAddGoods[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				//change by liya 2012-10-16
				DispTextLine(1,strtemp,1);
			
				Line_Reverse(0);
				InputSite = 0x80+strlen(Menu_InputAddGoods[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				//change by liya 2012-10-16
				DispTextLine(2,&Menu_InputAddGoods[SYSPara.Language][cMenuTemp1][0],0);			 //写第二行
				InputSite = 0x90+strlen(Menu_InputAddGoods[SYSPara.Language][cMenuTemp1])/2;
				CursorLine = LINE0;			
			   	break;
		
		}
	}
}

/*****************************************************************************
** Function name:	DispMenu_AddInputChannelNo	
**
** Descriptions:	添货菜单:添满层架货物
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
void DispMenu_AddInputChannelNo(uint8_t udkey)
{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_AddInputLayerNumID[cMenuTemp1])    
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();
			if( cMenuTemp1	< (KeyTab[iMenuNextID].KeyTab_MaxItems ))
			{
			
				ClearScreen();
				//	Line_Reverse(0)	;
				DispTextLine(1,&Menu_AddInputLayerNum[SYSPara.Language][cMenuTemp1][0],1);
				//change by liya 2012-10-16
				DispTextLine(2,"                     ",0);
				InputSite = 0x80+strlen(Menu_AddInputLayerNum[SYSPara.Language][cMenuTemp1])/2;
				CursorLine = LINE0;
			}
			else
			{
				ClearScreen();
			//	Line_Reverse(1)	;
			
				DispTextLine(2,&Menu_AddInputLayerNum[SYSPara.Language][cMenuTemp1][0],0);
				CursorLine = LINE1;
			}
		}
	}
}


/*****************************************************************************
** Function name:	DispMenu_AddInputChannelNo	
**
** Descriptions:	设备管理主菜单
**					
**
** parameters:		无
** Returned value:	无
** 
*****************************************************************************/
void DispMenu_Driver(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];
	uint8_t KeyTab_TempVisible;

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_DriverID[cMenuTemp1]) 	
		{
			ClearScreen();
			//用户模式菜单;by gzz 20121130
			if(SYSPara1.AdminStat != 1)
			{	
				//第一行
				strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				//change by liya 2012-10-16
				DispTextLine(1,strtemp,1);
				
				Line_Reverse(0);
				InputSite = 0x80+strlen(Menu_Channel[SYSPara.Language][cMenuTemp1])/2;
				//cMenuTemp1++;
				//if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				//	 cMenuTemp1 = 0;
				//第二行
				do
				{
					cMenuTemp1++;
					KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
				}
				while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
				if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
				{
					cMenuTemp1 = 0;
					KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
					}	
				}
				
				strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_DriverID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					//cMenuTemp1++;
					//if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					//	 cMenuTemp1 = 0;
					//第三行
					do
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
					}
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
					if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
					{
						cMenuTemp1 = 0;
						KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
						while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
						{
							cMenuTemp1++;
							KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
						}	
					}
					
					strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_DriverID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);

					//cMenuTemp1++;
					//if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					//	 cMenuTemp1 = 0;
					//第四行
					do
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
					}
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
					if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
					{
						cMenuTemp1 = 0;
						KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
						while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
						{
							cMenuTemp1++;
							KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
						}	
					}
					
					strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_DriverID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
					break;
			}
			else
			{	
				strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				//第一行
				//配置用户菜单可见;by gzz 20121204
				//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
				if(SYSPara1.VisibleStat == 1)
				{
					KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
					if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
					{
						strcpy(strtemp1,"×");	
					}
					else
					{
						strcpy(strtemp1,"√");	
					}
					strcat(strtemp1,strtemp);	
					DispTextLine(1,strtemp1,1);
					//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
				}
				else
				{
					//change by liya 2012-10-16
					DispTextLine(1,strtemp,1);
				}
				Line_Reverse(0);
				InputSite = 0x80+strlen(Menu_Channel[SYSPara.Language][cMenuTemp1])/2;

				//第二行
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_DriverID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);				
				//配置用户菜单可见;by gzz 20121204
				//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
				if(SYSPara1.VisibleStat == 1)
				{
					KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
					if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
					{
						strcpy(strtemp1,"×");	
					}
					else
					{
						strcpy(strtemp1,"√");	
					}
					strcat(strtemp1,strtemp);	
					DispTextLine(2,strtemp1,1);
					//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
				}
				else
				{
					DispTextLine(2,strtemp,0);
				}

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					//第三行
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_DriverID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					//配置用户菜单可见;by gzz 20121204
					//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
					if(SYSPara1.VisibleStat == 1)
					{
						KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
						if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
						{
							strcpy(strtemp1,"×");	
						}
						else
						{
							strcpy(strtemp1,"√");	
						}
						strcat(strtemp1,strtemp);	
						DispTextLine(3,strtemp1,1);
						//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
					}
					else
					{
						DispTextLine(3,strtemp,0);
					}

					//第四行
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_Driver[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_DriverID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					//配置用户菜单可见;by gzz 20121204
					//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
					if(SYSPara1.VisibleStat == 1)
					{
						KeyTab_TempVisible = Menu_DriverID[cMenuTemp1];
						if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
						{
							strcpy(strtemp1,"×");	
						}
						else
						{
							strcpy(strtemp1,"√");	
						}
						strcat(strtemp1,strtemp);	
						DispTextLine(4,strtemp1,1);
						//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
					}
					else
					{
						DispTextLine(4,strtemp,0);
					}
				}
				CursorLine = LINE0;
					break;
			}	
		}
	 }
}
void DispMenu_KeyTest(uint8_t udkey)
{
 	ClearScreen();
	DispTextLine(1,&Menu_KeyTest[SYSPara.Language][0][0],0);
	//change by liya  2012-10-16
	DispTextLine(2,"                    ",0);
	InputSite = 0x90;
}
void DispMenu_DriverInputNum(uint8_t udkey)
{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_DriverInputNumID[cMenuTemp1])   
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();									//初始化函数
			ClearScreen();
			
			DispTextLine(1,&Menu_DriverInputNum[SYSPara.Language][cMenuTemp1][0],0);
		//change by liya 20121013
			DispTextLine(2,"                   ",0);
		//	Line_Reverse(0);
			//InputSite = 0x80+strlen(Menu_DriverInputNum[SYSPara.Language][cMenuTemp1])/2;
			//CursorLine = LINE0;
				
		 }
	}
}

void DispMenu_Business(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];
	uint8_t KeyTab_TempVisible;
	
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_BusinessID[cMenuTemp1])   
		{
			
			ClearScreen();
			//用户模式菜单;by gzz 20121130
			if(SYSPara1.AdminStat != 1)
			{
				//第一行
				strcpy(strtemp,&Menu_Business[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				//change by liya 2012-10-16
				DispTextLine(1,strtemp,1);
				Line_Reverse(0);
				InputSite = 0x80+strlen(Menu_Business[SYSPara.Language][cMenuTemp1])/2;
				//	cMenuTemp1++;
		//		if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				//if( cMenuTemp1	== (BusinessNo))
				//	 cMenuTemp1 = 0;
				//第二行
				do
				{
					cMenuTemp1++;
					KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
				}
				while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
				if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
				{
					cMenuTemp1 = 0;
					KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
					}	
				}
				
				strcpy(strtemp,&Menu_Business[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_BusinessID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);
				CursorLine = LINE0;

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					//cMenuTemp1++;
			//		if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					//if( cMenuTemp1	== (BusinessNo))
					//	 cMenuTemp1 = 0;
					//第三行
					do
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
					}
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
					if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
					{
						cMenuTemp1 = 0;
						KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
						while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
						{
							cMenuTemp1++;
							KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
						}	
					}
					strcpy(strtemp,&Menu_Business[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_BusinessID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);
					//cMenuTemp1++;
			//		if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					//if( cMenuTemp1	== (BusinessNo))
					//	 cMenuTemp1 = 0;
					//第四行
					do
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
					}
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
					if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
					{
						cMenuTemp1 = 0;
						KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
						while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
						{
							cMenuTemp1++;
							KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
						}	
					}
					strcpy(strtemp,&Menu_Business[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_BusinessID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
					CursorLine = LINE0;					
				}
			 	break;
			}
			else
			{
				strcpy(strtemp,&Menu_Business[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				//第一行
				//配置用户菜单可见;by gzz 20121204
				//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
				if(SYSPara1.VisibleStat == 1)
				{
					KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
					if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
					{
						strcpy(strtemp1,"×");	
					}
					else
					{
						strcpy(strtemp1,"√");	
					}
					strcat(strtemp1,strtemp);	
					DispTextLine(1,strtemp1,1);
					//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
				}
				else
				{
					//change by liya 2012-10-16
					DispTextLine(1,strtemp,1);
				}
				Line_Reverse(0);
				InputSite = 0x80+strlen(Menu_Business[SYSPara.Language][cMenuTemp1])/2;

				//第二行
				cMenuTemp1++;
		//		if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				if( cMenuTemp1	== (BusinessNo))
					 cMenuTemp1 = 0;
				strcpy(strtemp,&Menu_Business[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_BusinessID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				//配置用户菜单可见;by gzz 20121204
				//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
				if(SYSPara1.VisibleStat == 1)
				{
					KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
					if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
					{
						strcpy(strtemp1,"×");	
					}
					else
					{
						strcpy(strtemp1,"√");	
					}
					strcat(strtemp1,strtemp);	
					DispTextLine(2,strtemp1,1);
					//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
				}
				else
				{
					DispTextLine(2,strtemp,0);
				}
				CursorLine = LINE0;

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					//第三行
					cMenuTemp1++;
			//		if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					if( cMenuTemp1	== (BusinessNo))
						 cMenuTemp1 = 0;
					strcpy(strtemp,&Menu_Business[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_BusinessID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					//配置用户菜单可见;by gzz 20121204
					//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
					if(SYSPara1.VisibleStat == 1)
					{
						KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
						if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
						{
							strcpy(strtemp1,"×");	
						}
						else
						{
							strcpy(strtemp1,"√");	
						}
						strcat(strtemp1,strtemp);	
						DispTextLine(3,strtemp1,1);
						//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
					}
					else
					{
						DispTextLine(3,strtemp,0);
					}

					//第四行
					cMenuTemp1++;
			//		if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					if( cMenuTemp1	== (BusinessNo))
						 cMenuTemp1 = 0;
					strcpy(strtemp,&Menu_Business[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_BusinessID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					//配置用户菜单可见;by gzz 20121204
					//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
					if(SYSPara1.VisibleStat == 1)
					{
						KeyTab_TempVisible = Menu_BusinessID[cMenuTemp1];
						if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
						{
							strcpy(strtemp1,"×");	
						}
						else
						{
							strcpy(strtemp1,"√");	
						}
						strcat(strtemp1,strtemp);	
						DispTextLine(4,strtemp1,1);
						//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
					}
					else
					{
						DispTextLine(4,strtemp,0);
					}
					CursorLine = LINE0;					
				}
			 	break;
			}
		}
	}

}


void DispMenu_Transactions(uint8_t udkey)
{
	uint8_t KeyTab_TempVisible;
	char strtemp[60];
	
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_TransactionsID[cMenuTemp1])  
		{
		
			ClearScreen();
			Line_Reverse(0)	;
			//用户模式菜单;by gzz 20121130
			if(SYSPara1.AdminStat != 1)
			{
				//第一行
				DispTextLine(1,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0],1);
				//	cMenuTemp1++;
				//if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				//	 cMenuTemp1 = 0;
				//第二行
				do
				{
					cMenuTemp1++;
					KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];
				}
				while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
				if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
				{
					cMenuTemp1 = 0;
					KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];
					}	
				}	
				DispTextLine(2,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0],0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					//cMenuTemp1++;
					//if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					//	 cMenuTemp1 = 0;
					//第三行
					do
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];
					}
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
					if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
					{
						cMenuTemp1 = 0;
						KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];
						while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
						{
							cMenuTemp1++;
							KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];
						}	
					}	
					DispTextLine(3,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0],0);
					
					//cMenuTemp1++;
					//if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					//	 cMenuTemp1 = 0;
					//第四行
					do
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];
					}
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
					if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
					{
						cMenuTemp1 = 0;
						KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];
						while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
						{
							cMenuTemp1++;
							KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];
						}	
					}	
					DispTextLine(4,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0],0);
				} 
				CursorLine = LINE0;
				break;
			}
			else
			{
				//第一行
				//配置用户菜单可见;by gzz 20121204
				//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
				if(SYSPara1.VisibleStat == 1)
				{
					KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];
					if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
					{
						strcpy(strtemp,"×");	
					}
					else
					{
						strcpy(strtemp,"√");	
					}
					strcat(strtemp,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0]);	
					DispTextLine(1,strtemp,1);
					//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
				}
				else
				{
					DispTextLine(1,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0],1);
				}
				
				//第二行
					cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				//配置用户菜单可见;by gzz 20121204
				//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
				if(SYSPara1.VisibleStat == 1)
				{
					KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];					
					if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
					{
						strcpy(strtemp,"×");	
					}
					else
					{
						strcpy(strtemp,"√");	
					}					
					strcat(strtemp,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0]);	
					DispTextLine(2,strtemp,1);
					//DispTextLine(2,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
				}
				else
				{
					DispTextLine(2,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0],0);
				}
				
				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					//第三行
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					//配置用户菜单可见;by gzz 20121204
					//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
					if(SYSPara1.VisibleStat == 1)
					{
						KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];					
						if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
						{
							strcpy(strtemp,"×");	
						}
						else
						{
							strcpy(strtemp,"√");	
						}					
						strcat(strtemp,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0]);	
						DispTextLine(3,strtemp,1);
						//DispTextLine(2,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
					}
					else
					{
						DispTextLine(3,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0],0);
					}

					//第四行
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					//配置用户菜单可见;by gzz 20121204
					//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
					if(SYSPara1.VisibleStat == 1)
					{
						KeyTab_TempVisible = Menu_TransactionsID[cMenuTemp1];					
						if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
						{
							strcpy(strtemp,"×");	
						}
						else
						{
							strcpy(strtemp,"√");	
						}					
						strcat(strtemp,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0]);	
						DispTextLine(4,strtemp,1);
						//DispTextLine(2,&Menu_Root[SYSPara.Language][cMenuTemp1][0],0);
					}
					else
					{
						DispTextLine(4,&Menu_Transactions[SYSPara.Language][cMenuTemp1][0],0);
					}
				} 
				CursorLine = LINE0;
				break;
			}
		}
	 }
}



void DispMenu_Payments(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_PaymentsID[cMenuTemp1])   
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				strcpy(strtemp,&Menu_Payments[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
			
				InputSite = 0x80+strlen(Menu_Payments[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				strcpy(strtemp,&Menu_Payments[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_PaymentsID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					strcpy(strtemp,&Menu_Payments[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_PaymentsID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					strcpy(strtemp,&Menu_Payments[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_PaymentsID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
			 	break;
		}
	}
}
void DispMenu_PaymentsJidu(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_PaymentsIDJidu[cMenuTemp1])   
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				strcpy(strtemp,&Menu_Payments[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
			
				InputSite = 0x80+strlen(Menu_Payments[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				strcpy(strtemp,&Menu_Payments[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_PaymentsIDJidu[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					strcpy(strtemp,&Menu_Payments[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_PaymentsIDJidu[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					strcpy(strtemp,&Menu_Payments[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_PaymentsIDJidu[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
			 	break;
		}
	}
}

void DispMenu_Goods(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_GoodsID[cMenuTemp1]) 	
		{
		
				ClearScreen();
				Line_Reverse(0)	;
				strcpy(strtemp,&Menu_Goods[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
			
				InputSite = 0x80+strlen(Menu_Goods[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				strcpy(strtemp,&Menu_Goods[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_GoodsID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					strcpy(strtemp,&Menu_Goods[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_GoodsID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					strcpy(strtemp,&Menu_Goods[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_GoodsID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
			 	break;
		}
	}

}

//用于季度结账用;by gzz 20121009
void DispMenu_GoodsJidu(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_GoodsIDJidu[cMenuTemp1]) 	
		{
		
				ClearScreen();
				Line_Reverse(0)	;
				strcpy(strtemp,&Menu_GoodsJidu[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
			
				WriteFull_LCD(LINE1,"",0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					WriteFull_LCD(LINE2,"",0);
					WriteFull_LCD(LINE3,"",0);
				}
				CursorLine = LINE0;
			 	break;
		}
	}

}


void DispMenu_ChannelTrans(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_TransChannelID[cMenuTemp1]) 	
		{
		
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_TransChannel[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
				
				InputSite = 0x80+strlen(Menu_TransChannel[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_TransChannel[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_TransChannelID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
				 if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_TransChannel[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_TransChannelID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_TransChannel[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_TransChannelID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
			 	break;
		}
	}
}

//用于季度结账用;by gzz 20121009
void DispMenu_ChannelTransJidu(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_TransChannelIDJIDU[cMenuTemp1]) 	
		{
		
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_TransChannelJidu[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
				
				InputSite = 0x80+strlen(Menu_TransChannelJidu[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_TransChannelJidu[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_TransChannelIDJIDU[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_TransChannelJidu[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_TransChannelIDJIDU[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_TransChannelJidu[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_TransChannelIDJIDU[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
			 	break;
		}
	}
}


void DispMenu_TransInputChannelNo(uint8_t udkey)
	{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_TransInputChannelNoID[cMenuTemp1])    
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();
			if( cMenuTemp1	< (KeyTab[iMenuNextID].KeyTab_MaxItems ))
			{
			
				ClearScreen();
				//	Line_Reverse(0)	;
				DispTextLine(1,&Menu_TransInputChannelNo[SYSPara.Language][cMenuTemp1][0],0);
				//change by liya 2012-10-16
				DispTextLine(2,"                    ",0);
				InputSite = 0x80+strlen(Menu_TransInputChannelNo[SYSPara.Language][cMenuTemp1])/2;
				CursorLine = LINE0;
			}
			else
			{
				ClearScreen();
			//	Line_Reverse(1)	;
			
				DispTextLine(2,&Menu_TransInputChannelNo[SYSPara.Language][cMenuTemp1][0],1);
				CursorLine = LINE1;
			}
		}
	}
}

//用于季度结账用;by gzz 20121009
void DispMenu_TransInputChannelNoJidu(uint8_t udkey)
	{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_TransInputChannelNoIDJIDU[cMenuTemp1])    
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();
			if( cMenuTemp1	< (KeyTab[iMenuNextID].KeyTab_MaxItems ))
			{
			
				ClearScreen();
				//	Line_Reverse(0)	;
				DispTextLine(1,&Menu_TransInputChannelNo[SYSPara.Language][cMenuTemp1][0],0);
				//change by liya 2012-10-16
				DispTextLine(2,"                    ",0);
				InputSite = 0x80+strlen(Menu_TransInputChannelNo[SYSPara.Language][cMenuTemp1])/2;
				CursorLine = LINE0;
			}
			else
			{
				ClearScreen();
			//	Line_Reverse(1)	;
			
				DispTextLine(2,&Menu_TransInputChannelNo[SYSPara.Language][cMenuTemp1][0],1);
				CursorLine = LINE1;
			}
		}
	}
}

void DispMenu_CoinsTestConfirm(uint8_t udkey)
{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_CoinsTestConfirmID[cMenuTemp1])  
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();
			ClearScreen();
		
			DispTextLine(1,&Menu_CoinsTestConfirm[SYSPara.Language][cMenuTemp1][0],0);
	//		DispTextLine(2,&Menu_InputChannelNo[cMenuTemp1+1][0],1);
			Line_Reverse(0);
			InputSite = 0x80+strlen(Menu_CoinsTestConfirm[SYSPara.Language][cMenuTemp1])/2;
			
			cMenuTemp1++;
			if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				 cMenuTemp1 = 0;
			
			DispTextLine(2,&Menu_CoinsTestConfirm[SYSPara.Language][cMenuTemp1][0],1);			 //写第二行
			InputSite = 0x90+strlen(Menu_CoinsTestConfirm[SYSPara.Language][cMenuTemp1])/2;
			CursorLine = LINE0;			
			break;
		}
	}
}

void DispMenu_NoteTestConfirm(uint8_t udkey)
{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_NoteTestConfirmID[cMenuTemp1])   
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();
			ClearScreen();
		
			DispTextLine(1,&Menu_NoteTestConfirm[SYSPara.Language][cMenuTemp1][0],0);
	//		DispTextLine(2,&Menu_InputChannelNo[cMenuTemp1+1][0],1);
			Line_Reverse(0);
			InputSite = 0x80+strlen(Menu_NoteTestConfirm[SYSPara.Language][cMenuTemp1])/2;
			
			cMenuTemp1++;
			if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				 cMenuTemp1 = 0;
			
			DispTextLine(2,&Menu_NoteTestConfirm[SYSPara.Language][cMenuTemp1][0],1);			 //写第二行
			InputSite = 0x90+strlen(Menu_NoteTestConfirm[SYSPara.Language][cMenuTemp1])/2;
			CursorLine = LINE0;			
			break;
		}
	}
}

void DispMenu_Confirmation(uint8_t udkey)
{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_ConfirmationID[cMenuTemp1])   
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();
			ClearScreen();
		
			DispTextLine(1,&Menu_Confirmation[SYSPara.Language][cMenuTemp1][0],0);
	//		DispTextLine(2,&Menu_InputChannelNo[cMenuTemp1+1][0],1);
			Line_Reverse(0);
			InputSite = 0x80+strlen(Menu_Confirmation[SYSPara.Language][cMenuTemp1])/2;
			
			cMenuTemp1++;
			if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				 cMenuTemp1 = 0;
			
			DispTextLine(2,&Menu_Confirmation[SYSPara.Language][cMenuTemp1][0],1);			 //写第二行
			InputSite = 0x90+strlen(Menu_Confirmation[SYSPara.Language][cMenuTemp1])/2;
			CursorLine = LINE0;			
			break;
		}
	}
}

//用于季度结账用;by gzz 20121009
void DispMenu_ConfirmationJidu(uint8_t udkey)
{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_ConfirmationIDJidu[cMenuTemp1])   
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();
			ClearScreen();
		
			DispTextLine(1,&Menu_Confirmation[SYSPara.Language][cMenuTemp1][0],0);
	//		DispTextLine(2,&Menu_InputChannelNo[cMenuTemp1+1][0],1);
			Line_Reverse(0);
			InputSite = 0x80+strlen(Menu_Confirmation[SYSPara.Language][cMenuTemp1])/2;
			
			cMenuTemp1++;
			if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				 cMenuTemp1 = 0;
			
			DispTextLine(2,&Menu_Confirmation[SYSPara.Language][cMenuTemp1][0],1);			 //写第二行
			InputSite = 0x90+strlen(Menu_Confirmation[SYSPara.Language][cMenuTemp1])/2;
			CursorLine = LINE0;			
			break;
		}
	}
}

void DispMenu_System(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];
	uint8_t KeyTab_TempVisible;

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_SystemID[cMenuTemp1]) 
		{		
			ClearScreen();
			Line_Reverse(0)	;

			//用户模式菜单;by gzz 20121130
			if(SYSPara1.AdminStat != 1)
			{
				//第一行
				strcpy(strtemp,&Menu_System[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
				
				InputSite = 0x80+strlen(Menu_System[SYSPara.Language][cMenuTemp1])/2;
				//cMenuTemp1++;
				//if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				//	 cMenuTemp1 = 0;
				//第二行
				do
				{
					cMenuTemp1++;
					KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
				}
				while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
				if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
				{
					cMenuTemp1 = 0;
					KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
					}	
				}
				
				strcpy(strtemp,&Menu_System[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_SystemID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					//cMenuTemp1++;
					//if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					//	 cMenuTemp1 = 0;
					//第三行
					do
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
					}
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
					if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
					{
						cMenuTemp1 = 0;
						KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
						while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
						{
							cMenuTemp1++;
							KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
						}	
					}
					
					strcpy(strtemp,&Menu_System[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_SystemID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);
					
					//cMenuTemp1++;
					//if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					//	 cMenuTemp1 = 0;
					//第四行
					do
					{
						cMenuTemp1++;
						KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
					}
					while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) );
					if( cMenuTemp1	>= (KeyTab[iMenuNextID].KeyTab_MaxItems))
					{
						cMenuTemp1 = 0;
						KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
						while( (KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)&&(KeyTab[iMenuNextID].KeyTab_PressDown!=999)&&(iMenuNextID!=999)&&(cMenuTemp1 < (KeyTab[iMenuNextID].KeyTab_MaxItems)) )
						{
							cMenuTemp1++;
							KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
						}	
					}
					
					strcpy(strtemp,&Menu_System[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_SystemID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
			 	break;
			}
			else
			{
				strcpy(strtemp,&Menu_System[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				//第一行
				//配置用户菜单可见;by gzz 20121204
				//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
				if(SYSPara1.VisibleStat == 1)
				{
					KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
					if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
					{
						strcpy(strtemp1,"×");	
					}
					else
					{
						strcpy(strtemp1,"√");	
					}
					strcat(strtemp1,strtemp);	
					DispTextLine(1,strtemp1,1);
					//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
				}
				else
				{
					DispTextLine(1,strtemp,1);
				}
				
				InputSite = 0x80+strlen(Menu_System[SYSPara.Language][cMenuTemp1])/2;

				//第二行
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_System[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_SystemID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				//配置用户菜单可见;by gzz 20121204
				//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
				if(SYSPara1.VisibleStat == 1)
				{
					KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
					if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
					{
						strcpy(strtemp1,"×");	
					}
					else
					{
						strcpy(strtemp1,"√");	
					}
					strcat(strtemp1,strtemp);	
					DispTextLine(2,strtemp1,1);
					//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
				}
				else
				{
					DispTextLine(2,strtemp,0);
				}

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					//第三行
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_System[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_SystemID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					//配置用户菜单可见;by gzz 20121204
					//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
					if(SYSPara1.VisibleStat == 1)
					{
						KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
						if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
						{
							strcpy(strtemp1,"×");	
						}
						else
						{
							strcpy(strtemp1,"√");	
						}
						strcat(strtemp1,strtemp);	
						DispTextLine(3,strtemp1,1);
						//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
					}
					else
					{
						DispTextLine(3,strtemp,0);
					}

					//第四行
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_System[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_SystemID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					//配置用户菜单可见;by gzz 20121204
					//if( (SYSPara1.VisibleStat == 1)&&(SYSPara.LCDSel== 2) )
					if(SYSPara1.VisibleStat == 1)
					{
						KeyTab_TempVisible = Menu_SystemID[cMenuTemp1];
						if(KeyTab[KeyTab_TempVisible].KeyTab_Visible == MENU_VISIBLE_FAUSE)
						{
							strcpy(strtemp1,"×");	
						}
						else
						{
							strcpy(strtemp1,"√");	
						}
						strcat(strtemp1,strtemp);	
						DispTextLine(4,strtemp1,1);
						//DispTextLine(1,&Menu_Root[SYSPara.Language][cMenuTemp1][0],1);
					}
					else
					{
						DispTextLine(4,strtemp,0);
					}
				}
				CursorLine = LINE0;
			 	break;
			}
		}
	}
}

void DispMenu_TimeSet(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];
	RTCn_GetFullTime ((RTC_TIME_Type *)&TranData.Time);
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_TimeSetID[cMenuTemp1]) 	
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_TimeSet[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
				
				InputSite = 0x80+strlen(Menu_TimeSet[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_TimeSet[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_TimeSetID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_TimeSet[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_TimeSetID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);

					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_TimeSet[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_TimeSetID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
			 	break;
		}
	}
}

//压缩机设置
void DispMenu_TempSet(uint8_t udkey)//添加压缩机控制;by gzz 20121224
{
	char strtemp[24];
	char strtemp1[16];	
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_TempSetID[cMenuTemp1]) 	
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_TempSet[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,0);
				
				InputSite = 0x80+strlen(Menu_TempSet[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_TempSet[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_TempSetID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_TempSet[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_TempSetID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);

					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_TempSet[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_TempSetID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
			 	break;
		}
	}
}

//展示灯设置
void DispMenu_LEDSet(uint8_t udkey)//添加展示灯控制;by gzz 20121229
{
	char strtemp[24];
	char strtemp1[16];	
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_LEDSetID[cMenuTemp1]) 	
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_LEDSet[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,0);
				
				InputSite = 0x80+strlen(Menu_LEDSet[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_LEDSet[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_LEDSetID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_LEDSet[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_LEDSetID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);

					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_LEDSet[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_LEDSetID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
			 	break;
		}
	}
}



void DispMenu_SetChannel(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_SetChannelID[cMenuTemp1]) 	
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_SetChannel[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
				
				InputSite = 0x80+strlen(Menu_SetChannel[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_SetChannel[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_SetChannelID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
		 		if(IsLargeLCD())
				{
					
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_SetChannel[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_SetChannelID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);

					
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_SetChannel[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_SetChannelID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
				break;
		}
	}
}


void DispMenu_NoteValue(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_NoteValueID[cMenuTemp1]) 	
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_NoteValue[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
				
				InputSite = 0x80+strlen(Menu_NoteValue[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_NoteValue[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_NoteValueID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_NoteValue[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_NoteValueID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);

					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_NoteValue[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_NoteValueID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
				break;
		}
	}
}
void DispMenu_ChangeValue(uint8_t udkey)
{
char strtemp[48];
char strtemp1[48];

for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
{
	
		if(iMenuNextID==Menu_ChangeValueID[cMenuTemp1]) 	
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_ChangeValue[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
				
				InputSite = 0x80+strlen(Menu_ChangeValue[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_ChangeValue[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_ChangeValueID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_ChangeValue[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_ChangeValueID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);

					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_ChangeValue[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_ChangeValueID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
				break;
		}
	}
}
void DispMenu_CoinValue(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_CoinValueID[cMenuTemp1]) 	
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_CoinValue[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
				
				InputSite = 0x80+strlen(Menu_CoinValue[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_CoinValue[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_CoinValueID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_CoinValue[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_CoinValueID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);

					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_CoinValue[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_CoinValueID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
				break;
		}
	}
}
void DispMenu_SetConfirmation(uint8_t udkey)
{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_SetConfirmationID[cMenuTemp1])   
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();
			ClearScreen();
		
			DispTextLine(1,&Menu_SetConfirmation[SYSPara.Language][cMenuTemp1][0],0);
	//		DispTextLine(2,&Menu_InputChannelNo[cMenuTemp1+1][0],1);
			Line_Reverse(0);
			InputSite = 0x80+strlen(Menu_SetConfirmation[SYSPara.Language][cMenuTemp1])/2;
			
			cMenuTemp1++;
			if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				 cMenuTemp1 = 0;
			
			DispTextLine(2,&Menu_SetConfirmation[SYSPara.Language][cMenuTemp1][0],1);			 //写第二行
			InputSite = 0x90+strlen(Menu_SetConfirmation[SYSPara.Language][cMenuTemp1])/2;
			CursorLine = LINE0;			
			break;
		}
	}
}

//用户模式菜单;by gzz 20121130
void DispMenu_SetAdmin(uint8_t udkey)
{
	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_SetAdminID[cMenuTemp1])   
		{
			KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
			(*KeyInitPtr)();
			ClearScreen();
		
			DispTextLine(1,&Menu_SetConfirmation[SYSPara.Language][cMenuTemp1][0],0);
	//		DispTextLine(2,&Menu_InputChannelNo[cMenuTemp1+1][0],1);
			Line_Reverse(0);
			InputSite = 0x80+strlen(Menu_SetConfirmation[SYSPara.Language][cMenuTemp1])/2;
			
			cMenuTemp1++;
			if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
				 cMenuTemp1 = 0;
			
			DispTextLine(2,&Menu_SetConfirmation[SYSPara.Language][cMenuTemp1][0],1);			 //写第二行
			InputSite = 0x90+strlen(Menu_SetConfirmation[SYSPara.Language][cMenuTemp1])/2;
			CursorLine = LINE0;			
			break;
		}
	}
}

void DispMenu_SubCHStat(uint8_t udkey)
{


	
	if (udkey == KEY_ENTER)	
	{
		  AllChannelStat(InLayerNO);
	}
	if (udkey ==KEY_DOWN)
	{
			if (InLayerNO < (MAXLAYER - 1) )
			{
					if(	ChnMapPara.LAYTODRV[InLayerNO +1] != 0) 
					{
						InLayerNO ++;
						AllChannelStat(InLayerNO);
					}
			}
		
	}
	if (udkey ==KEY_UP)
	{
			if (InLayerNO >0 )
			{
				InLayerNO --;
				
				AllChannelStat(InLayerNO);
		   	}
			
	}

}
void DispMenu_SubCHResult(uint8_t udkey)
{

	char	strtemp1[48];
	uint8_t	m,n;

	//Trace("\r\ninput display testresult");	
	strtemp1[0] = 0x00;

	NowLayerNo = ChnMapPara.LAYTODRV[InLayerNO];
	if (udkey ==KEY_DOWN)
	{
			
		if((InCHNO == (MAXCHANNEL-1)))	
		{
		   	
		   	if((InLayerNO) == (MAXLAYER -1)||(ChnMapPara.LAYTODRV[(InLayerNO +1)] == 0))	
			{
		   		//  NowLayerNo = 0;
				  return;						 //最大货道
			}
			InCHNO = 0;
			(InLayerNO++);
		}
		else
			InCHNO++;

	}
	if (udkey ==KEY_UP)
	{
		if(InCHNO == 0)
		{
			if( InLayerNO == 0)
			{
				return;		//顶部
			}
			else
			{
				InLayerNO--;			  //换层
				InCHNO = MAXCHANNEL-1;
			}
		}
		else
		{
			InCHNO--;
		}
	}
		   	
			
			
	while(1)
	{
		
		NowLayerNo = ChnMapPara.LAYTODRV[InLayerNO];
		if (NowLayerNo == 0)
			break;
		NowLayerNo--;
		NowCHNO = ChnMapPara.CHTODRV[NowLayerNo][InCHNO];
		if (NowCHNO == 0)
			break;
		NowCHNO--;
		if((Channels[NowLayerNo][NowCHNO].TestNum  ) > 0)	
		{	
			if((InLayerNO+1)>8)
				break;
			else
			{
				ClearScreen();
				Trace("\r\n%d,%derrone[%d]", InLayerNO+1, InCHNO+1,Channels[NowLayerNo][NowCHNO].TestNum);
				//change by liya 2012-10-16
				if(SYSPara.Language==3)//土耳其语;by gzz 20121115
					sprintf( strtemp1,"%d%dKoridorun ba\xf1\x5far\xf1\x31s\xf1\x31zl\xf1\x31klar\xf1\x31n\xf1\x31n say\xf1\x31s\xf1\x31%d", InLayerNO+1, InCHNO+1,Channels[NowLayerNo][NowCHNO].TestNum);
				else if(SYSPara.Language==2)
					sprintf( strtemp1,"%d%dКоличество отказов%d", InLayerNO+1, InCHNO+1,Channels[NowLayerNo][NowCHNO].TestNum);
				else
					sprintf( strtemp1,"%d%d货道故障次数%d", InLayerNO+1, InCHNO+1,Channels[NowLayerNo][NowCHNO].TestNum);
				Write_LCD(LINE0,strtemp1,strlen(strtemp1));
				break;
			}		 	
		}
		(InCHNO++);
		if((InCHNO == MAXCHANNEL) || (ChnMapPara.CHTODRV[NowLayerNo][InCHNO] == 0))
		{
			InCHNO = 0;
			InLayerNO++;
			if(( InLayerNO == MAXLAYER) )
			{
				break;
			}
		}
	
	}
		
		
	m=InLayerNO ;
	n= InCHNO;	
		
	Trace("\r\nStart m,n==============");	
	while(1)
	{	
		
		(n++);
		Trace("\r\nOnem=%d,n=%d", m, n);	
		if((n >= MAXCHANNEL) || (ChnMapPara.CHTODRV[NowLayerNo][n] == 0))
		{
			n = 0;
			m++;
			if(( m >= MAXLAYER) )
			{
				Trace("\r\nNow break;");
				break;
			}
		}
		Trace("\r\nTwom=%d,n=%d", m, n);
		NowLayerNo = ChnMapPara.LAYTODRV[m];
		if (NowLayerNo == 0)
			break;
		NowLayerNo--;
		NowCHNO = ChnMapPara.CHTODRV[NowLayerNo][n];
		if (NowCHNO == 0)
			break;
		NowCHNO--;
		
		if((Channels[NowLayerNo][NowCHNO].TestNum  ) > 0)	
		{	
			if((m+1)>8)
				break;
			else
			{
				Trace("\r\n%d,%derrtwo[%d]", m+1, n+1,Channels[NowLayerNo][NowCHNO].TestNum);
				//change by liya 2012-10-16
				if(SYSPara.Language==3)//土耳其语;by gzz 20121115
					sprintf( strtemp1,"%d%dKoridorun ba\xf1\x5far\xf1\x31s\xf1\x31zl\xf1\x31klar\xf1\x31n\xf1\x31n say\xf1\x31s\xf1\x31%d", m+1, n+1,Channels[NowLayerNo][NowCHNO].TestNum);
				else if(SYSPara.Language==2)
					sprintf( strtemp1,"%d%dКоличество отказов%d", m+1, n+1,Channels[NowLayerNo][NowCHNO].TestNum);
				else
					sprintf( strtemp1,"%d%d货道故障次数%d", m+1, n+1,Channels[NowLayerNo][NowCHNO].TestNum);
				Write_LCD(LINE1,strtemp1,strlen(strtemp1));
				break;
			}			
		}
	}

}

void DispMenu_SubNOTE(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_SetNoteID[cMenuTemp1]) 	
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_SetNote[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
				
				InputSite = 0x80+strlen(Menu_SetNote[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_SetNote[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_SetNoteID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_SetNote[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_SetNoteID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_SetNote[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_SetNoteID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
				break;
		}
	}
}


//读卡器菜单;by gzz 20121120
void DispMenu_SubReader(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_SetReaderID[cMenuTemp1]) 	
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_SetReader[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
				
				InputSite = 0x80+strlen(Menu_SetReader[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_SetReader[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_SetReaderID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);

				//添加品途屏幕的兼容;by gzz 20121025
				if(IsLargeLCD())
				{
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_SetReader[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_SetReaderID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(3,strtemp,0);
					cMenuTemp1++;
					if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
						 cMenuTemp1 = 0;
					
					strcpy(strtemp,&Menu_SetReader[SYSPara.Language][cMenuTemp1][0]);
					KeyInitPtr=KeyTab[Menu_SetReaderID[cMenuTemp1]].InitKeyTab;
					strcpy(strtemp1,(*KeyInitPtr)());
					strcat(strtemp, strtemp1);
					DispTextLine(4,strtemp,0);
				}
				CursorLine = LINE0;
				break;
		}
	}
}


void DispMenu_HoldNote(uint8_t udkey)
{
	char strtemp[48];
	char strtemp1[48];

	for(cMenuTemp1=0;cMenuTemp1<KeyTab[iMenuNextID].KeyTab_MaxItems;cMenuTemp1++)
	{
	
		if(iMenuNextID==Menu_HoldNoteID[cMenuTemp1]) 	
		{
			
				ClearScreen();
				Line_Reverse(0)	;
				
				strcpy(strtemp,&Menu_HoldNote[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[iMenuNextID].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(1,strtemp,1);
				
				InputSite = 0x80+strlen(Menu_HoldNote[SYSPara.Language][cMenuTemp1])/2;
				cMenuTemp1++;
				if( cMenuTemp1	== (KeyTab[iMenuNextID].KeyTab_MaxItems))
					 cMenuTemp1 = 0;
				
				strcpy(strtemp,&Menu_HoldNote[SYSPara.Language][cMenuTemp1][0]);
				KeyInitPtr=KeyTab[Menu_HoldNoteID[cMenuTemp1]].InitKeyTab;
				strcpy(strtemp1,(*KeyInitPtr)());
				strcat(strtemp, strtemp1);
				DispTextLine(2,strtemp,0);
				CursorLine = LINE0;
				break;
		}
	}
}


void JudgeHLight(uint8_t udkey)			//刷新界面
{
	switch(iMenuNextID)
	{
		case	MENU_PASSWORD:			
		case	MENU_PASSWORD_2://120724 by cq RecordPassword /**/
		case	MENU_PASSWORD_3://用于季度结账用;by gzz 20121009
		case	MENU_PASSWORD_ADMIN://用户模式菜单;by gzz 20121130
				DispMenu_Password(udkey);
				break;
			
		case	MENU_ENGLISH:
		case	MENU_CHINESE:
		case	MENU_RUSSIAN:	//俄罗斯文;by gzz 20121012		
				DispMenu_Language(udkey);
				break;
									//************************************************************************
									//* First Bmp--------------------->(MenuID=0)                            *
		case	MENU_LANGUAGE:		//*  ---+                                                                *
		case  	MENU_CHANNEL	:	//*     |货道参数----------------->(MenuID=1)                            *
				DispMenu_Root(udkey);
				break;		
									//*     +---+                                                            *
		case  	MENU_INPUTNUM	:   //*     |   |请输入编号----------->(MenuID=2)                            *
		   		DispMenu_InputNum(udkey);
				break;		
		case  MENU_CHANNELNO	:	//*     |   |货道号--------------->(MenuID=3)                            *
		case  MENU_PRICE		:	//*     |   |单价----------------->(MenuID=3)                            *
		case  MENU_CHANNELLEFT	:   //*     |   |货道数量------------->(MenuID=4)                            *	
		case  MENU_CHANNELSTAT	:	//*     |   |货道状态------------->(MenuID=4)                            *	
		case  MENU_CHANNELGOODS	:	//*     |   |货道状态------------->(MenuID=4)                            *
			DispMenu_Channel(udkey);
			break;	

		case  MENU_ADDGOODS		:	//*		|添货--------------------->(MenuID=5)                            *
		case  MENU_DRIVER		:	//*     |设备管理----------------->(MenuID=5)                            *
			DispMenu_Root(udkey);
			break;						
		case  MENU_INPUTADDGOODS:	
			DispMenu_AddGoods(udkey);	//*		|你确定要添货吗
			break;				
		case  MENU_ADDINPUTLAY:
			DispMenu_AddInputChannelNo(udkey);
			break;		
		case  MENU_NOTESTAT		:	//*     |   |纸币器状态----------->(MenuID=6)                            *
		case  MENU_COINSSTAT	:	//*     |   |硬币器状态----------->(MenuID=7)                            *
		case  MENU_HOPPERSTAT1	:	//*     |   |找零器状态----------->(MenuID=8)                            *
		case  MENU_HOPPERSTAT2	:	//*     |   |找零器状态----------->(MenuID=8)                            *
		case  MENU_HOPPERSTAT3	:	//*     |   |找零器状态----------->(MenuID=8)                            *
		case  MENU_KEYTEST		:	//*     |   |按钮检测------------->(MenuID=9)                            *
		case  MENU_CHANNELTEST	:	//*     |   |货道检测------------->(MenuID=10)                           *
		case  MENU_ALLCHANNELSTAT  :
		case  MENU_GPRSTEST		:	//*     |   |手机模块状态测试----->(MenuID=11)                           *
		case  MENU_SMSTEST		:	//*     |   |短消息检测----------->(MenuID=12)                           *
		case  MENU_CHTESTRESULT	:
		case  MENU_IRTEST		:
		case  MENU_LCDTEST		:
		case  MENU_TEMPRATURETEST:
		case  MENU_SELECTKEYTEST:	
		case  MENU_TEMPSTEST:		//添加压缩机控制;by gzz 20121224
		case  MENU_LEDTEST:			//添加展示灯控制;by gzz 20121229
		case  MENU_FLASHLEFT	:
			DispMenu_Driver(udkey);
			break;					
		case  MENU_DOKEYTEST	:	//*     |   +--------------                                              *
			DispMenu_KeyTest(udkey);
			break;	
		case  MENU_NOTETESTCONFIRM 	:
			DispMenu_NoteTestConfirm(udkey);	//*		|纸币器正常吗
			break;
		case  	MENU_COINSTESTCONFIRM	:
			DispMenu_CoinsTestConfirm(udkey);//*		|硬币器正常吗
			break;
		case  MENU_DRINPUTNUM		:	//*     |   |请输入编号----------->(MenuID=2)                            *
		   	DispMenu_DriverInputNum(udkey);
					break;		
		case  MENU_BUSINESS		:	//*     |系统参数----------------->(MenuID=13)                           *
			DispMenu_Root(udkey);
			break;					
									//*     +---+                                                            *
		case  MENU_COINVALUE	:	//*     |   |找零面值------------->(MenuID=15)                           *
		case  MENU_NOTEVALUE	:	//*     |   |找零面值------------->(MenuID=15)                           *
		case  MENU_CHANGEVALUE	:	//*     |   |找零面值------------->(MenuID=15)                           *
		case  MENU_NOLIMITEDBUY	:	//*     |   |收币上限------------->(MenuID=17)                           *
		case  MENU_RETURNCOINS	:	//*     |   |收币上限------------->(MenuID=17)                           *
		case  MENU_OPENCOIN	:		//*     |   |开启纸币器----------->(MenuID=17)                           *
		case  MENU_OPENNOTE	:		//*     |   |开启纸币器----------->(MenuID=17)                           *
		case  MENU_OPENCHANGE	:	//*     |   |开启纸币器----------->(MenuID=17)                           *
		case  MENU_OPENREADER	:	//*     |   |开启纸币器----------->(MenuID=17)                           *
		case  MENU_READERCOST	:	//*     |   |开启纸币器----------->(MenuID=17)                           *
		case  MENU_BALANCEMIN   :	//*     |   |余额上限------------->(MenuID=17)                           *
		case  MENU_MONEYMAXIN	:	//*     |   |收币上限------------->(MenuID=17)                           *
		case  MENU_DOUBTMONEY	:	//*     |   |疑问金额------------->(MenuID=21)                           *
		case  MENU_IOU			:	//*     |   |欠条：--------------->(MenuID=14)                           *
		case  MENU_CHANGESERVER	:	//*		|	|无零币是否服务：----->(MenuID=16)                           *
		case  MENU_DECPOINTNUM	:   //*		|	|小数点位数：--------->(MenuID=16)                           *
		case  MENU_SALECONFIRM	:   //*		|	|收获确认
		case  MENU_POSENABLE	:   //*		|	|开启POS机：------------->(MenuID=33)                   *
		case  MENU_VMCPC        :  	//*     |   |开启PC通讯----------->(MenuID=)                             *	
		case  MENU_LARGELCD     :  	//*     |   |屏幕类型 ----------->(MenuID=)                             *	
		case  MENU_SELECTKEY    :  	//*     |   |是否使用选货按键 ----------->(MenuID=)                             *	
		case  MENU_LOGOSEL      :  	//*     |   |是否使用logo----------->(MenuID=)                             *
		case  MENU_ADDCOLUMN    :  	//*     |   |是否自动补货----------->(MenuID=)                             *
		case  MENU_TENCOLUMN    :  	//*     |   |是否使用十货道的机器----------->(MenuID=)                             *
		case  MENU_HOLDNOTE		:   //*		|	|暂存     
			DispMenu_Business(udkey);
			break;					//*     |   +--------------   											 *
		case 	MENU_READEROFF		://*     |   +--------------   											 *
		case    MENU_READERMDB      ://*     |   +--------------   											 *
			DispMenu_SubReader(udkey);
			break;
		case  MENU_TRANSACTIONS	:	//*     |交易记录----------------->(MenuID=22)                           *
			DispMenu_Root(udkey);
			break;					//*     +---+                                                            *
		case  MENU_PAYMENTS		:	//*         |货币收支------------->(MenuID=23)                           *
			DispMenu_Transactions(udkey);
			break;					
									//*         +---+                                                        *
		case  MENU_INCOME		:	//*         |   |货币总收入------->(MenuID=24)                           *
		case  MENU_NOTEINCOME	:	//*         |   |纸币收入--------->(MenuID=25)                           *
		case  MENU_COINSINCOME	:	//*         |   |硬币收入--------->(MenuID=26)                           *
		case  MENU_TOTALTRANS	:	//*         |   |交易总金额------->(MenuID=27)                           *
		case  MENU_TOTALCHANGE  :	//*         |   |找零总金额------->(MenuID=28)                           *
		case  MENU_COINSCHANGE1  :	//*         |   |出币枚数--------->(MenuID=29)                           *
		case  MENU_COINSCHANGE2  :	//*         |   |出币枚数--------->(MenuID=29)                           *
		case  MENU_COINSCHANGE3  :	//*         |   |出币枚数--------->(MenuID=29)                           *
			DispMenu_Payments(udkey);
			break;	
		case  MENU_GOODS		:	//*         |商品----------------->(MenuID=30)                           *
			DispMenu_Transactions(udkey);
			break;					//*         +---+                                                        *
		
		case  MENU_SUCCESSNUM	:	//*         |   |成功交易次数----->(MenuID=31)                           *
		case  MENU_DOUBTNUM		:	//*         |   |疑问交易次数----->(MenuID=32)                           *
		case  MENU_CHANNELTRANS	:	//*         |   |单货道交易统计--->(MenuID=33)                           *
			DispMenu_Goods(udkey);
			break;				
		case  MENU_TRANSINPUTNUM:
			DispMenu_TransInputChannelNo(udkey);
			break;		
		case   MENU_TRANSCHANNELNO:	//*     	|   | 	|货道编号--->(MenuID=35)                   		     *
		case   MENU_TRANSSUCCESS:	//*         |   | 	|成功交易统计--->(MenuID=36)                         *
		case   MENU_TRANMONEY:		//*         |   | 	|商品交易金额--->(MenuID=36)                         *
		case   MENU_TRANSDOUBT	:	//*         |   | 	|疑问交易统计--->(MenuID=37)                         *
			DispMenu_ChannelTrans(udkey);
			break;										//*         |   	+--------------                                      *							//*         |   +--------------                                          *
		case  MENU_CLEARTRANS	:	//*         |清除记录------------->(MenuID=34)                           *
		
			DispMenu_Transactions(udkey);
			break;					
									//*         +---+                                                        *
		case  MENU_CONFIRMATION	 :
			DispMenu_Confirmation(udkey);
			break;	

		case  MENU_PAYMENTSJIDU		:	//*         |货币收支------------->(MenuID=23)                           *
			DispMenu_Transactions(udkey);
			break;					
			
									//*         +---+                                                        *
		case  MENU_INCOMEJIDU		:	//*         |   |货币总收入------->(MenuID=24)                           *
		case  MENU_NOTEINCOMEJIDU	:	//*         |   |纸币收入--------->(MenuID=25)                           *
		case  MENU_COINSINCOMEJIDU	:	//*         |   |硬币收入--------->(MenuID=26)                           *
		case  MENU_TOTALTRANSJIDU	:	//*         |   |交易总金额------->(MenuID=27)                           *
		case  MENU_TOTALCHANGEJIDU  :	//*         |   |找零总金额------->(MenuID=28)                           *
		case  MENU_COINSCHANGE1JIDU  :	//*         |   |出币枚数--------->(MenuID=29)                           *
		case  MENU_COINSCHANGE2JIDU  :	//*         |   |出币枚数--------->(MenuID=29)                           *
		case  MENU_COINSCHANGE3JIDU  :	//*         |   |出币枚数--------->(MenuID=29)                           *
			DispMenu_PaymentsJidu(udkey);
			break;	
			
		case  MENU_GOODSJIDU		:	//*         |商品----------------->(MenuID=30)                           *
			DispMenu_Transactions(udkey);
			break;					//*         +---+                                                        *
		
		case  MENU_CHANNELTRANSJIDU	:	//*         |   |单货道交易统计--->(MenuID=33)                           *
			DispMenu_GoodsJidu(udkey);
			break;				
		case  MENU_TRANSINPUTNUMJIDU:
			DispMenu_TransInputChannelNoJidu(udkey);
			break;		
		case   MENU_TRANSCHANNELNOJIDU:	//*     	|   | 	|货道编号--->(MenuID=35)                   		     *
		case   MENU_TRANSSUCCESSJIDU:	//*         |   | 	|成功交易统计--->(MenuID=36)                         *
		case   MENU_TRANMONEYJIDU:		//*         |   | 	|商品交易金额--->(MenuID=36)                         *		
			DispMenu_ChannelTransJidu(udkey);
			break;										//*         |   	+--------------                                      *							//*         |   +--------------                                          *
		
		case  MENU_CLEARTRANSJIDU	:	//*         |清除记录------------->(MenuID=34)                           *
		
			DispMenu_Transactions(udkey);
			break;					
									//*         +---+                                                        *
		case  MENU_CONFIRMATIONJIDU	 :
			DispMenu_ConfirmationJidu(udkey);
			break;	
			
		case  	MENU_SYSTEM     :	//*		系统参数
			DispMenu_Root(udkey);
			break;					//*     +---+           
		case  	MENU_TIMESET     :	//*		时间设置
			DispMenu_System(udkey);
			break;					//*     +---+           
		case  	MENU_SETYEAR	:
		case  	MENU_SETMONTH	:
		case  	MENU_SETDATE	:
		case  	MENU_SETHOUR	:
		case  	MENU_SETMINUTE	:
			DispMenu_TimeSet(udkey);
			break;					//*     +---+        
		case  MENU_CGPASSWORD	:	//*     |   |开启手机模块：------->(MenuID=18)                           *
		case  MENU_GPRSONOFF	:	//*     |   |开启手机模块：------->(MenuID=18)                           *
		case  MENU_SERIALNO		:	//*     |   |机器编号------------->(MenuID=19)                           *
		case  MENU_GSMNO		:	//*		|	|GSM NO：------------->(MenuID=20)                           *
		case  MENU_TEMPERATURE	:	//*		|	|温度设定：------------->(MenuID=20)                           *
		case  MENU_SYSLANGUAGE	:	//*		|	|语言控制：------------->(MenuID=20)                           *
		case  MENU_TEMPSET		:	//*		|	|温控器控制：------------->(MenuID=20)                           *
		case  MENU_LEDSET		:	//*		|	|展示灯控制：------------->(MenuID=20)                           *
			DispMenu_System(udkey);
			break;					//*     +---+ 
		case	MENU_SYSENGLISH:
		case	MENU_SYSCHINESE:
		case	MENU_SYSRUSSIAN:	//俄罗斯文;by gzz 20121012	
		case	MENU_SYSTURKEY :    //土耳其语;by gzz 20121115
				DispMenu_SysLanguage(udkey);
				break;	
		case  	MENU_TEMPON	        :	//*		|	|压缩机控制：------------->(MenuID=20)                           *//添加压缩机控制;by gzz 20121224
		case  	MENU_TEMPHOURSTR	:	//*		|	|温控器开始时：------------->(MenuID=20)                           *
		case  	MENU_TEMPMINUTESTR	:	//*		|	|温控器开始分：------------->(MenuID=20)                           *
		case  	MENU_TEMPHOUREND	:	//*		|	|温控器结束时：------------->(MenuID=20)                           *
		case  	MENU_TEMPMINUTEEND	:	//*		|	|温控器结束分：------------->(MenuID=20)                           *	
				DispMenu_TempSet(udkey);
				break;	
		case  	MENU_LEDON	        :	//*		|	|压缩机控制：------------->(MenuID=20)                           *//添加压缩机控制;by gzz 20121224
		case  	MENU_LEDHOURSTR	:	//*		|	|温控器开始时：------------->(MenuID=20)                           *
		case  	MENU_LEDMINUTESTR	:	//*		|	|温控器开始分：------------->(MenuID=20)                           *
		case  	MENU_LEDHOUREND	:	//*		|	|温控器结束时：------------->(MenuID=20)                           *
		case  	MENU_LEDMINUTEEND	:	//*		|	|温控器结束分：------------->(MenuID=20)                           *	
				DispMenu_LEDSet(udkey);
				break;			
			
		case  	MENU_SETCHANNEL :	//*		设置货道
			DispMenu_Root(udkey);
			break;					//*     +---+           
		case  MENU_INPUTLAYER :
			  DispMenu_InputLayerNum(udkey);
					break;	
		case  MENU_LAYERNO		:   //*     |   	|层架编号----------------->(MenuID=72)                   *
		case  MENU_LAYENABLE	:	//*     |   	|是否启用----------------->(MenuID=73)                   *
		case  MENU_MAXGOODS		:   //*     |   	|最大存货量--------------->(MenuID=74)                   *
		case  MENU_CHANNEL1		:   //*     |   	|第一货道----------------->(MenuID=75)                   *
		case  MENU_CHANNELNUM1	:	//*     |   	|第一货道存量----------------->(MenuID=75)                   *//按货道添存货量;by gzz 20121120
		case  MENU_CHANNEL2		:   //*     |   	|第二货道----------------->(MenuID=75)                   *
		case  MENU_CHANNELNUM2	:	//*     |   	|第二货道存量----------------->(MenuID=75)                   *
		case  MENU_CHANNEL3		:   //*     |   	|第三货道----------------->(MenuID=75)                   *
		case  MENU_CHANNELNUM3	:	//*     |   	|第三货道存量----------------->(MenuID=75)                   *
		case  MENU_CHANNEL4		:   //*     |   	|第四货道----------------->(MenuID=75)                   *
		case  MENU_CHANNELNUM4	:   //*     |   	|第四货道存量----------------->(MenuID=75)                   *
		case  MENU_CHANNEL5		:   //*     |   	|第五货道----------------->(MenuID=75)                   *
		case  MENU_CHANNELNUM5	:   //*     |   	|第五货道存量----------------->(MenuID=75)                   *
		case  MENU_CHANNEL6		:   //*     |   	|第六货道----------------->(MenuID=75)                   *
		case  MENU_CHANNELNUM6	:   //*     |   	|第六货道存量----------------->(MenuID=75)                   *
		case  MENU_CHANNEL7		:   //*     |   	|第七货道----------------->(MenuID=75)                   *
		case  MENU_CHANNELNUM7	:   //*     |   	|第七货道存量----------------->(MenuID=75)                   *
		case  MENU_CHANNEL8		:   //*     |   	|第八货道----------------->(MenuID=75)                   *
		case  MENU_CHANNELNUM8	:   //*     |   	|第八货道存量----------------->(MenuID=75)                   *
		//121105 by cq TenChannels
		case  MENU_CHANNEL9 	:	//* 	|		|第九货道----------------->(MenuID=75)					 *
		case  MENU_CHANNELNUM9	:   //*     |   	|第九货道存量----------------->(MenuID=75)                   *
		case  MENU_CHANNEL0 	:	//* 	|		|第零货道----------------->(MenuID=75)					 *
		case  MENU_CHANNELNUM0	:   //*     |   	|第零货道存量----------------->(MenuID=75)                   *
			DispMenu_SetChannel(udkey);
			break;					//*     +---+           
		case  MENU_NOTEVALUE1		:
		case  MENU_NOTEVALUE2		:
		case  MENU_NOTEVALUE3		:
		case  MENU_NOTEVALUE4		:
		case  MENU_NOTEVALUE5		:
		case  MENU_NOTEVALUE6		:
		case  MENU_NOTEVALUE7		:
		case  MENU_NOTEVALUE8		:
			 DispMenu_NoteValue(udkey);
			break;					//*     +---+           
		case  MENU_COINVALUE1		:
		case  MENU_COINVALUE2		:
		case  MENU_COINVALUE3		:
		case  MENU_COINVALUE4		:
		case  MENU_COINVALUE5		:
		case  MENU_COINVALUE6		:
		case  MENU_COINVALUE7		:
		case  MENU_COINVALUE8		:
			 DispMenu_CoinValue(udkey);
			break;					//*     +---+         
		case  MENU_CHANGEVALUE1		:
		case  MENU_CHANGEVALUE2		:
		case  MENU_CHANGEVALUE3		:
			DispMenu_ChangeValue(udkey);
			break;					//*     +---+    
		case  	MENU_DEFAULTSET :	//*		默认设置
			DispMenu_Root(udkey);
			break;					//*     +---+        
		case  MENU_CONFIRMSET	 :
			DispMenu_SetConfirmation(udkey);
			break;				
		//case  	MENU_SYNGOODSCOL :	//*		全系统同步
		//case  	MENU_ADDCOIN :	    //*		补硬币完成
		//case  	MENU_ADDBILL :	    //*		取纸币完成
		//	DispMenu_Root(udkey);
		//	break;					//*     +---+      	
		case  MENU_SUBALLCHANNELSTAT :
			DispMenu_SubCHStat(udkey);
			break;
		case  MENU_SUBCHTESTRESULT :
			DispMenu_SubCHResult(udkey);
			break;
		case 	MENU_SELECTGBA		:
		case 	MENU_SELECTMEI		:
		case    MENU_SELECTITL      :	
		case 	MENU_SELECTCLOSE	:			
			DispMenu_SubNOTE(udkey);
			break;	
		case 	MENU_HOLDNOTE5		:
		case 	MENU_HOLDNOTE10		:
		case 	MENU_HOLDNOTE20		:
			DispMenu_Business(udkey);
			//	DispMenu_HoldNote(udkey);
			break;	
		
		default:
				break;
	}

}









