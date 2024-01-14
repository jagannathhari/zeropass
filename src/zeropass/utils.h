#include<stdint.h>
#include <stdlib.h>
char *get_masterkey(const char *username, const char *site,
                    const char *master_password, int rounds); 
char *get_password(const char *to_include,const char *master_key, long unsigned int password_len);
void hidden_input(char *password);
