#pragma once
#ifndef _util_
#define _util_

#include "globals.h"

// 创建新的语句节点
TreeNode* newStmtNode(StmtKind kind);

// 创建新的表达式节点
TreeNode* newExpNode(ExpKind kind);

// 打印语法树
void printTree(TreeNode* tree);

#endif