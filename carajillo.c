#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

int main(int argc, char *argv[]){
 // Bandera para saber si no se puede extraer un byte
	int flag = 1;
 // Buffer tipo int
	int A[64];
//bandera para llenar ceros
	int a = 1;
//Contadores IOTA
  int r;int k;int k1;int k2;int h; int i; int j;
//puntero a file
	FILE *file;
  //MATRICES AUXILIARES
	unsigned char **C;
	unsigned char ***S;
	unsigned char ***S1;
	unsigned char ***S2;
	unsigned char ***S3;
	unsigned char ***S4;
  unsigned char ***S5;
	unsigned char **D;
  //////////////////////
  //Inicializar las matrices 3D
	S = (unsigned char ***) malloc (5*sizeof(unsigned char ***));

	for (r = 0; r< 5; r++)
	{
			 S[r] = (unsigned char **) malloc(5*sizeof(unsigned char *));
			 for (k = 0; k < 5; k++)
			 {
					 S[r][k] = (unsigned char *)malloc(64*sizeof(unsigned char));
			 }
	}
  //Inicializar las matrices 2D
	C = (unsigned char **) malloc(5*sizeof(unsigned char *));
	for (k1 = 0; k1 < 5; k1++)
	{
			C[k1] = (unsigned char *)malloc(64*sizeof(unsigned char));
	}
	D = (unsigned char **) malloc(5*sizeof(unsigned char *));
	for (k2 = 0; k2 < 5; k2++)
	{
			D[k2] = (unsigned char *)malloc(64*sizeof(unsigned char));
	}
	unsigned char *buffer = (unsigned char *) malloc (8*sizeof (unsigned char));
 //////////////////////////

	file = fopen(argv[2],"rb");   //-f nombrearchivo -d
	if (file==NULL)
	{
		fputs ("Error de archivo\n", stderr);
		exit(1);
	}
	for(h=0; h<8; h++){
		fread(buffer,8,1,file);
		A[0+8*h] = 0b0001 & buffer[h];
		A[1+8*h] = 0b0010 & buffer[h];
		A[2+8*h] = 0b0011 & buffer[h];
		A[3+8*h] = 0b0100 & buffer[h];
		A[4+8*h] = 0b0101 & buffer[h];
		A[5+8*h] = 0b0110 & buffer[h];
		A[6+8*h] = 0b0111 & buffer[h];
		A[7+8*h] = 0b1000 & buffer[h];
		}

  //LECTURA DEL ARCHIVO
		while (a != 0){
			for (j = 0; j<5; j++){
				for (i = 0; i<5; i++){
					if (i == 2 & j == 3){
						a = 0;
						break;
					}
					else{
						for (k = 0; k<64; k++){
							S[i][j][k] = A[k];
						}
					}
				}
				if (i == 2 & j == 3){
					a = 0;
					break;
				}
			}
		}
//COLUMNAS Y FILAS FALTANTES
		for(j = 3; j < 4; j++ ){
			for (i = 2; i < 5; i++) {
				for (k = 0; k < 64; k++) {
						S[i][j][k] = 0;
				}
			}
		}
		for(j = 4; j < 5; j++ ){
			for (i = 0; i < 5; i++) {
				for (k = 0; k < 64; k++) {
						S[i][j][k] = 0;

				}
			}
		}
/////////////////////////////




	//for (k = 0; k < 64; k++) {
		//printf("%d",S[0][0][k]);
	//}

	//printf("\n");//




	return 0;

}

int mod(int x, int y){
  	if(x<0){
		    x = x*-1;
  		  return (y-(x%y))%y;
            }
    else{
    	  return (x%y);
        }
}

unsigned char rc(int t){
  unsigned char ans;
	unsigned char * R= malloc(8*sizeof(unsigned char));
	unsigned char * R9= malloc(9*sizeof(unsigned char));
	int i;int j;int k; int l;
	R[0]='1';for(i=1;i<8;i++){
	R[i]='0';}
	if(mod(t,255)==1){
    free(R);
    free(R9);
	  return '1';}
	else{
	for(i =1;i<mod(t,255);i++){
		R9[0]='0';for(j=0;j<8;j++){R9[j+1]=R[j];}

		R9[0]= R9[0] ^ R9[8];
		R9[4] = R9[4] ^ R9[8];
		R9[5] = R9[5] ^ R9[8];
		R9[6] = R9[6] ^ R9[8];

		for(k=0;k<8;k++){
		R[k]=R9[k];}

  ans=R[0];
  free(R);
  free(R9);

  return ans;
}}}

void iota(unsigned char ***S5, unsigned char ***S4, int ir){
  int j; int k;int n;int f;
  unsigned char * RC= malloc(64*sizeof(unsigned char));
  for(k=0;k<64;k++){
    RC[k]='0';
  }
  for(j=0;j<6;j++){
    RC[(int)pow(2,j)-1]=rc(j+7*ir);
  }
  for(n=0;n<64;n++){
    S5[0][0][n]=S4[0][0][n] ^ RC[n];
  }
  free(RC);
}
void x(unsigned char ***S4, unsigned char ***S3){
  int x;int y;int z;
  for(x = 0;x<5;x++){
  	for (y = 0; y < 5; y++) {
  		for (z = 0; z < 64; z++) {
  				S4 [x][y][z] = S3 [x][y][z] ^( S3 [mod((x+1),5)][y][z] & S3 [mod((x+2),5)][y][z] ) ;

  		}
  	}
  }
}
void pi(unsigned char ***S3, unsigned char ***S2){
  int x; int y; int z;
  for(x = 0;x<5;x++){
  	for (y = 0; y < 5; y++) {
  		for (z = 0; z < 64; z++) {
  				S3 [x][y][z] = S2[mod((x+3*y),5)][x][z] ;
  		}
  	}
  }
}

void ro(unsigned char ***S2, unsigned char ***S1){
  int z; int i; int t; int y; int x;
  for (z = 0; z < 64; z++) {
  		S2 [0][0][z] = S1[0][0][z] ;
  }
  x= 1;y=0;
  for (int t = 0; t <24; t++) {
  	for (z = 0; z < 64; z++) {
  			S2 [x][y][z] = S1[x][y][mod((z-((t+1)*(t+2))/2),64)];
  	}
  	x = y;
  	y = mod((2*x+3*y),5);
  	}
}

void theta(unsigned char ***S1, unsigned char ***S, unsigned char **C, unsigned char **D){
  int x; int y; int z;
  for(x = 0;x<5;x++){
		for (z = 0; z < 64; z++) {
				C[x][z] = S[x][0][z]^S[x][1][z]^S[x][2][z]^S[x][3][z]^S[x][4][z];
		}
	}
	for(x = 0;x<5;x++){
		for (z = 0; z < 64; z++) {
				D[x][z] = C[mod((x-1),5)][z]^C[mod((x+1),5)][mod((z-1),64)];
		}
	}
	for(x = 0;x<5;x++){
		for (y = 0; y < 5; y++) {
			for (z = 0; z < 64; z++) {
					S1 [x][y][z] = S[x][y][z] ^ D[x][z];
			}
		}
	}
}
