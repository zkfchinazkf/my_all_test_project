#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

static int readlinefun(int count, int key)
{
    printf("count=%d,key=%c\n",count,key);
    printf("rl_line_buffer=%s,rl_point=%d\n",rl_line_buffer,rl_point);

}

int main(int argc,char **argv)
{
    // rl_set_keyboard_input_timeout(1000 * 10); 
    // int inp=rl_read_key();
    // printf("input is %d\n",inp);

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
            HIST_ENTRY **the_list;
            the_list = history_list();
            if(the_list)
            {
                for(int idx=0;the_list[idx];idx++)
                {
                    printf("%d: %s\r\n",idx + history_base,the_list[idx]->line);
                }
            }
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
    
    return 0;
}