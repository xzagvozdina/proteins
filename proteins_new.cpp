/*==========================================================================*/
/*                                                                          */
/*              Simple Sampling of Self-Avoiding Random Walks               */
/*                                                                          */
/*        In this program we generate self-avoiding walks as if they were   */
/*        random walks. Every walker carries a unique number which he       */
/*        leaves as a trace on a lattice. If he tries to step on a site     */
/*        which he visited before the self-avoiding condition is violated   */
/*        and the walk terminates. Unsuccessful attempts are noted on the   */
/*        screen to give an impression to how many random walks fail to be  */
/*        self-avoiding. The quantities caluculated in this program are the */
/*        radius of gyration and the end-to-end distance.                   */
/*                                                                          */
/*        The lattice size must be adjusted to the length of the SAW. The   */
/*        program terminates if the walkler leaves the lattice. Adjust the  */
/*        lattice size and the window size to give maximal resolution.      */
/*                                                                          */
/*       PROGRAM NAME:      saw                                             */
/*                                                                          */
/*       CALLING SEQUENCE:                                                  */
/*                                                                          */
/*         Main program without command line arguments.                     */
/*                                                                          */
/*       RETURN:                                                            */
/*                                                                          */
/*         Exit code -1     Walker left the lattice. Adjust the linear      */
/*                          dimension of the lattice                        */
/*                                                                          */
/*   Version 1.00                                                           */
/*                                                                          */
/*   (c) Satoru Ozawa and Dieter W. Heermann                                */
/*   Hitachi, Heidelberg July 1994                                          */
/*                                                                          */
/*==========================================================================*/

/* ==== Include files ==== */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// #include "sgl.h"               /* Include for the graphics library   */
// #include "r250.c"              /* Shift-register generator           */

/* ==== Constants ==== */

#define l_max       25         /* Linear dimension of the lattice on */                                /* on which the saw walks             */
#define NMax        50         /* largest saw which can be generated */
#define WindowSize  500        /* Size in pixle of the graphics      */

/*--------------------------------------------------------------------------*/
/*                         Main program                                     */
/*--------------------------------------------------------------------------*/
#ifdef  __STDC__
void    main(int argc, char *argv[])
#else
void    main(argc,argv)
        int     argc;
        char    *argv[];
#endif
  {
    /* -------------------------------------------------------------------- */
    /*                        Declarations                                  */
    /* -------------------------------------------------------------------- */

    /*   ==== Arrays    ==== */

    int g[l_max][l_max];
    int w[NMax][2];
    int mf[251];

    int i,j, sample;
    int N;
    int l;                         /*  True length the walk                 */
    int d;
    int x,y, xc, yc;
    int occupancy;
    int walk;
    int sample_size;
    int iseed, return_code;

    float end_to_end, radius_of_gyration, rad;
    float cmx,cmy;
    float ran[NMax];

    /* -------------------------------------------------------------------- */
    /*                       End of Declarations                            */
    /* -------------------------------------------------------------------- */

    /* -------------------------------------------------------------------- */
    /*                Read in the simulation parameters                     */
    /* -------------------------------------------------------------------- */

    /* ====  Print the header for this test  ==== */

    printf("=============================================\n");
    printf("======>  Self-Avoiding Random Walks  <=======\n");
    printf("=============================================\n");
    printf("\n\n");
    printf("(c) Satoru Ozawa and Dieter W. Heermann \n");
    printf("Version 1.00 (1994) \n");
    printf("\n\n");

    printf("Maximum walk length to generated = ");
    scanf("%d",&N);
    printf("Sample size                      = ");
    scanf("%d",&sample_size);
    printf("Seed for Random Number Generator = ");
    scanf("%d",&iseed);

    /* -------------------------------------------------------------------- */
    /*                    Set and Initialize                                */
    /* -------------------------------------------------------------------- */

    for (i=0; i < l_max; i++) {
       for (j=0; j < l_max; j++) {
        g[i][j] = -1;
       }
    }

    xc                 = (l_max - 1) / 2;
    yc                 = (l_max - 1) / 2;

    end_to_end         = 0;
    radius_of_gyration = 0;
    sample             = 0;
    walk               = 0;

    /*  ==== set up the random number generator ==== */

    iseed       = (iseed << 2) + 1;
    return_code = init_r250(iseed,mf);

    /* ==== initialize the graphics ==== */

    sgl_winsize(WindowSize,WindowSize);
    sgl_init();

    /*----------------------------------------------------------------------*/
    /*                    Begin of Sample Loop                              */
    /*----------------------------------------------------------------------*/

    while ( sample < sample_size ) {

      /* ==== Reset the walker to the origin ==== */

      w[0][0]   = xc;
      w[0][1]   = yc;
      x         = xc;
      y         = yc;
      l         = 0;
      occupancy = 0;
      walk++;
      g[xc][yc] = walk;

      return_code = r250( N,ran,mf);

      while ( (l < N)  &&  (occupancy == 0) )  {
        d  = ran[l] * 4;
        switch (d) {
            case 0:  x++;
                     break;

            case 1:  y++;
                     break;

            case 2:  x--;
                     break;

            case 3:  y--;
                     break;

        } /* end switch */

        if ( ( x < 0 ) || ( x == l_max ) || ( y < 0 ) || ( y == l_max ) ) {
            /* Random walker not on the lattice */
            exit(-1);
        }

        if ( g[x][y] < walk ) {
            g[x][y] = walk;
            l++;
            w[l][0] = x;
            w[l][1] = y;
            occupancy = 0;
        }
        else {
            occupancy = 1;
        }
      } /* end while */

      /* ==== Now check if a SAW was generated. If yes, then ==== */
      /* ==== we do the analysis, else we must try again     ==== */

      if ( l == N ) {
        /* ---- display the walk ---- */

         sgl_color_background(SGL_BLACK);
         for(i=1;i<l;i++) {
            if ( i > 0 ) {
                sgl_line((float) w[i-1][0]/ (float) l_max,
                         (float) w[i-1][1]/(float) l_max,
                         (float) w[i][0]/  (float) l_max,
                         (float) w[i][1]/  (float) l_max);
            }
         }
         sgl_display();

        /* ---- we can compute the end-to-end distance etc. ---- */

        x = xc - w[N-1][0];
        y = yc - w[N-1][1];
        end_to_end += x*x + y*y;

        cmx = 0;
        cmy = 0;
        for (i=0; i < N; i++) {
          cmx += w[i][0];
          cmy += w[i][1];
        }
        cmx /= N;
        cmy /= N;

        rad = 0;
        for (i=0; i < N; i++) {
            x    = cmx - w[i][0];
            y = cmy - w[i][1];
            rad += x*x + y*y;
        }
        radius_of_gyration += rad;
       printf(" walk ");
       getchar();

        sample++;
      }
      else {
        printf(" Walk %d	unsuccessful generation at %d \n",walk,l);
      }
    }  /* end while sample loop */

    radius_of_gyration /= sample;
    end_to_end         /= sample;

    printf("Results : \n");
    printf("Sample size                         = %d \n",sample);
    printf("Average radius of gyration squared  = %f \n",radius_of_gyration);
    printf("Average end to end distance squared = %f \n",end_to_end);

} /* end of main program */