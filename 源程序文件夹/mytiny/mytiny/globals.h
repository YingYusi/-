#pragma once
#ifndef _globals_
#define _globals_

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <ctype.h>


// ����ʷ���Ԫ����
enum TokenType {
    ENDFILE,                //��������
    ERROR,                  //����
    //������                
    IF,                     //if
    THEN,                   //then
    ELSE,                   //else
    END,                    //����
    REPEAT,                 //repeat
    UNTIL,                  //until
    READ,                   //read
    WRITE,                  //write
    //���ַ��ʷ���Ԫ        
    ID,                     //����ĸ�����ǲ��Ǳ�����
    NUM,                    //����
    //�������              
    ASSIGN,                 //��ֵ,:=
    EQUAL,                  //���ڣ�==
    LESS,                   //С��,<
    ADD,                    //��,+
    SUB,                    //����-
    MUL,                    //��,*
    DIV,                    //����/
    LPAREN,                 //�����ţ�(
    RPAREN,                 //�����ţ�)
    SEMI,                   //�ֺ�,:

    //����whileѭ��
    WHILE,                  //while
    DO,                     //do
    ENDDO,                  //enddo

    //����forѭ��          
    FOR,                    //for
    TO,                     //to

    //�����������ʽ���������
    ADD_ASSIGN,             //+=
    SUB_ASSIGN,             //-=
    MOD,                    //���࣬%
    POWER,                  //�˷���^

    //����Ƚ��������
    GREATER,                // ���ڣ�>
    LESS_EQUAL,             // С�ڵ��ڣ�<=
    GREATER_EQUAL,          // ���ڵ��� >=
    NOT_EQUAL               // �����ڣ�<>
};

//�ڵ�����
enum NodeKind { StmtK, ExpK };

//�������ͣ������whileѭ����forѭ�����ӵ��ڣ�������
enum StmtKind { IfK, RepeatK, AssignK, ReadK, WriteK, WhileK, ForK, ForToK, ADD_ASSIGNK, SUB_ASSIGNK };

//���ʽ����
enum ExpKind { OpK, ConstK, IdK };

// �����﷨���ڵ�
class TreeNode {
public:
    TreeNode* child[7];
    TreeNode* sibling; //ͬ�����ֵ�
    int lineno;  //��ǰ�ڵڼ���
    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp; } kind;
    union {
        TokenType op;
        int val;
        std::string* name;
    } attr;

    TreeNode() {
        for (int i = 0; i < 3; i++) child[i] = nullptr;
        sibling = nullptr;
    }
};

// ��ǰ�ʷ���Ԫ
extern TokenType token;

#endif