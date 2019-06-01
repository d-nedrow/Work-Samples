#pragma once
#include <string>
#include <vector>

using namespace std;

// Split a string into a vector of tokens
void split(const string &s, vector<string> & v) {
	// Delimeters are whitespace
	const char* delim = " \t\n";
	char* nextToken;

	// to avoid modifying original string
	// first duplicate the original string and return a char pointer then free the memory
	char * dup = _strdup(s.c_str());
	char * token = strtok_s(dup, delim, &nextToken);
	while (token != NULL) {
		v.push_back(string(token));
		// the call is treated as a subsequent calls to strtok:
		// the function continues from where it left in previous invocation
		token = strtok_s(NULL, delim, &nextToken);
	}
	free(dup);
}
