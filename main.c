#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//Funcion XOR
unsigned char XOR(unsigned char x, unsigned char y){
	if x!=y{
		return '1';
	}
	else {
		return '0';
	}

}

//Especificacion de theta  PREGUNTAR COMO RETORNAR, CREAR Y BORRAR EN FUNCION
unsigned char theta(unsigned char***A,unsigned char **C,unsigned char ** D){

	for(int x = 0; x < 5; x++)
	{
			for(int z = 0; z < w; z++)
			{
					C[x][z] = XOR(XOR(XOR(XOR(A[x][0][z],A[x][1][z]), A[x][2][z]), A[x][3][z]),A[x][4][z]);
					D[x][z] = XOR(C [(x-1)%	5][z], C [(x+1)%	5][(z-1) % w]);

			}

	}
	for(int x = 0; x < 5; x++)
	{
		for (z = 0; z < w; z++)
		{
			for(int y = 0; y < 5; y++)
			{
					A[x][y][z] = XOR(A[x][y][z], D [x][z]);
			}
		}
	}

}

//Especificacion de rho
unsigned char rho(unsigned char***A){
		for(int z = 0; z < w; z++)
		{
				Ac[0][0][z] = A[0][0][z];
		}
		x = 1; y = 0;
		for (int t = 0; t<24; t++)
		{
				for(z = 0; z < w; z++)
				{
							Ac [x][y][z] = A[x][y][(z-(t+1)*(t+2)/2)%w];
				}
				x = y; y = (2x+3y)%5;
		}

}

	//Especificacion de pi
unsigned char pi (unsigned char***A){
		for(int x = 0; x < 5; x++)
		{
				for(int y = 0; y < 5; y++)
				{
						for(int z = 0; z < w; z++)
						{
								Ac [x][y][z] = A[(x+3*y)%5][x][z];
						}
				}
		}
}

	//Especificacion de ji
unsigned char ji (unsigned char***A );

int main(int argc, const char* argv[])
{
	//Declaración de variables PREGUNTAR SI HAY Q USAR UNSIGNED CHAR

	int b;
	int w;
	int l;

	//Declaración de punteros

	FILE *file;
	unsigned char *S;
	unsigned char ***A;

	//Asignación de variables y punteros

	b = 1600;

	w = b / 25;

	l = log10(w) / log10(2);

	S=malloc(1600 * sizeof(unsigned char));
//Generar el array A, operar de la forma A[x][y][z]
	A = (unsigned char ***) malloc (5*sizeof(unsigned char ***));

	for (int i = 0; i< 5; i++)
	{
			 A[i] = (unsigned char **) malloc(5*sizeof(unsigned char *));
			 for (int j = 0; j < 5; j++)
			 {
					 A[i][j] = (unsigned char *)malloc(w*sizeof(unsigned char));
			 }
	}
	//Chequear si malloc no ha fallado en A
	if (A != NULL) {;
	} else {
			fputs ("Error de malloc en A\n",stderr);
			exit (1);
	}

	//Chequear que malloc no ha fallado
	if (A[0] != NULL) {;
 	} else {
 			fputs ("Error de malloc en A\n",stderr);
 			exit (1);
 	}
		if (A[0][0] != NULL) {;
	} else {
				fputs ("Error de malloc en A\n",stderr);
				exit (1);
	}
//Chequear si malloc no ha fallado en S
	if (S != NULL) {;
	} else {
			fputs ("Error de malloc en S\n",stderr);
			exit (1);
	}

 	//Abrir archivo

	file=fopen(argv[2],"rb");

	//En caso de error se sale

	if (file==NULL)
	{
		fputs ("Error de archivo\n",stderr);
		exit (1);
	}
	//Ir a inicio del archivo

	fseek (file , 0 , SEEK_SET);

	//Meterle a S 1 pedazo de 1600 bits de file

	fread(S,1600,1,file);

	////////////////////////////////////////
	//Cambiar de string a estados

	for(int x = 0; x < 5; x++)
	{
			for(int y = 0; y < 5; y++)
			{
				for(int z = 0; z < w; z++)
				{
						A[x][y][z] = S[w*(5*y+x)+z]
				}
			}
	}

//cerrar archivo
fclose(file);
return 0;
}
