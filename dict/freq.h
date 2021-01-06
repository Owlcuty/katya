#include <map>
#include <string>

// necessary to delete[]
char *getTextNew(const char *filename);

typedef std::map<std::string, size_t> FrequencyMap_t;

FrequencyMap_t setFrequencyWords(char *text, size_t len_text);