/*
Código creado por Carlos Pastor Ramírez y Miguel Ángel Alfonso Alfonso
para la asignatura Lenguajes y Herramientas de Diseño Digital.

Header file de Task.cpp

Curso: 2016-2017

*/


#ifndef _TSK_H_
#define _TSK_H_

#include <math.h>
#include "qdbmp.h"

#define B 8  //tamaño de los bloques

//Valores de la matriz 8x8 de la DCT y de la IDCT
const float T[64] = { 0.3536F, 0.3536F, 0.3536F, 0.3536F, 0.3536F, 0.3536F, 0.3536F, 0.3536F,
0.4904F, 0.4157F, 0.2778F, 0.0975F,-0.0975F,-0.2778F,-0.4157F,-0.4904F,
0.4619F, 0.1913F,-0.1913F,-0.4619F,-0.4619F,-0.1913F, 0.1913F, 0.4619F,
0.4157F,-0.0975F,-0.4904F,-0.2778F, 0.2778F, 0.4904F, 0.0975F,-0.4157F,
0.3536F,-0.3536F,-0.3536F, 0.3536F, 0.3536F,-0.3536F,-0.3536F, 0.3536F,
0.2778F,-0.4904F, 0.0975F, 0.4157F,-0.4157F,-0.0975F, 0.4904F,-0.2778F,
0.1913F,-0.4619F, 0.4619F,-0.1913F,-0.1913F, 0.4619F,-0.4916F, 0.1913F,
0.0975F,-0.2778F, 0.4157F,-0.4904F, 0.4904F,-0.4157F, 0.2778F,-0.0975F };

const float Tt[64] = { 0.3536F, 0.4904F, 0.4619F, 0.4157F, 0.3536F, 0.2778F, 0.1913F, 0.0975F,
0.3536F, 0.4157F, 0.1913F,-0.0975F,-0.3536F,-0.4904F,-0.4619F,-0.2778F,
0.3536F, 0.2778F,-0.1913F,-0.4904F,-0.3536F, 0.0975F, 0.4619F, 0.4157F,
0.3536F, 0.0975F,-0.4619F,-0.2778F, 0.3536F, 0.4157F,-0.1913F,-0.4904F,
0.3536F,-0.0975F,-0.4619F, 0.2778F, 0.3536F,-0.4157F,-0.1913F, 0.4904F,
0.3536F,-0.2778F,-0.1913F, 0.4904F,-0.3536F,-0.0975F, 0.4619F,-0.4157F,
0.3536F,-0.4157F, 0.1913F, 0.0975F,-0.3536F, 0.4904F,-0.4619F, 0.2778F,
0.3536F,-0.4904F, 0.4619F,-0.4157F, 0.3536F,-0.2778F, 0.1913F,-0.0975F };

#if 0
//Matrices usadas para varios ejemplos que usa JPEG para la compresión. En este código no se utiliza
const int Qmatrix50[64] = {
	16, 11, 10, 16, 24, 40, 51, 61,
	12, 12, 14, 19, 26, 58, 60, 55,
	14, 13, 16, 24, 40, 57, 69, 56,
	14, 17, 22, 29, 51, 87, 80, 62,
	18, 22, 37, 56, 68, 109, 103, 77,
	24, 35, 55, 64, 81, 104, 113, 92,
	49, 64, 78, 87, 103, 121, 120, 101,
	72, 92, 95, 98, 112, 100, 103, 99
};

const int Qmatrix10[64] = {
	80, 60, 50, 80, 120, 200, 255, 255,
	55, 60, 70, 95, 130, 255, 255, 255,
	70, 65, 80, 120, 200, 255, 255, 255,
	70, 85, 110, 145, 255, 255, 255, 255,
	90, 110, 185, 255, 255, 255, 255, 255,
	120, 175, 255, 255, 255, 255, 255, 255,
	245, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255
};
#endif


//Funciones para obtener los bloques 8x8 a partir de la imagen y de la imagen a partir de bloques 8x8
int blocker_8_512_sq(UCHAR *cdata, UCHAR *bstring);
int imager_8_512_sq(UCHAR *bstring, UCHAR *cdata);

//Funciones para realizar la DCT y la IDCT
void QnD_TATt(float A[B * B]);
void QnD_TtAT(float A[B * B]);

//Funciones de transformación de tipos de variables
void copy_F_U(UCHAR *copied, float *copy, int d);
void copy_U_F(float *copied, UCHAR *copy, int d);

//Operación para copia temporal de valores float
void copy_F(float *copied, float *copy, int d);

//Función para crear la máscara
void block_mask_8_512_sq(UCHAR *mask, int d);

//Operaciones con matrices
float * AxB_F(float M1[B * B], const float M2[B * B]);
float * BxA_F(float M1[B * B], const float M2[B * B]);
void Mask(float A[B*B], UCHAR mask[B*B]);

#endif