#pragma once
#ifndef _scan_
#define _scan_

#include "globals.h"
#include <string>

// 最大token长度
#define MAXTOKENLEN 40

// 存储当前token的字符串
extern std::string tokenString;  // 只声明，不定义

// 获取下一个token
TokenType getToken();

// 初始化参数
void init(const std::string& input);

#endif