#ifndef _READER_H
#define	_READER_H

#include <stdint.h>

#define MBV_R_RESET			0x10
#define MBV_R_SETUP			0x11
#define MBV_R_POLL			0x12
#define MBV_R_VEND			0x13
#define MBV_R_READER		0x14
#define MBV_R_REVALUE		0x15
#define MBV_R_EXPANSION		0x17

#define READER_STA_INIT				0x00
#define READER_STA_INACTIVE			0x01
#define READER_STA_DISABLED			0x02
#define READER_STA_ENABLED			0x03
#define READER_STA_SESSION_IDLE		0x04
#define READER_STA_VEND				0x05
#define READER_STA_REVALUE			0x06
#define READER_STA_NAGATIVE_VEND	0x07

#define VTR_VEND_REQUEST		0x01
#define VTR_VEND_SUCCESS		0x02
#define VTR_VEND_FAILURE		0x03
#define VTR_SESSION_COMPLETE	0x04
#define VTR_READER_DISABLE		0x10
#define VTR_READER_ENABLE		0x11
#define VTR_READER_CANCEL		0x12

#define RTV_JUST_RESET					0x00
#define RTV_READER_CONFIG_INFO			0x01
#define RTV_DISPLAY_REQUEST				0x02
#define RTV_BEGIN_SESSION				0x03
#define RTV_SESSION_CANCEL_REQUEST		0x04
#define RTV_VEND_APPROVED				0x05
#define RTV_VEND_DENIED					0x06
#define RTV_END_SESSION					0x07
#define RTV_CANCELLED					0x08
#define RTV_PERIPHERAL_ID				0x09
#define RTV_MALFUNCTION_ERROR			0x0A
#define RTV_CMD_OUT_OF_SEQUENCE			0x0B

#define READER_TIMEDLY			100// 1 = 10ms

typedef struct
{
	uint8_t cComErr;//故障次数
	uint8_t cVmcSta;
	uint8_t cReaderSta;//当前所处状态
	uint8_t cReaderEnabel;//是否可用
	uint8_t cSessionCancel;//拔卡取消交易流程
	uint16_t iValueUnit;//比例因子
	uint16_t iItemValue;//商品单价

	uint16_t iTimeDly;

	//case 0x01://Reader Config Info
	uint8_t cConfigData;
	uint8_t cFeatureLevel;
	uint16_t iCountryCode;
	uint8_t cScaleFactor;
	uint8_t cDecimalPlaces;
	uint8_t cAppMaxRespTime;
	uint8_t cMixcellaneousOpt;
	
	//case 0x02://Display Request
	
	//case 0x03://Begin Session
	uint16_t iFundsAvailable;//卡金额
	
	//case 0x04://Session Cancel Request
	
	//case 0x05://Vend Approved
	uint16_t iVendAmount;
	
	//case 0x06://Vend Denied
	//case 0x07://End Session
	//case 0x08://Cancelled
	
	//case 0x09://Peripheral ID	
	//uint8_t ManufacturerCode[3];//02-04
	//uint8_t SerialNumber[11];//05-16
	//uint8_t ModelNumber[11];//17-28
	//uint8_t SoftwareVersion[2];//29-30	
	
	//case 0x0A://Malfunction / Error
	uint8_t cErrorCode;
	
	//case 0x0B://Cmd Out Of Sequence
	//case 0x0D://Revalue Approved
	//case 0x0E://Revalue Denied
	//case 0x0F://Revalue Limit Amount
	//case 0x10://User File Data
	//case 0x11://Time/Data Request
	//case 0x12://Data Entry Request
	
}  ReaderParaStruct;

extern ReaderParaStruct ReaderPara;



#endif

