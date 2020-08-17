// CInputEncryptDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "PragEncryption.h"
#include "CInputEncryptDlg.h"
#include "afxdialogex.h"


// CInputEncryptDlg 대화 상자

IMPLEMENT_DYNAMIC(CInputEncryptDlg, CDialogEx)

CInputEncryptDlg::CInputEncryptDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INPUT_ENCRYPT_KEY, pParent)
	, masterkey(_T(""))
	, normalkey(_T(""))
	, nouse_master(FALSE)
{

}

CInputEncryptDlg::~CInputEncryptDlg()
{
}

void CInputEncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MASTER, masterkey);
	DDX_Text(pDX, IDC_EDIT_NORMAL, normalkey);
	//  DDX_Check(pDX, IDC_CHECK_NO_USE_MASTER, use_master);
	DDX_Check(pDX, IDC_CHECK_NO_USE_MASTER, nouse_master);
}


BEGIN_MESSAGE_MAP(CInputEncryptDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CInputEncryptDlg::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_CHECK_NO_USE_MASTER, &CInputEncryptDlg::OnClickedCheckNoUseMaster)
END_MESSAGE_MAP()


// CInputEncryptDlg 메시지 처리기


void CInputEncryptDlg::OnBnClickedButtonOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (normalkey == "")
	{
		MessageBox(TEXT("일반키를 입력해주세요"), TEXT("알림"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (!nouse_master && masterkey == "")
	{
		MessageBox(TEXT("마스터키를 등록 해주세요"), TEXT("알림"), MB_OK);
		return;
	}

	CDialogEx::OnOK();
}


void CInputEncryptDlg::OnClickedCheckNoUseMaster()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_MASTER)->EnableWindow(!nouse_master);
}
