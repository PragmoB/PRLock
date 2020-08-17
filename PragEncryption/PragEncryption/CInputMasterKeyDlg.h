#pragma once


// CInputMasterKeyDlg 대화 상자

class CInputMasterKeyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputMasterKeyDlg)

public:
	CInputMasterKeyDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CInputMasterKeyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INPUT_MASTER_KEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString masterkey;
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeEditMasterKey();
	CStatic sec_level;
	afx_msg void OnBnClickedCancel();
};
