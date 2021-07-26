#pragma once

#include "BigInteger/BigInteger.h"

// CFileEncryptDlg 대화 상자

class CFileEncryptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileEncryptDlg)

public:
	CFileEncryptDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFileEncryptDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString FilePath;
	afx_msg void OnClickedButtonFilePath();
	afx_msg void OnClickedButtonFileEncrypt();
	afx_msg void OnClickedButtonFileDecrypt();
private:
	BigInteger AccessKey(CFile* file);
	int GetFindCharCount(CString& parm_string, char parm_find_char);
public:
	virtual BOOL OnInitDialog();
	CBitmapButton buttFileEncrypt;
	CBitmapButton buttFileDecrypt;
	afx_msg void OnEnChangeEditFilePath();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
