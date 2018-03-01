#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[])
{
	//Declaración de punteros
	FILE *file;
	unsigned char *buffer;
	buffer=malloc(1600 * sizeof(unsigned char));
 //Abrir archivo
	file=fopen(argv[2],"rb");
//En caso de error se sale
	if (file==NULL) {fputs ("Error de archivo\n",stderr); exit (1);}
	//Ir a inicio del archivo
	fseek (file , 0 , SEEK_SET);
	//Meterle a buffer 1 pedazo de 1600 bits de file
	fread(buffer,1600,1,file);
	//barrer buffer
	for(int i=0;i<1600;i++){
	printf("%x\n",buffer[i]);
}
//cerrar archivo
fclose(file);
return 0;
}
