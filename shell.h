#include "sheel.h"  // Assume this contains necessary macros and declarations
#define DEL " "  // Token delimiter

typedef struct s_builtin {
    const char *name;
    int (*func)(char **args);  
} s_builtin;

// Function prototypes
void print_banner_from_file(const char *filename);
char *sheel_line(void);
void split_line(char *line);
int shell_echo(char **args);  
int shell_env(char **args);  
int shell_exit(char **args);  
void sheel_exec(char **args);
void shell_launch(char **args);  
int shell_ls(char **args);
int shell_help(char **args);
int   shell_cd(char **args);
int shell_cls(char **args);
int shell_touch(char **args);
int shell_mkdir(char **argv);
s_builtin s_builtins[] = {
    { .name = "echo", .func = shell_echo },
    { .name = "env",  .func = shell_env },
    { .name = "exit", .func = shell_exit },
    {.name = "ls" , .func = shell_ls},
    { .name = "help", .func = shell_help },
    {.name = "pwd" ,.func = shell_env},
    {.name = "cd" , .func = shell_cd},
    {.name = "cls", .func = shell_cls},
    {.name = "touch", .func = shell_touch},
    {.name = "mkdir", .func = shell_mkdir},


    { .name = NULL }
};

int main() {
    char *line;

    print_banner_from_file("banner.txt");

    while ((line = sheel_line()) != NULL) {
        split_line(line);
        free(line);
    }

    p("\n\n");
    p(GREEN"EXACTING SHELL...\n");
    p(YELLOW "Exited input loop.\n" RESET);
    return 0;
}

char *sheel_line(void) {
    char *buff = NULL;
    size_t buffsize = 0;
    ssize_t nread;
    char cwd[BUFFERSIZE];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd failed");
        return NULL;
    }

    p(GREEN "$ < %s >  > ", cwd);
    nread = getline(&buff, &buffsize, stdin);

    if (nread == -1) {
        if (feof(stdin)) {
            p(RED "EOF detected\n" RESET);
        } else {
            p(RED "getline failed\n" RESET);
        }
        free(buff);
        return NULL;
    }

    return buff;
}

void print_banner_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening banner file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf(GREEN "%s\n", line);
    }

    fclose(file);
}

void split_line(char *line) {
    char *args[64];  // Max 64 tokens
    int i = 0;

    line[strcspn(line, "\n")] = 0;

    char *token = strtok(line, DEL);
    while (token != NULL && i < 63) {
        args[i++] = token;
        token = strtok(NULL, DEL);
    }
    args[i] = NULL;

    if (i == 0) return;

    sheel_exec(args);
}

void sheel_exec(char **args) {
    if (args[0] == NULL) return;

    const char *curr;
    int z = 0;

    while ((curr = s_builtins[z].name)) {
        if (strcmp(args[0], curr) == 0) {
            int res = s_builtins[z].func(args);
            if (res != 0) {
                p(RED "error: %s\n" RESET, curr);
            }
            return;  // Important: Stop here if built-in matched
        }
        z++;
    }

    // Not a built-in — try launching external command
    shell_launch(args);
}

void shell_launch(char **args) {
    pid_t pid = fork();
int shell_cls(char **args);

    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");  // Only runs if execvp fails
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork");
    }
}
int shell_exit(char **args){
    exit(0);
}

int shell_echo(char **args){
    if(args[1] == NULL){
        p("\n");
    }else{
        for(int s = 1 ; args[s]!= NULL;s++){
            p("%s",args[s]);
        }
        p("\n");
    }
    return 0 ;
}
int shell_env(char **args){

    extern char **environ;
    char **env = environ;
    while(*env){
        p("%s\n",*env);
        env++;
    }
    return 0;
}
int shell_ls(char **args){
pid_t pid = fork();
if (pid < 0) {
        perror("fork");
        return 1;
    }
if(pid == 0){
    if(execvp(args[0],args)==-1){
        perror("execvp");
        exit(EXIT_FAILURE);
    }
   
   }   else {
        int status;
        waitpid(pid,&status,0);

    }
    return 0;

}
int shell_help(char **args) {

    p(GREEN "Available commands:\n" RESET);
    for (int i = 0; s_builtins[i].name != NULL; i++) {
        p(GREEN"  %s\n", s_builtins[i].name);
    }
    return 0;
}


int   shell_cd(char **args){
    if(args[1] == NULL){
        p(RED "cd: missing argument\n",RESET);
        return 1;
    }
 if(chdir(args[1]) != 0){
    perror("cd");
    return 1;
 }else{
    char cwd[BUFFERSIZE];
    if(getcwd(cwd,sizeof(cwd))!= NULL){
        p(GREEN"changed directory to : %s\n"RESET,cwd);
        
    }else{
        perror("getcwd");
        return 1;
    }
 }
 return 0;
}

int shell_cls(char **args){
    if(system("clear" ) == -1) {
        perror("clear"); 
        return -1; }
return 0;
}
int shell_touch(char **args){
    if(args[1] == NULL){
        p(RED"touch missing name\n"RESET);
        return -1;
    }
    else {
        FILE *file = fopen(args[1], "a");
        if(file==NULL){
            perror("touch");
            return -1;
          }  else{
            fclose(file);
             
            }
        }
    }


int shell_mkdir(char **argv){
        if(argv[1] == NULL){
            p(RED"mkdir : missing argument\n"RESET);
            return -1;
        }
        for(int s = 1 ; argv[s] != NULL;s++){
            if(mkdir(argv[s],0755) == -1){
                 perror(argv[s]);
                 continue;
                
            }
        }
        return 0;
    }

  
