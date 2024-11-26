#include "scan.h"
#include "globals.h"
#include <cctype>
#include <unordered_map>

static std::string input;
static size_t currentPos = 0;
static int lineno = 1;
//�洢���еĹؼ���
static std::unordered_map<std::string, TokenType> reservedWords = {
    {"if", IF},           //if
    {"then", THEN},       //then
    {"else", ELSE},       //else
    {"end", END},         //��������
    {"repeat", REPEAT},   //repeat
    {"until", UNTIL},     //until
    {"read", READ},       //read
    {"write", WRITE},     //write
    {"while", WHILE},     //while
    {"do", DO},           //do
    {"enddo", ENDDO},     //����do
    {"for", FOR},         //for
    {"to", TO},           //to
};

//��ʼ��
void init(const std::string& inputText) {
    input = inputText;  //����
    currentPos = 0;     //��¼��ǰλ��
    lineno = 1;         //��¼��ǰ����
}

//��ȡ��һ���ַ�
char getNextChar() {
    if (currentPos >= input.length()) return EOF;
    return input[currentPos++];
}

//����һ���ַ�
void ungetNextChar() {
    if (currentPos > 0) currentPos--;
}

//�ж��Ƿ�Ϸ�������ǰλ���Ƿ��Ѿ������������յ�
bool is_ledal() {
    return currentPos < input.length();
}

TokenType getToken() {
    tokenString = "";
    TokenType currentToken;

    // �����հ��ַ���ע��
    bool skip = true;
    while (skip && is_ledal) {
        char c = getNextChar();
        // �����ո�,�������з�Ҫ�ǵ���lineno��һ
        if (isspace(c)) {
            if (c == '\n') lineno++;
            continue;
        }
        // ����ע�ͣ��������з�Ҫ�ǵ���lineno��һ
        if (c == '{') {
            while ((c = getNextChar()) != '}' && c != EOF) {
                if (c == '\n') lineno++;
            }
            continue;
        }
        //�ܵ���һ����˵��û�������ո��ע�ͣ���������ѭ����
        skip = false;
    }
    //û��token��ֱ�ӽ���
    if (currentPos >= input.length()) {
        return ENDFILE;
    }

    char c = input[currentPos - 1];

    // �����ʶ���͹ؼ���
    if (isalpha(c)) {
        //�������ĸ
        tokenString = c;
        while (is_ledal && isalpha((c = getNextChar()))) {
            tokenString += c;
        }
        ungetNextChar();//��ΪgetNextChar�������������λ����ǰ��һ������Ҫ����һ��

        // ����tokenString��tokenID
        auto it = reservedWords.find(tokenString);
        if (it != reservedWords.end()) {
            return it->second;
        }
        return ID;
    }

    //���������
    if (isdigit(c)) {
        tokenString = c;
        while (is_ledal && isdigit((c = getNextChar()))) {
            tokenString += c;
        }
        ungetNextChar();
        return NUM;
    }

    //������������
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
            //�����+=
            tokenString += '=';
            return ADD_ASSIGN;
        }
        ungetNextChar();
        //Ҳ������+
        return ADD;
    case '-':
        if (is_ledal && getNextChar() == '=') {
            //�����-=
            tokenString += '=';
            return SUB_ASSIGN;
        }
        ungetNextChar();
        //Ҳ������-
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