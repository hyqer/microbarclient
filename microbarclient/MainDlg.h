// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "lua.hpp"

class CMainDlg : public CAxDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
	public CMessageFilter, public CIdleHandler,
	public IDispEventImpl<IDC_EXPLORER1,CMainDlg>
{
public:
	lua_State *L;
	enum { IDD = IDD_MAINDLG };
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}
	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();
		AtlAdviseSinkMap ( this, true );
		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		CAxWindow wndIE = GetDlgItem(IDC_EXPLORER1);
		CComPtr<IWebBrowser2> pWB2;
		HRESULT hr;

		hr = wndIE.QueryControl ( &pWB2 );
		if ( pWB2 )
		{
			CComVariant v;  // empty variant

			pWB2->Navigate ( CComBSTR("http://microbar.rmbback.com/login/weibo/"), 
				&v, &v, &v, &v );
		}
		L = luaL_newstate();
		luaL_openlibs(L);
		if (luaL_loadfile(L,"funcs.lua")==0)
		{
			lua_pcall(L,0,1,0);
			const char* str = lua_tostring(L,-1);
			lua_pop(L,1);
			if (str)
			{
				TCHAR wstr[150];

				MultiByteToWideChar(CP_UTF8, 0, str, -1,  wstr, 150);
				this->SetWindowText(wstr);
			}
			
		}
		else
		{
			const char* error = lua_tostring(L,-1);
			lua_pop(L,1);
			if (error)
			{
				MessageBox(CComBSTR(error));
			}
		}


		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);
		AtlAdviseSinkMap ( this, false );
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		luaL_loadstring(L,"return 3+5");
		lua_call(L,0,1);
		const char* str = lua_tostring(L,1);
		lua_pop(L,1);

		TCHAR wstr[150];

		MultiByteToWideChar(CP_UTF8, 0, str, -1,  wstr, 150);
		this->SetWindowText(wstr);
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
		lua_close( L );
	}
	BEGIN_SINK_MAP(CMainDlg)
		SINK_ENTRY(IDC_EXPLORER1, 106, DownloadBeginExplorer1)
		SINK_ENTRY(IDC_EXPLORER1, 250, BeforeNavigate2Explorer1)
		SINK_ENTRY(IDC_EXPLORER1, 252, NavigateComplete2Explorer1)
	END_SINK_MAP()
	//void __stdcall DownloadBeginExplorer1();
	void __stdcall CMainDlg::DownloadBeginExplorer1()
	{
		// TODO: 在此处添加消息处理程序代码

	}
	//void __stdcall BeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
	void __stdcall CMainDlg::BeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
	{
		// TODO: 在此处添加消息处理程序代码
	}

	//void __stdcall NavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT* URL);

	void __stdcall CMainDlg::NavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT* URL)
	{
		// TODO: 在此处添加消息处理程序代码
	}

};
