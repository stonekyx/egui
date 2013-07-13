#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static void trim(char *line)
{
    int len = strlen(line);
    while(len && isspace(line[len-1])) len--;
    line[len]=0;
}

const char *libs[] = {"widget", "application", "server_lib", "graph", "client_lib", "geometry", "event", "comm", "config_parser", "data_structures", "color"};
int app[20];
const int std[]={0,1,0,2,3,4,5,6,7,8,9,10};

const int depends[11][4]={
    {1,5,8,9},
    {0,4,7,9},
    {3,5,7},
    {5,10},
    {7},
    {},
    {7,9},
    {9},
    {9},
    {},
    {},
};
const int depcnt[11]={4,4,3,2,1,0,2,1,1,0,0};

void mark_depend(int x)
{
    int i;
    if(app[x]>0) return;
    app[x]++;
    for(i=0; i<depcnt[x]; i++) {
        mark_depend(depends[x][i]);
    }
}

void record_app(char *token)
{
    int i;
    if(token==NULL) {
        memset(app, 0, sizeof(app));
        return;
    }
    for(i=0; i<sizeof(libs)/sizeof(char*); i++) {
        if(!strcmp(token, libs[i])) {
            mark_depend(i);
        }
    }
}

void flush_records(char *new)
{
    int i;
    for(i=0; i<sizeof(std)/sizeof(int); i++) {
        if(app[std[i]]) {
            strcat(new, " -l");
            strcat(new, libs[std[i]]);
        }
    }
    record_app(NULL);
}

void process_file(FILE *fin, FILE *fout)
{
    static char line[100000], bkp[100000], new[100000];
    while(fgets(line, 100000, fin)) {
        char *token;
        char *nonspace=line;
        trim(line);
        if(line[0]==0) {
            fprintf(fout, "\n");
            continue;
        }
        strcpy(bkp, line);
        new[0]=0;
        while(isspace(*nonspace)) {
            strncat(new, nonspace++, 1);
        }
        token = strtok(nonspace, " \n\t");
        strcat(new, token);
        record_app(NULL);
        while((token=strtok(NULL, " \n\t"))) {
            if(!strncmp(token, "-l", 2)) {
                if(!strcmp(token, "-lpthread")) {
                    strcat(new, " ");
                    strcat(new, token);
                } else {
                    record_app(token+2);
                }
            } else {
                flush_records(new);
                strcat(new, " ");
                strcat(new, token);
            }
        }
        flush_records(new);
        /*puts(bkp);
        puts(new);
        fgets(line, 10, stdin);*/
        fprintf(fout, "%s\n", new);
    }
}

void copy_file(FILE *fout, char *tar)
{
    int ch;
    rewind(fout);
    while((ch=fgetc(fout))!=EOF) {
        fputc(ch, stdout);
    }
}

int main(int argc, char *argv[])
{
    FILE *fin, *fout;
    if(argc!=2) {
        return 1;
    }
    fin = fopen(argv[1], "r");
    fout = tmpfile();
    process_file(fin, fout);
    copy_file(fout, argv[1]);
    fclose(fin);
    fclose(fout);
    return 0;
}
