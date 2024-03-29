// CIOProgress.cpp: 구현 파일
//

#include "stdafx.h"
#include "PragEncryption.h"
#include "CIOProgress.h"
#include "afxdialogex.h"

// CIOProgress 대화 상자

IMPLEMENT_DYNAMIC(CIOProgress, CDialogEx)

CIOProgress::CIOProgress(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_IO_PROGRESS, pParent)
{

	unit = 0;
	all_byte = 0;
}

CIOProgress::~CIOProgress()
{
}

void CIOProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, progress_text);
	DDX_Control(pDX, IDC_PROGRESS, progress_graphic);
}

BEGIN_MESSAGE_MAP(CIOProgress, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int CIOProgress::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	isValid = TRUE;

	/* 오른쪽 위 x버튼 비활성화 */
	HWND m_hwnd = AfxGetMainWnd()->m_hWnd;
	//SetClassLong(m_hwnd, GCL_STYLE, GetClassLong(m_hwnd, GCL_STYLE) | CS_NOCLOSE); 

	return 0;
}



void CIOProgress::SetInfo(ULONGLONG all_byte)
{
	// TODO: 여기에 구현 코드 추가.

	progress_graphic.SetRange32(0, all_byte);
	progress_graphic.SetPos(0);

	this->all_byte = all_byte;

	CString str, unit;
	if (all_byte > 999999) // 100만 바이트 이상이면
	{
		unit = TEXT("MB");
		this->unit = MB;
	}
	else if (all_byte > 999) // 1000 바이트 이상이면
	{
		unit = TEXT("KB");
		this->unit = KB;
	}
	else
	{
		unit = TEXT("B");
		this->unit = B;
	}
	str.Format(TEXT("%d"), all_byte);
	str = str + unit + TEXT(" 중 0") + unit + TEXT(" 완료됨 (0%)");
	progress_text.SetWindowTextW(str);

	UpdateData(FALSE);
}


void CIOProgress::OffsetInfo(UINT offset)
{
	// TODO: 여기에 구현 코드 추가.

	CString str;
	progress_graphic.OffsetPos(offset);

	switch (unit)
	{
	case B:
		str.Format(TEXT("%dB 중 %dB 완료됨 (%d%%)"),
			all_byte, progress_graphic.GetPos(), 100 * progress_graphic.GetPos() / all_byte);
		break;

	case KB:
		str.Format(TEXT("%dKB 중 %dKB 완료됨 (%d%%)"),
			all_byte / 1000, progress_graphic.GetPos() / 1000, 100 * progress_graphic.GetPos() / all_byte);
		break;

	case MB:
		str.Format(TEXT("%dMB 중 %dMB 완료됨 (%d%%)"),
			all_byte / 1000000, progress_graphic.GetPos() / 1000000, progress_graphic.GetPos() / (all_byte / 100));
		break;
	}
	progress_text.SetWindowTextW(str);
}


void CIOProgress::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	isValid = FALSE;

	CDialogEx::OnCancel();
}
