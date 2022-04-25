#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

static int readlinefun(int count, int key)
{
    printf("count=%d,key=%c\n",count,key);
}

int main(int argc,char **argv)
{
    rl_bind_key('?',readlinefun);
    while(1)
    {
        char *pget = readline("you need input data is :");
        if(!strcmp(pget,"get_1"))
        {
            printf("get_1 is %d\n",1);
        }
        else if(!strcmp(pget,"history"))
        {
            
            free(pget);
        }
        else if(!strcmp(pget,"exit"))
        {
            free(pget);
            break;
        }
        else 
        {
            printf("input fault ,need input new data\n");
        }
        add_history(pget);
        printf("data is %s,history len is %d\n",pget,history_length);
        free(pget);
    }
    // rl_bind_key('?',readlinefun);
    // while(1)
    // {
    //     sleep(10);
    // }
    return 0;
}