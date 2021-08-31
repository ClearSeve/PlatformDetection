#pragma once
#include <list>

class CMainDialog : public CDialogImpl<CMainDialog>, public CMessageFilter
{
public:
    CMainDialog();
    ~CMainDialog();

public:
    enum { IDD = IDD_PLATFORM_DETECTION };
    BEGIN_MSG_MAP(CMainDialog)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
        COMMAND_ID_HANDLER(IDC_BUT_SELECT_FILE, OnSelectFile)
        COMMAND_ID_HANDLER(IDC_BUT_SELECT_FOLDER, OnSelectFolder)

    END_MSG_MAP()

public:
    LRESULT OnInitDialog(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnSelectFile(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnSelectFolder(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
    void InsterList(std::list<CString>& list);
    CString GetFilePlatform(CString strFilePath);
    CAtlString GetSuffix(LPCTSTR pszFilePath);

private:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    int ShowMessageBox(LPCTSTR pszText, UINT uType = MB_OK | MB_ICONINFORMATION);
    void SetSysMenu();

private:
    CListViewCtrl m_wndListCtrl;
};
