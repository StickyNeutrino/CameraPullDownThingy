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

void request(std::string link, std::string command, CURL *curl,
			 struct curl_slist	*headers);

void PlaceholderCaptureThread();


