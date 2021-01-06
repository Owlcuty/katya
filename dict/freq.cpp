#include "freq.h"
#include <iostream>
#include <fstream>
#include <cerrno>
#include <string.h>

char *getTextNew(const char *filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in.fail())
	{
		return NULL;
	}

	ssize_t size_file = in.seekg(0, std::ios::end).tellg();
	if (size_file == -1)
	{
		return NULL;
	}
	in.seekg(0);

	char *buf = new char[size_file + 1];
	if (buf == 0)
	{
		return NULL;
	}

	in.read(buf, size_file);
	buf[size_file] = 0;

	return buf;
}

class tolowerConverter
{
public:
	tolowerConverter();
	char getLower(char c);
	void strToLower(char *str);
	void strToLower(char *str, bool &is_changed);
private:
	char lowBuf[256];
};

tolowerConverter::tolowerConverter()
{
	for (size_t c = 0; c < 256; ++c)
	{
		lowBuf[c] = std::tolower((char)c);
	}
}

char tolowerConverter::getLower(char c)
{
	return lowBuf[size_t(c)];
}

void tolowerConverter::strToLower(char *str)
{
	if (str == nullptr)
	{
		throw std::runtime_error("toLower nullptr");
	}
	for (char *cur = str; cur < str + strlen(str); ++cur)
	{
		*cur = getLower(*cur);
	}
}

void tolowerConverter::strToLower(char *str, bool &is_changed)
{
	if (str == nullptr)
	{
		throw std::runtime_error("toLower nullptr");
	}
	for (char *cur = str; cur < str + strlen(str); ++cur)
	{
		if (*cur != getLower(*cur))
			is_changed = true;
		*cur = getLower(*cur);
	}
}

// Dangerous!!! : deteriorate original string
char *getNextWord_DNGRS(char *src)
{
	const char *delim = " `~!@#$%^&*()№;:?[]\\/.,<>{}|\"'\n\t\r";

	char *ret = strtok(src, delim);
	return ret;
}

typedef std::map<std::string, size_t> FrequencyMap_t;

FrequencyMap_t setFrequencyWords(char *text, size_t len_text)
{
	FrequencyMap_t result;
	tolowerConverter low_conv;

	char *buf_text = new char[len_text + 1];
	memcpy(buf_text, text, len_text + 1);
	buf_text[len_text] = 0; // if got text without \0

	char *cur = getNextWord_DNGRS(buf_text);
	char *cur_orig = text;

	std::string word = "";
	std::string low_word = "";

	while (cur)
	{
		word = cur;
		bool is_begin = false;
		if (cur == buf_text)
		{
			is_begin = true;
		}
		else
		{
			char *tmp_point = strchr(cur_orig, '.');
			if (tmp_point != NULL)
			{
				if (tmp_point < text + (cur - buf_text))
				{
					is_begin = true;
				}
			}
		}
		if (is_begin)
		{
			if (result.find(word) != result.end())
			{
				result.at(word)++;
			}
			else
			{
				low_conv.strToLower(const_cast<char*>(word.c_str()));
				result[word]++;
			}
		}
		else
		{
			low_word = cur;
			bool is_changed = false;
			low_conv.strToLower(const_cast<char*>(low_word.c_str()), is_changed);
			if (is_changed == false)
			{
				result[word]++;
			}
			else
			{
				if (result.find(low_word) != result.end())
				{
					result[word] += result.at(low_word);
					result.erase(low_word);
				}
				result[word]++;
			}
		}
		cur_orig = text + (cur - buf_text);
		cur = getNextWord_DNGRS(0);
	}

	delete[] buf_text;
	return result;
}
