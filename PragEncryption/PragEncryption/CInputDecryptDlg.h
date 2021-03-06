#pragma once


// CInputDecryptDlg 대화 상자

class CInputDecryptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDecryptDlg)

public:
	CInputDecryptDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CInputDecryptDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INPUT_DECRYPT_KEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int KeyOpt;
	afx_msg void OnClickedButtonOk();
	afx_msg void OnBnClickedDecOpt(UINT KeyOpt);
	CString normalkey;
	virtual BOOL OnInitDialog();
};
