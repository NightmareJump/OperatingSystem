#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define MAX_SQUENCE_LEN 20
#define MAX_FRAME_SIZE 5

struct Block
{
    int page;
    int order;
}block[MAX_FRAME_SIZE];
int array[MAX_SQUENCE_LEN];
void FIFO(int page_num,int block_num)
{
    int i,j;
    int page_fault=0;
    FILE *fout=fopen("Output.txt","w");
    if(fout==NULL)
    {
        printf("file open failed");
        exit(-1);
    }
    for(i=0;i<page_num;i++)
    {
        int find=0;
        for(j=0;j<block_num;j++)
        {
            if(array[i]==block[j].page)
            {
                find=1;
                break;
            }
        }
        if(!find)
        {
            int index=-1;
            for(j=0;j<block_num;j++)
            {
                if(block[j].order==block_num||block[j].page==-1)
                {
                    index=j;
                    break;
                }
            }
            block[index].page=array[i];
            block[index].order=1;
            for(j=0;j<block_num;j++)
            {
                if(j==index||block[j].page==-1)
                    continue;
                block[j].order+=1;
            }
            page_fault++;
        }
        for(j=0;j<block_num;j++)
        {
            if(block[j].page!=-1)
                fprintf(fout,"%d ",block[j].page);
            else
                fprintf(fout,"N ");
        }
        fprintf(fout,"\n");
    }
    fprintf(fout,"Number of Page Faults:%d\n",page_fault);
    fclose(fout);
}


int main()
{
    /* read data */
    FILE *fin=fopen("Input.txt","r");
    char str[MAX_SQUENCE_LEN*3];
    int i,block_num,page_num=0;
    if(fin==NULL)
    {
        printf("file open failed");
        exit(-1);
    }
    fscanf(fin,"%d",&block_num);
    fgetc(fin);
    //fgetc(fin);
    fgets(str,MAX_SQUENCE_LEN*3,fin);
    char *token;
    token=strtok(str,",");
    while(token!=NULL)
    {
        array[page_num++]=atoi(token);
        token=strtok(NULL,",");
    }
    fclose(fin);

    /* init block */
    for(i=0;i<block_num;i++)
    {
        block[i].page=-1;
        block[i].order=0;
    }

    FIFO(page_num,block_num);
}
