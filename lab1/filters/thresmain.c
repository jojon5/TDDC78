#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ppmio.h"
#include "thresfilter.h"

int main (int argc, char ** argv) {
    int xsize, ysize, colmax, num_threads;
	int i,equal_work_unit;
	pixel *src = malloc(sizeof(pixel)*MAX_PIXELS);
    struct timespec stime, etime;
    struct thresfilter_attr threads_params[MAX_THREADS];
	unsigned long int thread_ids[MAX_THREADS];
	double run_time, float_ops;

	if(!src){
		printf("Could not allocate memory, exiting");
		exit(1);
	}

    /* Take care of the arguments */

    if (argc != 4) {
	fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
	exit(1);
    }

    /* read file */
    if(read_ppm (argv[2], &xsize, &ysize, &colmax, (char *) src) != 0)
        exit(1);

	/* thread n arg */
    num_threads = atoi(argv[1]);

    if (colmax > 255) {
	fprintf(stderr, "Too large maximum color-component value\n");
	exit(1);
    }

    printf("Has read the image, calling filter\n");

    clock_gettime(CLOCK_REALTIME, &stime);

    /* thread input */
	equal_work_unit = xsize*ysize/num_threads;
	for (i = 0; i < num_threads-1; ++i) {
		threads_params[i].start = src + equal_work_unit*i;
		threads_params[i].end = threads_params[i].start + equal_work_unit;
	}
	threads_params[num_threads-1].start = src + equal_work_unit*(num_threads-1);
	threads_params[num_threads-1].end = src + xsize*ysize;

	pthread_create(&thread_ids

    thresfilter(xsize, ysize, src);

    clock_gettime(CLOCK_REALTIME, &etime);

    printf("Filtering took: %g secs\n", (etime.tv_sec  - stime.tv_sec) +
	   1e-9*(etime.tv_nsec  - stime.tv_nsec)) ;

    /* write result */
    printf("Writing output file\n");
    
    if(write_ppm (argv[3], xsize, ysize, (char *)src) != 0)
      exit(1);


    return(0);
}
