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
	DDX_Text(pDX, IDC_EDIT_NORMAL, normalkey);
}


BEGIN_MESSAGE_MAP(CInputDecryptDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CInputDecryptDlg::OnClickedButtonOk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_OPTION, IDC_RADIO2, &CInputDecryptDlg::OnBnClickedDecOpt)
	ON_BN_CLICKED(IDC_BUTTON_TEMP_OPEN, &CInputDecryptDlg::OnBnClickedButtonTempOpen)
END_MESSAGE_MAP()


// CInputDecryptDlg 메시지 처리기



afx_msg void CInputDecryptDlg::OnBnClickedDecOpt(UINT KeyOpt)
{
	switch (KeyOpt)
	{
	// 일반키 복호화 옵션
	case IDC_RADIO2:
		GetDlgItem(IDC_EDIT_NORMAL)->EnableWindow(TRUE);
		break;

	default :
		GetDlgItem(IDC_EDIT_NORMAL)->EnableWindow(FALSE);
	
	}
}

BOOL CInputDecryptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDC_EDIT_NORMAL)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CInputDecryptDlg::OnClickedButtonOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	OpenOpt = NORMAL;
	CDialogEx::OnOK();
}

void CInputDecryptDlg::OnBnClickedButtonTempOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	OpenOpt = TEMP;
	CDialogEx::OnOK();
}
