//
//  main.c
//  charctrl
//
//  Created by mm012 on 2014/03/26.
//  Copyright (c) 2014年 mm012. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#define MAXLINE 4000
//MAXLINE-1まで出力可能。
/**ファイル読み込み*/
int readFile(char *filepath);
/**標準入力*/
void strStdInput(char line[],int maxline);
/**文字列の探索及び置換*/
void searchAndReplace(char line[],char ptn[],char spat[],bool opt_r);
/**ファイル入力による処理*/
void fileInput(char line[],char ptn[],char spat[],char filepath[],int lim);
/**文字列の長さを測定*/
int length(char s[]);
/**オプション別の機能を実行*/
void runOption(bool opt_c,bool opt_r,bool opt_f,char line[],char ptn[],char spat[],char filepath[]);


int count;//探索文字の累計ヒット件数
int overchar = 0;
int overreplace = 0;
FILE *fp;

int main(int argc, const char * argv[])
{
    int i;
    char opt;
    bool opt_c = false;
    bool opt_r = false;
    bool opt_f = false;
    char *spat = NULL;
    char line[MAXLINE] = "";
    char *ptn = NULL;
    char *filepath = NULL;
    
    for(i = 0; i < argc; ++i)
    {
        //  printf("argv[%d]=%s\n", i, argv[i]);
        if(*argv[i] == '-')
        {
            opt = *(argv[i]+1);
            if(opt == 'c')
            {
                //カウントするコマンドプション
                ptn = argv[i+1];
                opt_c = true;
            }
            else if(opt == 'r')
            {
                //置換処理するコマンドオプション
                spat = argv[i+1];
                opt_r = true;
            }
            else if(opt == 'f')
            {
                //ファイル読み込みのコマンドオプション
                opt_f = true;
                filepath = argv[i+1];
            }
        }
    }
    runOption(opt_c, opt_r, opt_f, line, ptn, spat,filepath);
    return 0;
}


int readFile(char *filepath)
{
    const char *fname = filepath;
    fp = fopen( fname, "r" );
    
    if( fp == NULL )
    {
        if (filepath != NULL)
        {
            printf( "ファイルが開けません\n");
        }
        return -1;
    }
    return 1;
}

void strStdInput(char line[],int lim)
{
    printf("文字列を入力してください\n");
    int c,i;
    for (i = 0 ; (c = getchar()) != EOF && i < lim -1 ; i++)
    {
        line[i] = c;
    }
    if (i == lim -1&&c != EOF)
    {
        overchar = 1;
    }
    line[i] = '\0';
}

//文字列１行カウント関数
int length(char s[])
{
    int i;
    i = 0;
    while (s[i] != '\0' || s[i] != NULL)
	{
        ++i;
    }
    return i;
}

void searchAndReplace(char line[],char ptn[],char spat[],bool opt_r)
{
    int j,k = 0;
    j=0;
    int hit= 0;
    int ptn_length = length(ptn);
    char temp[MAXLINE] = "";
    for (int i = 0; i < length(line) ; )
    {
        while (line[i+j] == *(ptn+j))
        {
            j++;
            if(j == ptn_length)
            {
                hit++;
                if(opt_r == true)
                {
                    //許容文字の範囲内
                    if(length(line)-ptn_length+length(spat) < MAXLINE)
                    {
                        for(k = 0 ;k <length(line)-i-j; k++)
                        {
                            temp[k] =  line[i+j+k];
                        }
                        temp[k] = '\0';
                        for (k = 0;k<length(spat); k++)
                        {
                            line[i+k] = *(spat+k);
                        }
                        for(k = 0;k<=length(temp); k++)
                        {
                            line[i+length(spat)+k] = temp[k];
                        }
                    }
                    else
                    {
                        overreplace = 1;
                    }
                }
            }
        }
        j = 0;
        i++;
    }
    count += hit;
}

void inputFile(bool opt_r,char line[],char ptn[],char spat[],char filepath[],int lim)
{
    int c;
    int i = 0 ;
    printf("ファイルパス: %s \n",filepath);
    if(readFile(filepath)>0)
    {
       
        for (i = 0 ; (c = fgetc(fp)) != EOF && i < lim -1 ; i++)
        {
            line[i] = c;
        if (i == lim -1&& c != EOF)
        {
            overchar = 1;
        }

        }
        line[i+1] = '\0';
        if(length(line) == lim -1)
        {
            printf("\n---出力---\n");
            line[lim] = '\0';
            overchar = 1;
        }
            searchAndReplace(line, ptn, spat, opt_r);
            printf("%s",line); 
    }
    
}

void runOption(bool opt_c,bool opt_r,bool opt_f,char line[],char ptn[],char spat[],char filepath[])
{
    if(opt_c == false)
    {
        puts("CHARCTRL REQUIRES OPTION -c");
        exit(1);
    }
    //オプション-fが有効。
    if (opt_f == true)//ファイル入力による処理
    {
        inputFile(opt_r,line,ptn,spat,filepath,MAXLINE);
    }
    else//以下標準入力
    {
        strStdInput(line,MAXLINE);
        printf("\n---出力---\n");
        searchAndReplace(line, ptn,  spat, opt_r);
        printf("%s",line);
    }
    printf("\n該当文字列は%d個ありました。\n",count);
    
    if(overchar == 1)
    {
        printf("入力が許容文字数を越えました\n");
    }
    if(overreplace == 1)
    {
        printf("許容文字数を越える文字は、置換できません\n");
    }
    // ファイルを閉じる。
    if (opt_f == true)
    {
        fclose( fp );
    }
}