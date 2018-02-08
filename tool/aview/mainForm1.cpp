/** 
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
//---------------------------------------------------------------------------
#include <Filectrl.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vcl.h>
#pragma hdrstop

#define WIN32
#include <sql.h>
#include <sqlext.h>

#include "mainForm1.h"
#include "dsnManager.h"
#include "addUser.h"
#include "delimeter.h"
#include "winISQL.h"
#include "resource.h"
#include "createIndex.h"
#include "iloaderStatus.h"
#include "createTbs.h"
#include "addDatafile2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;

// iloader �ڵ��� �����ϱ� ���� ��Դ´�. �ɷ��� �𸣰γ�..
#define ILOADER_MAX_PROCESS  255
typedef struct {
	char uname[41];
	char tname[41];
	STARTUPINFO          si;
	PROCESS_INFORMATION  pi;
	char startTime[40];
	char endTime[40];
    int self;
} _ILOADERP;
_ILOADERP IloaderProcess[ILOADER_MAX_PROCESS];
int iloader_count =0;


// �÷������� ������� ����ü
typedef struct {
	char ColumnName[255];
	char ColumnType[255];
	char IsNull[255];
	char DefaultValue[255];
	int  Precision;
    int  Scale;
} _COLUMNNODE;

_COLUMNNODE  ColumnNode[1024];
int ColumnCount;

// Index������ ��� ���� ����ü
typedef struct {
	char IndexName[255];
    int  Unique;
	char ColumnName[255];
	char AscDesc[255];
} _INDEXNODE;

_INDEXNODE  IndexNode[1024];
int IndexCount;

char PKNAME[255];



// ForeignKey������ ��� ���� ����ü.
typedef struct {
	char FKTable[255];
	char FKColumn[255];
	char PKColumn[255];
	char FKName[255];
} _FKNODE;
_FKNODE FkNode[1024];
int FkCount ;


// ����ü ���� (DSNNAME, env, dbc) �� ��� �ϳ� �׶��׶� �����ұ�??
// ���ϱ�� �Ź� ���ᱸ���� ���ϴ�.
SQLHENV env = NULL;
SQLHDBC dbc = NULL;

// selectTable, selectView���� ���� ����, statement��ü
SQLHENV env2 = NULL;
SQLHDBC dbc2 = NULL;
SQLHSTMT stmt2 = NULL;

//---------------------------------------------------------------------------
// DB ErrorMessage�� �����ֱ�
void dbErrMsg(SQLHENV _env, SQLHDBC _dbc, SQLHSTMT _stmt)
{
    SQLCHAR errMsg[4096];
	SQLSMALLINT msgLen;
	SQLINTEGER errNo;
	char gMsg[8192];

	memset(errMsg, 0x00, sizeof(errMsg));
	SQLError(_env, _dbc, _stmt, NULL, &errNo, errMsg, 4095, &msgLen);
	sprintf(gMsg, "MainFrame] ErrNo=[%d]:ErrMsg=%s", errNo, errMsg);
	ShowMessage(gMsg);
}
//---------------------------------------------------------------------------
// DB handle Free
void freeDBHandle()
{

	SQLDisconnect(dbc);
	SQLFreeConnect(dbc);
	SQLFreeEnv(env);

	env = NULL;
	dbc = NULL;
	
	return;
}
//---------------------------------------------------------------------------
// DB Connect
int dbConnect(AnsiString DSN)
{
	SQLCHAR USER[41];
    SQLCHAR PASSWD[41];

	if (dbc != NULL)
	{
		ShowMessage("Already Connected !!");
		return 0;    
	}
	// ������ ���� �޸� �Ҵ�.
	if (SQLAllocEnv(&env) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocEnv Fail");
		return 0;
	}

	if (SQLAllocConnect(env, &dbc) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocConnect Fail");
		freeDBHandle();
		return 0;
	}

	// ���������� �����ɴϴ�..
	if (!Form5->getDsnInfo(DSN, "User",     USER))
	{
		ShowMessage("Can't Get User");
		return 0;
	}
	if (!Form5->getDsnInfo(DSN, "Password", PASSWD))
	{
		ShowMessage("Can't Get Password");
		return 0;
	}

	// ��¥ �����غ��ϴ�.
	if (SQLConnect(dbc, DSN.c_str(), SQL_NTS, USER, SQL_NTS, PASSWD, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, SQL_NULL_HSTMT);
		freeDBHandle();
		return 0;
	}

	return 1;
}


//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm5::AddDSN1Click(TObject *Sender)
{
    // FORM1 �� DSN MANAGER�̴�.!!
	Form1->ShowModal();
}
//---------------------------------------------------------------------------
// ����ڰ� �ʿ��� �κп� ���ؼ�
// DSN��������� ��û�ϸ� �������ش�.
int _fastcall TForm5::getDsnInfo(AnsiString DSN, char *type, char *ret)
{
	HKEY hKey2;
	DWORD value_type, length;
	TCHAR  ByVal1[1024], sBuf[1024], sBuf2[1024];
	AnsiString x;


	// DSN Name�� Ű�� �Ͽ� ��ü �ɼǰ��� ������ ã�´�.
	wsprintf(sBuf, "Software\\ODBC\\ODBC.INI\\%s", DSN.c_str() );
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					 sBuf,
					 0,
					 KEY_ALL_ACCESS,
					 &hKey2) != 0)
	{
		ShowMessage("RegOpenKeyEx-3 Fail");
		return 0;
	}

	// Database
	length = 1024;
	value_type = NULL;
	memset(ByVal1 , 0x00, sizeof(ByVal1));
	wsprintf(sBuf2, "%s", type);
	if (RegQueryValueEx(hKey2,
						sBuf2,
						0,
						&value_type,
						ByVal1,
						&length) == 0)
	{
		wsprintf(ret, "%s", ByVal1);
		RegCloseKey(hKey2);
		return 1;
	}

	return 0;
}
//---------------------------------------------------------------------------
// DSN LIST�� �������� �Լ�
void __fastcall TForm5::GetDsnList()
{
    HKEY hKey, hKey2;
	DWORD value_type, length;
	DWORD key_num;
	DWORD subkey_length;
	TCHAR  ByVal1[1024], sBuf[1024], subkey_name[1024], sBuf2[1024];
	FILETIME file_time;
	AnsiString x;

	// MainRootKey�� ����.
	wsprintf(sBuf , "Software\\ODBC\\ODBC.INI");
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				  sBuf,
				  0,
				  KEY_ALL_ACCESS,
				  &hKey) != 0)
	{
        ShowMessage("RegOpenKeyEx-1 Fail!!");
		return;
	}

	key_num = 0;
    DBNODE->Items->Clear();
	
	// Enum�� ������ �������� ODBC.INI�� ������.
	while (1)
	{
		subkey_length = 1024;
		memset(subkey_name , 0x00, sizeof(subkey_name));
		// �� �Լ��� ���ϸ� DSNLIST�� ���´�.
		if (RegEnumKeyEx( hKey,
						  key_num,
						  subkey_name,
						  &subkey_length,
						  0,
						  0 ,
						  0 ,
						  &file_time) != 0)
		{
			//ShowMessage("RegEnumKeyEx-1 Fail!!");
			break;
		}

		// DSN���� ������ �ٽ� Key�� ����.
		wsprintf(sBuf, "Software\\ODBC\\ODBC.INI\\%s", subkey_name);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
						 sBuf,
						 0,
						 KEY_ALL_ACCESS,
						 &hKey2) != 0)
		{
			//ShowMessage("RegOpenKeyEx-2 Fail");
			break;
		}

		// ���� Key���� Dirver�� Altibase������ Ȯ���Ѵ�.
		length = 1024;
		value_type = NULL;
		memset(ByVal1 , 0x00, sizeof(ByVal1));
		wsprintf(sBuf2, "Driver");
		if (RegQueryValueEx(hKey2,
							sBuf2,
							0,
							&value_type,
							ByVal1,
							&length) == 0)
		{
			// AltibaseDLL�� ���³��̳�?
		   AnsiString x = ByVal1;
		   int c;

		   // a4_CM451.dll �̴�.
		   c = x.Pos("a4_");
		   if (c != 0)
		   {
			  // ListBox�� ����Ѵ�.
			  TTreeNode *tNode = DBNODE->Items->GetFirstNode();
              TTreeNode *xNode;
			  xNode = DBNODE->Items->Add(tNode, subkey_name) ;
              xNode->ImageIndex = 2;
		   }
		}

		// ���ʿ� �����͸� �ݴ´�.
		RegCloseKey(hKey2);
		key_num++;

	}

	// ���� Key�ݴ´�.
	RegCloseKey(hKey);

}
//---------------------------------------------------------------------------
// Form������ DSN������������Ʈ�� �������� �װ� Ʈ���� Root���� ��Ͻ�Ų��.
void __fastcall TForm5::FormShow(TObject *Sender)
{
    memset(&IloaderProcess, 0x00, sizeof(IloaderProcess));
	GetDsnList();
	if (DBNODE->Items->GetFirstNode())
		SERVERNAME->Caption = DBNODE->Items->GetFirstNode()->Text;
}
//---------------------------------------------------------------------------
// TreeNode�� Ŭ�������� ó��.
void __fastcall TForm5::DBNODEClick(TObject *Sender)
{
	// ���� ���õ� TreeNode�� �̸��� �����´�.
	TTreeNode *tNode = DBNODE->Selected;
	TTreeNode *orgNode = DBNODE->Selected;
	int level = 0;
    AnsiString _user;

    //tNode->ImageIndex = -1;
    // ����������Ʈ�� ��� ���������.
	ColGrid->Visible = false;
	IndexGrid->Visible = false;
	DataGrid->Visible = false;
	CRTTBL_PANEL->Visible =false;
	CRTTBL_BUTTON->Visible = false;
	CRTMEMO->Visible = false;
	FETCHPANEL->Visible = false;
	TNAMES->Caption = "";
	PROC->Visible = false;
	PROCPANEL->Visible = false;
	ProcGrid->Visible = false;
    TBS_PANEL->Visible = false;

	//���õ� �׳��� �θ� �ִ��� ������ ������..
	while (tNode->Parent)
	{
		tNode = tNode->Parent;
		level++;
	}

	DBNODE->PopupMenu = NULL;

	// system-level
	if (level == 0) {
		DBNODE->PopupMenu = TreePopup1;
	}
	// user-level
	else if (level == 1) {
		if (orgNode->Text != "TABLESPACES" &&
			orgNode->Text != "PERFORMANCE VIEW" &&
			orgNode->Text != "ADMIN VIEW" )
		{
            DBNODE->PopupMenu = TreePopup2;
		}
	}// tablespace�� �ڽĳ����̸�..
	else if (level == 2 && orgNode->Parent->Text == "TABLESPACES") {
         DBNODE->PopupMenu = TreePopup7;
	}else if (level == 2 && ( orgNode->Text == "Tables" || orgNode->Text == "Views" ||
							  orgNode->Text == "Procedures"  || orgNode->Text == "Triggers") ){
         DBNODE->PopupMenu = TreePopup8;
	}	// table-level ������ tables, procedure, views, triggers���� popup�� �޶�� �Ѵ�.
	else if (level == 3) {
		 if (orgNode->Parent->Text == "Tables")
			 DBNODE->PopupMenu = TreePopup3;
		 else if (orgNode->Parent->Text == "Views")
			 DBNODE->PopupMenu = TreePopup4;
		 else if (orgNode->Parent->Text == "Triggers")
			 DBNODE->PopupMenu = TreePopup5;
		 else if (orgNode->Parent->Text == "Procedures")
			 DBNODE->PopupMenu = TreePopup6;
	}

	// �׳��� ���� DSN�Ѹ��� ������Ʈ�� ���ý�Ų��.
	SERVERNAME->Caption = tNode->Text;
}
//---------------------------------------------------------------------------
// TableSpaceList�� �����´�.
int BuildTablespaceTree(TTreeNode *tNode, AnsiString uname)
{
	SQLHSTMT stmt;
	char tname[255];
	char query[1024];
	int rc;
	int x;
    TTreeNode *xNode;


	wsprintf(query, "select a.name from v$datafiles a, v$tablespaces b "
				   "where a.spaceid = b.id "
				   "and   b.name = '%s' order by a.id asc ", uname.c_str());

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return 0;
	}
	if (SQLPrepare(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	if (SQLBindCol(stmt, 1, SQL_C_CHAR, tname, sizeof(tname), NULL) != SQL_SUCCESS)
	{
		ShowMessage("SQLBindCol-1 Fail");
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}
	if (SQLExecute(stmt) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	tNode->DeleteChildren();
	while (1)
	{
		memset(tname, 0x00, sizeof(tname));

		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA)
		{
			 break;
		}
		else if (rc != SQL_SUCCESS)
		{
			dbErrMsg(env, dbc, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			return 0;
		}
		xNode = Form5->DBNODE->Items->AddChild(tNode, tname);
        xNode->ImageIndex = 4;
	}
	SQLFreeStmt(stmt, SQL_DROP);
	return 1;
}
//---------------------------------------------------------------------------
// TableList�� �����´�.
int BuildTableTree(TTreeNode *tNode, AnsiString uname)
{
	SQLHSTMT stmt;
	char tname[255];
	char query[1024];
	int rc;
	int x;
    TTreeNode *xNode;


	wsprintf(query, "select a.table_name from system_.sys_tables_ a, system_.sys_users_ b "
				   "where a.user_id = b.user_id "
				   "and   a.table_type = 'T' "
				   "and   b.user_name = '%s' order by a.table_name asc ", uname.c_str());


	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return 0;
	}
	if (SQLPrepare(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	if (SQLBindCol(stmt, 1, SQL_C_CHAR, tname, sizeof(tname), NULL) != SQL_SUCCESS)
	{
		ShowMessage("SQLBindCol-1 Fail");
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}
	if (SQLExecute(stmt) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	tNode->DeleteChildren();
	while (1)
	{
		memset(tname, 0x00, sizeof(tname));

		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA)
		{
			 break;
		}
		else if (rc != SQL_SUCCESS)
		{
			dbErrMsg(env, dbc, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			return 0;
		}
		xNode = Form5->DBNODE->Items->AddChild(tNode, tname);
        xNode->ImageIndex = 4;
	}
	SQLFreeStmt(stmt, SQL_DROP);
	return 1;
}
//---------------------------------------------------------------------------
// Procedure List �����´�.
int BuildProcTree(TTreeNode *tNode, AnsiString uname)
{
	SQLHSTMT stmt;
	char tname[255];
	char query[1024];
	int rc;
	int x;
	TTreeNode *xNode;


	wsprintf(query, "select a.proc_name from system_.sys_procedures_ a, system_.sys_users_ b "
				   "where a.user_id = b.user_id "
				   "and   b.user_name = '%s' order by a.proc_name asc ", uname.c_str());

				   
	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return 0;
	}
	if (SQLPrepare(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	if (SQLBindCol(stmt, 1, SQL_C_CHAR, tname, sizeof(tname), NULL) != SQL_SUCCESS)
	{
		ShowMessage("SQLBindCol-1 Fail");
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}
	if (SQLExecute(stmt) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	tNode->DeleteChildren();
	while (1)
	{
		memset(tname, 0x00, sizeof(tname));

		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA)
		{
			 break;
		}
		else if (rc != SQL_SUCCESS)
		{
			dbErrMsg(env, dbc, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			return 0;
		}
		xNode = Form5->DBNODE->Items->AddChild(tNode, tname);
		xNode->ImageIndex = 5;
	}
	SQLFreeStmt(stmt, SQL_DROP);
	return 1;
}
//---------------------------------------------------------------------------
// View List �����´�.
int BuildViewTree(TTreeNode *tNode, AnsiString uname)
{
	SQLHSTMT stmt;
	char tname[255];
	char query[1024];
	int rc;
	int x;
    TTreeNode *xNode;

	wsprintf(query, "select a.table_name from system_.sys_tables_ a, system_.sys_users_ b "
				   "where a.user_id = b.user_id "
				   "and   a.table_type = 'V' "
				   "and   b.user_name = '%s' order by a.table_name asc ", uname.c_str());



	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return 0;
	}
	if (SQLPrepare(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	if (SQLBindCol(stmt, 1, SQL_C_CHAR, tname, sizeof(tname), NULL) != SQL_SUCCESS)
	{
		ShowMessage("SQLBindCol-1 Fail");
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}
	if (SQLExecute(stmt) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	tNode->DeleteChildren();
	while (1)
	{
		memset(tname, 0x00, sizeof(tname));

		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA)
		{
			 break;
		}
		else if (rc != SQL_SUCCESS)
		{
			dbErrMsg(env, dbc, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			return 0;
		}
		xNode = Form5->DBNODE->Items->AddChild(tNode, tname);
		xNode->ImageIndex = 5;
	}
	SQLFreeStmt(stmt, SQL_DROP);
	return 1;
}
//---------------------------------------------------------------------------
// trigger List �����´�.
int BuildTriggerTree(TTreeNode *tNode, AnsiString uname)
{
	SQLHSTMT stmt;
	char tname[255];
	char query[1024];
	int rc;
	int x;
    TTreeNode *xNode;

	wsprintf(query, "select a.trigger_name from system_.sys_triggers_ a, system_.sys_users_ b "
				   "where a.user_id = b.user_id "
				   "and   b.user_name = '%s' order by a.trigger_name asc ", uname.c_str());



	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return 0;
	}
	if (SQLPrepare(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	if (SQLBindCol(stmt, 1, SQL_C_CHAR, tname, sizeof(tname), NULL) != SQL_SUCCESS)
	{
		ShowMessage("SQLBindCol-1 Fail");
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}
	if (SQLExecute(stmt) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	tNode->DeleteChildren();
	while (1)
	{
		memset(tname, 0x00, sizeof(tname));

		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA)
		{
			 break;
		}
		else if (rc != SQL_SUCCESS)
		{
			dbErrMsg(env, dbc, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			return 0;
		}
		xNode = Form5->DBNODE->Items->AddChild(tNode, tname);
		xNode->ImageIndex = 5;
	}
	SQLFreeStmt(stmt, SQL_DROP);
	return 1;
}
//---------------------------------------------------------------------------
// tablespace List �����´�.
int BuildTbsTree(TTreeNode *tNode)
{
	SQLHSTMT stmt;
	char tname[255];
	char query[1024];
	int rc;
	int x;
    TTreeNode *xNode;

	wsprintf(query, "select name from v$tablespaces order by name asc ");

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return 0;
	}
	if (SQLPrepare(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	if (SQLBindCol(stmt, 1, SQL_C_CHAR, tname, sizeof(tname), NULL) != SQL_SUCCESS)
	{
		ShowMessage("SQLBindCol-1 Fail");
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}
	if (SQLExecute(stmt) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	tNode->DeleteChildren();
    xNode = Form5->DBNODE->Items->AddChild(tNode, "SYS_TBS_MEMORY");
	xNode->ImageIndex = 5;
	while (1)
	{
		memset(tname, 0x00, sizeof(tname));

		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA)
		{
			 break;
		}
		else if (rc != SQL_SUCCESS)
		{
			dbErrMsg(env, dbc, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			return 0;
		}
		xNode = Form5->DBNODE->Items->AddChild(tNode, tname);
		xNode->ImageIndex = 5;
        
		//BuildTablespaceTree(xNode, tname);
	}
	SQLFreeStmt(stmt, SQL_DROP);
            
	return 1;
}
//---------------------------------------------------------------------------
// performance List �����´�.
int BuildPviewTree(TTreeNode *tNode)
{
	SQLHSTMT stmt;
	char tname[255];
	char query[1024];
	int rc;
	int x;
    TTreeNode *xNode;

	sprintf(query, "select name from v$table where name like 'V$%' order by name asc ");
	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return 0;
	}
	if (SQLPrepare(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	if (SQLBindCol(stmt, 1, SQL_C_CHAR, tname, sizeof(tname), NULL) != SQL_SUCCESS)
	{
		ShowMessage("SQLBindCol-1 Fail");
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}
	if (SQLExecute(stmt) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	tNode->DeleteChildren();
    x = 0;
	while (1)
	{
		memset(tname, 0x00, sizeof(tname));

		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA)
		{
             //ShowMessage(x);
			 break;
		}
		else if (rc != SQL_SUCCESS)
		{
			dbErrMsg(env, dbc, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			return 0;
		}
		xNode = Form5->DBNODE->Items->AddChild(tNode, tname);
		xNode->ImageIndex = 5;
		x++;
	}
	SQLFreeStmt(stmt, SQL_DROP);
            
	return 1;
}
//---------------------------------------------------------------------------
// AdminView List �����´�.
int BuildAviewTree(TTreeNode *tNode)
{
	SQLHSTMT stmt;
	char tname[255];
	char query[1024];
	int rc;
	int x;
    TTreeNode *xNode;

	sprintf(query, "select table_name from system_.sys_tables_ "
					"where table_name like 'ADM_%' "
					"and   table_type = 'V' order by table_name asc ");
	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return 0;
	}
	if (SQLPrepare(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	if (SQLBindCol(stmt, 1, SQL_C_CHAR, tname, sizeof(tname), NULL) != SQL_SUCCESS)
	{
		ShowMessage("SQLBindCol-1 Fail");
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}
	if (SQLExecute(stmt) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return 0;
	}

	tNode->DeleteChildren();
    x = 0;
	while (1)
	{
		memset(tname, 0x00, sizeof(tname));

		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA)
		{
			 //ShowMessage(x);
			 break;
		}
		else if (rc != SQL_SUCCESS)
		{
			dbErrMsg(env, dbc, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			return 0;
		}
		xNode = Form5->DBNODE->Items->AddChild(tNode, tname);
        xNode->ImageIndex = 5;
		x++;
	}
	SQLFreeStmt(stmt, SQL_DROP);
            
	return 1;
}
//---------------------------------------------------------------------------
// DB�� �����ϴ� �޴��� Ŭ��������.
void __fastcall TForm5::Connect1Click(TObject *Sender)
{
	TTreeNode *tNode = DBNODE->Selected;
	TTreeNode *sNode, *kNode;
	SQLHSTMT stmt;
	char query[4096], uname[41];
    int rc;


	// ������ DSN�� ã�ư���.
	if (tNode->HasChildren == true)
	{
		ShowMessage("Already Connected !!");
		return;
	}

	// DB�� �����Ѵ�.
	if (!dbConnect(tNode->Text))
	{
		return;
	}

	// User���� �� user�� ���̺� List�� �ϴ� ���غ���.
	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail!!");
		freeDBHandle();
		return;
	}

	wsprintf(query, "select user_name from system_.sys_users_  order by user_name asc ");
	if (SQLPrepare(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
        freeDBHandle();
		return;
	}

	if (SQLBindCol(stmt, 1, SQL_C_CHAR, uname,  sizeof(uname), NULL) != SQL_SUCCESS)
	{
		ShowMessage("SQLBindCol-1 Fail");
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return;
	}

	if (SQLExecute(stmt) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
        freeDBHandle();
		return;
	}

	while (1)
	{
        memset(uname, 0x00, sizeof(uname));

		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA)
		{
			 break;
		}
		else if (rc != SQL_SUCCESS)
		{
			dbErrMsg(env, dbc, stmt);
			break;
		}


		// USER�� Ʈ���� ����Ѵ�.
		sNode = DBNODE->Items->AddChild(tNode, uname);
		sNode->ImageIndex = 1;

		// USER�� ���� ���̺��� ����Ѵ�.
		kNode = DBNODE->Items->AddChild(sNode, "Tables");
		kNode->ImageIndex = 3;
		rc = BuildTableTree(kNode, uname);

		// USER�� ���� Procedure�� ����Ѵ�.
		kNode = DBNODE->Items->AddChild(sNode, "Procedures");
		kNode->ImageIndex = 3;
		rc = BuildProcTree(kNode, uname);

		// USER�� ���� View�� ����Ѵ�.
		kNode = DBNODE->Items->AddChild(sNode, "Views");
		kNode->ImageIndex = 3;
		rc = BuildViewTree(kNode, uname);

		// USER�� ���� Trigger�� ����Ѵ�.
		kNode = DBNODE->Items->AddChild(sNode, "Triggers");
        kNode->ImageIndex = 3;
		rc = BuildTriggerTree(kNode, uname);
	}

	// tablespace tree����, user������ �ǹ̰� ������ �ϴ�.
	kNode = DBNODE->Items->AddChild(tNode, "TABLESPACES");
	kNode->ImageIndex = 3;
	BuildTbsTree(kNode);

	// performance View����,
	kNode = DBNODE->Items->AddChild(tNode, "PERFORMANCE VIEW");
	kNode->ImageIndex = 3;
	BuildPviewTree(kNode);

	// adminView ����.
	kNode = DBNODE->Items->AddChild(tNode, "ADMIN VIEW");
	kNode->ImageIndex = 3;
	BuildAviewTree(kNode);

	// Statemnet ����
	SQLFreeStmt(stmt, SQL_DROP);
	
	// ��������.
	freeDBHandle();
}
//---------------------------------------------------------------------------
// �������� (��� ��������� �ƴϰ� treeNode���� Clearó��.
void __fastcall TForm5::DisConnect1Click(TObject *Sender)
{
	TTreeNode *tNode = DBNODE->Selected;

	if (tNode->Parent)
	{
		ShowMessage("which do you want to disconnect?, maybe it's not DSN!");
		return;
	}

    tNode->DeleteChildren();
}
//---------------------------------------------------------------------------
// ���̺���� ����Ŭ�������� �÷������� �����´�.
void __fastcall TForm5::getColumnInfo(AnsiString uname, AnsiString tname)
{
	SQLHSTMT     stmt = NULL;
	SQLRETURN    rc;

	SQLCHAR   szCatalog[255], szSchema[255], szIndexName[255], szAscDesc[2];
	SQLCHAR   szTableName[255], szColumnName[255];
	SQLCHAR   szTypeName[255], szDefault[255];
	SQLCHAR   szColumnDefault[255], szIsNullable[255];
	SQLINTEGER  ColumnSize, BufferLength, CharOctetLength, OrdinalPosition;
	SQLSMALLINT DataType, DecimalDigits, NumPrecRadix, Nullable;
	SQLSMALLINT SQLDataType, DatetimeSubtypeCode;
	SQLSMALLINT NonUnique;

	SQLINTEGER cbCatalog, cbSchema, cbTableName, cbColumnName, cbDefault, cbPkName, cbNonUnique;
	SQLINTEGER cbDataType, cbTypeName, cbColumnSize, cbBufferLength, cbIndexName, cbAscDesc;
	SQLINTEGER cbDecimalDigits, cbNumPrecRadix, cbNullable ;
	SQLINTEGER cbColumnDefault, cbSQLDataType, cbDatetimeSubtypeCode, cbCharOctetLength;
	SQLINTEGER cbOrdinalPosition, cbIsNullable;
	int i ;

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return ;
	}
	// table columns��������
	if (SQLColumns(stmt, NULL, 0, uname.c_str(), SQL_NTS,  tname.c_str(), SQL_NTS,  NULL, 0) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return ;
	}
	SQLBindCol(stmt, 1, SQL_C_CHAR, szCatalog, 255,&cbCatalog);
	SQLBindCol(stmt, 2, SQL_C_CHAR, szSchema, 255, &cbSchema);
	SQLBindCol(stmt, 3, SQL_C_CHAR, szTableName, 255,&cbTableName);
	SQLBindCol(stmt, 4, SQL_C_CHAR, szColumnName, 255, &cbColumnName);
	SQLBindCol(stmt, 5, SQL_C_SSHORT, &DataType, 0, &cbDataType);
	SQLBindCol(stmt, 6, SQL_C_CHAR, szTypeName, 255, &cbTypeName);
	SQLBindCol(stmt, 7, SQL_C_SLONG, &ColumnSize, 0, &cbColumnSize);
	SQLBindCol(stmt, 8, SQL_C_SLONG, &BufferLength, 0, &cbBufferLength);
	SQLBindCol(stmt, 9, SQL_C_SSHORT, &DecimalDigits, 0, &cbDecimalDigits);
	SQLBindCol(stmt, 10, SQL_C_SSHORT, &NumPrecRadix, 0, &cbNumPrecRadix);
	SQLBindCol(stmt, 11, SQL_C_SSHORT, &Nullable, 0, &cbNullable);
	SQLBindCol(stmt, 13, SQL_C_CHAR,  szDefault, 255, &cbDefault);
	SQLBindCol(stmt, 17, SQL_C_SLONG, &OrdinalPosition, 0, &cbOrdinalPosition);
	SQLBindCol(stmt, 18, SQL_C_CHAR, szIsNullable, 255, &cbIsNullable);

	memset(&ColumnNode, 0x00, sizeof(ColumnNode));
	ColumnCount = 0;
	while (1)
	{

		memset(szColumnName, 0x00, sizeof(szColumnName));
		memset(szTypeName,   0x00, sizeof(szTypeName));
		memset(szDefault,    0x00, sizeof(szDefault));
		memset(szIsNullable, 0x00, sizeof(szIsNullable));

		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA) {
			 break;
		}
		else if (rc != SQL_SUCCESS) {
			dbErrMsg(env, dbc, stmt);
			break;
		}

		memcpy(ColumnNode[ColumnCount].ColumnName,   szColumnName, strlen(szColumnName));
		memcpy(ColumnNode[ColumnCount].ColumnType,   szTypeName,   strlen(szTypeName));
		memcpy(ColumnNode[ColumnCount].IsNull,       szIsNullable, strlen(szIsNullable));
		memcpy(ColumnNode[ColumnCount].DefaultValue, szDefault,    strlen(szDefault));
		ColumnNode[ColumnCount].Precision = ColumnSize;
		ColumnNode[ColumnCount].Scale     = DecimalDigits;
        ColumnCount++;
	}
	SQLFreeStmt(stmt, SQL_DROP);

}
//---------------------------------------------------------------------------
// ���̺���� ����Ŭ�������� Index������ �����´�.
void __fastcall TForm5::getIndexInfo(AnsiString uname, AnsiString tname)
{
	SQLHSTMT     stmt = NULL;
	SQLRETURN    rc;

	SQLCHAR   szCatalog[255], szSchema[255], szIndexName[255], szAscDesc[2];
	SQLCHAR   szTableName[255], szColumnName[255];
	SQLCHAR   szTypeName[255], szDefault[255];
	SQLCHAR   szColumnDefault[255], szIsNullable[255];
	SQLINTEGER  ColumnSize, BufferLength, CharOctetLength, OrdinalPosition;
	SQLSMALLINT DataType, DecimalDigits, NumPrecRadix, Nullable;
	SQLSMALLINT SQLDataType, DatetimeSubtypeCode;
	SQLSMALLINT NonUnique;

	SQLINTEGER cbCatalog, cbSchema, cbTableName, cbColumnName, cbDefault, cbPkName, cbNonUnique;
	SQLINTEGER cbDataType, cbTypeName, cbColumnSize, cbBufferLength, cbIndexName, cbAscDesc;
	SQLINTEGER cbDecimalDigits, cbNumPrecRadix, cbNullable ;
	SQLINTEGER cbColumnDefault, cbSQLDataType, cbDatetimeSubtypeCode, cbCharOctetLength;
	SQLINTEGER cbOrdinalPosition, cbIsNullable;
	int i ;

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return ;
	}
    
	// table Index������ �����Ѵ�.
	if (SQLStatistics(stmt, NULL, 0, uname.c_str(), SQL_NTS, tname.c_str(), SQL_NTS, SQL_INDEX_ALL, 0) != SQL_SUCCESS) {
		dbErrMsg(env, dbc, stmt);
        SQLFreeStmt(stmt, SQL_DROP);
		return;
	}
	SQLBindCol(stmt, 2, SQL_C_CHAR, szSchema, 255, &cbSchema);
	SQLBindCol(stmt, 3, SQL_C_CHAR, szTableName, 255,&cbTableName);
	SQLBindCol(stmt, 4, SQL_C_SSHORT, &NonUnique, 0, &cbNonUnique);
	SQLBindCol(stmt, 6, SQL_C_CHAR, szIndexName, 255, &cbIndexName);
    SQLBindCol(stmt, 8, SQL_C_SSHORT, &OrdinalPosition, 0, &cbOrdinalPosition);
	SQLBindCol(stmt, 9, SQL_C_CHAR, szColumnName, 255, &cbColumnName);
	SQLBindCol(stmt, 10, SQL_C_CHAR, szAscDesc, 2, &cbAscDesc);
	
    memset(&IndexNode, 0x00, sizeof(IndexNode));
    IndexCount = 0;
	while (1)
	{

		memset(szIndexName, 0x00, sizeof(szIndexName));
		memset(szColumnName, 0x00, sizeof(szColumnName));
		memset(szAscDesc, 0x00, sizeof(szAscDesc));
		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA) {
			break;
		} else if (rc != SQL_SUCCESS) {
			dbErrMsg(env, dbc, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			return;
		}

		memcpy(IndexNode[IndexCount].IndexName,    szIndexName,    strlen(szIndexName));
		memcpy(IndexNode[IndexCount].ColumnName,   szColumnName,   strlen(szColumnName));
		memcpy(IndexNode[IndexCount].AscDesc,       szAscDesc,     strlen(szAscDesc));
		IndexNode[IndexCount].Unique = NonUnique;
		IndexCount++;
	}
	SQLFreeStmt(stmt, SQL_DROP);

}
//---------------------------------------------------------------------------
// Index�����߿� PK������ �����´�.
void __fastcall TForm5::getPKInfo(AnsiString uname, AnsiString tname)
{
	SQLHSTMT     stmt = NULL;
	SQLRETURN    rc;

	SQLCHAR   szPkName[255],szColumnName[255], szCatalog[255], szSchema[255], szTableName[255];
	SQLINTEGER cbCatalog, cbSchema, cvTableName, cbColumnName, cbPkName, cbTableName;

    if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return ;
	}

	// table primary key ��������
	if (SQLPrimaryKeys(stmt, NULL, 0, uname.c_str(), SQL_NTS, tname.c_str(), SQL_NTS) != SQL_SUCCESS)
    {
        dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return;
    }

	SQLBindCol(stmt, 1, SQL_C_CHAR, szCatalog, 255,&cbCatalog);
	SQLBindCol(stmt, 2, SQL_C_CHAR, szSchema, 255, &cbSchema);
	SQLBindCol(stmt, 3, SQL_C_CHAR, szTableName, 255,&cbTableName);
	SQLBindCol(stmt, 4, SQL_C_CHAR, szColumnName, 255, &cbColumnName);
	SQLBindCol(stmt, 6, SQL_C_CHAR, szPkName, 255, &cbPkName);

    memset(szPkName, 0x00, sizeof(szPkName));
	rc = SQLFetch(stmt);
	if (rc != SQL_SUCCESS) {
		memset(PKNAME, 0x00, sizeof(PKNAME));
	} else {
		memcpy(PKNAME, szPkName, strlen(szPkName));
	}

	SQLFreeStmt(stmt, SQL_DROP);
}
//---------------------------------------------------------------------------
// �ε��������߿� ����Ű�� �����´�.
void __fastcall TForm5::getFKInfo(AnsiString uname, AnsiString tname)
{
    SQLHSTMT     stmt = NULL;
	SQLRETURN    rc;

	SQLCHAR   szFKTableName[255],szPKColumnName[255], szFKColumnName[255], szFkName[255];
	SQLINTEGER cbFKTableName, cbPKColumnName, cbFKColumnName,cbFkName;

    if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return ;
	}

	// table primary key ��������
	if (SQLForeignKeys(stmt, NULL, 0, NULL, 0,  NULL, 0, NULL, 0,
					   uname.c_str(), SQL_NTS,
					   tname.c_str(), SQL_NTS) != SQL_SUCCESS)
	{
        dbErrMsg(env, dbc, stmt);
        SQLFreeStmt(stmt, SQL_DROP);
		return;
	}
	SQLBindCol(stmt, 3, SQL_C_CHAR, szFKTableName, 255 ,&cbFKTableName);
	SQLBindCol(stmt, 4, SQL_C_CHAR, szPKColumnName, 255, &cbPKColumnName);
	SQLBindCol(stmt, 8, SQL_C_CHAR, szFKColumnName, 255, &cbFKColumnName);
	SQLBindCol(stmt, 12, SQL_C_CHAR, szFkName, 255, &cbFkName);

	memset(&FkNode, 0x00, sizeof(FkNode));
	FkCount = 0;
	while (1)
	{

		memset(szFKTableName, 0x00, sizeof(szFKTableName));
		memset(szPKColumnName, 0x00, sizeof(szPKColumnName));
		memset(szFKColumnName, 0x00, sizeof(szFKColumnName));
		memset(szFkName, 0x00, sizeof(szFkName));

		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA) {
			break;
		} else if (rc != SQL_SUCCESS) {
			dbErrMsg(env, dbc, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			return;
		}

		memcpy(FkNode[FkCount].FKTable,  szFKTableName, strlen(szFKTableName));
		memcpy(FkNode[FkCount].PKColumn, szPKColumnName, strlen(szPKColumnName));
		memcpy(FkNode[FkCount].FKColumn, szFKColumnName, strlen(szFKColumnName));
		memcpy(FkNode[FkCount].FKName,   szFkName,       strlen(szFkName));
		FkCount++;
    }
	SQLFreeStmt(stmt, SQL_DROP);
}
//---------------------------------------------------------------------------
// ���̺������� �����´�.
void __fastcall TForm5::getTableInfo(AnsiString uname, AnsiString tname, AnsiString ServerName)
{
	SQLHSTMT stmt;
	int rc;
	int row;
	int maxLen = 1;
	int maxLen1 = 1;
	int maxLen2 = 1;
	int x;
    int i;
	char oldName[255];

	//ShowMessage("getTableInfo");
	if (!dbConnect(SERVERNAME->Caption)) {
		return;
	}

    //�÷������� ����ü�� ��Ƶδ� �Լ��� ȣ���Ѵ�.
	getColumnInfo(uname, tname);
    ColGrid->ColCount = 6;
	ColGrid->Cells[0][0] = "ColumnName";
	ColGrid->Cells[1][0] = "ColumnType";
	ColGrid->Cells[2][0] = "Precision";
	ColGrid->Cells[3][0] = "Scale";
	ColGrid->Cells[4][0] = "IsNull";
	ColGrid->Cells[5][0] = "DefaultValue";

    row = 1;
	for (i = 0; i < ColumnCount; i++)
	{
		if ((int)strlen(ColumnNode[i].ColumnName) > maxLen) {
			maxLen = strlen(ColumnNode[i].ColumnName);
		}
		ColGrid->Cells[0][row] = ColumnNode[i].ColumnName;
		ColGrid->Cells[1][row] = ColumnNode[i].ColumnType;
		ColGrid->Cells[2][row] = ColumnNode[i].Precision;
		ColGrid->Cells[3][row] = ColumnNode[i].Scale;
		ColGrid->Cells[4][row] = ColumnNode[i].IsNull;
        ColGrid->Cells[5][row] = ColumnNode[i].DefaultValue;
		row++;
	}


	if (row == 1) {
		ColGrid->RowCount = 2;
		ColGrid->Cells[0][1] = "";
		ColGrid->Cells[1][1] = "";
		ColGrid->Cells[2][1] = "";
		ColGrid->Cells[3][1] = "";
		ColGrid->Cells[4][1] = "";
	}else {
		ColGrid->RowCount = row;
	}
	
	if (maxLen < (int)strlen("ColumnName"))
	{
		maxLen = strlen("ColumnName") + 1;
	}

	ColGrid->ColWidths[0] = maxLen*10;
	ColGrid->ColWidths[1] = 150;
	ColGrid->ColWidths[2] = 100;
	ColGrid->ColWidths[3] = 100;
	ColGrid->ColWidths[4] = 100;
    ColGrid->ColWidths[5] = 100;
	ColGrid->Top = TNAMES->Top + TNAMES->Height;
	ColGrid->Left = TNAMES->Left;
	ColGrid->Align = alClient;
	ColGrid->Visible = true;


	// ������ ���̺��� index������ ��� �����´�.
    getIndexInfo(uname, tname);


	IndexGrid->FixedCols = 0;
	IndexGrid->FixedRows = 1;
	IndexGrid->ColCount = 4;
	IndexGrid->Cells[0][0] = "IndexName";
	IndexGrid->Cells[1][0] = "IsUnique";
	IndexGrid->Cells[2][0] = "IndexColumn";
	IndexGrid->Cells[3][0] = "Sorting";
	row = 1;
	maxLen = 1;
    memset(oldName, 0x00, sizeof(oldName));
	for (i = 0; i < IndexCount; i++)
	{
		if (memcmp(oldName, IndexNode[i].IndexName, strlen(IndexNode[i].IndexName)) != 0)
		{
			IndexGrid->Cells[0][row] = IndexNode[i].IndexName;
			if (IndexNode[i].Unique == 0)
				IndexGrid->Cells[1][row] = "UNIQUE";
			else
				IndexGrid->Cells[1][row] = " ";
			IndexGrid->Cells[2][row] = IndexNode[i].ColumnName;
			IndexGrid->Cells[3][row] = IndexNode[i].AscDesc;
		}else {
			IndexGrid->Cells[0][row] = " ";
			IndexGrid->Cells[1][row] = " ";
			IndexGrid->Cells[2][row] = IndexNode[i].ColumnName;
			IndexGrid->Cells[3][row] = IndexNode[i].AscDesc;
		}

		memset(oldName, 0x00, sizeof(oldName));
		memcpy(oldName, IndexNode[i].IndexName, strlen(IndexNode[i].IndexName));
		if ((int)strlen(IndexNode[i].ColumnName) > maxLen1) {
			maxLen1 = strlen(IndexNode[i].IndexName);
		}
		if ((int)strlen(IndexNode[i].ColumnName) > maxLen2) {
		   maxLen2 = strlen(IndexNode[i].ColumnName);
		}
		row++;
	}

	if (row == 1) {
		IndexGrid->RowCount = 2;
		IndexGrid->Cells[0][1] = "";
		IndexGrid->Cells[1][1] = "";
		IndexGrid->Cells[2][1] = "";
		IndexGrid->Cells[3][1] = "";
	}else {
		IndexGrid->RowCount = row;
	}

	if (maxLen1 < (int)strlen("IndexName")) {
		maxLen1 = strlen("IndexName") + 1;
	}
	if (maxLen2 < (int)strlen("IndexColumn")) {
		maxLen2 = (int)strlen("IndexColumn") + 1;
	}

	IndexGrid->ColWidths[0] = maxLen1*10;
	IndexGrid->ColWidths[1] = 100;
	IndexGrid->ColWidths[2] = maxLen2*10;
	IndexGrid->ColWidths[3] = 100;

	IndexGrid->Top = ColGrid->Top + ColGrid->Height;
	IndexGrid->Left = ColGrid->Left;
	IndexGrid->Align = alBottom;
	IndexGrid->Height = Form5->Height/3;
	IndexGrid->Visible = true;
	freeDBHandle();

}
//---------------------------------------------------------------------------
// ������� Procedure ������ �����ؼ� ȭ�鿡 �ѷ��ش�.
AnsiString __fastcall TForm5::getProcedure( AnsiString user, AnsiString pname)
{
	SQLHSTMT stmt;
	AnsiString px = "", _tmp;
	char query[1024], parse[1024];
	int PROC_ID, rc;


	if (!dbConnect(SERVERNAME->Caption))
	{
		return "";
	}

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		freeDBHandle();
		return "";
	}
	sprintf(query,  "select a.parse "
					"from system_.sys_proc_parse_ a , system_.sys_users_ b, system_.sys_procedures_ c "
					"where b.user_name = '%s' "
					"and   a.user_id = b.user_id "
					"and   a.user_id = c.user_id "
					"and   c.proc_name = '%s' "
					"and   a.proc_oid = c.proc_oid order by a.seq_no",  user.c_str(), pname.c_str());
	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return "";
	}
    SQLBindCol(stmt, 1, SQL_C_CHAR, parse, sizeof(parse), NULL);
	px = "";
	while (1)
	{

         memset(parse, 0x00, sizeof(parse));
		 rc = SQLFetch(stmt);
		 if (rc == SQL_NO_DATA) {
			 break;
		 } else if (rc != SQL_SUCCESS) {
			 dbErrMsg(env, dbc, stmt);
			 SQLFreeStmt(stmt, SQL_DROP);
			 freeDBHandle();
			 return "";
		 }
		 px = px + parse;
	}
	// returning ���� �������ش�.
	SQLFreeStmt(stmt, SQL_DROP);
	freeDBHandle();

    return px;
}
//---------------------------------------------------------------------------
// ������� Views ������ �����ؼ� ȭ�鿡 �ѷ��ش�.
AnsiString __fastcall TForm5::getView( AnsiString user, AnsiString pname)
{
	SQLHSTMT stmt;
	AnsiString px = "", _tmp;
	char query[1024], parse[1024];
	int PROC_ID, rc;


	if (!dbConnect(SERVERNAME->Caption))
	{
		return "";
	}

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		freeDBHandle();
		return "";
	}
	sprintf(query,  "select a.parse "
					"from system_.sys_view_parse_ a , system_.sys_users_ b, system_.sys_tables_ c "
					"where b.user_name = '%s' "
					"and   a.user_id = b.user_id "
					"and   a.user_id = c.user_id "
					"and   c.table_name = '%s' and c.table_type = 'V' "
					"and   a.view_id = c.table_id order by a.seq_no",  user.c_str(), pname.c_str());
	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return "";
	}
    SQLBindCol(stmt, 1, SQL_C_CHAR, parse, sizeof(parse), NULL);
	px = "";
	while (1)
	{

         memset(parse, 0x00, sizeof(parse));
		 rc = SQLFetch(stmt);
		 if (rc == SQL_NO_DATA) {
			 break;
		 } else if (rc != SQL_SUCCESS) {
			 dbErrMsg(env, dbc, stmt);
			 SQLFreeStmt(stmt, SQL_DROP);
			 freeDBHandle();
			 return "";
		 }
		 px = px + parse;
	}
	// returning ���� �������ش�.
	SQLFreeStmt(stmt, SQL_DROP);
	freeDBHandle();

    return px;
}
//---------------------------------------------------------------------------
// ������� Views ������ �����ؼ� ȭ�鿡 �ѷ��ش�.
AnsiString __fastcall TForm5::getTrigger( AnsiString user, AnsiString pname)
{
	SQLHSTMT stmt;
	AnsiString px = "", _tmp;
	char query[1024], parse[1024];
	int PROC_ID, rc;


	if (!dbConnect(SERVERNAME->Caption))
	{
		return "";
	}

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		freeDBHandle();
		return "";
	}
	sprintf(query,  "select a.substring "
					"from system_.sys_trigger_strings_ a , system_.sys_users_ b, system_.sys_triggers_ c "
					"where b.user_name = '%s' "
					"and   b.user_id = c.user_id "
					"and   c.trigger_name = '%s' "
					"and   a.trigger_oid = c.trigger_oid order by a.seqno",  user.c_str(), pname.c_str());
	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return "";
	}
	SQLBindCol(stmt, 1, SQL_C_CHAR, parse, sizeof(parse), NULL);
	px = "";
	while (1)
	{

		 memset(parse, 0x00, sizeof(parse));
		 rc = SQLFetch(stmt);
		 if (rc == SQL_NO_DATA) {
			 break;
		 } else if (rc != SQL_SUCCESS) {
			 dbErrMsg(env, dbc, stmt);
			 SQLFreeStmt(stmt, SQL_DROP);
			 freeDBHandle();
			 return "";
		 }
		 px = px + parse;
	}
	// returning ���� �������ش�.
	SQLFreeStmt(stmt, SQL_DROP);
	freeDBHandle();

    return px;
}
//---------------------------------------------------------------------------
// ȭ�鿡 �Ѹ��� CR+LF ��ó�������� �Ѱǵ�.. �ָ���ó����.
void __fastcall TForm5::displayObject2(AnsiString x, int OSFLAG)
{
    int rc;
	AnsiString _tmp;
    
	// ��»��� ������ ���ο� ����� CR/LF���� ������� �Ѵ�. ���??
    PROC->Clear();
	while (1)
	{
		rc = x.Pos("\n");
		if (rc == 0)
			break;
		// �̻��ϰ� 2byte�� �߶󳻾� �Ѵ�. NT, unix�� �ٸ� �����ΰ� ������..
        // ���� ������ �𸣰ڱ�.
		_tmp = x.SubString(1, rc-OSFLAG);
		x = x.SubString(rc+1, x.Length()-rc);
        PROC->Lines->Add(_tmp);
	}
	PROC->Lines->Add(x);

	
    // ��ġ�����ϱ�,
	PROC->Top = TNAMES->Top + TNAMES->Height;
    PROC->Align = alClient;
	PROC->Visible = true;
	PROCPANEL->Align = alBottom;
	PROCPANEL->Visible = true;
}
//---------------------------------------------------------------------------
// �޸����̺����̽��� ����Ŭ���������� ó��
void __fastcall TForm5::getMemTbsInfo(AnsiString uname, AnsiString tname, AnsiString CONN)
{
	SQLHSTMT stmt;
	unsigned long used, TID, page_size, allocd;
	char query[1024], dname[255], temp[50], memMax1[512];
	int rc, row;
	unsigned long nextsize, maxsize, initsize, currsize, autoextend;
	unsigned long table_use, index_use, extend_page_cnt, max_sum, use1, use2;
	SQLINTEGER dname_ind;
	double rate1, total;
	AnsiString x;


    if (!dbConnect(CONN)) {
		return;
	}
    if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		freeDBHandle();
		return ;
	}


	// tablespace���� �⺻���� ����
    // �̻��ϰ� ODBC���� ����ȯ���� �������� ���Ѵ�. ���ϱ�?
	sprintf(query, "select value1 from v$property where name = 'MEM_MAX_DB_SIZE' ");
	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}

	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}
	SQLBindCol(stmt, 1, SQL_C_CHAR, memMax1, 255, NULL);
    memset(memMax1, 0x00, sizeof(memMax1));
	rc = SQLFetch(stmt);
	if  (rc != SQL_SUCCESS) {
		 dbErrMsg(env, dbc, stmt);
		 SQLFreeStmt(stmt, SQL_DROP);
		 freeDBHandle();
		 return ;
	}
	SQLFreeStmt(stmt, SQL_CLOSE);
	//ShowMessage(memMax1);

	sprintf(query, "select alloc_size from v$memstat where name = 'Storage_Memory_Manager' ");
	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}

	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}
	SQLBindCol(stmt, 1, SQL_C_ULONG, &allocd, 0, NULL);
	rc = SQLFetch(stmt);
	if (rc != SQL_SUCCESS) {
		 dbErrMsg(env, dbc, stmt);
		 SQLFreeStmt(stmt, SQL_DROP);
		 freeDBHandle();
		 return ;
	}
    SQLFreeStmt(stmt, SQL_CLOSE);

	sprintf(query, "select sum(fixed_used_mem+var_used_mem) from v$memtbl_info");
	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}

	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}
	SQLBindCol(stmt, 1, SQL_C_ULONG, &used, 0, NULL);
	rc = SQLFetch(stmt);
	if (rc != SQL_SUCCESS) {
		 dbErrMsg(env, dbc, stmt);
		 SQLFreeStmt(stmt, SQL_DROP);
		 freeDBHandle();
		 return ;
	}
	SQLFreeStmt(stmt, SQL_CLOSE);


    sprintf(query, "select alloc_size from v$memstat where name = 'Index_Memory' ");
	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}

	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}
	SQLBindCol(stmt, 1, SQL_C_ULONG, &index_use, 0, NULL);
	rc = SQLFetch(stmt);
	if (rc != SQL_SUCCESS) {
		 dbErrMsg(env, dbc, stmt);
		 SQLFreeStmt(stmt, SQL_DROP);
		 freeDBHandle();
		 return ;
	}
	SQLFreeStmt(stmt, SQL_CLOSE);



	// atol�Լ��� ���� �ʾƼ� ansistring�� �޼ҵ带 �������..
	x = memMax1;
	total = x.ToDouble();
	TOTAL_SIZE->Text = total;
	USED_SIZE->Text = allocd;
	TABLE_USE->Text = used;
    INDEX_USE->Text = index_use;


	row = 1;
	// tablespace�� ��뷮�� �׷�����.
    //rate1= 0;
	if (total != 0) {
		rate1 = (double)(allocd) / total * 100.0;
	}else
		rate1 = 0;

	sprintf(temp, "%6.2f%% allocated", rate1 );
	RATE1->Caption = temp;
	ProgressBar1->Position = (int)rate1;
	ProgressBar1->Width = TBS_PANEL->Width;
	RATE1->Top = ProgressBar1->Top+ProgressBar1->Height;
	RATE1->Left = ProgressBar1->Left;
	
    // DATAFILE_GRID ����.
	DATAFILE->ColWidths[0] = 250;
	DATAFILE->ColWidths[1] = 150;
	DATAFILE->ColWidths[2] = 150;
	DATAFILE->ColWidths[3] = 150;
	DATAFILE->ColWidths[4] = 150;
	DATAFILE->ColWidths[5] = 150;

	if (row == 1) {
		DATAFILE->RowCount = 2;
	} else
        DATAFILE->RowCount = row;

	TBS_PANEL->Left = TNAMES->Left;
	TBS_PANEL->Top  = TNAMES->Top + TNAMES->Height;
    TBS_PANEL->Align = alClient;
	TBS_PANEL->Visible = true;
    DATAFILE->Visible = false;

	SQLFreeStmt(stmt, SQL_DROP);
    freeDBHandle();
}
//---------------------------------------------------------------------------
// ��ũ���̺����̽��� ����Ŭ���������� ó��
void __fastcall TForm5::getTbsInfo(AnsiString uname, AnsiString tname, AnsiString CONN)
{
	SQLHSTMT stmt;
	unsigned long total, used, TID, page_size;
	char query[1024], dname[255], temp[50], VER[50];
	int rc, row;
	unsigned long nextsize, maxsize, initsize, currsize, autoextend;
	unsigned long table_use, index_use, extend_page_cnt, max_sum, use1, use2;
	SQLINTEGER dname_ind, ver_ind;
	double rate1 ;


	TOTAL_SIZE->Text = "";
    USED_SIZE->Text = "";
    TABLE_USE->Text = "";
    INDEX_USE->Text = "";
    DATAFILE->ColCount = 6;
	DATAFILE->RowCount = 2;
	DATAFILE->Cells[0][0] = "Datafile";
	DATAFILE->Cells[1][0] = "currsize";
	DATAFILE->Cells[2][0] = "maxsize";
	DATAFILE->Cells[3][0] = "initsize";
	DATAFILE->Cells[4][0] = "nextsize";
	DATAFILE->Cells[5][0] = "autoextend";
    DATAFILE->Cells[0][1] = "";
	DATAFILE->Cells[1][1] = "";
	DATAFILE->Cells[2][1] = "";
	DATAFILE->Cells[3][1] = "";
	DATAFILE->Cells[4][1] = "";
	DATAFILE->Cells[5][1] = "";

	if (tname == "SYS_TBS_MEMORY") {
        getMemTbsInfo(uname, tname, CONN);
		return;
	}


	if (!dbConnect(CONN)) {
		return;
	}
    if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		freeDBHandle();
		return ;
	}

    // Altibase Version������ �����´�.
    sprintf(query, "select PRODUCT_VERSION from v$version ");
    if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}
	SQLBindCol(stmt, 1, SQL_C_CHAR, VER, sizeof(VER), &ver_ind);
    memset(VER, 0x00, sizeof(VER));
	rc = SQLFetch(stmt);
	if (rc != SQL_SUCCESS) {
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}
    SQLFreeStmt(stmt, SQL_CLOSE);


	// tablespace���� �⺻���� ����
	sprintf(query, "select a.id, (select nvl(sum(b.maxsize*a.page_size), 0)  "
					   "            from v$datafiles b where b.spaceid = a.id ), "
					   "             (select nvl(sum(b.currsize*a.page_size), 0)  "
					   "            from v$datafiles b where b.spaceid = a.id ), "
					   "       a.page_size, a.A_EXTENT_PAGE_CNT  "
					   "from v$tablespaces  a "
					   "where name = '%s' ", tname);

	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}
	SQLBindCol(stmt, 1, SQL_C_ULONG, &TID, 0, NULL);
	SQLBindCol(stmt, 2, SQL_C_ULONG, &total, 0, NULL);
	SQLBindCol(stmt, 3, SQL_C_ULONG, &used, 0, NULL);
	SQLBindCol(stmt, 4, SQL_C_ULONG, &page_size, 0, NULL);
	SQLBindCol(stmt, 5, SQL_C_ULONG, &extend_page_cnt, 0, NULL);
	while (1)
	{
		 rc = SQLFetch(stmt);
		 if (rc == SQL_NO_DATA) {
			 break;
		 } else if (rc != SQL_SUCCESS) {
			 dbErrMsg(env, dbc, stmt);
			 SQLFreeStmt(stmt, SQL_DROP);
			 freeDBHandle();
			 return ;
		 }

	}
	TOTAL_SIZE->Text = total;
    USED_SIZE->Text = used;
    SQLFreeStmt(stmt, SQL_CLOSE);


	// TALBE_OBJECT�� �������� ��뷮
	sprintf(query, "select nvl(sum(a.EXTENT_TOTAL_COUNT),0) "
				   "from v$segment a, v$disktbl_info b "
				   "where a.space_id = %ld "
				   "and   a.space_id = b.tablespace_id "
                   "and   a.table_oid = b.table_oid "
				   "and   a.segment_desc = b.seg_desc ", TID);

	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}
	SQLBindCol(stmt, 1, SQL_C_ULONG, &table_use, 0, NULL);
	while (1)
	{
		 rc = SQLFetch(stmt);
		 if (rc == SQL_NO_DATA) {
			 break;
		 } else if (rc != SQL_SUCCESS) {
			 dbErrMsg(env, dbc, stmt);
			 SQLFreeStmt(stmt, SQL_DROP);
			 freeDBHandle();
			 return ;
		 }

	}
	SQLFreeStmt(stmt, SQL_CLOSE);

	TABLE_USE->Text = table_use * page_size * extend_page_cnt;
	use1 = (double)(table_use * page_size * extend_page_cnt);

    // INDEX_OBJECT �������� ��뷮
	if (memcmp(VER, "4.3.7", 5) == 0) {
		sprintf(query, "select nvl(sum(a.EXTENT_TOTAL_COUNT),0) "
				   "from v$segment a, v$disk_index b "
				   "where a.space_id = %ld "
				   "and   a.table_oid = b.table_oid "
				   "and   a.segment_desc = b.index_seg_desc ", TID);
	}else {
       sprintf(query, "select nvl(sum(a.EXTENT_TOTAL_COUNT),0) "
				   "from v$segment a, v$index b "
				   "where a.space_id = %ld "
				   "and   a.table_oid = b.table_oid "
				   "and   a.segment_desc = b.index_seg_desc ", TID);
	}


	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}
	SQLBindCol(stmt, 1, SQL_C_ULONG, &table_use, 0, NULL);
	while (1)
	{
		 rc = SQLFetch(stmt);
		 if (rc == SQL_NO_DATA) {
			 break;
		 } else if (rc != SQL_SUCCESS) {
			 dbErrMsg(env, dbc, stmt);
			 SQLFreeStmt(stmt, SQL_DROP);
			 freeDBHandle();
			 return ;
		 }

	}
	INDEX_USE->Text = table_use * page_size * extend_page_cnt;
	SQLFreeStmt(stmt, SQL_CLOSE);
    use2 = (double)(table_use * page_size * extend_page_cnt);


    // ����Ÿ���Ͽ��� �⺻���� ����
	sprintf(query, "select name, nextsize, maxsize, initsize, currsize, autoextend "
				   "from v$datafiles  "
				   "where spaceid = %ld" , TID);

    if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return ;
	}


	SQLBindCol(stmt, 1, SQL_C_CHAR,  dname, sizeof(dname), &dname_ind);
	SQLBindCol(stmt, 2, SQL_C_ULONG, &nextsize, 0, NULL);
	SQLBindCol(stmt, 3, SQL_C_ULONG, &maxsize, 0, NULL);
	SQLBindCol(stmt, 4, SQL_C_ULONG, &initsize, 0, NULL);
	SQLBindCol(stmt, 5, SQL_C_ULONG, &currsize, 0, NULL);
	SQLBindCol(stmt, 6, SQL_C_ULONG, &autoextend, 0, NULL);

	row = 1;
    max_sum = 0;
	while (1)
	{
         memset(dname, 0x00, sizeof(dname));
		 rc = SQLFetch(stmt);
		 if (rc == SQL_NO_DATA) {
			 break;
		 } else if (rc != SQL_SUCCESS) {
			 dbErrMsg(env, dbc, stmt);
			 SQLFreeStmt(stmt, SQL_DROP);
			 freeDBHandle();
			 return ;
		 }
		 DATAFILE->Cells[0][row] = dname;
		 DATAFILE->Cells[1][row] = currsize*page_size;
		 DATAFILE->Cells[2][row] = maxsize*page_size;
		 DATAFILE->Cells[3][row] = initsize*page_size ;
		 DATAFILE->Cells[4][row] = nextsize*page_size;
		 if (autoextend == 0) {
			DATAFILE->Cells[5][row] = "OFF" ;
		 }else{
            DATAFILE->Cells[5][row] = "ON" ;
		 }

         row++;
		 max_sum = max_sum + (double)(maxsize*page_size);
	}

	// tablespace�� ��뷮�� �׷�����.
    //rate1= 0;
	if (max_sum != 0) {
		rate1 = (double)(use1 + use2) / max_sum * 100.0;
	}else
		rate1 = 0;

	if (tname == "SYS_TBS_UNDO" || tname == "SYS_TBS_TEMP") {
        rate1 = (double)used / total * 100.0;
	}

	sprintf(temp, "%6.2f%% used", rate1 );
	RATE1->Caption = temp;
	ProgressBar1->Position = (int)rate1;
	ProgressBar1->Width = TBS_PANEL->Width;
	RATE1->Top = ProgressBar1->Top+ProgressBar1->Height;
	RATE1->Left = ProgressBar1->Left;
	
    // DATAFILE_GRID ����.
	DATAFILE->ColWidths[0] = 250;
	DATAFILE->ColWidths[1] = 150;
	DATAFILE->ColWidths[2] = 150;
	DATAFILE->ColWidths[3] = 150;
	DATAFILE->ColWidths[4] = 150;
	DATAFILE->ColWidths[5] = 150;

	if (row == 1) {
		DATAFILE->RowCount = 2;
	} else
        DATAFILE->RowCount = row;

	TBS_PANEL->Left = TNAMES->Left;
	TBS_PANEL->Top  = TNAMES->Top + TNAMES->Height;
    TBS_PANEL->Align = alClient;
	TBS_PANEL->Visible = true;
    DATAFILE->Visible = true;
	SQLFreeStmt(stmt, SQL_DROP);
	freeDBHandle();
}
//---------------------------------------------------------------------------
// TreeNode�� ����Ŭ���������� ó��.
void __fastcall TForm5::DBNODEDblClick(TObject *Sender)
{
    AnsiString x;
    TTreeNode *tNode = DBNODE->Selected;
	TTreeNode *pNode = NULL, *ppNode = NULL;

	ColGrid->Visible = false;
	IndexGrid->Visible = false;
	DataGrid->Visible = false;
	CRTTBL_PANEL->Visible = false;
	CRTTBL_BUTTON->Visible = false;
    CRTMEMO->Visible = false;
	FETCHPANEL->Visible = false;
	PROC->Visible = false;
	PROCPANEL->Visible = false;
	TBS_PANEL->Visible = false;

	//ShowMessage("step-1");
	// ������ Object�� USER�� ���ؾ߸� �Ѵ�.
	// ���� �� ���� ������Ʈ�̸� pNode = {tables, procedures, views, triggers�� 1��}
	//                            ppNode = {userName}
	if (tNode->Parent) {
		pNode = tNode->Parent;
	}

	if (pNode) {
		ppNode = pNode->Parent;
	}
	else
		return;

	//ShowMessage("step-2");
	// �������� �ʴ´ٸ� �ƹ��� �Ұ� ����.!!
	if (!pNode || !ppNode) {
        //ShowMessage("????");
		return;
	}

	if (pNode->Text == "Tables") {
		//���̺� �÷�,index������ �����ش�.
		TNAMES->Caption = ppNode->Text + "." + tNode->Text;
		getTableInfo( ppNode->Text, tNode->Text, SERVERNAME->Caption);
	}
	if (pNode->Text == "PERFORMANCE VIEW" || pNode->Text == "ADMIN VIEW") {
        TNAMES->Caption = SERVERNAME->Caption + "." + tNode->Text;
		selectObject( tNode->Text );
	}
	if (pNode->Text == "Procedures") {
		TNAMES->Caption = ppNode->Text + "." + tNode->Text;
		x = getProcedure( ppNode->Text, tNode->Text);
        displayObject2(x, 2);
	}
    if (pNode->Text == "Views") {
        TNAMES->Caption = ppNode->Text + "." + tNode->Text;
		x = getView( ppNode->Text, tNode->Text);
        displayObject2(x, 1);
	}
	if (pNode->Text == "Triggers") {
		TNAMES->Caption = ppNode->Text + "." + tNode->Text;
		x = getTrigger( ppNode->Text, tNode->Text);
        displayObject2(x, 1);
	}
	if (pNode->Text == "TABLESPACES") {
        TNAMES->Caption = ppNode->Text + "." + tNode->Text; 
		getTbsInfo(ppNode->Text, tNode->Text, SERVERNAME->Caption);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm5::Connect2Click(TObject *Sender)
{
    Connect1Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm5::DisConnect2Click(TObject *Sender)
{
    DisConnect1Click(this);    	
}
//---------------------------------------------------------------------------
// DBMS ����ڸ� �߰��ϴ� �޴��� ��������.
void __fastcall TForm5::AddUser1Click(TObject *Sender)
{
	TTreeNode *tNode = DBNODE->Selected;

	while (tNode->Parent)
	{
		tNode = tNode->Parent;
	}

	// find Root and if has child?
	if (tNode->HasChildren == false )
	{
		ShowMessage("Maybe, DSN not connected!!, First, connect ");
		return;
	}

	Form2->ShowModal();
}
//---------------------------------------------------------------------------
// ���� ���ó�� ȣ��.
void __fastcall TForm5::userAdd1Click(TObject *Sender)
{
    AddUser1Click(this);
}
//---------------------------------------------------------------------------
// ����� ����.
void __fastcall TForm5::DropUser2Click(TObject *Sender)
{
	TTreeNode *tNode = DBNODE->Selected;
	SQLHSTMT stmt;
	char query[1024];

	
	// ������ ��尡 tables�� ������ �ϰ� parent->parent�� NULL�̾ �Ѵ�.
	if (tNode->Parent->Parent != NULL)
	{
        ShowMessage("Maybe , You selected is not USER!!");
		return;    
	}
	if (tNode->getFirstChild())
	{
		if (tNode->getFirstChild()->Text != "Tables")
		{
            ShowMessage("Maybe , You selected is not USER!!");
			return;
		}

	}

	// �ѹ� �� ����ڿ��� Ȯ�ιް� ����..
	if (MessageBox(NULL, "Really Drop user ??", "Confirm", MB_OKCANCEL) == ID_CANCEL)
	{
		return;
	}

	if (!dbConnect(SERVERNAME->Caption))
	{
		return;
	}

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS) {
		ShowMessage("SQLAllocStmt Fail!!");
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();        
        return;
	}
	sprintf(query, "drop user %s cascade", tNode->Text.UpperCase().c_str());
	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS) {
		dbErrMsg(dbc, env, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return;
	}

	SQLFreeStmt(stmt, SQL_DROP);
	freeDBHandle();

	tNode->Delete();
	ShowMessage("Drop User Success!!");

}
//---------------------------------------------------------------------------

void __fastcall TForm5::dropUser1Click(TObject *Sender)
{
    DropUser2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm5::createTable1Click(TObject *Sender)
{
	TTreeNode *tNode = DBNODE->Selected, *xNode;
	int i = 0;

	// ���� ���õ� ���������� �����ؾ� �Ѵ�.
	if (tNode->Parent != NULL)
	{
		xNode = tNode->Parent;
		if (!xNode->Parent)
		{
			i = 1;
		}
	}

	if (i != 1)
	{
		ShowMessage("Must Select User(Owner) on TreeNode");
		return;    
	}

	targetUser->Text = tNode->Text;

	// Component Visible �����Ѵ�. �� �̷��� �������� �������� �ؾ� �Ѵ�.
	// �װ͵� �ſ� ������..
	ColGrid->Visible = false;
	IndexGrid->Visible = false;
    CRTMEMO->Visible = false;
	FETCHPANEL->Visible = false;
	PROC->Visible = false;
    PROCPANEL->Visible = false;
	CRTTBL_PANEL->Visible =true;
	CRTTBL_PANEL->Top = TNAMES->Top+TNAMES->Height;
	TBS_PANEL->Visible = false;

	CRTTBL_PANEL->Align = alTop;
	DataGrid->Align = alClient;
	DataGrid->ColCount = 6;
	DataGrid->RowCount = 2;
	DataGrid->FixedCols = 0;
	DataGrid->FixedRows = 1;
	DataGrid->Cells[0][0] = "ColumnName";DataGrid->ColWidths[0] = 150;
	DataGrid->Cells[1][0] = "ColumnType";DataGrid->ColWidths[1] = 100;
	DataGrid->Cells[2][0] = "Precision"; DataGrid->ColWidths[2] = 100;
	DataGrid->Cells[3][0] = "Scale";     DataGrid->ColWidths[3] = 100;
	DataGrid->Cells[4][0] = "NullAble";  DataGrid->ColWidths[4] = 100;
	DataGrid->Cells[5][0] = "Default Value"; DataGrid->ColWidths[5] = 100;
	for (i = 0; i < 6; i++) {
		 DataGrid->Cells[i][1] = "";
	}
	
	DataGrid->Visible = true;
	
	CRTTBL_BUTTON->Visible = true;
	CRTTBL_BUTTON->Align = alBottom;
}
//---------------------------------------------------------------------------
// ���̺�����޴��� ��������.
void __fastcall TForm5::Button1Click(TObject *Sender)
{
	int i = DataGrid->Row;

	// check validation
	if (CNAME->Text.Length() == 0)
	{
		ShowMessage("Column Name too short to use");
		return;
	}
	if (CTYPE->Text.Length() == 0)
	{
		ShowMessage("Column Type not defined");
		return;
	}

	//����üũ
	if (
		 CTYPE->Text == "char"    || CTYPE->Text == "varchar" ||
		 CTYPE->Text == "numeric" || CTYPE->Text == "number"
	   )
	{
		if ((int)atoi(CPRE->Text.c_str()) == 0)
		{
			ShowMessage("must input column precision.(Value > 0)");
			return;  
		}   
	}
	if (
		 CTYPE->Text == "numeric" || CTYPE->Text == "number"
	   )
	{
		if (CSCALE->Text.Length() == 0)
		{
			ShowMessage("must input column scale. (Value >= 0)");
			return;
		}
	}

    if (ISNULL->Text.Length() == 0)
	{
		ISNULL->Text = "NULL";
	}

	// add Row
	DataGrid->Cells[0][i] = CNAME->Text;
	DataGrid->Cells[1][i] = CTYPE->Text;
	DataGrid->Cells[2][i] = CPRE->Text;
	DataGrid->Cells[3][i] = CSCALE->Text;
	DataGrid->Cells[4][i] = ISNULL->Text;
	DataGrid->Cells[5][i] = CDEFAULT->Text;

	// GridRow �� ������Ų��. ��, ��������..
	if (DataGrid->Cells[0][DataGrid->RowCount-1].TrimLeft().Length() != 0)
	{
	   DataGrid->RowCount++;
	}
	DataGrid->Row = DataGrid->RowCount-1;

	CNAME->Text = "";
	CTYPE->Text = "";
	CPRE->Text = "";
	CSCALE->Text = "";
	ISNULL->Text = "";
	CDEFAULT->Text = "";
}
//---------------------------------------------------------------------------
// �׳� �ϳ� �����ΰ� ����.
int __fastcall TForm5::_ExecDirect(char *sql)
{
	SQLHSTMT stmt;

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS) {
		ShowMessage("SQLAllocStmt Fail!!");
		return 0;
	}

	if (SQLExecDirect(stmt, sql, SQL_NTS) != SQL_SUCCESS) {
		dbErrMsg(dbc, env, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
        //ShowMessage(sql);
		return 0;
	}

	SQLFreeStmt(stmt, SQL_DROP);
    return 1;
}
//---------------------------------------------------------------------------
// �־��� �ܾ���� ������� username�� object�� ������ ������ �Ѵ�.
AnsiString __fastcall TForm5::ExecLineSQL(AnsiString sql)
{
    TTreeNode *tNode = DBNODE->Selected;
	AnsiString return_str = "";
	char query[64000];
	int tok_count = 0, i, j, k, p, rc;
    AnsiString tok[8192];

    // ã�� SQL������ table���� user�� ��õ� ���� ������ ���� �����Ѵ�.
	tok_count = 0;
	p = 1;
	for (k = 1; k <= sql.Length(); k++)
	{
		if ( sql.SubString(k, 1) == " "
		   &&  sql.SubString(p, k - p).TrimLeft().TrimRight().Trim() != " "
		   &&  sql.SubString(p, k - p).TrimLeft().TrimRight().Trim().Length() >= 1)
		{
			tok[tok_count] = sql.SubString(p, k - p) ;
			p = k;
			tok_count++;
		}
		//p = k;
	}
	// �������� �� ���� ������ ������ �ѹ� �� ��ū�� ������ �Ѵ�.
	tok[tok_count] = sql.SubString(p, k - p);
	tok_count++;
	//ShowMessage(tok_count);
	// �������� �Ϻ��� �ٽ� ������Ѵ�.
	// ��, ���������� ã�� user�� add����� �Ѵ�. �� �̷��ڵ� �ȴ�.
	memset(query, 0x00, sizeof(query));
	for (k=0; k <tok_count; k++)
	{
		// create table
		if (tok[0].Trim() == "create" && tok[1].Trim() == "table")
		{
			rc = tok[2].AnsiPos(".");
			if (rc == 0 && k == 2) {
				strcat(query, tNode->Text.c_str());
				strcat(query, ".");
			}
		}
		// drop table
		if (tok[0].Trim() == "drop" && tok[1].Trim() == "table")
		{
			rc = tok[2].AnsiPos(".");
			if (rc == 0 && k == 2) {
				strcat(query, tNode->Text.c_str());
				strcat(query, ".");
			}
		}
		// create index name
		if (tok[0].Trim() == "create" && tok[1].Trim() == "index")
		{
			rc = tok[2].AnsiPos(".");
			if (rc == 0 && k == 2) {
				strcat(query, tNode->Text.c_str());
				strcat(query, ".");
			}
		}
		// create index table
		if (tok[0].Trim() == "create" && tok[1].Trim() == "index" && tok[3].Trim() == "on")
		{
			rc = tok[4].AnsiPos(".");
			if (rc == 0 && k == 4) {
				strcat(query, tNode->Text.c_str());
				strcat(query, ".");
			}
		}
		// alter table
		if (tok[0].Trim() == "alter" && tok[1].Trim() == "table" && tok[3].Trim() == "add")
		{
			rc = tok[2].AnsiPos(".");
			if (rc == 0 && k == 2) {
				strcat(query, tNode->Text.c_str());
				strcat(query, ".");
			}
		}
		// alter table constraints
		if (tok[0].Trim() == "alter" && tok[1].Trim() == "table" && tok[3].Trim() == "add" &&
			tok[4].Trim() == "constraint")
		{
			rc = tok[5].AnsiPos(".");
			if (rc == 0 && k == 5) {
				strcat(query, tNode->Text.c_str());
				strcat(query, ".");
			}
		}

		strcat(query, tok[k].Trim().c_str());
		strcat(query, " ");
	}
	return_str = query;
    return return_str;
	
}
//---------------------------------------------------------------------------
void __fastcall TForm5::Button2Click(TObject *Sender)
{
	TTreeNode *tNode = DBNODE->Selected, *xNode;
	SQLHSTMT stmt;
	char query[8192];
	char cols[1024];
	int i, j, k, rc, start, p;
    AnsiString sql;



	// DB execute
	if (!dbConnect(SERVERNAME->Caption))
	{
		return;
	}

	// Memoâ���� ������ script�� ������ �̿��ؼ� createTable�� ����.
	if (CRTMEMO->Visible == true)
	{
		// 2���� ��λ����� �ִ�.
        // ��� �������� SQL���� �Ľ��Ұ��ΰ�?? �� TableNode tree������ ���??
        // script�������� �������� ����SQL���� �����Ҽ� ������ ����ϰ� �ڵ��ؾ� �Ѵ�.
        // token���� �����ϰ� token�� ù��° ����� �ٰŷ� �����غ���.
		// create...
		// drop ....
		// ����1] select���� select��..���?? ==> ( �ڿ� ������ select�� ����ó��..
		// ����2] ������ SYS user�� �ߴµ� Ÿ �������� DDL�� �ϴµ� ��ũ��Ʈ������ user������ ����.
		// ==> �����?  table�� ������ "." ������ ã�� ������ ���� ������ user�� �߰�..
        // ��Ģ������ isql�� ���� �Ѵٸ�??  ���� �����ٵ�.. �ִ°� ���Ϸ� ����?

		j = 0; start = 1;
		for (i = 1; i < CRTMEMO->Text.Length(); i++)
		{
			if (CRTMEMO->Text.SubString(i, 4).LowerCase() == "drop"   ||
				CRTMEMO->Text.SubString(i, 6).LowerCase() == "create" ||
				CRTMEMO->Text.SubString(i, 5).LowerCase() == "alter"  )
			{
				if (j == 1) {
					sql = CRTMEMO->Text.SubString(start, (i - start));

					// ã�� SQL������ �ǵ��� �����ݷ��� ������ ���ش�.
					sql = sql.TrimLeft().TrimRight().LowerCase();
					if (sql.SubString(sql.Length(), 1) == ';') {
						sql = sql.SubString(1, sql.Length()-1);
					}

                    // user������..
                    sql = ExecLineSQL(sql);
                    ShowMessage(sql);
					_ExecDirect(sql.c_str());
					start = i;
				}else{
                    j = 1;
                    start = i;
				}
			}
		}
        sql = CRTMEMO->Text.SubString(start, (i - start));
		sql = sql.TrimLeft().TrimRight();
		if (sql.SubString(sql.Length(), 1) == ';') {
			sql = sql.SubString(1, sql.Length()-1);
		}
        // user������..
		sql = ExecLineSQL(sql);
        ShowMessage(sql);
		_ExecDirect(sql.c_str());
	}
	else {
		// üũ�׸��� ColGrid�� �̿��� �����ÿ��� �Ѵ�.
		if (CRTNAME->Text.Length() == 0)
		{
			ShowMessage ("Input Table name you create!");
			freeDBHandle();
			return;
		}
		if (DataGrid->Cells[0][1].Length() == 0)
		{
			ShowMessage ("Column Count is 0, input Column");
            freeDBHandle();
			return;
		}
		// create script
		sprintf(query, "create table %s.%s ( ", tNode->Text.c_str(), CRTNAME->Text.c_str());
		for (i = 1; i < DataGrid->RowCount; i++)
		{
			if (DataGrid->Cells[0][i].Length() == 0) {
				continue;
			}
		
			if (DataGrid->Cells[1][i] == "char" || DataGrid->Cells[1][i] == "varchar")
			{
				sprintf(cols, " %s %s (%s) ",   DataGrid->Cells[0][i].c_str(),
												DataGrid->Cells[1][i].c_str(),
												DataGrid->Cells[2][i].c_str());
			}
			if (DataGrid->Cells[1][i] == "number" || DataGrid->Cells[1][i] == "numeric")
			{
				sprintf(cols, " %s %s (%s, %s) ",   DataGrid->Cells[0][i].c_str(),
													DataGrid->Cells[1][i].c_str(),
													DataGrid->Cells[2][i].c_str(),
													DataGrid->Cells[3][i].c_str());
			}
			if (DataGrid->Cells[1][i] == "integer" || DataGrid->Cells[1][i] == "double" ||
				DataGrid->Cells[1][i] == "date")
			{
				sprintf(cols, " %s %s ",   DataGrid->Cells[0][i].c_str(),
										DataGrid->Cells[1][i].c_str());
			}

			if (DataGrid->Cells[5][i].Length() != 0)
			{
				strcat (cols, " default ");
				if (DataGrid->Cells[1][i] == "char" || DataGrid->Cells[1][i] == "varchar")
					strcat (cols, "'");
				strcat (cols, DataGrid->Cells[5][i].c_str());
				if (DataGrid->Cells[1][i] == "char" || DataGrid->Cells[1][i] == "varchar")
					strcat (cols, "'");
	
				strcat (cols, " ");
			}
			strcat (cols, DataGrid->Cells[4][i].c_str());
			strcat (cols, ",");
			strcat (query, cols);
		}
		i = strlen(query);
		query[i-1] = ')'; query[i] = 0x00;

        // Grid Refresh
		for (i = 1; i < DataGrid->RowCount; i++)
		{
			for (j=0; j< DataGrid->ColCount; j++)
			{
                DataGrid->Cells[j][i] = "";
			}
		}
        DataGrid->RowCount = 2;
		CRTNAME->Text = "";

		// SQL���� ����ó���Ѵ�.
		rc = _ExecDirect(query);
		if (rc == 1) {
           ShowMessage("Create Table Success!!"); 
		}
		
    }
    
	xNode = DBNODE->Selected->getFirstChild() ;
	BuildTableTree(xNode, DBNODE->Selected->Text);
	xNode->Expand(true);
    freeDBHandle();
}
//---------------------------------------------------------------------------
// ���Ϸ� �о ȭ�鿡 �Ѹ���.
void __fastcall TForm5::Button3Click(TObject *Sender)
{
	CRTMEMO->Clear();

    TBS_PANEL->Visible = false;
	FETCHPANEL->Visible = false;
	DataGrid->Visible = false;
	CRTTBL_PANEL->Visible = false;
	CRTMEMO->Visible = true;
	CRTMEMO->Top = TNAMES->Top + TNAMES->Height;
	CRTMEMO->Align = alClient;
	PROC->Visible =false;
	PROCPANEL->Visible = false;

	if (!OpenDialog1->Execute())
	{
		CRTMEMO->Visible=false;
		DataGrid->Top = TNAMES->Top + TNAMES->Height;
		CRTTBL_PANEL->Visible = true;
		CRTTBL_PANEL->Align = alTop;
		DataGrid->Align = alClient;
		DataGrid->Visible=true;
		return;
	}


	CRTMEMO->Lines->LoadFromFile(OpenDialog1->FileName);

}
//---------------------------------------------------------------------------
void __fastcall TForm5::Button4Click(TObject *Sender)
{
     TBS_PANEL->Visible = false;
	 PROCPANEL->Visible = false;
     PROC->Visible = false;
	 CRTMEMO->Visible=false;
     FETCHPANEL->Visible = false;
	 DataGrid->Top = TNAMES->Top + TNAMES->Height;
	 CRTTBL_PANEL->Visible = true;
	 CRTTBL_PANEL->Align = alTop;
	 DataGrid->Align = alClient;
	 DataGrid->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm5::droptable1Click(TObject *Sender)
{
	TTreeNode *tNode;
	SQLHSTMT stmt;
	char query[1024];
	int i;

	// ���õ� selection������ ���� �������� üũ
	if (!checkSelection())
		return;

	// ���õ� selection������ ���� ���̺�������.. üũ�ؾ� �Ѵ�.
    for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		if (!tNode->Parent) {
			ShowMessage("Selections must have only table-type Object.");
			return;
		}
		if (tNode->Parent->Text != "Tables")
		{
			ShowMessage("Selections must have only table-type Object.");
			return;
		}
	}

	if (MessageBox(NULL, "Really, Drop table?", "confirm", MB_OKCANCEL) == ID_CANCEL)
	{
		return;
	}

	// DB execute
	if (!dbConnect(SERVERNAME->Caption))
	{
		return;
	}

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS) {
		ShowMessage("SQLAllocStmt Fail!!");
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return;
	}

	//for (i = 0; i < DBNODE->SelectionCount; i++)
	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		//ShowMessage(tNode->Text);
		// �ش� ���� ������ ã�Ƴ��� �Ѵ�.!! ��¥�� ������ �ɸ��� ������ �����ϱ�..
		sprintf(query, "drop table %s.%s", tNode->Parent->Parent->Text.c_str(), tNode->Text.c_str());
		if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
		{
			dbErrMsg(dbc, env, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			freeDBHandle();
			ShowMessage(query);
			return;
		}
        DBNODE->Selections[i]->Delete();
	}


	SQLFreeStmt(stmt, SQL_DROP);
	freeDBHandle();


	ShowMessage("Drop Table Success!!");
}
//---------------------------------------------------------------------------
void __fastcall TForm5::truncate1Click(TObject *Sender)
{
	TTreeNode *tNode;
	SQLHSTMT stmt;
	char query[1024];
	int i;

    if (!checkSelection())
		return;

	if (MessageBox(NULL, "Really, truncate table?", "confirm", MB_OKCANCEL) == ID_CANCEL)
	{
		return;
	}

	// DB execute
	if (!dbConnect(SERVERNAME->Caption))
	{
		return;
	}

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail!!");
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return;
	}

    // ����ó���� �����ϵ���..
	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		// �ش� ���� ������ ã�Ƴ��� �Ѵ�.!! ��¥�� ������ �ɸ��� ������ �����ϱ�..
		sprintf(query, "truncate table %s.%s", tNode->Parent->Parent->Text.c_str(), tNode->Text.c_str());
		if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
		{
			dbErrMsg(dbc, env, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
			freeDBHandle();
			ShowMessage(query);
			return;
		}
	}

	SQLFreeStmt(stmt, SQL_DROP);
	freeDBHandle();

	ShowMessage("Truncate Table Success!!");
}
//---------------------------------------------------------------------------
// ���ڷ� ���� Target (����.object) �� ����Ÿ�� display�Ѵ�.
void __fastcall TForm5::selectObject(AnsiString target)
{
	char query[1024];
	int rc;
	int row;
	int maxLen[1024];
	SQLSMALLINT columnCount=0, nullable, dataType, scale, columnNameLength;
    int x, i;
    char columnName[255];
	void       **columnPtr;
	SQLINTEGER  *columnInd;
	unsigned long columnSize;
    char USER[41], PASSWD[41];
	AnsiString _emsg;


	// �Ʒ� ����������� dbc2�� �ٸ����� �ٲ��츦 ����ϰ� ���ٴ�
	// ��ȿ�� �����̳ĸ� ������ �Ѱǵ�.. ������ �ֳ�..
	// �ܼ��ϰ� �� �Լ��� ȣ��Ǿ��ٸ� ������ free��Ű�� ������̴�.!!
    // DSN���� �ѳ����� �ϱ� ������ �����ϰ� �ϴ� ��������.!!
	if (dbc2 != NULL)
	{
		SQLFreeStmt(stmt2, SQL_DROP);
		SQLDisconnect(dbc2);
		SQLFreeConnect(dbc2);
		SQLFreeEnv(env2);
		stmt2 = NULL; dbc2 = NULL; env2 = NULL;
	}

	if (SQLAllocEnv(&env2) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocEnv Fail");
		stmt2 = NULL; dbc2 = NULL; env2 = NULL;
		return;
	}

	if (SQLAllocConnect(env2, &dbc2) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocConnect Fail");
		SQLFreeEnv(env2);
		stmt2 = NULL; dbc2 = NULL; env2 = NULL;
		return;
	}
	// ���������� �����ɴϴ�..
	if (!Form5->getDsnInfo(SERVERNAME->Caption, "User",     USER))
	{
		ShowMessage("Can't Get User");
		SQLFreeConnect(dbc2);
		SQLFreeEnv(env2);
		stmt2 = NULL; dbc2 = NULL; env2 = NULL;
		return;
	}
	if (!Form5->getDsnInfo(SERVERNAME->Caption, "Password", PASSWD))
	{
		ShowMessage("Can't Get Password");
		SQLFreeConnect(dbc2);
		SQLFreeEnv(env2);
		stmt2 = NULL; dbc2 = NULL; env2 = NULL;
		return;
	}

	// ��¥ �����غ��ϴ�.
	if (SQLConnect(dbc2, SERVERNAME->Caption.c_str(), SQL_NTS, USER, SQL_NTS, PASSWD, SQL_NTS) != SQL_SUCCESS)
	{
		_emsg = "ConnecErr:" + SERVERNAME->Caption + "." + USER + "." +  PASSWD;
		ShowMessage(_emsg);
		SQLFreeConnect(dbc2);
		SQLFreeEnv(env2);
		stmt2 = NULL; dbc2 = NULL; env2 = NULL;
		return;
	}
	SQLFreeStmt(stmt2, SQL_DROP);
	if (SQLAllocStmt(dbc2, &stmt2) != SQL_SUCCESS) {
		ShowMessage("SQLAllocStmt Fail!!");
		SQLDisconnect(dbc2);
		SQLFreeStmt(stmt2, SQL_DROP);
		SQLFreeConnect(dbc2);
		SQLFreeEnv(env2);
		stmt2 = NULL; dbc2 = NULL; env2 = NULL;
		return;
	}
	sprintf(query, "alter session set select_header_display = 1");
	if (SQLExecDirect(stmt2, query, SQL_NTS) != SQL_SUCCESS)
	{
		SQLDisconnect(dbc2);
		SQLFreeStmt(stmt2, SQL_DROP);
		SQLFreeConnect(dbc2);
		SQLFreeEnv(env2);
		stmt2 = NULL; dbc2 = NULL; env2 = NULL;
		return;
	}
	SQLFreeStmt(stmt2, SQL_CLOSE);


	sprintf(query, "select * from %s", target.c_str());
	if (SQLExecDirect(stmt2, query, SQL_NTS) != SQL_SUCCESS) {
		dbErrMsg(env2, dbc2, stmt2);
		SQLDisconnect(dbc2);
		SQLFreeStmt(stmt2, SQL_DROP);
		SQLFreeConnect(dbc2);
		SQLFreeEnv(env2);
		stmt2 = NULL; dbc2 = NULL; env2 = NULL;
		ShowMessage(query);
		return;
	}

	SQLNumResultCols(stmt2, &columnCount);
	columnPtr = (void**) malloc( sizeof(void*) * columnCount );
	columnInd = (SQLINTEGER*) malloc( sizeof(SQLINTEGER) * columnCount );
	if ( columnPtr == NULL )
	{
		free(columnInd);
		SQLFreeStmt(stmt2, SQL_DROP);stmt2 = NULL;
		return ;
	}

	//�÷��� �Ѹ��鼭... Binding�� �ϰ� �޸𸮵� ���..
	for ( i=0; i<columnCount; i++ )
	{
		SQLDescribeCol(stmt2, i+1,
					   columnName, sizeof(columnName), &columnNameLength,
					   &dataType,
					   &columnSize,
					   &scale,
					   &nullable);

		columnPtr[i] = (char*) malloc( columnSize + 1 );
		SQLBindCol(stmt2, i+1, SQL_C_CHAR, columnPtr[i], columnSize+1, &columnInd[i]);
		DataGrid->Cells[i][0] = columnName;
		maxLen[i] = strlen(columnName);
	}

	row = 1;
	DataGrid->RowCount = 2;
	DataGrid->ColCount = columnCount;

   // ����Ÿ fetch�ϸ鼭 Grid�� �����Ѵ�. ������ gridSize �� ������..
	while (1)
	{
		 rc = SQLFetch(stmt2);
		 if (rc == SQL_NO_DATA) {
			break;
		 }
		 else if (rc != SQL_SUCCESS) {
			dbErrMsg(env2, dbc2, stmt2);
			SQLFreeStmt(stmt2, SQL_DROP);
			SQLFreeConnect(dbc2);
			SQLFreeEnv(env2);
			stmt2 = NULL; dbc2 = NULL; env2 = NULL;
            NEXTFETCH->Enabled = false;
			break;
		 }

		 for ( i=0; i<columnCount; i++ )
		 {
			if ( columnInd[i] == SQL_NULL_DATA )
			{
				DataGrid->Cells[i][row] = "";
				continue;
			}
			DataGrid->Cells[i][row] = (char*)columnPtr[i];
			if (columnInd[i] > maxLen[i]) {
				maxLen[i] = columnInd[i];
			}
		 }
		 row++;
		 if (row > 3000) {
			 break;
		 }
	}
	if (rc != SQL_SUCCESS)
	{
        SQLFreeStmt(stmt2, SQL_DROP);
		SQLDisconnect(dbc2);
		SQLFreeConnect(dbc2);
		SQLFreeEnv(env2);
		stmt2 = NULL; dbc2 = NULL; env2 = NULL;
		NEXTFETCH->Enabled = false;
	}else {
        NEXTFETCH->Enabled = true;
   }
	
	// ����� �޸� ���� !!
	for ( i=0; i<columnCount; i++ )
	{
        free( columnPtr[i] );
	}
	free( columnPtr );
	free( columnInd );

	// Grid ũ�� , ��ġ �����ϱ�
	if (row == 1)
	{
		DataGrid->RowCount = 2;
		for (i = 0; i < columnCount; i++)
		{
			DataGrid->Cells[i][1] = "";
		}
	}else {
		DataGrid->RowCount = row;
	}

	for (i = 0; i < columnCount; i++)
	{
		if (maxLen[i] < 10) {
            maxLen[i] = 10;
		}
		DataGrid->ColWidths[i] = maxLen[i] * 8;
	}
	
	DataGrid->Top = TNAMES->Top + TNAMES->Height;
	DataGrid->Left = TNAMES->Left;
	DataGrid->Align=alClient;

    TBS_PANEL->Visible = false;
	ColGrid->Visible = false;
	IndexGrid->Visible = false;
	CRTMEMO->Visible = false;
	CRTTBL_PANEL->Visible = false;
    PROC->Visible = false;
    PROCPANEL->Visible = false;
	DataGrid->Visible = true;
    FETCHPANEL->Align = alBottom;
    FETCHPANEL->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm5::selectTable1Click(TObject *Sender)
{

    // table������ ������+���̺���� ���ڷ� �����ش�.
    TNAMES->Caption = SERVERNAME->Caption + "." + DBNODE->Selected->Parent->Parent->Text + "." + DBNODE->Selected->Text;
	selectObject(DBNODE->Selected->Parent->Parent->Text + "."  + DBNODE->Selected->Text);
}
//---------------------------------------------------------------------------
// �ݵ�� selectObject���� Open�� �Ǿ� �־�� �Ѵ�.!!
void __fastcall TForm5::NEXTFETCHClick(TObject *Sender)
{
	int rc;
	int row, row2;
	SQLSMALLINT columnCount=0, nullable, dataType, scale, columnNameLength;
	int x, i;
    char columnName[255];
	void       **columnPtr;
	SQLINTEGER  *columnInd;
	unsigned long columnSize;
	AnsiString _emsg;
    
    // �׷����� ������ stmt2�� NULL�̶�� �׳� �����Ѵ�.
	if (stmt2 == NULL)
	{
        return;    
	}

	SQLNumResultCols(stmt2, &columnCount);
	columnPtr = (void**) malloc( sizeof(void*) * columnCount );
	columnInd = (SQLINTEGER*) malloc( sizeof(SQLINTEGER) * columnCount );
	if ( columnPtr == NULL )
	{
		free(columnInd);
		SQLFreeStmt(stmt2, SQL_DROP);stmt2 = NULL;
		return ;
	}

	//�÷��� �Ѹ��鼭... Binding�� �ϰ� �޸𸮵� ���..
	for ( i=0; i<columnCount; i++ )
	{
		SQLDescribeCol(stmt2, i+1,
					   columnName, sizeof(columnName), &columnNameLength,
					   &dataType,
					   &columnSize,
					   &scale,
					   &nullable);

		columnPtr[i] = (char*) malloc( columnSize + 1 );
		SQLBindCol(stmt2, i+1, SQL_C_CHAR, columnPtr[i], columnSize+1, &columnInd[i]);
	}

    row = 1;
    row2 = DataGrid->RowCount;
	// ����Ÿ fetch�ϸ鼭 Grid�� �����Ѵ�. ������ gridSize �� ������..
	while (1)
	{
		 rc = SQLFetch(stmt2);
		 if (rc == SQL_NO_DATA) {
			break;
		 }
		 else if (rc != SQL_SUCCESS) {
			dbErrMsg(env2, dbc2, stmt2);
			SQLFreeStmt(stmt2, SQL_DROP);
            SQLDisconnect(dbc2);
			SQLFreeConnect(dbc2);
			SQLFreeEnv(env2);
			stmt2 = NULL; dbc2 = NULL; env2 = NULL;
			break;
		 }

		 for ( i=0; i<columnCount; i++ )
		 {
			if ( columnInd[i] == SQL_NULL_DATA )
			{
                DataGrid->Cells[i][row2] = "";
				continue;
			}
			DataGrid->Cells[i][row2] = (char*)columnPtr[i];
		 }
		 row++; row2++;
		 if (row > 3000) {
			 break;
		 }
	}
	if (rc != SQL_SUCCESS)
	{
        SQLFreeStmt(stmt2, SQL_DROP);
		SQLDisconnect(dbc2);
		SQLFreeConnect(dbc2);
        SQLFreeEnv(env2);
		stmt2 = NULL; dbc2 = NULL; env2 = NULL;
        NEXTFETCH->Enabled = false;
	}else {
        NEXTFETCH->Enabled = true;
   }
	
	// ����� �޸� ���� !!
	for ( i=0; i<columnCount; i++ )
	{
        free( columnPtr[i] );
	}
	free( columnPtr );
	free( columnInd );

	DataGrid->RowCount = row2;
}
//---------------------------------------------------------------------------
void __fastcall TForm5::selectView1Click(TObject *Sender)
{

    // table������ ������+VIEW�̸���  ���ڷ� �����ش�.
	selectObject(DBNODE->Selected->Parent->Parent->Text + "."  + DBNODE->Selected->Text);
}
//---------------------------------------------------------------------------
void __fastcall TForm5::Button6Click(TObject *Sender)
{
	FILE *fp;
	int i, j;
    char col_del[255], row_del[255];

	if (!SaveDialog1->Execute()) {
		return;
	}

    // DataGrid�� �Ⱥ��̰� �޸����� ���̴� ��Ȳ�̶�� �����Ѵ�.
	if (DataGrid->Visible == false)
	{
		if (CRTMEMO->Visible == true)
		{
			CRTMEMO->Lines->SaveToFile(SaveDialog1->FileName);
			ShowMessage("File Save Success");
			return;
		}
        return;
	}

    // �����ڴ� õ�� �ɼ����Ϸ� ������ �ҵ� �ϴ�.
    fp = fopen("delimeter.conf", "r");
	if (fp == NULL) {
		ShowMessage("NO Delimeter: setup option, click [option]->[delimeter]");
		return;
	}
	memset(col_del, 0x00, sizeof(col_del));
	memset(row_del, 0x00, sizeof(row_del));
	fscanf(fp ,"%s %s", col_del, row_del);
	fclose(fp);

	fp = fopen(SaveDialog1->FileName.c_str(), "w+");
	if (fp == NULL)
	{
		ShowMessage("File Create Error, check diskspace or path");
		return;
	}

	for (i = 0; i < DataGrid->RowCount; i++)
	{
		for (j = 0; j < DataGrid->ColCount; j++)
		{
			fprintf(fp, "%s%s", DataGrid->Cells[j][i].c_str(), col_del);
		}
		fprintf(fp, "%s\n", row_del);
		FETCHPANEL->Caption = i + " row saved";
	}
	fflush(fp);
	fclose(fp);

    ShowMessage("File Save Success!!");
}
//---------------------------------------------------------------------------
void __fastcall TForm5::Delimeter1Click(TObject *Sender)
{
    Form3->ShowModal();
}
//---------------------------------------------------------------------------
// ����� ���� �� �̺�Ʈ�� ���߿� ���� ���������ұ� �ϴ� �ǹ��� ��..
// 1. TableColumn ���� ����
// 2. Pk������ �о��.
// 3. index������ �����ϵ�, PK�� ��ġ�� ������ pk���������� ������ �ξ���
// 4. foreignkey������ �����ϴµ�. �ٷ� ����� �Ұ��������� �о���� ���Ŀ� ����.
void __fastcall TForm5::tableScript(AnsiString _user, AnsiString _target)
{
	SQLHSTMT     stmt = NULL;
	SQLRETURN    rc;
	SQLCHAR  szCatalog[255];
	SQLINTEGER cbCatalog;
	AnsiString tmp;
	char sql[8192], tmp2[1024], oldName[255],oldszFKTableName[255];
	char _tmpFkColumn[30][255], _tmpPkColumn[30][255];
	int i, rcount;

	memset(_tmpFkColumn, 0x00, sizeof(_tmpFkColumn));
	memset(_tmpPkColumn, 0x00, sizeof(_tmpPkColumn));
	memset(oldszFKTableName, 0x00, sizeof(oldszFKTableName));

    TBS_PANEL->Visible = false;
	ColGrid->Visible = false;
	IndexGrid->Visible = false;
	DataGrid->Visible = false;
	CRTTBL_PANEL->Visible = false;
	CRTTBL_BUTTON->Visible = false;
	PROC->Visible = false;
    
	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return ;
	}

	// �÷� ������ ����ü�� �����Ѵ�.
	getColumnInfo(_user, _target);

	CRTMEMO->Lines->Add("drop table " + _target + ";");
	CRTMEMO->Lines->Add("create table " + _target);
	CRTMEMO->Lines->Add("(");

	for (i = 0; i < ColumnCount; i++) 
	{
		 if (memcmp(ColumnNode[i].ColumnType, "CHAR", 4) == 0 ||
			 memcmp(ColumnNode[i].ColumnType, "VARCHAR", 7) == 0)
		 {
			sprintf(tmp2, "%s %s (%d) ", ColumnNode[i].ColumnName, ColumnNode[i].ColumnType,
										 ColumnNode[i].Precision);
		 }
		 else if (memcmp(ColumnNode[i].ColumnType, "NUMBER", 6) == 0 ||
				  memcmp(ColumnNode[i].ColumnType, "NUMERIC", 7) == 0)
		 {
			sprintf(tmp2, "%s %s (%d, %d) ", ColumnNode[i].ColumnName, ColumnNode[i].ColumnType,
											 ColumnNode[i].Precision,  ColumnNode[i].Scale);
		 } else
		 {
			sprintf(tmp2, "%s %s ", ColumnNode[i].ColumnName, ColumnNode[i].ColumnType);
		 }

		 if (strlen(ColumnNode[i].DefaultValue) > 0)
		 {
			 strcat(tmp2, "default ");
			 /*if (memcmp(ColumnNode[i].ColumnType, "CHAR", 4) == 0 ||
				 memcmp(ColumnNode[i].ColumnType, "VARCHAR", 7) == 0)
			 {
				 strcat (tmp2, "'");
			 } */
			 strcat (tmp2, ColumnNode[i].DefaultValue);

			 /*if (memcmp(ColumnNode[i].ColumnType, "CHAR", 4) == 0 ||
				 memcmp(ColumnNode[i].ColumnType, "VARCHAR", 7) == 0)
			 {
				 strcat (tmp2, "'");
			 }*/
		 }
		 if (memcmp(ColumnNode[i].IsNull , "YES", 3) == 0)
			 strcat(tmp2, " NULL ");
		 else
			 strcat(tmp2, " NOT NULL ");

         // �÷��� ������ ������ �޸��� ������ �ȵȴ�.
		 if (i < (ColumnCount - 1))
			strcat(tmp2, ",");

		 CRTMEMO->Lines->Add(tmp2);
	}

	// �ش� ���̺����̽��� ��������� �˾Ƴ��� �Ѵ�.
	sprintf(sql, "select a.name "
				 "from v$tablespaces a, system_.sys_users_ b, system_.sys_tables_ c "
				 "where a.id = c.tbs_id "
				 "and b.user_id = c.user_id and b.user_name = '%s' "
				 "and c.table_name = '%s' ", _user.c_str(), _target.c_str());

	if (SQLExecDirect(stmt, sql, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return ;
	}
	SQLBindCol(stmt, 1, SQL_C_CHAR, szCatalog, 255,&cbCatalog);
	rc = SQLFetch(stmt);
	if (rc == SQL_NO_DATA)
	{
		sprintf(tmp2, ") tablespace SYS_TBS_MEMORY;");
	}else {
        sprintf(tmp2, ") tablespace %s;", szCatalog);
	}
	SQLFreeStmt(stmt, SQL_DROP);
	CRTMEMO->Lines->Add(tmp2);

	// �ش� ���̺��� ������ PK�� ���س��� �Ѵ�.
	getPKInfo(_user, _target);
   

	// ������ ���̺��� index������ ��� �����´�.
	getIndexInfo(_user, _target);

    memset(tmp2, 0x00, sizeof(tmp2));
	memset(oldName, 0x00, sizeof(oldName));
	for (i = 0; i < IndexCount; i++) 
	{

		//ShowMessage(IndexNode[i].IndexName);
        //ShowMessage(IndexNode[i].ColumnName);
		if (memcmp(oldName, IndexNode[i].IndexName, strlen(IndexNode[i].IndexName)) == 0 )
		{
			strcat(tmp2, IndexNode[i].ColumnName);
			if (IndexNode[i].AscDesc[0] == 'A')
			{
				strcat(tmp2, " ASC,");
			}
			else {
				strcat(tmp2, " DESC,");
			}
		} else {
            // ������ ���� indexName�� �ٸ��� ���������� ȭ�鿡 �Ѹ���.
            if (strlen(tmp2) > 0)
			{
				rc = strlen(tmp2);
				tmp2[rc-1] = ')';
				tmp2[rc] = ';';
				tmp2[rc+1] = 0x00;
				CRTMEMO->Lines->Add(tmp2);
                memset(tmp2, 0x00, sizeof(tmp2));
			}
            // SQLPrimary, SQLStatics�� pk������ ���� �ٸ���.??
            // �׷��� constraint���� ����� �̸��� �� ����.. �ӿ�...����??
			// ���� __SYS�̸� PK�� ���� szPkName�� ������ PK�ε� ����.
			if (memcmp(PKNAME, IndexNode[i].IndexName, strlen(IndexNode[i].IndexName)) == 0 ||
				memcmp(IndexNode[i].IndexName, "__SYS",  5) == 0) {
				if (memcmp(PKNAME, "__SYS", 5) == 0) {
					sprintf(tmp2, "alter table %s add primary key (%s ",
						_target.c_str(),  IndexNode[i].ColumnName);
				}else {
					sprintf(tmp2, "alter table %s add constraint %s primary key (%s ",
						_target.c_str(), PKNAME, IndexNode[i].ColumnName);
				}
			}
			// PK�� �ƴϸ� unique�� �ƴϳĸ� �����ؼ�..
			else if (IndexNode[i].Unique == 0) {
				sprintf(tmp2, "create unique index  %s on %s (%s ",
					IndexNode[i].IndexName, _target.c_str(), IndexNode[i].ColumnName);
			} else {
				sprintf(tmp2, "create index %s on %s (%s ",
					IndexNode[i].IndexName, _target.c_str(), IndexNode[i].ColumnName);
			}
			if (IndexNode[i].AscDesc[0] == 'A')
			{
				strcat(tmp2, "ASC,");
			}
			else {
				strcat(tmp2, "DESC,");
			}
		}
		memset(oldName, 0x00, sizeof(oldName));
        memcpy(oldName, IndexNode[i].IndexName, strlen(IndexNode[i].IndexName));
	}
	if (strlen(tmp2) > 0) {
		i = strlen(tmp2);
		tmp2[i-1] = ')';
		tmp2[i] = ';';
		tmp2[i+1] = 0x00;
		CRTMEMO->Lines->Add(tmp2);
	}

    // ���� ForeignKey, ������ PK
	getFKInfo(_user, _target);

    memset(oldName, 0x00, sizeof(oldName));
	memset(tmp2, 0x00, sizeof(tmp2));
	rcount=0;
	for (i = 0; i <FkCount; i++)
	{
		if (memcmp(oldName, FkNode[i].FKName, strlen(FkNode[i].FKName)) == 0 )
		{
			memcpy(_tmpPkColumn[rcount], FkNode[i].PKColumn, strlen(FkNode[i].PKColumn));
			memcpy(_tmpFkColumn[rcount], FkNode[i].FKColumn, strlen(FkNode[i].FKColumn));
			rcount++;
		}
		else {
			// �̰� �̷��� �ڵ��س��� ���߿� ��� debugging�ҷ�?? �� �����..
			// ���� �ϰ���..
			if (strlen(tmp2) > 0) {
				for (rc = 0; rc < rcount; rc++) {
					strcat(tmp2, _tmpFkColumn[rc]);
					strcat(tmp2, ",");
				}
				tmp2[strlen(tmp2)-1] = ')';
				strcat(tmp2, " references ");
				strcat(tmp2, oldszFKTableName);
                strcat(tmp2, " (");
				for (rc = 0; rc < rcount; rc++) {
					strcat(tmp2, _tmpPkColumn[rc]);
					strcat(tmp2, ",");
				}
				tmp2[strlen(tmp2)-1] = ')' ;
                strcat(tmp2, ";");
                rcount = 0;
				CRTMEMO->Lines->Add(tmp2);
                memset(oldszFKTableName, 0x00, sizeof(oldszFKTableName));
				memset(_tmpPkColumn, 0x00, sizeof(_tmpPkColumn));
				memset(_tmpFkColumn, 0x00, sizeof(_tmpFkColumn));
                memset(tmp2, 0x00, sizeof(tmp2));
			}
			memcpy(oldszFKTableName, FkNode[i].FKTable , strlen(FkNode[i].FKTable));
			sprintf(tmp2, "alter table %s add constraint %s foreign key (",
						   _target.c_str(), FkNode[i].FKName);
			memcpy(_tmpPkColumn[rcount], FkNode[i].PKColumn, strlen(FkNode[i].PKColumn));
			memcpy(_tmpFkColumn[rcount], FkNode[i].FKColumn, strlen(FkNode[i].FKColumn));
			rcount++;
		}
		memset(oldName, 0x00, sizeof(oldName));
		memcpy(oldName, FkNode[i].FKName, strlen(FkNode[i].FKName));
	}
	if (strlen(tmp2) > 0)
	{
		for (i = 0; i < rcount; i++) {
			strcat(tmp2, _tmpFkColumn[i]);
			strcat(tmp2, ",");
		}
		tmp2[strlen(tmp2)-1] = ')';
		strcat(tmp2, " references ");
		strcat(tmp2, oldszFKTableName);
		strcat(tmp2, " (");
		for (i = 0; i < rcount; i++) {
			strcat(tmp2, _tmpPkColumn[i]);
			strcat(tmp2, ",");
		}
		tmp2[strlen(tmp2)-1] = ')' ;
		strcat(tmp2, ";");
		CRTMEMO->Lines->Add(tmp2);
	}

	FETCHPANEL->Align = alBottom;
	FETCHPANEL->Visible = true;

	CRTMEMO->Top = TNAMES->Top+TNAMES->Height;
	CRTMEMO->Align = alClient;
	CRTMEMO->Visible = true;
    NEXTFETCH->Enabled = false;
}
//---------------------------------------------------------------------------
// Table�� ��ũ��Ʈ�� �����´�.  (����ó���� �����ϵ���..)
void __fastcall TForm5::ScriptOut1Click(TObject *Sender)
{
	TTreeNode *tNode;
	unsigned int i;

    if (!checkSelection())
		return;

	if (!dbConnect(SERVERNAME->Caption))
	{
		return;
	}
	CRTMEMO->Clear();
	//ShowMessage(DBNODE->SelectionCount );
	for (i = 0; i < DBNODE->SelectionCount; i++)
	{
		//ShowMessage(DBNODE->Selections[i]->Text);
		// tableScript�� ��������.
        tNode = DBNODE->Selections[i];
		CRTMEMO->Lines->Add("-----------------------------------------");
		CRTMEMO->Lines->Add("--" + tNode->Parent->Parent->Text + "." + tNode->Text);
		CRTMEMO->Lines->Add("-----------------------------------------");
		tableScript(tNode->Parent->Parent->Text, tNode->Text);
	}

	freeDBHandle();

}
//---------------------------------------------------------------------------
// ���̺� ��Ű���� ����Ѵ�.
void __fastcall TForm5::schemaOut2Click(TObject *Sender)
{
	TTreeNode *tNode = DBNODE->Selected;
	TTreeNode *xNode;
	AnsiString x;
	
    if (!dbConnect(SERVERNAME->Caption))
	{
		return;
	}

	CRTMEMO->Clear();
    // tables�� ����Ű�ϱ�...ũ...
	xNode = DBNODE->Selected->getFirstChild()->getFirstChild();
	while (xNode)
	{
		 CRTMEMO->Lines->Add("-----------------------------------------");
		 CRTMEMO->Lines->Add("--" + tNode->Text + "." + xNode->Text);
		 CRTMEMO->Lines->Add("-----------------------------------------");
		 tableScript(tNode->Text, xNode->Text);
		 xNode = xNode->GetNextChild(xNode);

	}
    
	freeDBHandle();

	// procedure ��ũ��Ʈ.
	xNode = DBNODE->Selected->getFirstChild() ;
	xNode = xNode->GetNextChild(xNode)->getFirstChild() ;
	//ShowMessage(xNode->Text);
	while (xNode)
	{
		 CRTMEMO->Lines->Add("-----------------------------------------");
		 CRTMEMO->Lines->Add("--" + tNode->Text + "." + xNode->Text);
		 CRTMEMO->Lines->Add("-----------------------------------------");
		 //tableScript(tNode->Text, xNode->Text);
		 x = getProcedure( tNode->Text, xNode->Text);
         CRTMEMO->Lines->Add(x + ";");
		 xNode = xNode->GetNextChild(xNode);

	}

    // View ��ũ��Ʈ.
	xNode = DBNODE->Selected->getFirstChild() ;
    xNode = xNode->GetNextChild(xNode);
	xNode = xNode->GetNextChild(xNode)->getFirstChild() ;
	//ShowMessage(xNode->Text);
	while (xNode)
	{
		 CRTMEMO->Lines->Add("-----------------------------------------");
		 CRTMEMO->Lines->Add("--" + tNode->Text + "." + xNode->Text);
		 CRTMEMO->Lines->Add("-----------------------------------------");
		 //tableScript(tNode->Text, xNode->Text);
		 x = getView( tNode->Text, xNode->Text);
         CRTMEMO->Lines->Add(x + ";");
		 xNode = xNode->GetNextChild(xNode);

	}

    // Trigger ��ũ��Ʈ.
	xNode = DBNODE->Selected->getFirstChild() ;
    xNode = xNode->GetNextChild(xNode);
    xNode = xNode->GetNextChild(xNode);
	xNode = xNode->GetNextChild(xNode)->getFirstChild() ;
	//ShowMessage(xNode->Text);
	while (xNode)
	{
		 CRTMEMO->Lines->Add("-----------------------------------------");
		 CRTMEMO->Lines->Add("--" + tNode->Text + "." + xNode->Text);
		 CRTMEMO->Lines->Add("-----------------------------------------");
		 //tableScript(tNode->Text, xNode->Text);
		 x = getTrigger( tNode->Text, xNode->Text);
         CRTMEMO->Lines->Add(x + ";");
		 xNode = xNode->GetNextChild(xNode);

	}
	//freeDBHandle();
}
//---------------------------------------------------------------------------
// procedure�� view �����ϱ�
void __fastcall TForm5::Button5Click(TObject *Sender)
{
	TTreeNode *tNode;
	int rc;

	if (!dbConnect(SERVERNAME->Caption)) {
		return;
	}

	rc = _ExecDirect((char*)PROC->Text.c_str());
	if (rc == 1)
	{
        ShowMessage("Compile Success!!");    
	}

	// ���� Ʈ���� child���� ��������Ŷ��
	if (DBNODE->Selected->Parent->Text == "Triggers") {
		tNode = DBNODE->Selected->Parent;
		BuildTriggerTree(DBNODE->Selected->Parent, DBNODE->Selected->Parent->Parent->Text);
		tNode->Expand(true);
	// ������������ �����ȰŶ��
	} 
	else if (DBNODE->Selected->Parent->Text == "Procedures") {
		tNode = DBNODE->Selected->Parent;
		BuildProcTree(DBNODE->Selected->Parent, DBNODE->Selected->Parent->Parent->Text);
		tNode->Expand(true);
	// ������������ �����ȰŶ��
	}
	else if (DBNODE->Selected->Parent->Text == "Views") {
		tNode = DBNODE->Selected->Parent;
		BuildViewTree(DBNODE->Selected->Parent, DBNODE->Selected->Parent->Parent->Text);
		tNode->Expand(true);
	// ������������ �����ȰŶ��
	} 
	freeDBHandle();
}
//---------------------------------------------------------------------------
// ���� ���� procedure�� ���Ϸ� �����Ѵ�.
void __fastcall TForm5::Button7Click(TObject *Sender)
{
	if (!SaveDialog1->Execute())
	{
		return;
	}
	PROC->Lines->SaveToFile(SaveDialog1->FileName);
    ShowMessage("File Save Success!!");
}
//---------------------------------------------------------------------------
// ���Ͽ��� ���� sql���� �����ִµ� CR+LF������ �Ӹ� ������..
void __fastcall TForm5::Button8Click(TObject *Sender)
{
	AnsiString x;
    int rc;

	if (!OpenDialog1->Execute())
	{
		return;
	}
	PROC->Lines->LoadFromFile(OpenDialog1->FileName);

	//displayObject2(PROC->Text, 2);
}
//---------------------------------------------------------------------------
void __fastcall TForm5::ReCompile3Click(TObject *Sender)
{
    TTreeNode *tNode;
	int rc, i;
	char query[1024];

    if (!checkSelection())
		return;

	if (!dbConnect(SERVERNAME->Caption)) {
		return;
	}

	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		// user.objectName
		sprintf(query, "alter procedure %s.%s compile", tNode->Parent->Parent->Text.c_str(),
														tNode->Text.c_str());
		rc = _ExecDirect(query);
		if (rc != 1) {
			ShowMessage("recompile Fail!!");
		}
	}

	freeDBHandle();
	ShowMessage("recompile Success!!");
}
//---------------------------------------------------------------------------

void __fastcall TForm5::ReCompile1Click(TObject *Sender)
{
	TTreeNode *tNode;
	int rc, i;
	char query[1024];

    if (!checkSelection())
		return;

	if (!dbConnect(SERVERNAME->Caption)) {
		return;
	}

    for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		// user.objectName
		sprintf(query, "alter view %s.%s compile",  tNode->Parent->Parent->Text.c_str(),
													tNode->Text.c_str());
		rc = _ExecDirect(query);
		if (rc != 1) {
			ShowMessage("recompile Fail!!");
		}
	}

	freeDBHandle();
	ShowMessage("recompile Success!!");
}
//---------------------------------------------------------------------------

void __fastcall TForm5::DropProcedure1Click(TObject *Sender)
{
	TTreeNode *tNode;
	int rc, i;
	char query[1024];

    if (!checkSelection())
		return;

	// ���õ� selection������ ���� ���̺�������.. üũ�ؾ� �Ѵ�.
	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		if (!tNode->Parent){
			ShowMessage("Selections must have only procedure-type Object.");
			return;
		}
		if (tNode->Parent->Text != "Procedures")
		{
			ShowMessage("Selections must have only procedure-type Object.");
			return;
		}
	}

	if (MessageBox(NULL, "Really Drop procedure??", "confirm", MB_OKCANCEL) == ID_CANCEL) {
		return;
	}

	if (!dbConnect(SERVERNAME->Caption)) {
		return;
	}

	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		// user.objectName
		sprintf(query, "drop procedure %s.%s", tNode->Parent->Parent->Text.c_str(),
											   tNode->Text.c_str());
		rc = _ExecDirect(query);
		if (rc != 1) {
			ShowMessage("Drop Procedure Fail!!");
		}
        tNode->Delete();
	}

	freeDBHandle();
	ShowMessage("Drop Procedure Success!!");
}
//---------------------------------------------------------------------------
// View����� ó��
void __fastcall TForm5::DropObject1Click(TObject *Sender)
{
	TTreeNode *tNode;
	int rc, i;
	char query[1024];

	if (!checkSelection())
		return;

	// ���õ� selection������ ���� ���̺�������.. üũ�ؾ� �Ѵ�.
	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		if (!tNode->Parent ) {
			ShowMessage("Selections must have only View-type Object.");
			return;
		}
		if (tNode->Parent->Text != "Views")
		{
			ShowMessage("Selections must have only View-type Object.");
			return;
		}
	}

	if (MessageBox(NULL, "Really Drop View??", "confirm", MB_OKCANCEL) == ID_CANCEL) {
		return;
	}

	if (!dbConnect(SERVERNAME->Caption)) {
		return;
	}

	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		// user.objectName
		sprintf(query, "drop view %s.%s",  DBNODE->Selected->Parent->Parent->Text.c_str(),
										   DBNODE->Selected->Text.c_str());
		rc = _ExecDirect(query);
		if (rc != 1) {
			ShowMessage("Drop view Fail!!");
		}

		tNode->Delete();
	}

	freeDBHandle();
	ShowMessage("Drop view Success!!");
}
//---------------------------------------------------------------------------

void __fastcall TForm5::winISQL1Click(TObject *Sender)
{
	Form4->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm5::System1Click(TObject *Sender)
{
    Form6->Show();	
}
//---------------------------------------------------------------------------
// ����ڰ� ���ν����� �׽�Ʈ�غ� �� �ְ� �Ѵ�.
void __fastcall TForm5::MakeexecuteProc(AnsiString uname, AnsiString pname)
{
	SQLHSTMT stmt;
	int rc;
	char ParaName[255], DataType[255];
	SQLSMALLINT IOType, Precision;
	SQLINTEGER cbParaName, cbIOType, cbPrecision, cbDataType;
    int row, outCount, i;
	char query[1024];
    char tmp[1024];

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return;
	}

	sprintf(query, "select a.para_name, a.inout_type, a.precision, "
				   "decode(A.data_type, 9 , 'DATE' ,               "
				   "					1 , 'CHAR' ,               "
				   "					4, 'INTEGER' ,            "
				   "					12 , 'VARCHAR',            "
				   "					8  , 'DOUBLE' ,            "
				   "					2  , 'NUMERIC' ,           "
				   "					5  , 'SMALLINT' ,          "
                   "                    6  , 'NUMBER'   ,          "
				   "					30 , 'BLOB'    ,           "
				   "					40 , 'CLOB'    ,           "
				   "					20002 , 'NIBBLE'  ,          "
				   "					-5 , 'BIGINT'  , 'UNKNOWN TYPE') as Type "
				   "from system_.sys_proc_paras_ a, system_.sys_users_ b, system_.sys_procedures_ c "
				   "where a.user_id = b.user_id and a.user_id = c.user_id "
				   "and   c.proc_name = '%s' and b.user_name = '%s' "
                   "and   a.proc_oid = c.proc_oid order by a.para_order ", pname.c_str(), uname.c_str());

	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return;
	}
    SQLBindCol(stmt, 1, SQL_C_CHAR, ParaName, 255, &cbParaName);
	SQLBindCol(stmt, 2, SQL_C_SSHORT, &IOType, 0,  &cbIOType);
	SQLBindCol(stmt, 3, SQL_C_SSHORT, &Precision, 0,  &cbPrecision);
	SQLBindCol(stmt, 4, SQL_C_CHAR, DataType, 255, &cbDataType);

	ProcGrid->ColCount = 4;
	ProcGrid->Cells[0][0] = "Param Name";
	ProcGrid->Cells[1][0] = "I/O Type";
	ProcGrid->Cells[2][0] = "Precision";
	ProcGrid->Cells[3][0] = "Value";
	ProcGrid->ColWidths[0] = 150;
	ProcGrid->ColWidths[1] = 100;
	ProcGrid->ColWidths[2] = 150;
	ProcGrid->ColWidths[3] = 300;
    row = 1;
    outCount=0;
	while (1)
	{
		memset(ParaName, 0x00, sizeof(ParaName));
        memset(DataType, 0x00, sizeof(DataType));
		rc = SQLFetch(stmt);
		if (rc == SQL_NO_DATA) {
			break;
		} else if (rc != SQL_SUCCESS) {
			dbErrMsg(env, dbc, stmt);
			SQLFreeStmt(stmt, SQL_DROP);
            return;
		}
		ProcGrid->Cells[0][row] = ParaName;
		if (IOType == 0) {
			ProcGrid->Cells[1][row] = "Input";
		}else {
            ProcGrid->Cells[1][row] = "Output";
            outCount++;
		}
        sprintf(tmp, "%s (%d)", DataType, Precision);
		ProcGrid->Cells[2][row] = tmp;
		row++;
	}

	// output�� 10�� �̻��� �����Ǹ� ���ϰ� ���´�.
	if (outCount >= 10)
	{
		ShowMessage("if Ouput Parameter > 10, can't execute!!");
		SQLFreeStmt(stmt, SQL_DROP);
		return;    
	}

	if (row == 1) {
		ProcGrid->RowCount = 2;
		ProcGrid->Cells[0][1] = "";
		ProcGrid->Cells[1][1] = "";
        ProcGrid->Cells[2][1] = "";
		ProcGrid->Cells[3][1] = "";
	}else {
        ProcGrid->RowCount = row;
		for (i = 1; i < ProcGrid->RowCount; i++) {
            ProcGrid->Cells[3][i] = "";
		}
	}
	ProcGrid->Top = TNAMES->Top + TNAMES->Height;
	ProcGrid->Align = alClient;
	ProcGrid->Visible = true;
    PROCPANEL->Visible = true;
	SQLFreeStmt(stmt, SQL_DROP);
}
//---------------------------------------------------------------------------
void __fastcall TForm5::estProcedure1Click(TObject *Sender)
{
	//ShowMessage("Not Yet");
	TTreeNode *tNode = DBNODE->Selected;


	// ���� Ʈ����忡�� ���ν����� ���õǾ��°��� ���� �Ѵ�.
	if (DBNODE->Selected->Level == 3)
	{
		if (DBNODE->Selected->Parent->Text != "Procedures") {
			ShowMessage("Selection is not Procedures.");
			return; 
		}   
	}else {
        ShowMessage("Selection is not Procedures.");
        return;
	}

	if (!dbConnect(SERVERNAME->Caption)) {
		return;
	}

		// ����ڸ�, ���ν�����
	TNAMES->Caption = tNode->Parent->Parent->Text + "." + tNode->Text;
	MakeexecuteProc(tNode->Parent->Parent->Text, tNode->Text);

	PROCPANEL->Align = alBottom;
	PROCPANEL->Visible = true;
	freeDBHandle();
}
//---------------------------------------------------------------------------

void __fastcall TForm5::alterTable1Click(TObject *Sender)
{
    ShowMessage("Not Yet");	
}
//---------------------------------------------------------------------------
// Ʈ���� �������� ó��
void __fastcall TForm5::ReCompile2Click(TObject *Sender)
{
	TTreeNode *tNode;
	int rc, i;
	char query[1024];

    if (!checkSelection())
		return;

	if (!dbConnect(SERVERNAME->Caption)) {
		return;
	}

    for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		// user.objectName
		sprintf(query, "alter trigger %s.%s compile",   tNode->Parent->Parent->Text.c_str(),
														tNode->Text.c_str());
		rc = _ExecDirect(query);
		if (rc != 1) {
			ShowMessage("recompile Fail!!");
		}
    }

	freeDBHandle();
	ShowMessage("recompile Success!!");
}
//---------------------------------------------------------------------------
// Ʈ���� dropó��
void __fastcall TForm5::dropTrigger1Click(TObject *Sender)
{
	TTreeNode *tNode;
	int rc, i;
	char query[1024];

    if (!checkSelection())
		return;

	// ���õ� selection������ ���� ���̺�������.. üũ�ؾ� �Ѵ�.
	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		if (!tNode->Parent) {
			ShowMessage("Selections must have only Trigger-type Object.");
			return;
		}
		if (tNode->Parent->Text != "Triggers")
		{
			ShowMessage("Selections must have only Trigger-type Object.");
			return;
		}
	}

	if (MessageBox(NULL, "Really Drop Trigger??", "confirm", MB_OKCANCEL) == ID_CANCEL) {
		return;
	}

	if (!dbConnect(SERVERNAME->Caption)) {
		return;
	}

    for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		// user.objectName
		sprintf(query, "drop trigger %s.%s",   tNode->Parent->Parent->Text.c_str(),
											   tNode->Text.c_str());
		rc = _ExecDirect(query);
		if (rc != 1) {
			ShowMessage("Drop Trigger Fail!!");
		}
		tNode->Delete();
	}

	freeDBHandle();
	ShowMessage("Drop Trigger Success!!");
}
//---------------------------------------------------------------------------
void __fastcall TForm5::createProcedure1Click(TObject *Sender)
{
    TTreeNode *tNode = DBNODE->Selected;
	int i = 0;

	// ���� ���õ� ���������� �����ؾ� �Ѵ�.
	if (tNode->Parent != NULL)
	{
	    if (tNode->Parent->Parent == NULL)
		{
			i = 1;
		}
	}

	if (i != 1)
	{
		ShowMessage("Must Select User(Owner) on TreeNode");
		return;    
	}


	// click���� �ʰ� ���� �־� ȭ�������� �ؾ���.
	CRTTBL_PANEL->Visible = false;
	CRTTBL_BUTTON->Visible = false;
	DataGrid->Visible = false;

	PROC->Top = TNAMES->Top + TNAMES->Height;
    PROC->Align = alClient;
    PROC->Text = "";
	PROC->Visible = true;
    PROCPANEL->Align = alBottom;
    PROCPANEL->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm5::createView1Click(TObject *Sender)
{
	createProcedure1Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm5::create1Click(TObject *Sender)
{
	createProcedure1Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm5::DBNODEGetImageIndex(TObject *Sender, TTreeNode *Node)
{
	if (Node->ImageIndex == -1)
        return;
}
//---------------------------------------------------------------------------
// Function execute!!
void __fastcall TForm5::_procFunc()
{
    SQLHSTMT stmt;
	int i, bindCount=0, ssize, outCount;
	char query[32000], chkQuery[1024];
	char outX[4096];
	int rc;
	long outXind;


	sprintf(query, "select %s(", TNAMES->Caption.c_str());

	//  ���� �����ع�����. �������ϱ�.
	bindCount = 0;
	for (i = 1; i < ProcGrid->RowCount; i++)
	{
		if (ProcGrid->Cells[1][i] == "Input")
		{
            strcat(query, ProcGrid->Cells[3][i].c_str());
			strcat(query, ",");
			bindCount++;
		}
	}

	// input���� ������ �ֳ� ���Ŀ� ����
	if (bindCount == 0) {
        strcat (query, ")");
	}else {
		i = strlen(query);
		query[i-1] = ')';
        query[i] = 0x00;
	}
    strcat(query, " from dual limit 1");

	//ShowMessage(query);
	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return;
	}

	sprintf(chkQuery, "select 1 from dual limit 1");
	if (SQLExecDirect(stmt, chkQuery, SQL_NTS) != SQL_SUCCESS)
	{
		sprintf(chkQuery, "create table dual (x char(1))");
		SQLExecDirect(stmt, chkQuery, SQL_NTS);
        SQLFreeStmt(stmt, SQL_CLOSE);
		sprintf(chkQuery, "insert into dual values ('x')");
        SQLExecDirect(stmt, chkQuery, SQL_NTS);
        SQLFreeStmt(stmt, SQL_CLOSE);
	}

	memset(outX, 0x00, sizeof(outX));
	// ���ุ �ϰ� procedure�� SQLFetch�� �ʿ����.
	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return;
	}
    SQLBindCol(stmt, 1, SQL_C_CHAR, outX, sizeof(outX), &outXind);
    SQLFetch(stmt);

	// �޽����ڽ��� �����ְ� ������.
    ShowMessage(outX);

    SQLFreeStmt(stmt, SQL_DROP);
}
//---------------------------------------------------------------------------
// procedure execute!!
void __fastcall TForm5::_procExec()
{
    SQLHSTMT stmt;
	int i, bindCount=0, ssize, outCount;
	char query[32000];
	char bindx[10][4096];
	char outX[10][4096];
	int rc;
	long outXind[10];
	int saveRow[10];


	sprintf(query, "exec %s (", TNAMES->Caption.c_str());

	//  ���� �����ع�����. �������ϱ�.
	bindCount = 0;
	for (i = 1; i < ProcGrid->RowCount; i++)
	{
		if (ProcGrid->Cells[1][i] == "Input")
		{
            strcat(query, ProcGrid->Cells[3][i].c_str());
            strcat(query, ",");
		}else {
            strcat(query, "?,");
		}
        bindCount++;
	}

	// input���� ������ �ֳ� ���Ŀ� ����
	if (bindCount == 0) {
        strcat (query, ")");
	}else {
		i = strlen(query);
		query[i-1] = ')';
        query[i] = 0x00;
	}
	//ShowMessage(query);
	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		return;
	}

	if (SQLPrepare(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
        SQLFreeStmt(stmt, SQL_DROP);
		return;
	}

	// ���ε��� ������ŭ input�� ���� output�� ���� �ؾ� �Ѵ�.
    // �������� char�� ���ε��ص� �ӵ��� ������ �Ǵ°� �ƴϴϱ� ���������..
    // ����, output���� Row�� ��ġ�� ����ص־� �Ѵ�.:saveRow
	outCount=0;
	for (i = 1; i < ProcGrid->RowCount; i++)
	{
		if (ProcGrid->Cells[1][i] == "Output")
		{
			SQLBindParameter(stmt, outCount+1,
							 SQL_PARAM_OUTPUT, SQL_C_CHAR,  SQL_CHAR,
							 0, 0,
							 outX[outCount],  sizeof(outX[outCount]),
							 &outXind[outCount]);
            saveRow[outCount] = i;
			outCount++;
		}
	}


	memset(outX, 0x00, sizeof(outX));
	// ���ุ �ϰ� procedure�� SQLFetch�� �ʿ����.
	if (SQLExecute(stmt) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		return;
	}

    // �����ص� Row��ġ�� ���� ���� ����Ѵ�.
	for (i = 0; i < outCount; i++)
	{
		ProcGrid->Cells[3][saveRow[i]] = outX[i];
	}

    SQLFreeStmt(stmt, SQL_DROP);
}
//---------------------------------------------------------------------------
void __fastcall TForm5::Button9Click(TObject *Sender)
{
	SQLHSTMT stmt;
	char query[1024];
    AnsiString uname, pname;
	int ret;
	long ret_ind;


    // ���� Ʈ����忡�� ���ν����� ���õǾ��°��� ���� �Ѵ�.
	if (DBNODE->Selected->Level == 3)
	{
		if (DBNODE->Selected->Parent->Text != "Procedures") {
			ShowMessage("Selection is not Procedures.");
			return; 
		}   
	}else {
        ShowMessage("Selection is not Procedures.");
        return;
	}


	if (ProcGrid->Visible == false)
	{
		estProcedure1Click(this);
		return;
	}

	if (!dbConnect(SERVERNAME->Caption)) {
		return;
	}

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail!!");
		freeDBHandle();
		return;
	}

	// ������ ȭ����� ��°���� ���� uname, pname�� �˾Ƴ���.
	// ��, �ָ��� �ڵ��̴�. ���߿� �̷��Ŵ� ���� ����� ã�ƺ���.
	uname = TNAMES->Caption.SubString(1, TNAMES->Caption.Pos(".")-1);
	pname = TNAMES->Caption.SubString(TNAMES->Caption.Pos(".")+1, TNAMES->Caption.Length()-TNAMES->Caption.Pos("."));

	sprintf(query, "select object_type from system_.sys_procedures_ "
				   "where  user_id = (select user_id from system_.sys_users_ where user_name = '%s') "
				   "and    proc_name = '%s' limit 1", uname.c_str(), pname.c_str());

	// �ϴ� �̰� Procedure����..   function���� �Ǵ��� �ؾ� �Ѵ�.
	// �ű⿡ �°� ó���Լ��� �и��ؾ� �Ѵ�.
	// procedure�� callable���·� , function�̸� select���·�
	// binding��ĵ� �Ѵ� �ٸ���.. �ӿ� �̰�..
	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return;    
	}
	SQLBindCol(stmt, 1, SQL_C_SSHORT, &ret, 0, &ret_ind);
    SQLFetch(stmt);
    SQLFreeStmt(stmt, SQL_DROP);
	if (ret == 0) {
		_procExec();
	} else {
		_procFunc();
	}
	
	freeDBHandle();
}
//---------------------------------------------------------------------------
// Ʈ������ ������ �κе��� ���� ������ �־�� �� �Ҽ� �ִ�.
bool __fastcall TForm5::checkSelection()
{
	TTreeNode *tNode;
    AnsiString rootDSN1, rootDSN2;
	unsigned int i;
	int level1, level2;

	// �����Ѱ� ������ �϶��� 
	if (DBNODE->SelectionCount > 1)
	{
		level1 = DBNODE->Selections[0]->Level;
		for (i = 1; i < DBNODE->SelectionCount; i++)
		{
			level2 = DBNODE->Selections[i]->Level;
			// level�� �̸� �˾Ҵ���� ������ ��������ٵ�.. ���..
			if (level1 != level2)
			{
				ShowMessage("Maybe, Selection of Object is not same-level. !!");
				return false;
			}
		}
		// ������ child���� DSN�� �ٸ��� �������� ����.
		tNode = DBNODE->Selections[0];
		while (tNode->Parent)
		{
			tNode = tNode->Parent;
		}
		rootDSN1 = tNode->Text;
        for (i = 1; i < DBNODE->SelectionCount; i++)
		{
			tNode = DBNODE->Selections[i];
			while (tNode->Parent)
			{
				tNode = tNode->Parent;
			}
			rootDSN2 = tNode->Text;
			if (rootDSN1 != rootDSN2)
			{
                ShowMessage("Maybe, Selection of Object is not same-DSN. !!");
				return false;
			}
        }
	}

	return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm5::DataGridDblClick(TObject *Sender)
{
	int i;
	// table���� ȭ���� Ȱ��ȭ �� ���¶�� �Ѵٸ�
	// ����Ŭ���� ���� �Է�ȭ������ �÷��ִ� ó��
	if (CRTTBL_PANEL->Visible == true)
	{
        i = DataGrid->Row;
		CNAME->Text = DataGrid->Cells[0][i] ;
		CTYPE->Text = DataGrid->Cells[1][i] ;
		CPRE->Text  = DataGrid->Cells[2][i] ;
		CSCALE->Text  = DataGrid->Cells[3][i] ;
		ISNULL->Text  = DataGrid->Cells[4][i] ;
		CDEFAULT->Text  = DataGrid->Cells[5][i] ;
	}
}
//---------------------------------------------------------------------------
// ���̺���������� �÷� ������ư�� �����ٸ� ó�� 
void __fastcall TForm5::Button10Click(TObject *Sender)
{
	int i = DataGrid->Row,  j;

	for (j=0; j < DataGrid->ColCount; j++)
	{
		DataGrid->Cells[j][i] = "";
	}

}
//---------------------------------------------------------------------------
// �ε����� �����ϴ� ȭ���� ����.
// ��ð� ���� �𸣱� ������ �ϴ� ���Ǹ� ���¡.
// longrunQuery�� �� ���ɼ��� ŭ���� ������ �и��Ѵ�.
void __fastcall TForm5::CreateIndex1Click(TObject *Sender)
{
	int i;

	Form7->ColGrid->Cells[0][0] = "ColumnName";
	Form7->ColGrid->Cells[1][0] = "ColumnType";
	Form7->ColGrid->Cells[2][0] = "Nullable";

	// ���� ���� ColumnGrid�� �����Ͻð�.
    Form7->ColGrid2->Clear();
	for (i = 1; i < ColGrid->RowCount; i++)
	{
		Form7->ColGrid2->Items->Add(ColGrid->Cells[0][i]);
	}

    // Index�÷��׸��带 clear���ش�.
	Form7->IndexGrid->Cells[0][0] = "IndexColumn";
    Form7->IndexGrid->Cells[1][0] = "Ordering";

	Form7->ColGrid->RowCount = ColGrid->RowCount;

    Form7->IndexGrid->RowCount = ColGrid->RowCount;
	for (i = 1; i < ColGrid->RowCount; i++)
	{
		Form7->IndexGrid->Cells[0][i] = "";
		Form7->IndexGrid->Cells[1][i] = "";

	}

	// ObjectName����
	Form7->TARGET->Text = TNAMES->Caption ;
	Form7->SERVERNAME->Text = SERVERNAME->Caption;
	Form7->USERS->Text = DBNODE->Selected->Parent->Parent->Text;

	Form7->ShowModal();

	DBNODEDblClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm5::DropIndex1Click(TObject *Sender)
{
    SQLHSTMT stmt;
	AnsiString IndexName = IndexGrid->Cells[0][IndexGrid->Row];
    AnsiString cMsg;
	char query[1024], uname[255];
	int rc;
	long uname_ind;
    SQLINTEGER uid, indexType;

	// ������ �Ǿ����� üũ�ϰ�
	if (IndexName.Trim().Length() == 0)
	{
		ShowMessage("I can't found indexName in RowValues you selected");
		return;
	}

	// �켱 Ȯ���ϰ�
	cMsg = "Really Drop Index [" + IndexName + "]";
	if (MessageBox(NULL, cMsg.c_str(), "Confirm", MB_OKCANCEL) == ID_CANCEL) {
        return;
	}

	if (!dbConnect(SERVERNAME->Caption))
	{
		return;
	}

	if (SQLAllocStmt(dbc, &stmt) != SQL_SUCCESS)
	{
		ShowMessage("SQLAllocStmt Fail");
		freeDBHandle();
        return;
	}


    // �ش� INDEX�� �����ڰ� ������ ã�ƾ߸� �Ѵ�.
	sprintf(query, "select a.user_name, a.user_id "
				   "from system_.sys_users_ a "
				   "where a.user_id = (select c.user_id from "
				   "                   system_.sys_tables_ b, system_.sys_indices_ c "
				   "                   where b.table_name = '%s' "
				   "                   and   c.index_name = '%s' "
				   "                   and   c.table_id = b.table_id) " ,
				   DBNODE->Selected->Text.Trim().c_str(),
				   IndexName.Trim().c_str() );
	if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return;
	}

	SQLBindCol(stmt, 1, SQL_C_CHAR, uname, 255, &uname_ind);
	SQLBindCol(stmt, 2, SQL_C_SLONG, &uid, 0, NULL);
	memset(uname, 0x00, sizeof(uname));
	rc = SQLFetch(stmt);
	if (rc == SQL_NO_DATA)
	{
		ShowMessage("who is owner??");
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return;
	}
	SQLFreeStmt(stmt, SQL_CLOSE);


   // �� �ε����� PK�� �׳� index�Ŀ� ���� �� ������ �ٸ��ϱ� Ȯ���ؾ� �Ѵ�.
   sprintf(query, "select a.constraint_type from system_.sys_constraints_ a , system_.sys_indices_ b "
				  "where a.user_id = b.user_id "
				  "and   a.table_id = b.table_id "
				  "and   a.index_id = b.index_id "
				  "and   b.index_name = '%s' "
                  "and   b.user_id    = %ld ", IndexName.Trim().c_str(), uid);
        
    if (SQLExecDirect(stmt, query, SQL_NTS) != SQL_SUCCESS)
	{
		dbErrMsg(env, dbc, stmt);
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return;
	}

	SQLBindCol(stmt, 1, SQL_C_SLONG, &indexType, 0, NULL);
	rc = SQLFetch(stmt);
	if (rc == SQL_NO_DATA)
	{
		ShowMessage("who is owner??");
		SQLFreeStmt(stmt, SQL_DROP);
		freeDBHandle();
		return;
	}
	SQLFreeStmt(stmt, SQL_DROP);


	if (indexType != 3) {
       // User + IndexName ã�� drop indexó���Ѵ�.
	   sprintf(query, "drop index %s.%s", uname,
									   IndexName.Trim().c_str());
	}else {
	   sprintf(query, "alter table %s.%s drop primary key ",
	                                    uname,
										DBNODE->Selected->Text.Trim().c_str());
	}

	rc = _ExecDirect(query);
	freeDBHandle();

	if (rc == 1) {
		ShowMessage("Drop Index Success!");
	}

	DBNODEDblClick(this);

}
//---------------------------------------------------------------------------

void __fastcall TForm5::createTable2Click(TObject *Sender)
{
    createTable1Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm5::createProcedure2Click(TObject *Sender)
{
	createProcedure1Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm5::createView2Click(TObject *Sender)
{
	createProcedure1Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm5::createTrigger1Click(TObject *Sender)
{
    createProcedure1Click(this);	
}
//---------------------------------------------------------------------------

void __fastcall TForm5::droptable2Click(TObject *Sender)
{
    droptable1Click(this);	
}
//---------------------------------------------------------------------------

void __fastcall TForm5::dropProcedure2Click(TObject *Sender)
{
    DropProcedure1Click(this);	
}
//---------------------------------------------------------------------------


void __fastcall TForm5::dropView1Click(TObject *Sender)
{
    DropObject1Click(this);    	
}
//---------------------------------------------------------------------------

void __fastcall TForm5::dropTrigger2Click(TObject *Sender)
{
    dropTrigger1Click(this);	
}
//---------------------------------------------------------------------------

void __fastcall TForm5::recompile4Click(TObject *Sender)
{
    TTreeNode *tNode, *xNode;
	SQLHSTMT stmt;
	char query[1024];
	int i;

	// ���õ� selection������ ���� �������� üũ
	if (!checkSelection())
		return;

	// ���õ� ù��°��尡 DB��?
	xNode = DBNODE->Selections[0];
	if (xNode->Parent == NULL)
	{
		ShowMessage("Selections is not DB-Object.[-1]");
		return;
	}

    // �׷� �� �θ� ó������� �³�?
	if (xNode->Parent->Text != "Procedures" &&
		xNode->Parent->Text != "Views" &&
		xNode->Parent->Text != "Triggers" )
	{
        ShowMessage("Selections is not Object to recompile.[-2]");
		return;
	}

	// ���õ� selection������ ���� ���� Object������.. üũ�ؾ� �Ѵ�.
    for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		if (xNode->Parent->Text != tNode->Parent->Text)
		{
		    ShowMessage("Selections must have only same-type Object.");
			return;
		}
	}

    // procedure, view, trigger object�� �°� recompileó���Լ��� ȣ���Ѵ�.
	if (xNode->Parent->Text == "Procedures"){
        ReCompile3Click(this);
	}else if (xNode->Parent->Text == "Views") {
        ReCompile1Click(this);      
	}else if (xNode->Parent->Text == "Triggers") {
        ReCompile2Click(this);          
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm5::createIndex2Click(TObject *Sender)
{
	TTreeNode *tNode = DBNODE->Selected;


	// ���̺����� üũ�ϰ� ����
    if (!tNode->Parent)
	{
        ShowMessage("Selections is not Table-Object.");
		return;
	}
	if (tNode->Parent->Text != "Tables")
	{
		ShowMessage("Selections is not Table-Object.");
		return;
	}

	TNAMES->Caption = tNode->Parent->Parent->Text + "." + tNode->Text;
	getTableInfo( tNode->Parent->Parent->Text, tNode->Text, SERVERNAME->Caption);
	CreateIndex1Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm5::schemaout3Click(TObject *Sender)
{
	TTreeNode *tNode = DBNODE->Selected;
	AnsiString x;

	//ShowMessage(tNode->Level);
	// USER-level�̴ϱ�..��ü�� �̵��� �Ѵ�.

	if (tNode->Level == 1) {
		schemaOut2Click(this);
	}else if (tNode->Level == 3) {
		if (tNode->Parent->Text == "Tables"){
            ScriptOut1Click(this);
		}else if (tNode->Parent->Text == "Procedures") {
			TNAMES->Caption = tNode->Parent->Parent->Text + "." + tNode->Text;
			x = getProcedure( tNode->Parent->Parent->Text, tNode->Text);
			displayObject2(x, 2);
		}else if (tNode->Parent->Text == "Views") {
			TNAMES->Caption = tNode->Parent->Parent->Text + "." + tNode->Text;
			x = getView( tNode->Parent->Parent->Text, tNode->Text);
			displayObject2(x, 1);
		}else if (tNode->Parent->Text == "Triggers") {
			TNAMES->Caption = tNode->Parent->Parent->Text + "." + tNode->Text;
			x = getTrigger( tNode->Parent->Parent->Text, tNode->Text);
			displayObject2(x, 1);
	    }
	}else {
		ShowMessage("Selections is not supported.!!");
		return;
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm5::selectdata1Click(TObject *Sender)
{
	if (DBNODE->Selected->Level == 3)
	{
		if (DBNODE->Selected->Parent->Text == "Tables" ||
			DBNODE->Selected->Parent->Text == "Views" )
		{
			selectTable1Click(this);
		}
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm5::testProcedure1Click(TObject *Sender)
{

	if (DBNODE->Selected->Level == 3) {
		if (DBNODE->Selected->Parent->Text == "Procedures")
		   estProcedure1Click (this);
		else
           ShowMessage("Selections is not Procedures.");
	}else {
		ShowMessage("Selections is not Procedures.");
	}

}
//---------------------------------------------------------------------------
// ���� aexport �۾��� ��׶���ó���ϴ� �Լ�
void __fastcall TForm5::execAexportOut(int i, AnsiString uname, AnsiString tname)
{
    char runx[1024];
    char USER[41], PASSWD[41], PORT[41];
	STARTUPINFO          _si;
	PROCESS_INFORMATION  _pi;
	TDateTime t;
	DWORD dwExitCode;
    char d_col[255], d_row[255];
	FILE *fp;


    // get delimeter info
	memset(d_col, 0x00, sizeof(d_col));
	memset(d_row, 0x00, sizeof(d_row));
    fp = fopen("delimeter.conf", "r");
	if (fp == NULL) {
		strcpy(d_col, "^^##") ;
		strcpy(d_row, "##^^");
	}else {
		fscanf(fp ,"%s %s", d_col, d_row);
		fclose(fp);
    }

	// TEMP��
	ZeroMemory( &_si, sizeof(STARTUPINFO) );
	ZeroMemory( &_pi, sizeof(PROCESS_INFORMATION) );

	_si.cb            = sizeof( STARTUPINFO );
	_si.dwFlags       = STARTF_USESHOWWINDOW;
	_si.wShowWindow   = SW_SHOWNORMAL;

    // ILOADER��
	ZeroMemory( &IloaderProcess[i].si, sizeof(STARTUPINFO) );
	ZeroMemory( &IloaderProcess[i].pi, sizeof(PROCESS_INFORMATION) );

	IloaderProcess[i].si.cb            = sizeof( STARTUPINFO );
	IloaderProcess[i].si.dwFlags       = STARTF_USESHOWWINDOW;
	IloaderProcess[i].si.wShowWindow   = SW_SHOWNORMAL;

	memset(USER,   0x00, sizeof(USER));
	memset(PASSWD, 0x00, sizeof(PASSWD));
	memset(PORT,   0x00, sizeof(PORT));

	Form5->getDsnInfo(SERVERNAME->Caption, "User",         USER);
	Form5->getDsnInfo(SERVERNAME->Caption, "Password",     PASSWD);
	Form5->getDsnInfo(SERVERNAME->Caption, "Port",         PORT);

    // for manager
	if (!DirectoryExists("FORM"))
	{
		CreateDir("FORM");
	}
	if (!DirectoryExists("DATA"))
	{
		CreateDir("DATA");
	}
	if (!DirectoryExists("LOG"))
	{
		CreateDir("LOG");
	}
	if (!DirectoryExists("BAD"))
	{
		CreateDir("BAD");
	}
	// FormOut :���������� ��ٷ��߸� �Ѵ�.
	sprintf(runx, "iloader_451 -S %s -U %s -P %s -PORT %s formout -T %s.%s -f FORM//%s_%s.fmt",
				   SERVERNAME->Caption.c_str(),
				   USER, PASSWD, PORT,
				   uname.c_str(), tname.c_str(),
				   uname.c_str(), tname.c_str()  );
    CreateProcess( NULL, runx, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, NULL,
	               &_si, &_pi );
    // ���ڵ� ��ü�� Ÿ�Ӱ��� ������ ����� �����غ��δ�.. no idea now..
	WaitForSingleObject(_pi.hProcess, INFINITE);
	

    // DataOut
	sprintf(runx, "iloader_451 -S %s -U %s -P %s -PORT %s out "
				  " -T %s.%s -f FORM//%s_%s.fmt -d DATA//%s_%s.dat "
				  " -log LOG//%s_%s.log -bad BAD//%s_%s.bad -t %s -r %s ",
				   SERVERNAME->Caption.c_str(),
				   USER, PASSWD, PORT,
				   uname.c_str(), tname.c_str(),
				   uname.c_str(), tname.c_str(),
				   uname.c_str(), tname.c_str(),
				   uname.c_str(), tname.c_str(),
				   uname.c_str(), tname.c_str(),
				   d_col, d_row  );

	CreateProcess( NULL, runx, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, NULL,
				   &IloaderProcess[i].si, &IloaderProcess[i].pi );

	memset(IloaderProcess[i].uname, 0x00, sizeof(IloaderProcess[i].uname));
	memset(IloaderProcess[i].tname, 0x00, sizeof(IloaderProcess[i].tname));

	memcpy(IloaderProcess[i].uname, uname.c_str(), uname.Length());
	memcpy(IloaderProcess[i].tname, tname.c_str(), tname.Length());
	sprintf(IloaderProcess[i].startTime, "%s,%s", t.CurrentDate().DateString().c_str(),
	                                              t.CurrentTime().TimeString().c_str());
    IloaderProcess[i].self = i;

	sprintf(runx, "%-10ld %15s.%-40s %-26s %-26s",
	                                      IloaderProcess[i].pi.dwProcessId,
										  IloaderProcess[i].uname,
										  IloaderProcess[i].tname,
										  IloaderProcess[i].startTime,
										  "running");

	Form8->CheckListBox1->Items->Add(runx);
	//ShowMessage(pi.dwProcessId);
}
//---------------------------------------------------------------------------
// iloader Process ����ü���� ������ ã��.
int findEmptyNode()
{
	 int i;
	 DWORD dwExitCode;

     // ��ü���� �׾� �ִ� ���� ã�Ƽ� ������.
	 for (i = 0; i < ILOADER_MAX_PROCESS; i++) {
		  //GetExitCodeProcess(IloaderProcess[i].pi.hProcess, &dwExitCode);
		  if (IloaderProcess[i].startTime[0] == 0x00)
		  {
			  return i;
		  }
	 }

	 return -1;
}
//---------------------------------------------------------------------------
// ���� aexport �۾��� ��׶���ó���ϴ� �Լ�
void __fastcall TForm5::execAexportIn(int i, AnsiString uname, AnsiString tname)
{
    char runx[1024];
    char USER[41], PASSWD[41], PORT[41];
	STARTUPINFO          _si;
	PROCESS_INFORMATION  _pi;
	TDateTime t;
	DWORD dwExitCode;
    char d_col[255], d_row[255];
	FILE *fp;


    // get delimeter info
	memset(d_col, 0x00, sizeof(d_col));
	memset(d_row, 0x00, sizeof(d_row));
    fp = fopen("delimeter.conf", "r");
	if (fp == NULL) {
		strcpy(d_col, "^^##") ;
		strcpy(d_row, "##^^");
	}else {
		fscanf(fp ,"%s %s", d_col, d_row);
		fclose(fp);
    }

	// TEMP��
	ZeroMemory( &_si, sizeof(STARTUPINFO) );
	ZeroMemory( &_pi, sizeof(PROCESS_INFORMATION) );

	_si.cb            = sizeof( STARTUPINFO );
	_si.dwFlags       = STARTF_USESHOWWINDOW;
	_si.wShowWindow   = SW_SHOWNORMAL;

    // ILOADER��
	ZeroMemory( &IloaderProcess[i].si, sizeof(STARTUPINFO) );
	ZeroMemory( &IloaderProcess[i].pi, sizeof(PROCESS_INFORMATION) );

	IloaderProcess[i].si.cb            = sizeof( STARTUPINFO );
	IloaderProcess[i].si.dwFlags       = STARTF_USESHOWWINDOW;
	IloaderProcess[i].si.wShowWindow   = SW_SHOWNORMAL;

	memset(USER,   0x00, sizeof(USER));
	memset(PASSWD, 0x00, sizeof(PASSWD));
	memset(PORT,   0x00, sizeof(PORT));

	Form5->getDsnInfo(SERVERNAME->Caption, "User",         USER);
	Form5->getDsnInfo(SERVERNAME->Caption, "Password",     PASSWD);
	Form5->getDsnInfo(SERVERNAME->Caption, "Port",         PORT);

    // for manager
	if (!DirectoryExists("FORM"))
	{
		CreateDir("FORM");
	}
	if (!DirectoryExists("DATA"))
	{
		CreateDir("DATA");
	}
	if (!DirectoryExists("LOG"))
	{
		CreateDir("LOG");
	}
	if (!DirectoryExists("BAD"))
	{
		CreateDir("BAD");
	}


    // DataIn
	sprintf(runx, "iloader_451 -S %s -U %s -P %s -PORT %s in "
				  " -T %s.%s -f FORM//%s_%s.fmt -d DATA//%s_%s.dat "
				  " -log LOG//%s_%s.log -bad BAD//%s_%s.bad -t %s -r %s ",
				   SERVERNAME->Caption.c_str(),
				   USER, PASSWD, PORT,
				   uname.c_str(), tname.c_str(),
				   uname.c_str(), tname.c_str(),
				   uname.c_str(), tname.c_str(),
				   uname.c_str(), tname.c_str(),
				   uname.c_str(), tname.c_str(),
				   d_col, d_row  );

	CreateProcess( NULL, runx, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, NULL,
				   &IloaderProcess[i].si, &IloaderProcess[i].pi );

	memset(IloaderProcess[i].uname, 0x00, sizeof(IloaderProcess[i].uname));
	memset(IloaderProcess[i].tname, 0x00, sizeof(IloaderProcess[i].tname));

	memcpy(IloaderProcess[i].uname, uname.c_str(), uname.Length());
	memcpy(IloaderProcess[i].tname, tname.c_str(), tname.Length());
	sprintf(IloaderProcess[i].startTime, "%s,%s", t.CurrentDate().DateString().c_str(),
	                                              t.CurrentTime().TimeString().c_str());
    IloaderProcess[i].self = i;

	sprintf(runx, "%-10ld %15s.%-40s %-26s %-26s",
	                                      IloaderProcess[i].pi.dwProcessId,
										  IloaderProcess[i].uname,
										  IloaderProcess[i].tname,
										  IloaderProcess[i].startTime,
										  "running");

	Form8->CheckListBox1->Items->Add(runx);
	//ShowMessage(pi.dwProcessId);
}
//---------------------------------------------------------------------------
// export �ޱ�, aexport�� �̿��ϴ� �������� �����Ѵ�.
void __fastcall TForm5::exportData4Click(TObject *Sender)
{
	TTreeNode *tNode;
    AnsiString uname;
	int i, pid;
	//ShowMessage("aa");
	//execAexport(0, "a", "b");
	//execAexport(1, "a", "b");
	// ���õ� selection������ ���� �������� üũ
	if (!checkSelection())
		return;

	// ���õ� selection������ ���� ���̺�������.. üũ�ؾ� �Ѵ�.
	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		if (!tNode->Parent) {
			ShowMessage("Selections must have only table-type Object.");
			return;
		}
		if (tNode->Parent->Text != "Tables")
		{
			ShowMessage("Selections must have only table-type Object.");
			return;
		}
	}

	//aexport�� ����Ÿ �ޱ� ����.
	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		uname = DBNODE->Selections[0]->Parent->Parent->Text;
		pid = findEmptyNode();
		// iloaderINdex, UserName , TableName
		if (pid != -1)
			execAexportOut(pid, uname, tNode->Text);
		else {
			ShowMessage("Insufficient Resource of Process. try later");
			if (Timer1->Enabled == false) {
			   Timer1->Enabled = true;
			}

			return;
		}
	}

	//ShowMessage("export Started, check Management->iloaderStatus");
	if (Timer1->Enabled == false) {
	   Timer1->Enabled = true;
	}

	Form8->Show();
}
//---------------------------------------------------------------------------
// iloader ���μ��� ���� ���̺��� �����ַ��� ó���ϴ� �Լ�
void __fastcall TForm5::Timer1Timer(TObject *Sender)
{
	int i, j;
    char sMsg[1024];
	DWORD  dwExitCode;
	TDateTime t;

    // �����ü�� ����ΰ͸� �޽����� ���� FOrm8�� ������.
	for (i = 0; i < ILOADER_MAX_PROCESS; i++)
	{
		if (IloaderProcess[i].startTime[0] == 0x00) {
			continue;
		}

		GetExitCodeProcess(IloaderProcess[i].pi.hProcess, &dwExitCode);
		if (dwExitCode == STILL_ACTIVE)
		{
			sprintf(sMsg, "%-10ld %15s.%-40s %-s %-s",
										  IloaderProcess[i].pi.dwProcessId,
										  IloaderProcess[i].uname,
										  IloaderProcess[i].tname,
										  IloaderProcess[i].startTime,
										  "running");
		}else {
			if (IloaderProcess[i].endTime[0] == 0x00) {
				sprintf(IloaderProcess[i].endTime, "%s,%s", t.CurrentDate().DateString().c_str(),
												            t.CurrentTime().TimeString().c_str());
			}
			sprintf(sMsg, "%-10ld %15s.%-40s %-26s %-26s",
                                          IloaderProcess[i].pi.dwProcessId,
										  IloaderProcess[i].uname,
										  IloaderProcess[i].tname,
										  IloaderProcess[i].startTime,
										  IloaderProcess[i].endTime);
		}

		//ShowMessage(Form8->CheckListBox1->Items->Count);
        // refresh�� ������ ������ ��·�� ȭ�鿡 ���� ��ġ�� ã�Ƽ� ���.
        // PID�� �����ϴ�.!!
		for (j=0; j < Form8->CheckListBox1->Items->Count; j++)
		{
			if (memcmp(Form8->CheckListBox1->Items->Strings[j].SubString(1, 10).c_str(),
					   sMsg, 10) == 0 ) {
                Form8->CheckListBox1->Items->Strings[j] = sMsg;
                break;
			}
		}

	}
    
}
//---------------------------------------------------------------------------
void __fastcall TForm5::iLoaderStatus1Click(TObject *Sender)
{
    Form8->Show();	
}
//---------------------------------------------------------------------------

void __fastcall TForm5::FormCloseQuery(TObject *Sender, bool &CanClose)
{
//TerminateProcess(IloaderProcess[0].pi.hProcess, -1);
}
//---------------------------------------------------------------------------
void __fastcall TForm5::freeIloaderNode()
{
	 int i, j;
	 DWORD pid;

	 // �ϴ� Ÿ�̸� ���߰�
	 Timer1->Enabled = false;

	 // delte�Լ������� �Ųٷ� �˻������Ѵ�.
	 for (i = Form8->CheckListBox1->Items->Count-1 ; i >=0 ; i--)
	 {
         // ���õ� �����..
		 if (Form8->CheckListBox1->Checked[i] == true)
		 {
			 // ȭ�鿡 �������� PID�� ã�Ƴ���.
			 pid = (unsigned long)atol(Form8->CheckListBox1->Items->Strings[i].SubString(1, 10).c_str());
			 //ShowMessage(pid);

			 // ���� ����� ilodaerProcess table�� �ִ°��� �˻�.
			 for (j = 0; j < ILOADER_MAX_PROCESS; j++)
			 {
				 if (IloaderProcess[j].startTime[0] == 0x00) {
                     continue;
				 }
                 // PID�� ����.!!!
				 if (pid == IloaderProcess[j].pi.dwProcessId)
				 {
					 // ����ü�� free��Ű�� checkListBox������ �����Ѵ�.
                     TerminateProcess(IloaderProcess[j].pi.hProcess, -1);
					 memset(&IloaderProcess[j], 0x00, sizeof(IloaderProcess[j]));
					 Form8->CheckListBox1->Items->Delete(i);
                     //ShowMessage("deleteok");
					 break;
				 }
			 }
		 }
	}

	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
// find iloader struct 
int __fastcall TForm5::findTaskIloader()
{
   int sel = Form8->CheckListBox1->ItemIndex;
   int j;
   unsigned long pid;
   char fname[255];

   pid = (unsigned long)atol( Form8->CheckListBox1->Items->Strings[sel].SubString(1, 10).c_str() );
   // ���� ����� ilodaerProcess table�� �ִ°��� �˻�.
   for (j = 0; j < ILOADER_MAX_PROCESS; j++)
   {
		 if (IloaderProcess[j].startTime[0] == 0x00) {
			  continue;
		 }
		 // PID�� ����.!!!
		 if (pid == IloaderProcess[j].pi.dwProcessId)
		 {
			 Form8->Memo1->Clear();
			 Form8->Memo1->Left = Form8->CheckListBox1->Left;
			 Form8->Memo1->Align = alClient;
			 Form8->Memo1->Visible = true;
             sprintf(fname, "LOG//%s_%s.log", IloaderProcess[j].uname, IloaderProcess[j].tname);
			 Form8->Memo1->Lines->LoadFromFile(fname);
			 return j;
		 }
    }
	return -1;
}
//---------------------------------------------------------------------------
// iloader ���̱�
void __fastcall TForm5::TerminateIloaderNode()
{
	 int i, j;
	 DWORD pid;

	 // �ϴ� Ÿ�̸� ���߰�
	 Timer1->Enabled = false;

	 // delte�Լ������� �Ųٷ� �˻������Ѵ�.
	 for (i = Form8->CheckListBox1->Items->Count-1 ; i >=0 ; i--)
	 {
         // ���õ� �����..
		 if (Form8->CheckListBox1->Checked[i] == true)
		 {
			 // ȭ�鿡 �������� PID�� ã�Ƴ���.
			 pid = (unsigned long)atol(Form8->CheckListBox1->Items->Strings[i].SubString(1, 10).c_str());
			 //ShowMessage(pid);

			 // ���� ����� ilodaerProcess table�� �ִ°��� �˻�.
			 for (j = 0; j < ILOADER_MAX_PROCESS; j++)
			 {
				 if (IloaderProcess[j].startTime[0] == 0x00) {
                     continue;
				 }
                 // PID�� ����.!!!
				 if (pid == IloaderProcess[j].pi.dwProcessId)
				 {
					 TerminateProcess(IloaderProcess[j].pi.hProcess, -1);
					 // ����ü�� free��Ű�� checkListBox������ �����Ѵ�.
					 memset(&IloaderProcess[j], 0x00, sizeof(IloaderProcess[j]));
                     Form8->CheckListBox1->Items->Delete(i);
                     //ShowMessage("deleteok");
					 break;
				 }
			 }
		 }
	}

	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm5::ExportData1Click(TObject *Sender)
{
    exportData4Click(this);	
}
//---------------------------------------------------------------------------

void __fastcall TForm5::ExportData2Click(TObject *Sender)
{
	exportData4Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm5::ExportData3Click(TObject *Sender)
{
	exportData4Click(this);
}
//---------------------------------------------------------------------------


void __fastcall TForm5::ImportData2Click(TObject *Sender)
{
	TTreeNode *tNode;
    AnsiString uname;
	int i, pid;
	//ShowMessage("aa");
	//execAexport(0, "a", "b");
	//execAexport(1, "a", "b");
	// ���õ� selection������ ���� �������� üũ
	if (!checkSelection())
		return;

	// ���õ� selection������ ���� ���̺�������.. üũ�ؾ� �Ѵ�.
	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		if (!tNode->Parent) {
			ShowMessage("Selections must have only table-type Object.");
			return;
		}
		if (tNode->Parent->Text != "Tables")
		{
			ShowMessage("Selections must have only table-type Object.");
			return;
		}
	}

	//aexport�� ����Ÿ �ޱ� ����.
	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		tNode = DBNODE->Selections[i];
		uname = DBNODE->Selections[0]->Parent->Parent->Text;
		pid = findEmptyNode();
		// iloaderINdex, UserName , TableName
		if (pid != -1)
			execAexportIn(pid, uname, tNode->Text);
		else {
			ShowMessage("Insufficient Resource of Process. try later");
			if (Timer1->Enabled == false) {
			   Timer1->Enabled = true;
			}

			return;
		}
	}

	//ShowMessage("export Started, check Management->iloaderStatus");
	if (Timer1->Enabled == false) {
	   Timer1->Enabled = true;
	}

	Form8->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm5::importData4Click(TObject *Sender)
{
	ImportData2Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm5::ImportData1Click(TObject *Sender)
{
    ImportData2Click(this);	
}
//---------------------------------------------------------------------------


void __fastcall TForm5::CreateTableSpace1Click(TObject *Sender)
{
    TTreeNode *tNode = DBNODE->Selected;

	Form9->ListBox1->Clear();
	Form9->SPACENAME->Text = "";
	Form9->SPACENAME->Text = "";
	Form9->DATAFILE->Text = "";
	Form9->INITSIZE->Text = "";
	Form9->MAXSIZE->Text = "";
    Form9->EXTENDSIZE->Text = "";

	Form9->ShowModal();

	while (tNode->Text != "TABLESPACES")
	{
        //ShowMessage(tNode->Text);
		tNode = tNode->GetNext();
	}

	if (!dbConnect(SERVERNAME->Caption)) {
		 return;
	}              
	BuildTbsTree(tNode);
    tNode->Expand(true);

	freeDBHandle();
}
//---------------------------------------------------------------------------

void __fastcall TForm5::droptablespace1Click(TObject *Sender)
{
	//TTreeNode *tNode = DBNODE->Selected;
	SQLHSTMT stmt;
    int rc, i;
	char query[1024];


    // ���õ� selection������ ���� �������� üũ
	if (!checkSelection())
		return;

	// ���õ� selection������ ���� ���̺����̽�������.. üũ�ؾ� �Ѵ�.
	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		 if (DBNODE->Selections[i]->Parent == NULL) {
             ShowMessage("selectdObject is not TABLESPACE");
			 return;
		 }
		 if (DBNODE->Selections[i]->Parent->Text != "TABLESPACES" ) {
			 ShowMessage("selectdObject is not TABLESPACE");
			 return;
		 }
    }

	if (MessageBox(NULL, "Really, Drop Tablespace ?", "confirm", MB_OKCANCEL) == ID_CANCEL) {
        return;
	}


	if (!dbConnect(SERVERNAME->Caption)) {
        return;
	}


	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		sprintf(query, "drop tablespace %s including contents and datafiles cascade constraints",
                     DBNODE->Selections[i]->Text.c_str());

		rc = _ExecDirect(query);
		if (rc == 1) {
            DBNODE->Selections[i]->Delete(); 
		}else{
            break;
		}
	}

	freeDBHandle();


	if (rc == 1) {
        ShowMessage("Drop Tablespace Success!!");
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm5::DropTableSpace2Click(TObject *Sender)
{
    droptablespace1Click(this);
}
//---------------------------------------------------------------------------


void __fastcall TForm5::AlterTableSpace1Click(TObject *Sender)
{
	TTreeNode *tNode = DBNODE->Selected;
    int i;


    // �޸𸮴� �ʿ�����ϱ�..
	if (tNode->Text == "SYS_TBS_MEMORY") {
        return; 
	}


	// ���õ� selection������ ���� �������� üũ
	if (!checkSelection())
		return;

	// ���õ� selection������ ���� ���̺����̽�������.. üũ�ؾ� �Ѵ�.
	for (i = DBNODE->SelectionCount-1; i >=0; i--)
	{
		 if (DBNODE->Selections[i]->Parent == NULL) {
			 ShowMessage("selectdObject is not TABLESPACE");
			 return;
		 }
		 if (DBNODE->Selections[i]->Parent->Text != "TABLESPACES" ) {
			 ShowMessage("selectdObject is not TABLESPACE");
			 return;
		 }
	}

	Form10->ListBox1->Clear();
	Form10->SPACENAME->Text = "";
	Form10->SPACENAME->Text = "";
	Form10->DATAFILE->Text = "";
	Form10->INITSIZE->Text = "";
	Form10->MAXSIZE->Text = "";
    Form10->EXTENDSIZE->Text = "";

	Form10->SPACENAME->Text = tNode->Text;
    Form10->ShowModal();
	DBNODEDblClick(this);
}
//---------------------------------------------------------------------------
// refresh ����� �ִ´�.
void __fastcall TForm5::Reload1Click(TObject *Sender)
{
	if (!dbConnect(SERVERNAME->Caption)) {
        return;
	}

	if (DBNODE->Selected->Text == "Tables") {
		BuildTableTree(DBNODE->Selected, DBNODE->Selected->Parent->Text);
	}
	if (DBNODE->Selected->Text  == "Views") {
		BuildViewTree(DBNODE->Selected, DBNODE->Selected->Parent->Text);
	}
	if (DBNODE->Selected->Text  == "Triggers") {
		BuildTriggerTree(DBNODE->Selected, DBNODE->Selected->Parent->Text);
	}
	if (DBNODE->Selected->Text  == "Procedures") {
		BuildProcTree(DBNODE->Selected, DBNODE->Selected->Parent->Text);
	}
	DBNODE->Selected->Expand(true);


	freeDBHandle();
}
//---------------------------------------------------------------------------

void __fastcall TForm5::SchemaScript1Click(TObject *Sender)
{
    ScriptOut1Click(this);
}
//---------------------------------------------------------------------------

