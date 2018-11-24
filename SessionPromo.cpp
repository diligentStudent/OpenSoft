#include "SessionPromo.h"

Session :: Session(int sock)
{
	socket_ = sock;
}

bool Session :: recvMessage()
{
	char buff[MAX_BUFF_SIZE] = {NULL,};
	total_bytes = 0;
	while(1)
	{
		packet_* packet = new packet_();
		r_bytes = recv(socket_,buff,MAX_BUFF_SIZE,0);

		if(r_bytes <= 0)//r_bytes가 0보다 작거나 같으면 정상적으로 recv가 안된것이므로 false 리턴
			return false;
		
		if(total_bytes == 0)//total_bytes가 처음recv를 받았다면고대로 total_bytes에 받은 바이트만큼 넣어줌
			total_bytes += r_bytes ;
		
		else
			total_bytes += r_bytes - sizeof(int)*2;//두번째부터는size와 type에관한 
							       //크기는빼서 total_bytes에 넘겨줌
 	
		memcpy(&packet->size_, buff, sizeof(packet->size_));
		//packet->size_ = atoi(packet->size_);//받은 char형태의 숫자를 int로 바꿔줌
		printf("size: %d",packet->size_);
		memcpy(&packet->type_, buff+sizeof(int), sizeof(int));
		//packet->type_ = atoi(packet->type_);
		printf("type: %d",packet->type_);
		memcpy(packet->data_, buff+sizeof(int)*2, packet->size_ - sizeof(int)*2);

		if(total_bytes <= packet-> size_)
		{
			pack_enqueue(packet);
			memset(buff, NULL, sizeof(buff));//다음 recv를 위해 버퍼를 NULL로초기화 해줌
			continue;// total_bytes가 소켓이 보낸 사이즈보다작으면 recv를하기위해 while을계속돔
		}

		else
			break;//total_bytes가 사이즈보다 크거나 같다면 while을 멈춤

	}
		return true;
}

void Session :: pack_enqueue(packet_* pack)
{
	p.push(pack);
}

void Session :: pack_dequeue(packet_* pack)
{
	pack = p.front();
	p.pop();
}

void Session :: SendtoOther(packet_* pack )
{
	char buff[MAX_BUFF_SIZE] = {NULL,};
	int total_Send_bytes = 0;
	while(!p.empty())
	{
		packet_* tmppack = NULL; 
		pack_dequeue(tmppack);
		int len = sizeof(tmppack->data_) + sizeof(tmppack->type_);	
		sscanf(buff, "%d", len);//길이를 버퍼에 넣어줌
		sscanf(buff + sizeof(int), "%d", tmppack->type_);//int형태의 타입을 버퍼에 넣어줌
		memcpy(buff + sizeof(int)*2, tmppack->data_, sizeof(tmppack->data_));
		
		s_bytes = send(socket_,buff,MAX_BUFF_SIZE,0);
		printf("pack_len: %d, pack_type: %d, pack_data: %s\n", len, tmppack->type_, tmppack->data_);
	}
}
