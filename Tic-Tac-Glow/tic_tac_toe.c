#include <stdio.h>
#include <string.h>

char board[3][3]; // 3x3 board
char player1[50], player2[50]; // Player names
char symbol1, symbol2; // Player symbols
char currentPlayer[50]; // Current player's name
char currentSymbol; // Current player's symbol

void initializeBoard() {
    int count = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = count + '0';
            count++;
        }
    }
}

void printBoard() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", board[i][j]);
            if (j < 2) printf("|");
        }
        printf("\n");
        if (i < 2) printf("---|---|---\n");
    }
    printf("\n");
}

int checkWin() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return 1;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return 1;
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return 1;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return 1;

    return 0;
}

int isDraw() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] >= '1' && board[i][j] <= '9') return 0;
    return 1;
}

void switchPlayer() {
    if (strcmp(currentPlayer, player1) == 0) {
        strcpy(currentPlayer, player2);
        currentSymbol = symbol2;
    } else {
        strcpy(currentPlayer, player1);
        currentSymbol = symbol1;
    }
}

void makeMove() {
    int choice;
    while (1) {
        printf("%s (%c), enter your move (1-9): ", currentPlayer, currentSymbol);
        scanf("%d", &choice);

        if (choice < 1 || choice > 9) {
            printf("Invalid move! Try again.\n");
            continue;
        }

        int row = (choice - 1) / 3;
        int col = (choice - 1) % 3;

        if (board[row][col] == 'X' || board[row][col] == 'O') {
            printf("Spot already taken! Try again.\n");
        } else {
            board[row][col] = currentSymbol;
            break;
        }
    }
}

int main() {
    printf("Enter Player 1 name: ");
    scanf("%s", player1);
    printf("Enter Player 2 name: ");
    scanf("%s", player2);

    // Choosing symbols
    while (1) {
        printf("%s, choose your symbol (X or O): ", player1);
        scanf(" %c", &symbol1);
        if (symbol1 == 'X' || symbol1 == 'O') break;
        printf("Invalid symbol! Please choose X or O.\n");
    }
    symbol2 = (symbol1 == 'X') ? 'O' : 'X';
    printf("%s will be %c\n", player2, symbol2);

    // Set the first player
    strcpy(currentPlayer, player1);
    currentSymbol = symbol1;

    initializeBoard();
    printBoard();

    while (1) {
        makeMove();
        printBoard();

        if (checkWin()) {
            printf("Congratulations %s! You win!\n", currentPlayer);
            break;
        }

        if (isDraw()) {
            printf("It's a draw!\n");
            break;
        }

        switchPlayer();
    }

    return 0;
}
