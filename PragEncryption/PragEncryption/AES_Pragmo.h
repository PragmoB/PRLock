#pragma once

#define AES128_BLOCK 16

class AES_Pragmo {

private:
	UCHAR RoundKey[11][16];
	void Shift(UCHAR* target, int times, int length);

	void AddRoundKey(UCHAR* state, const UCHAR* key, int length);
	void SubBytes(UCHAR* state, const UCHAR* S_BOX, int length);
	void ShiftRows(UCHAR* state, BOOL inv);
	void MixColumn(UCHAR * data, size_t data_len);
	void InvMixColumn(UCHAR * data, size_t data_len);

public :
	void KeySchedule(const UCHAR* CipherKey);
	void Encrypt(const UCHAR* source, UCHAR* result);
	void Decrypt(const UCHAR* source, UCHAR* result);
};