#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileSystem.h"

#define MAX_INPUT_LINE_SIZE 300
#define MAX_NAME_LEN 50

int main () {
	char *line = malloc(sizeof(*line) * (MAX_INPUT_LINE_SIZE + 1));
	if (!line) {
		fprintf(stderr, "malloc failed\n");
		return -1;
	}

	Dir *home = create_dir(NULL, "home");
	Dir *target = home;

	while (fgets(line, MAX_INPUT_LINE_SIZE, stdin)) {
		/*
		Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/
		line[strlen(line) - 1] = '\0';

		if (strstr(line, "touch") == line) {
			char *command = malloc(sizeof(*command) * MAX_NAME_LEN);
			char *name = malloc(sizeof(*name) * MAX_NAME_LEN);

			/* se sparge linia citita in comanda si nume */
			sscanf(line, "%s %s", command, name);

			touch(target, name);

			free(command);
			free(name);
			continue;
		}

		if (strstr(line, "mkdir") == line) {
			char *command = malloc(sizeof(*command) * MAX_NAME_LEN);
			char *name = malloc(sizeof(*name) * MAX_NAME_LEN);

			/* se sparge linia citita in comanda si nume */
			sscanf(line, "%s %s", command, name);

			mkdir(target, name);

			free(command);
			free(name);
			continue;
		}

		if (strstr(line, "ls") == line) {
			ls(target);
			
			continue;
		}

		if (strstr(line, "rmdir") == line) {
			char *command = malloc(sizeof(*command) * MAX_NAME_LEN);
			char *name = malloc(sizeof(*name) * MAX_NAME_LEN);

			/* se sparge linia citita in comanda si nume */
			sscanf(line, "%s %s", command, name);

			rmdir(target, name);

			free(command);
			free(name);
			continue;
		}

		if (strstr(line, "rm") == line) {
			char *command = malloc(sizeof(*command) * MAX_NAME_LEN);
			char *name = malloc(sizeof(*name) * MAX_NAME_LEN);

			/* se sparge linia citita in comanda si nume */
			sscanf(line, "%s %s", command, name);

			rm(target, name);

			free(command);
			free(name);
			continue;
		}

		if (strstr(line, "cd") == line) {
			char *command = malloc(sizeof(*command) * MAX_NAME_LEN);
			char *name = malloc(sizeof(*name) * MAX_NAME_LEN);

			/* se sparge linia citita in comanda si nume */
			sscanf(line, "%s %s", command, name);

			cd(&target, name);

			free(command);
			free(name);
			continue;
		}

		if (strstr(line, "tree") == line) {
			tree(target, 0);

			continue;
		}

		if (strstr(line, "pwd") == line) {
			char *path = pwd(target);
			printf("%s\n", path);

			free(path);
			continue;
		}

		if (strstr(line, "mv") == line) {
			char *command = malloc(sizeof(*command) * MAX_NAME_LEN);
			char *oldname = malloc(sizeof(*oldname) * MAX_NAME_LEN);
			char *newname = malloc(sizeof(*newname) * MAX_NAME_LEN);

			/* se sparge linia citita in comanda si doua nume */
			sscanf(line, "%s %s %s", command, oldname, newname);

			mv(target, oldname, newname);

			free(command);
			free(oldname);
			free(newname);
			continue;
		}

		if (strstr(line, "stop") == line) {
			stop(home);

			free(line);

			break;
		}
	}
	
	return 0;
}
