/************************************************  
 * 文件名称:    catprint.h
 * 作    者:    张瑞强    
 * 说    明:    实现打印功能(销售,退货,重打小票,交接等)
 * 版 本 号：   V1.0  
 * 创建日期:    2015-6-30  
 **************************************************/

#ifndef CATPRINT_H
#define CATPRINT_H

#include <QtCore/QString>
#include <map>
#include <QPainter>
#include <QRect>
#include <vector>

//这里增加一个打开钱箱的功能
class catprint
{
public:
	typedef struct stPrint
	{
		QString strTicket;                             //小票号
		QString strMemNo;                              //会员卡号
		QString strScores;                             //剩余积分
		QString strAllSum;                             //合计
		QString strPayable;                            //应付金额
		QString strPrivilege;                          //优惠金额
		QString strTotal;                              //实付金额
		QString strCash;                               //现金
		QString strBank;                               //银联卡
		QString strChange;                             //找零
		QString strDate;                               //时间
	}S_PRINT;

	typedef struct stPrintSet
	{
		int nTextSize;                               //正文大小 %
		int nTicketSize;                             //58,80mm
		int nPages;                                  //份数
		bool bOpenCashbox;                           //开启钱箱
		QString strPrinterName;                      //打印机名称
		QString strTitle;                            //标题
		QString strTel;                              //电话
		QString strEnd;                              //结束语
	}S_PRINT_SET;

	//int nTicketSize, int nPages, const QString &strPrinterName
	typedef struct stHtml
	{
		bool bBarcode;
		int nTicketSize;
		int nPages;
		QString strPrinterName;
		QString strTicket;
	}S_HTML;

	typedef struct stOrderItem
	{
		QString strName;
		QString strCount;
		QString strRetail;
		QString strSum;
		QString strDiscount;
		QString strUnit;
	}S_ORDER_ITEM;

	typedef struct stSubmitItem
	{
		int nSellOrder;
		int nBackOrder;
		int nSellCount;
		int nBackCount;
		double dwSellCash;
		double dwSellBank;
		double dwBackCash;
		double dwBackBank;
		QString strDate;
	}S_SUBMIT_ITEM;

	typedef enum ePrintFormat
	{
		E_PRINT_TITLE            =            0,
		E_PRINT_SHOP             =            1,
		E_PRINT_BARCODE          =            2,
		E_PRINT_TICKET           =            3,
		E_PRINT_CHECKSTAND       =            4,
		E_PRINT_CASHIER          =            5,
		E_PRINT_DATE             =            6,
		E_PRINT_MEMNO            =            7,
		E_PRINT_SCORE            =            8,
		E_PRINT_PAYABLE          =            9,
		E_PRINT_PRIVILEGE        =            10,
		E_PRINT_TOTAL            =            11,
		E_PRINT_NAME             =            12,
		E_PRINT_COUNT            =            13,
		E_PRINT_RETAIL           =            14,
		E_PRINT_SUM              =            15,
		E_PRINT_DISCOUNT         =            16,
		E_PRINT_UNIT             =            17,
		E_PRINT_CASH             =            18,
		E_PRINT_BANK             =            19,
		E_PRINT_CHANGE           =            20,
		E_PRINT_TEL              =            21,
		E_PRINT_END              =            22
	}E_PRINT_FORMAT;

	typedef std::map<E_PRINT_FORMAT, bool> MapPrintFormat;
	typedef std::vector<S_ORDER_ITEM>      VecOrderItem;

public:
	catprint();
	~catprint();

public:
	/*
	 * 函数介绍: 实现打印部分初始化并开始打印
	 * 输入参数: 参数名称       说明              可否为空
	 *           strTicket      小票号            N
	 *           bsell          是否销售          N
	 * 输出参数: 
	 * 返 回 值: 
	 */
	void init(const S_PRINT &stInfo, bool bsell = true, const VecOrderItem &items = VecOrderItem()); 

	/*
	 * 函数介绍: 实现交接班部分打印
	 * 输入参数: 参数名称       说明              可否为空
	 *           item           交接班信息        N
	 * 输出参数: 
	 * 返 回 值: 
	 */
	void submitPrint(const S_SUBMIT_ITEM &item);

	void opencashbox();
 
private:
	void printHtml(const QString &strHtml, S_HTML &html);

	//打印html
	void getHtml(const S_PRINT &stInfo, QString &strHtml);                   //拼装html
	bool getPrinterSet(S_PRINT_SET &stInfo);
	bool getPrintFormat(MapPrintFormat &mapInfo);
	bool drawBarcode(QPainter *painter, const QString &strTicket, const QRect &rtBarcode);
	QString getPrinterName();

private:
	static QString m_strPrinterName;        //这里搞下要
	bool m_bsell;                           //销售
	VecOrderItem m_items;
};

#endif

//可能存在的问题
//这里如果打印机出现故障应该怎么处理---------------------先不考虑这个