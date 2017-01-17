/*
Código creado por Carlos Pastor Ramírez y Miguel Ángel Alfonso Alfonso
para la asignatura Lenguajes y Herramientas de Diseño Digital.

Este fichero contendrá las funciones necesarias en Top.cpp para
realizar correctamente la DCT y la IDCT sobre una imagen conocida.

Curso: 2016-2017

*/


#include "Tasks.h"

//Function deff

/*
Function : blocker_8_512_sq
--------------------------

Ordenará la imagen que le entra como argumento en bloques de 8x8. Por lo tanto, se tendrá a su
salida un vector en el que los primeros 64 valores corresponden al primer bloque de 8x8 de la
imagen de entrada, los siguientes 64 valores corresponderán al segundo bloque, etc.
Para ello es necesario crear el vector block_pattern para conocer qué posiciones coger del vector
de entrada para conseguir el orden correcto.
block_pattern = [0, 1, ..., 7, 512, 513, ..., 519, 1024, 1025, ..., 1031,..., 3583, 3584, ..., 3590]

cdata:	vector de 512x512 correspondiente a los valores de los píxeles de la imagen de entrada.
Primeros 512 valores corresponden a la primera fila de la imagen
Segundos 512 valores corresponden a la segunda final de la imagen
Etc.
bstring:	vector de 512x512 valores en los que los píxeles estarán ordenados en bloques de 8x8.
Primeros 64 valores corresponden al primer bloque de 8x8 píxeles de la imagen original
Segundos 64 valores corresponden al segundo bloque de 8x8 píxeles de la imagen original
Etc.

returns:	Número de bloques resultantes.

*/
int blocker_8_512_sq(UCHAR *cdata, UCHAR *bstring)
{
	const int n = 512, m = 512;
	int block_pattern[B*B], c = 0, i = 0;
	
	//Creación del vector block_pattern. Descripción arriba.
	for (int i = 0; i < 8; i++)
		for (int j = i*n; j - i*n < 8; j++)		
			block_pattern[c++] = j;

	//Obtención del vector de datos ordenados en bloques de 8x8
	for (int i = 0; i < (m / B); i++)
		for (int j = 0; j < (n / B); j++)
			for (int k = 0; k < B*B; k++)
				bstring[k + j*(B*B) + i*(n / B)*(B*B)] = cdata[block_pattern[k] + B*j + (n / B)*B*B*i];
	
	//Devuelve el número de bloques resultante
	return (m / B)*(m / B);
}


/*
Function : imager_8_512_sq
--------------------------

Generará la imagen a partir de un vector con los bloques de 8x8 valores.
Para ello es necesario crear el vector block_pattern para conocer que posiciones coger del vector
de entrada para conseguir el orden correcto en la imagen final.
block_pattern = [0, 1, ..., 7, 512, 513, ..., 519, 1024, 1025, ..., 1031,..., 3583, 3584, ..., 3590]

bstring:	vector de 512x512 correspondiente a los bloques de 8x8 píxeles.
Primeros 64 valores corresponden el primer bloque de 8x8
Segundos 64 valores corresponden al segundo bloque de 8x8
Etc.
cdata:	vector de 512x512 valores en los que los píxeles estarán ordenados para obtener una imagen de 512x512.
Primeros 512 valores corresponden a la primera fila de la imagen
Segundos 512 valores corresponden a la segunda fila de la imagen
Etc.

returns:	Número de bloques resultantes.

*/
int imager_8_512_sq(UCHAR *bstring, UCHAR *cdata)
{
	const int n = 512, m = 512;
	int block_pattern[B*B], c = 0;

	//Creación del vector block_pattern. Descripción arriba.
	for (int i = 0; i < 8; i++)
		for (int j = i*n; j - i*n < 8; j++)		
			block_pattern[c++] = j;

	//Obtención de la imagen de 512x512 píxeles
	for (int i = 0; i < (m / B); i++)
		for (int j = 0; j < (n / B); j++)
			for (int k = 0; k < B*B; k++)
				cdata[block_pattern[k] + B*j + (n / B)*B*B*i] = bstring[k + j*(B*B) + i*(n / B)*(B*B)];

	//Devuelve el número de bloques resultante
	return (m / B)*(m / B);
}


/*
Function : copy_F_U
-------------------

Copia los valores de formato unsigned char del vector del primer argumento en el vector del segundo argumento
con formato float

copied:	puntero a los datos que se quieren copiar
copy:	puntero a los datos donde se quiere copiar
d:	número de datos que se quieren copiar

*/
void copy_F_U(UCHAR *copied, float *copy, int d)
{
	for (int i = 0; i < d; i++)
		copy[i] = (float)(copied[i]);
}


/*
Function : copy_U_F
-------------------

Copia los valores de formato float del vector del primer argumento en el vector del segundo argumento
con formato unsigned char

copied:	puntero a los datos que se quieren copiar
copy:	puntero a los datos donde se quiere copiar
d:	número de datos que se quieren copiar

*/
void copy_U_F(float *copied, UCHAR *copy, int d)
{
	for (int i = 0; i < d; i++)
		copy[i] = (UCHAR)(copied[i]);
}



/*
Function : copy_F
-----------------

Copia los valores de formato float del vector del primer argumento en el vector del segundo argumento

copied:	puntero a los datos que se quieren copiar
copy:	puntero a los datos donde se quiere copiar
d:	número de datos que se quieren copiar

*/
void copy_F(float *copied, float *copy, int d)
{
	for (int i = 0; i < d; i++)
		copy[i] = copied[i];
}


/*
Function : block_mask_8_512_sq
------------------------------

Creará la máscara para quedarnos con los valores deseados tras la DCT. Esta máscara será un vector
de 8x8 valores que tendrá los 1s deseados en la esquina superior izquierda y 0s en las posiciones
restantes. Ejemplo, si d es 5:
mask =	1 1 1 1 1 0 0 0
		1 1 1 1 0 0 0 0
		1 1 1 0 0 0 0 0
		1 1 0 0 0 0 0 0
		1 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0

mask:	puntero al vector donde guardaremos los valores de la máscara
d:	número diagonales de 1s deseado. Valor máximo 8.

*/
void block_mask_8_512_sq(UCHAR *mask, int d)
{
	int c = 0;

	for (int i = 0; i < B; i++)
		for (int j = 0; j < B; j++)
			if (d - i > j)
				mask[c++] = 1;
			else
				mask[c++] = 0;
}


// TEMP /////////////////////////////////////////////////////////////////////////////////////////////////

	double vectorResult[B*B];
	double vectorMask[B*B];
	double vectorTrans[B*B];
	double mask[B*B];


/*
Function : AxB_F
----------------

Multiplicación matricial de valores float M1xM2

M1:	vector de 8x8 valores que será el primer operando de la multiplicación matricial
M2: vector de 8x8 valores que será el segundo operando de la multiplicación matricial

returns:	puntero para permitir llamadas recursivas.

*/
float * AxB_F(float M1[B * B], const float M2[B * B])
{
	float TMP[B * B];
	for (int k = 0; k < B; k++) {
		for (int i = 0; i < B; i++) {
			TMP[i + k*B] = 0;
			for (int j = 0; j < B; j++) {
				TMP[k*B + i] += ((float)M1[j + k*B]) * M2[i + j*B];
			}
		}
	}
	//Copiamos los valores obtenidos en TMP
	copy_F(TMP, M1, B * B);
	//Devuelve el puntero para permitir llamadas recursivas
	return M1; 
}


/*
Function : BxA_F
----------------

Multiplicación matricial de valores float M2xM1

M1:	vector de 8x8 valores que será el primer operando de la multiplicación matricial
M2: vector de 8x8 valores que será el segundo operando de la multiplicación matricial

returns:	puntero para permitir llamadas recursivas.

*/
float * BxA_F(float M1[B * B], const float M2[B * B])
{
	float TMP[B * B];
	for (int k = 0; k < B; k++) {
		for (int i = 0; i < B; i++) {
			TMP[i + k*B] = 0;
			for (int j = 0; j < B; j++) {
				TMP[k*B + i] += (M2[j + k*B]) * M1[i + j*B];
			}
		}
	}
	//Copiamos los valores obtenidos en TMP
	copy_F(TMP, M1, B * B);
	//Devuelve el puntero para permitir llamadas recursivas
	return M1; 
}


/*
Function : QnD_TATt
-------------------

Bloque encargado de realizar la DCT sobre los valores de entrada.
Tendrá como entrada bloques de 8x8 valores float y los guardará en el mismo vector

A:	vector de 8x8 valores en formato float sobre los que vamos a realizar la DCT

*/
void QnD_TATt(float A[B * B])
{
	BxA_F(AxB_F(A, Tt), T);
}


/*
Function : QnD_TtAT
-------------------

Bloque encargado de realizar la IDCT sobre los valores de entrada.
Tendrá como entrada bloques de 8x8 valores float y los guardará en el mismo vector

A:	vector de 8x8 valores en formato float sobre los que vamos a realizar la IDCT

*/
void QnD_TtAT(float A[B * B])
{
	BxA_F(AxB_F(A, T), Tt);
}


/*
Function : Mask
-------------------

Aplicará la máscara creada al bloque de 8x8 de entrada. De esta forma, nos quedaremos con únicamente los valores
deseados de la DCT.

A:	vector de 8x8 valores en formato float sobre los que vamos a aplicar la máscara
mask:	vector de 8x8 valores que corresponderá con la máscara

*/
void Mask(float A[B*B], UCHAR mask[B*B]) {
	for (int p = 0; p < B*B; p++)
		A[p] *= mask[p];
}