#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_map>

using namespace std;

enum TokenType {
    USING_KEYWORD,
    INT_KEYWORD,
    MAIN_KEYWORD,
    CIN_KEYWORD,
    COUT_KEYWORD,
    FOR_KEYWORD,
    IF_KEYWORD,

    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    LEFT_CURLY_BRACE,
    RIGHT_CURLY_BRACE,
    LEFT_SQUARE_BRACKET,
    RIGHT_SQUARE_BRACKET,

    SEMICOLON,

    OUTPUT_OPERATOR,
    INSERTION_OPERATOR,
    QUOTATION_MARK,
    
    PLUS_OPERATOR,
    EQUALS_SIGN,
    MULTIPLICATION_OPERATOR,
    DIVISION_OPERATOR,
    INC_OPERATOR,

    GREATER_THAN_OPERATOR,
    LESS_THAN_OPERATOR,

    IDENTIFIER,
    NUMBER,
    SYSTEM_HEADER_INCLUDE,
    PREPROCESSOR_DIRECTIVE,
    STRING_LITERAL,
    UNKNOWN
};

std::unordered_map<std::string, int> tokenMap = {
    {"using", USING_KEYWORD},
    {"int", INT_KEYWORD},
    {"main", MAIN_KEYWORD},
    {"cin", CIN_KEYWORD},
    {"cout", COUT_KEYWORD},
    {"for", FOR_KEYWORD},
    {"if", IF_KEYWORD},
    {"(", LEFT_PARENTHESIS},
    {")", RIGHT_PARENTHESIS},
    {"{", LEFT_CURLY_BRACE},
    {"}", RIGHT_CURLY_BRACE},
    {"[", LEFT_SQUARE_BRACKET},
    {"]", RIGHT_SQUARE_BRACKET},
    {";", SEMICOLON},
    {"<<", OUTPUT_OPERATOR},
    {">>", INSERTION_OPERATOR},
    {"\"", QUOTATION_MARK},
    {"+", PLUS_OPERATOR},
    {"/", DIVISION_OPERATOR},
    {"=", EQUALS_SIGN},
    {"*", MULTIPLICATION_OPERATOR},
    {"++", INC_OPERATOR},
    {">", GREATER_THAN_OPERATOR},
    {"<", LESS_THAN_OPERATOR}
};

string input;
int token;
string lexeme("");
int idx = 0;

string readFileContents(const string& filename);
char getChar();
bool match(string expectedLexeme);
void lex();
void getLexeme();
void reset();

void program();
void includeDirective();
void stringContent();
void namespaceDeclaration();
void identifier();
void statement();
void variableDeclaration();

int main()
{
    string filename = "in.cpp";
    input = readFileContents(filename);
    if (input.empty()) {
        return 1;
    }

    program();

    return 0;
}

void program()
{
    lex();
    if (token == PREPROCESSOR_DIRECTIVE)
    {
        includeDirective();
    }

    reset();
    lex();
    if (token == USING_KEYWORD)
    {
        namespaceDeclaration();
    }

    reset();
    lex();

    reset();
    lex();

    reset();
    lex();

    reset();
    lex();

    reset();
    lex();

    statement();
}

void includeDirective()
{
    reset();
    lex();

    reset();
    lex();

    stringContent();

    reset();
    lex();
}

void stringContent()
{
    reset();
    lex();
}

void namespaceDeclaration()
{
    reset();
    lex();

    reset();
    lex();

    identifier();

    reset();
    lex();
}

void identifier()
{
    getLexeme();
}

void statement()
{
    reset();
    lex();
    if (token == INT_KEYWORD)
    {
        variableDeclaration();
    }
}

void variableDeclaration()
{
    identifier();

    reset();
    lex();

    reset();
    lex();
}

void lex()
{
    char c;
    do {
        c = getChar();
        if (isspace(c))
        {
            ++idx;
        }
    } while (isspace(c));

    switch (c)
    {
    case '#':
        lexeme += c;
        token = PREPROCESSOR_DIRECTIVE;
        break;
    
    case '<':
        lexeme += c;
        token = LESS_THAN_OPERATOR;
        break;
    
    case '>':
        lexeme += c;
        token = GREATER_THAN_OPERATOR;
        break;
    
    case ';':
        lexeme += c;
        token = SEMICOLON;
        break;

    case '(':
        lexeme += c;
        token = LEFT_PARENTHESIS;
        break;
    
    case ')':
        lexeme += c;
        token = RIGHT_PARENTHESIS;
        break;
    
    default:
        token = UNKNOWN;
        getLexeme();

        if(lexeme == "using")
        {
            token = USING_KEYWORD;
        }

        if(lexeme == "int")
        {
            token = INT_KEYWORD;
        }
    }

    cout << "The next Token is: " << token << ", the next Lexeme is: " << lexeme << endl;
}

void getLexeme()
{
    char c = getChar();
    while(
        !isspace(c)
    )
    {
        if (
            c == '>' || c == '>' ||
            c == ';' || c == '"' ||
            c == '(' || c == ')'
        )
        {
            --idx;
            return;
        }
        lexeme += c;
        ++idx;
        c = getChar();
    };
}

void reset()
{
    ++idx;
    lexeme = "";
}

char getChar()
{
    return input[idx];
}

bool match(string expectedLexeme)
{
    return (lexeme == expectedLexeme);
}

string readFileContents(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return "";
    }

    string contents;
    string line;
    while (getline(file, line)) {
        contents += line + '\n';
    }

    file.close();
    return contents;
}