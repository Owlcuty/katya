#include <iostream>
#include <fstream>
#include <cerrno>
#include <map>
#include <string.h>

#include "freq.h"

int main(int argc, char **argv)
{
	char *text = getTextNew("text.txt");
	if (text == NULL)
	{
		perror("fail: ");
		return -1;
	}

	FrequencyMap_t freq = setFrequencyWords(text, strlen(text));

	std::ofstream out("res.txt", std::ios::out);
	for (auto it = freq.begin(); it != freq.end(); ++it)
	{
		out << "{" << it->first << "}: " << it->second << std::endl;
	}

	delete[] text;

	return 0;
}
