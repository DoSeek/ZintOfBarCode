#include "catprint.h"
#include <QtPrintSupport\QPrinter>
#include <QtPrintSupport\QPrintDialog>
#include <QtWebKitWidgets\QWebPage>
#include <QtWebKitWidgets\QWebFrame>
#include <QtWebKit\QWebElement>
#include <QtGui\QPageSize>
#include <QtCore\QRectF>
#include <QtGui\QPainter>
#include <QtGui\QTextDocument>
#include <QtCore\QFile>
#include <QtCore\QTextCodec>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QPrinterInfo>
#include "../../catmodel/userinfomodel.h"
#include "../../catmodel/shopcashmodel.h"
#include "../../catmodel/sellgoodmodel.h"
#include "../pages/pagelogindlg.h"
#include "Code39.h"

#ifdef _DEBUG
#pragma comment(lib, "../Win32/Debug/catmodel.lib")
#else
#pragma comment(lib, "../Win32/Release/catmodel.lib")
#endif

QString catprint::m_strPrinterName = QString("");
catprint::catprint()
{
	m_bsell = true;
	m_items = VecOrderItem();
}

catprint::~catprint()
{

}

void catprint::init(const S_PRINT &stInfo, bool bsell, const VecOrderItem &items)
{
	m_bsell = bsell;

	m_items = items;

	//�Ա��������¸�ֵ
	QString strHtml("");
	getHtml(stInfo, strHtml);

	//printHtml(strHtml);
}

void catprint::getHtml(const S_PRINT &stInfo, QString &strHtml)
{
	QString strCashName("");
	QTextCodec *gbk = QTextCodec::codecForName("GB18030");
	if (gbk)
	{
		std::string strcashname = shopmodel::instance().GetCashname();
		strCashName = gbk->toUnicode(strcashname.c_str());
	}

	S_PRINT_SET stSetInfo;
	if (!getPrinterSet(stSetInfo))
	{
		LOG_ALL_ERROR(QStringLiteral("failed get print set info."));
		return;
	}

	MapPrintFormat mapFormat;
	if (!getPrintFormat(mapFormat))
	{
		LOG_ALL_ERROR(QStringLiteral("failed get print fotmat info."));
		return;
	}

	//������Щ������Ҫ����
	//ͷ��
	QString strTitle = stSetInfo.strTitle;//loginmodel::instance().GetDealername();                  
	QString strShop = shopmodel::instance().GetShopname();
	QString strBarcode = stInfo.strTicket;//QString("");                 //���Ҫ������

	//����
	//m_strTicket; strCashName,
	QString strCashier = PageLoginDlg::m_strUserName;
	QString strDate("");
	if (stInfo.strDate.isEmpty())
		strDate = QDateTime::currentDateTime().toString("yyyy-MM--dd hh:mm:ss");
	else
		strDate = stInfo.strDate;

	//��β
	//����Ľ�����ҲҪ�ܽ��б���İ�
	QString strTel = stSetInfo.strTel;//shopmodel::instance().GetShoptel();
	QString strEnd = stSetInfo.strEnd;//QStringLiteral("��Ʊ��Ϊ�˻�ƾ֤");                   //���Ҳ��Ҫ�ܽ������õ�

	//���ڿ��Խ�������ƴװ��ӡ��
	//����
	strHtml = "";
	strHtml += QString("<html>"
		       "<head><style type=\"text/css\">"
		       "body {margin-left:15;}"
	           "h1 {font-size:110%;text-align:center;}"
	           "h2 {font-size:100%;text-align:center;}"
	           "h3 {font-size:90%;text-align:center;}");
	strHtml += QString("th {font-size:%1%;text-align:left;margin-left:0;}"
		"td {font-size:%1%;text-align:left;margin-left:0;}"
		"table {border:0;}</style></head>").arg(stSetInfo.nTextSize);

	//ͷ��
	strHtml += QString("<body>");
	if (mapFormat[E_PRINT_TICKET])
	{
		strHtml += QString("<h1 class=\"ticket\">");
		strHtml += strTitle;
		if (mapFormat[E_PRINT_SHOP])
		{
			strHtml += QString("<sub>");
			strHtml += strShop;
			strHtml += QString("</sub>");
		}
		strHtml += QString("</h1>");
	}
	else
	{
		if (mapFormat[E_PRINT_SHOP])
		{
			strHtml += QString("<h1 class=\"ticket\">");
			strHtml += strShop;
			strHtml += QString("</h1>");
		}
	}

	if (mapFormat[E_PRINT_BARCODE])
	{
		strHtml += QString("<table height=\"40\" width=\"100%\"><tr text-align=\"center\"><td text-align=\"center\">");
		strHtml += stInfo.strTicket;
		strHtml += QString("</td></tr></table>");
	}
	strHtml += QString("<hr />");

	//����Ӹ�40���� ��Ⱥ�ֽ�Ų���Ԫ��
	//����
	strHtml += QString("<table>");
	if (mapFormat[E_PRINT_TICKET])
	{
		strHtml += QString("<tr><th>");
		if (m_bsell)
			strHtml += QStringLiteral("���۵���:");
		else 
			strHtml += QStringLiteral("�˻�����:");

		strHtml += QString("</th><td>");
		strHtml += stInfo.strTicket;
		strHtml += QString("</td></tr>");
	}

	if (mapFormat[E_PRINT_CHECKSTAND])
	{
		strHtml += QString("<tr><th>");
		strHtml += QStringLiteral("����̨:");
		strHtml += QString("</th><td>");
		strHtml += strCashName;
		strHtml += QString("</td></tr>");
	}
	
	if (mapFormat[E_PRINT_CASHIER])
	{
		strHtml += QString("<tr><th>");
		strHtml += QStringLiteral("����Ա:");
		strHtml += QString("</th><td>");
		strHtml += strCashier;
		strHtml += QString("</td></tr>");
	}
	
	if (mapFormat[E_PRINT_DATE])
	{
		strHtml += QString("<tr><th>");
		strHtml += QStringLiteral("����:");
		strHtml += QString("</th><td>");
		strHtml += strDate;
		strHtml += QString("</td></tr>");
	}
	
	if (!stInfo.strMemNo.isEmpty())
	{
		if (mapFormat[E_PRINT_MEMNO])
		{
			strHtml += QString("<tr><th>");
			strHtml += QStringLiteral("��Ա��:");
			strHtml += QString("</th><td>");
			strHtml += stInfo.strMemNo;
			strHtml += QString("</td></tr>");
		}
		
		if (mapFormat[E_PRINT_SCORE])
		{
			strHtml += QString("<tr><th>");
			strHtml += QStringLiteral("ʣ�����:");
			strHtml += QString("</th><td>");
			strHtml += stInfo.strScores;
			strHtml += QString("</td></tr>");
		}	
	}

	strHtml += QString("</table><hr />");

	//������ʵ����������ǲ��Ǹ��������������нϺ�-----����Ʒ��
	//��Ʒ   
	strHtml += QString("<table>");
	if (mapFormat[E_PRINT_NAME])
	{
		strHtml += QString("<tr><th colspan=\"5\">");
		strHtml += QStringLiteral("Ʒ��");
		strHtml += QString("</th></tr>");
	}

	strHtml += QString("<tr>");
	if (mapFormat[E_PRINT_COUNT])
	{
		strHtml += QString("<th>");
		strHtml += QStringLiteral("����");
		strHtml += QString("</th>");
	}
	
	if (mapFormat[E_PRINT_RETAIL])
	{
		strHtml += QString("<th>");
		strHtml += QStringLiteral("����");
		strHtml += QString("</th>");
	}
	
	if (mapFormat[E_PRINT_SUM])
	{
		strHtml += QString("<th>");
		strHtml += QStringLiteral("���");
		strHtml += QString("</th>");
	}

	//modified by zrq for bug #6070
	//strHtml += QString("</tr>");
	//strHtml += QString("<tr>");

	if (mapFormat[E_PRINT_DISCOUNT])
	{
		strHtml += QString("<th>");
		strHtml += QStringLiteral("�ۿ�");
		strHtml += QString("</th>");
	}

	if (mapFormat[E_PRINT_UNIT])
	{
		strHtml += QString("<th>");
		strHtml += QStringLiteral("�ۺ��");
		strHtml += QString("</th>");   //�����б�����
	}
	strHtml += QString("</tr>");

	strHtml += QString("<tr><td colspan=\"5\"><hr /></td></tr>");
	if (!m_items.empty())
	{
		//����ֱ�Ӷ�����
		int ncount = m_items.size();
		for (int nindex = 0; nindex < ncount; ++nindex)
		{
			S_ORDER_ITEM item(m_items.at(nindex));
			if (!item.strName.isEmpty())
			{
				if (mapFormat[E_PRINT_NAME])
				{
					strHtml += QString("<tr><td colspan=\"3\">");
					strHtml += item.strName;
					strHtml += QString("</td></tr>");
				}

				strHtml += QString("<tr>");
				if (mapFormat[E_PRINT_COUNT])
				{
					strHtml += QString("<td>");
					strHtml += item.strCount;
					strHtml += QString("</td>");
				}

				if (mapFormat[E_PRINT_RETAIL])
				{
					strHtml += QString("<td>");
					strHtml += item.strRetail;
					strHtml += QString("</td>");
				}

				if (mapFormat[E_PRINT_SUM])
				{
					strHtml += QString("<td>");
					strHtml += item.strSum;
					strHtml += QString("</td>");
				}
				//strHtml += QString("</tr>");

				//strHtml += QString("<tr>");
				if (mapFormat[E_PRINT_DISCOUNT])
				{
					strHtml += QString("<td>");
					strHtml += item.strDiscount;
					strHtml += QString("</td>");
				}
				if (mapFormat[E_PRINT_UNIT])
				{
					strHtml += QString("<td>");
					strHtml += item.strUnit;
					strHtml += QString("</td>");
				}
				strHtml += QString("</tr>");
			}
		}
	}
	else
	{
		int ncount = goodmodel::instance().rowCount();
		for (int nindex = 0; nindex < ncount; ++nindex)
		{
			QString strName(""), strCount(""), strRetail(""), strSum(""), strDiscount(""), strUnit("");
			QModelIndex index = goodmodel::instance().index(nindex, sellgoodmodel::E_GOOD_NAME);
			if (index.isValid())
				strName = goodmodel::instance().data(index, Qt::DisplayRole).toString();

			index = goodmodel::instance().index(nindex, sellgoodmodel::E_GOOD_COUNT);
			if (index.isValid())
				strCount = goodmodel::instance().data(index, Qt::DisplayRole).toString();

			index = goodmodel::instance().index(nindex, sellgoodmodel::E_GOOD_RETAIL);
			if (index.isValid())
				strRetail = goodmodel::instance().data(index, Qt::DisplayRole).toString();

			index = goodmodel::instance().index(nindex, sellgoodmodel::E_GOOD_SUM);
			if (index.isValid())
				strSum = goodmodel::instance().data(index, Qt::DisplayRole).toString();

			index = goodmodel::instance().index(nindex, sellgoodmodel::E_GOOD_DISCOUNT);
			if (index.isValid())
				strDiscount = goodmodel::instance().data(index, Qt::DisplayRole).toString();

			index = goodmodel::instance().index(nindex, sellgoodmodel::E_GOOD_UNIT);
			if (index.isValid())
				strUnit = goodmodel::instance().data(index, Qt::DisplayRole).toString();

			if (!strName.isEmpty())
			{
				if (mapFormat[E_PRINT_NAME])
				{
					strHtml += QString("<tr><td colspan=\"5\">");
					strHtml += strName;
					strHtml += QString("</td></tr>");
				}


				strHtml += QString("<tr>");
				if (mapFormat[E_PRINT_COUNT])
				{
					strHtml += QString("<td>");
					strHtml += strCount;
					strHtml += QString("</td>");
				}

				if (mapFormat[E_PRINT_RETAIL])
				{
					strHtml += QString("<td>");
					strHtml += strRetail;
					strHtml += QString("</td>");
				}

				if (mapFormat[E_PRINT_SUM])
				{
					strHtml += QString("<td>");
					strHtml += strSum;
					strHtml += QString("</td>");
				}
				//strHtml += QString("</tr>");

				//strHtml += QString("<tr>");
				if (mapFormat[E_PRINT_DISCOUNT])
				{
					strHtml += QString("<td>");
					strHtml += strDiscount;
					strHtml += QString("</td>");
				}
				if (mapFormat[E_PRINT_UNIT])
				{
					strHtml += QString("<td>");
					strHtml += strUnit;
					strHtml += QString("</td>");
				}
				strHtml += QString("</tr>");
			}
		}
	}
	strHtml += QString("</table><hr />");

	//Ӧ��
	if (m_bsell)
	{
		strHtml += QString("<table><tr><th>");
		strHtml += QStringLiteral("�ϼ�");
		strHtml += QString("</th>");
		if (mapFormat[E_PRINT_PAYABLE])
		{
			strHtml += QString("<th>");
			strHtml += QStringLiteral("Ӧ�����");
			strHtml += QString("</th>");
		}
		strHtml += QString("</tr>");
		strHtml += QString("<tr>");
		if (mapFormat[E_PRINT_PRIVILEGE])
		{
			strHtml += QString("<th>");
			strHtml += QStringLiteral("�Żݽ��");
			strHtml += QString("</th>");
		}
		if (mapFormat[E_PRINT_TOTAL])
		{
			strHtml += QString("<th>");
			strHtml += QStringLiteral("ʵ�����");
			strHtml += QString("</th>");
		}
		strHtml += QString("</tr><tr><td colspan=\"2\"><hr /></td></tr>");

		strHtml += QString("<tr>");
		strHtml += QString("<td>");
		strHtml += stInfo.strAllSum;
		strHtml += QString("</td>");
		if (mapFormat[E_PRINT_PAYABLE])
		{
			strHtml += QString("<td>");
			strHtml += stInfo.strPayable;
			strHtml += QString("</td>");
		}
		strHtml += QString("</tr>");

		strHtml += QString("<tr>");
		if (mapFormat[E_PRINT_PRIVILEGE])
		{
			strHtml += QString("<td>");
			strHtml += stInfo.strPrivilege;
			strHtml += QString("</td>");
		}
		if (mapFormat[E_PRINT_TOTAL])
		{
			strHtml += QString("<td>");
			strHtml += stInfo.strTotal;
			strHtml += QString("</td>");
		}
		strHtml += QString("</tr></table><hr />");
	}
	

	//���㷽ʽ
	strHtml += QString("<h2>");
	strHtml += QStringLiteral("���㷽ʽ");
	strHtml += QString("</h2><table><tr>");
	if (mapFormat[E_PRINT_CASH])
	{
		strHtml += QString("<th>");
		strHtml += QStringLiteral("�ֽ�");
		strHtml += QString("</th>");
	}
	if (mapFormat[E_PRINT_BANK])
	{
		strHtml += QString("<th>");
		strHtml += QStringLiteral("������");
		strHtml += QString("</th>");
	}
	if (mapFormat[E_PRINT_CHANGE] && m_bsell)
	{
		strHtml += QString("<th>");
		strHtml += QStringLiteral("����");
		strHtml += QString("</th>");
	}
	strHtml += QString("</tr><tr>");
	if (mapFormat[E_PRINT_CASH])
	{
		strHtml += QString("<td>");
		strHtml += stInfo.strCash;
		strHtml += QString("</td>");
	}
	if (mapFormat[E_PRINT_BANK])
	{
		strHtml += QString("<td>");
		strHtml += stInfo.strBank;
		strHtml += QString("</td>");
	}
	if (mapFormat[E_PRINT_CHANGE] && m_bsell)
	{
		strHtml += QString("<td>");
		strHtml += stInfo.strChange;
		strHtml += QString("</td>");
	}
	strHtml += QString("</tr></table><hr />");

	//��β
	strHtml += QString("<h3>");
	strHtml += QStringLiteral("��ӭ�ٴι���");
	strHtml += QString("</h3>");
	if (mapFormat[E_PRINT_TEL])
	{
		strHtml += QString("<h3>");
		strHtml += strTel;
		strHtml += QString("</h3>");
	}
	if (mapFormat[E_PRINT_END])
	{
		strHtml += QString("<h3>");
		strHtml += strEnd;
		strHtml += QString("</h3>");
	}
	strHtml += QString("</body></html>");

	S_HTML shtml;
	shtml.nTicketSize = stSetInfo.nTicketSize;
	shtml.nPages = stSetInfo.nPages;
	shtml.strPrinterName = stSetInfo.strPrinterName;
	shtml.bBarcode = mapFormat[E_PRINT_END];
	shtml.strTicket = stInfo.strTicket;
	printHtml(strHtml, shtml);
}

//����ȫ���㶨���ٽ����ж�
void catprint::printHtml(const QString &strHtml, S_HTML &shtml)
{
	if (shtml.strPrinterName.isEmpty())
	{
		LOG_ALL_ERROR(QStringLiteral("failed get printer name."));
		return;
	}
	
	QPrinter printer;
	printer.setPrinterName(shtml.strPrinterName);
	//ò��ûʲô�ã������һ�����������
	printer.setFullPage(true);

	//�����ڴ�ӡ����DPI�£�58mm��Ӧ�ĵ���
	int dpi=printer.logicalDpiX();
	int width=shtml.nTicketSize*(dpi*1.0/25.4);//58mm

	//ʹ��QWebPage�������������������ĵ�����ӡ��
	QWebPage page;
	page.mainFrame()->setHtml(strHtml);

	QWebFrame *frame=page.mainFrame();


	//������ҳ�ӿڴ�С����Ϊ����html�ĵ�������Դ�С���ֵ�
	page.setViewportSize(QSize(width,frame->findFirstElement("body").geometry().height()));

	//����ҳͨ��painter��ӡ����
	//������δ�ӡ��ҳ��
	//����ط����Լ�Щ����ֱ�ӻ��Ƶ�ҳ����
	if (!shtml.bBarcode)
	{
		for (int npage = 0; npage < shtml.nPages;)
		{
			QPainter painter(&printer);
			if (painter.isActive())
				frame->render(&painter);
			++npage;
			if (npage < shtml.nPages)
				printer.newPage();
		}
	}
	else
	{
		QSize imgsize(width, 40);
		QRect rtBarcode(0, frame->findFirstElement("h1.ticket").geometry().height(), 
			            width, 40);
		for (int npage = 0; npage < shtml.nPages;)
		{
			QPainter painter(&printer);
			if (painter.isActive())
				frame->render(&painter);
			drawBarcode(&painter, shtml.strTicket, rtBarcode);
			++npage;
			if (npage < shtml.nPages)
				printer.newPage();
		}
	}
}

bool catprint::getPrinterSet(S_PRINT_SET &stInfo)
{
	bool bret = false;
	QSqlQuery query(QSqlDatabase::database("file", true));
	if (query.exec("select * from printset"))
	{
		while (query.next())
		{
			stInfo.strTitle         =       query.value("title").toString();
			stInfo.strTel           =       query.value("tel").toString();
			stInfo.strEnd           =       query.value("end").toString();
			stInfo.nTextSize        =       query.value("textsize").toInt();
			stInfo.strPrinterName   =       query.value("printername").toString();
			stInfo.nTicketSize      =       query.value("ticketsize").toInt();
			stInfo.bOpenCashbox     =       query.value("opencashbox").toBool();
			stInfo.nPages           =       query.value("pages").toInt();
			bret = true;
			break;
		}
	}
	return bret;
}

bool catprint::getPrintFormat(MapPrintFormat &mapInfo)
{
	bool bret = false;
	QSqlQuery query(QSqlDatabase::database("file", true));
	if (query.exec("select * from printformat"))
	{
		while (query.next())
		{
			mapInfo[E_PRINT_TITLE]         =      (bool)query.value("btitle").toInt();
			mapInfo[E_PRINT_SHOP]          =      (bool)query.value("bshop").toInt();
			mapInfo[E_PRINT_BARCODE]       =      (bool)query.value("bbarcode").toInt();
			mapInfo[E_PRINT_TICKET]        =      (bool)query.value("bsellid").toInt();
			mapInfo[E_PRINT_CHECKSTAND]    =      (bool)query.value("bcashdesk").toInt();
			mapInfo[E_PRINT_CASHIER]       =      (bool)query.value("bcashier").toInt();
			mapInfo[E_PRINT_DATE]          =      (bool)query.value("bdate").toInt();
			mapInfo[E_PRINT_MEMNO]         =      (bool)query.value("bmemid").toInt();
			mapInfo[E_PRINT_SCORE]         =      (bool)query.value("bscore").toInt();
			mapInfo[E_PRINT_PAYABLE]       =      (bool)query.value("bpayable").toInt();
			mapInfo[E_PRINT_PRIVILEGE]     =      (bool)query.value("bprivilege").toInt();
			mapInfo[E_PRINT_TOTAL]         =      (bool)query.value("btotal").toInt();
			mapInfo[E_PRINT_NAME]          =      (bool)query.value("bgoodname").toInt();
			mapInfo[E_PRINT_COUNT]         =      (bool)query.value("bgoodcount").toInt();
			mapInfo[E_PRINT_RETAIL]        =      (bool)query.value("bunit").toInt();
			mapInfo[E_PRINT_SUM]           =      (bool)query.value("bsum").toInt();
			mapInfo[E_PRINT_DISCOUNT]      =      (bool)query.value("bdiscount").toInt();
			mapInfo[E_PRINT_UNIT]          =      (bool)query.value("bprice").toInt();
			mapInfo[E_PRINT_CASH]          =      (bool)query.value("bcash").toInt();
			mapInfo[E_PRINT_BANK]          =      (bool)query.value("bbank").toInt();
			mapInfo[E_PRINT_CHANGE]        =      (bool)query.value("bchange").toInt();
			mapInfo[E_PRINT_TEL]           =      (bool)query.value("btel").toInt();
			mapInfo[E_PRINT_END]           =      (bool)query.value("bend").toInt();
			bret = true;
			break;
		}
	}
	return bret;
}

bool catprint::drawBarcode(QPainter *painter, const QString &strTicket, const QRect &rtBarcode)
{
	CCode39 mycode;
	mycode.LoadData(strTicket, QString("pos.jpg"), 1, (rtBarcode.height() - 10), rtBarcode.left() + 5, rtBarcode.top() + 5, 2.0);
	mycode.DrawBitmap(painter);
	return true;
}

void catprint::submitPrint(const S_SUBMIT_ITEM &item)
{
	//����ƴ��html����
	QString strHtml("");
	S_PRINT_SET stInfo;
	S_HTML html;
	if (getPrinterSet(stInfo))
	{
		QString strCashName("");
		QTextCodec *gbk = QTextCodec::codecForName("GB18030");
		if (gbk)
		{
			std::string strcashname = shopmodel::instance().GetCashname();
			strCashName = gbk->toUnicode(strcashname.c_str());
		}
		QString strCashier = PageLoginDlg::m_strUserName;

		//���ﻹҪ���ݸ�ʽ����
		strHtml += QString("<html>"
			"<head><style type=\"text/css\">"
			"body {margin-left:15;}"
			"h1 {font-size:120%;}"
			"h2 {font-size:100%;}"
			"h3 {font-size:90%;}");
		strHtml += QString("th {font-size:%1%;text-align:left;margin-left:0;}"
			"td {font-size:%1%;text-align:left;margin-left:0;}"
			"table {border:0;}</style></head>").arg(stInfo.nTextSize);

		strHtml += "<body>";
		strHtml += "<h2 align=\"center\">";
		strHtml += item.strDate;
		strHtml += "</h2>";

		strHtml += "<table width=\"100%\" border=\"0\" style=\"font-size:120%\"><tr><td width=\"50%\"><h3 align=\"left\">";
		strHtml += QStringLiteral("����Ա:");
		strHtml += strCashier;
		strHtml += "</h3></td><td width=\"50%\"><h3 align=\"left\">";
		strHtml += QStringLiteral("����̨:");
		strHtml += strCashName;
		strHtml += "</h3></td></tr><tr><td width=\"50%\"><h3 align=\"left\">";
		strHtml += QStringLiteral("ʵ�ս��:");
		strHtml += "</h3></td><td width=\"50%\"><h3 align=\"left\">";
		strHtml += QString("%1").arg(item.dwSellBank + item.dwSellCash - item.dwBackCash - item.dwBackBank, 0, 'F', 2);
		strHtml += "</h3></td><tr></table>";

		strHtml += "<table align=\"center\"><tr><td>";
		strHtml += QStringLiteral("�ֽ�:");
		strHtml += "</td><td>";
		strHtml += QString("%1").arg(item.dwSellCash - item.dwBackCash, 0, 'F', 2);
		strHtml += "</td><tr><td>";
		strHtml += QStringLiteral("����:");
		strHtml += "</td><td>";
		strHtml += QString("%1").arg(item.dwSellBank - item.dwBackBank, 0, 'F', 2);
		strHtml += "</td></tr></table>";

		strHtml += "<table width=\"100%\" border=\"0\"><tr><td width=\"50%\" colspan=\"2\"><h3 align=\"left\">";
		strHtml += QStringLiteral("����:");
		strHtml += "</h3></td><td width=\"50%\" colspan=\"2\"><h3 align=\"left\">";
		strHtml += QStringLiteral("�˻�:");
		strHtml += "</h3></td><tr>";

		strHtml += "<tr><td>";
		strHtml += QStringLiteral("����:");
		strHtml += "</td><td>";
		strHtml += QString("%1").arg(item.dwSellBank, 0, 'F', 2);
		strHtml += "</td><td>";
		strHtml += QStringLiteral("����:");
		strHtml += "</td><td>";
		strHtml += QString("%1").arg(item.dwBackBank, 0, 'F', 2);
		strHtml += "</td></tr>";

		strHtml += "<tr><td>";
		strHtml += QStringLiteral("�ֽ�:");
		strHtml += "</td><td>";
		strHtml += QString("%1").arg(item.dwSellCash, 0, 'F', 2);
		strHtml += "</td><td>";
		strHtml += QStringLiteral("�ֽ�:");
		strHtml += "</td><td>";
		strHtml += QString("%1").arg(item.dwBackCash, 0, 'F', 2);
		strHtml += "</td></tr>";

		strHtml += "<tr><td>";
		strHtml += QStringLiteral("����:");
		strHtml += "</td><td>";
		strHtml += QString("%1").arg(item.nSellOrder);
		strHtml += "</td><td>";
		strHtml += QStringLiteral("����:");
		strHtml += "</td><td>";
		strHtml += QString("%1").arg(item.nBackOrder);
		strHtml += "</td></tr>";

		strHtml += "<tr><td>";
		strHtml += QStringLiteral("����:");
		strHtml += "</td><td>";
		strHtml += QString("%1").arg(item.nSellCount);
		strHtml += "</td><td>";
		strHtml += QStringLiteral("����:");
		strHtml += "</td><td>";
		strHtml += QString("%1").arg(item.nBackCount);
		strHtml += "</td></tr>";

		strHtml += "<tr><td>";
		strHtml += QStringLiteral("�͵���:");
		strHtml += "</td><td>";
		if (item.nSellOrder > 0)
			strHtml += QString("%1").arg((item.dwSellBank + item.dwSellCash) / item.nSellOrder, 0, 'F', 2);
		else
			strHtml += "0.00";
		strHtml += "</td></tr>";

		strHtml += "</table>";

		strHtml += "<h2>";
		strHtml += QStringLiteral("ǩ��:");
		strHtml += "</h2>";

		strHtml += "</body>";

		html.bBarcode = false;
		html.nPages = 1;
		html.nTicketSize = stInfo.nTicketSize;
		html.strPrinterName = stInfo.strPrinterName;
		printHtml(strHtml, html);
	}
}

void catprint::opencashbox()
{
	QString strHtml("<html><body></body></html>");
	QString strPrinterName("");
	if (!(strPrinterName = getPrinterName()).isEmpty())
	{
		QPrinter printer;
		printer.setPrinterName(strPrinterName);
		//ò��ûʲô�ã������һ�����������
		printer.setFullPage(true);

		QTextDocument doc;
		doc.setHtml(strHtml);
		doc.print(&printer);
	}
}

QString catprint::getPrinterName()
{
	S_PRINT_SET stSetInfo;
	QString strPrinterName("");
	if (getPrinterSet(stSetInfo))
		strPrinterName = stSetInfo.strPrinterName;
	else
		strPrinterName = QPrinterInfo::defaultPrinterName();

	if (strPrinterName.isEmpty())
	{
		common::createTipDlg(QStringLiteral("û�п��õĴ�ӡ��,���Ȱ�װ"), nullptr);
	}
	return "";
}
//��ȡϵͳĬ�ϴ�ӡ��

//�����Ƿ�Ҫ�Ը�ʽ�������ٿ��ƻ᲻��������� 
//ԭ���Ĵ�ӡ ���Ǻ���900���нӽ�1000�д��� �������ǽ�������������200����
//���������ʵ������
//��Ҫ�ܵ����Ի������ѡ���ӡ�� 
//����Ҫ�����ݿ��м��ظ�ʽ����
//����ʹ�ӡ�йصĻ����˻����ش�СƱ��СƱ�ŵ�ɨ����