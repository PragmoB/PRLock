#pragma once


// CIOProgress 대화 상자

class CIOProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CIOProgress)

public:
	CIOProgress(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CIOProgress();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IO_PROGRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic progress_text;
	CProgressCtrl progress_graphic;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void SetInfo(ULONGLONG all_byte);
	int unit;

	// 위쪽부터 바이트, 킬로바이트, 메가바이트
	enum
	{
		B = 0,
		KB = 1,
		MB = 2
	};
	ULONGLONG all_byte;
	void OffsetInfo(UINT offset);
	virtual void OnCancel();
	BOOL isValid;
};
