
// PragEncryptionDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "PragEncryption.h"
#include "PragEncryptionDlg.h"
#include "CEnvironmentSetDlg.h"
#include "afxdialogex.h"

#include <direct.h>
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPragEncryptionDlg 대화 상자



CPragEncryptionDlg::CPragEncryptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRAGENCRYPTION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPragEncryptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, Main);
}

BEGIN_MESSAGE_MAP(CPragEncryptionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CPragEncryptionDlg::OnSelchangeTabMain)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPragEncryptionDlg 메시지 처리기

BOOL CPragEncryptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	CRect size;
	Main.GetWindowRect(&size);

	Main.InsertItem(0, TEXT("파일 설정"), TCIF_TEXT);
	Main.InsertItem(1, TEXT("마스터키 생성"), TCIF_TEXT);
	Main.InsertItem(2, TEXT("환경 설정"), TCIF_TEXT);
	Main.SetCurSel(0);

	FDlg.Create(IDD_DIALOG_FILE, &Main);
	FDlg.MoveWindow(0, 24, size.Width(), size.Height());
	FDlg.ShowWindow(SW_SHOW);

	KDlg.Create(IDD_DIALOG_KEY, &Main);
	KDlg.MoveWindow(0, 24, size.Width(), size.Height());
	KDlg.ShowWindow(SW_HIDE);

	EDlg.Create(IDD_DIALOG_ENVIRONMENT, &Main);
	EDlg.MoveWindow(0, 24, size.Width(), size.Height());
	EDlg.ShowWindow(SW_HIDE);
	EDlg.get();

	EDlg.UpdateData(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CPragEncryptionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPragEncryptionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPragEncryptionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPragEncryptionDlg::OnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	FDlg.ShowWindow(SW_HIDE);
	KDlg.ShowWindow(SW_HIDE);
	EDlg.ShowWindow(SW_HIDE);

	switch (Main.GetCurSel())
	{
	case 0:
		FDlg.ShowWindow(SW_SHOW);
		break;

	case 1:
		KDlg.ShowWindow(SW_SHOW);
		break;

	case 2:
		EDlg.ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}


void CPragEncryptionDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 복호화에서 임시로 열기를 사용했을 경우 종료 전 그에 대한 캐시 파일을 삭제해줘야함
	CEnvironmentSetDlg EnvSet;
	
	TCHAR dir[50] = TEXT("");
	CString str;

	GetEnvironmentVariable(TEXT("APPDATA"), dir, 50);
	str = str + dir + TEXT("\\PRLock ") + EnvSet.version + TEXT("\\cache");

	// 디렉터리 삭제에 실패한 경우
	if (!DeleteDirectoryFile(str))
	{
		MessageBox(TEXT("캐시 삭제에 실패했습니다. 임시로 열린 파일을 닫아주세요"), TEXT("오류"), MB_ICONERROR | MB_OK);
		return;
	}
	CDialogEx::OnClose();
}
