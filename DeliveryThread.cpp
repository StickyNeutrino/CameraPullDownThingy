
#include "DeliveryThread.hpp"

using namespace std;

extern vector<char> newestImage;
extern mutex imageVectorMutex;



void DeliveryThread(){
	int listenSocket, sendingSocket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	/* Creating socket */
	if ((listenSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		cout << "Failed to creat socket" << endl;
		return;
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons( 2418 );
	
	/* bind to port 2048 */
	if (bind(listenSocket, (struct sockaddr *)&address,
			 sizeof(address))<0){
		cout << "bind failed" << endl;
		return;
	}
	
	
	if (listen(listenSocket, 3) < 0){
		cout << "listen failed" << endl;
		return;
	}
	
	
	for(;;){
		
		if ((sendingSocket = accept(listenSocket, (struct sockaddr *)&address,
							 (socklen_t*)&addrlen))<0)
		{
			cout << "accept failed" << endl;
			return;
		}
	
		imageVectorMutex.lock();
		cout << "locked" << endl;
		if(newestImage.size()){
			send(sendingSocket , newestImage.data() , newestImage.size() , 0 );
			cout << "sent" << endl;
			imageVectorMutex.unlock();
		}else{ /* incase there is no image sleep and check untill there is */
			imageVectorMutex.unlock();
			cout << newestImage.size() << endl;
			while(1){
				imageVectorMutex.lock();
				if(newestImage.size()){
					break;
				}
				imageVectorMutex.unlock();
				
				/* sleep 5 milliseconds */
				usleep(5000);
			}
			send(sendingSocket , newestImage.data() , newestImage.size() , 0 );
			imageVectorMutex.unlock();
			
		}
		
		
		
	}
}
