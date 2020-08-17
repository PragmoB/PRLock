// CInputMasterKeyDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "PragEncryption.h"
#include "CInputMasterKeyDlg.h"
#include "afxdialogex.h"


// CInputMasterKeyDlg 대화 상자

IMPLEMENT_DYNAMIC(CInputMasterKeyDlg, CDialogEx)

CInputMasterKeyDlg::CInputMasterKeyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INPUT_MASTER_KEY, pParent)
	, masterkey(_T(""))
{

}

CInputMasterKeyDlg::~CInputMasterKeyDlg()
{
}

void CInputMasterKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MASTER_KEY, masterkey);
	DDX_Control(pDX, IDC_STATIC_SEC_LEVEL, sec_level);
}


BEGIN_MESSAGE_MAP(CInputMasterKeyDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInputMasterKeyDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_MASTER_KEY, &CInputMasterKeyDlg::OnChangeEditMasterKey)
END_MESSAGE_MAP()


// CInputMasterKeyDlg 메시지 처리기


void CInputMasterKeyDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (!masterkey.GetLength())
	{
		MessageBox(TEXT("비밀번호를 입력해 주세요."), TEXT("알림"), MB_ICONINFORMATION | MB_OK);
		return;
	}
	CDialogEx::OnOK();
}


void CInputMasterKeyDlg::OnChangeEditMasterKey()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UINT length = masterkey.GetLength();
	
	if (!length)
		return;

	wchar_t first;
	first = masterkey[length - 1];

	if (masterkey[length - 1] < 33 || 126 < masterkey[length - 1]) // 맨 끝 글자가 스페이스거나 이상한 문자면
	{
		MessageBox(TEXT("한글, 스페이스 쓰지 마요"), TEXT("잘못된 비밀번호 입력"), MB_OK | MB_ICONWARNING);
		masterkey.Delete(length - 1, 1); // 답은 뭐다?
		UpdateData(FALSE);
		return; // 거른다
	}

	if (length < 3)
		sec_level.SetWindowTextW(TEXT("이건 그냥 뚫림"));

	else if (length < 8)
		sec_level.SetWindowTextW(TEXT("우리 엄마는 못뚫음"));

	else if (length < 13)
		sec_level.SetWindowTextW(TEXT("뚫는데 노트북으로 10년 이상"));

	else if (length < 23)
		sec_level.SetWindowTextW(TEXT("슈퍼컴퓨터로 반세기 이상"));

	else
		sec_level.SetWindowTextW(TEXT("그만해 이색기야"));
}
