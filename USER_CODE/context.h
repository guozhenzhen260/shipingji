/****************************************Copyright (c)****************************************************
**                            fuzhou powerorigin Co.,LTD.
**
**                                 http://www.powerorigin.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           context.h
** Last modified Date:  2011-07-22
** Last Version:         
** Descriptions:        交易显示菜单定义
**                      
**------------------------------------------------------------------------------------------------------
** Created by:           
** Created date:        2011-07-22
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
#ifndef __CONTEXT_H
#define __CONTEXT_H

#if DISP_MODE == LCD_DISPLAY



const char  *Disp_Init[4][1] =	 
{
   {
      "设备初始化中.."
   },
   {
      "Initializing.."
   },
   //change by liya 2012-10-16
   {
   	  "Инициализация..."
   },
   //土耳其语;by gzz 20121115
   {
   	  "Cihaz ba\xf1\x5flatma"
   }
};

const char  *Disp_Fault[4][20] =
{
   {
	  "抱歉，本机暂停服务",   
	  "故障码:	",
	  "商品已售完",
	  "找零器故障",
	  "纸币器故障",
	  "硬币器故障",
	  "找币器缺币",
	  "所有货道无货",
	  "所有货道故障",
	  "货道板故障",
	  "货道不可用",
	  "系统参数故障",
	  "LCD 故障",
	  "货道单价异常",
	  "出货确认故障",
	  "Flash故障",
	  "时钟故障",
	  "取货机构故障",
	  "货柜门没关",
	  "抱歉"
	  
   },
   {	
	  "Out of Service!",
	  "Err Code: ",
	  "Channel err",
	  "Hopper is wrong",
	  "bill err",
	  "coin err",
	  "hopper empty",
	  "column empty",
	  "column err",
	  "column err",
	  "column err",
	  "system err",
	  "LCD err",
	  "price err",
	  "GOC err",
	  "Flash err",
	  "RTC err",
	  "FGP err",
	  "Door Open",
	  "Sorry"
   },
   //change by liya 2012-10-16
   {
	  "Не работает",
	  "Код ошибки:",
	  "Ошибка канала",
	  "Хоппер ",
	  "Билл ",
	  "Монета",
	  "Отсутствие валюты",
	  "проданный",
	  "неисправность",
	  "неисправность",
	  "неисправность",
	  "параметры системы",
	  "LCD",
	  "Цена неправильно",
	  "GOC",
	  "Flash",
	  "RTC",
	  "FGP",
	  "двери контейнера",
	  "извините"
   },
   //土耳其语;by gzz 20121115
   {
	  "Servis d\xf1\x31\xf1\x5f\xf1\x31",   
	  "Hata kodu:  ",
	  "\xf0\xdcr\xf0\xfcn sat\xf1\x31ld\xf1\x31",
	  "De\xf1\x1fi\xf1\x5fikli\xf1\x1fi hatas\xf1\x31 tutun",
	  "Bill yetmezli\xf1\x1fi",
	  "Coin hatas\xf1\x31",
	  "Paralar eksik sikke bul",
	  "T\xf0\xfcm g\xf0\xF6nderiler hi\xf0\xe7bir mal road",
	  "T\xf0\xfcm Kargo Karayolu fay",
	  "Kargo Karayolu kart\xf1\x31 ar\xf1\x31zas\xf1\x31",
	  "Kargo Karayolu kullan\xf1\x31lam\xf1\x31yor",
	  "Sistem parametreleri ar\xf1\x31zas\xf1\x31",
	  "LCD ar\xf1\x31za",
	  "Fiyat anormal",
	  "GOC ar\xf1\x31za",
	  "Flash ar\xf1\x31za",
	  "RTC ar\xf1\x31za",
	  "FGP ar\xf1\x31za",
	  "Kap\xf1\x31 kapal\xf1\x31 de\xf1\x1fil Pick",
	  "\xf0\xfczg\xf0\xfcn\xf0\xfcm"
   }
};


const char  *Disp_Main[4][2] =	 
{
   {
      "        现金消费",
	  "  投币->选货->取货"
   },
   {
      "     Cash trade",
	  " Money->Sel->Goods"
   },
   //change by liya	2012-10-16
   {
   	  "торговля за наличные",
	  "Деньги->Продажа->Еда"
   },
   //土耳其语;by gzz 20121115
   {
   		"Nakitsiz al\xf1\x31m",
		"Para->\xf0\xfcr\xf0\xfcn sec ->Al"
   }
};	

//120914 by cq Cashless
const char  *Disp_Main_2[4][2] =	 
{
   {
      "      插卡消费",
	  "  插卡->选货->取货"
   },
   {
      "   Cashless trade",
	  "  Card->Sel->Goods"
  },
  //change by liya 2012-10-16
  {
	 "торговля за наличные",
	 "Деньги->Продажа->Еда"
  },
  //土耳其语;by gzz 20121115
  {
      "      Kart t\xf0\xfcketimi",
	  "kart->\xf0\xfcr\xf0\xfcn sec ->Al"
   } 
};	


const char  *Disp_Input[4][7] =
{
   {
      "        欢迎光临",
	  "  投币->选货->取货",
	  "  投币->选货->取货",
	  "  投币->选货->取货",
	  //"不找零，购买请投硬币",//121106 by cq BillSingleWork
	  "  投币->选货->取货",//121106 by cq BillSingleWork
	  //"		 硬币不足",
	  "  投币->选货->取货",//121106 by cq BillSingleWork
	  "输入商品编号确认购买"
   },
   {
      "      Welcome",
	  " Money->Sel->Goods",
	  " Money->Sel->Goods",
	  " Money->Sel->Goods",
	  " Money->Sel->Goods",
	  " Money->Sel->Goods",
	  "Input Column No."
   },
   //change by liya	2012-10-16
   {
   	  "Добро пожаловать",
	  "Вставте:купюру/монету",
	  "Вставте:купюру",
	  "Вставте:монету",
	  "Не давать изменения,Вставтемонету",
	  "Сдачи недостаточности",
	  "Введите номер продукта"
   },
   //土耳其语;by gzz 20121115
   {
      "        kar\xf1\x5f\xf1\x31lama",
	  "  Notlar veya paralar: giri\xf1\x5fL\xf0\xfctfen",
	  "    Kagit para gir",
	  "    Madeni para gir",
	  //"不找零，购买请投硬币",//121106 by cq BillSingleWork
	  "De\xf1\x1fi\xf1\x5fim gerekebilir",//121106 by cq BillSingleWork
	  //"		 硬币不足",
	  "S\xf1\x31f\xf1\x31r paralar ya da ba\xf1\x5far\xf1\x31s\xf1\x31zl\xf1\x31k i\xf0\xe7in de\xf1\x1fi\xf1\x5fiklik",//121106 by cq BillSingleWork
	  "\xf0\xdcr\xf0\xfcn Kodu sat\xf1\x31n onaylay\xf1\x31n Gir"
   }
};

const char  *Disp_ChInput[4][4] =
{
   {
      "输入商品编号：",
	  "输入错误",
	  "请输入其他商品编号",
	  "请选货或继续投币!"
   },
   {
      "Input Item code:",
	  "Invalid choice",
	  "Input other code",
	  "Make Selection!"
   },
   //change by liya 2012-10-16
   {
      "Введите код позиции:",
	  "Неправильный выбор",
	  "Введите другой код",
	  "Сделайте выбор!"	
   },
   //土耳其语;by gzz 20121115
   {
      "\xf0\xdcr\xf0\xfcn kodunu giriniz:",
	  "Giri\xf1\x5f hatalar\xf1\x31",
	  "Farkl\xf1\x31 bir \xf0\xfcr\xf0\xfcn numaras\xf1\x31 giriniz",
	  "Mal se\xf0\xe7mek veya jeton devam edin!"
   }
};		

const char  *Disp_ChnPara[4][2] =
{
   {
      "货道  ",
	  "单价："
   },
   {
      "Channel ",
	  "Price: "
   },
   //change by liya	2012-10-16
   {
   	  "Канал",
	  "Цена"
   },
   //土耳其语;by gzz 20121115
   {
      "S\xf0\xfctun  ",
	  "Fiyat:"
   }
};

const char  *Disp_ChnState[4][4] =
{
   {
      " -- 禁能",
	  " -- 有货",
	  " -- 故障",
	  " -- 缺货"
   },
   {
      "-Disable",
	  "-OK",
	  "-Error",
	  "-Out"
   },
   //change by liya	2012-10-16
   {
      "-Выключено",
	  "-OK",
	  "-Ошибка",
	  "-Выдача"
   },
   //土耳其语;by gzz 20121115
   {
      " -- Disable",
	  " -- OK",
	  " -- Hatal\xf1\x31",
	  " -- Bitti"
   }
};

//修改交易提示;by gzz 20121009
const char  *Disp_Deal[4][13] =
{
   {
	  "余额",
	  "金额不足，",
	  "还要购买商品吗？",
	  "商品已售完",
	  "请插入更小面额纸币",
	  "客户余额",
	  "    按确定键出货",
	  "    按返回键退币",	  
	  " ￥",
	  "金额",
	  "请按键选购",
	  "继续选购",
	  "找零退币"
   },
   {
	  "Balance:",
	  "Insufficient fund",
	  "Buy another one?",
	  "   Soldout",
	  "Insert a smaller one",
	  "Price paid:",
	  "Enter OutGood",
	  "Cancel Payout",	  
	  "  ＄",
	  "Amount:",
	  "Please Select by Button",
	  "Continue",
	  "Coin Refund"
   },
   //change by liya 2012-10-16
   {
	  "Баланс:",
	  "Найдено несоотв.?",
	  "Купить ещё?",
	  "Продано",
	  "Вставьте поменьше",
	  "Цена оплачена:",
	  "ENTER   судоходство",
	  "CANCEL  возвращать",	  
	  "  ",
	  "Баланс:",
	  "Пожалуйста, нажмите купить",
	  "продолжать",
	  "CASHOUT"	  
   },
   //土耳其语;by gzz 20121115
   {
	  "Bakiye: ",
	  "Yetersiz fon?",
	  "Ba\xf1\x5fka bier \xf1\x5fey almak inter misiniz?",
	  "   Urun yok",
	  "Kucuk banknot girin",
	  "Bakiye:",
	  "nakliye",
	  "\xf0\xF6deme alma",	  
	  "  ",
	  "Bakiye: ",
	  "\xf0\xdcr\xf0\xfcn kodunu giriniz",
	  "Yeni \xf0\xfcr\xf0\xfcn",
	  "Para iade"
   }
};





const char  *Disp_GoodsOut[4][2] =
{
   {
      "正在出货..",
	  "选购成功, 请取货"
   },
   {
      "Taking the goods",
	  "Please Take the Item"
   },
   //change by liya	 2012-10-16
   {
      "Захват еды",
	  "Заберите еду"
   },
   //土耳其语;by gzz 20121115
   {
      "\xf0\xdcr\xf0\xfcn veriliyor ,bekleyiniz",
	  "\xf0\xfcr\xf0\xfcn\xf0\xfc alabilirsiniz.."
   }
};

const char  *Disp_Change[4][7] =
{
   {
      "已退币",
	  "正在找零",
	  "    零钱不足",
	  "欠款: ",
	  "    正在找零..",
	  "请致电客服热线",
	  "正在退款"
   },
   {
      "Pls Get the Change",
	  "Change",
	  "Not enough coins",
	  "IOU: ",
	  "Changing",
	  "Change failure",
	  "Refund of fare"
   },
   //change by liya	2012-10-16
   {
      "Набор монет",
	  "Замена:",
	  "Недостаточно монет",
	  "я долен Вам: ",
	  "Замена",
	  "Замена не получилась",
	  "Переопределите пищу"
   },
   //土耳其语;by gzz 20121115
   {
      "\xf0\xfcr\xf0\xfcn\xf0\xfc de\xf1\x1fi\xf1\x5ftirebilirsiniz",
	  "De\xf1\x1fi\xf1\x5fim:",
	  "s\xf1\x31k\xf1\x31nt\xf1\x31s\xf1\x31 de\xf1\x1fi\xf1\x5ftirin",
	  "Bor\xf0\xe7 makbuzu: ",
	  "de\xf1\x1fi\xf1\x5ftirebilirsiniz..",
	  "de\xf1\x1fi\xf1\x5fikli\xf1\x1fi ba\xf1\x5far\xf1\x31s\xf1\x31z tutun",
	  "de\xf1\x1fi\xf1\x5ftirebilirsiniz"
   }
};

const char  *Disp_Wait[4][1] =	 
{
   {
	  "请稍候.."
   },
   {
	  "Please wait.."
   },
   //change by liya	2012-10-16
   {
   	  "Пожалуйста подождите..."
   },
   //土耳其语;by gzz 20121115
   {
	  "L\xf0\xfctfen bekleyin.."
   }
};

const char  *Disp_Timeout[4][1] =	 
{
   {
	  "      操作超时"
   },
   {
	  " Operation timeout"
   },
   //change by liya	2012-10-16
   {
   	  "Время истекло"
   },
   //土耳其语;by gzz 20121115
   {
	  "      Zaman A\xf1\x5f\xf1\x31m\xf1\x31"
   }
};

const char  *Disp_End[4][3] =
{
    {
	    "        谢谢",
		"    品味旅途，收藏记忆",
		"        谢谢惠顾!"
	},
	{
	    "Thank You",
		" Thank you",
		"    Thank you"
	},
	//change by liya  2012-10-16
	{
		"Спасибо",
		"Спасибо",
		"Спасибо"
	},
    //土耳其语;by gzz 20121115
    {
	    "Te\xf1\x5fekk\xf0\xfcrler",
		"Te\xf1\x5fekk\xf0\xfcrler",
		"    Te\xf1\x5f\ekk\xf0\xfcrler"
	}
};



//修改输入货道出货的信息;by gzz 20121016
const char  *Disp_Confirm[4][4] =
{
    {
	    "确认ENTER 返回CANCEL",
		"          返回CANCEL",
		"请按确定键出货 ENTER",
		"请按返回键修改CANCEL"
	},
	{
	    "  ENTER     CANCEL",
		"            CANCEL",
		"  ENTER     outgood",
		"  CANCEL    back"
	},
	//change by liya 2012-10-16
	{
	    "Введите     канал",
		"          Выход",
		"ENTER   судоходство",
		"CANCEL  возвращать"
	},
    //土耳其语;by gzz 20121115
    {
	    "  ENTER     CANCEL",
		"            CANCEL",
		"  ENTER     nakliye",
		"  CANCEL    de\xf1\x1fi\xf1\x5ftirmek"
	}
};

const char  *Disp_Blank = "";
/*
const uint8_t Disp_Bmp[640] = 
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x02,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x3F,0x00,0x18,0x40,0x02,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xE1,0x00,0x31,0xC0,0x02,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x81,0x00,0x2F,0x00,0x02,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0x03,0x00,0x78,0x00,0x02,0x00,0x0F,0xFF,0xFF,0xE0,0x00,0x03,0x80,0x00,0x00,0x00,
0x00,0x00,0x00,0x02,0x06,0x03,0xC0,0x00,0x06,0x01,0xF8,0x08,0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x3F,0xBC,0xC0,0x00,0x04,0x7E,0x00,0x08,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x80,0x86,0x00,0x0F,0xC0,0x00,0x0F,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x10,0x81,0x83,0x00,0x74,0x00,0x00,0x1D,0x01,0x00,0x03,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x10,0x81,0x01,0x03,0xC4,0x70,0x00,0x32,0x0F,0x00,0x06,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x81,0x01,0xFC,0x04,0xD0,0x08,0x76,0x78,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x81,0x1E,0x80,0x0D,0x90,0x08,0x79,0xC0,0x00,0x70,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x81,0x30,0x80,0x09,0x10,0x08,0x7E,0x80,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x15,0x01,0x60,0x80,0x0B,0x10,0x18,0x79,0xC0,0x03,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x17,0x01,0xC0,0x80,0x0A,0x10,0x10,0x73,0x40,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x10,0x01,0x00,0x80,0x14,0x10,0x10,0x76,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x10,0x01,0x00,0x80,0x14,0x10,0x11,0x9C,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x10,0x00,0x00,0xF0,0x1E,0x10,0x31,0x18,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x30,0x00,0x80,0x90,0x1F,0x10,0x30,0x10,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x80,0x90,0x1D,0x90,0x30,0x17,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x80,0x80,0x34,0xD0,0x30,0x1D,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x01,0x8C,0x80,0x34,0x60,0x38,0x12,0xA0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x01,0x07,0x00,0x24,0x70,0x08,0x17,0xA0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x03,0x00,0x26,0xCC,0x00,0x30,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x06,0x00,0x23,0x82,0x00,0x20,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x60,0x00,0x06,0x00,0x20,0x01,0xD8,0x63,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x38,0x61,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};*/

#endif

#endif
