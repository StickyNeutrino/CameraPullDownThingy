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

struct postData{
	const char *readptr;
	size_t sizeleft;
};

void post(std::string link, std::string command, CURL *curl, std::string body,
			 struct curl_slist	*headers);

void get(std::string link, std::string command,
			 CURL *curl, struct curl_slist *headers);

void PlaceholderCaptureThread();

void initCamera(CURL *curl, struct curl_slist *headers);
//Set up camera options for use


std::size_t postDataCallBack(char *buffer, size_t size, size_t nitems, void *instream);

const std::string File_WIFI_Data =
"<?xml version=\"1.0\"?>"
"<set>"
"<value>:"
"DESTINATION_FILE_WIFI"
"</value>"
"</set>";
