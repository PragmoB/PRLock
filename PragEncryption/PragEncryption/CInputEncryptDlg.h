#pragma once


// CInputEncryptDlg 대화 상자

class CInputEncryptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputEncryptDlg)

public:
	CInputEncryptDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CInputEncryptDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INPUT_ENCRYPT_KEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOk();
//	CString masterkey;
	CString normalkey;
//	BOOL use_master;
	afx_msg void OnClickedCheckNoUseMaster();
	BOOL nouse_master;
	CString m_edit_password_check;
};
