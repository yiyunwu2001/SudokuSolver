//
//  ECE175_HW8_V1.c
//  PersonalProjects
//
//  Created by Yiyun Wu on 5/26/21.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct FontArt_s{
    char LetterBox[15][20];
    int NRows, NCols;
    struct FontArt_s *next;
}FontArt;

void FontMenu(char FName[]);
void OpenFile(char FName[], FILE **inp);
void LoadFont(char FName[], FontArt **HN, FILE *fptr);
void EnterString(char PrintString[]);
void PrintOut(FontArt *HN, char PrintString[]);
int Continue(void);
void MyGets(char *input, int MaxLength, FILE *fptr);
int CheckFontName(char input[]);
char ScanCharIgnore (char IgnoreChar);
void MakeAllCaps(char s[]);
int CheckBlankLine(char line[]);
void ClearOutString (char s[]);
void DeleteSymbols(char s[]);
void AddNull(int numNull, int onwards, char s[]);
FontArt* GetNthNode (FontArt *HN, int N);
int CountListNodes(FontArt *HN);

int main (void){
    char FName[50],PrintString[50];
    FontArt *LoadedFont;
    FILE *inp;
    
    do{
        FontMenu(FName);
        OpenFile(FName, &inp);
        LoadFont(FName, &LoadedFont, inp);
        EnterString(PrintString);
        PrintOut(LoadedFont, PrintString);
    } while (Continue()==1);
    
    return 0;
}

void LoadFont(char FName[], FontArt **HN, FILE *fptr){
    char myChar2;
    int numRows;
    int i, j, k;
    FontArt *tmp;
    FILE *inp = fptr;
    
    *HN = (FontArt*)calloc(sizeof(FontArt), 1);
    tmp = *HN;
    MyGets(&myChar2, 20, inp);
    numRows = myChar2 - '0';
    for (i=0;i<36;i++){
        tmp->NCols = 0;
        tmp->NRows = numRows;
        for (j=0;j<numRows;j++){
            MyGets(tmp->LetterBox[j], 20, inp);
            if (tmp->NCols<(int)strlen(tmp->LetterBox[j])){ //find widest part of letter
                tmp->NCols = (int)strlen(tmp->LetterBox[j]);
            }
        }
        while (CheckBlankLine(tmp->LetterBox[numRows-1])==0){
            for (k=0;k<numRows-1;k++){
                strcpy(tmp->LetterBox[numRows-k-1], tmp->LetterBox[numRows-2-k]);
                ClearOutString(tmp->LetterBox[numRows-2-k]);
            }
        }
        if (i==35){
            tmp->next = NULL;
        } else{
            tmp->next = (FontArt*)calloc(sizeof(FontArt), 1);
            tmp = tmp->next;
        }
    }
    fclose(inp);
}

void OpenFile(char FName[], FILE **inp){
    if (strcmp(FName,"FIRE")==0){
        *inp = fopen("FireFont.txt", "r");
    } else if (strcmp(FName,"DANCING")==0) {
        *inp = fopen("DancingFont.txt", "r");
    } else if (strcmp(FName,"BULBHEAD")==0) {
        *inp = fopen("BulbHeadFont.txt", "r");
    } else if (strcmp(FName,"GHOST")==0) {
        *inp = fopen("GhostFont.txt", "r");
    } else if (strcmp(FName,"CHISELED")==0) {
        *inp = fopen("ChiseledFont.txt", "r");
    } else {
        *inp = NULL;
    }
}

FontArt* GetNthNode (FontArt *HN, int N){
    int i;
    if (N<1 || N> CountListNodes(HN)){ //error checking
        return NULL;
    }
    for (i=1; i<N; i++){
        HN = HN->next;
    }
    return(HN);
}

int CountListNodes(FontArt *HN){ //counts number of nodes in linked list
    int i = 0;
    while (HN != NULL){
        i++;
        HN = HN->next;
    }
    return i;
}

void PrintOut(FontArt *HN, char PrintString[]){
    int totalCols = 0, lastTotalCols = 0, numColsOut = 0, saveLetter = 0;
    int i, j, k, boxnum;
    char PrintFont[15][300];
    
    for (i=0;i<strlen(PrintString);i++){
        if (PrintString[i]>='A' && PrintString[i]<='Z'){
            boxnum = PrintString[i]-'A';
        } else if (PrintString[i]>='0' && PrintString[i]<='9'){
            boxnum = PrintString[i]-'0'+26;
        } else {
            boxnum = 37;
            lastTotalCols = totalCols;
            totalCols+=5;
        }
        if (boxnum<37){ //if character is letter or number
            lastTotalCols = totalCols;
            totalCols += GetNthNode(HN,boxnum+1)->NCols;
            if (GetNthNode(HN,boxnum+1)->NCols>numColsOut){ //find widest letter
                numColsOut = GetNthNode(HN,boxnum+1)->NCols;
                saveLetter = PrintString[i];
            }
            for (j=0;j<HN->NRows;j++){
                if (CheckBlankLine(GetNthNode(HN,boxnum+1)->LetterBox[j])!=0){ //if we don't meet blank line
                    AddNull(GetNthNode(HN,boxnum+1)->NCols, lastTotalCols, PrintFont[j]);
                    strcat(PrintFont[j], GetNthNode(HN,boxnum+1)->LetterBox[j]);
                    while (strlen(PrintFont[j])<totalCols){
                        PrintFont[j][strlen(PrintFont[j])] = ' ';
                    }
                    PrintFont[j][totalCols] = '\0';
                } else{ //if we meet blank line
                    for (k=lastTotalCols;k<totalCols;k++){
                        PrintFont[j][k] = ' ';
                    }
                    PrintFont[j][totalCols] = '\0';
                }
            }
        } else { //if character is space
                for (j=0;j<HN->NRows;j++){
                    AddNull(5, lastTotalCols, PrintFont[j]);
                    strcat(PrintFont[j], "     ");
                    PrintFont[j][totalCols] = '\0';
                }
            }
        }
    
    printf("\n\n");
    for (i=0;i<HN->NRows;i++){
        printf("%s\n", PrintFont[i]);
    }
    printf("\n\n");
    printf("These letters have %d rows.\n", HN->NRows);
    printf("The letter with the most columns was %c with %d columns\n\n", saveLetter, numColsOut);
}

int Continue(void){
    int cont = 1;
    char myChar;
    
    printf("Would you like to continue (Y/N)? ");
    myChar = ScanCharIgnore('\n');
    printf("\n");
    if (myChar == 'N' || myChar == 'n'){
        cont = 0;
        printf("Goodbye\n");
    }
    
    return cont; // cont = 0 means stop
}

void EnterString(char PrintString[]){
    printf("Enter a string to print out: ");
    MyGets(PrintString,50,stdin);
    MakeAllCaps(PrintString);
    DeleteSymbols(PrintString);
}

void FontMenu(char FName[]){
    int error;
    do{
        printf("What font do you want to use?\n\tFire\n\tDancing\n\tBulbHead\n\tGhost\n\tChiseled\n");
        MyGets(FName, 50, stdin);
        MakeAllCaps(FName);
        error = CheckFontName(FName);
        if (error == 1){
            printf("%s is not a valid choice\n\n", FName);
        }
    } while (error == 1);
}

void DeleteSymbols(char s[]){
    int i, k=0;
    char tmp[100];
    strcpy(tmp, s);
    ClearOutString(s);
    for (i=0;i<strlen(tmp);i++){
        if ((tmp[i]>='A' && tmp[i]<='Z') || (tmp[i]>='0' && tmp[i]<='9') || tmp[i]==' '){
            s[k] = tmp[i];
            k+=1;
        }
    }
}

void AddNull(int numNull, int onwards, char s[]){
    int i;
    for(i=onwards;i<onwards+numNull;i++){
        s[i]='\0';
    }
}

int CheckBlankLine(char line[]){
    int blank = 0, i;
    
    if (line[0] == '\0'){
        blank = 0;
    }
    for (i = 0; i<strlen(line);i++){
        if(line[i]!=' ' && line[i]!='\n'){
            blank = 1;
            break;
        }
    }
    return blank; //0 means blank, 1 means no
}

void ClearOutString (char s[]){ //clear out a given string
    int i=0;
    while (s[i]!='\0'){
        s[i]='\0';
        i++;
    }
}

void MakeAllCaps(char s[]){
    int i;
    
    for (i=0;i<strlen(s); i++){
          if(s[i]>='a' && s[i]<='z'){
              s[i]-=('a'-'A');
          }
      }
}

int CheckFontName(char input[]){
    int error = 1, i;
    char tempString[50];
    strcpy(tempString,input);
    for (i=0;i<strlen(input); i++){ //Make all lower case letters upper case
        if(input[i]>='a'){
            input[i]-=('a'-'A');
        }
    }
    if (strcmp(input,"FIRE")==0){
        error = 0;
    } else if (strcmp(input,"DANCING")==0) {
        error = 0;
    } else if (strcmp(input,"BULBHEAD")==0) {
        error = 0;
    } else if (strcmp(input,"GHOST")==0) {
        error = 0;
    } else if (strcmp(input,"CHISELED")==0) {
        error = 0;
    } else {
        error = 1;
    }
    strcpy(input,tempString);
    return error;
}

void MyGets(char *input, int MaxLength, FILE *fptr){
    do{
    fgets(input, MaxLength, fptr);
    } while(input[0]=='\n');
    while (input[strlen(input)-1]=='\n' || input[strlen(input)-1]=='\r'){
        input[strlen(input)-1]='\0';
    }
}

char ScanCharIgnore (char IgnoreChar){ //ignores specified char e.g. '/n' when scanning
    char MyChar;
    do{
        scanf("%c", &MyChar);
    } while (MyChar == IgnoreChar);
    if(MyChar>='a'){ //if user enters in lowercase letter, change to upper case
        MyChar-=('a'-'A');
    }
    return MyChar;
}

