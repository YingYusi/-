#pragma once
#ifndef _parse_
#define _parse_

#include "globals.h"
#include <string>

// ��������
TreeNode* parse(const std::string& program);

// �﷨��������
TreeNode* stmt_sequence();
TreeNode* statement();
TreeNode* if_stmt();
TreeNode* repeat_stmt();
TreeNode* assign_stmt();
TreeNode* read_stmt();
TreeNode* write_stmt();
TreeNode* while_stmt();  
TreeNode* for_stmt();   
TreeNode* exp();
TreeNode* simple_exp();
TreeNode* term();
TreeNode* power();  
TreeNode* factor();

#endif