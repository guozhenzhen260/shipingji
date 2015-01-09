
#include "language.h"


//change by liya 2012-10-16
const char *   Language[24][4]=
{
 	{
 		"【      不足      】" ,
		"【    Noenough    】",
		" Достаточно  ",
		" yetersiz              "//土耳其语;by gzz 20121115
		
	},
	{
 		"【      故障      】" ,
		"【      Error     】" ,
		"  Ошибка  ",
		"  Hata"//土耳其语;by gzz 20121115
	},	
	{
 		"【      正常      】" ,
		"【     Normal     】" ,
		"    Хорошо   ",
		"    OK"//土耳其语;by gzz 20121115
	},
	{
 		"正在检测%2d，请等待 ." ,
		"Test COL%d,Waitting." ,
		"Тест COL%d,подождите",
		"S\xf0\xfctun Denetleme:%d,L\xf0\xfctfen bekleyin"//土耳其语;by gzz 20121115
	},
	{
 		"【  货道驱动正常  】" ,
		"【   Column  OK   】" ,
		"   КолонкаOK   ",
		"S\xf0\xfctun OK"//土耳其语;by gzz 20121115
	},
	{
 		"【  货道驱动异常  】" ,
		"【   Column Err   】" ,
		"   Колонка  ERR   ",
		"S\xf0\xfctun Hatal\xf1\x31"//土耳其语;by gzz 20121115
	},
	{
 		"【    保存成功    】" ,
		"【    Save OK     】" ,
		"   Сохранить  OK  ",
		"kurtarmak OK"//土耳其语;by gzz 20121115
	},

	{
 		"正常" ,
		"OK",
		"ОК",
		"OK"//土耳其语;by gzz 20121115
	},
	{
 		"异常" ,
		"E",
		"Е",
		"E"//土耳其语;by gzz 20121115
	},
	{
 		"正" ,
		"OK" ,
		"ОК",
		"OK"//土耳其语;by gzz 20121115
	},
	{
 		"故" ,
		"F ",
		"F",
		"F "//土耳其语;by gzz 20121115
	},
	 {
 		"未" ,
		"D " ,
		"D",
		"D "//土耳其语;by gzz 20121115
 	 },
	 {
 		"货道 %d:故障" ,
		"Column %d:Error" ,
		"Колонка %d:Ошибка",
		"S\xf0\xfctun %d Hatal\xf1\x31"//土耳其语;by gzz 20121115
	},
	{
 	 	"货道 %d:正常"  ,
		"Column %d: OK"	,
		"Колонка %d: OK",
		"S\xf0\xfctun %d OK"//土耳其语;by gzz 20121115
	},
	 {
 		 "货道 %d:缺货"  ,
		 //120703 by cq MDBCompatible
		 //"Column %d：Soldout"
		"Column %d:Soldout"	,
		"Колонка %d:Распродана",
		"S\xf0\xfctun %d Bitti"//土耳其语;by gzz 20121115
 	 },
	{
		"请投入硬币",
 	    "Insert coin",
		"Вставте монету",
		"Bozuk para girin"//土耳其语;by gzz 20121115
	 },
	{	
		"请投入纸币",
 	    "Insert bill" ,
		"Вставте купюру",
		"Ka\xf1\x1f\xf1\x31t para testi"//土耳其语;by gzz 20121115
 	 },
	{	
		"投币金额：",
 	    "Amount:"	,
		"Баланс:",
		"para girin"//土耳其语;by gzz 20121115
	},
	{
 		"缺" ,
		"N " ,
		"N",
		"N"//土耳其语;by gzz 20121115
	},
	{
	    "出货确认模块故障",
		"GOC Module Err" ,
		"GOC модуль ERR",
		"GOC Mod\xf0\xfcl hatas\xf1\x31"//土耳其语;by gzz 20121115
	},
	{
	    "出货确认模块正常",
		"GOC Module OK"	 ,
		"GOC модуль ОК",
		"GOC Mod\xf0\xfcl OK"//土耳其语;by gzz 20121115
	},
	{
	    "出货确认模块关闭",
		"GOC Module Off" ,
		"GOC модуль Выкл.",
		"GOC Mod\xf0\xfcl Off"//土耳其语;by gzz 20121115
	},
	{
	    "确定将第 %d层填满吗",
		"Add No.%d lay full?"  ,
		"Добав.кол. до макс?",
		"% D katman\xf1\x31doldurun belirleyin"//土耳其语;by gzz 20121115
	},
	{
	    "确定将全部层填满吗",
		"Add all lay full?"	,
		"Добав. все до макс?",
		"Doldurur belirleyin"//土耳其语;by gzz 20121115
	} 
};
