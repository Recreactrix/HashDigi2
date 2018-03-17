#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
/*int ind(int a){

    switch ( a ) {
    case 0:
        return 3    ;
         break;
    case 1:
        return 4;
          break;
    case 2:
         return 0;
          break;
    case 3:
        return 1;
          break;
    case 4:
        return 2;
          break;
    default:
          break;
    }
}*/


uint64_t _bswap64(uint64_t a)
{
  a = ((a & 0x00000000000000FFULL) << 56) |
      ((a & 0x000000000000FF00ULL) << 40) |
      ((a & 0x0000000000FF0000ULL) << 24) |
      ((a & 0x00000000FF000000ULL) <<  8) |
      ((a & 0x000000FF00000000ULL) >>  8) |
      ((a & 0x0000FF0000000000ULL) >> 24) |
      ((a & 0x00FF000000000000ULL) >> 40) |
      ((a & 0xFF00000000000000ULL) >> 56);
  return a;
}

int modulo(int x, int y){
  	if(x<0){
		    x = x*-1;
  		  return (y-(x%y))%y;
            }
    else{
    	  return (x%y);
        }
}


void theta(uint64_t **A,uint64_t **Aaux,uint64_t *C,uint64_t *D)
{
  uint64_t temp;
  for (int i=0;i<5;i++){
    C[i] =  A[i][0] ^ A[i][1] ^ A[i][2] ^ A[i][3] ^ A[i][4];

  }
  for (int j = 0;j<5;j++){
    temp = (C[modulo(j+1,5)] ) << 63 | (C[modulo(j+1,5)]  >> 1);
    D[j] = C[modulo(j-1,5)] ^ temp;
  }
  for (int k = 0;j<5;j++){
    for(int l=0;l<5;l++){
    Aaux[k][l]=A[k][l] ^ D[k];}
  }
}

void rho(uint64_t **A,uint64_t **Aaux){

A[0][0]=Aaux[0][0];
int x=1;
int y=0
int offset=0;
//CALCULAR OFFSET A TRAVES DEL INDICE X
for (int i=0; i<24; i++) {
  if (x==0){
    if (y==0){offset=0;}
    else if (y==1){offset=36;}
    else if (y==2){offset=3;}
    else if (y==3){offset=41;}//105 - 64 = 41
    else if (y==4){offset=18;}//210-192 = 18
  }
  else if (x==1){
    if (y==0){offset=1;}
    else if (y==1){offset=44;}//300-256=44
    else if (y==2){offset=10;}
    else if (y==3){offset=45;}
    else if (y==4){offset=2;}//66-64=2
  }
  else if (x==2){
      if (y==0){offset=62;}//190-128=62
      else if (y==1){offset=6;}
      else if (y==2){offset=43;}//171-128=43
      else if (y==3){offset=15;}
      else if (y==4){offset=61;}//253-192=61
    }
  else if (x==3){
      if (y==0){offset=28;}
      else if (y==1){offset=55;}
      else if (y==2){offset=25;}//153-128=25
      else if (y==3){offset=21;}
      else if (y==4){offset=56;}//120-64=56
    }
  else if (x==4){
      if (y==0){offset=27;}//91-64=27
      else if (y==1){offset=20;}//276-256=20
      else if (y==2){offset=39;}//231-192=39
      else if (y==3){offset=8;}//136-128=8
      else if (y==4){offset=14;}//78-64=14
  }

//DESFASAR BITS A PARTIR DEL OFFSET Y REVISAR
  A[x][y] = Aaux[x][y]>>offset | Aaux[x][y]<<64-offset;
  x=y;
  y=modulo(2*x+3*y,5);
}
}

void pi(uint64_t **A,uint64_t **Aaux){
  for (int i = 0; i<5; i++) {
    for(int j = 0;j<5;j++){
      Aaux[i][j]=A[modulo(i+3*j)][i];
    }
  }
}
void xci(uint64_t **A,uint64_t **Aaux){
  uint64_t uno =1;
  for (int i = 0; i < 5; i++) {
    for (int i = 0; j < 5; j++) {
      Aaux[i][j]=A[i][j] ^ ( (A[modulo(i+1,5)][j] ^ uno) & A[modulo(i+2,5)][j]);
    }
  }
}
uint64_t rc(int t,unsigned char *R){
unsigned char *R= malloc(9*sizeof(unsigned char));
R[0]='0';R[1]='1';for(int j=2;j<9;j++){R[j]='0';}
for (int i = 0; i < modulo(t,255); i++) {
  R=


}


}


int main(/* int argc, const char* argv[] */)
{
	int flag = 1;
  int end;
	int l;
	int r;
	int b = 1600;
	int w = b/25;
	int i = 0;
	int j = 0;
	int h = 0;
	int y = 0;

	FILE *file;
	uint64_t **A;
  uint64_t **C;
  uint64_t **D;
	uint64_t *N;
  uint64_t **Aaux;



	A = (unsigned char **) malloc(5* sizeof(unsigned char *));      //Matriz antes de Theta

	for (r = 0; r<5;r++)
	 {
	    A[r] = (unsigned char *) malloc(sizeof(unsigned char)*5);
	   }

  Aaux = (unsigned char **) malloc(5* sizeof(unsigned char *));      //Matriz auxiliar de A

  for (r = 0; r<5;r++)
    {
      A[r] = (unsigned char *) malloc(sizeof(unsigned char)*5);
    }



  //C = (uint64_t *) malloc(5* sizeof(uint64_t));      //Matriz C antes de Theta
  //D = (uint64_t *) malloc(5* sizeof(uint64_t));      //Matriz  Dantes de Theta

unsigned char

	unsigned char *buffer,Zeros;
	buffer = (unsigned char *) malloc(25*sizeof(unsigned char));
	Zeros = 0;
	file = fopen("prueba","rb");   //-f nombrearchivo -d
	if (file==NULL)
	{
		fputs ("Error de archivo\n", stderr);
		exit(1);
	}

  end = fread(buffer,8,1,file);
	while(end != 0)
	{
		printf("\n");
    printf("%lu",*buffer);
    A[i][j] = *buffer;
	/*	for (h = 0;h<8;h++)
		{
			printf("\n");

			printf("\n");
		}*/
		printf("\n");
		printf("%lu", A[i][j]);
    printf("\n");
    printf("%lu", sizeof(A[i][j]));
    end = fread(buffer,8,1,file);
    //printf("%d",end);
    printf("\n");
		if (j==5){
			i += 1;
		}
		else{
			j+=1;
		}
		if (i==3 && j ==2){
			break;
		}
	}



  A = (unsigned char ***) malloc (5*sizeof(unsigned char ***));

	for (int i = 0; i< 5; i++)
	{
			 A[i] = (unsigned char **) malloc(5*sizeof(unsigned char *));
			 for (int j = 0; j < 5; j++)
			 {
					 A[i][j] = (unsigned char *)malloc(w*sizeof(unsigned char));
			 }
	}

  /*for(j = 2;j<5;j++){
    A[3][j]=Zeros;
  }
	printf("%lu", A[i][j]);
  for (i = 4;i<5;i++)
  {
    for (j = 0;j<5;j++){
      A[i][j] = Zeros;
    }

  }*/
//Si no logra leer el bloque se debe leer bloques mas pequenos
/*  while (end == 0) {
      printf("%lu",*buffer);
      A[i][j] = *buffer;
      for (h = 0;h<8;h++)
      {
        printf("\n");

        //printf("\n");
      }
      end = fread(buffer,8,1,file);
      if (j==5){
        i += 1;
      }
      else{
        j+=1;
      }
      if (i==3 && j ==2){
        break;
      }

      if (i<3 || j<1){
        printf("Se necesita padding \n");
      }
  }*/
	printf("\n");
	/*printf(sizeof(buffer));*/
void free (void* buffer);
return 0;

}
