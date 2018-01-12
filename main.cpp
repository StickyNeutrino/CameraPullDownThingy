
#include "main.hpp"


int main(){
    std::thread captureThread(CaptureThread);
	
	captureThread.join();
    return 0;
}
