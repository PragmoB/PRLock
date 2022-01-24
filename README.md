What is PRLock?
=============================
Free file encryptor less risky from losing password

You can free from this risk by setting "masterkey" option

The file masterkey-registered can be decrypted by masterkey whenever losing password or not

So, you should keep this masterkey secure from hackers

This util is designed more useful for organization using complex information management system than indivisual

Usage
=============================
### 기본 암/복호화
파일 선택 후 비밀번호를 입력하고 암호화 합니다

![비밀번호로 암호화](https://user-images.githubusercontent.com/67177785/150730144-13d9567f-1e4f-439d-9d03-c3b9ae075fa8.PNG)

**그림 1 | 비밀번호로 암호화**

결과적으로 테스트.txt.pecr을 생성합니다

이렇게 암호화된 파일을 비밀번호를 입력해서 복호화 할 수 있습니다

![비밀번호로 복호화](https://user-images.githubusercontent.com/67177785/150730164-c8a7c465-cd21-442c-9474-c11632c8b979.PNG)

**그림 2 | 비밀번호로 복호화**

완전히 복호화 하지 않고 잠깐 내용만 확인하고 싶은 경우 임시로 열기를 선택합시다

임시로 열린 파일은 프로그램 종료 전 꼭 닫아줘야 합니다

![캐시 파일 삭제 실패](https://user-images.githubusercontent.com/67177785/150730173-bc39222c-3e70-4cf9-9d64-4676869f3cc3.PNG)

**그림 3 | 캐시 삭제 실패**

이 오류는 임시로 열린 파일이 닫히지 않은 상태에서 프로그램을 끄려고 시도했을 경우 발생합니다

### 마스터키 기능

비밀번호 분실에 대비하기 위해 마스터키를 만들어봅시다

키 길이가 길어질수록 보안이 견고해지지만, 그만큼 시간도 오래 걸립니다

보통 2048bit를 주로 사용합니다

![마스터키 생성중](https://user-images.githubusercontent.com/67177785/127697147-f9b61375-e187-42a5-b0b3-cd8dac70047e.PNG)

**그림 4 | 마스터키 생성중**

응답 없음으로 뜨지만 걱정 마세요, 제대로 작동 중입니다

![키 파일 생성 확인](https://user-images.githubusercontent.com/67177785/126549268-ac447802-bb05-4346-b526-edf56194593d.PNG)

**그림 5  | 키 파일 생성 확인**

암호화 키, 복호화 키 한 쌍이 잘 생성 됬습니다

암호화 키는 파일을 암호화 할 때 사용하고, 복호화 키는 복호화 할 때 사용합니다

복호화 키 정보는 중요하므로 USB나 아니면 다른 어딘가에 숨기는게 좋습니다

환경 설정 탭에서 생성된 키 파일의 경로를 붙여넣고 저장합니다

![마스터키 환경설정](https://user-images.githubusercontent.com/67177785/126549316-54986771-fe54-46e6-b688-c4263473864c.PNG)

**그림 6 | 마스터키 환경 설정**

복호화 키 정보는 중요하므로 컴퓨터에서 삭제하고 어딘가에 숨겨놓았습니다

이제 파일을 암호화 할 때 마스터키를 등록할 수 있습니다

마스터키를 등록하고 싶지 않다면 등록 안함 옵션을 체크해줍시다

![암호화시 마스터키 등록](https://user-images.githubusercontent.com/67177785/126549343-4142c997-0045-4ab8-960a-0050054c7d25.PNG)

**그림 7 | 암호화 시 마스터키 등록**

이런! 파일을 암호화했는데 비밀번호를 까먹었네요ㅠㅠ

숨겨뒀던 복호화 키를 사용할 차례입니다

환경설정에서 복호화 키를 설정하고 마스터키로 복호화를 해봅시다

![마스터키로 복호화](https://user-images.githubusercontent.com/67177785/150730196-c3b6c824-a4b2-4a99-99ad-3bb56b92e7c4.PNG)

**그림 7 | 마스터키로 복호화**

다행입니다
 
암호화된 파일에 마스터키가 등록됬기에 비밀번호를 분실해도 복구 할 수 있었습니다

### 비밀번호 길이

비밀번호 길이가 너무 길어 암호화를 거부하는 경우가 있습니다
 
이는 마스터키를 등록할 때 마스터키의 길이가 비밀번호 길이보다 짧아 수학적인 오류가 발생한 것으로 해결방법은 다음과 같습니다 :
 
1. 마스터키를 등록하지 않는다
2. 마스터키 길이를 늘인다
3. 비밀번호를 줄인다

Build Settings
=============================
### Tool
- Visual Studio 2017

### Framework
- MFC

### Library

OpenSSL
-------------------------
download OpenSSL : http://slproweb.com/products/Win32OpenSSL.html

![OpenSSL 다운로드](https://user-images.githubusercontent.com/67177785/127702119-9bc2e801-1535-4f3a-9c00-0fc92ca8325d.PNG)
 
**OpenSSL download option**
 
and set library path after donwloading
 
![프로젝트 인클루드 셋팅](https://user-images.githubusercontent.com/67177785/127702158-bb31a936-3b85-4a57-9307-63fb0ae45b54.PNG)
 
**OpenSSL include settings**
 
![프로젝트 링크 경로 셋팅](https://user-images.githubusercontent.com/67177785/127749501-5f481aa3-2df4-4166-b2cf-af19e676d07a.PNG)
 
**OpenSSL linker settings**

This underlined settings are right for my environment, maybe not also for yours
 
You have to modify this suitable for your environment as forming inclusion directory $(your OpenSSL Path)\include, linker directory $(your OpenSSL Path)\lib\VC\static

License
==========================

This project is licensed under the terms of GPL-3.0 license.