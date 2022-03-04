#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileSystem.h"

#define MAX_PATH_SIZE 100

File *create_file(Dir *parent, char *name) {
	/* se aloca memoria pentru structura */
	File *file = malloc(sizeof(*file));
	if (!file) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	/* se aloca memoria pentru numele fisierului si se initializeaza */
	file->name = malloc(strlen(name) + 1);
	if (!file->name) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}
	memcpy(file->name, name, strlen(name) + 1);

	/* se initializeaza restul campurilor */
	file->parent = parent;
	file->next = NULL;

	return file;
}

void free_file(File *file) {
	free(file->name);
	free(file);
}

Dir *create_dir(Dir *parent, char *name) {
	/* se aloca memoria pentru structura */
	Dir *dir = malloc(sizeof(*dir));
	if (!dir) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	/* se aloca memoria pentru numele directorului si se initializeaza */
	dir->name = malloc(strlen(name) + 1);
	if (!dir->name) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}
	memcpy(dir->name, name, strlen(name) + 1);

	/* se initializeaza restul campurilor */
	dir->parent = parent;
	dir->head_children_files = NULL;
	dir->head_children_dirs = NULL;
	dir->next = NULL;

	return dir;
}

void free_dir(Dir *dir) {
	/* se elibereaza memoria ocupata de fisiere */
	File *curr_file = dir->head_children_files;
	while (curr_file) {
		File *tmp = curr_file->next;
		free_file(curr_file);
		curr_file = tmp;
	}

	free(dir->name);

	free(dir);
}

void free_dir_recursive(Dir *dir) {
	if (!dir) {
		return;
	}

	free_dir_recursive(dir->next);

	free_dir_recursive(dir->head_children_dirs);
	
	free_dir(dir);
}

void touch (Dir* parent, char* name) {
	if (!parent->head_children_files) {
		/* insereaza primul fisier din lista de fisiere */
		File *new_file = create_file(parent, name);
		parent->head_children_files = new_file;
	} else {
		/*
		 se itereaza prin lista de fisiere pentru a gasi ultimul fisier din
		 lista, iar noul fisier se leaga de acesta 
		*/
		File *curr_file = parent->head_children_files, *prev_file = NULL;

		while (curr_file) {
			if (strcmp(curr_file->name, name) == 0) {
				printf("File already exists\n");
				return;
			}
			prev_file = curr_file;
			curr_file = curr_file->next;
		}

		File *new_file = create_file(parent, name);
		prev_file->next = new_file;
	}
}

void mkdir (Dir* parent, char* name) {
	/* insereaza primul fisier din lista de fisiere */
	if (!parent->head_children_dirs) {
		Dir *new_dir = create_dir(parent, name);
		parent->head_children_dirs = new_dir;
	} else {
		/*
		 se itereaza prin lista de directoare pentru a gasi ultimul director
		 din lista, iar noul director se leaga de acesta
		*/
		Dir *curr_dir = parent->head_children_dirs, *prev_dir = NULL;

		while (curr_dir) {
			if (strcmp(curr_dir->name, name) == 0) {
				printf("Directory already exists\n");
				return;
			}
			prev_dir = curr_dir;
			curr_dir = curr_dir->next;
		}

		Dir *new_dir = create_dir(parent, name);
		prev_dir->next = new_dir;
	}
}

void ls (Dir* parent) {
	/* se afiseaza lista cu directoare */
	Dir *curr_dir = parent->head_children_dirs;
	while (curr_dir) {
		printf("%s\n", curr_dir->name);
		curr_dir = curr_dir->next;
	}

	/* se afiseaza lista cu fisiere */
	File *curr_file = parent->head_children_files;
	while (curr_file) {
		printf("%s\n", curr_file->name);
		curr_file = curr_file->next;
	}
}

void rm (Dir* parent, char* name) {
	File *curr_file = parent->head_children_files, *prev_file = NULL;
	int found_file = 0;

	/* se parcurge lista de fisiere pentru a gasi fisierul care trbuie sters */
	while (curr_file) {
		if (strcmp(name, curr_file->name) == 0) {
			if (curr_file == parent->head_children_files) {
				/* 
				 fisierul care va fi sters se afla pe prima pozitie in lista
				 si trebuie actualizat parent->head_children_files
				 */
				parent->head_children_files = curr_file->next;
				free_file(curr_file);
			} else {
				/* 
				 fisierul care va fi sters trebuie scos din lista prin
				 actualizarea legaturii fisierului anterior lui
				 */
				prev_file->next = curr_file->next;
				free_file(curr_file);
			}
			found_file = 1;
			break;
		}
		prev_file = curr_file;
		curr_file = curr_file->next;
	}

	if (!found_file) {
		printf("Could not find the file\n");
	}
}

void rmdir (Dir* parent, char* name) {
	Dir *curr_dir = parent->head_children_dirs, *prev_dir = NULL;
	int found_dir = 0;

	/* se parcurge lista de fisiere pentru a gasi fisierul care trbuie sters */
	while (curr_dir) {
		if (strcmp(name, curr_dir->name) == 0) {
			if (curr_dir == parent->head_children_dirs) {
				/* 
				 directorul care va fi sters se afla pe prima pozitie in lista
				 si trebuie actualizat parent->head_children_dirs
				 */
				parent->head_children_dirs = curr_dir->next;
				free_dir_recursive(curr_dir->head_children_dirs);
				free_dir(curr_dir);
			} else {
				/* 
				 fisierul care va fi sters trebuie scos din lista prin
				 actualizarea legaturii directorului anterior lui
				 */
				prev_dir->next = curr_dir->next;
				free_dir_recursive(curr_dir->head_children_dirs);
				free_dir(curr_dir);
			}
			found_dir = 1;
			break;
		}

		prev_dir = curr_dir;
		curr_dir = curr_dir->next;
	}

	if (!found_dir) {
		printf("Could not find the dir\n");
	}
}

void cd(Dir** target, char *name) {
	/* 
	 se verifica daca trebuie schimbat directorul curent cu directorul
	 parinte
	*/
	if (strcmp(name, "..") == 0) {
		if ((*target)->parent) {
			*target = (*target)->parent;
		}
		return;
	}

	/* 
	 se itereaza prin lista de directoare pentru a gasi directorul cu
	 numele dat
	*/
	Dir *curr_dir = (*target)->head_children_dirs;

	while (curr_dir) {
		if (strcmp(name, curr_dir->name) == 0) {
			*target = curr_dir;
			return;
		}

		curr_dir = curr_dir->next;
	}

	printf("No directories found!\n");
}

/* 
 functia construieste recursiv in sirul path calea de la directorul dat la
 radacina
*/
static void build_path(char *path, Dir *target) {
	if (!target) {
		return;
	}

	build_path(path, target->parent);

	strcat(path, "/");
	strcat(path, target->name);
}

char *pwd (Dir* target) {
	/* se aloca memoria pentru sirul in care urmeaza sa fie construita calea */
	char *path = malloc(sizeof(*path) * MAX_PATH_SIZE);
	if (!path) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}
	path[0] = '\0';

	/* se apeleaza functia buid_path care construieste sirul path */
	build_path(path, target);

	return path;
}

void stop (Dir* target) {
	/* se elibereaza memoria alocata pentru sistemul de directoare */
	free_dir_recursive(target->head_children_dirs);

	free_dir(target);
}

void tree (Dir* target, int level) {
	if (!target) {
		return;
	}

	if (level != 0) {
		for (int i = 0; i < level - 1; ++i) {
			printf("    ");
		}
		printf("%s\n", target->name);
	}

	tree(target->head_children_dirs, level + 1);

	File *file = target->head_children_files;
	while (file) {
		for (int i = 0; i < level; ++i) {
			printf("    ");
		}
		printf("%s\n", file->name);
		file = file->next;
	}

	tree(target->next, level);
}

void mv(Dir* parent, char *oldname, char *newname) {
	File *curr_file = parent->head_children_files, *prev_file = NULL,
			*file = NULL;
	Dir *curr_dir = parent->head_children_dirs, *prev_dir = NULL,
			*dir = NULL;

	/* se verifica daca un fisier cu numele newname exista deja */
	while (curr_file) {
		if (strcmp(newname, curr_file->name) == 0) {
			printf("File/Director already exists\n");
			return;
		}
		curr_file = curr_file->next;
	}

	/* se verifica daca un director cu numele newname exista deja */
	while (curr_dir) {
		if (strcmp(newname, curr_dir->name) == 0) {
			printf("File/Director already exists\n");
			return;
		}
		curr_dir = curr_dir->next;
	}

	/* se cauta un fisier cu numele oldname */
	curr_file = parent->head_children_files;
	while (curr_file) {
		if (strcmp(oldname, curr_file->name) == 0) {
			memcpy(curr_file->name, newname, strlen(newname) + 1);

			if (curr_file == parent->head_children_files) {
				if (curr_file->next != NULL) {
					/* 
					 in acest caz fisierul cu numele oldname se afla pe prima
					 pozitie in lista si trebuie actualizat
					 parent->head_children_files
					*/
					parent->head_children_files = curr_file->next;
					file = curr_file;
				} else {
					/*
				 	 daca curr_file->next == NULL inseamna ca lista de fisiere
					 are un singur element si atunci nu mai trebuie mutat
					 fisierul
					*/
					return;
				}
			} else {
				/* 
				 fisierul trebuie scos din lista pentru a fi adaugat la final
				 ulterior
				*/
				prev_file->next = curr_file->next;
				file = curr_file;
			}
		}

		prev_file = curr_file;
		curr_file = curr_file->next;
	}

	/* daca s-a gasit un fisier cu numele oldname se adauga la finalul listei */
	if (file) {
		prev_file->next = file;
		file->next = NULL;
		return;
	}

	/* se cauta un director cu numele oldname */
	curr_dir = parent->head_children_dirs;
	while(curr_dir) {
		if (strcmp(oldname, curr_dir->name) == 0) {
			memcpy(curr_dir->name, newname, strlen(newname) + 1);

			if (curr_dir == parent->head_children_dirs) {
				if (curr_dir->next != NULL) {
					/* 
					 in acest caz directorul cu numele oldname se afla pe prima
					 pozitie in lista si trebuie actualizat
					 parent->head_children_dirs
					*/
					parent->head_children_dirs = curr_dir->next;
					dir = curr_dir;
				} else {
					/*
				 	 daca curr_dir->next == NULL inseamna ca lista de
					 directoare are un singur element si atunci nu mai trebuie
					 mutat directorul
					*/
					return;
				}
			} else {
				/* 
				 directorul trebuie scos din lista pentru a fi adaugat la final
				 ulterior
				*/
				prev_dir->next = curr_dir->next;
				dir = curr_dir;
			}
		}

		prev_dir = curr_dir;
		curr_dir = curr_dir->next;
	}

	/* 
	 daca s-a gasit un director cu numele oldname se adauga la finalul
	 listei
	*/
	if (dir) {
		prev_dir->next = dir;
		dir->next = NULL;
		return;
	}

	printf("File/Director not found\n");
}
