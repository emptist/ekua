#include "stdafx.h"
#include <windows.h>
#include <winbase.h>
#include <iostream>

using namespace std;

//开发文档
//http://chaoguwaigua.yuanqiinv.com/downloads/apicpp.txt

//1.交易API均是Trade.dll文件的导出函数，包括以下函数：
//基本版的10个函数：
//void   OpenTdx();
//void   CloseTdx();
//  int  Logon(char* IP, short Port, char* Version, char* AccountNo,char* JyPassword,   char* TxPassword, char* ErrInfo);
// void  Logoff(int ClientID);
// void  QueryData(int ClientID, int Category, char* Result, char* ErrInfo);
// void  SendOrder(int ClientID, int Category ,int PriceType,  char* Gddm,  char* Zqdm , float Price, int Quantity,  char* Result, char* ErrInfo);
// void  CancelOrder(int ClientID, char* hth, char* Result, char* ErrInfo);
// void  GetQuote(int ClientID, char* Zqdm, char* Result, char* ErrInfo);
// void  Repay(int ClientID, char* Amount, char* Result, char* ErrInfo);
// void  GetEdition(char* Result);

//批量版新增的5个函数：
// void  QueryHistoryData(int ClientID, int Category, char* StartDate, char* EndDate, char* Result, char* ErrInfo);
// void  QueryDatas(int ClientID, int Category[], int Count, char* Result[], char* ErrInfo[]);
// void  SendOrders(int ClientID, int Category[] , int PriceType[], char* Gddm[],  char* Zqdm[] , float Price[], int Quantity[],  int Count, char* Result[], char* ErrInfo[]);
// void  CancelOrders(int ClientID, char* hth[], int Count, char* Result[], char* ErrInfo[]);
// void  GetQuotes(int ClientID, char* Zqdm[], int Count, char* Result[], char* ErrInfo[]);

//2.API使用流程为: 应用程序先调用OpenTdx打开通达信实例，一个实例下可以同时登录多个交易账户，每个交易账户称之为ClientID.
//通过调用Logon获得ClientID，然后可以调用其他API函数向各个ClientID进行查询或下单; 应用程序退出时应调用Logoff注销ClientID, 最后调用CloseTdx关闭通达信实例.
//OpenTdx和CloseTdx在整个应用程序中只能被调用一次.


//3. 各个函数功能说明

/// <summary>
/// 打开通达信实例
/// </summary>
///void   OpenTdx();
typedef void(__stdcall* OpenTdxDelegate)();


/// <summary>
/// 关闭通达信实例
/// </summary>
///void   CloseTdx();
typedef void(__stdcall* CloseTdxDelegate)();


/// <summary>
/// 交易账户登录
/// </summary>
/// <param name="IP">券商交易服务器IP</param>
/// <param name="Port">券商交易服务器端口</param>
/// <param name="Version">设置通达信客户端的版本号</param>
/// <param name="AccountNo">资金账号</param>
/// <param name="JyPassword">交易密码</param>
/// <param name="TxPassword">通讯密码</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
/// <returns>客户端ID，失败时返回-1</returns>
///  int  Logon(char* IP, short Port, char* Version, char* AccountNo,char* JyPassword,   char* TxPassword, char* ErrInfo);
typedef int(__stdcall* LogonDelegate)( char* IP, short Port, char* Version, char* AccountNo, char* JyPassword, char* TxPassword, char* ErrInfo);

/// <summary>
/// 交易账户注销
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// void  Logoff(int ClientID);
typedef void(__stdcall* LogoffDelegate)(int ClientID);

/// <summary>
/// 查询各种交易数据
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">表示查询信息的种类，0资金  1股份   2当日委托  3当日成交     4可撤单   5股东代码  6融资余额   7融券余额  8可融证券</param>
/// <param name="Result">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">同Logon函数的ErrInfo说明</param>
/// void  QueryData(int ClientID, int Category, char* Result, char* ErrInfo);
typedef void(__stdcall* QueryDataDelegate)(int ClientID, int Category, char* Result, char* ErrInfo);

//比如查询股东代码时返回的结果字符串就是 "股东代码\t股东名称\t帐号类别\t保留信息\n0000064567\t\t0\t\nA000064567\t\t1\t\n2000064567\t\t2\t\nB000064567\t\t3\t"
//查得此数据之后，通过分割字符串， 可以恢复为几行几列的表格形式的数据


/// <summary>
/// 属于批量版功能,查询各种历史数据
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">表示查询信息的种类，0历史委托  1历史成交   2交割单</param>
/// <param name="StartDate">表示开始日期，格式为yyyyMMdd,比如2014年3月1日为  20140301
/// <param name="EndDate">表示结束日期，格式为yyyyMMdd,比如2014年3月1日为  20140301
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  QueryHistoryData(int ClientID, int Category, char* StartDate, char* EndDate, char* Result, char* ErrInfo);
typedef void(__stdcall* QueryHistoryDataDelegate)(int ClientID, int Category, char* StartDate, char* EndDate, char* Result, char* ErrInfo);

/// <summary>
/// 属于批量版功能,批量查询各种交易数据,用数组传入每个委托的参数，数组第i个元素表示第i个查询的相应参数
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">信息的种类的数组, 第i个元素表示第i个查询的信息种类，0资金  1股份   2当日委托  3当日成交     4可撤单   5股东代码  6融资余额   7融券余额  8可融证券</param>
/// <param name="Count">查询的个数，即数组的长度</param>
/// <param name="Result">返回数据的数组, 第i个元素表示第i个委托的返回信息. 此API执行返回后，Result[i]含义同上。</param>
/// <param name="ErrInfo">错误信息的数组，第i个元素表示第i个委托的错误信息. 此API执行返回后，ErrInfo[i]含义同上。</param>
/// void  QueryDatas(int ClientID, int Category[], int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* QueryDatasDelegate)(int ClientID, int Category[], int Count, char* Result[], char* ErrInfo[]);



/// <summary>
/// 下委托交易证券
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">表示委托的种类，0买入 1卖出  2融资买入  3融券卖出   4买券还券   5卖券还款  6现券还券</param>
/// <param name="PriceType">表示报价方式 0  上海限价委托 深圳限价委托 1深圳对方最优价格  2深圳本方最优价格  3深圳即时成交剩余撤销  4上海五档即成剩撤 深圳五档即成剩撤 5深圳全额成交或撤销 6上海五档即成转限价
/// <param name="Gddm">股东代码, 交易上海股票填上海的股东代码；交易深圳的股票填入深圳的股东代码</param>
/// <param name="Zqdm">证券代码</param>
/// <param name="Price">委托价格</param>
/// <param name="Quantity">委托数量</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  SendOrder(int ClientID, int Category ,int PriceType,  char* Gddm,  char* Zqdm , float Price, int Quantity,  char* Result, char* ErrInfo);
typedef void(__stdcall* SendOrderDelegate)(int ClientID, int Category, int PriceType, char* Gddm, char* Zqdm, float Price, int Quantity, char* Result, char* ErrInfo);



/// <summary>
/// 属于批量版功能,批量下委托交易证券，用数组传入每个委托的参数，数组第i个元素表示第i个委托的相应参数
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">委托种类的数组，第i个元素表示第i个委托的种类，0买入 1卖出  2融资买入  3融券卖出   4买券还券   5卖券还款  6现券还券</param>
/// <param name="PriceType">表示报价方式的数组,  第i个元素表示第i个委托的报价方式, 0  上海限价委托 深圳限价委托 1深圳对方最优价格  2深圳本方最优价格  3深圳即时成交剩余撤销  4上海五档即成剩撤 深圳五档即成剩撤 5深圳全额成交或撤销 6上海五档即成转限价
/// <param name="Gddm">股东代码数组，第i个元素表示第i个委托的股东代码，交易上海股票填上海的股东代码；交易深圳的股票填入深圳的股东代码</param>
/// <param name="Zqdm">证券代码数组，第i个元素表示第i个委托的证券代码</param>
/// <param name="Price">委托价格数组，第i个元素表示第i个委托的委托价格</param>
/// <param name="Quantity">委托数量数组，第i个元素表示第i个委托的委托数量</param>
/// <param name="Count">委托的个数，即数组的长度</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  SendOrders(int ClientID, int Category[] , int PriceType[], char* Gddm[],  char* Zqdm[] , float Price[], int Quantity[],  int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* SendOrdersDelegate)(int ClientID, int Category[], int PriceType[], char* Gddm[], char* Zqdm[], float Price[], int Quantity[], int Count, char* Result[], char* ErrInfo[]);


/// <summary>
/// 撤委托
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="hth">表示要撤的目标委托的编号</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  CancelOrder(int ClientID, char* hth, char* Result, char* ErrInfo);
typedef void(__stdcall* CancelOrderDelegate)(int ClientID, char* hth, char* Result, char* ErrInfo);




/// <summary>
/// 属于批量版功能,批量撤委托, 用数组传入每个委托的参数，数组第i个元素表示第i个撤委托的相应参数
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="hth">表示要撤的目标委托的编号</param>
/// <param name="Count">撤委托的个数，即数组的长度</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  CancelOrders(int ClientID, char* hth[], int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* CancelOrdersDelegate)(int ClientID, char* hth[], int Count, char* Result[], char* ErrInfo[]);


/// <summary>
/// 获取证券的实时五档行情
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Zqdm">证券代码</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  GetQuote(int ClientID, char* Zqdm, char* Result, char* ErrInfo);
typedef void(__stdcall* GetQuoteDelegate)(int ClientID, char* Zqdm, char* Result, char* ErrInfo);


/// <summary>
/// 属于批量版功能,批量获取证券的实时五档行情
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Zqdm">证券代码</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  GetQuotes(int ClientID, char* Zqdm[], int Count, char* Result[], char* ErrInfo[]);
typedef void(__stdcall* GetQuotesDelegate)(int ClientID, char* Zqdm[], int Count, char* Result[], char* ErrInfo[]);


/// <summary>
/// 融资融券直接还款
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Amount">还款金额</param>
/// <param name="Result">同上</param>
/// <param name="ErrInfo">同上</param>
/// void  Repay(int ClientID, char* Amount, char* Result, char* ErrInfo);
typedef void(__stdcall* RepayDelegate)(int ClientID, char* Amount, char* Result, char* ErrInfo);


/// <summary>
/// 获取api适用的券商账户类型
/// </summary>
/// <param name="Result">此API执行返回后，保存了返回的账户类型结果，为一字符串。一般要分配256字节的空间。</param>
/// void  GetEdition(char* Result);
typedef void(__stdcall* GetEditionDelegate)(char* Result);


int _tmain(int argc, _TCHAR* argv[])
{
	//载入dll, 所有dll都要复制到debug目录下,或者release目录下,采用多字节字符集编程设置
	HMODULE HMODULE1 = LoadLibrary("Trade.dll");

	//获取api函数
	OpenTdxDelegate OpenTdx = (OpenTdxDelegate)GetProcAddress(HMODULE1, "OpenTdx");
	CloseTdxDelegate CloseTdx = (CloseTdxDelegate)GetProcAddress(HMODULE1, "CloseTdx");
	LogonDelegate Logon = (LogonDelegate)GetProcAddress(HMODULE1, "Logon");
	LogoffDelegate Logoff = (LogoffDelegate)GetProcAddress(HMODULE1, "Logoff");
	QueryDataDelegate QueryData = (QueryDataDelegate)GetProcAddress(HMODULE1, "QueryData");
	SendOrderDelegate SendOrder = (SendOrderDelegate)GetProcAddress(HMODULE1, "SendOrder");
	CancelOrderDelegate CancelOrder = (CancelOrderDelegate)GetProcAddress(HMODULE1, "CancelOrder");
	GetQuoteDelegate GetQuote = (GetQuoteDelegate)GetProcAddress(HMODULE1, "GetQuote");
	RepayDelegate Repay = (RepayDelegate)GetProcAddress(HMODULE1, "Repay");
	GetEditionDelegate GetEdition = (GetEditionDelegate)GetProcAddress(HMODULE1, "GetEdition");

	//以下是批量版功能函数
	QueryDatasDelegate QueryDatas = (QueryDatasDelegate)GetProcAddress(HMODULE1, "QueryDatas");
	QueryHistoryDataDelegate QueryHistoryData = (QueryHistoryDataDelegate)GetProcAddress(HMODULE1, "QueryHistoryData");
	SendOrdersDelegate SendOrders = (SendOrdersDelegate)GetProcAddress(HMODULE1, "SendOrders");
	CancelOrdersDelegate CancelOrders = (CancelOrdersDelegate)GetProcAddress(HMODULE1, "CancelOrders");
	GetQuotesDelegate GetQuotes = (GetQuotesDelegate)GetProcAddress(HMODULE1, "GetQuotes");


	//开始交易
	char* Result = new char[1024 * 1024];
	char* ErrInfo = new char[256];

	GetEdition(Result);//获取dll适用券商
	cout << "dll适用券商:"<<Result << endl;

	OpenTdx();//初始化服务器地址
	int ClientID = Logon("222.178.70.12", 7706, "2.20", "99999", "11111", "", ErrInfo);//登录
	if (ClientID == -1)
	{
		cout << ErrInfo << endl;//登录失败
	}
	else
	{
		//登录成功
		QueryData(ClientID, 0, Result, ErrInfo);//查询资金
		cout << "查询资金结果:"<<Result << " " << ErrInfo;


		SendOrder(ClientID, 0, 0, "A111111", "601988", 2.7, 100, Result, ErrInfo);//买入股票
		cout << "买入下单结果:" << Result << " " << ErrInfo;

		//批量下50个单
		const int Count = 50;

		int Category[Count];
		int PriceType[Count];
		char* Gddm[Count];
		char* Zqdm[Count];
		float Price[Count];
		int Quantity[Count];

		char* Result1[Count];
		char* ErrInfo1[Count];

		for (int i = 0; i < Count; i++)
		{
			Category[i] = 0;
			PriceType[i] = 0;
			Gddm[i] = "A11111";
			Zqdm[i] = "601988";
			Price[i] = 2.27;
			Quantity[i] = 100;
			Result1[i] = new char[1024];
			ErrInfo1[i] = new char[256];
		}

		SendOrders(ClientID, Category, PriceType, Gddm, Zqdm, Price, Quantity, Count, Result1, ErrInfo1);


		for (int i = 0; i < Count; i++)
		{
			cout << "第"<<i<<"单结果:" << Result1[i] << " " << ErrInfo1[i];

			delete[] Result1[i];
			delete[] ErrInfo1[i];
		}



		Logoff(ClientID);//注销
	}


	CloseTdx();//退出服务器

	delete[] Result;
	delete[] ErrInfo;



	FreeLibrary(HMODULE1);

	return 0;
}
