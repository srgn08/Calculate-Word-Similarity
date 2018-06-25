#include <math.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void add_item(char input[20], int count,int freq_control);
int control_word(char word[20],int freq_control);
void create_matrix(int **matrix, int row, int column);
void read_file(char* filename, int count,int control);
void print(int **matrix,int count, char **str, int column);

typedef struct node
{
    char word[20]; /*word name*/
    int frequence; /*total frequence all file*/
    struct node* next; /* point next word*/
    int freq_file[500]; /*This array keeps the number of the frequence in alphabetical order of the files.*/

};
struct node *rear;
struct node *front;

int main() {
    DIR *dir;
    struct dirent *entry;
    char path[1025];
    int len;
    int count=0;
    int control=0;
    char temp[100];
    int i,j,m;

char **str = (char**) calloc(100, sizeof(char*)); /*str keeps all file name alphabetically*/
for ( m = 0; m < 100; m++ )
{
    str[m] = (char*) calloc(100, sizeof(char));
}

    if ((dir = opendir("./")) == NULL)
        perror("opendir() error");
    else {
        while ((entry = readdir(dir)) != NULL) {  /*read all files and decide to number of files*/
            strcat(path, entry->d_name);
            len = strlen(path);
            if (path[len - 4] == '.' && path[len - 3] == 't' && path[len - 2] == 'x' && path[len - 1] == 't')
            {
                str[count]= malloc(sizeof(char)*strlen(entry->d_name));
		strcpy(str[count],entry->d_name);
                count++; /*keep number of files*/
            }

        }
        closedir(dir);
    }

    for (i = 0; i < count; i++) {    /*Loop that sorts file names*/
        for (j = 0; j < count - 1; j++) {
            if (strcmp(str[j], str[j + 1]) > 0) {
                strcpy(temp, str[j]);
                strcpy(str[j], str[j + 1]);
                strcpy(str[j + 1], temp);
            }
        }
    }

    int z=0;
    if ((dir = opendir("./")) == NULL)
        perror("opendir() error");
    else {
        while (z<count) {

                read_file(str[z],count,z); /*call read file function*/
                control++;
            z++;
            }
        }
        closedir(dir);
		
    int valid_frequence=0;
    struct node *temp2;
    temp2=front;
    while(temp2!=NULL)
    {
        if((temp2->frequence)<10 && temp2->frequence>=count)  /*this loop found all files for 10>frequence>=nunber of files*/
        {
            valid_frequence++;
        }

        temp2=temp2->next;

    }
    int **matrix;   /*matrix allocation*/
    int t=0;
    matrix=(int**)calloc(count,sizeof(int*));

    for (t=0; t<count; t++)
    {
        matrix[t]=(int*)calloc(valid_frequence,sizeof(int));

    }
	
    create_matrix(matrix,count,valid_frequence);
    print(matrix,count,str,valid_frequence);
    free(matrix);
	free(str);
    return 0;
}

/*
filename is the name of the file to read.
count is number of read file numbers.
control is keep filename is how index is array.
*/
void read_file(char* filename, int count,int control) /* this function read file and find all word*/
{
    FILE * fp;
    fp = fopen(filename, "r");
    char token[1024];
    while (fgets(token, 1024, fp)){
        char file[1024];
        strcpy(file,token);
        int i=0;
        char temp[100]="";
        int flag=0;
        char *point_word=token;
        int j;

        while(i<strlen(file))
        {
            if( isalpha(file[i])) /*charachter or not control*/
            {
                if(flag==0)
                    point_word=token+i;
                flag++;
            }
            else
            {
                strncat(temp,point_word,flag);
                if(flag>=1) /*if flag equals is 1 , so word hasn't been.*/
                {
                    for(j=0;j<strlen(temp);j++)
                        temp[j] = tolower(temp[j]);
                    add_item(temp,count,control);  /*call add function*/

                }
                point_word=token+i;
                flag=0;
                strcpy(temp,"");
            }
            i++;
        }
    }
}
/*
input is name of word.
count is number of files.
freq control is keep filename index
*/

void add_item(char input[20], int count,int freq_control) /*add item function*/
{
    int control=control_word(input,freq_control);
    if(control==0)
    {
        struct node *temp;
        temp = (struct node*)malloc(sizeof(struct node));
        temp->frequence=1;
        temp->next=NULL;
        strcpy(temp->word,input);
        int x;
        for(x=0; x<count;x++)
            temp->freq_file[x]=0;
        temp->freq_file[freq_control]++;

        if (rear  ==  NULL)  /*    queue is empty*/
        {
            front =temp;
            rear = temp;
        }
        else
        {
            rear->next = temp;
            rear = temp;
        }
    }
}
/*
word is reading word.
freq control is keep filename index.
*/
int control_word(char word[20],int freq_control)
{
    struct node *temp;
    temp=front;
    int control=0;
    while(temp!=NULL)
    {
        if(strcmp(temp->word,word)==0)  /*word has already been or not control*/
        {
            temp->frequence++;
            control=1;
            temp->freq_file[freq_control]++;
        }
        temp=temp->next;
    }
    return control;
}

/*
matrix is keep valid word.
row is matrix row number.
column is matrix column number.
*/
void create_matrix(int **matrix, int row, int column)
{
    int l=0,k,m=0;
    while(front!=NULL)
    {
        if((front->frequence)<10 && front->frequence>=row)
        {
                for(k=0;k<row;k++)
                {
                    matrix[k][l]=front->freq_file[m];
                    m++;
                }
            l++;
            m=0;
        }
        front=front->next;
    }
}
/*
matrix is keep valid word.
count is number of files.
str is keep filenames.
column is total column number of matrix.
*/
void print(int **matrix,int count, char **str, int column)
{
    int i,j,k;
    double result=0;
    double result2=0;
    double result3=0;
    char file1[100];
    char file2[100];
    char file3[100];
    char file4[100];
    for(i=0;i<count;i++)
    {
        for(j=i+1;j<count;j++)
        {
            for(k=0;k<column;k++)
            {
                result+=matrix[i][k]*matrix[j][k];
                result2+=matrix[i][k]*matrix[i][k];
                result3+=matrix[j][k]*matrix[j][k];
            }
	    strcpy(file1,str[i]);
	    strcpy(file2,str[j]);
	    memset(file3, '\0', sizeof(file3));
	    strncpy(file3,file1,strlen(file1)-4);
	    memset(file4, '\0', sizeof(file4));
	    strncpy(file4,file2,strlen(file2)-4);

		if(result2==0 || result3==0)
			printf("Cos Sim of %s and %s = %.3lf\n",file3, file4,0.000);
		else
            		printf("Cos Sim of %s and %s = %.3lf\n",file3, file4,result/(sqrt(result2)*sqrt(result3)));
	    result=0;
	    result2=0;
	    result3=0;
            
        }
    }
}
