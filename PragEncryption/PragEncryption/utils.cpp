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

	// �ش� ���丮�� ��� ������ �˻��Ѵ�.
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

		// . or .. �� ��� ���� �Ѵ�. 
		if (find.IsDots() == FALSE)
		{
			// Directory �� ��� ���ȣ�� �Ѵ�.
			if (find.IsDirectory())
				DeleteDirectoryFile(find.GetFilePath());

			// file�� ��� ����
			else
				bRval = DeleteFile(find.GetFilePath());
		}
	}
	find.Close();
	bRval = RemoveDirectory(RootDir);
	return bRval;
}