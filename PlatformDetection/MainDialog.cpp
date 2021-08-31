#include "stdafx.h"
#include "MainDialog.h"
#include "PlatformDetectionCore.h"
#include "utils/FileFind.h"

CMainDialog::CMainDialog()
{
}


CMainDialog::~CMainDialog()
{
}

LRESULT CMainDialog::OnInitDialog(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // 消息路由
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    pLoop->AddMessageFilter(this);

    // 设置标题
    CString strAppTitle;
    strAppTitle.Format(_T("%s · ClearSeve"), APP_NAME);
    SetWindowText(strAppTitle);

    // 设置程序图标
    HICON hIcon = AtlLoadIcon(MAKEINTRESOURCE(IDI_PLATFORM_DETECTION));
    SetIcon(hIcon, FALSE);
    SetIcon(hIcon, TRUE);

    // 窗口居中
    CenterWindow();

    // 修改菜单
    SetSysMenu();

    // 加载List控件
    m_wndListCtrl = GetDlgItem(IDC_LIST1);
    m_wndListCtrl.SetExtendedListViewStyle(m_wndListCtrl.GetExtendedListViewStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_wndListCtrl.InsertColumn(1, _T("路径"), LVCFMT_LEFT, 430);
    m_wndListCtrl.InsertColumn(2, _T("平台"), LVCFMT_LEFT, 50);

#ifdef DEBUG

#endif // DEBUG
    return 0;
}

LRESULT CMainDialog::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    DestroyWindow();
    ::PostQuitMessage(wID);
    return 0;
}

LRESULT CMainDialog::OnSelectFile(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    LPCTSTR lpszFilter = _T("二进制文件(*.exe;*.dll)\0*.exe;*.dll\0\0");
    CMultiFileDialog fileDialog(NULL, NULL, OFN_ALLOWMULTISELECT, lpszFilter);
    if (IDOK != fileDialog.DoModal(m_hWnd)) {
        return 0;
    }

    CString strDirectory;
    fileDialog.GetDirectory(strDirectory);

    std::list<CString> list;
    LPCTSTR pszFileName = fileDialog.GetFirstFileName();
    while (pszFileName) {
        CString strFilePath;
        strFilePath.Format(_T("%s\\%s"), strDirectory, pszFileName);
        list.push_back(strFilePath);
        pszFileName = fileDialog.GetNextFileName();
    }

    InsterList(list);
    return 0;
}

LRESULT CMainDialog::OnSelectFolder(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    CFolderDialog folder;
    if (IDOK != folder.DoModal()) {
        return 0;
    }

    CString strPath;
    strPath.Format(_T("%s\\*.*"), folder.m_szFolderPath);
    utils::CFileFind find(strPath);

    std::list<CString> list;
    BOOL bFind = find.FindFile();
    while (bFind)
    {
        if (find.IsFile()) {
            list.push_back(find.GetFilePath());
        }
        bFind = find.FindNext();
    }

    InsterList(list);
    return 0;
}

void CMainDialog::InsterList(std::list<CString>& list)
{
    if (0 != m_wndListCtrl.GetItemCount()) {
        if (IDYES == ShowMessageBox(_T("列表中已存在文件, 是否清空并导入?"), MB_YESNO)) {
            m_wndListCtrl.DeleteAllItems();
        }
    }

    for (auto item : list) {
        CString strFileSuffix = GetSuffix(item);
        strFileSuffix.MakeLower();
        if (!(_T("dll") == strFileSuffix || _T("exe") == strFileSuffix)) {
            continue;
        }

        int nPos = m_wndListCtrl.InsertItem(0, item);
        m_wndListCtrl.SetItemText(nPos, 1, GetFilePlatform(item));
    }
}

CString CMainDialog::GetFilePlatform(CString strFilePath)
{
    ComponentInfo info = GetArchitectureInfo(CW2A(strFilePath.GetBuffer()));

    switch (info)
    {
    case ComponentInfo::UNKNOWN:
        return _T("未知");
        break;
    case ComponentInfo::ANYCPU:
        return _T("AnyCPU");
        break;
    case ComponentInfo::X86:
        return _T("x86");
        break;
    case ComponentInfo::X64:
        return _T("x64");
        break;
    case ComponentInfo::rom:
        return _T("rom");
        break;
    default:
        return _T("未知");
        break;
    }
    return _T("未知");
}

CAtlString CMainDialog::GetSuffix(LPCTSTR pszFilePath)
{
    CAtlString strFilePath = pszFilePath;
    int nPos = strFilePath.ReverseFind(_T('.'));
    if (-1 == nPos) {
        return _T("");
    }
    return strFilePath.Mid(nPos + 1);
}

BOOL CMainDialog::PreTranslateMessage(MSG* pMsg)
{
    // 屏蔽窗口ESC和回车按键
    if (WM_KEYDOWN == pMsg->message && (VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam)) {
        return TRUE;
    }

    BOOL bDialogMessage = ::IsDialogMessage(m_hWnd, pMsg);
    if (WM_SYSCOMMAND == pMsg->message) {
        int nId = LOWORD(pMsg->wParam);
        if (IDM_ABOUT == nId) {
            ShellExecute(NULL, _T("OPEN"), APP_URL, NULL, NULL, SW_SHOWNORMAL);
            return TRUE;
        }
    }
    else if (WM_KEYDOWN == pMsg->message && VK_F1 == pMsg->wParam) {
        ShellExecute(NULL, _T("OPEN"), APP_URL, NULL, NULL, SW_SHOWNORMAL);
        return TRUE;
    }
    return bDialogMessage;
}

int CMainDialog::ShowMessageBox(LPCTSTR pszText, UINT uType)
{
    return AtlMessageBox(m_hWnd, pszText, APP_NAME, uType);
}

void CMainDialog::SetSysMenu()
{
    HMENU hSysMenu = GetSystemMenu(FALSE);
    if (hSysMenu == NULL) {
        return;
    }

    CString strAboutMenu;
    strAboutMenu.Format(_T("关于 %s (F1)..."), APP_NAME);

    CMenu menu(hSysMenu);
    menu.AppendMenu(MF_SEPARATOR);
    menu.AppendMenu(MF_STRING, IDM_ABOUT, strAboutMenu);
    menu.Detach();
}
