#pragma once


// CDialogProgressFileEncypt 대화 상자

class CDialogProgressFileEncypt : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogProgressFileEncypt)

public:
	CDialogProgressFileEncypt(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogProgressFileEncypt();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE_ENCYPT_PROGRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic static_size;
};
