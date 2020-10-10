bool parse_property_from_config_file(const char* key, char *value);
bool parse_property_from_config_file(const char* filename, const char* key, char *value);
void LOG(const char* fmt,...);
void string_trim(char *s);

void handle_exception();

//here is the api opeating files

bool file_exists(const char * filename);
int get_file_size(const char *file);