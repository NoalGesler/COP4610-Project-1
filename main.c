#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct {
	int size;
	char **items;
} tokenlist;

char *get_input(void);
tokenlist *get_tokens(char *input);

tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);
void expand_Env(tokenlist *tokens); //Expands tokens with ~(home) or $(env), no return
char* path_Search(tokenlist *tokens); //Returns string with command location for use in execv(), only use with commands with no "/"

int main()
{
	int x = 1;
	while (x==1) {
		printf("%s@%s:%s> ", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */

		char *input = get_input();

		tokenlist *tokens = get_tokens(input);
		for(int i = 0; i < tokens->size; i++){
			printf("token %d: (%s)\n", i, tokens->items[i]);
		}
		expand_Env(tokens);
                char* command_check = (char *) malloc(sizeof(tokens->items[0]));
                strcpy(command_check, tokens->items[0]);
                command_check = strchr(command_check, '/');
                char* cmd_Path = (char *) malloc(sizeof(tokens->items[0]));
                if(command_check == NULL){
                        cmd_Path = path_Search(tokens);
                }
                else{
                        strcpy(cmd_Path, tokens->items[0]);
                }
                if(cmd_Path != NULL){
			int pid = fork();
			if(pid == 0){
				execv(tokens->items[0], tokens->items);
			}
			else{
				waitpid(pid, NULL, 0);
			}
                }
		free(input);
		free_tokens(tokens);
		x = 0;		
	}

	return 0;
}

tokenlist *new_tokenlist(void)
{
	tokenlist *tokens = (tokenlist *) malloc(sizeof(tokenlist));
	tokens->size = 0;
	tokens->items = (char **) malloc(sizeof(char *));
	tokens->items[0] = NULL; /* make NULL terminated */
	return tokens;
}

void add_token(tokenlist *tokens, char *item)
{
	int i = tokens->size;

	tokens->items = (char **) realloc(tokens->items, (i + 2) * sizeof(char *));
	tokens->items[i] = (char *) malloc(strlen(item) + 1);
	tokens->items[i + 1] = NULL;
	strcpy(tokens->items[i], item);

	tokens->size += 1;
}

char *get_input(void)
{
	char *buffer = NULL;
	int bufsize = 0;

	char line[5];
	while (fgets(line, 5, stdin) != NULL) {
		int addby = 0;
		char *newln = strchr(line, '\n');
		if (newln != NULL)
			addby = newln - line;
		else
			addby = 5 - 1;

		buffer = (char *) realloc(buffer, bufsize + addby);
		memcpy(&buffer[bufsize], line, addby);
		bufsize += addby;

		if (newln != NULL)
			break;
	}

	buffer = (char *) realloc(buffer, bufsize + 1);
	buffer[bufsize] = 0;

	return buffer;
}

tokenlist *get_tokens(char *input)
{
	char *buf = (char *) malloc(strlen(input) + 1);
	strcpy(buf, input);

	tokenlist *tokens = new_tokenlist();

	char *tok = strtok(buf, " ");
	while (tok != NULL) {
		add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}

	free(buf);
	return tokens;
}

void free_tokens(tokenlist *tokens)
{
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);
	free(tokens->items);
	free(tokens);
}

void expand_Env(tokenlist *tokens){
	for(int i = 0; i < tokens->size; i++){
		if(tokens->items[i][0] == '$'){
			char *temp = tokens->items[i] + 1;
			char *expanded = getenv(temp);
			int j = strlen(getenv(temp));
			expanded[j] = '\0';
			tokens->items[i] = (char *) realloc(tokens->items[i], strlen(expanded));
			strcpy(tokens->items[i], expanded);
		}
		else if(tokens->items[i][0] == '~'){
			char *temp = tokens->items[i] + 1;
			char *home = getenv("HOME");
			strcat(home, temp);
			tokens->items[i] = (char *) realloc(tokens->items[i], strlen(home));
			strcpy(tokens->items[i], home);
		}
	}
}

char* path_Search(tokenlist *tokens){
	char *path = getenv("PATH");
	char *command = (char *) malloc(strlen(tokens->items[0]) + 1);
	strcpy(command, tokens->items[0]);
	int max = strlen(path) + strlen(command) + 1;
	int endcheck = 0;
	int success = 0;
	char *pathcopy = (char *) malloc(strlen(path));
	strcpy(pathcopy, path);
	while(endcheck == 0){
		char *current = (char *) malloc(strlen(path));
		strcpy(current, pathcopy);
		current = strtok(current, ":");
		pathcopy = strchr(pathcopy, ':');
		if(pathcopy == NULL){
			endcheck = 1;
		}
		else{
			pathcopy = pathcopy + 1;
		}
		strcat(current, "/");
		strcat(current, command);
		FILE *tester = fopen(current, "r");
		if(tester){
			tokens->items[0] = (char *) realloc(tokens->items[0], strlen(current));
			strcpy(tokens->items[0], current);
			return current;
		}
	}
	printf("Command not found\n");
	char notfound[100] = "DNE";
	strcpy(path, notfound);
	return path;
}
