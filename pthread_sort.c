#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/*Declare variable*/
int array_length, file_length;
int *array_master;
FILE *freader;

/* Used to read data from a file */
int *read_file() 
{
    freader = fopen("IntegerList.txt", "rt"); 
    int bufsize = file_length*80; 
    char line[80];
    int integer;
    int index = 0;
    int *input = (int *)malloc(bufsize*sizeof(int)); 

    while (fgets(line, 80, freader) != NULL)
    {
		char *p = line;
		char *t_p = p;
		int counter = 0;
		while(*p != '\0')/*(Complete conversion of string to integer)*/
		{
			counter++;
			if(*(p + 1) == ',' ||*(p + 1) == '\0')
			{
				//printf("2\n%d\n%s",counter,t_p);

				char t_str[5] = {};
				strncpy(t_str,t_p,counter);
				//printf("%s\n",t_str);
				if(p + 1 == NULL)
				{
					t_p = NULL;
				}else t_p = p + 2;
				counter = 0;
				sscanf(t_str,"%d",&integer);
				input[index] = integer;
				//printf("%d\n\n",input[index]);
				++index;
				++array_length;
			}
			++p;
		 }
	}

    fclose(freader);
    return input;
}

/*The amount of data*/
int read_length() 
{
    freader = fopen("IntegerList.txt", "rt"); 
    char line[80];
    int file_length = 0;

    while (fgets(line, 80, freader) != NULL)
        file_length += 1;
//	fclose(freader);
    return file_length;
}

/* merge sort */
void sorter(int arr[], int left, int middle, int right)
{
    int i, j, k, ss = 0;
    int half1 = middle - left + 1; /* The amount of data in the first half of the array */
    int half2 = right - middle;  /* The amount of data in the second half of the array */

    int first[half1], second[half2]; /* Declare two temporary arrays,
									After the first half scores according to and half of the data */

    /* Copy the first half of the data from the arr array from left to right */
    for (i = 0; i < half1; i++) 
        first[i] = arr[left + i]; 

    /* Copy the second half of the data between left and right from the ARR array */
    for (j = 0; j < half2; j++) 
        second[j] = arr[middle + 1 + j];

    i = 0;
    j = 0;
    k = left;

    /* Compare the number of two temporary arrays, find out the current minimum number, and store them in the ARR in order */
    while (i < half1 && j < half2) 
    {

        if (first[i] <= second[j]) 
        {
           arr[k] = first[i];
            ++i; 
        }
        else 
        {
            arr[k] = second[j];
            j++;
        }

        k++; /* arr idex */
    }

    /* Stores the remainder of the temporary array in the arr array */
    while (i < half1) 
    {
        arr[k] = first[i];
        i++;
        k++;
    }

    while (j < half2)
    {
        arr[k] = second[j];
        j++;
        k++;
    }
}

/* Merger */
void* merger(void* arg) 
{
    int *arr = array_master; 
    int *argu = (int*)arg;
    int l = argu[0]; /* The parameter passed in by the thread to get the minimum index value of the data to be sorted */
    int r = argu[1]; /* The parameter passed in by the thread to get the maximum index value of the data to sort */
	//printf("%d %d\n",argu[0],argu[1]);

    /* If l==r, you don't have to sort */
    if (l < r) 
    {    
        /* Declare two thread buy descriptors */
        pthread_t tid1;
        pthread_t tid2;

        /* Declares the argument to the calling thread handler */
        int arg1[2];
        int arg2[2];

        int middle;
        middle = (l + (r - 1)) / 2;
        arg1[0] = l;
        arg1[1] = middle;
        arg2[0] = middle + 1;
        arg2[1] = r;

        /* Since the binary method is used to sort the array into two parts, 
		there is the possibility of parallelism */
        pthread_create(&tid1, NULL, merger, arg1);
        pthread_create(&tid2, NULL, merger, arg2);

	/* You have to wait until both of these are sorted before you can merge,
		So here you call pthread_join to synchronize the threads */
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);

	/* Merge the two sorted subsequences */
        sorter(arr, l, middle, r);
		
        pthread_exit(0);
    }
    
    return NULL;
}

/* The main function */
int main(int argc, char *argv[])
{
    /* Gets the length of the data */
    file_length = read_length();
	//printf("%d\n",file_length);
    /* Read data from a data file */
    array_master = read_file();
	//printf("%d\n",sizeof(array_master));

    int arg[2];
    arg[0] = 0;
    arg[1] = array_length - 1;

    /* Create thread to perform merge sort */
    pthread_t tid;
    pthread_create(&tid, NULL, merger, arg);

    /* Process synchronization */
    pthread_join(tid, NULL);

    /* Prints sorted array to file */
    int j;
	FILE *Fp = fopen("SortedIntegerList.txt","w");
    for (j = 0; j < array_length; j++) 
    {
        if (j == array_length - 1) 
            fprintf(Fp,"%d\n", array_master[j]); /* Prints the last element of the sorted array */
        
        else 
            fprintf(Fp,"%d, ", array_master[j]); /* Prints the non-last element of the sorted array */
    }
	fclose(Fp);

    return 0;
}
