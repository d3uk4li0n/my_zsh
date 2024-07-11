#include "../include/my_zsh.h"

//run built in command
int processBuiltinCommand(arguments* argv);

//parse
arguments* parseCommand(char* line, int ac, char** av, char** env);

//count number of arguments including the command
//such as "ls -l" the number of arguments is 2
int countCommandArgs(char* line);

//get absolute path
char* getAbsolutePath(char* path, char** env);

/*
main function to start C application
*/
int main(int ac, char** av, char** env){

    int i;

    //run command and process it
    while (1){

        // Read command line 
        printf("%s ", "GTN - my_zsh $>");
        fflush(stdout);

        char* cmdline = read_input();

        //empty command
        if (my_strlen(cmdline) == 0)
        {
            continue;
        }        

        arguments* cmd = parseCommand(cmdline, ac, av, env);
        free(cmdline);

        if (processBuiltinCommand(cmd) == 0) //0 means not yet run
        {
            //fprintf(stderr, "debug: cmd->paths[0] = %s\n", cmd->paths[0]);

            //get absolute path
            char* absolutePath = getAbsolutePath(cmd->paths[0], env);

            //fprintf(stderr, "debug: absolutePath = %s\n", absolutePath);

            if (absolutePath == NULL)
            {
                printf("%s: Command not found\n", cmd->paths[0]); //handling error
            }else{   

                free(cmd->paths[0]);         
                cmd->paths[0] = absolutePath;

                //process id 
                pid_t pid = fork();

                // error handling 
                if (pid < 0){
                    fprintf(stderr, "fgets error\n");
                    exit(EXIT_FAILURE); 
                }     

                //check child process
                if (pid == 0)
                {   
                    if (execve(cmd->paths[0], cmd->paths, cmd->env) < 0) //child running user job
                    {
                        printf("%s: Command not found\n", cmd->paths[0]); //handling error
                        exit(EXIT_FAILURE); 
                    }
                }else{
                    int status;
                    pid_t w;

                    do {
                        w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
                        if (w == -1) { 
                            fprintf(stderr, "waitpid error\n");
                            exit(EXIT_FAILURE); 
                        }
                    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                }
            }
        }

        for (i = 0; i < cmd->paths_size; i++)
        {
            free(cmd->paths[i]);
        }
        
        free(cmd->paths);
        free(cmd);
    }

    return 0;
}

//count number of arguments including the command
//such as "ls -l" the number of arguments is 2
int countCommandArgs(char* line){

    char* copyOfLine = strdup(line);
    int nTokens = 0;

    char* ptr;
    char* tok = strtok_r(copyOfLine, " \n", &ptr);

    while (tok != NULL)
    {
        nTokens++;

        //nex token
        tok = strtok_r(NULL, " \n", &ptr);
    }

    free(copyOfLine);

    return nTokens;

}

//parse
arguments* parseCommand(char* line, int ac, char** av, char** env){

    arguments* argv = (arguments*)malloc(sizeof(arguments));

    //enviroment
    int i = 0;
    for (; env[i] != NULL; i++){
       
    }
    argv->env = env;
    argv->env_length = i;
    argv->size = ac;
    argv->args = av;

    //number of tokens
    int nTokens = countCommandArgs(line);
    argv->paths_size = nTokens;

    //create arguments
    argv->paths = (char**)malloc(sizeof(char*) * (nTokens + 1));

    char* copyOfLine = strdup(line);
    char* ptr;
    char* tok = strtok_r(copyOfLine, " \n", &ptr);
    i = 0;
    while (tok != NULL)
    {
        argv->paths[i] = malloc((strlen(tok) + 1)* sizeof(char));
        strcpy(argv->paths[i], tok);
        i++;

        //nex token
        tok = strtok_r(NULL, " \n", &ptr);
    }

    //last argument is NULL
    argv->paths[i] = NULL;

    free(copyOfLine);

    //set builtins
    if (my_strcmp(argv->paths[0], "cd") == 0)
    {
        argv->command = CD;
    }else if (my_strcmp(argv->paths[0], "echo") == 0)
    {
        argv->command = ECHO;
    }else if (my_strcmp(argv->paths[0], "setenv") == 0)
    {
        argv->command = SETENV;
    }else if (my_strcmp(argv->paths[0], "unsetenv") == 0)
    {
        argv->command = UNSETENV;
    }else if (my_strcmp(argv->paths[0], "env") == 0)
    {
        argv->command = ENV;
    }else if (my_strcmp(argv->paths[0], "exit") == 0)
    {
        argv->command = EXIT;
    }else if (my_strcmp(argv->paths[0], "pwd") == 0)
    {
        argv->command = PWD;
    }else if (my_strcmp(argv->paths[0], "which") == 0)
    {
        argv->command = WHICH;
    }else{
        argv->command = RUN; //or EXECUTE
    }
    

    return argv;
}

//get absolute path
char* getAbsolutePath(char* path, char** env){

    int i;
    char fullPath[MAX_STR_LEN];

    for (i = 0; env[i] != NULL; i++)
    {
        if (my_strncmp(env[i], "PATH=", 5) == 0)
        {
            char* copyOfLine = strdup(env[i]);
            char* ptr;
            char* tok = strtok_r(copyOfLine, ":", &ptr);
            while (tok != NULL)
            {                
                sprintf(fullPath, "%s/%s", tok, path);

                //check is program
                if (access(fullPath, F_OK) == 0) {

                    free(copyOfLine);
                    
                    char* absolutePath = (char*)malloc(sizeof(char) * (strlen(fullPath) + 1));
                    my_strcpy(absolutePath, fullPath);
                    return absolutePath;            
                }

                //nex token
                tok = strtok_r(NULL, ":", &ptr);
            }


            free(copyOfLine);

        }
    }

    return NULL;

}

//run built in command
int processBuiltinCommand(arguments* argv)
{
    int i;
    if (argv->command == EXIT) //EXIT command
    {
        exit(0);
    }else if (argv->command == CD) //CD command
    {
        if (argv->paths_size == 1)
        {
            chdir(getenv("HOME"));
            setenv("PWD", getenv("HOME"), 1);
        }
        else if (argv->paths_size > 1){
            if (chdir(argv->paths[1]) == 0) {
                setenv("PWD", argv->paths[1], 1);
            }
        }

        return 1;
    }    
    else  if (argv->command == ECHO) //echo
    {
        if (argv->paths[1][0] == '$')
        {
            printf("%s\n", getenv(argv->paths[1] + 1));
        }else{

            for (i = 1; i < argv->paths_size; i++)
            {
                printf("%s ",argv->paths[i]);
            }
            printf("\n");
        }

        return 1;
    }
    else  if (argv->command == SETENV) //SETENV
    {
        if (argv->paths_size >= 3)
        {
            setenv(argv->paths[1], argv->paths[2], 1);
        }

        return 1;
    }
    else  if (argv->command == UNSETENV) //UNSETENV
    {
        if (argv->paths_size >= 2)
        {
            unsetenv(argv->paths[1]);
        }

        return 1;
    }
    else  if (argv->command == ENV) //ENV
    {
        if (argv->paths_size >= 2)
        {
            char* res = getenv(argv->paths[1]);
            if (res == NULL)
            {
                printf("%s\n", "Not found!\n");
            }else{            
                printf("%s\n", res);
            }
        }

        return 1;
    }
    else  if (argv->command == PWD) //PWD
    {
        char cwd[MAX_STR_LEN];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        }

        return 1;
    }
    else  if (argv->command == WHICH) //WHICH
    {
        if (argv->paths_size >= 2){
            
            int i;
            char fullPath[MAX_STR_LEN];

            for (i = 0; argv->env[i] != NULL; i++)
            {
                if (my_strncmp(argv->env[i], "PATH=", 5) == 0)
                {
                    char* copyOfLine = strdup(argv->env[i]);
                    char* ptr;
                    char* tok = strtok_r(copyOfLine, ":", &ptr);
                    while (tok != NULL)
                    {                
                        sprintf(fullPath, "%s/%s", tok, argv->paths[1]);

                        //check is program
                        if (access(fullPath, F_OK) == 0) {

                            printf("%s\n", fullPath);         
                        }

                        //nex token
                        tok = strtok_r(NULL, ":", &ptr);
                    }


                    free(copyOfLine);

                }
            }

        }

        return 1;
    }

    return 0; 
}