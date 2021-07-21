#pragma once

#include "BigInteger/BigInteger.h"

// CKeyEncryptDlg 대화 상자

class CKeyEncryptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKeyEncryptDlg)

public:
	CKeyEncryptDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CKeyEncryptDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_KEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString KeyPath;
	afx_msg void OnClickedButtonKeyPath();
	afx_msg void OnClickedButtonKeySet();
	afx_msg void OnClickedButtonKeyUpdate();
	int WriteBigint(CFile* file, BigInteger num, u_char* key = NULL);
	int key_length;
};
