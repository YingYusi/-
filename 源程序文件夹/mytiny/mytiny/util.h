#pragma once
#ifndef _util_
#define _util_

#include "globals.h"

// �����µ����ڵ�
TreeNode* newStmtNode(StmtKind kind);

// �����µı��ʽ�ڵ�
TreeNode* newExpNode(ExpKind kind);

// ��ӡ�﷨��
void printTree(TreeNode* tree);

#endif