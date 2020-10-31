#include<stdio.h>
#include<string.h>
int main()
{
    FILE* fp=fopen("./Demo","a+");
    if(!fp)
    {
        perror("fopen");
        return -1;
    }

    char buf[1024]={0};
    strcpy(buf,"Linux");
    int w_ret=fwrite(buf,1,5,fp);
    printf("w_ret:%d\n",w_ret);
    fseek(fp,0,SEEK_SET);
    memset(buf,0,sizeof(buf));
    int r_ret=fread(buf,1,sizeof(buf)-1,fp);
    printf("r_ret:%d\n",r_ret);
    printf("buf:%s\n",buf);
    fclose(fp);
    return 0;
}
