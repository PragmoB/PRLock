// CKeyEncryptDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "PragEncryption.h"
#include "CKeyEncryptDlg.h"
#include "afxdialogex.h"

#include "RSA.h"
#include "KISA_SHA256.h"

// CKeyEncryptDlg 대화 상자

IMPLEMENT_DYNAMIC(CKeyEncryptDlg, CDialogEx)

CKeyEncryptDlg::CKeyEncryptDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_KEY, pParent)
	, KeyPath(_T(""))
	, key_length(0)
{

}

CKeyEncryptDlg::~CKeyEncryptDlg()
{
}

void CKeyEncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEY_PATH, KeyPath);
	DDX_Text(pDX, IDC_EDIT_KEY_LENGTH, key_length);
}


BEGIN_MESSAGE_MAP(CKeyEncryptDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_KEY_PATH, &CKeyEncryptDlg::OnClickedButtonKeyPath)
	ON_BN_CLICKED(IDC_BUTTON_KEY_SET, &CKeyEncryptDlg::OnClickedButtonKeySet)
END_MESSAGE_MAP()


// CKeyEncryptDlg 메시지 처리기


void CKeyEncryptDlg::OnClickedButtonKeyPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	BROWSEINFO bi;
	LPITEMIDLIST idl;
	wchar_t dir[100] = TEXT("");


	ZeroMemory(&bi, sizeof(BROWSEINFO));

	bi.hwndOwner = this->m_hWnd;
	bi.pszDisplayName = NULL;  // 선택된 디렉토리명 저장 
	bi.lpszTitle = TEXT("키 파일이 만들어질 경로를 선택해 주세요.");
	bi.ulFlags = BIF_EDITBOX    // 선택된 디렉토리명을 표시하고 편집할 수 있는 창
		| 0x0040             // 새로운 디렉토리를 만들수 있는 버튼 추가 
		;

	idl = SHBrowseForFolder(&bi);

	if (idl)
	{
		SHGetPathFromIDList(idl, dir); // 전체 PATH을 포함한 디렉토리명
		KeyPath = dir;
		UpdateData(FALSE);
	}
}


void CKeyEncryptDlg::OnClickedButtonKeySet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (key_length < 16)
	{
		MessageBox(TEXT("키 길이가 너무 작습니다"), TEXT("알림"), MB_OK | MB_ICONWARNING );
		return;
	}
		
	if (KeyPath == "")
	{
		MessageBox(TEXT("키 파일이 생성될 경로를 선택해주세요"), TEXT("알림"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	CFile N, e, d, N1;
	CFileException eex;
	RSA key;

	MessageBox(TEXT("생성까지 조금만 기다려주세요"), TEXT("알림"), MB_OK);

	key.SetKeyAuto(key_length);

	CreateDirectory(KeyPath + TEXT("\\RSA_암호화키"), NULL);
	CreateDirectory(KeyPath + TEXT("\\RSA_복호화키"), NULL);

	try
	{
		N.Open(KeyPath + TEXT("\\RSA_암호화키\\N.pkey"),
			CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive, NULL);

		e.Open(KeyPath + TEXT("\\RSA_암호화키\\e.pkey"),
			CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive, NULL);

		N1.Open(KeyPath + TEXT("\\RSA_복호화키\\N.pkey"),
			CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive, NULL);

		d.Open(KeyPath + TEXT("\\RSA_복호화키\\d.pkey"),
			CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive, NULL);
	}
	catch (CFileException* eex)
	{
		eex->ReportError();
		return;
	}

	WriteBigint(&N, key.N, NULL);
	WriteBigint(&N1, key.N, NULL);
	WriteBigint(&e, key.e, NULL);
	WriteBigint(&d, key.d, NULL);

	MessageBox(TEXT("생성 성공"), TEXT("알림"), MB_OK);

	N.Close();
	N1.Close();
	e.Close();
	d.Close();
}


void CKeyEncryptDlg::OnClickedButtonKeyUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


int CKeyEncryptDlg::WriteBigint(CFile* file, BigInteger num, u_char* key)
{
	// TODO: 여기에 구현 코드 추가.

	char letter[1] = "";
	BigInteger temp, next;
	int i = 0;

	temp = num;
	next = num;

	// 키를 숫자형식으로 파일에 저장하는 과정.
	for (; temp != 0; temp /= 10)
	{
		next /= 10; // 10으로 나눠 첫번째 자리의 수를 버림함.

		/* next에 다시 10을 곱하면 결과적으로 next의 첫번째 자리 숫자는 0이 되므로
		 * 이를 원본 숫자에서 빼면 원본 숫자의 첫번째 자리수를 구할 수 있다.
		 * ex) 48 - 48 / 10 * 10 = 8
		 * ^ key[i++] < 보안 강화를 위한 xor 블록 암호화 코드.
		 */
		if(key == NULL)
			letter[0] = (temp - next * 10).toInt() + 0x30;

		else
		{
			letter[0] = (temp - next * 10).toInt() ^ key[i++];
			if (key[i] == '\0')
				i = 0;
		}

		try 
		{
			file->Write(letter, 1);
		}
		catch(CFileException& e)
		{
			e.ReportError();
			return FALSE;
		}
	}
	return 1;
}