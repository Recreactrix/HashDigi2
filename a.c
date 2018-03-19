#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

//Funcion para swap de bits, el fin de esta es cambiar el endian para tratar los datos
uint64_t _bswap64(uint64_t a)
{
	//hacemos AND cada 8 bits y los intercambiamos con los extremos
	a = ((a & 0x00000000000000FFULL) << 56) |
	((a & 0x000000000000FF00ULL) << 40) | 
	((a & 0x0000000000FF0000ULL) << 24) |
	((a & 0x00000000FF000000ULL) <<  8) |  //hasta acabar con los del medio
	((a & 0x000000FF00000000ULL) >>  8) |
	((a & 0x0000FF0000000000ULL) >> 24) |
	((a & 0x00FF000000000000ULL) >> 40) |
	((a & 0xFF00000000000000ULL) >> 56);
	return a; //retornar el nuevo dato
}


//operacion modulo redefinida
int modulo(int x, int y){
	if(x<0)
	{		//si el numero es negativo, el modulo se redefine
		x = x*-1;  
		return (y-(x%y))%y;
	}
	else		//si el numero es positivo el modulo no cambia
	{
		return (x%y);
	}
}

//TRANSFORMACIONES


//IMPORTANTE: todas las transformaciones se hacen con el Estado A
//y una copia Aaux para ahorrar memoria y tiempo, la idea es modificar
// una y retornarle la otra a la siguiente transformacion

//paso theta
void theta(uint64_t **A,uint64_t **Aaux,uint64_t *C,uint64_t *D)
{

	uint64_t temp;int i,j,k,l; //variable auxiliar y contadores

	for (i=0;i<5;i++)
	{	//Hacer XOR de los bits del estado con los pares de las columnas cercanas
	         C[i] =  A[(i)][0] ^ A[(i)][1] ^ 
				A[(i)][2] ^ A[(i)][3] ^ A[(i)][4];
	}
	for (j = 0;j<5;j++)
	{	//hacemos variable temporal que ayude despues para hacer el XOR
	//para el primer bit, como se trabaja en little endian, el bit mas significativo se va al final
	//despues el resto se corre una posicion para dejarlo entrar y se hace la OR
		temp = (C[modulo(j+1,5)] ) >> 63 | (C[modulo(j+1,5)]  << 1);
		D[j] = C[modulo(j-1,5)] ^ temp;
	}
	//A cada elemento se le hace XOR de nuevo con el arreglo auxiliar D, 
	//este es una copia de un pedazo del estado que se le hace XOR de nuevo con el arreglo principal
	for (k = 0;j<5;j++)
	{
		for(l=0;l<5;l++)
		{
			Aaux[(k)][(l)]=A[(k)][(l)] ^ D[k];
		}
	}

}


//paso rho
void rho(uint64_t **A,uint64_t **Aaux){
	//segun el estandar, se debe asignar el primer lane como estaba antes
	A[(0)][(0)]=Aaux[(0)][(0)];
	int x=1;
	int y=0;
	int t;
	//variables de posicion y contador, de acuerdo al estandar
	for(t=0;t<24;t++)
	{
		A[(x)][(y)] = Aaux[(x)][(y)] >> modulo(((t+1)*(t+2)/2),64) |
					 Aaux[(x)][(y)]<< ( 64 - modulo(((t+1)*(t+2)/2),64));
		//cada elemento se le hace un offset especificado por el estandar, 
		// hacia adelante, y como este debe ser circular 
		// se hace la or con el mismo elemento corrido hacia atras
		// por 64 - el valor que se le ordenó	
		x = y;
		y = modulo((2*x + 3*y),5);//asignacion de valores de acuerdo a estandar

	}
}

void pi(uint64_t **A,uint64_t **Aaux){
	int i,j; //variables auxiliares
	for (i = 0; i<5; i++) {
		for(j = 0;j<5;j++){
			Aaux[i][j]=A[modulo(i+3*j,5)][i];
		//rotacion circular de los elementos segun especificacion
		//el efecto es reorganizar las lineas de bits en forma "circular" y despues realinearlas
		}
	}
}

void xci(uint64_t **A,uint64_t **Aaux){
	uint64_t uno =1;int i;int j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			A[i][j]= Aaux[i][j] ^
				 ( (Aaux[modulo(i+1,5)][j] ^ uno) &
					 Aaux[modulo(i+2,5)][j]);
		//esta transformacion es una funcion no linear XOR de un bit
		// con otros dos bits de la fila siguiente
		// y se realiza la AND de la forma especificada en el
		// estandar		
		}
	}
}

//constante rc, esta es clave para iota
uint64_t rc(int t){
	//Inicializacoin de variables auxiliares
	uint64_t ansuint;
	unsigned char ans;
	unsigned char * R= malloc(8*sizeof(unsigned char));
	unsigned char * R9= malloc(9*sizeof(unsigned char));
	int i,j,k,l;
	
	//inicializamos un arreglo de 8 y 9 unsigned chars
	
	R[0]='1'; //el primer elemento del arreglo de 8 elem. es un 1

	for(i=1;i<8;i++)
	{
		R[i]='0'; //el resto lo llenaremos con ceros
	}
	if(modulo(t,255)==1)
	{
		free(R);	//segun el estandar retornar un 1 si se cumple
				//la condicion del if
				//liberamos la memoria que pedimos de lo
		free(R9);	//ya que no se hizo nada
		ansuint = 1;
		return ansuint;
	}
	else		//proceso principal
	{
		for(i =1;i<modulo(t,255);i++) //contador segun estandar
		{
			
		  //se llena el arreglo de 9 con un cero seguido de todos
		  //los datos del arreglo de 8 elementos
			R9[0]='0';for(j=0;j<8;j++){R9[j+1]=R[j];}
			
		 //operacion interna del arreglo segun estandar
			R9[0] = R9[0] ^ R9[8];
			R9[4] = R9[4] ^ R9[8];
			R9[5] = R9[5] ^ R9[8];
			R9[6] = R9[6] ^ R9[8];
		//llenamos el arrelgo de 8 con los elementos del de 9
		//excluyendo el ultimo, esta es la funcion TRUNC8
			for(k=0;k<8;k++)
			{
				R[k]=R9[k];
			}
		
			
		}
		//al terminar el elemento que haya quedado en R[0] se retorna
		// es decir, un cero o un uno, para lo cual se retrona su valor
		//como unsigned long int, para hacerle el XOR con los elementos
		//del estado en iota
			ans=R[0];

			free(R); //liberar memoria pedida
			free(R9);

			if(ans=='1')
			{
				ansuint = 1;
				return ansuint; //retornamos 1 o 0 en el caso necesario
			}
		 	else
			{
				ansuint = 0;
				return ansuint;
			}
	}
}


//especificacion de iota
void iota(uint64_t **A , uint64_t **Aaux ,int ir){

	int j,x;
	uint64_t RC = 0;// este sera un numero que variara su valor
		     	//
		
	for(j=0;j<7;j++) //para los j menores a l = 6 para keccakf1600
	{

		//estos son los casos posibles en el paso 3
		// solo puede ser el indice 0,1,3,7,15,31 y 63
		// rc(j+7ir), ir decide si sale un 1 o un 0 de rc
		if( (int) ((pow(2,j)-1) ) == 0)
		{
		// Hay que modificar un solo bit del numero, entonces si es un uno
		// el resultado de rc se hace una or, puesto que de cualquier modo}
		// habra un uno en esa posicion del indice, si es un 0, irá una and
		// se trata de hacer una operacion and o or para modificar un bit
		// en especifico del arreglo mediante RC

			if ( rc(j+7*ir) == 0x0000000000000001ULL )
			{
	  			RC =( (RC >> 63) | 0x0000000000000001ULL ) << 63 | RC;
			}
			else
			{
				RC = (RC<<1)>>1;
			}
		}
		//modificar bit 1 de RC
		else if( (int) ((pow(2,j)-1) ) == 1   )
		{
			if ( rc(j+7*ir)== 0x0000000000000001ULL )
			{
				RC = RC | ~(0xBFFFFFFFFFFFFFFFULL); 
			}
			else
			{
				RC = RC & (0xBFFFFFFFFFFFFFFFULL);
			}
		}
		//modificar bit 3 de RC
		else if( (int) ((pow(2,j)-1) ) == 3   )
		{
			if ( rc(j+7*ir)== 0x0000000000000001ULL )
			{
			  RC = RC | ~(0xEFFFFFFFFFFFFFFFULL);
			}
			else
			{
			  RC = RC & 0xEFFFFFFFFFFFFFFFULL;
			}
		}
		//modificar bit 7 de RC
		else if( (int) ((pow(2,j)-1) ) == 7   )
		{
			if ( rc(j+7*ir)== 0x0000000000000001ULL )
			{
				RC = RC | ~(0xFEFFFFFFFFFFFFFFULL);
			}
			else
			{
				 RC = RC & 0xFEFFFFFFFFFFFFFFULL;
			}
		}
		//modificar bit 15 de RC
		else if( (int) ((pow(2,j)-1) ) == 15)
		{
			if ( rc(j)== 0x0000000000000001ULL )
			{
				RC = RC | ~(0xFFFEFFFFFFFFFFFFULL);
			}
			else
			{
				RC = RC & 0xFFFEFFFFFFFFFFFFULL;
			}
		}
		//modificar bit 31 de RC
		else if ( (int) ((pow(2,j)-1) ) == 31   )
		{
			if ( rc(j+7*ir)== 0x0000000000000001ULL ){
				RC = RC | ~(0xFFFFFFFEFFFFFFFFULL);
			}
			else{
				RC = RC & 0xFFFFFFFEFFFFFFFFULL;
			}
		}
		//modificar bit 63 de RC
		else
		{
			if ( rc(j+7*ir)== 0x0000000000000001ULL )
			{
				RC = RC | ~(0xFFFFFFFFFFFFFFFEULL);
			}
			else{
				RC = RC & 0xFFFFFFFFFFFFFFFEULL;
			}
		}
	Aaux[0][0]=A[0][0] ^ RC ; //modificar la lane 0,0 de A con el XOR a RC
	}

		
		
}

//juntar todo, la funcion se ejecuta en bas a las matrices dadas
// ademas se depende de una ronda para iota
void RND(uint64_t **A, uint64_t **Aaux, uint64_t *C,uint64_t *D, int ir){
	theta(A,Aaux,C,D);
	rho(A, Aaux);
	pi(A,Aaux);
	xci(A,Aaux);
	iota(A,Aaux,ir);

}

//forma de el estandar
void keccak(uint64_t **A,uint64_t **Aaux, uint64_t *C,uint64_t *D){
	int ir;
	for(ir = 0; ir<24; ir++){
		RND(A, Aaux, C, D, ir);
	}
}


int main( int argc, const char* argv[])
{
	//definir banderas
	int flag;
  	int end;
	//contadores
	
	int i = 0;
	int j = 0;
	int h = 0;
	int hc = 0;
	int y = 0;
 	int by = 0;
  	int p = 0;
  	int to = 0;
	int l;
	int r;
	//variables auxiliares para determinar
	//si se llenaron los 17 bloques
	int bloque;
	int nr = 24;
	int ifinal = 2;
	int jfinal = 2;
	int e = 0;
	int hf = 0;
	int ma = 0;
	//estado 
	uint64_t S[25] = {0};

	//valores de la funcion
	int b = 1600;
	int w = b/25;
	int d = atoi(argv[4]);
	//puntero al archivo
	FILE *file;
	//Matrices de estado, y auxiliares
	uint64_t **A;
  	uint64_t *C;
  	uint64_t *D;
  	uint64_t **Aaux;



	A = (uint64_t **) malloc(5* sizeof(uint64_t *));      //Matriz antes de Theta

	for (r = 0; r<5;r++)
	{
	    A[r] = (uint64_t *) malloc(5*sizeof(uint64_t));
	}

	Aaux = (uint64_t **) malloc(5* sizeof(uint64_t*));      //Matriz auxiliar de A

	for (r = 0; r<5;r++)
	{
		Aaux[r] = (uint64_t *) malloc(5*sizeof(uint64_t));
	}


	C = (uint64_t *) malloc(5* sizeof(uint64_t));      //Matriz C antes de Theta
	D = (uint64_t *) malloc(5* sizeof(uint64_t));      //Matriz D antes de Theta

		//PUNTERO al buffer y el numero cero, que es con el 
		//que se va a llenar los 8 elementos restantes del arreglo
	uint64_t *buffer,Zeros;
	//pedir memoria para el buffer
	buffer = (uint64_t *) malloc(25*sizeof(uint64_t));
	Zeros = 0;

	//abrir archivo con el valor de consola

	file = fopen(argv[2],"rb");   //-f nombrearchivo -l  bytessalida
	if (file==NULL)
	{	//salir si no se pudo abrir el archivo
		fputs ("Error de archivo\n", stderr);
		exit(1);
	}
	if(file!=NULL){

		//escanear el archivo en busca de bloques
		fseek(file,0,SEEK_END);
		to=ftell(file);
		// ver cuantos paquetes de 64 bits puedo sacar
		p= 8*to/64;
		//escanear cuantos huerfanos hay
		by= ((to*8)%64)/8;
		//devolverse a la cabecera del archivo
		rewind(file);
	}
	//leemos el archivo
	while(!feof(file))
	{
	//inicializamos variables auxiliares y banderas
		hc = 0;
		h = 0; j = 2; i = 2; flag = 0;
	  	fread(buffer,8,17,file);
	//leer 17 bloques de 8 bytes en buffer, para irlos metiendo al estado
	//despues del swap
		while(h<17){
			A[i][j] = buffer [hc] ;
			//bandera que dice si estamos en los elementos que
			//se deben llenar con ceros	
			flag = flag+1;

			if(i >= 4)
			{
				i = 0;
			}
			else {
				i = i+1;
			}
			//estos son los subindices que se van
			// a llenar solos
			if((flag+1)%5 == 0 && flag != 0)
			{
				if(j==0)
				{
					j = 4;
				}
				else{
					j = j-1;
				}
			}
			//subir contadores
			h = h+1;
			hc = hc+1;
		}

		while(h< 24){
			A[i][j] = 0;
			flag = flag + 1;
			if(i >= 4)
			{
				i = 0;
			}
			else {
				i = i+1;
			}
			if((flag+1)%5 == 0 && flag != 0 && flag !=24)
			{
				if(j==0)
				{
					j = 4;
				}
				else{
					j = j-1;
				}
			}
			h = h+1;
			hc = hc+1;
		}
		flag = 0;
		
		for(jfinal = 0; jfinal <5; jfinal ++){
			for(ifinal = 0; ifinal <5; ifinal ++){
				A[(ifinal)][(jfinal)] =_bswap64(A[(ifinal)][(jfinal)]);
			}
		}
		keccak(A,Aaux,C,D);
		ma = 2;
		for(jfinal = 0; jfinal <5; jfinal ++){
			for(ifinal = 0; ifinal <5; ifinal ++){
				printf("%lx", Aaux[(ifinal)][(jfinal)]);
				printf("\n");
			}
		}

	}
	//imprimir el arreglo
	printf("fin");
	printf("\n");
	//liberar memoria
	free (buffer);
	free(A);
	free(Aaux);
	free(C);
	free(D);
	return 0;
}

