// CInputDecryptDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "PragEncryption.h"
#include "CInputDecryptDlg.h"
#include "afxdialogex.h"


// CInputDecryptDlg 대화 상자

IMPLEMENT_DYNAMIC(CInputDecryptDlg, CDialogEx)

CInputDecryptDlg::CInputDecryptDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INPUT_DECRYPT_KEY, pParent)
	, KeyOpt(0)
	, masterkey(_T(""))
	, normalkey(_T(""))
{

}

CInputDecryptDlg::~CInputDecryptDlg()
{
}

void CInputDecryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_OPTION, KeyOpt);
	DDX_Text(pDX, IDC_EDIT_MASTER, masterkey);
	DDX_Text(pDX, IDC_EDIT_NORMAL, normalkey);
}


BEGIN_MESSAGE_MAP(CInputDecryptDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CInputDecryptDlg::OnClickedButtonOk)
END_MESSAGE_MAP()


// CInputDecryptDlg 메시지 처리기


void CInputDecryptDlg::OnClickedButtonOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
