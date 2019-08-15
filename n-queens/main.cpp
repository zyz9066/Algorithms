//  NQueens - main.cpp
//

#include <cstdio>
#include <cstring>
#include "NQ.h"

#define Ndefault 8

static const char * noneswitch = "none";
static const char * shortswitch = "short";
static const char * boardswitch = "board";
static const char * longswitch = "long";
static const char * helpswitch = "help";

void help(  );

int main(int argc, const char * argv[])
{
    
    bool shortflag = false;
    bool boardflag = false;
    bool noneflag = false;
    bool helpflag = true;

    if(helpflag) {
        help();
    }

    unsigned n = Ndefault;
    char c[20];

    printf("\nEnter number of queens to solve: ");
    scanf("%d", &n);

    printf("Enter the above mentioned output format: ");
    scanf("%s", c);

    if(!std::strcmp(c, "none")) noneflag = true;
    else if(!std::strcmp(c, "short")) shortflag = true;
    else if(!std::strcmp(c, "board") || !std::strcmp(c, "long")) boardflag = true;
    else{
        puts("Invalid code!!!");
        return 1;
    }

    printf("Solving for %d queens:\n", n);
    NQ q(n);
    if(boardflag) q.boardflag(boardflag);
    if(shortflag) q.shortflag(shortflag);
    q.solve();
    printf("I found %ld solutions.\n", q.solution_count());
    return 0;
}

void help(  )
{
    printf("n (default: 8): size of board and number of queens.\n\n");
    printf("  none      summary only\n");
    printf("  short     short format\n");
    printf("  board     board format\n");
    printf("  long      synonym for --board\n");
}
