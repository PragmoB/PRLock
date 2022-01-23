// CEnvironmentSetDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "PragEncryption.h"
#include "CEnvironmentSetDlg.h"
#include "afxdialogex.h"


// CEnvironmentSetDlg 대화 상자

IMPLEMENT_DYNAMIC(CEnvironmentSetDlg, CDialogEx)

CEnvironmentSetDlg::CEnvironmentSetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ENVIRONMENT, pParent)
	, FILE_AUTO_DELETE(0)
	, EncKeyPath(_T(""))
	, DecKeyPath(_T(""))
	, version(_T("2.1.1"))
{

}

CEnvironmentSetDlg::~CEnvironmentSetDlg()
{
}

void CEnvironmentSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_AUTO_DELETE, FILE_AUTO_DELETE);
	DDX_Text(pDX, IDC_EDIT_DECRYPT_KEY_PATH, DecKeyPath);
	DDX_Text(pDX, IDC_EDIT_ENCRYPT_KEY_PATH, EncKeyPath);
}


BEGIN_MESSAGE_MAP(CEnvironmentSetDlg, CDialogEx)
//	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK_AUTO_DELETE, &CEnvironmentSetDlg::OnClickedCheckAutoDelete)
	ON_BN_CLICKED(IDC_BUTTON_ENCRYPT_KEY_PATH, &CEnvironmentSetDlg::OnBnClickedButtonEncryptKeyPath)
	ON_BN_CLICKED(IDC_BUTTON_DECRYPT_KEY_PATH, &CEnvironmentSetDlg::OnBnClickedButtonDecryptKeyPath)
	ON_BN_CLICKED(IDC_BUTTON_ENV_SAVE, &CEnvironmentSetDlg::OnBnClickedButtonEnvSave)
END_MESSAGE_MAP()


// CEnvironmentSetDlg 메시지 처리기

int CEnvironmentSetDlg::update()
{
	// TODO: 여기에 구현 코드 추가.

	CFile EnvData;
	TCHAR dir[50] = TEXT("");
	CFileException eex;

	CString str;

	GetEnvironmentVariable(TEXT("APPDATA"), dir, 50);
	str = dir;
	CreateDirectory(str + TEXT("\\PRLock ") + version, NULL);

	if (!EnvData.Open(str + TEXT("\\PRLock ") + version + TEXT("\\EnvironmentSet.ini"),
		CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive | CFile::typeBinary, &eex))
	{
		eex.ReportError();
		return 0;
	}
	
	char temp[3] = "";
	temp[1] = '\n';
	temp[2] = 0;


	// 여기서부터 값 설정

	temp[0] = FILE_AUTO_DELETE + 48;
	EnvData.Write("FILE_AUTO_DELETE=", strlen("FILE_AUTO_DELETE="));	EnvData.Write(temp, strlen(temp));

	str = TEXT("ENCRYPT_KEY_PATH=") + EncKeyPath;
	EnvData.Write(CT2A(str), strlen(CT2A(str)));
	EnvData.Write("\n", 1);

	str = TEXT("DECRYPT_KEY_PATH=") + DecKeyPath;
	EnvData.Write(CT2A(str), strlen(CT2A(str)));
	EnvData.Write("\n", 1);

	EnvData.Close();
	return 1;
}


int CEnvironmentSetDlg::get()
{
	// TODO: 여기에 구현 코드 추가.
	CFile EnvData;
	TCHAR dir[50] = TEXT("");
	CHAR content[500] = "";
	CFileException eex;
	CString str;

	GetEnvironmentVariable(TEXT("APPDATA"), dir, 50);
	str = dir;

	if (!EnvData.Open(str + TEXT("\\PRLock ") + version + TEXT("\\EnvironmentSet.ini"),
		CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &eex))
		return 0;

	EnvData.Read(content, 500);
	str = content;
	int index;

	// 여기서부터 값 설정

	index = str.Find(TEXT("FILE_AUTO_DELETE="));
	FILE_AUTO_DELETE = 0;
	if(index != -1) // 값이 설정 되어있는 경우에만
		FILE_AUTO_DELETE = content[index + strlen("FILE_AUTO_DELETE=")] - 48; // 플래그 셋

	index = str.Find(TEXT("ENCRYPT_KEY_PATH="));
	EncKeyPath = "";
	if(index != -1)
		for (int i = index + strlen("ENCRYPT_KEY_PATH="); str[i] != '\n'; i++)
			EncKeyPath += str[i];

	index = str.Find(TEXT("DECRYPT_KEY_PATH="));
	DecKeyPath = "";
	if(index != -1)
		for (int i = index + strlen("DECRYPT_KEY_PATH="); str[i] != '\n'; i++)
			DecKeyPath += str[i];


	EnvData.Close();
	return 1;
}

void CEnvironmentSetDlg::OnClickedCheckAutoDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	update();
}


void CEnvironmentSetDlg::UpdateData(BOOL st)
{
	// TODO: 여기에 구현 코드 추가.
	CWnd::UpdateData(st);
}


void CEnvironmentSetDlg::OnBnClickedButtonEncryptKeyPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	BROWSEINFO bi;
	LPITEMIDLIST idl;
	wchar_t dir[100] = TEXT("");


	ZeroMemory(&bi, sizeof(BROWSEINFO));

	bi.hwndOwner = this->m_hWnd;
	bi.pszDisplayName = NULL;  // 선택된 디렉토리명 저장 
	bi.lpszTitle = TEXT("RSA 암호화키 파일(N.pkey, e.pkey)의 경로를 선택해 주세요.");
	bi.ulFlags = BIF_EDITBOX    // 선택된 디렉토리명을 표시하고 편집할 수 있는 창
		| 0x0040             // 새로운 디렉토리를 만들수 있는 버튼 추가 
		;

	idl = SHBrowseForFolder(&bi);

	if (idl)
	{
		SHGetPathFromIDList(idl, dir); // 전체 PATH을 포함한 디렉토리명
		EncKeyPath = dir;
		UpdateData(FALSE);
	}
	update();
}


void CEnvironmentSetDlg::OnBnClickedButtonDecryptKeyPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BROWSEINFO bi;
	LPITEMIDLIST idl;
	wchar_t dir[100] = TEXT("");


	ZeroMemory(&bi, sizeof(BROWSEINFO));

	bi.hwndOwner = this->m_hWnd;
	bi.pszDisplayName = NULL;  // 선택된 디렉토리명 저장 
	bi.lpszTitle = TEXT("RSA 복호화키 파일(N.pkey d.pkey)의 경로를 선택해 주세요.");
	bi.ulFlags = BIF_EDITBOX    // 선택된 디렉토리명을 표시하고 편집할 수 있는 창
		| 0x0040             // 새로운 디렉토리를 만들수 있는 버튼 추가 
		;

	idl = SHBrowseForFolder(&bi);

	if (idl)
	{
		SHGetPathFromIDList(idl, dir); // 전체 PATH을 포함한 디렉토리명
		DecKeyPath = dir;
		UpdateData(FALSE);
	}
	update();
}


void CEnvironmentSetDlg::OnBnClickedButtonEnvSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	update();
	MessageBox(TEXT("변경사항이 저장되었습니다"), TEXT("알림"),
		MB_OK | MB_ICONINFORMATION);
}