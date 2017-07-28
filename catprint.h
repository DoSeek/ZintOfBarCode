/************************************************  
 * �ļ�����:    catprint.h
 * ��    ��:    ����ǿ    
 * ˵    ��:    ʵ�ִ�ӡ����(����,�˻�,�ش�СƱ,���ӵ�)
 * �� �� �ţ�   V1.0  
 * ��������:    2015-6-30  
 **************************************************/

#ifndef CATPRINT_H
#define CATPRINT_H

#include <QtCore/QString>
#include <map>
#include <QPainter>
#include <QRect>
#include <vector>

//��������һ����Ǯ��Ĺ���
class catprint
{
public:
	typedef struct stPrint
	{
		QString strTicket;                             //СƱ��
		QString strMemNo;                              //��Ա����
		QString strScores;                             //ʣ�����
		QString strAllSum;                             //�ϼ�
		QString strPayable;                            //Ӧ�����
		QString strPrivilege;                          //�Żݽ��
		QString strTotal;                              //ʵ�����
		QString strCash;                               //�ֽ�
		QString strBank;                               //������
		QString strChange;                             //����
		QString strDate;                               //ʱ��
	}S_PRINT;

	typedef struct stPrintSet
	{
		int nTextSize;                               //���Ĵ�С %
		int nTicketSize;                             //58,80mm
		int nPages;                                  //����
		bool bOpenCashbox;                           //����Ǯ��
		QString strPrinterName;                      //��ӡ������
		QString strTitle;                            //����
		QString strTel;                              //�绰
		QString strEnd;                              //������
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
	 * ��������: ʵ�ִ�ӡ���ֳ�ʼ������ʼ��ӡ
	 * �������: ��������       ˵��              �ɷ�Ϊ��
	 *           strTicket      СƱ��            N
	 *           bsell          �Ƿ�����          N
	 * �������: 
	 * �� �� ֵ: 
	 */
	void init(const S_PRINT &stInfo, bool bsell = true, const VecOrderItem &items = VecOrderItem()); 

	/*
	 * ��������: ʵ�ֽ��Ӱಿ�ִ�ӡ
	 * �������: ��������       ˵��              �ɷ�Ϊ��
	 *           item           ���Ӱ���Ϣ        N
	 * �������: 
	 * �� �� ֵ: 
	 */
	void submitPrint(const S_SUBMIT_ITEM &item);

	void opencashbox();
 
private:
	void printHtml(const QString &strHtml, S_HTML &html);

	//��ӡhtml
	void getHtml(const S_PRINT &stInfo, QString &strHtml);                   //ƴװhtml
	bool getPrinterSet(S_PRINT_SET &stInfo);
	bool getPrintFormat(MapPrintFormat &mapInfo);
	bool drawBarcode(QPainter *painter, const QString &strTicket, const QRect &rtBarcode);
	QString getPrinterName();

private:
	static QString m_strPrinterName;        //�������Ҫ
	bool m_bsell;                           //����
	VecOrderItem m_items;
};

#endif

//���ܴ��ڵ�����
//���������ӡ�����ֹ���Ӧ����ô����---------------------�Ȳ��������