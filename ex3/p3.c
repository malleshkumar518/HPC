#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<mpi.h>

#define array_size 103
#define rand_limit 5

int main(int argc, char **argv)
{
    int rank, size;
    int *full_array = NULL;
    int *local_array = NULL;
    int target_number;
    int local_count = 0, global_count = 0;
    int element_per_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    element_per_proc = array_size / size;

    if(rank == 0)
    {
        printf("Enter the number to find: ");
	fflush(stdout);
        scanf("%d", &target_number);

        full_array = (int*)malloc(array_size * sizeof(int));

        printf("Array: ");
        for(int i = 0; i < array_size; i++)
        {
            full_array[i] = rand() % rand_limit;
            printf("%d ", full_array[i]);
        }
        printf("\n");
    }

    MPI_Bcast(&target_number, 1, MPI_INT, 0, MPI_COMM_WORLD);

    local_array = (int*)malloc(element_per_proc * sizeof(int));

    MPI_Scatter(full_array, element_per_proc, MPI_INT,
                local_array, element_per_proc, MPI_INT,
                0, MPI_COMM_WORLD);

    for(int i = 0; i < element_per_proc; i++)
    {
        if(local_array[i] == target_number)
        {
            local_count++;
        }
    }

    printf("Process %d received %d elements and found number %d times\n",
           rank, element_per_proc, local_count);

    MPI_Reduce(&local_count, &global_count, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        printf("Total frequency of number %d is %d\n",
               target_number, global_count);
        free(full_array);
    }

    free(local_array);
    MPI_Finalize();

    return 0;
}

