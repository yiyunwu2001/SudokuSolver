//
//  createSudokufile.c
//  PersonalProjects
//
//  Created by Yiyun Wu on 5/20/21.
//

#include <stdio.h>
#include <math.h>

void ClearOutArray (int s[]);

//FIX ME WHY ABORT FOR SOME NUM COMBOS?
//Enter row 1: 602300000
//Enter row 2: 050090000
//Enter row 3: 000000803
//Enter row 4: 761528009
//Enter row 5: 349716500
//Enter row 6: 528439671
//Enter row 7: 900000000
//Enter row 8: 200050010
//Enter row 9: 000007204

int main(void){
    FILE *exp;  exp = fopen("file1.txt","w");
    int i,j,in,printNum;
    
    for (i=0;i<9;i++){
        //ClearOutArray(row);
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
    return 0;
}
