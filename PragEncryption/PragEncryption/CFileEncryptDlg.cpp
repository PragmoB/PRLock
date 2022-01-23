// CFileEncryptDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "PragEncryption.h"
#include "CFileEncryptDlg.h"
#include "afxdialogex.h"
#include "CInputEncryptDlg.h"
#include "CInputEncryptDlg.h"
#include "CInputDecryptDlg.h"
#include "CIOProgress.h"
#include "CEnvironmentSetDlg.h"

#include "AES_Pragmo.h"
#include "RSA_Pragmo.h"
#include <openssl/sha.h>
#include "libbcrypt/bcrypt.h"

// CFileEncryptDlg 대화 상자

IMPLEMENT_DYNAMIC(CFileEncryptDlg, CDialogEx)

CFileEncryptDlg::CFileEncryptDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FILE, pParent)
	, FilePath(_T(""))
{

}

CFileEncryptDlg::~CFileEncryptDlg()
{
}

void CFileEncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILE_PATH, FilePath);
	DDX_Control(pDX, IDC_BUTTON_FILE_ENCRYPT, buttFileEncrypt);
	DDX_Control(pDX, IDC_BUTTON_FILE_DECRYPT, buttFileDecrypt);
}


BEGIN_MESSAGE_MAP(CFileEncryptDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FILE_PATH, &CFileEncryptDlg::OnClickedButtonFilePath)
	ON_BN_CLICKED(IDC_BUTTON_FILE_ENCRYPT, &CFileEncryptDlg::OnClickedButtonFileEncrypt)
	ON_BN_CLICKED(IDC_BUTTON_FILE_DECRYPT, &CFileEncryptDlg::OnClickedButtonFileDecrypt)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CFileEncryptDlg 메시지 처리기
CString GetErrorDescription(CException* ex)
{ CString strExceptionDescription;TCHAR szException[1024] = { 0, }; if (ex->GetErrorMessage(szException, 1024)) strExceptionDescription = szException; return strExceptionDescription; } CString GetDetailErrorMessage(DWORD dwErrorCode, CString strErrorDescription, CString& strFile, DWORD& dwLineNo) { CString strErrorMessage; COleDateTime datetime = COleDateTime::GetCurrentTime(); strErrorMessage.Format(_T("Date Time: %s\n") _T("ErrorCode: %d\n\n") _T("%s\n\n") _T("File: %s\n") _T("Line: %d"), datetime.Format(), dwErrorCode, strErrorDescription, strFile, dwLineNo); return strErrorMessage; } void DetailErrorMessageBox(CException* ex, CString strFile, DWORD dwLineNo, UINT uMB_IconButton /*= MB_OK*/) { CString strErrorDescription = GetErrorDescription(ex); CString strDetailErrorMessage = GetDetailErrorMessage(GetLastError(), strErrorDescription, strFile, dwLineNo); ::AfxMessageBox(strDetailErrorMessage, uMB_IconButton); }


BOOL CFileEncryptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	buttFileEncrypt.LoadBitmaps(IDB_BITMAP_FILE_ENCRYPT , NULL, NULL, NULL);
	buttFileDecrypt.LoadBitmaps(IDB_BITMAP_FILE_DECRYPT , NULL, NULL, NULL);

	buttFileEncrypt.SizeToContent();
	buttFileDecrypt.SizeToContent();

	
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	DragAcceptFiles();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CFileEncryptDlg::OnClickedButtonFilePath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog FileDir(TRUE, NULL, NULL, OFN_HIDEREADONLY, TEXT("모든 파일 (*.*)||"));

	if (IDOK ==	FileDir.DoModal())
	{
		FilePath = FileDir.GetPathName();
		UpdateData(FALSE);
	}
}


void CFileEncryptDlg::OnClickedButtonFileEncrypt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (FilePath == "")
	{
		MessageBox(TEXT("암호화할 파일을 선택해주세요."),
			TEXT("알림"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	;
	CFile original, result;
	CFileException eex;

	if (!original.Open(FilePath,
		CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &eex))
	{
		eex.ReportError();
		return;
	}
	if (original.GetLength() == 0)
	{
		MessageBox(TEXT("파일 내용이 없습니다"), TEXT("알림"), MB_OK | MB_ICONERROR);
		return;
	}

	CInputEncryptDlg input;
	RSA_Pragmo key;
	CEnvironmentSetDlg EnvSet;
	char letter[1] = "";

	if (input.DoModal() != IDOK)
		return;
	
	EnvSet.get();

	if (!input.nouse_master && EnvSet.EncKeyPath == "")
	{
		MessageBox(TEXT("환경 설정에서 RSA 암호화키 파일(N.pkey, e.pkey)의 경로를 선택해주세요."),
			TEXT("알림"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	CFile N, e;

	/* 타겟 파일 액세스, 암호화(.pecr)파일 생성. */

	CString FileName, FileDir;
	int letternum = GetFindCharCount(FilePath, '\\');
	int count = 0;
	for (int i = 0; i < FilePath.GetLength(); i++)
	{
		if(FilePath[i] == '\\')
			if (++count >= letternum)
			{
				FileDir = FilePath.Left(i + 1);
				break;
			}
	}
	AfxExtractSubString(FileName, FilePath, letternum, '\\');

	if (!result.Open(FilePath + TEXT(".pecr"),
		CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive | CFile::typeBinary, &eex))
	{
		eex.ReportError();
		return;
	}

	CStringA str;

	if (!input.nouse_master)
	{
		if (!N.Open(EnvSet.EncKeyPath + TEXT("\\N.pkey"),
			CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &eex))
		{
			eex.ReportError();
			return;
		}
		if (!e.Open(EnvSet.EncKeyPath + TEXT("\\e.pkey"),
			CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &eex))
		{
			eex.ReportError();
			return;
		}

		/* 마스터키 액세스 */

		key.N = AccessKey(&N);
		key.e = AccessKey(&e);

		if (key.N == 0 || key.e == 0)
		{
			MessageBox(TEXT("마스터키 액세스 실패"), TEXT("알림"),
				MB_OK | MB_ICONERROR);
			result.Close();
			DeleteFile(FilePath + ".pecr");
			return;
		}
		str = input.normalkey;

		/* 일반 키(normalkey) 암호화 */

		const UCHAR* pChar = (const UCHAR*)str.GetBuffer();

		int length = strlen((const char*)pChar);
		BigInteger m = 0; // 암호화할 수
		for (int i = length - 1; i >= 0; i--)
		{
			/* 
			 * 예를 들어 my_pass를 암호화한다 치면
			 * my_pass를 바이너리 단위로 쪼개면 6D 79 5F 70 61 73 73 이 되는데
			 * 역순으로 합쳐서 0x737361705F796D라는 수를 만드는 과정임.
			 * 이렇게 하는 이유는 RSA 공개키로 일반키를 한꺼번에 암호화하기 위함.
			 */
			m += pChar[i];
			m *= 1 << 8; // BigInteger는 쉬프트 연산이 안되더라
		}
		m /= 1 << 8;
		if (key.N <= m) // 평서문은 N보다 크면 안됨
		{
			MessageBox(TEXT("비밀번호가 너무 길어 마스터키를 등록할 수 없습니다\n마스터키 길이를 늘려주세요"), TEXT("오류"),
				MB_OK | MB_ICONERROR);
			result.Close();
			DeleteFile(FilePath + ".pecr");
			return;
		}
		BigInteger nextnum, num = key.Encrypt(m); // 비밀번호를 암호화 한 결과물 num
		// 파일에 기록
		for (nextnum = num / 10; num != 0; num /= 10)
		{
			letter[0] = (num - nextnum * 10).toInt() + 48;
			result.Write(letter, 1);
			nextnum /= 10;
		}
	}

	letter[0] = 7;
	result.Write(letter, 1);

	/* 비밀번호 체크용 bcrypt 암호화 */

	char bcrypt_hashed[100] = "";

	str = input.normalkey;
	strcpy_s(bcrypt_hashed, 100, bcrypt::generateHash(str.GetBuffer(), 11).c_str());

	result.Write(bcrypt_hashed, strlen(bcrypt_hashed));
	letter[0] = 7; // ascii 7문자로 데이터 끝을 명시
	result.Write(letter, 1);
	
	/* 파일 콘텐츠 암호화 */
	
	ULONGLONG original_length = original.GetLength();
	UINT BUFFER_SIZE = 0;

	if (original_length / 100 >= 64000) // 64KB 이상이면
		BUFFER_SIZE = 63999 - (63999 % AES128_BLOCK); // Read함수의 크기 제한을 넘으므로 안전하게 설정.
	else if (original_length / 1600 == 0) // 1600B 미만이면
		BUFFER_SIZE = AES128_BLOCK;
	else // 그 사이면
		BUFFER_SIZE = original_length / 100 - ((original_length / 100) % AES128_BLOCK);

	CIOProgress* progress = new CIOProgress();
	UINT pNormalkey = 0;
	progress->Create(IDD_DIALOG_IO_PROGRESS, AfxGetMainWnd());
	progress->ShowWindow(SW_SHOW);
	progress->SetInfo(original.GetLength());

	UCHAR* buff = new UCHAR[BUFFER_SIZE + 1];
	MSG msg;

	try {
		int i = 0;
		AES_Pragmo aeskey;
		UCHAR normalsha[33] = "";

		// 비밀번호를 SHA256으로 해쉬 후
		str = input.normalkey;
		SHA256_CTX sha256;
		SHA256_Init(&sha256);
		SHA256_Update(&sha256, (UCHAR*)str.GetBuffer(), strlen(str.GetBuffer()));
		SHA256_Final(normalsha, &sha256);

		// 해쉬 값으로 암호화 키 구성
		aeskey.KeySchedule(normalsha);

		for (i = 0; i < (signed long long)(original_length - BUFFER_SIZE); i += BUFFER_SIZE)
		{
			memset(buff, 0, BUFFER_SIZE + 1);
			original.Seek(i, CFile::begin);
			original.Read(buff, BUFFER_SIZE);

			for (int j = 0; j < BUFFER_SIZE; j += AES128_BLOCK)
				aeskey.Encrypt(buff + j, buff + j);

			result.Write(buff, BUFFER_SIZE);
			progress->OffsetInfo(BUFFER_SIZE);

			// progress 창이 꺼졌을때
			if (!progress->isValid) 
			{
				delete progress;
				delete buff;

				original.Close();
				result.Close();

				MessageBox(TEXT("암호화가 취소되었습니다"), TEXT("알림"), MB_OK | MB_ICONINFORMATION);
				DeleteFile(FilePath + TEXT(".pecr"));
				return;
			}

			PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 마지막 블럭은 조금 특별하게 작업 해줘야 오류가 안생김
		memset(buff, 0, BUFFER_SIZE + 1);
		original.Seek(i, CFile::begin);
		BUFFER_SIZE = original.Read(buff, BUFFER_SIZE);

		// 마지막 블럭은 남겨놓고 암호화 연산 진행
		for (int j = 0; j < (signed int)(BUFFER_SIZE - AES128_BLOCK); j += AES128_BLOCK)
			aeskey.Encrypt(buff + j, buff + j);

		/* 
		 * 암호 블럭 크기가 안맞춰질 경우를 고려,
		 * 쓰레기 데이터를 집어넣어 블럭 크기를 맞추고 암호화 한 뒤
		 */
		int garbage_length = AES128_BLOCK - original.GetLength() % AES128_BLOCK; // 블럭에서 비는 공간 계산
		aeskey.Encrypt(buff + BUFFER_SIZE - BUFFER_SIZE % AES128_BLOCK,
					   buff + BUFFER_SIZE - BUFFER_SIZE % AES128_BLOCK);

		result.Write(buff, BUFFER_SIZE + AES128_BLOCK - BUFFER_SIZE % AES128_BLOCK);
		progress->OffsetInfo(BUFFER_SIZE + AES128_BLOCK - BUFFER_SIZE % AES128_BLOCK);

		// 쓰레기 데이터의 크기를 파일 맨 끝에 삽입함
		letter[0] = garbage_length;
		result.Write(letter, 1);
	}
	catch (CException* ex)
	{
#ifdef _DEBUG
		DetailErrorMessageBox(ex, TEXT(__FILE__), __LINE__, MB_ICONASTERISK | MB_OK);
#else
		MessageBox(TEXT("알 수 없는 오류가 발생했습니다"), TEXT("오류"), MB_ICONERROR | MB_OK);
#endif
	}
	progress->DestroyWindow();

	delete progress;
	delete buff;

	original.Close();
	result.Close();

	if (EnvSet.FILE_AUTO_DELETE)
		DeleteFile(FilePath);
}

UINT WINAPI system_wrap(void* p)
{
	char buf[100] = "";
	strcpy_s(buf, (const char*)p);
	system(buf);
	return 0;
}
void CFileEncryptDlg::OnClickedButtonFileDecrypt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (FilePath == "")
	{
		MessageBox(TEXT("복호화할 파일을 선택해주세요."),
			TEXT("알림"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	CInputDecryptDlg input;
	CEnvironmentSetDlg EnvSet;
	RSA_Pragmo key;
	char letter[1] = "";
	UINT FileContentStart = 0;
	CString str;
	CStringA strA;

	EnvSet.get();
	/* 원본 파일(original) 액세스, 복호화된 파일(result) 생성 */

	CFile original, result;
	CFileException eex;

	CString EncryptedFileName, FileDir, DecryptedFileName;
	int count = 0;
	for (int i = 0; i < FilePath.GetLength(); i++)
	{
		if (FilePath[i] == '\\')
			if (++count >= GetFindCharCount(FilePath, '\\'))
			{
				FileDir = FilePath.Left(i + 1);
				break;
			}
	}
	// 예를 들어 thefile.txt.pecr라는 문자열이 있을때, 여기서 .pecr을 떼어내 thefile.txt을 얻어내는 과정.
	CString temp;
	AfxExtractSubString(EncryptedFileName, FilePath, GetFindCharCount(FilePath, '\\'), '\\');

	AfxExtractSubString(temp, EncryptedFileName, GetFindCharCount(EncryptedFileName, '.'), '.');
	if (temp != TEXT("pecr"))
	{
		MessageBox(TEXT("암호화된 파일이 아닙니다"), TEXT("오류"), MB_OK | MB_ICONERROR);
		return;
	}

	for (int i = 0; i < GetFindCharCount(EncryptedFileName, '.') - 1; i++)
	{
		AfxExtractSubString(temp, EncryptedFileName, i, '.');
		DecryptedFileName += temp + '.';
	}
	AfxExtractSubString(temp, EncryptedFileName, GetFindCharCount(EncryptedFileName, '.') - 1, '.');
	DecryptedFileName += temp;
	// ------------------------------------------------------------------------------------------------

	if (!original.Open(FilePath,
		CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &eex))
	{
		eex.ReportError();
		return;
	}
	if (original.GetLength() == 0)
	{
		MessageBox(TEXT("파일 내용이 없습니다"), TEXT("알림"), MB_OK | MB_ICONERROR);
		return;
	}

	if (input.DoModal() != IDOK)
		return;

	if (input.KeyOpt == 0) // 마스터키 복호화 옵션이면
	{
		if (EnvSet.DecKeyPath == "") // 키 파일을 선택하지 않았을경우
		{
			MessageBox(TEXT("환경 설정에서 RSA 복호화키 파일(N.pkey, d.pkey) 경로를 선택해주세요."), TEXT("알림"),
				MB_OK | MB_ICONINFORMATION);
			return;
		}
		/* 마스터키 액세스 */

		CFile N, d;

		if (!N.Open(EnvSet.DecKeyPath + TEXT("\\N.pkey"),
			CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &eex))
		{
			eex.ReportError();
			return;
		}
		if (!d.Open(EnvSet.DecKeyPath + TEXT("\\d.pkey"),
			CFile::modeRead | CFile::shareExclusive | CFile::typeBinary, &eex))
		{
			eex.ReportError();
			return;
		}

		key.N = AccessKey(&N);
		key.d = AccessKey(&d);

		if (key.N == 0 || key.d == 0)
		{
			MessageBox(TEXT("마스터키 액세스 실패"), TEXT("알림"),
				MB_OK | MB_ICONERROR);
			return;
		}

		/* 일반 키(normalkey) 복호화 */
		BigInteger digit = 1, e_normalkey = 0;
		UINT i = 0, j = 0;
		
		for (i = 0; letter[0] != 7; i++)
		{
			original.Seek(i, CFile::begin);
			original.Read(letter, 1);
			e_normalkey += digit * (letter[0] - 0x30);
			digit *= 10;
		}
		if (i == 1) // 마스터키가 없는경우
		{
			MessageBox(TEXT("마스터키가 등록되지 않았습니다"), TEXT("알림"),
				MB_OK | MB_ICONERROR);
			return;
		}

		// ascii 7문자도 더해버렸으므로 빼줘야함
		digit /= 10;
		e_normalkey -= digit * (letter[0] - 0x30);
		BigInteger d_normalkey = key.Decrypt(e_normalkey); // 일반키 복호화
		/*
		 * 복호화된 일반키는 각각의 바이트가 하나의 수로 합쳐져있어 이를 분리하는 작업이 필요함.
		 * 예를 들어 일반키가 my_pass이면
		 * 복호화 직후 값은 이렇게 나올 것임 : 0x737361705F796D
		 * 이를 쉬프트 연산을 사용해 바이트 단위로 쪼개서 6D 79 5F 70 61 73 73로 만드는 과정이 필요함.
		 * 아래 코드는 그 작업을 수행함
		 */
		UCHAR normalkey[1000] = { 0, };
		for (int i = 0;d_normalkey != 0; i++)
		{
			normalkey[i] = (d_normalkey - ((d_normalkey / 0x100) * 0x100)).toInt();
			d_normalkey /= 0x100;
		}

		input.normalkey = normalkey;

		N.Close();
		d.Close();

		if (MessageBox(TEXT("비밀번호: ") + input.normalkey + TEXT("\n복호화를 계속하시겠습니까?"),
			TEXT("비밀번호 복호화 완료"), MB_YESNO) == IDNO)
			return;
	}
	/* 콘텐츠 시작지점 계산 */
	{
		UINT i;
		letter[0] = 0;
		for (i = 0; letter[0] != 7; i++)
		{
			original.Seek(i, CFile::begin);
			original.Read(letter, 1);
		}
		FileContentStart = i;
	}

	/* bcrypt로 비밀번호 일치 여부 확인 */

	char bcrypt_hashed[100] = "";
	original.Seek(FileContentStart, CFile::begin);
	original.Read(bcrypt_hashed, 100);
	
	for (int i = 0; i < 100; i++)
		if (bcrypt_hashed[i] == 7)
			bcrypt_hashed[i] = 0;

	FileContentStart += strlen(bcrypt_hashed) + 1;

	strA = input.normalkey;
	if (!bcrypt::validatePassword(std::string(strA.GetBuffer()), std::string(bcrypt_hashed)))
	{
		MessageBox(TEXT("비밀번호가 일치하지 않습니다"), TEXT("알림"), MB_OK | MB_ICONERROR);
		return;
	}

	/* 파일 콘텐츠 복호화 */

	// 복호화될 파일 생성
	TCHAR AppdataDir[50] = L"";
	switch (input.OpenOpt)
	{
	case TEMP : // 임시로 열기인 경우

		// AppData의 cache경로에 임시적으로 생성

		GetEnvironmentVariable(TEXT("APPDATA"), AppdataDir, 50);
		str = str + AppdataDir + TEXT("\\PRLock ") + EnvSet.version;
		CreateDirectory(str, NULL);
		str += TEXT("\\cache");
		CreateDirectory(str, NULL);
		if (!result.Open(str + TEXT("\\") + DecryptedFileName,
			CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeBinary, &eex))
		{
			eex.ReportError();
			return;
		}
		break;
	default : // 기본은 원래 있는 자리(복호화 대상 파일이 있는 경로)에 생성하는게 원칙
		if (!result.Open(FileDir + DecryptedFileName,
			CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeBinary, &eex))
		{
			eex.ReportError();
			return;
		}
	}

	UINT pNormalkey = 0;
	CIOProgress* progress = new CIOProgress();

	UINT BUFFER_SIZE = 0;
	ULONGLONG original_length = original.GetLength();
	UINT buffer_module;

	if (original_length / 100 >= 64000) // 64KB 이상이면
		BUFFER_SIZE = 63998 - (63998 % AES128_BLOCK); // Read함수의 크기 제한을 넘으므로 안전하게 설정.
	else if (original_length / 1600 == 0) // 1600B 미만이면
		BUFFER_SIZE = AES128_BLOCK;
	else // 그 사이면
		BUFFER_SIZE = original_length / 100 - ((original_length / 100) % AES128_BLOCK);

	// 다 복호화 하고 나서 쓰레기 데이터의 크기를 기록한 바이트가 마지막에 남게 되면
	if ((original_length - FileContentStart) % BUFFER_SIZE == 1)
		buffer_module = 2 * BUFFER_SIZE;
	else
		buffer_module = BUFFER_SIZE;

	progress->Create(IDD_DIALOG_IO_PROGRESS, AfxGetMainWnd());
	progress->ShowWindow(SW_SHOW);
	progress->SetInfo(original.GetLength() - FileContentStart);

	if (input.KeyOpt == 1)
	{
		for (int i = 0; letter[0] != 7; i++)
		{
			original.Seek(i, CFile::begin);
			original.Read(letter, 1);
		}
	}

	UCHAR* buff = new UCHAR[BUFFER_SIZE + 1];
	try {
		MSG msg;
		int i = 0;

		strA = input.normalkey;

		AES_Pragmo aeskey;
		UCHAR normalsha[33] = "";

		// 비밀번호를 SHA256으로 해쉬 후
		SHA256_CTX sha256;
		SHA256_Init(&sha256);
		SHA256_Update(&sha256, (UCHAR*)strA.GetBuffer(), strlen(strA.GetBuffer()));
		SHA256_Final(normalsha, &sha256);

		// 해쉬값으로 복호화 키 구성
		aeskey.KeySchedule(normalsha);

		for (i = FileContentStart; i < (signed long long)(original_length - buffer_module); i += BUFFER_SIZE)
		{
			memset(buff, 0, BUFFER_SIZE + 1);
			original.Seek(i, CFile::begin);
			original.Read(buff, BUFFER_SIZE);
			
			for (int j = 0; j < BUFFER_SIZE; j += AES128_BLOCK)
				aeskey.Decrypt(buff + j, buff + j);

			result.Write(buff, BUFFER_SIZE);
			progress->OffsetInfo(BUFFER_SIZE);

			if (!progress->isValid)
			{
				delete progress;
				delete buff;

				original.Close();
				result.Close();

				MessageBox(TEXT("복호화가 취소되었습니다"), TEXT("알림"), MB_OK | MB_ICONINFORMATION);
				DeleteFile(FileDir + DecryptedFileName);
				return;
			}

			PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		memset(buff, 0, BUFFER_SIZE + 1);
		original.Seek(i, CFile::begin);
		BUFFER_SIZE = original.Read(buff, BUFFER_SIZE + 1);

		for(i = 0;i < (signed int)(BUFFER_SIZE - AES128_BLOCK);i +=  AES128_BLOCK)
			aeskey.Decrypt(buff + i, buff + i);

		/*
		 * 암호블럭 크기를 맞추기 위해 어쩔수 없이 끼워넣은
		 * 쓰레기 데이터의 크기를 읽어들임
		 */
		letter[0] = buff[BUFFER_SIZE - 1];

		// 쓰레기 데이터의 크기만큼 결과에서 제외시킴
		result.Write(buff, BUFFER_SIZE - letter[0] - 1);
		progress->OffsetInfo(BUFFER_SIZE - letter[0] - 1);
	}
	catch (CException* ex)
	{
#ifdef _DEBUG
		DetailErrorMessageBox(ex, TEXT(__FILE__), __LINE__, MB_ICONASTERISK | MB_OK);
#else
		MessageBox(TEXT("알 수 없는 오류가 발생했습니다"), TEXT("오류"), MB_ICONERROR | MB_OK);
#endif

	}
	progress->DestroyWindow();
	delete progress;
	delete buff;

	original.Close();
	result.Close();

	// 복호화된 파일 열기 옵션에 따라 다르게 동작
	switch (input.OpenOpt)
	{
	case TEMP : // 임시로 열기 : %appdata%\\PRLock\\cache에 복호화된 파일 생성 후 cmd로 열기
		str = AppdataDir;
		strA = TEXT("\"") + str + TEXT("\\PRLock ") + EnvSet.version + TEXT("\\cache\\") + DecryptedFileName + TEXT("\"");
		setlocale(LC_ALL, ""); // cmd에서 한글 지원

		DWORD dwThreadID;
		_beginthreadex(NULL, 0, system_wrap, strA.GetBuffer(), 0, (unsigned*)&dwThreadID);
		break;

	default :
		if (EnvSet.FILE_AUTO_DELETE)
			DeleteFile(FilePath);
	}

}

BigInteger CFileEncryptDlg::AccessKey(CFile* file)
{
	// TODO: 여기에 구현 코드 추가.
	UCHAR letter[1] = "";
	BigInteger digit = 1;
	UCHAR masterkeysha[64] = ""; // 마스터키를 SHA256한 결과를 저장.
	UINT pMasterkey = 0; // xor 암호화를 풀기 위한 포인터.

	BigInteger result = 0;

	try {
		for (UINT i = 1; file->Read(letter, 1); i++)
		{
			int decrypted = letter[0] - 48;

			// (마스터키 비밀번호를 등록 했을 경우만 해당)복호화했을때 데이터가 깨지면
			if (decrypted < 0 || 9 < decrypted)
			{
				result = FALSE; // 실패!
				break;
			}
			result += digit * decrypted;
			digit *= 10;
			if (++pMasterkey >= 64) // (마스터키 비밀번호를 등록 했을 경우만 해당)
				pMasterkey = 0;

			file->Seek(i, CFile::begin);
		}

	}
	catch (CFileException& e)
	{
		e.ReportError();
	}
	return result;
}


int CFileEncryptDlg::GetFindCharCount(CString& parm_string, char parm_find_char)
{
	// TODO: 여기에 구현 코드 추가.
	int length = parm_string.GetLength(), find_count = 0;
	for (int i = 0; i < length; i++)
	{
		if (parm_string[i] == parm_find_char)
			find_count++;
	}
	return	find_count;
}


void CFileEncryptDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TCHAR FileName[MAX_PATH] = { 0, };
	UINT count = DragQueryFile(hDropInfo, -1, FileName, MAX_PATH); // 드래그된 파일 개수
	DragQueryFile(hDropInfo, 0, FileName, MAX_PATH);
	FilePath = FileName;
	UpdateData(FALSE);
	CDialogEx::OnDropFiles(hDropInfo);
}
