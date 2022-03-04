#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

struct File;
struct Dir;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

/* functia returneaza un pointer la o structura de tip File */
File *create_file(Dir *parent, char *name);

/* functia elibereaza memoria ocupata de o structura File */
void free_file(File *file);

/* functia returneaza un pointer la o structura de tip Dir */
Dir *create_dir(Dir *parent, char *name);

/* functia elibereaza memoria ocupata de o strucutura Dir */
void free_dir(Dir *dir);

/* functia elibereaza recursiv memoria ocupata de o lista de directoare */
void free_dir_recursive(Dir *dir);

void touch (Dir* parent, char* name);

void mkdir (Dir* parent, char* name);

void ls (Dir* parent);

void rm (Dir* parent, char* name);

void rmdir (Dir* parent, char* name);

void cd(Dir** target, char *name);

char *pwd (Dir* target);

void stop (Dir* target);

void tree (Dir* target, int level);

void mv(Dir* parent, char *oldname, char *newname);

#endif /* FILE_SYSTEM_H_ */
