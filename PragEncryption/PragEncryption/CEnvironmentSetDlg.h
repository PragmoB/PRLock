#pragma once


// CEnvironmentSetDlg 대화 상자

class CEnvironmentSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnvironmentSetDlg)

public:
	CEnvironmentSetDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEnvironmentSetDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ENVIRONMENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int update();
	int get();
	int FILE_AUTO_DELETE;
	afx_msg void OnClickedCheckAutoDelete();
	void UpdateData(BOOL st);
	afx_msg void OnBnClickedButtonEncryptKeyPath();
	afx_msg void OnBnClickedButtonDecryptKeyPath();
	CString DecKeyPath;
	CString EncKeyPath;
	afx_msg void OnBnClickedButtonEnvSave();
};
