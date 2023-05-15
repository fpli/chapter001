#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

struct person
{
    int id;
    char fname[20];
    char lname[20];
};

double average(int num, ...)
{
    va_list valist;
    double sum = 0.0;
    int i;

    /* initialize valist for num number of arguments */
    va_start(valist, num);

    /* access all the arguments assigned to valist */
    for (i = 0; i < num; i++)
    {
        sum += va_arg(valist, int);
    }

    /* clean memory reserved for valist */
    va_end(valist);

    return sum / num;
}

int main(int argc, char const *argv[])
{
    printf("Average of 2, 3, 4, 5 = %f\n", average(4, 2, 3, 4, 5));
    printf("Average of 5, 10, 15 = %f\n", average(3, 5, 10, 15));

    FILE *infile;

    // Open person.dat for reading
    infile = fopen("person1.dat", "wb+");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

    struct person write_struct = {1, "Rohan", "Sharma"};

    // writing to file
    fwrite(&write_struct, sizeof(write_struct), 1,
           infile);

    struct person read_struct;

    // setting pointer to start of the file
    rewind(infile);

    // reading to read_struct
    fread(&read_struct, sizeof(read_struct), 1, infile);

    printf("Name: %s %s \nID: %d", read_struct.fname,
           read_struct.lname, read_struct.id);

    // close file
    fclose(infile);
    return 0;
}
