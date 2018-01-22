#include "lexer.hpp"

void insertEOL(TokenList &list, unsigned int &line);
void insertERR(TokenList &list, unsigned int &line, bool &err);

// implement the lexer function here
TokenList tokenize(std::istream & ins)
{
	size_t line = 1;
	std::string result;
	char c;
	TokenList tokens;
	bool stringDelim = false;
	bool err = false;
	enum parenthesisCheck{OPEN, CLOSED} parenChk = CLOSED;

	while (ins.get(c) && err == false)
	{
		if (stringDelim == true) {
			while (c != '"')
			{
				result.push_back(c);
				ins.get(c);
				if (ins.fail()) break;
			}
			if (c == '"') {
				stringDelim = false;
				Token stringToken(STRING, line, result);
				tokens.emplace_back(stringToken);
				Token quoteToken(STRING_DELIM, line);
				tokens.emplace_back(quoteToken);
				result = "";
			}
			else
				insertERR(tokens, line, err);
		}
		else if (c == '#') {
			do {
				ins.get(c);
			} while (c != '\n' && !ins.fail());
			insertEOL(tokens, line);
		}
		else if (c == '(') {
			if (parenChk == OPEN)
				insertERR(tokens, line, err);
			else {
				Token openParenToken(OPEN_PAREN, line);
				tokens.emplace_back(openParenToken);
				parenChk = OPEN;
			}
		}
		else if (c == ')') {
			if (parenChk == CLOSED)
				insertERR(tokens, line, err);
			else {
				Token closeParenToken(CLOSE_PAREN, line);
				tokens.emplace_back(closeParenToken);
				parenChk = CLOSED;
			}
		}
		else if (c == ',') {
			Token sepToken(SEP, line);
			tokens.emplace_back(sepToken);
		}
		else if (c == '=') {
			Token equalToken(EQUAL, line);
			tokens.emplace_back(equalToken);
		}
		else if (c == '"') {
			Token quoteToken(STRING_DELIM, line);
			tokens.emplace_back(quoteToken);
			stringDelim = true;
		}
		else if (c == '\n' || ins.fail()) {
			if (!tokens.empty())
				insertEOL(tokens, line);
			else line++;
		}
		else if (isalnum(c) || ispunct(c)) {
			result.push_back(c);
			if (ins.peek() == '(' || ins.peek() == ')' || ins.peek() == '"' || 
				ins.peek() == ',' || (!isalnum(ins.peek()) && !ispunct(ins.peek()))) {
				Token stringToken(STRING, line, result);
				tokens.emplace_back(stringToken);
				result = "";
			}
			if (ins.fail())
				insertEOL(tokens, line);
		}
	}
	if (parenChk == OPEN)
		insertERR(tokens, line, err);
	return tokens;
}


// Helper function to insert EOL token
void insertEOL(TokenList &list, unsigned int &line)
{
	Token eolToken(EOL, line);
	list.emplace_back(eolToken);
	line++;
}

void insertERR(TokenList &list, unsigned int &line, bool &err)
{
	Token errToken(ERROR, line, "Error: ");
	list.emplace_back(errToken);
	err = true;
}