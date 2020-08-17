# PRLock
Free file encryptor

Language MFC

Built with Visual Studio 2017

I'm afraid I didn't use git before.
so, release 1.0.0, 1.0.1 and 1.0.2 don't exist now :(

Usage

일반키 NormalKey : The password encrypting source file

RSA Encrypt/Decrypt key file(further option, not necessarily required) : The key encrypting NormalKey

마스터키 MasterKey : The password encrypting RSA key file

you can decrypt encrypted file by both NormalKey or MasterKey after register RSA Encrypt/Decrypt key path in environment settings if you encrypted NormalKey by RSA key file.
