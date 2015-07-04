
//TODO includeするヘッダを絞り込むこと
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>   // fork, read, write, open, close
#include <netinet/in.h>    // in_port_t, sockaddr_in等

#include <string>

/**********************************************/
/*     ソケットの初期設定（クライアント）     */
/*          return : ソケットのディスクリプタ */
/**********************************************/
//#include <stdio.h>
//#include <string.h>   // 文字列操作，memset，FD_ZERO等
#include <netdb.h>   // ソケット（hostent等）
#include <sys/socket.h>   // ソケット（AF_INET, select等）
#include <netinet/in.h>    // in_port_t, sockaddr_in等
#include <unistd.h>   // fork, read, write, open, close

//int c_setup(char *hostname, in_port_t port)
int c_setup(char *hostname, int port)    // コンパイラのバージョンによっては，
                                           // 上の行の代わりにこの行を使用
{
	int client_socket = -1;   // ソケット
	hostent *server_inf1;   // クライアント情報１
	sockaddr_in server_inf2;   // クライアント情報２

	//サーバ名からサーバ情報を得る
	//gethostbyname()の結果はfreeの必要は無い
	//(同一の領域がgethostbyname()の実行の度に使用されるため)
	if ((server_inf1 = gethostbyname(hostname)) == NULL)
	{
		//失敗した場合
		printf("***error***  サーバ名が不適当です\n");
	}
	else
	{
		//サーバ情報をsockaddr_in構造体に設定
		memset((char *)&server_inf2, 0, sizeof(server_inf2));
		server_inf2.sin_family		= AF_INET;		//アドレスファミリー
		server_inf2.sin_port		= htons(port);	//htons（２バイト）
													//整数の上位及び下位バイトの指定

		memcpy((char *)&server_inf2.sin_addr.s_addr, server_inf1->h_addr,
				server_inf1->h_length);

		//ソケットを作成
		if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			//失敗した場合
			printf("***error***  socketを作成できませんでした\n");
		}
		else
		{
			//サーバへ接続
			if (connect(client_socket, (sockaddr *)&server_inf2, sizeof(server_inf2)) == -1)
			{
				client_socket = -1;
			}
		}
	}

	return client_socket;
}


/********************************/
/*     ＨＴＴＰクライアント     */
/*          coded by Y.Suganuma */
/********************************/
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   // fork, read, write, open, close
#include <netinet/in.h>    // in_port_t, sockaddr_in等
*/
#define BUF_LEN 512   // バッファサイズ
#define PORT 50000   // ポート番号

//int c_setup(char *, in_port_t);
int c_setup(char *, int);    // コンパイラのバージョンによっては，
                               // 上の行の代わりにこの行を使用
int connect(int argc, char *argv[])
{
	int n;
	int client_socket;   // 送受信用のソケット
	char buf[BUF_LEN];   // 送受信用バッファ
	char hostname[50] = "192.168.0.6";   // ホスト名
/*
		初期設定
*/
	if ((client_socket = c_setup(hostname, PORT)) == -1) {
		printf("***error***  接続できませんでした\n");
		exit(1);
	}
/*
		サーバとの送信
*/
	else {
					// 送信内容の指定（サーバに対するリクエスト）
		strcpy(buf, "GET ");
		write(client_socket, buf, strlen(buf));
					// 受信内容の表示
		n = 1;
		while (n > 0) {
			n = read(client_socket, buf, BUF_LEN);
			if (n > 0)
				write(1, buf, n);//標準出力(1)にbufの内容を出力
		}
	}

	close(client_socket);

	return 0;
}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////






class TCPClient
{
private:
	//サーバ接続用のクライアント側ソケット
	int ClientSocket;
	//接続先サーバのポート番号
	int Port;
	//サーバ名
	std::string ServerName;
public:
	TCPClient():
		ClientSocket(-1),
		Port(0),
		ServerName("")
	{
	}
	TCPClient(std::string serveraddress,int port):
		ClientSocket(-1),
		Port(port),
		ServerName(serveraddress)
	{
		Init();
	}
	//サーバへの接続を確立する
	//TODO 失敗時は例外を投げるように改める
	void Init()
	{
		hostent *server_inf1;   // クライアント情報１
		sockaddr_in server_inf2;   // クライアント情報２

		//サーバ名からサーバ情報を得る
		//gethostbyname()の結果はfreeの必要は無い
		//(同一の領域がgethostbyname()の実行の度に使用されるため)
		if ((server_inf1 = gethostbyname(ServerName.c_str())) == NULL)
		{
			//名前解決に失敗した場合
			printf("***error***  サーバ名が不適当です\n");
		}
		else
		{
			//サーバ情報をsockaddr_in構造体に設定
			memset((char *)&server_inf2, 0, sizeof(server_inf2));
			server_inf2.sin_family		= AF_INET;		//アドレスファミリー
			server_inf2.sin_port		= htons(Port);	//htons（２バイト）
														//整数の上位及び下位バイトの指定

			memcpy((char *)&server_inf2.sin_addr.s_addr, server_inf1->h_addr,
					server_inf1->h_length);

			//ソケットを作成
			if ((ClientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
				//失敗した場合
				printf("***error***  socketを作成できませんでした\n");
			}
			else
			{
				//サーバへ接続
				if (connect(ClientSocket, (sockaddr *)&server_inf2, sizeof(server_inf2)) == -1)
				{
					ClientSocket = -1;
				}
			}
		}
	}
	//サーバへ文字列を送信する
	void SendText(const std::string& sendtext)
	{
		write(ClientSocket, sendtext.c_str(), sendtext.length());
	}
	//サーバから文字列を受信する
	//TODO バッファのサイズは要検討
	//TODO 受信をstringではなくstringstreamでやるべきか検討すること
	void ReceiveText(std::string& receivetext)
	{
		//受信バッファのサイズ
		const int buf_len=512;//
		//受信バッファ
		char buf[buf_len];
		//read()の成否確認用
		int n;

		//受信用文字列を空にする
		receivetext.clear();

		// 受信内容の表示
		//whileの条件に適合する様に初期化
		n = 1;
		while (n > 0)
		{
			//テキストデータの受信
			//n=0 : EOF
			//n=-1 : error
			n = read(ClientSocket, buf, BUF_LEN);
			if (n > 0)
			{
				//取得したデータの末尾にNULL文字を付加して
				//NULL終端文字列にする
				buf[n]='\0';
				//受信用文字列にバッファの内容を追加する
				receivetext+=buf;
			}
		}

	}

};
