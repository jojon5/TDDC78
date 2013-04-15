#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#include "ppmio.h"
#include "blurfilter.h"
#include "gaussw.h"

#define MAX_RAD 1000

int main (argc, argv)
  int argc;
  char *argv[];
{
  int sendcount;
  int rank, size;
  int y, i, blocksize, overlapping_data, recv_count, l_ysize;
  pixel *src, *l_src, *l_dst;
  MPI_STATUS status;

  MPI_Init (&argc, &argv);  /* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);  /* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);  /* get number of processes */

  /* Rank 0 reads file.*/
  if(rank == 0){
    src = malloc(sizeof(pixel)*MAX_PIXELS);
    dst = malloc(sizeof(pixel)*MAX_PIXELS);
    if(!src || !dst){
      printf("Could not allocate memory, exiting");
      exit(1);
    }

    sendcounts = malloc(sizeof(int)*size);
    displs = malloc(sizeof(int)*size);

    /* Take care of the arguments */

    if (argc != 4) {
      fprintf(stderr, "Usage: %s threads radius infile outfile\n", argv[0]);
      exit(1);
    }
    radius = atoi(argv[1]);
    if((radius > MAX_RAD) || (radius < 1)) {
      fprintf(stderr, "Radius (%d) must be greater than zero and less then %d\n", radius, MAX_RAD);
      exit(1);
    }

    /* read file */
    if(read_ppm (argv[2], &xsize, &ysize, &colmax, (char *) src) != 0)
      exit(1);

    if (colmax > 255) {
      fprintf(stderr, "Too large maximum color-component value\n");
      exit(1);
    }

    /*printf("Has read the image, generating coefficients\n");*/

    /* filter */
    get_gauss_weights(radius, w);

    /* Calculate offsets for data to be sent to processes. */
    y = 0;
    blocksize = ysize/size;
    overlapping_data = (radius-1)*2;
    
    sendcounts[0] = (blocksize + (0 < ysize%size ? 1:0) + radius-1)*xsize*3;
    displs[0] = y*xsize*3;
    y += blocksize + (0 < ysize%size ? 1:0);
    for(i = 1; i < size-1; ++i){
      sendcount = (blocksize + (i < ysize%size ? 1:0) + overlapping_data)*xsize*3;
      MPI_Isend(src+y*xsize*3, sendcount, MPI_CHAR, i, 0, MPI_COMM_WORLD);
      y += blocksize + (0 < ysize%size ? 1:0);
    }
    sendcount = (blocksize + (size-1 < ysize%size ? 1:0) + radius-1)*xsize*3;
    MPI_Send(src+y*xsize*3, sendcount, MPI_CHAR, i, 0, MPI_COMM_WORLD);

    l_src = src;
    l_ysize = blocksize + (0 < ysize%size ? 1:0) + radius - 1;
    l_dst = malloc(l_ysize*xsize*3);
  }

  MPI_Bcast(&xsize, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
  MPI_Bcast(&ysize, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
  MPI_Bcast(&radius, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

  if(rank > 0){
    l_ysize = ysize/size + (radius-1)*((rank == size-1)?1:2) + (rank < ysize%size ? 1:0);
    l_src = malloc(l_ysize*xsize*3);
    l_dst = malloc(l_ysize*xsize*3);
    MPI_Recv(l_src, l_ysize*xsize*3, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
  }

  blurfilter(l_src, xsize, rank*(ysize/size)*(rank < ysize%size ? :0)
void blurfilter(pixel* start, int xsize, int ystart, int yend, double *w, int radius){


  MPI_Finalize();
  return 0;
}

