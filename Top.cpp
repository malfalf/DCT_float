/*
Código creado por Carlos Pastor Ramírez y Miguel Ángel Alfonso Alfonso
para la asignatura Lenguajes y Herramientas de Diseño Digital.

Este código se utilizará en la obtención de la DCT y la IDCT de 8x8 valores
de una imagen de entrada conocida de 512x512 pixeles (lena.bmp). Para ello,
se utilizarán 4 grandes bloques:
- blocker -> ordena los píxeles de la imagen en bloques de 8x8.
- QnD_TATt -> realizará la DCT de los bloques 8x8 que se le vayan pasando como argumento
- QnD_TtAT -> realizará la DCT inversa de los bloques 8x8 que se le vayan pasando como argumento
- imager -> función inversa a blocker. Colocará los píxeles para una correcta visualización de la imagen.

En este código se utilizarán variables con formato entero y flotante para intentar
perder la mínima precisión posible.

Asimismo, cabe destacar que se hará uso de la librería qdbmp, no creada por nosotros.

Curso: 2016-2017

*/


#include <stdio.h>
#include <math.h>
#include "qdbmp.h"
#include "Tasks.h"


//Variables globales
BMP*    bmp;
UCHAR   r, g, b;
UINT    width, height;
UINT    h, w, s;
//--

//Función que nos permitirá representar las imágenes obtenidas en diferentes puntos del código
int debug_outputBPM(UCHAR * image_tonal_data, const char * app);

//Vectores donde guardaremos los valores de los píxeles en diferentes puntos del código
//Estos vectores están pensados para una imagen de 512x512 píxeles
UCHAR image_tonal_data_bss[512 * 512];			//8bit...+250kByte
//UCHAR image_tonal_data_bss_ini[512 * 512];	//8bit...+250kByte
UCHAR image_tonal_data_blk[512 * 512];			//8bit...+250kByte
float image_tonal_data_csn[512 * 512];			//8bit...+250kByte


//Función main
int main(int argc, char* argv[])
{
	
	char buff[100];
	sprintf(buff, "res/lena512.bmp");		//target img

	UCHAR cmp_mask[64];							//Mask vetor (pre-generated?)

	int nblocks = 0;

	// Obtenemos la imagen y valores particulares/ 

	//Almacenamos la imagen en bmp/
	bmp = BMP_ReadFile(buff);
	//Se notifica si se produce un error
	BMP_CHECK_ERROR(stderr, -1);
	//Obtenemos las dimensiones de la imagen
	width = BMP_GetWidth(bmp);
	height = BMP_GetHeight(bmp);


	//Después de este bucle, tenemos en image_tonal_data_bss los pixels de la imagen colocados en un VECTOR de 512*512 valores
	for (h = 0; h < height; ++h)
	{
		for (w = 0; w < width; ++w)
		{
			//Obtenemos los valores de los píxeles
			BMP_GetPixelIndex(bmp, w, h, &image_tonal_data_bss[w + width*h]);
			//BMP_GetPixelIndex(bmp, w, h, &image_tonal_data_bss_ini[w + width*h]);

		}
	}


	// Obtenemos los bloques de 8x8 píxeles

	//Para entender el funcionamiento de la función blocker_8_512_sq ver descripción en task.cpp
	//Tendremos en image_tonal_data_blk los valores ordenador por bloques 8x8
	//nblocks indicará el número de bloques 8x8 que tiene la imagen
	nblocks = blocker_8_512_sq(image_tonal_data_bss, image_tonal_data_blk);
	
	//En image_total_data_csn tienes los valores de image_tonal_data_blk transformados en float
	copy_F_U(image_tonal_data_blk, image_tonal_data_csn, 512*512);
	

	//Para entender el funcionamiento de la función block_mask_8_512_sq ver descripción en task.cpp
	//Generamos la máscara. En este caso nos quedamos con los 6 valores de la esquina superior izquierda
	block_mask_8_512_sq(cmp_mask, 3);

	//Vas haciendo la DCT de todos los bloques 8x8
	//Para entender el funcionamiento de la función QnD_TATt ver descripción en task.cpp
	for (int n = 0; n < nblocks; n++)
	{
		QnD_TATt(&image_tonal_data_csn[n * B * B]);
	}

	//Aplicas la máscara a los bloques anteriores con la transformación
	for (int n = 0; n < nblocks; n++)
	{
		Mask(&image_tonal_data_csn[n * B * B], cmp_mask);
	}


	//Aplicamos la IDCT
	//Para entender el funcionamiento de la función QnD_TtAT ver descripción en task.cpp
	for (int n = 0; n < nblocks; n++)
	{
		QnD_TtAT(&image_tonal_data_csn[n * B * B]);
	}

	//Los siguientes pasos son para obtener una imagen para comprobar que todo ha ido bien
	//Transformamos de valores tipo float a valores tipo unsigned char para tenerlos en el rango 0-255
	copy_U_F(image_tonal_data_csn, image_tonal_data_blk, 512 * 512);
	//Colocamos los bloques 8x8 en su posición final para una imagen correcta
	nblocks = imager_8_512_sq(image_tonal_data_blk, image_tonal_data_bss);
	//Creamos una imagen para visualizar los resultados
	debug_outputBPM(image_tonal_data_bss, "test");
		
#if 0
	Esta parte es solo para mostrar diferentes resultados que se han ido obteniendo durante la simulación
	El siguiente bucle es para ver la diferencia entre la imagen de salida y la original
	El if es porque si con UCHAR restas un valor a otro que es menor, el resultado es 255 y falsea la prueba.
	for (int k = 0; k < 512 * 512; k++) {
		if (image_tonal_data_bss_ini[k] > image_tonal_data_bss[k]) image_tonal_data_bss_ini[k] -= image_tonal_data_bss[k];
		else image_tonal_data_bss_ini[k] = image_tonal_data_bss[k] - image_tonal_data_bss_ini[k];
	}

	debug_outputBPM(image_tonal_data_bss_ini, "diffMaskTodo1");

#endif

	//Liberamos la memoria reservada para la imagen
	BMP_Free(bmp);

	//Si todo ha ido bien devolvemos un 0
	return 0;
}


/*
Function : debug_outputBPM
--------------------------

Creará una imagen a partir del vector de entrada que se le pase.

image_tonal_data: vector de 256x256 que queremos transformar en imagen.
app:apéndice que se le pondrá al nombre de la imagen para distinguirla de otras.

returns: 0 si todo ha ido bien o -2 en caso de error.

*/
int debug_outputBPM(UCHAR * image_tonal_data, const char * app)
{

	for (h = 0; h < height; ++h)
	{
		for (w = 0; w < width; ++w)
		{
			// Activas los valores de los píxeles
			BMP_SetPixelIndex(bmp, w, h, image_tonal_data[w + width*h]);

		}
	}

	//Creas la imagen y compruebas que todo OK
	char buff[50];
	sprintf(buff, "res/lena512_%s.bmp", app);
	BMP_WriteFile(bmp, buff);
	BMP_CHECK_ERROR(stderr, -2);
	return 0;

}