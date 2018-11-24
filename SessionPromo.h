#include<queue>
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>

using namespace std;

#define MAX_BUFF_SIZE 1024

typedef struct PACKET{
 int size_;	//packet의 총길이
 int type_;	//packet의 type
 char* data_;	//나뉘어서 오는 데이타
}packet_;

class Session{
private:
	int socket_;
	int r_bytes;
	int total_bytes;
	int s_bytes;
	queue<packet_*> p;
public:
	Session(int sock);
	bool recvMessage();
	void pack_dequeue(packet_* pack);
	void pack_enqueue(packet_* pack);
	void SendtoOther(packet_* pack);
};
