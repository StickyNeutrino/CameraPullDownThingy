//
//  CaptureThread.cpp
//  
//
//  Tasked with Capturing Images regularly
//  and pulling them down from the camera
//

#include "CaptureThread.hpp"

using namespace std;

void PlaceholderCaptureThread(){
	string air = "http://192.168.0.10/";
	//Headers
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "user-agent: OlympusCameraKit");
	headers = curl_slist_append(headers, "content-length: 4096");
	
	//Set up CURL
	CURL *curl; //create state for CURL called "curl"
	CURLcode res;
	curl = curl_easy_init();
	//End setup
	
	//request(air, "get_imglist.cgi?DIR=%2FDCIM%2F100OLYMP", curl, headers);
	request(air, "get_connectmode.cgi", curl, headers);
	request(air, "switch_cameramode.cgi?mode=rec", curl, headers);
	request(air, "get_state.cgi", curl, headers);
	request(air, "exec_takemisc.cgi?com=startliveview&port=5555", curl, headers);
	request(air, "exec_takemotion.cgi?com=newstarttake", curl, headers);
	
	curl_easy_cleanup(curl); //ALWAYS HAVE THIS - frees CURL resources
}

void request(string link, string command, CURL *curl, struct curl_slist *headers) {
	link = "http://192.168.0.10/";
	link = link + command;
	
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
