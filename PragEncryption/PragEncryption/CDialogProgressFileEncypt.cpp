// CDialogProgressFileEncypt.cpp: 구현 파일
//

#include "stdafx.h"
#include "PragEncryption.h"
#include "CDialogProgressFileEncypt.h"
#include "afxdialogex.h"


// CDialogProgressFileEncypt 대화 상자

IMPLEMENT_DYNAMIC(CDialogProgressFileEncypt, CDialogEx)

CDialogProgressFileEncypt::CDialogProgressFileEncypt(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FILE_ENCYPT_PROGRESS, pParent)
{

}

CDialogProgressFileEncypt::~CDialogProgressFileEncypt()
{
}

void CDialogProgressFileEncypt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SIZE, static_size);
}


BEGIN_MESSAGE_MAP(CDialogProgressFileEncypt, CDialogEx)
END_MESSAGE_MAP()


// CDialogProgressFileEncypt 메시지 처리기
