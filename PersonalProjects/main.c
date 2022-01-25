//
//  main.c
//  PersonalProjects
//
//  Created by Yiyun Wu on 5/15/21.
//

//[0] H position (col)
//[1] V position (row)
//[2] box number
//1 2 3
//4 5 6
//7 8 9

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct node_s{
    int N;
    int pos[9];
    int loc[3]; //loc[0] is H position 1-9, loc[1] V position 1-9, loc[2] is box 1-9
    struct node_s *right;
    struct node_s *down;
    struct node_s *boxNext;
} numNode;

typedef struct rownode_s {
    numNode *aRow;
    struct rownode_s *next;
} rowNode;

void CreateFile(void);

void CreateRowLinkedList(numNode **HN, FILE *fptr, int vpos);
void CreateLinkedList(rowNode **HN, FILE *fptr);
void ColumnLinking(rowNode *HN);
void BoxLinking(rowNode *HN);

void PrintRow(numNode *HN, int choice, rowNode *topNode);
void PrintGrid(rowNode *HN);
void PrintPos(rowNode *HN);

void Markings(rowNode *HN);
void Doubles(rowNode *HN);
void HiddenSingles(rowNode *HN);
void HiddenDoubles(rowNode *HN);
int Singles(rowNode *HN);
void LockedCandidate(rowNode *HN);
void PointingTuple(rowNode *HN);
void XWing(rowNode *HN);
void YWing(rowNode *HN);

int GenCheckN(numNode *HN, int checkN, int choice);
int GenCheckPos(numNode *HN, int checkPos[], int choice, int ignoreBox[]);
void GenerateFreqArray(int numPos[], numNode *HN, int choice);

void GenDelete(numNode *HN, int delNum[], int ignoreBox[], int choice);

numNode* GetNthNodeH (numNode *HN, int N);
rowNode* GetNthNodeV (rowNode *HN, int N);
numNode* GetNthBox (rowNode *HN, int N);
numNode* GetBoxThirdH(int boxNum, int row, rowNode *HN);
numNode* GetBoxThirdV(int boxNum, int col, rowNode *HN);

void CheckSolved(rowNode *HN);
void FreeMemory(rowNode **HN);
void ClearOutArray (int s[]);
int ArrayLength(int s[]);
int CompareArray(int a1[], int a2[]);
void CopyArray(int a1[], int a2[]);
void MakeAllCaps (char Input[100]);
int FindColWidest(numNode *HN);
int FindRCGivenB(int boxNum, int choice);
void GenerateTargetPos (numNode *HN1, numNode *HN2, int save[]);
void FindVisibleBoxes (int currentBox, int boxList[]);

// FIX ME
// error checking for input file
// more algorithms
// option for solving variations
// program decides when to stop loop
// file: should not allow me to enter in sequence of numbers less than or greater than 9
// tells me when each algorithm was used to eliminate pos (effect)

int main(void) {
    CreateFile();
    FILE *inp;
    inp = fopen("file1.txt","r"); //FILENAME
    rowNode *RowHN;
    
    CreateLinkedList(&RowHN, inp);
    ColumnLinking(RowHN);
    BoxLinking(RowHN);
    
    printf("ORIGINAL PUZZLE:\n");
    PrintGrid(RowHN);
    
    Markings(RowHN);
    
    int i, N = 20; //arbitrary # of alg iterations
    for (i=0;i<N;i++){
        Doubles(RowHN);
        HiddenSingles(RowHN);
        HiddenDoubles(RowHN);
        LockedCandidate(RowHN);
        PointingTuple(RowHN);
        XWing(RowHN);
        YWing(RowHN);
    }
    
    CheckSolved(RowHN);
    printf("FINISHED PROCESSING:\n");
    PrintGrid(RowHN);
    
    FreeMemory(&RowHN);
    fclose(inp);
    return 0;
}

void FindVisibleBoxes (int currentBox, int boxList[]){
    int i, q = 0;
    
    if (currentBox>=1 && currentBox<=3){
        for (i=1;i<=2;i++){ //move horizontally
            if (currentBox+i<=3){
                boxList[q] = currentBox+i;
                q++;
            } else {
                boxList[q] = currentBox+i-3;
                q++;
            }
        }
    } else if (currentBox>=4 && currentBox <=6){
        for (i=1;i<=2;i++){ //move horizontally
            if (currentBox+i<=6){
                boxList[q] = currentBox+i;
                q++;
            } else {
                boxList[q] = currentBox+i-3;
                q++;
            }
        }
    } else if (currentBox>=7 && currentBox <=9){
        for (i=1;i<=2;i++){ //move horizontally
            if (currentBox+i<=9){
                boxList[q] = currentBox+i;
                q++;
            } else {
                boxList[q] = currentBox+i-3;
                q++;
            }
        }
    }
    
    if (currentBox == 1 || currentBox == 4 || currentBox == 7){
        for (i=1;i<=2;i++){ //move vertically
            if (currentBox+3*i<=7){
                boxList[q] = currentBox+3*i;
                q++;
            } else {
                boxList[q] = currentBox+3*i-9;
                q++;
            }
        }
    } else if (currentBox == 2 || currentBox == 5 || currentBox == 8){
        for (i=1;i<=2;i++){ //move vertically
            if (currentBox+3*i<=8){
                boxList[q] = currentBox+3*i;
                q++;
            } else {
                boxList[q] = currentBox+3*i-9;
                q++;
            }
        }
    } else if (currentBox == 3 || currentBox == 6 || currentBox == 9){
        for (i=1;i<=2;i++){ //move vertically
            if (currentBox+3*i<=9){
                boxList[q] = currentBox+3*i;
                q++;
            } else {
                boxList[q] = currentBox+3*i-9;
                q++;
            }
        }
    }
}

numNode* GetBoxThirdH(int boxNum, int row, rowNode *HN){
    numNode *tmp;
    int i;
    
    tmp = GetNthBox(HN, boxNum);
    for (i=0;i<3;i++){
        if (row != tmp->loc[1]){
            tmp = tmp->down;
        } else {
            break;
        }
    }
    
    return tmp;
}

numNode* GetBoxThirdV(int boxNum, int col, rowNode *HN){
    numNode *tmp;
    int i;
    
    tmp = GetNthBox(HN, boxNum);
    for (i=0;i<3;i++){
        if (col != tmp->loc[0]){
            tmp = tmp->right;
        } else {
            break;
        }
    }
    
    return tmp;
}

void YWing(rowNode *HN){
    int i, j, k, q, m, n, delNum[2] = {0, 0}, *ignoreBox = calloc(9, sizeof(int)), *save = calloc(2, sizeof(int)), *boxList = calloc(4, sizeof(int)), *ignoreBox2 = calloc(9, sizeof(int)), effect;
    rowNode *topNode = HN;
    numNode *tmp = HN->aRow, *saveHeader, *targetBox;
    
    for (j=1;j<=2;j++){ // 1 for box method, 2 for regular method
        for (i=1;i<=9;i++){
            switch (j){
                case 1:
                    tmp = GetNthNodeV(topNode, i)->aRow;
                    break;
                case 2:
                    tmp = GetNthBox(topNode, i);
                    break;
            }
             for (k=0;k<9;k++){ //go along row/col, looking for 2 cells that share a pos
                 if (ArrayLength(tmp->pos)==2){ //if we see 2 digit pos
                     saveHeader = tmp;
                     switch (j){
                         case 1:
                             tmp = tmp->right;
                             break;
                         case 2:
                             tmp = tmp->boxNext;
                             break;
                     }
                     for (q=k+1;q<9;q++){ //look for another 2 digit pos along row/col
                         if (ArrayLength(tmp->pos)==2){
                             GenerateTargetPos(saveHeader, tmp, save);
                             if (save[0]!=0){ //find the third cell, make deletion
                                 switch(j){
                                     case 1:
                                         if (GenCheckPos(GetNthNodeH(topNode->aRow, saveHeader->loc[0]), save, 2, ignoreBox)==1) { //FIX ME
                                             if (save[0]==tmp->pos[0] || save[0]==tmp->pos[1]){
                                                 delNum[0] = save[0];
                                             } else {
                                                 delNum[0] = save[1];
                                             }
                                             targetBox = GetNthNodeV(topNode, ignoreBox[0]/10)->aRow;
                                             targetBox = GetNthNodeH(targetBox, tmp->loc[0]);
                                             if (saveHeader->loc[2]!=targetBox->loc[2] && tmp->loc[2]!=targetBox->loc[2]){
                                                 GenDelete(targetBox, delNum, ignoreBox, 4);
                                             }
                                         } else if (GenCheckPos(GetNthNodeH(topNode->aRow, tmp->loc[0]), save, 2, ignoreBox)==1) {
                                             if (save[0]==saveHeader->pos[0] || save[0]==saveHeader->pos[1]){
                                                 delNum[0] = save[0];
                                             } else {
                                                 delNum[0] = save[1];
                                             }
                                             targetBox = GetNthNodeV(topNode, ignoreBox[0]/10)->aRow;
                                             targetBox = GetNthNodeH(targetBox, saveHeader->loc[0]);
                                             if (saveHeader->loc[2]!=targetBox->loc[2] && tmp->loc[2]!=targetBox->loc[2]){
                                                 GenDelete(targetBox, delNum, ignoreBox, 4);
                                             }
                                         }
                                         break;
                                     case 2:
                                         FindVisibleBoxes(tmp->loc[2], boxList);
                                         for (m=0;m<4;m++){
                                             if (GenCheckPos(GetNthBox(topNode, boxList[m]), save, 3, ignoreBox)>0){
                                                 for (n=0;n<ArrayLength(ignoreBox);n++){
                                                     if (ignoreBox[n]/10 == saveHeader->loc[1]){ //shares row with saveheader
                                                         if (save[0]==tmp->pos[0] || save[0]==tmp->pos[1]){
                                                             delNum[0] = save[0];
                                                         } else {
                                                             delNum[0] = save[1];
                                                         }
                                                         ignoreBox2[0] = ignoreBox[n];
                                                         ignoreBox2[1] = 10*saveHeader->loc[1]+saveHeader->loc[0];
                                                         ignoreBox2[1] = 10*tmp->loc[1]+tmp->loc[0];
                                                         GenDelete(GetBoxThirdH(boxList[m], tmp->loc[1], topNode), delNum, ignoreBox2, 5);
                                                         GenDelete(GetBoxThirdH(tmp->loc[2], ignoreBox[n]/10, topNode), delNum, ignoreBox2, 5);
                                                     } else if (ignoreBox[n]/10 == tmp->loc[1]){ //shares row with tmp
                                                         if (save[0]==saveHeader->pos[0] || save[0]==saveHeader->pos[1]){
                                                             delNum[0] = save[0];
                                                         } else {
                                                             delNum[0] = save[1];
                                                         }
                                                         ignoreBox2[0] = ignoreBox[n];
                                                         ignoreBox2[1] = 10*saveHeader->loc[1]+saveHeader->loc[0];
                                                         ignoreBox2[1] = 10*tmp->loc[1]+tmp->loc[0];
                                                         GenDelete(GetBoxThirdH(boxList[m], saveHeader->loc[1], topNode), delNum, ignoreBox2, 5);
                                                         GenDelete(GetBoxThirdH(tmp->loc[2], ignoreBox[n]/10, topNode), delNum, ignoreBox2, 5);
                                                     } else if (ignoreBox[n]%10 == saveHeader->loc[0]){ //shares col with saveHeader
                                                         if (save[0]==tmp->pos[0] || save[0]==tmp->pos[1]){
                                                             delNum[0] = save[0];
                                                         } else {
                                                             delNum[0] = save[1];
                                                         }
                                                         ignoreBox2[0] = ignoreBox[n];
                                                         ignoreBox2[1] = 10*saveHeader->loc[1]+saveHeader->loc[0];
                                                         ignoreBox2[1] = 10*tmp->loc[1]+tmp->loc[0];
                                                         GenDelete(GetBoxThirdV(boxList[m], tmp->loc[0], topNode), delNum, ignoreBox2, 6);
                                                         GenDelete(GetBoxThirdV(tmp->loc[2], ignoreBox[n]%10, topNode), delNum, ignoreBox2, 6);
                                                     } else if (ignoreBox[n]%10 == tmp->loc[0]){ //shares col with tmp
                                                         if (save[0]==saveHeader->pos[0] || save[0]==saveHeader->pos[1]){
                                                             delNum[0] = save[0];
                                                         } else {
                                                             delNum[0] = save[1];
                                                         }
                                                         ignoreBox2[0] = ignoreBox[n];
                                                         ignoreBox2[1] = 10*saveHeader->loc[1]+saveHeader->loc[0];
                                                         ignoreBox2[1] = 10*tmp->loc[1]+tmp->loc[0];
                                                         GenDelete(GetBoxThirdV(boxList[m], saveHeader->loc[0], topNode), delNum, ignoreBox2, 6);
                                                         GenDelete(GetBoxThirdV(tmp->loc[2], ignoreBox[n]%10, topNode), delNum, ignoreBox2, 6);
                                                     }
                                                 }
                                                 ClearOutArray(ignoreBox);
                                             }
                                         }
                                         break;
                                 }
                             }
                             ClearOutArray(ignoreBox2);
                             ClearOutArray(save);
                             delNum[0] = 0;
                             ClearOutArray(boxList);
                         }
                         switch (j){
                             case 1:
                                 if (tmp->right != NULL){
                                     tmp = tmp->right;
                                 }
                                 break;
                             case 2:
                                 if (tmp->boxNext != NULL){
                                     tmp = tmp->boxNext;
                                 }
                                 break;
                         }
                     }
                     switch (j){
                         case 1:
                             tmp = saveHeader->right;
                             break;
                         case 2:
                             tmp = saveHeader->boxNext;
                             break;
                     }
                 } else {
                     switch (j){
                         case 1:
                             tmp = tmp->right;
                             break;
                         case 2:
                             tmp = tmp->boxNext;
                             break;
                     }
                 }
             } //k for loop
        } // i for loop
    } // j for loop
    do {
        effect = Singles(HN);
    } while (effect>0);
}

void GenerateTargetPos (numNode *HN1, numNode *HN2, int save[]){
    numNode *saveHeader = HN1, *tmp = HN2;
    if (saveHeader->pos[0] == tmp->pos[0]){
        if (saveHeader->pos[1]<tmp->pos[1]){
            save[0] = saveHeader->pos[1];
            save[1] = tmp->pos[1];
        } else if (saveHeader->pos[1]>tmp->pos[1]){
            save[0] = tmp->pos[1];
            save[1] = saveHeader->pos[1];
        }
    } else if (saveHeader->pos[0] == tmp->pos[1]){
        if (saveHeader->pos[1]<tmp->pos[0]){
            save[0] = saveHeader->pos[1];
            save[1] = tmp->pos[0];
        } else if (saveHeader->pos[1]>tmp->pos[0]){
            save[0] = tmp->pos[0];
            save[1] = saveHeader->pos[1];
        }
    } else if (saveHeader->pos[1] == tmp->pos[0]){
        if (saveHeader->pos[0]<tmp->pos[1]){
            save[0] = saveHeader->pos[0];
            save[1] = tmp->pos[1];
        } else if (saveHeader->pos[0]>tmp->pos[1]){
            save[0] = tmp->pos[1];
            save[1] = saveHeader->pos[0];
        }
    } else if (saveHeader->pos[1] == tmp->pos[1]){
        if (saveHeader->pos[0]<tmp->pos[0]){
            save[0] = saveHeader->pos[0];
            save[1] = tmp->pos[0];
        } else if (saveHeader->pos[0]>tmp->pos[0]){
            save[0] = tmp->pos[0];
            save[1] = saveHeader->pos[0];
        }
    }
}

void XWing(rowNode *HN){
    rowNode *topNode = HN;
    numNode *tmp = HN->aRow;
    int i, j, k, m, q , n, effect, *numPos = calloc(9, sizeof(int)), *ignoreBox = calloc(9,sizeof(int)), *save = calloc(50, sizeof(int)), delNum[2] = {0,0};
    
    for (k=1;k<=2;k++){ //k=1 for row, k=2 for col
        q = 0;
        for (i=1;i<=9;i++){
            switch (k){
                case 1:
                    tmp = GetNthNodeV(topNode, i)->aRow;
                    break;
                case 2:
                    tmp = GetNthNodeH(topNode->aRow, i);
                    break;
            }
            GenerateFreqArray(numPos, tmp, k);
            for (m=0;m<9;m++){
                if (numPos[m]>=11 && numPos[m]<=99){ //look for # that appears twice in the row/col
                    save[q] = m+1; //0 3 6 save num
                    save[q+1] = numPos[m]; //save numPos[m]
                    save[q+2] = i; //save row/col #
                    q+=3;
                }
            }
            for (j=0;j<9;j++){
                numPos[j] = 0;
            }
        }
        for (n=0;n<ArrayLength(save);n+=3){
            for (j=n+3;j<ArrayLength(save);j+=3){
                if ((save[n]==save[j]) && (save[n+1]==save[j+1])){
                    switch (k){
                        case 1:
                            delNum[0] = save[n];
                            ignoreBox[0]= 10*save[n+2]+save[n+1]/10;
                            ignoreBox[1]= 10*save[n+2]+save[n+1]%10;
                            ignoreBox[2]= 10*save[j+2]+save[j+1]/10;
                            ignoreBox[3]= 10*save[j+2]+save[j+1]%10;
                            GenDelete(GetNthNodeH(topNode->aRow, save[n+1]/10), delNum, ignoreBox, 2);
                            GenDelete(GetNthNodeH(topNode->aRow, save[n+1]%10), delNum, ignoreBox, 2);
                            break;
                        case 2:
                            delNum[0] = save[n];
                            ignoreBox[0]= 10*(save[n+1]/10)+save[n+2];
                            ignoreBox[1]= 10*(save[n+1]%10)+save[n+2];
                            ignoreBox[2]= 10*(save[j+1]/10)+save[j+2];
                            ignoreBox[3]= 10*(save[j+1]%10)+save[j+2];
                            GenDelete(GetNthNodeV(topNode, save[n+1]/10)->aRow, delNum, ignoreBox, 1);
                            GenDelete(GetNthNodeV(topNode, save[n+1]%10)->aRow, delNum, ignoreBox, 1);
                            break;
                    }
                }
                delNum[0] = 0;
                for (m=0;m<9;m++){
                    ignoreBox[m] = 0;
                }
            }
        }
        ClearOutArray(save);
    }
    do {
        effect = Singles(HN);
    } while (effect>0);
}

int FindRCGivenB(int boxNum, int choice){
    int k = 0;
    
    if (choice == 2) { //find col number
        if (boxNum == 1 || boxNum == 4 || boxNum == 7 ){
            k = 1;
        } else if (boxNum == 2 || boxNum == 5 || boxNum == 8 ){
            k = 4;
        } else if (boxNum == 3 || boxNum == 6 || boxNum == 9 ){
            k = 7;
        }
    } else if (choice == 1){ //find row number
        if (boxNum == 1 || boxNum == 2 || boxNum == 3 ){
            k = 1;
        } else if (boxNum == 4 || boxNum == 5 || boxNum == 6 ){
            k = 4;
        } else if (boxNum == 7 || boxNum == 8 || boxNum == 9 ){
            k = 7;
        }
    }
    return k;
}

void PointingTuple (rowNode *HN){
    rowNode *topNode = HN;
    int i, j=0, k = 0, q, m, *numPos = calloc(9, sizeof(int)) , *ignoreBox = calloc(9, sizeof(int)), *delNum = calloc(2,sizeof(int)), effect;
    
    for (i=1;i<=9;i++){ //go through 9 different boxes
        GenerateFreqArray(numPos, GetNthBox(topNode, i), 3);
        for(q=0;q<9;q++){ //figure which row or col to delete from
            if (numPos[q] == 321 || numPos[q] == 32 || numPos[q] == 31 || numPos[q] == 21){
                delNum[0] = q+1;
                if (i>=1 && i<=3){
                    j = 1;
                } else if (i>=4 && i<=6){
                    j = 4;
                } else if (i>=7 && i<=9){
                    j = 7;
                }
                k = FindRCGivenB(i, 2);
                ignoreBox[0] = 10*j+k; ignoreBox[1] = 10*j+k+1; ignoreBox[2] = 10*j+k+2;
                GenDelete(GetNthNodeV(topNode, j)->aRow, delNum, ignoreBox, 1);
            } else if (numPos[q] == 654 || numPos[q] == 65 || numPos[q] == 54 || numPos[q] == 64) {
                delNum[0] = q+1;
                if (i>=1 && i<=3){
                    j = 2;
                } else if (i>=4 && i<=6){
                    j = 5;
                } else if (i>=7 && i<=9){
                    j = 8;
                }
                k = FindRCGivenB(i, 2);
                ignoreBox[0] = 10*j+k; ignoreBox[1] = 10*j+k+1; ignoreBox[2] = 10*j+k+2;
                GenDelete(GetNthNodeV(topNode, j)->aRow, delNum, ignoreBox, 1);
            } else if (numPos[q] == 987 || numPos[q] == 98 || numPos[q] == 87 || numPos[q] == 97) {
                delNum[0] = q+1;
                if (i>=1 && i<=3){
                    j = 3;
                } else if (i>=4 && i<=6){
                    j = 6;
                } else if (i>=7 && i<=9){
                    j = 9;
                }
                k = FindRCGivenB(i, 2);
                ignoreBox[0] = 10*j+k; ignoreBox[1] = 10*j+k+1; ignoreBox[2] = 10*j+k+2;
                GenDelete(GetNthNodeV(topNode, j)->aRow, delNum, ignoreBox, 1);
            } else if (numPos[q] == 741 || numPos[q] == 74 || numPos[q] == 41 || numPos[q] == 71) {
                delNum[0] = q+1;
                if (i==1 || i==4 || i==7){
                    j = 1;
                } else if (i==2 || i==5 || i==8){
                    j = 4;
                } else if (i==3 || i==6 || i==9){
                    j = 7;
                }
                k = FindRCGivenB(i, 1);
                ignoreBox[0] = 10*k+j; ignoreBox[1] = 10*(k+1)+j; ignoreBox[2] = 10*(k+2)+j;
                GenDelete(GetNthNodeH(topNode->aRow, j), delNum, ignoreBox, 2);
            } else if (numPos[q] == 852 || numPos[q] == 85 || numPos[q] == 52 || numPos[q] == 82) {
                delNum[0] = q+1;
                if (i==1 || i==4 || i==7){
                    j = 2;
                } else if (i==2 || i==5 || i==8){
                    j = 5;
                } else if (i==3 || i==6 || i==9){
                    j = 8;
                }
                k = FindRCGivenB(i, 1);
                ignoreBox[0] = 10*k+j; ignoreBox[1] = 10*(k+1)+j; ignoreBox[2] = 10*(k+2)+j;
                GenDelete(GetNthNodeH(topNode->aRow, j), delNum, ignoreBox, 2);
            } else if (numPos[q] == 963 || numPos[q] == 96 || numPos[q] == 63 || numPos[q] == 93) {
                delNum[0] = q+1;
                if (i==1 || i==4 || i==7){
                    j = 3;
                } else if (i==2 || i==5 || i==8){
                    j = 6;
                } else if (i==3 || i==6 || i==9){
                    j = 9;
                }
                k = FindRCGivenB(i, 1);
                ignoreBox[0] = 10*k+j; ignoreBox[1] = 10*(k+1)+j; ignoreBox[2] = 10*(k+2)+j;
                GenDelete(GetNthNodeH(topNode->aRow, j), delNum, ignoreBox, 2);
            }
        }
        for(m=0;m<9;m++){ // initialize
            numPos[m] = 0;
            ignoreBox[m]=0;
        }
    }
    do {
        effect = Singles(HN);
    } while (effect>0);
}

void CheckSolved(rowNode *HN){
    rowNode *tmp2 = HN;
    numNode *tmp = tmp2->aRow;
    int solved = 1;
    
    while (tmp2!= NULL) {
        tmp = tmp2->aRow;
        while (tmp!=NULL) {
            if (tmp->N==0){
                solved = 0;
                break;
            }
            tmp = tmp->right;
        }
        tmp2 = tmp2->next;
    }
    
    switch (solved){
        case 1: //if solved
            printf("SOLVED! ");
            return;
            break;
        case 0: //if not solved
            printf("REMAINING MARKINGS:\n");
            PrintPos(HN);
            break;
    }
}

void LockedCandidate(rowNode *HN){
    rowNode *topNode = HN;
    numNode *tmp = HN->aRow;
    int i, j, q, m, *numPos = calloc(9, sizeof(int)), box, *ignoreBox = calloc(5, sizeof(int)), *delnum = calloc(2, sizeof(int)), effect;
    
    for (i=1;i<=2;i++){ //1 for checking rows, 2 for checking columns
        for (j=1;j<=9;j++){ //get to each of 9 rows or columns
            box = 0;
            switch (i){
                case 1:
                    tmp = GetNthNodeV(topNode, j)->aRow; //get to the right now
                    break;
                case 2:
                    tmp = GetNthNodeH(topNode->aRow, j); //get to the right col
                    break;
            }
            GenerateFreqArray(numPos, tmp, i);
            //printf("%d %d %d %d %d %d %d %d %d\n", numPos[0], numPos[1], numPos[2], numPos[3], numPos[4], numPos[5], numPos[6], numPos[7], numPos[8]);
            
            for(q=0;q<9;q++){ //figure which box to delete from
                if (numPos[q] == 321 || numPos[q] == 32 || numPos[q] == 31 || numPos[q] == 21){
                    if (i==1){ //delete from box 1,4,7
                        ignoreBox[0] = 10*j+1; ignoreBox[1] = 10*j+2; ignoreBox[2] = 10*j+3;
                        if (j>=1 && j<=3){ //j is row num
                            box = 1;
                        } else if (j>=4 && j<=6){
                            box = 4;
                        } else if (j>=7 && j<=9){
                            box = 7;
                        }
                    } else{ //delete from box 1,2,3
                        ignoreBox[0] = 10*1+j; ignoreBox[1] = 10*2+j; ignoreBox[2] = 10*3+j;
                        if (j>=1 && j<=3){ //j is col num
                            box = 1;
                        } else if (j>=4 && j<=6){
                            box = 2;
                        } else if (j>=7 && j<=9){
                            box = 3;
                        }
                    }
                    delnum[0] = q+1;
                    GenDelete(GetNthBox(topNode, box), delnum, ignoreBox, 3);
                } else if (numPos[q] == 654 || numPos[q] == 65 || numPos[q] == 54 || numPos[q] == 64) {
                    if (i==1){ //delete from box 2,5,8
                        ignoreBox[0] = 10*j+4; ignoreBox[1] = 10*j+5; ignoreBox[2] = 10*j+6;
                        if (j>=1 && j<=3){
                            box = 2;
                        } else if (j>=4 && j<=6){
                            box = 5;
                        } else if (j>=7 && j<=9){
                            box = 8;
                        }
                    } else{ //delete from box 4,5,6
                        ignoreBox[0] = 10*4+j; ignoreBox[1] = 10*5+j; ignoreBox[2] = 10*6+j;
                        if (j>=1 && j<=3){
                            box = 4;
                        } else if (j>=4 && j<=6){
                            box = 5;
                        } else if (j>=7 && j<=9){
                            box = 6;
                        }
                    }
                    delnum[0] = q+1;
                    GenDelete(GetNthBox(topNode, box), delnum, ignoreBox, 3);
                } else if (numPos[q] == 987 || numPos[q] == 98 || numPos[q] == 87 || numPos[q] == 97) {
                    if (i==1){ //delete from box 3,5,9
                        ignoreBox[0] = 10*j+7; ignoreBox[1] = 10*j+8; ignoreBox[2] = 10*j+9;
                        if (j>=1 && j<=3){
                            box = 3;
                        } else if (j>=4 && j<=6){
                            box = 6;
                        } else if (j>=7 && j<=9){
                            box = 9;
                        }
                    } else{ //delete from box 7,8,9
                        ignoreBox[0] = 10*7+j; ignoreBox[1] = 10*8+j; ignoreBox[2] = 10*9+j;
                        if (j>=1 && j<=3){
                            box = 7;
                        } else if (j>=4 && j<=6){
                            box = 8;
                        } else if (j>=7 && j<=9){
                            box = 9;
                        }
                    }
                    delnum[0] = q+1;
                    GenDelete(GetNthBox(topNode, box), delnum, ignoreBox, 3);
                }
            }
            delnum[0] = 0;
            for(m=0;m<9;m++){ // initialize
                numPos[m] = 0;
                ignoreBox[m]=0;
            }
            
        }
    }
    do {
        effect = Singles(topNode);
    } while (effect>0);
}

void CreateFile(void) {
    char YoN = 'K';
    
    printf("File exists? (Y or N) ");
    while (YoN!='y' && YoN!='Y'&& YoN!='n'&& YoN!='N'){
        scanf("%c", &YoN);
    }
    
    if (YoN == 'Y' || YoN == 'y') {
        printf("\n");
        return;
    } else if (YoN == 'N' || YoN == 'n') {
        FILE *exp;  exp = fopen("file1.txt","w");
        int i,j,in,printNum;
        
        for (i=0;i<9;i++){
            in = 0; //initialization
            printf("Enter row %d: ", i+1);
            scanf("%d",&in);
            for (j=0;j<9;j++){
                printNum = in/pow(10,8-j);
                in = in - printNum*pow(10,8-j);
                fprintf(exp,"%d ", printNum);
            }
            fprintf(exp,"\n");
        }
        fclose(exp);
        printf("\n");
    }
    return;
}

void FreeMemory(rowNode **HN){
    rowNode *tmp;
    int i;
    
    for (i=0; i<9; i++){
        tmp = *HN;
        *HN = tmp->next; //move head node to second node
        tmp->next = NULL;
        free(tmp);
    }
}

numNode* GetNthBox (rowNode *HN, int N){
    int box[3] = {1, 4, 7};
    numNode *tmp;
    
    if (N>=1 && N<=3){
        tmp = HN->aRow;
        switch (N){
            case 1:
                tmp = GetNthNodeH(tmp, box[0]);
                break;
            case 2:
                tmp = GetNthNodeH(tmp, box[1]);
                break;
            case 3:
                tmp = GetNthNodeH(tmp, box[2]);
                break;
        }
    } else if (N>=4 && N<=6){
        tmp = GetNthNodeV(HN, box[1])->aRow;
        switch (N){
            case 4:
                tmp = GetNthNodeH(tmp, box[0]);
                break;
            case 5:
                tmp = GetNthNodeH(tmp, box[1]);
                break;
            case 6:
                tmp = GetNthNodeH(tmp, box[2]);
                break;
        }
    } else {
        tmp = GetNthNodeV(HN, box[2])->aRow;
        switch (N){
            case 7:
                tmp = GetNthNodeH(tmp, box[0]);
                break;
            case 8:
                tmp = GetNthNodeH(tmp, box[1]);
                break;
            case 9:
                tmp = GetNthNodeH(tmp, box[2]);
                break;
        }
    }
    
    return tmp;
}

void GenerateFreqArray(int numPos[], numNode *HN, int choice){
    int j, k, count, count2;
    
    for (j=1;j<=9;j++){ //check the pos of each box in the row/col/box
        if (HN == NULL){
            break;
        }
        for (k = 0;k<ArrayLength(HN->pos);k++){
            if (numPos[HN->pos[k]-1] == 0){
                numPos[HN->pos[k]-1]+=j;
            } else {
                count = 0;
                count2 = numPos[HN->pos[k]-1];
                while (count2 >= 10){
                    count2 = count2 / 10;
                    count+=1;
                }
                numPos[HN->pos[k]-1]+=j*pow(10,count+1);
            }
        }
        switch (choice) {
            case 1:
                HN = HN->right; //move on to next box in row
                break;
            case 2:
                HN = HN->down;
                break;
            case 3:
                HN = HN->boxNext;
                break;
        }
    }
}

void HiddenDoubles(rowNode *HN){
    rowNode *topNode = HN;
    numNode *tmp = HN->aRow, *saveHeader;
    int *numPos = calloc(9,sizeof(int));
    int i, j, m, n, q, p, effect, new, *save = calloc(9,sizeof(int)), *pair = calloc(2, sizeof(int));
    
    for (j=1;j<=3;j++){ //j=1 for row, 2 for col, 3 for box
        for (i=1;i<=9;i++){ //go down through each row/col/box linked list
            
            switch (j) { //get address of first node in the rcb linked list
                case 1:
                    tmp = GetNthNodeV(topNode, i)->aRow; //move on to next box in row
                    break;
                case 2:
                    tmp = GetNthNodeH(topNode->aRow, i);
                    break;
                case 3:
                    tmp = GetNthBox(topNode, i);
                    break;
            }
            saveHeader = tmp;
            
            GenerateFreqArray(numPos, tmp, j);
            
            n = 0; new = 1;
            for(m=0;m<9;m++){ // see if a two digit number appears twice in the row
                if (numPos[m]>=11 && numPos[m]<=99) { //only looking at 2 digit numbers
                    for (q=0;q<ArrayLength(save);q++) {
                        if (save[q]==numPos[m]){ //if this two digit number has already been saved
                            new = numPos[m]; //we have a hidden double!!
                        }
                    }
                    if (save[0]==0 || new==1){
                        save[n] = numPos[m];
                        n++;
                    }
                }
            }
            
            if (new != 1){ // if found double, delete all other pos from the two boxes
                n = 0;
                for(m=0;m<9;m++){
                    if (numPos[m] == new) {
                        pair[n] = m+1;
                        n++;
                    }
                }
                tmp = saveHeader;
                switch (j) { //get address of first node in the rcb linked list
                    case 1:
                        ClearOutArray(GetNthNodeH(tmp, new%10)->pos);
                        CopyArray(pair, GetNthNodeH(tmp, new%10)->pos);
                        ClearOutArray(GetNthNodeH(tmp, new/10)->pos);
                        CopyArray(pair, GetNthNodeH(tmp, new/10)->pos);
                        break;
                    case 2:
                        for (p=0;p<new%10-1;p++){
                            tmp = tmp->down;
                        }
                        ClearOutArray(tmp->pos);
                        CopyArray(pair, tmp->pos);
                        tmp = saveHeader;
                        for (p=0;p<new/10-1;p++){
                            tmp = tmp->down;
                        }
                        ClearOutArray(tmp->pos);
                        CopyArray(pair, tmp->pos);
                        break;
                    case 3:
                        for (p=0;p<new%10-1;p++){
                            tmp = tmp->boxNext;
                        }
                        ClearOutArray(tmp->pos);
                        CopyArray(pair, tmp->pos);
                        tmp = saveHeader;
                        for (p=0;p<new/10-1;p++){
                            tmp = tmp->boxNext;
                        }
                        ClearOutArray(tmp->pos);
                        CopyArray(pair, tmp->pos);
                        break;
                        break;
                }
                
            }
            
            for(m=0;m<9;m++){ // initialize
                numPos[m] = 0;
                save[m] = 0;
            }
        }
    }
    do {
        effect = Singles(HN);
    } while (effect>0);
}

void GenDelete(numNode *HN, int delNum[], int ignoreBox[], int choice){ //delete the numbers in delNum[] from all boxes in this row, except for from ignoreBox(es)
    //choice 1: delete numbers from row, pass in left most box in the row
    //choice 2: delete numbers from col, pass in top most box in the col
    //choice 3: delete numbers from bold box, pass in top left most box in bold box
    //choice 4: delete numbers from a box
    //choice 5: delete numbers from a horizontal third of a box
    //choice 6: delete numbers from a vertical third of a box
    
    int i, k, j, q, m, checkIgnore;
    
    if (choice == 5 || choice == 6){
        for (i=0;i<3;i++){
            checkIgnore = 0;
            for (j=0;j<ArrayLength(ignoreBox);j++){ //check if we should delete pos from this box
                if (10*HN->loc[1]+HN->loc[0]==ignoreBox[j]){
                    checkIgnore = 1;
                }
            }
            if (checkIgnore == 0){
                for (q=0;q<ArrayLength(HN->pos);q++){ // run through each of the delNum[]
                    for (k=0;k<ArrayLength(HN->pos);k++){ //run through each of the positions in the box
                        if (HN->pos[k] == delNum[q]){
                            HN->pos[k]=0; //delete that delNum
                            m = 1;
                            while (HN->pos[k+m] != 0){ //shift non-zero values forward
                                HN->pos[k+m-1]=HN->pos[k+m];
                                HN->pos[k+m] = 0;
                                m++;
                            }
                        }
                    }
                }
            }
            if (i!=2){
                switch (choice) {
                    case 5:
                        HN = HN->right; //move on to next box in row
                        break;
                    case 6:
                        HN = HN->down;
                        break;
                }
            }
        }
        return;
    }
  
    while (HN!=NULL){
        checkIgnore = 0;
        for (j=0;j<ArrayLength(ignoreBox);j++){ //check if we should delete pos from this box
            if (10*HN->loc[1]+HN->loc[0]==ignoreBox[j]){
                checkIgnore = 1;
            }
        }
        if (checkIgnore == 0){
            for (q=0;q<ArrayLength(HN->pos);q++){ // run through each of the delNum[]
                for (k=0;k<ArrayLength(HN->pos);k++){ //run through each of the positions in the box
                    if (HN->pos[k] == delNum[q]){
                        HN->pos[k]=0; //delete that delNum
                        m = 1;
                        while (HN->pos[k+m] != 0){ //shift non-zero values forward
                            HN->pos[k+m-1]=HN->pos[k+m];
                            HN->pos[k+m] = 0;
                            m++;
                        }
                    }
                }
            }
        }
        switch (choice) {
            case 1:
                HN = HN->right; //move on to next box in row
                break;
            case 2:
                HN = HN->down;
                break;
            case 3:
                HN = HN->boxNext;
                break;
            case 4:
                return;
                break;
        }
    }
}

int GenCheckN(numNode *HN, int checkN, int choice){
    int presence = 0;
    
    while (HN!=NULL){
        if (HN->N==checkN){
            presence+=1;
        }
        switch (choice) {
            case 1:
                HN = HN->right; //move on to next box in row
                break;
            case 2:
                HN = HN->down;
                break;
            case 3:
                HN = HN->boxNext;
                break;
        }
    }
    return presence;
}

int GenCheckPos(numNode *HN, int checkPos[], int choice, int ignoreBox[]){
    int presence = 0, m = 0, i;
    
    while (HN!=NULL){
        if (ArrayLength(checkPos)>1){
            if (CompareArray(checkPos, HN->pos)==1){ //1 for matching arrays
                presence+=1;
                ignoreBox[m] = 10*HN->loc[1] + HN->loc[0];
                m++;
            }
        } else {
            for (i=0;i<ArrayLength(HN->pos);i++){
                if (checkPos[0]==HN->pos[i]){
                    presence+=1;
                }
            }
        }
        switch (choice) {
            case 1:
                HN = HN->right; //move on to next box in row
                break;
            case 2:
                HN = HN->down;
                break;
            case 3:
                HN = HN->boxNext;
                break;
        }
    }
    
    return presence;
}

void ColumnLinking(rowNode *HN){
    numNode *tmp;
    rowNode *tmpDown;
    int i,j;
    
    for (i=1;i<=9;i++){ //i represents horizontal position
        tmpDown = HN; //reset to pos (1, 1)
        tmp = GetNthNodeH(tmpDown->aRow, i);
        for (j=1;j<=9;j++){ //repeat column assignment all the way down the column
            if (tmp->loc[1]!=9){ //while vertical position is not 9
                tmpDown = tmpDown->next; //get the address of the next row
                tmp->down = GetNthNodeH(tmpDown->aRow, i); //assign the address of bottom box
                tmp = tmp->down;
            } else {
                tmp->down = NULL;
            }
        }
    }
}

void BoxLinking(rowNode *HN){
    numNode *tmp, *saveHeader;
    rowNode *tmp2;
    int i,j,k,m,box[3] = {1, 4, 7};
    
    for (m=0;m<3;m++){ //block of 3-BOXES vertically
        tmp2 = GetNthNodeV(HN,box[m]);  //represents either entire row 1, 4, or 7
        for (i=0;i<3;i++){ //block of 3 BOXES horizontally
            tmp = GetNthNodeH(tmp2->aRow, box[i]); //get to either box 1, 4, or 7 of given row
            saveHeader = tmp;
            for (j=0;j<3;j++){ //move down three rows
                for (k=0;k<3;k++){ //move right through three boxes
                    tmp->boxNext = tmp->right;
                    if (k!=2){
                        tmp = tmp->boxNext;
                    }
                }
                if (j!=2){
                    tmp->boxNext = saveHeader->down;
                    tmp = tmp->boxNext;
                    saveHeader = saveHeader->down;
                } else {
                    tmp->boxNext = NULL;
                }
            }
        }
    }
    
}

void ClearOutArray (int s[]){ //clear out a given array
    int i=0;
    while(s[i]!=0){
        s[i] = 0;
        i++;
    }
}

void Markings(rowNode *HN){
    rowNode *tmp;
    numNode *tmp2;
    int i,j,k=0, l, effect, *ignoreBox = calloc(9, sizeof(int));
    
    tmp = HN;
    tmp2 = tmp->aRow;
    
    for (l=1;l<=9;l++){ //goes through entire grid vertically
        for (i=1;i<=9;i++){ //goes through entire row horizontally
            if (tmp2->N == 0){ //if we encounter a blank space on a row
                for (j=1;j<=9;j++){ //check every possible num from 1 to 9
                    if (GenCheckN(tmp->aRow, j, 1)==0 && GenCheckN(GetNthNodeH(HN->aRow, tmp2->loc[0]), j, 2)==0 && GenCheckN(GetNthBox(HN, tmp2->loc[2]), j, 3)==0){
                        tmp2->pos[k] = j; //save possible answers for that row
                        k+=1;
                    }
                    
                }
                if (tmp2->pos[1]==0){
                    tmp2->N = tmp2->pos[0]; //filling in
                    GenDelete(GetNthNodeV(HN, tmp2->loc[1])->aRow, &tmp2->N, ignoreBox, 1);
                    GenDelete(GetNthNodeH(HN->aRow, tmp2->loc[0]), &tmp2->N, ignoreBox, 2);
                    GenDelete(GetNthBox(HN, tmp2->loc[2]),  &tmp2->N, ignoreBox, 3);
                }
            }
            k = 0;
            tmp2 = tmp2->right; //look at next box in row
        }
        if (l!=9){
            tmp = tmp->next; //look at next row
            tmp2 = tmp->aRow;
            k = 0;
        }
    }
    do {
        effect = Singles(HN);
    } while (effect>0);
}

void Doubles(rowNode *HN){
    rowNode *topNode = HN;
    numNode *tmp = HN->aRow;
    int i, j, save[2], ignoreBox[2], effect = 0;
    
    for (i=0;i<9;i++){
        for (j=0;j<9;j++){
            if (tmp->N == 0){
                if (ArrayLength(tmp->pos)==2){
                    CopyArray(tmp->pos, save);
                    if (GenCheckPos(HN->aRow, save, 1, ignoreBox)==2){ //pair appears twice in row
                        GenDelete(HN->aRow, save, ignoreBox, 1);
                    }
                    if (GenCheckPos(GetNthNodeH(topNode->aRow, tmp->loc[0]), save, 2, ignoreBox)==2){
                        GenDelete(GetNthNodeH(topNode->aRow, tmp->loc[0]), save, ignoreBox, 2);
                        
                    }
                    if (GenCheckPos(GetNthBox(topNode, tmp->loc[2]), save, 3,ignoreBox)==2){
                        GenDelete(GetNthBox(topNode, tmp->loc[2]), save, ignoreBox, 3);
                    }
                }
            }
            tmp = tmp->right;
        }
        if (i!=8){
            HN = HN->next;
            tmp = HN->aRow;
        }
    }
    do {
        effect = Singles(topNode);
    } while (effect>0);
}

void HiddenSingles(rowNode *HN){
    rowNode *topNode = HN;
    numNode *tmp = HN->aRow;
    int k, ignoreBox[2] = {0,0}, save[2], effect = 0;
    
    while (HN!=NULL){
        tmp = HN->aRow;
        while (tmp!=NULL){ //moving horizontally
            if (tmp->N == 0 && ArrayLength(tmp->pos)>1){ //if we meet empty box
                for (k=0;k<ArrayLength(tmp->pos);k++){ //runs through all possible values in that box
                    save[0] = tmp->pos[k]; save[1] = 0;
                    if (GenCheckPos(HN->aRow, save, 1, ignoreBox)==1){ //row single
                        tmp->N = tmp->pos[k];
                        ClearOutArray(tmp->pos);
                        GenDelete(GetNthNodeH(topNode->aRow,tmp->loc[0]), &tmp->N, ignoreBox, 2); //delete from col
                        GenDelete(GetNthBox(topNode, tmp->loc[2]), &tmp->N, ignoreBox, 3); //delete from box
                    } else if (GenCheckPos(GetNthNodeH(topNode->aRow, tmp->loc[0]), save, 2, ignoreBox)==1){ //col single
                        tmp->N = tmp->pos[k];
                        ClearOutArray(tmp->pos);
                        GenDelete(GetNthNodeV(topNode, tmp->loc[1])->aRow, &tmp->N, ignoreBox, 1); //delete from row
                        GenDelete(GetNthBox(topNode, tmp->loc[2]), &tmp->N, ignoreBox, 3); //delete from box
                    } else if (GenCheckPos(GetNthBox(topNode, tmp->loc[2]), save, 3, ignoreBox)==1){ //box single
                        tmp->N = tmp->pos[k];
                        ClearOutArray(tmp->pos);
                        GenDelete(GetNthNodeH(topNode->aRow,tmp->loc[0]), &tmp->N, ignoreBox, 2); //delete from row
                        GenDelete(GetNthNodeV(topNode, tmp->loc[1])->aRow, &tmp->N, ignoreBox, 1); //delete from col
                    }
                }
            }
            tmp = tmp->right;
        }
        HN = HN->next;
    }
    do {
        effect = Singles(topNode);
    } while (effect>0);
    return;
}

int Singles(rowNode *HN){
    int i,j, ignoreBox[2] = {0,0}, effect = 0;
    rowNode *topNode = HN;
    numNode *tmp = HN->aRow;
    
    for (i=0;i<9;i++){
        for (j=0;j<9;j++){
            if (tmp->N == 0 && tmp->pos[1]==0){
                effect = 1;
                tmp->N = tmp->pos[0];
                GenDelete(GetNthNodeV(topNode, tmp->loc[1])->aRow, &tmp->N, ignoreBox, 1);
                GenDelete(GetNthNodeH(topNode->aRow, tmp->loc[0]), &tmp->N, ignoreBox, 2);
                GenDelete(GetNthBox(topNode, tmp->loc[2]),  &tmp->N, ignoreBox, 3);
            }
            tmp = tmp->right;
        }
        if (i!=8){
            HN = HN->next;
            tmp = HN->aRow;
        }
    }
    return effect;
}

void CopyArray(int a1[], int a2[]){
    int i=0;
    while (i<ArrayLength(a1) || i<ArrayLength(a2)){
        a2[i]=a1[i];
        i++;
    }
}

int CompareArray(int a1[], int a2[]){
    int i=0, num = 0; //num = 0 for not the same, = 1 for exactly the same
    while (a1[i]==a2[i] && a1[i] != 0 && a2[i] != 0){
        i++;
    }
    if (i == ArrayLength(a1) && i == ArrayLength(a2)){
        num = 1;
    }
    return num;
}

void PrintGrid(rowNode *HN){
    rowNode *topNode = HN;
    int i;
    
    printf(" -----------------------\n");
    for (i=1; i<=9; i++){
        PrintRow(HN->aRow, 1, topNode);
        if (i%3==0){
            printf(" -----------------------\n");
        }
        HN = HN->next;
    }
}

void CreateLinkedList(rowNode **HN, FILE *fptr){
    rowNode *tmp;
    int i;
    
    *HN = (rowNode*)calloc(sizeof(rowNode), 1);
    tmp = *HN;
    for (i=1; i<=9; i++){ // scan in 1 row of 9 number
        CreateRowLinkedList(&tmp->aRow, fptr, i);
        if (i==9){
            (tmp)->next = NULL;
        } else {
            (tmp)->next = (rowNode*)calloc(sizeof(rowNode), 1);
        }
        tmp = (tmp)->next;
    }
}

void CreateRowLinkedList(numNode **HN, FILE *fptr, int vpos){ //[0] H position; [1] V position; [2] box number
    numNode *tmp;
    int j=0;
    
    *HN = (numNode*)calloc(sizeof(numNode), 1);
    tmp = *HN;
    
    for (j=1; j<=9; j++){ // scan in 1 row of 9 numbers
        fscanf(fptr, "%d ",&tmp->N);
        tmp->loc[0] = j; //horizontal position
        tmp->loc[1] = vpos; //vertical position
        if (tmp->loc[1]<=3){ //box 1, 2, 3
            if (tmp->loc[0]<=3){
                tmp->loc[2]=1;
            } else if (tmp->loc[0]<=6){
                tmp->loc[2]=2;
            } else {
                tmp->loc[2]=3;
            }
        } else if (tmp->loc[1]<=6){ //box 4, 5, 6
            if (tmp->loc[0]<=3){
                tmp->loc[2]=4;
            } else if (tmp->loc[0]<=6){
                tmp->loc[2]=5;
            } else {
                tmp->loc[2]=6;
            }
        } else { //box 7, 8, 9
            if (tmp->loc[0]<=3){
                tmp->loc[2]=7;
            } else if (tmp->loc[0]<=6){
                tmp->loc[2]=8;
            } else {
                tmp->loc[2]=9;
            }
        }
        if (j==9){
            tmp->right = NULL;
        } else {
            tmp->right = (numNode*)calloc(sizeof(numNode),1);
        }
        tmp = tmp->right;
    }
}

void PrintPos(rowNode *HN){
    rowNode *topNode = HN;
    numNode *tmp = topNode->aRow;
    int i, j, count = 5; //1 for space before 1st num, 2+2 for 2 middle vertical lines
    
    for (i=0;i<9;i++){
        count+=FindColWidest(tmp);
        count++; //for the space
        tmp = tmp->right;
    }
    
    printf(" ");
    for (j=0;j<count;j++){
        printf("-");
    }
    printf("\n");
    
    for (i=1; i<=9; i++){
        PrintRow(HN->aRow, 2, topNode);
        if (i%3==0){
            printf(" ");
            for (j=0;j<count;j++){
                printf("-");
            }
            printf("\n");
        }
        HN = HN->next;
    }
}

void PrintRow(numNode *HN, int choice, rowNode *topNode){
    int i, j, k, numSpace, n, m;
    
    printf("| ");
    for (i = 1; i<=3; i++){
        for (j = 1; j<=3; j++){
            if (choice == 1 || HN->N != 0){
                printf("%d", HN->N); //add dot for debugging
            } else {
                for (k=0;k<ArrayLength(HN->pos);k++){
                    printf("%d", HN->pos[k]);
                }
            }
            if (choice == 2){
                numSpace = FindColWidest(GetNthNodeH(topNode->aRow, HN->loc[0]));
                if (HN->N != 0) {
                    m = 1;
                } else {
                    m = ArrayLength(HN->pos);
                }
                for (n=0;n<numSpace-m;n++){
                    printf(" ");
                }
            }
            printf(" ");
            HN = HN->right;
        }
        printf("| ");
    }
    printf("\n");
}

int FindColWidest(numNode *HN){
    int widthNum = 1;
    
    while (HN!=NULL){
        if (ArrayLength(HN->pos)>widthNum){
            widthNum = ArrayLength(HN->pos);
        }
        HN = HN->down;
    }
    
    return widthNum;
}

numNode* GetNthNodeH (numNode *HN, int N){
    int i;
    if (N<1 || N> 9){ //error checking
        return NULL;
    }
    for (i=1; i<N; i++){
        HN = HN->right;
    }
    return(HN);
}

rowNode* GetNthNodeV (rowNode *HN, int N){
    int i;
    if (N<1 || N> 9){ //error checking
        return NULL;
    }
    for (i=1; i<N; i++){
        HN = HN->next;
    }
    return(HN);
}

int ArrayLength(int s[]){
    int i=0;
    while ((s[i]>=1 && s[i]<=9) || (s[i]>=11 && s[i]<=99)){
        i++;
    }
    return i;
}

void MakeAllCaps (char Input[100]){
    int i;
    for (i=0;i<strlen(Input); i++){
        if(Input[i]>='a'){
            Input[i]-=('a'-'A');
        }
    }
}
