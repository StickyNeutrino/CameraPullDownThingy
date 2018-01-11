
#include "main.hpp"


int main(){
    std::thread captureThread(PlaceholderCaptureThread);
	
	captureThread.join();
    return 0;
}
