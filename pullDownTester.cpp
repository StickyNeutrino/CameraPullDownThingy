

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <vector>

using namespace std;

int main(){
	
	int recSocket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char recBuff [2048];
	int LENGTH =  2048;
	bzero(recBuff, LENGTH);
	FILE* filePtr = fopen("Testing.jpg","wb");
	
	if ((recSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		cout << "Failed to creat socket" << endl;
		return 1;
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons( 2418 );
	
	connect(recSocket, (struct sockaddr *)&address, addrlen);

	int recSize;
	while((recSize = recv(recSocket, recBuff, LENGTH, 0)) > 0){

		int write_sz = fwrite(recBuff, sizeof(char), recSize, filePtr);
		bzero(recBuff, LENGTH);
	}
	fclose(filePtr);
	return 0;
}


