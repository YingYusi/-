#include "scan.h"
#include "globals.h"
#include <cctype>
#include <unordered_map>

static std::string input;
static size_t currentPos = 0;
static int lineno = 1;
//存储所有的关键字
static std::unordered_map<std::string, TokenType> reservedWords = {
    {"if", IF},           //if
    {"then", THEN},       //then
    {"else", ELSE},       //else
    {"end", END},         //结束符号
    {"repeat", REPEAT},   //repeat
    {"until", UNTIL},     //until
    {"read", READ},       //read
    {"write", WRITE},     //write
    {"while", WHILE},     //while
    {"do", DO},           //do
    {"enddo", ENDDO},     //结束do
    {"for", FOR},         //for
    {"to", TO},           //to
};

//初始化
void init(const std::string& inputText) {
    input = inputText;  //输入
    currentPos = 0;     //记录当前位置
    lineno = 1;         //记录当前行数
}

//获取下一个字符
char getNextChar() {
    if (currentPos >= input.length()) return EOF;
    return input[currentPos++];
}

//回退一个字符
void ungetNextChar() {
    if (currentPos > 0) currentPos--;
}

//判断是否合法，即当前位置是否已经遍历超过了终点
bool is_ledal() {
    return currentPos < input.length();
}

TokenType getToken() {
    tokenString = "";
    TokenType currentToken;

    // 跳过空白字符和注释
    bool skip = true;
    while (skip && is_ledal) {
        char c = getNextChar();
        // 跳过空格,遇到换行符要记得让lineno加一
        if (isspace(c)) {
            if (c == '\n') lineno++;
            continue;
        }
        // 跳过注释，遇到换行符要记得让lineno加一
        if (c == '{') {
            while ((c = getNextChar()) != '}' && c != EOF) {
                if (c == '\n') lineno++;
            }
            continue;
        }
        //能到这一步，说明没有遇到空格和注释，可以跳出循环了
        skip = false;
    }
    //没有token，直接结束
    if (currentPos >= input.length()) {
        return ENDFILE;
    }

    char c = input[currentPos - 1];

    // 处理标识符和关键字
    if (isalpha(c)) {
        //如果是字母
        tokenString = c;
        while (is_ledal && isalpha((c = getNextChar()))) {
            tokenString += c;
        }
        ungetNextChar();//因为getNextChar函数会让输入的位置往前走一格，所以要回退一格

        // 返回tokenString的tokenID
        auto it = reservedWords.find(tokenString);
        if (it != reservedWords.end()) {
            return it->second;
        }
        return ID;
    }

    //如果是数字
    if (isdigit(c)) {
        tokenString = c;
        while (is_ledal && isdigit((c = getNextChar()))) {
            tokenString += c;
        }
        ungetNextChar();
        return NUM;
    }

    //如果是特殊符号
    tokenString = c;
    switch (c) {
    case ':':
        if (is_ledal && getNextChar() == '=') {
            //:=
            tokenString += '=';
            return ASSIGN;
        }
        break;
    case '+':
        if (is_ledal && getNextChar() == '=') {
            //如果是+=
            tokenString += '=';
            return ADD_ASSIGN;
        }
        ungetNextChar();
        //也可能是+
        return ADD;
    case '-':
        if (is_ledal && getNextChar() == '=') {
            //如果是-=
            tokenString += '=';
            return SUB_ASSIGN;
        }
        ungetNextChar();
        //也可能是-
        return SUB;
    case '<':
        if (is_ledal) {
            char nextChar = getNextChar();
            if (nextChar == '=') { // <=
                tokenString += '=';
                return LESS_EQUAL;
            }
            else if (nextChar == '>') {  //<>
                tokenString += '>';
                return NOT_EQUAL;
            }
            else {
                // <
                ungetNextChar();
                return LESS;
            }
        }
        break;
    case '>':
        if (is_ledal && getNextChar() == '=') { // >=
            tokenString += '=';
            return GREATER_EQUAL;
        }
        else {
            ungetNextChar();
            return GREATER;
        }
        break;
    case '*': return MUL;
    case '/': return DIV;
    case '%': return MOD;
    case '^': return POWER;
    case '=': return EQUAL;
    case '(': return LPAREN;
    case ')': return RPAREN;
    case ';': return SEMI;
    }

    return ERROR;
}