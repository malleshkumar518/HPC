#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char name[50];
    int roll;
    float total_mark;
    char grade;
} Student;
char find_grade(float marks) {
    if (marks >= 90) 
       return 'A';
    else if (marks >= 75) 
       return 'B';
    else if (marks >= 50) 
       return 'C';
    else if (marks >= 35) 
       return 'D';
    else 
       return 'F';
}
int main(int argc, char *argv[]) {
    int rank, size, n, i;
    MPI_Datatype MPI_STUDENT;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int blocklengths[4] = {50, 1, 1, 1};
    MPI_Aint displacements[4];
    MPI_Aint addresses[4];
    Student temp;
    MPI_Get_address(&temp.name, &addresses[0]);
    MPI_Get_address(&temp.roll, &addresses[1]);
    MPI_Get_address(&temp.total_mark, &addresses[2]);
    MPI_Get_address(&temp.grade, &addresses[3]);
    for (i = 0; i < 4; i++) {
        displacements[i] = addresses[i] - addresses[0];
    }
    MPI_Datatype types[4] = {MPI_CHAR, MPI_INT, MPI_FLOAT, MPI_CHAR};
    MPI_Type_create_struct(4, blocklengths, displacements, types, &MPI_STUDENT);
    MPI_Type_commit(&MPI_STUDENT);
    Student *students = NULL;
    Student *local_students;
    int local_n;
    if (rank == 0) {
        FILE *fp = fopen("student.txt", "r");
        if (fp) {
            students = malloc(100 * sizeof(Student));
            n = 0;
            while (fscanf(fp, "%s %d %f", students[n].name, &students[n].roll, &students[n].total_mark) == 3) {
                n++;
            }
            if(n%size!=0)
            {
                    printf("it is not divisible");
		    fflush(stdout);
                    MPI_Abort(MPI_COMM_WORLD,1);
            }
            fclose(fp);
        }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_n = n / size;
    local_students = malloc(local_n * sizeof(Student));
    MPI_Scatter(students, local_n, MPI_STUDENT, local_students, local_n, MPI_STUDENT, 0, MPI_COMM_WORLD);
    for (i = 0; i < local_n; i++) {
        local_students[i].grade = find_grade(local_students[i].total_mark);
    }
    MPI_Gather(local_students, local_n, MPI_STUDENT, students, local_n, MPI_STUDENT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        FILE *out = fopen("grade.txt", "w");
        if (out) {
            for (i = 0; i < n; i++) {
                fprintf(out, "%s %d %.2f %c\n", students[i].name, students[i].roll, students[i].total_mark, students[i].grade);
            }
            fclose(out);
        }
        free(students);
    }

    free(local_students);
    MPI_Type_free(&MPI_STUDENT);
    MPI_Finalize();
    return 0;
}
