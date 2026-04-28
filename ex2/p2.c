#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <mpi.h>

int isPalindrome(char str[]){
    int i = 0, j = strlen(str) - 1;
    while (i < j){
        if (str[i] != str[j])
            return 0;
        i++;
        j--;
    }
    return 1;
}

void toUpperCase(char str[]){
    for (int i = 0; str[i]; i++)
        str[i] = toupper(str[i]);
}

int main(int argc, char *argv[]){
    int rank, size;
    char message[50];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    char inp[4][50] = {"madam", "malayalam", "bolero", "sujan"};
    if (rank == 0){
        MPI_Status status;
        for (int i = 1; i < size; i++){
            MPI_Recv(message, 50, MPI_CHAR, MPI_ANY_SOURCE,
                     MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            int sender = status.MPI_SOURCE;
            int type = status.MPI_TAG;

            if (type == 1){
                toUpperCase(message);
                printf("\n[Type1] from proc %d : uppercase = %s \n",
                       sender, message);
            }else if (type == 2){
                if (isPalindrome(message))
                    printf("\n[Type2] from proc %d : %s is Palindrome \n",
                           sender, message);
                else
                    printf("\n[Type2] from proc %d : %s is NOT Palindrome \n",
                           sender, message);
            }
        }
    }
    else{
        if (rank % 2 == 1){
            strcpy(message, inp[rank-1]);
            MPI_Send(message, 50, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        }else{
            strcpy(message, inp[rank-1]);
            MPI_Send(message, 50, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}


