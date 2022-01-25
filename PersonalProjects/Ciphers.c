//
//  Ciphers.c
//  PersonalProjects
//
//  Created by Yiyun Wu on 5/22/21.
//

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

void MyGets(char *input, int MaxLength, FILE *fptr);
void Caesar(int choice);
void RomanNum(int choice);
void Morse(int choice);
void Morse2Plain(char in[]);
void Plain2Morse(char in[]);
void NumSub(int choice);
void InputTxt(int choice, char *in);
void Menu(int *x, int *choice);
int NumChar2Int(char s[]);
void ClearOutString (char s[]);
void MakeAllCaps (char Input[100]);
void Ascii(int choice);

//FIX ME
//index of coincidence
//frequency analysis
//morse linked list, scan in file

int main(void){
    int x, choice, cont = 1;
    
    do {
        Menu(&x, &choice);
        switch (x){
            case 1:
                Caesar(choice);
                break;
            case 2:
                RomanNum(choice);
                break;
            case 3:
                Morse(choice);
                break;
            case 4:
                NumSub(choice);
                break;
            case 5:
                Ascii(choice);
                break;
        }
        printf("\n---\nContinue? (Y=1, N=0): ");
        scanf("%d", &cont);
    } while (cont == 1);
    
    return 0;
}

void Menu(int *x, int *choice){
    printf("---\nCaesar - 1\nRoman numerals - 2\nMorse - 3\nNumber substitution - 4\nAscii - 5\n---\nChoice: ");
    scanf("%d", x);
    printf("Encoder - 1, Decoder - 2: ");
    scanf("%d", choice);
    printf("---\n");
}

void InputTxt(int choice, char *in){
    switch (choice) {
        case 1: //encoder
            printf("Enter plaintext: ");
            MyGets(in, 100, stdin);
            printf("Ciphertext: ");
            break;
        case 2: //decoder
            printf("Enter ciphertext: ");
            MyGets(in, 100, stdin);
            printf("Plaintext: ");
            break;
    }
}

void Ascii(int choice){
    char in[100], s[100];
    int i, k = 0, num;
    
    printf("ASCII\n");
    InputTxt(choice, in);
    
    switch (choice){
        case 1: //plain to ascii
            for (i=0; i<strlen(in);i++){
                printf("%d ",in[i]);
            }
            break;
        case 2: //ascii to plain
            while (k<strlen(in)){
                ClearOutString(s);
                i = 0;
                while (in[k] != ' ' && in[k] != '\0' && in[k] != '\n') {
                    s[i] = in[k];
                    i++;
                    k++;
                }
                s[i]='\0';
                k++;
                num = NumChar2Int(s);
                printf("%c", num);
            }
            break;
    }
}

void NumSub(int choice){
    char in[100], s[5];
    s[0] = '\0';
    int i = 0, num, k = 0;
    
    printf("NUMBER SUBSTITUTION\n");
    InputTxt(choice, in);
    
    switch (choice){
        case 1: //abc to num
            for (i=0;i<strlen(in);i++){
                if (in[i]>='a' && in[i]<='z'){
                    printf("%d ", in[i]-('a'-1));
                } else if (in[i]>='A' && in[i]<='Z') {
                    printf("%d ", in[i]-('A'-1));
                }
            }
            break;
        case 2: //num to abc
            while (k<strlen(in)){
                ClearOutString(s);
                i = 0;
                while (in[k] != ' ' && in[k] != '\0' && in[k] != '\n') {
                    s[i] = in[k];
                    i++;
                    k++;
                }
                s[i]='\0';
                k++;
                num = NumChar2Int(s);
                printf("%c", num+('A'-1));
            }
            break;
    }
}

void ClearOutString (char s[]){ //clear out a given string
    int i=0;
    while(s[i]!='\0'){
        s[i]='\0';
        i++;
    }
}

int NumChar2Int(char s[]){
    int sum = 0, i;
    for (i=0;i<strlen(s);i++){
        sum += pow(10,strlen(s)-i-1)*(s[i]-'0');
    }
    return sum;
}

void RomanNum(int choice){
    char in[100], out[100];
    int i, sum = 0;
    out[0] = '\0';
    
    printf("ROMAN NUMERALS\n");
    InputTxt(choice, in);
    
    switch (choice){
        case 1: //dec to roman
            sum = NumChar2Int(in);
            while (sum>=1000){
                strcat(out,"M");
                sum = sum - 1000;
            } while (sum>=500){
                if (sum/100 == 9){
                    strcat(out,"CM");
                    sum = sum - 900;
                } else{
                    strcat(out,"D");
                    sum = sum - 500;
                }
            } while (sum>=100){
                if (sum/100 == 4){
                    strcat(out,"CD");
                    sum = sum - 400;
                } else{
                    strcat(out,"C");
                    sum = sum - 100;
                }
            } while (sum>=50){
                if (sum/10 == 9){
                    strcat(out,"XC");
                    sum = sum - 90;
                } else{
                    strcat(out,"L");
                    sum = sum - 50;
                }
            } while (sum>=10){
                if (sum/10 == 4){
                    strcat(out,"XL");
                    sum = sum - 40;
                } else{
                    strcat(out,"X");
                    sum = sum - 10;
                }
            } while (sum>=5){
                if (sum == 9){
                    strcat(out,"IX");
                    sum = sum - 9;
                } else {
                    strcat(out,"V");
                    sum = sum - 5;
                }
            } while (sum>0){
                if (sum == 4){
                    strcat(out,"IV");
                    sum = sum - 4;
                } else {
                    strcat(out,"I");
                    sum = sum - 1;
                }
            }
            printf("%s", out);
            break;
        case 2: // roman to dec
            for (i=0;i<strlen(in);i++){
                if (in[i]=='M' || in[i]=='m'){
                    sum +=1000;
                } else if (in[i]=='D' || in[i]=='d') {
                    sum +=500;
                } else if (in[i]=='C' || in[i]=='c') {
                    if (in[i+1]=='D' || in[i+1]=='d' || in[i+1]=='M' || in[i+1]=='m'){
                        sum-=100;
                    } else {
                        sum +=100;
                    }
                } else if (in[i]=='L' || in[i]=='l') {
                    sum +=50;
                } else if (in[i]=='X' || in[i]=='x') {
                    if (in[i+1]=='L' || in[i+1]=='l' || in[i+1]=='C' || in[i+1]=='c'){
                        sum-=10;
                    } else {
                        sum +=10;
                    }
                } else if (in[i]=='V' || in[i]=='v') {
                    sum +=5;
                } else if (in[i]=='I' || in[i]=='i') {
                    if (in[i+1]=='X' || in[i+1]=='x' || in[i+1]=='V' || in[i+1]=='v'){
                        sum-=1;
                    } else {
                        sum +=1;
                    }
                }
            }
            printf("%d", sum);
            break;
    }
}

void Morse(int choice){
    char in[100];
    
    printf("MORSE CODE\n");
    InputTxt(choice, in);
    MakeAllCaps(in);
    switch (choice){
        case 1: //plain to morse
            Plain2Morse(in);
            break;
        case 2:
            Morse2Plain(in);
            break;
    }
}

void Caesar(int choice){
    char in[100], outtext[100];
    int rot = 0, i;
    
    printf("CAESAR CIPHER\n");
    printf("Rot: ");
    scanf("%d", &rot);
    if (choice == 2) {
        rot = 26 - rot;
    }
    InputTxt(choice, in);
    
    for(i=0;i<strlen(in);i++){ //print "out text"
        if (in[i]>='a' && in[i]<='z') { //lowercase
            if (in[i]+rot<='z'){
                outtext[i] = in[i]+rot;
            } else {
                outtext[i] = in[i]+rot-26;
            }
        } else if (in[i]>='A' && in[i]<='Z'){ //uppercase
            if (in[i]+rot<='Z'){
                outtext[i] = in[i]+rot;
            } else {
                outtext[i] = in[i]+rot-26;
            }
        } else { // space, symbols, punctuation
            outtext[i] = in[i];
        }
        printf("%c",outtext[i]);
    }
    return;
}

void MyGets(char *input, int MaxLength, FILE *fptr){
    do{
    fgets(input, MaxLength, fptr);
    } while(input[0]=='\n');
    while (input[strlen(input)-1]=='\n'){
        input[strlen(input)-1]='\0';
    }
    while (input[strlen(input)-1]=='\r'){ //get rid of carriage return
        input[strlen(input)-1]='\0';
    }
}

void MakeAllCaps (char Input[100]){
    int i;
    for (i=0;i<strlen(Input); i++){
        if(Input[i]>='a'){
            Input[i]-=('a'-'A');
        }
    }
}

void Morse2Plain(char in[]){
    int i , k = 0;
    char s[100];
    
    while (k<strlen(in)){
        ClearOutString(s);
        i = 0;
        while (in[k] != ' ' && in[k] != '\0' && in[k] != '\n') {
            s[i] = in[k];
            i++;
            k++;
        }
        s[i]='\0';
        k++;
        if (strcmp(".-",s) == 0){
            printf("A");
        } else if (strcmp("-...",s) == 0){
            printf("B");
        } else if (strcmp("-.-.",s) == 0){
            printf("C");
        } else if (strcmp("-..",s) == 0){
            printf("D");
        } else if (strcmp(".",s) == 0){
            printf("E");
        } else if (strcmp("..-.",s) == 0){
            printf("F");
        } else if (strcmp("--.",s) == 0){
            printf("G");
        } else if (strcmp("....",s) == 0){
            printf("H");
        } else if (strcmp("..",s) == 0){
            printf("I");
        } else if (strcmp(".---",s) == 0){
            printf("J");
        } else if (strcmp("-.-",s) == 0){
            printf("K");
        } else if (strcmp(".-..",s) == 0){
            printf("L");
        } else if (strcmp("--",s) == 0){
            printf("M");
        } else if (strcmp("-.",s) == 0){
            printf("N");
        } else if (strcmp("---",s) == 0){
            printf("O");
        } else if (strcmp(".--.",s) == 0){
            printf("P");
        } else if (strcmp("--.-",s) == 0){
            printf("Q");
        } else if (strcmp(".-.",s) == 0){
            printf("R");
        } else if (strcmp("...",s) == 0){
            printf("S");
        } else if (strcmp("-",s) == 0){
            printf("T");
        } else if (strcmp("..-",s) == 0){
            printf("U");
        } else if (strcmp("...-",s) == 0){
            printf("V");
        } else if (strcmp(".--",s) == 0){
            printf("W");
        } else if (strcmp("-..-",s) == 0){
            printf("X");
        } else if (strcmp("-.--",s) == 0){
            printf("Y");
        } else if (strcmp("--..",s) == 0){
            printf("Z");
        } else if (strcmp(".----",s) == 0){
            printf("1");
        } else if (strcmp("..---",s) == 0){
            printf("2");
        } else if (strcmp("...--",s) == 0){
            printf("3");
        } else if (strcmp("....-",s) == 0){
            printf("4");
        } else if (strcmp(".....",s) == 0){
            printf("5");
        } else if (strcmp("-....",s) == 0){
            printf("6");
        } else if (strcmp("--...",s) == 0){
            printf("7");
        } else if (strcmp("---..",s) == 0){
            printf("8");
        } else if (strcmp("----.",s) == 0){
            printf("9");
        } else if (strcmp("-----",s) == 0){
            printf("0");
        } else if (strcmp(".-.-.-",s) == 0){
            printf(".");
        } else if (strcmp("--..--",s) == 0){
            printf(",");
        } else if (strcmp("..--..",s) == 0){
            printf("?");
        } else {
            printf("%s", s);
        }
    }
}

void Plain2Morse(char in[]){
    int i;
    for (i=0;i<strlen(in);i++){
        if (in[i]=='a' || in[i]=='A'){
            printf(".- ");
        } else if (in[i]=='b' || in[i]=='B'){
            printf("-... ");
        } else if (in[i]=='c' || in[i]=='C'){
            printf("-.-. ");
        } else if (in[i]=='d' || in[i]=='D'){
            printf("-.. ");
        } else if (in[i]=='e' || in[i]=='E'){
            printf(". ");
        } else if (in[i]=='f' || in[i]=='F'){
            printf("..-. ");
        } else if (in[i]=='g' || in[i]=='G'){
            printf("--. ");
        } else if (in[i]=='h' || in[i]=='H'){
            printf(".... ");
        } else if (in[i]=='i' || in[i]=='I'){
            printf(".. ");
        } else if (in[i]=='j' || in[i]=='J'){
            printf(".--- ");
        } else if (in[i]=='k' || in[i]=='K'){
            printf("-.- ");
        } else if (in[i]=='l' || in[i]=='L'){
            printf(".-.. ");
        } else if (in[i]=='m' || in[i]=='M'){
            printf("-- ");
        } else if (in[i]=='n' || in[i]=='N'){
            printf("-. ");
        } else if (in[i]=='o' || in[i]=='O'){
            printf("--- ");
        } else if (in[i]=='p' || in[i]=='P'){
            printf(".--. ");
        } else if (in[i]=='q' || in[i]=='Q'){
            printf("--.- ");
        } else if (in[i]=='r' || in[i]=='R'){
            printf(".-. ");
        } else if (in[i]=='s' || in[i]=='S'){
            printf("... ");
        } else if (in[i]=='t' || in[i]=='T'){
            printf("- ");
        } else if (in[i]=='u' || in[i]=='U'){
            printf("..- ");
        } else if (in[i]=='v' || in[i]=='V'){
            printf("...- ");
        } else if (in[i]=='w' || in[i]=='W'){
            printf(".-- ");
        } else if (in[i]=='x' || in[i]=='X'){
            printf("-..- ");
        } else if (in[i]=='y' || in[i]=='Y'){
            printf("-.-- ");
        } else if (in[i]=='z' || in[i]=='Z'){
            printf("--.. ");
        } else if (in[i]=='1'){
            printf(".---- ");
        } else if (in[i]=='2'){
            printf("..--- ");
        } else if (in[i]=='3'){
            printf("...-- ");
        } else if (in[i]=='4'){
            printf("....- ");
        } else if (in[i]=='5'){
            printf("..... ");
        } else if (in[i]=='6'){
            printf("-.... ");
        } else if (in[i]=='7'){
            printf("--... ");
        } else if (in[i]=='8'){
            printf("---.. ");
        } else if (in[i]=='9'){
            printf("----. ");
        } else if (in[i]=='0'){
            printf("----- ");
        } else if (in[i]=='.'){
            printf(".-.-.- ");
        } else if (in[i]==','){
            printf("--..-- ");
        } else if (in[i]=='?'){
            printf("..--.. ");
        } else {
            printf("%c", in[i]);
        }
    }
}
