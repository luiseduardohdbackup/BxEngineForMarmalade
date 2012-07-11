#pragma once
#include "BxString.hpp"

//! \brief HTTP기반 쿼리송수신, 파일다운로드
class BxWebContent
{
	byte* ResponseBytes;
	int ResponseLength;

public:
	BxWebContent() : ResponseBytes(null), ResponseLength(0) {}
	~BxWebContent() {BxFree(ResponseBytes);}

	/*!
	\brief 캐시폴더이름 받기
	\return 캐시폴더이름
	*/
	global_func inline string GetCachePath() {return "webcontent_cache/";}

	/*!
	\brief 쿼리결과 받기
	\return 쿼리결과 바이트배열
	*/
	inline const byte* _tmp_ GetBytes() {return ResponseBytes;}

	/*!
	\brief 쿼리결과의 사이즈얻기(바이너리)
	\return 바이트길이
	*/
	inline int GetDataLength() {return ResponseLength;}

	/*!
	\brief 쿼리결과의 사이즈얻기(스트링)
	\return 바이트길이
	*/
	inline int GetTextLength() {return BxUtilGlobal::StrLenEx((string) ResponseBytes);}

	/*!
	\brief HTTP쿼리실행
	\param Domain : 접속할 서버(예: "www.naver.com")
	\param Port : 포트번호(HTTP는 보통 80번포트)
	\param Path : 다운받을 파일 또는 실행할 서블릿의 경로(예: "Update/Run.aspx")
	\param Arguments : 서블릿의 경우 전달할 인자들(예: "Data0=aaa&Data1=bbb&Data2=ccc")
	\param Timeout : 각 단계별 지연허용시간
	\param Progress : 프로그레스의 전달
	\return 송수신 성공여부
	*/
	bool Query(string Domain, ushort Port, string Path, string Arguments = null, int Timeout = -1, callback_progress Progress = null)
	{
		if(Timeout < 0) Timeout = 3000;
		// 소켓연결, 쿼리제작 및 전송
		id_socket Socket = BxCore::Socket::Create();
		if(connect_connected == BxCore::Socket::Connect(Socket, Domain, Port, Timeout, Progress))
		{
			char RequestChars[4096];
			BxCore::Util::SnPrintf(RequestChars, 4096,
				"GET /%s%s%s HTTP/1.1\r\n"
				"Accept: text/html, application/xhtml+xml, */*\r\n"
				"Accept-Language: ko-KR\r\n"
				"User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0\r\n"
				"Accept-Encoding: gzip, deflate\r\n"
				"Host: %s\r\n"
				"Connection: Keep-Alive\r\n"
				"\r\n", Path, (!Arguments || !*Arguments)? "" : "?", (!Arguments)? "" : Arguments, Domain);
			int SendResult = 0, SendedLength = 0;
			const int RequestCharsLength = BxUtilGlobal::StrLen(RequestChars);
			do
			{
				if(Progress && !Progress(-1, 0)) return false; // 업데이트
				SendResult = BxCore::Socket::Send(Socket, RequestChars + SendedLength, RequestCharsLength - SendedLength);
				if(SendResult < 0)
				{
					BxCore::Socket::Release(Socket);
					return false; // 송신실패
				}
			}
			while((SendedLength += SendResult) < RequestCharsLength);
			BxCore::System::Sleep(0);
		}
		else
		{
			BxCore::Socket::Release(Socket);
			return false; // 연결실패
		}

		// HTTP헤더 수신
		char ResponseChars[4096];
		char* EndCodeSearchFocus = ResponseChars;
		int RecvResult = 0, RecvedLength = 0;
		int ErrorTime = 0, SleepTime = 0;
		const int HTMLEndCodeLength = BxUtilGlobal::StrLen("\r\n\r\n");
		while(0 <= (RecvResult = BxCore::Socket::Recv(Socket, ResponseChars + RecvedLength, (4096 - 1) - RecvedLength)))
		{
			if(Progress && !Progress(-1, 0)) return false; // 업데이트
			RecvedLength += RecvResult;
			// 받은 내용이 있을 경우
			if(0 < RecvResult)
			{
				ErrorTime = 0;
				SleepTime >>= 1;
				// HTML헤더 종료코드 검색
				if(HTMLEndCodeLength < RecvedLength)
				{
					ResponseChars[RecvedLength] = '\0';
					while(*(++EndCodeSearchFocus) && BxUtilGlobal::StrSameCount(EndCodeSearchFocus, "\r\n\r\n") != HTMLEndCodeLength);
					if(*EndCodeSearchFocus)
					{
						EndCodeSearchFocus += HTMLEndCodeLength;
						// 데이터사이즈 조사
						char* ContentLengthSearchFocus = ResponseChars;
						while(++ContentLengthSearchFocus < EndCodeSearchFocus && BxUtilGlobal::StrSameCount(ContentLengthSearchFocus, "Content-Length:") != 15);
						ContentLengthSearchFocus += 15;
						if(ContentLengthSearchFocus == EndCodeSearchFocus)
						{
							BxCore::Socket::Release(Socket);
							return false; // 데이터사이즈 정보없음
						}
						// 버퍼확보
						BxFree(ResponseBytes);
						ResponseLength = BxUtilGlobal::AtoI(ContentLengthSearchFocus, EndCodeSearchFocus - ContentLengthSearchFocus);
						ResponseBytes = (byte*) BxAlloc(ResponseLength + 1);
						break;
					}
					else EndCodeSearchFocus = (char*) BxUtilGlobal::Max((int) ResponseChars, (int)(EndCodeSearchFocus - HTMLEndCodeLength));
				}
			}
			// 받은 내용이 없을 경우
			else if(Timeout < (ErrorTime += SleepTime))
			{
				BxCore::Socket::Release(Socket);
				return false; // 대기시간초과
			}
			else BxCore::System::Sleep(BxUtilGlobal::Min(SleepTime++, Timeout >> 6));
		}
		if(RecvResult < 0)
		{
			BxCore::Socket::Release(Socket);
			return false; // 수신실패
		}

		// 데이터 이관
		RecvedLength = ResponseChars + RecvedLength - EndCodeSearchFocus;
		BxCore::Util::MemMove(ResponseBytes, EndCodeSearchFocus, RecvedLength);
		if(Progress && !Progress(RecvedLength, ResponseLength)) // 프로그레스
			return false;

		// 데이터 수신
		RecvResult = 0;
		if(RecvedLength < ResponseLength)
		{
			// 추가수신
			ErrorTime = 0, SleepTime = 0;
			while(0 <= (RecvResult = BxCore::Socket::Recv(Socket, ResponseBytes + RecvedLength, ResponseLength - RecvedLength)))
			{
				RecvedLength += RecvResult;
				if(Progress && !Progress(RecvedLength, ResponseLength)) // 프로그레스
					return false;
				// 받은 내용이 있을 경우
				if(0 < RecvResult)
				{
					ErrorTime = 0;
					SleepTime >>= 1;
					// 추가수신완료
					if(RecvedLength == ResponseLength)
						break;
				}
				// 받은 내용이 없을 경우
				else if(Timeout < (ErrorTime += SleepTime))
				{
					BxCore::Socket::Release(Socket);
					return false; // 대기시간초과
				}
				else BxCore::System::Sleep(BxUtilGlobal::Min(SleepTime++, Timeout >> 6));
			}
		}
		BxCore::Socket::Release(Socket);
		if(RecvResult < 0)
		{
			BxFree(ResponseBytes);
			ResponseLength = 0;
			return false; // 수신실패
		}
		ResponseBytes[ResponseLength] = '\0';
		return true;
	}

	/*!
	\brief 캐시파일 로드(없으면 생성)
	\param Domain : 접속할 서버(예: "www.naver.com")
	\param Port : 포트번호(HTTP는 보통 80번포트)
	\param FilePath : 다운받을 파일경로(예: "Update/Abc.bmp")
	\param ValidFileSize : 유효체크용 파일사이즈(-1은 체크안함)
	\param DoLoad : 로딩할지의 여부
	\param Progress : 프로그레스의 전달
	\return 캐시로드 성공여부
	*/
	bool Cache(string Domain, ushort Port, string FilePath, int ValidFileSize = -1, bool DoLoad = true, callback_progress Progress = null)
	{
		// 캐시로드
		BxString CacheFileName = BxString("webcontent_cache/") + FilePath;
		if(BxCore::File::IsExist(CacheFileName))
		{
			id_file CacheFile = BxCore::File::Open(CacheFileName, "rb");
			const int FileSize = BxCore::File::GetSize(CacheFile);
			if(FileSize == ValidFileSize || ValidFileSize == -1)
			{
				if(DoLoad)
				{
					ResponseLength = FileSize;
					BxFree(ResponseBytes);
					ResponseBytes = (byte*) BxAlloc(ResponseLength + 1);
					BxCore::File::Read(CacheFile, ResponseBytes, ResponseLength);
					BxCore::File::Close(CacheFile);
					ResponseBytes[ResponseLength] = '\0';
				}
				else 
				{
					ResponseLength = 0;
					BxFree(ResponseBytes);
					ResponseBytes = null;
				}
				return true;
			}
			BxCore::File::Close(CacheFile);
		}
		// 쿼리실행
		if(!Query(Domain, Port, FilePath, null, -1, Progress))
			return false;
		// 캐시저장
		BxCore::File::MakeDirectory(CacheFileName.GetFilePath(), false);
		id_file CacheFile = BxCore::File::Open(CacheFileName, "wb");
		BxCore::File::Write(CacheFile, ResponseBytes, ResponseLength);
		BxCore::File::Close(CacheFile);
		if(!DoLoad)
		{
			ResponseLength = 0;
			BxFree(ResponseBytes);
			ResponseBytes = null;
		}
		return true;
	}
};
