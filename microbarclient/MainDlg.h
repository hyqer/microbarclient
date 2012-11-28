// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "lua.hpp"
#include <atlstr.h>
#define WB_MOD (MOD_CONTROL|MOD_SHIFT)
HWND g_hLastWnd=0;
CString g_sLastTitle;
int restore_text(lua_State*L)
{
	if (IsWindow(g_hLastWnd))
		SetWindowText(g_hLastWnd,g_sLastTitle);
	return 0;
}
int show_text(lua_State*L)
{
	int n = lua_gettop(L);  /* number of arguments */

	lua_getglobal(L, "tostring");

	const char *s;
	lua_pushvalue(L, -1);  /* function to be called */
	lua_pushvalue(L, 1);   /* value to print */
	lua_pcall(L, 1, 1, 0);
	s = lua_tostring(L, -1);  /* get result */
	if (s == NULL)
		return 0;

	//fputs(s, stdout);
	lua_pop(L, 1);  /* pop result */

	HWND hTop=GetForegroundWindow();
	if (IsWindow(hTop))
	{
		TCHAR wstr[1500];
		GetWindowText(hTop,wstr,1500);
		
		if (g_hLastWnd!=hTop)
		{
			if (IsWindow(g_hLastWnd))
			{
				SetWindowText(g_hLastWnd,g_sLastTitle);
			}
			g_sLastTitle = wstr;
			g_hLastWnd=hTop;
		}
		MultiByteToWideChar(CP_UTF8, 0, s, -1,  wstr, 1500);
		
		SetWindowText(hTop,wstr);
	}
	lua_pushinteger(L,LUA_INTEGER(hTop) );
	return 0;
}
class CMainDlg : public CAxDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
	public CMessageFilter, public CIdleHandler,
	public IDispEventImpl<IDC_EXPLORER1,CMainDlg>
{
public:
	lua_State *L;
	int keyid;
	char token[1024];
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
		MESSAGE_HANDLER (WM_HOTKEY, OnHotkeyDown) 
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
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
		luaL_loadstring(L,"show_text('23344')");
		lua_call(L,0,1);
		const char* str = lua_tostring(L,1);
		lua_pop(L,1);

		//TCHAR wstr[150];

		//MultiByteToWideChar(CP_UTF8, 0, str, -1,  wstr, 150);
		//this->SetWindowText(wstr);
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		//CloseDialog(wID);
		luaL_loadstring(L,"show_text('23344')");
		lua_call(L,0,1);
		const char* str = lua_tostring(L,1);
		lua_pop(L,1);
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
		SINK_ENTRY(IDC_EXPLORER1, 259, DocumentCompleteExplorer1)
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
	LRESULT OnHotkeyDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
	{ 
		//TODO: Add Code for message handler. Call DefWindowProc if necessary. 
		//MessageBox (L"aaa"); 
		lua_getglobal(L,"OnHotKey");
		lua_pushinteger(L,wParam);
		lua_call(L, 1, 0);
		return 0; 
	}

	void __stdcall DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
	{
		// TODO: 在此处添加消息处理程序代码
		CAxWindow wndIE = GetDlgItem(IDC_EXPLORER1);
		CComPtr<IWebBrowser2> spWebBrowser2;
		HRESULT hr;

		hr = wndIE.QueryControl ( &spWebBrowser2 );
		if ( spWebBrowser2 )
		{
			CComVariant v;  // empty variant

			IDispatch * spDispDoc;
			spWebBrowser2->get_Document(&spDispDoc);
			CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> spHTMLDoc;
			hr = spDispDoc->QueryInterface(IID_IHTMLDocument2, (void**) &spHTMLDoc);
			if (SUCCEEDED(hr) && spHTMLDoc)
			{
				IHTMLElement *pElemt;
				hr = spHTMLDoc->get_body(&pElemt);

				if (SUCCEEDED(hr))
				{
					//strUrl = COLE2T(bstrUrl);
					BSTR strHtml;
					pElemt->get_innerHTML(&strHtml);
					TCHAR*pStartToken = _T("<SPAN id=user-access-token>");

					TCHAR*pEndToken = _T("</SPAN>");
					TCHAR *pStart = wcsstr(strHtml,pStartToken);
					if (!pStart)
					{
						return;
					}
					pStart+=wcslen(pStartToken);
					TCHAR *pEnd = wcsstr(pStart,pEndToken);
					memset(token,0,1024);
					WideCharToMultiByte(CP_UTF8,0,pStart,(pEnd-pStart)/*/sizeof(*pStart)*/,token,1024,NULL,NULL);
					//MessageBox(strHtml);
					char assign[2048];
					sprintf(assign,
						"token = '%s' \
						s=weiboapiget('statuses/home_timeline',token,'page='..1) \
						tl = json.decode(s)['statuses']",
						token);
					luaL_dostring(L,assign);
					//wndIE.ShowWindow(SW_HIDE);
				}


			}
		}
		
	}

	//LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		static bool init=false;
		if (!init)
		{
			init = true;
			CAxWindow wndIE = GetDlgItem(IDC_EXPLORER1);
			CComPtr<IWebBrowser2> pWB2;
			HRESULT hr;

			hr = wndIE.QueryControl ( &pWB2 );
			if ( pWB2 )
			{
				CComVariant v;  // empty variant

				pWB2->Navigate ( CComBSTR("http://microbar.rmbback.com/"), 
					&v, &v, &v, &v );
			}
			L = luaL_newstate();
			luaL_openlibs(L);
			lua_register (L,
				"show_text",
				show_text);
			lua_register (L,
				"restore_text",
				restore_text);
			if (luaL_loadfile(L,"funcs.lua")==0)
			{
				lua_pcall(L,0,0,0);
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
			keyid=2012;
			RegisterHotKey(this->m_hWnd,keyid,WB_MOD,VK_DOWN);
			keyid++;//13
			RegisterHotKey(this->m_hWnd,keyid,WB_MOD,VK_UP);
			keyid++;//14
			RegisterHotKey(this->m_hWnd,keyid,WB_MOD,'R');
			keyid++;//15
			RegisterHotKey(this->m_hWnd,keyid,WB_MOD,'Z');
			keyid++;//16
			RegisterHotKey(this->m_hWnd,keyid,WB_MOD,'C');
			keyid++;//17
			RegisterHotKey(this->m_hWnd,keyid,WB_MOD,VK_RIGHT);
			keyid++;//18
			RegisterHotKey(this->m_hWnd,keyid,WB_MOD,VK_LEFT);
			keyid++;//19
		
			RegisterHotKey(this->m_hWnd,keyid,WB_MOD,'O');
			keyid++;//20
			RegisterHotKey(this->m_hWnd,keyid,WB_MOD,'F');
			keyid++;//20
			RegisterHotKey(this->m_hWnd,keyid,WB_MOD,'H');
		}
		return 0;
	}
};
