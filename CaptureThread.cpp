//
//  CaptureThread.cpp
//  
//
//  Tasked with Capturing Images regularly
//  and pulling them down from the camera
//

#include "CaptureThread.hpp"

using namespace std;

extern vector<char> newestImage;
extern mutex imageVectorMutex;

const string ipAddr = "http://192.168.0.10/";

void CaptureThread(){
	//Headers
	
	std::cout << File_WIFI_Data << std::endl;

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "User-Agent: OlympusCameraKit");
	//headers = curl_slist_append(headers, "content-length: 4096");
	
	//Set up CURL
	CURL *curl; //create state for CURL called "curl"
	curl = curl_easy_init();
	//End setup
	
	initCamera(curl, headers);
	for(int i = 0; i < 1000; i++){
		getPicture(curl, headers);
		/* sleep defined in <unistd.h> */
		sleep(2);
	}
	
	get("exec_pwoff.cgi", curl, headers);
	
	curl_easy_cleanup(curl); //ALWAYS HAVE THIS - frees CURL resources
}

void get(string command, CURL *curl, struct curl_slist *headers) {
	//make URL
	string link = ipAddr + command;
	
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
		cout << endl << link << endl;
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		CURLcode res = curl_easy_perform(curl);
		
		if (res != CURLE_OK) { //Check for errors
			cout << "curl_easy_perform() failed: " << stderr << endl;
			curl_easy_strerror(res);
		}
	}
}

void post(string command, string body, struct curl_slist *headers) {
	/* make URL */
	sleep(2);
	string link = ipAddr + command;
	
	
	CURL *curl; //create state for CURL called "curl"
	curl = curl_easy_init();
	
	if (curl) {
		cout << endl << link << endl;
		struct curl_slist *NewHeaders;
		curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
		NewHeaders = curl_slist_append(headers, "Content-type:");
		//headers = curl_slist_append(headers, "Content-Length:");
		NewHeaders = curl_slist_append(NewHeaders, "Accept:");
		curl_easy_setopt(curl,  CURLOPT_POSTFIELDS, body.c_str());
		
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		
		CURLcode res = curl_easy_perform(curl);
		
		if (res != CURLE_OK) { /* Check for errors */
			cout << "curl_easy_perform() failed: " << stderr << endl;
			curl_easy_strerror(res);
		}
	}
}

void initCamera(CURL *curl, struct curl_slist *headers){
	
	get("get_connectmode.cgi", curl, headers);
	
	get("switch_cameramode.cgi?mode=rec", curl, headers);

	get("get_connectmode.cgi", curl, headers);
	
	get("get_state.cgi", curl, headers);
	get("exec_takemisc.cgi?com=startliveview&port=5555", curl, headers);
	/* put camera in record mode */
	//get("get_camprop.cgi?com=get&propname=TAKE_DRIVE",curl, headers);
	//post("set_camprop.cgi?com=set&propname=TAKE_DRIVE",SHOOTMODE_DATA, curl, headers);
	/* tell camera not to save photos to SD card */
	get("get_camprop.cgi?com=get&propname=DESTINATION_FILE", curl, headers);
	post("set_camprop.cgi?com=set&propname=DESTINATION_FILE",File_WIFI_Data, headers);
	get("get_camprop.cgi?com=get&propname=DESTINATION_FILE", curl, headers);
}


void getPicture(CURL *curl, struct curl_slist *headers){
	//take a picture
	get("exec_takemotion.cgi?com=newstarttake", curl, headers);
	//recover the picture

	
	//basically a get request that reads the responce into the vector
	string link = ipAddr + "exec_storeimage.cgi";
	
	if (curl) {
		sleep(1);
		curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, imageWriteCallback);
		//FILE* filePtr = fopen("/Users/danielfitchmun/Desktop/Tech/CameraPullDownThingy/Testing.jpg","wb");

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &newestImage);
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA,filePtr);
		/* lock the mutex so other thread allways gets full image */
		imageVectorMutex.lock();
		
		/* clear last image */
		newestImage.clear();
		
		/* preform the request */
		CURLcode res = curl_easy_perform(curl);
		imageVectorMutex.unlock();
		std::cout <<"GOT" << endl;
		if (res != CURLE_OK) { //Check for errors
			cout << "curl_easy_perform() failed: " << stderr << endl;
			curl_easy_strerror(res);
		}
		//fclose(filePtr);
	}

}

size_t imageWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata){
	/* kinda ugly but just gets the vector from void pointer */
	std::vector<char> * imageVector = ((std::vector<char> *) userdata);
	size_t incomingSize = size * nmemb;
	if(!incomingSize){
		/* if we don't get anything */
		return 0;
	}
	int i = 0;
	for(i; i * sizeof(char) < incomingSize; i++){
		/* push all data into vector */
		imageVector->push_back(ptr[i]);
	}
	return i * sizeof(char);
}

