//
//  main.hpp
//  
//
//  Created by Daniel Fitchmun on 1/11/18.
//
//


#pragma once

#include <stdio.h>
#include <thread>
#include "CaptureThread.hpp"
#include <mutex>
#include <vector>
#include "DeliveryThread.hpp"

std::vector<char> newestImage;
std::mutex imageVectorMutex;


