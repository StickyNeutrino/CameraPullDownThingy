
#include "main.hpp"


int main(){
    std::thread captureThread(CaptureThread);
	std::thread deliveryThread(DeliveryThread);
	
	deliveryThread.join();
	captureThread.join();
    return 0;
}
