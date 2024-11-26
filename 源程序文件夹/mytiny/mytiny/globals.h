#pragma once
#ifndef _globals_
#define _globals_

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <ctype.h>


// 定义词法单元类型
enum TokenType {
    ENDFILE,                //结束输入
    ERROR,                  //出错
    //保留字                
    IF,                     //if
    THEN,                   //then
    ELSE,                   //else
    END,                    //结束
    REPEAT,                 //repeat
    UNTIL,                  //until
    READ,                   //read
    WRITE,                  //write
    //多字符词法单元        
    ID,                     //是字母，但是不是保留字
    NUM,                    //数字
    //特殊符号              
    ASSIGN,                 //赋值,:=
    EQUAL,                  //等于，==
    LESS,                   //小于,<
    ADD,                    //加,+
    SUB,                    //减，-
    MUL,                    //乘,*
    DIV,                    //除，/
    LPAREN,                 //左括号，(
    RPAREN,                 //右括号，)
    SEMI,                   //分号,:

    //增加while循环
    WHILE,                  //while
    DO,                     //do
    ENDDO,                  //enddo

    //增加for循环          
    FOR,                    //for
    TO,                     //to

    //扩充算术表达式的运算符号
    ADD_ASSIGN,             //+=
    SUB_ASSIGN,             //-=
    MOD,                    //求余，%
    POWER,                  //乘方，^

    //扩充比较运算符号
    GREATER,                // 大于，>
    LESS_EQUAL,             // 小于等于，<=
    GREATER_EQUAL,          // 大于等于 >=
    NOT_EQUAL               // 不等于，<>
};

//节点类型
enum NodeKind { StmtK, ExpK };

//句子类型，添加了while循环，for循环，加等于，减等于
enum StmtKind { IfK, RepeatK, AssignK, ReadK, WriteK, WhileK, ForK, ForToK, ADD_ASSIGNK, SUB_ASSIGNK };

//表达式类型
enum ExpKind { OpK, ConstK, IdK };

// 定义语法树节点
class TreeNode {
public:
    TreeNode* child[7];
    TreeNode* sibling; //同级的兄弟
    int lineno;  //当前在第几行
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

// 当前词法单元
extern TokenType token;

#endif