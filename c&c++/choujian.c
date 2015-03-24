
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>

  unsigned int myrand(unsigned int r)
  {
      //return r + 50000 * 9 + 5000 * 8 + 500 * 7 + 50 * 6 + r % 10;                                                                           
      return r + r*2 + r*3 + r*5 + r*7 + r *11 + r *13;
  }

  int f(int n, int cn)
  {
      if( n <= 0 || cn <= 0 || cn > n ){
          return -1;
      }
      if( n > 1000 * 1000 * 10 ){
          return -1;
      }
      int i = 0;
      char *a = (char *)malloc(sizeof(char)*n);
      memset(a, 0, sizeof(char)*n);
      if ( NULL == a ) {
          return -1;
      }
      unsigned int r = time();
      int cnn = 0;
      do {
          int k = r%n;
          if(*(a+k) == 1){
              if(k >= n-1){
                  k = 0;
             }
              for( ; *(a+k) == 1; k++ ){};
          }
          printf("%ith one: %i\n", cnn+1, k);
          *(a+k) = 1;
          r = myrand(r);
      }while(++cnn < cn);
      return 0;
  }

  int main()
  {
      return f(500*1000, 5000);
  }
