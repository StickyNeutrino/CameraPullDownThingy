//
//  CaptureThread.hpp
//  
//
//  Tasked with Capturing Images regularly
//  and pulling them down from the camera
//

#pragma once

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector> 
#include <mutex>

struct postData{
	const char *readptr;
	size_t sizeleft;
};

/* Post data without reading the results, used for commands */
void post(std::string link, std::string command, CURL *curl, std::string body,
			 struct curl_slist	*headers);

/* Get request without reading, used for commands */
void get(std::string link, std::string command,
			 CURL *curl, struct curl_slist *headers);

void CaptureThread();

/* Set up camera options for use */
void initCamera(CURL *curl, struct curl_slist *headers);

/* used by post to give curl the data */
std::size_t postDataCallBack(char *buffer, size_t size, size_t nitems, void *instream);

/* Callback for getPictures request */
size_t imageWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);

/* Sends get to take picture and records picture to vector */
void getPicture(CURL *curl, struct curl_slist *headers);

const std::string File_WIFI_Data =
"<?xml version=\"1.0\"?>"
"<set>"
"<value>:"
"DESTINATION_FILE_WIFI"
"</value>"
"</set>";
