#pragma once
#include "BxString.hpp"

//! \brief HTTP��� �����ۼ���, ���ϴٿ�ε�
class BxWebContent
{
	byte* ResponseBytes;
	int ResponseLength;

public:
	BxWebContent() : ResponseBytes(null), ResponseLength(0) {}
	~BxWebContent() {BxFree(ResponseBytes);}

	/*!
	\brief ĳ�������̸� �ޱ�
	\return ĳ�������̸�
	*/
	global_func inline string GetCachePath() {return "webcontent_cache/";}

	/*!
	\brief ������� �ޱ�
	\return ������� ����Ʈ�迭
	*/
	inline const byte* _tmp_ GetBytes() {return ResponseBytes;}

	/*!
	\brief ��������� ��������(���̳ʸ�)
	\return ����Ʈ����
	*/
	inline int GetDataLength() {return ResponseLength;}

	/*!
	\brief ��������� ��������(��Ʈ��)
	\return ����Ʈ����
	*/
	inline int GetTextLength() {return BxUtilGlobal::StrLenEx((string) ResponseBytes);}

	/*!
	\brief HTTP��������
	\param Domain : ������ ����(��: "www.naver.com")
	\param Port : ��Ʈ��ȣ(HTTP�� ���� 80����Ʈ)
	\param Path : �ٿ���� ���� �Ǵ� ������ ������ ���(��: "Update/Run.aspx")
	\param Arguments : ������ ��� ������ ���ڵ�(��: "Data0=aaa&Data1=bbb&Data2=ccc")
	\param Timeout : �� �ܰ躰 �������ð�
	\param Progress : ���α׷����� ����
	\return �ۼ��� ��������
	*/
	bool Query(string Domain, ushort Port, string Path, string Arguments = null, int Timeout = -1, callback_progress Progress = null)
	{
		if(Timeout < 0) Timeout = 3000;
		// ���Ͽ���, �������� �� ����
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
				if(Progress && !Progress(-1, 0)) return false; // ������Ʈ
				SendResult = BxCore::Socket::Send(Socket, RequestChars + SendedLength, RequestCharsLength - SendedLength);
				if(SendResult < 0)
				{
					BxCore::Socket::Release(Socket);
					return false; // �۽Ž���
				}
			}
			while((SendedLength += SendResult) < RequestCharsLength);
			BxCore::System::Sleep(0);
		}
		else
		{
			BxCore::Socket::Release(Socket);
			return false; // �������
		}

		// HTTP��� ����
		char ResponseChars[4096];
		char* EndCodeSearchFocus = ResponseChars;
		int RecvResult = 0, RecvedLength = 0;
		int ErrorTime = 0, SleepTime = 0;
		const int HTMLEndCodeLength = BxUtilGlobal::StrLen("\r\n\r\n");
		while(0 <= (RecvResult = BxCore::Socket::Recv(Socket, ResponseChars + RecvedLength, (4096 - 1) - RecvedLength)))
		{
			if(Progress && !Progress(-1, 0)) return false; // ������Ʈ
			RecvedLength += RecvResult;
			// ���� ������ ���� ���
			if(0 < RecvResult)
			{
				ErrorTime = 0;
				SleepTime >>= 1;
				// HTML��� �����ڵ� �˻�
				if(HTMLEndCodeLength < RecvedLength)
				{
					ResponseChars[RecvedLength] = '\0';
					while(*(++EndCodeSearchFocus) && BxUtilGlobal::StrSameCount(EndCodeSearchFocus, "\r\n\r\n") != HTMLEndCodeLength);
					if(*EndCodeSearchFocus)
					{
						EndCodeSearchFocus += HTMLEndCodeLength;
						// �����ͻ����� ����
						char* ContentLengthSearchFocus = ResponseChars;
						while(++ContentLengthSearchFocus < EndCodeSearchFocus && BxUtilGlobal::StrSameCount(ContentLengthSearchFocus, "Content-Length:") != 15);
						ContentLengthSearchFocus += 15;
						if(ContentLengthSearchFocus == EndCodeSearchFocus)
						{
							BxCore::Socket::Release(Socket);
							return false; // �����ͻ����� ��������
						}
						// ����Ȯ��
						BxFree(ResponseBytes);
						ResponseLength = BxUtilGlobal::AtoI(ContentLengthSearchFocus, EndCodeSearchFocus - ContentLengthSearchFocus);
						ResponseBytes = (byte*) BxAlloc(ResponseLength + 1);
						break;
					}
					else EndCodeSearchFocus = (char*) BxUtilGlobal::Max((int) ResponseChars, (int)(EndCodeSearchFocus - HTMLEndCodeLength));
				}
			}
			// ���� ������ ���� ���
			else if(Timeout < (ErrorTime += SleepTime))
			{
				BxCore::Socket::Release(Socket);
				return false; // ���ð��ʰ�
			}
			else BxCore::System::Sleep(BxUtilGlobal::Min(SleepTime++, Timeout >> 6));
		}
		if(RecvResult < 0)
		{
			BxCore::Socket::Release(Socket);
			return false; // ���Ž���
		}

		// ������ �̰�
		RecvedLength = ResponseChars + RecvedLength - EndCodeSearchFocus;
		BxCore::Util::MemMove(ResponseBytes, EndCodeSearchFocus, RecvedLength);
		if(Progress && !Progress(RecvedLength, ResponseLength)) // ���α׷���
			return false;

		// ������ ����
		RecvResult = 0;
		if(RecvedLength < ResponseLength)
		{
			// �߰�����
			ErrorTime = 0, SleepTime = 0;
			while(0 <= (RecvResult = BxCore::Socket::Recv(Socket, ResponseBytes + RecvedLength, ResponseLength - RecvedLength)))
			{
				RecvedLength += RecvResult;
				if(Progress && !Progress(RecvedLength, ResponseLength)) // ���α׷���
					return false;
				// ���� ������ ���� ���
				if(0 < RecvResult)
				{
					ErrorTime = 0;
					SleepTime >>= 1;
					// �߰����ſϷ�
					if(RecvedLength == ResponseLength)
						break;
				}
				// ���� ������ ���� ���
				else if(Timeout < (ErrorTime += SleepTime))
				{
					BxCore::Socket::Release(Socket);
					return false; // ���ð��ʰ�
				}
				else BxCore::System::Sleep(BxUtilGlobal::Min(SleepTime++, Timeout >> 6));
			}
		}
		BxCore::Socket::Release(Socket);
		if(RecvResult < 0)
		{
			BxFree(ResponseBytes);
			ResponseLength = 0;
			return false; // ���Ž���
		}
		ResponseBytes[ResponseLength] = '\0';
		return true;
	}

	/*!
	\brief ĳ������ �ε�(������ ����)
	\param Domain : ������ ����(��: "www.naver.com")
	\param Port : ��Ʈ��ȣ(HTTP�� ���� 80����Ʈ)
	\param FilePath : �ٿ���� ���ϰ��(��: "Update/Abc.bmp")
	\param ValidFileSize : ��ȿüũ�� ���ϻ�����(-1�� üũ����)
	\param DoLoad : �ε������� ����
	\param Progress : ���α׷����� ����
	\return ĳ�÷ε� ��������
	*/
	bool Cache(string Domain, ushort Port, string FilePath, int ValidFileSize = -1, bool DoLoad = true, callback_progress Progress = null)
	{
		// ĳ�÷ε�
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
		// ��������
		if(!Query(Domain, Port, FilePath, null, -1, Progress))
			return false;
		// ĳ������
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
