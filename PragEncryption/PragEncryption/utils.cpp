#include "stdafx.h"

BOOL DeleteDirectoryFile(LPCTSTR RootDir)
{
	if (RootDir == NULL)
		return TRUE;

	if (_waccess(RootDir, 0) == -1)
		return TRUE;

	BOOL bRval = FALSE;

	CString szNextDirPath = _T("");
	CString szRoot = _T("");

	// 해당 디렉토리의 모든 파일을 검사한다.
	szRoot.Format(_T("%s\\*.*"), RootDir);

	CFileFind find;

	bRval = find.FindFile(szRoot);

	if (bRval == FALSE)
	{
		return bRval;
	}

	while (bRval)
	{
		bRval = find.FindNextFile();

		// . or .. 인 경우 무시 한다. 
		if (find.IsDots() == FALSE)
		{
			// Directory 일 경우 재귀호출 한다.
			if (find.IsDirectory())
				DeleteDirectoryFile(find.GetFilePath());

			// file일 경우 삭제
			else
				bRval = DeleteFile(find.GetFilePath());
		}
	}
	find.Close();
	bRval = RemoveDirectory(RootDir);
	return bRval;
}