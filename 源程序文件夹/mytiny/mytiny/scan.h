#pragma once
#ifndef _scan_
#define _scan_

#include "globals.h"
#include <string>

// ���token����
#define MAXTOKENLEN 40

// �洢��ǰtoken���ַ���
extern std::string tokenString;  // ֻ������������

// ��ȡ��һ��token
TokenType getToken();

// ��ʼ������
void init(const std::string& input);

#endif