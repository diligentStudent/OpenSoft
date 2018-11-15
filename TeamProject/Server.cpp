#include <stdio.h>
#include "/root/WorkSpace/TeamProject/ServerHeader.h"


//g++ -std=gnu++11 -pthread Server.cpp

Server::Server(){

	port = 9000;
	Initialize();
}


void Server::Initialize(){
	CreateEpoll();	
	Listen();
	RegistEpollEvent(lstnfd,listen_event, EPOLLIN);

	Create_Thread();

}

void Server::Listen(){
	
	struct sockaddr_in addr_in;
	
	lstnfd = socket(PF_INET,SOCK_STREAM, 0);
	
	if(lstnfd < 0){
		printf("Make Listen socket Failed");
		return;
	}

	memset(&addr_in, 0, sizeof(sockaddr_in));

	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_in.sin_port = htons(port);
	
	if(bind(lstnfd, (struct sockaddr*)&addr_in, sizeof(sockaddr_in))<0){
		printf("bind error");
		return;
	}

	if(listen(lstnfd, 5) < 0){
		printf("listen Failed ");
		return;
	}
	

}

void Server::SetNonBlocking(int fd){
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag|O_NONBLOCK);

}

void Server::CreateEpoll(){
	if((epfd = epoll_create(INCREASE_COUNT * 10)) < 0){
		printf("epoll_create Failed\n");
		return;
	}		
}

void Server::RegistEpollEvent(int fd, epoll_event& event, unsigned int flag){

memset(&event, 0, sizeof(epoll_event));

event.events = flag;
event.data.fd = fd;
epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);

}
/*
void Server::CreateSession(){
	
}
*/
void Server::Run(){

	epoll_event event;
	int client;	
	char buf[BUF_SIZE] = { 0,};
	while(true){
	try{
		int ret = epoll_wait(epfd,&event, 1 , -1);
		if( ret < 0 ){
			printf("epoll_wait Error\n ");
			return; 
		}


		if(event.data.fd == lstnfd){
			
			sockaddr_in client_addr;
			socklen_t addrlen = sizeof(sockaddr_in);
		
			client = accept(lstnfd, (sockaddr*)&client_addr,&addrlen);
			if(client < 0 ){
				printf("Accept Error\n");
				return;
			}			
			SetNonBlocking(client);
			epoll_event clientEpollEvent;
			RegistEpollEvent(client,clientEpollEvent,EPOLLIN | EPOLLET);
		}

		else{
			client = event.data.fd;
			int recvBytes = read(client, buf , BUF_SIZE);
			
			printf("Recv Bytes => %d\n" , recvBytes);
			buf[recvBytes] = '\0';	
			printf("buf : %s\n " , buf);
			
			/*
				TODO : Get Session From Session_Map
				TODO : Request Data Analyzer to Session
				TODO : Push Package to Contents
			*/

		}
		}catch(int fd){
			int client = event.data.fd;
			RegistEpollEvent(client,event,EPOLL_CTL_DEL);
			printf("Error Client Out\n");
		
		}
	}
	

}

Server::~Server(){
	for(int i = 0; i < THREAD_COUNT ; i++)
		epoll_Thread[i].join();

}


void Server::test(){

	printf("test");
}


void Server::Create_Thread(){
	for(int i = 0 ; i < THREAD_COUNT; i++){
		epoll_Thread[i] = std::thread {&Server::Run, this   };
	}

	//	epoll_Thread[i] = std::thread {&Server::Run, this};

}

static const void print(int a){
	
	printf("print");
}

int main(){

	Server* server = new Server();
//	server->Create_Thread();

//	std::thread p(print,1);	
//	p.join();
//	for(int i = 0 ; i < THREAD_COUNT ; i++)

//		std::thread t{&Server::test,&server};
	while(true){

	}		
	return 0;

}
