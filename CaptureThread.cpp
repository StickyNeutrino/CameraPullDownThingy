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
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "user-agent: OlympusCameraKit");
	headers = curl_slist_append(headers, "content-length: 4096");
	
	//Set up CURL
	CURL *curl; //create state for CURL called "curl"
	curl = curl_easy_init();
	//End setup
	
	initCamera(curl, headers);
	
	for(;;){
		getPicture(curl, headers);
		/* sleep defined in <unistd.h> */
		sleep(2);
	}
	
	curl_easy_cleanup(curl); //ALWAYS HAVE THIS - frees CURL resources
}

void get(string command, CURL *curl, struct curl_slist *headers) {
	//make URL
	string link = ipAddr + command;
	
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		CURLcode res = curl_easy_perform(curl);
		
		if (res != CURLE_OK) { //Check for errors
			cout << "curl_easy_perform() failed: " << stderr << endl;
			curl_easy_strerror(res);
		}
	}
}

void post(string command, string body, CURL *curl, struct curl_slist *headers) {
	/* make URL */
	string link = ipAddr + command;
	
	if (curl) {
		struct postData bodyData;
		bodyData.readptr = body.c_str();
		bodyData.sizeleft = body.size();
		curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		
		/*Define the write callback and data for it */
		curl_easy_setopt(curl,  CURLOPT_READFUNCTION, postDataCallBack);
		curl_easy_setopt(curl,  CURLOPT_READDATA, &bodyData);
		
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		
		CURLcode res = curl_easy_perform(curl);
		
		if (res != CURLE_OK) { /* Check for errors */
			cout << "curl_easy_perform() failed: " << stderr << endl;
			curl_easy_strerror(res);
		}
	}
}

void initCamera(CURL *curl, struct curl_slist *headers){
	/* put camera in record mode */
	get("switch_cameramode.cgi?mode=rec", curl, headers);
	/* tell camera not to save photos to SD card */
	post("set_camprop.cgi?com=set&propname=DESTINATION_FILE",File_WIFI_Data, curl, headers);
}

/*basically this funiton gets called by libcurl when it needs data for a post*/
std::size_t postDataCallBack(char *buffer, size_t size, size_t nitems, void *instream){
	struct postData *data = (struct postData *)instream;
	size_t buffer_size = size * nitems;
 
	if(data->sizeleft) {
		/* copy as much as possible from the source to the destination */
		size_t copy_this_much = data->sizeleft;
		if(copy_this_much > buffer_size){
			copy_this_much = buffer_size;
		}
		memcpy(buffer, data->readptr, copy_this_much);
		
		data->readptr += copy_this_much;
		data->sizeleft -= copy_this_much;
		return copy_this_much; /* we copied this many bytes */
		
	}
	return 0;
}

void getPicture(CURL *curl, struct curl_slist *headers){
	//take a picture
	get("exec_takemotion.cgi", curl, headers);

	//recover the picture
	//basically a get request that reads the responce into the vector
	string link = ipAddr + "exec_storeimage.cgi";
	
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, imageWriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &newestImage);
		
		/* lock the mutex so other thread allways gets full image */
		imageVectorMutex.lock();
		
		/* clear last image */
		newestImage.clear();
		
		/* preform the request */
		CURLcode res = curl_easy_perform(curl);
		imageVectorMutex.unlock();
		
		if (res != CURLE_OK) { //Check for errors
			cout << "curl_easy_perform() failed: " << stderr << endl;
			curl_easy_strerror(res);
		}
	}
}

size_t imageWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata){
	/* kinda ugly but just gets the vector from void pointer */
	std::vector<int> imageVector = *((std::vector<int> *) userdata);
	size_t incomingSize = size * nmemb;
	if(!incomingSize){
		/* if we don't get anything */
		return 0;
	}
	int i = 0;
	for(i; i * sizeof(char) < incomingSize; i++){
		/* push all data into vector */
		imageVector.push_back(ptr[i]);
	}
	return i * sizeof(char);
}

