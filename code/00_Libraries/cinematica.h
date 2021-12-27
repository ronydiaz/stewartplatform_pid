/*
 * cinematica.h
 *
 * Created: 13/12/2020 01:44:06 p. m.
 *  Author: rony_
 */ 


#ifndef CINEMATICA_H_
#define CINEMATICA_H_

#include <math.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////	Funcion para convertir grados a radianes y viceversa	///////////////////////////////////////
#define PI 3.14159265359
#ifndef degToRad
#define degToRad(degree) (degree * PI / 180.0)
#endif
#ifndef radToDeg
#define radToDeg(radians) ((radians * 180.0) / PI)
#endif
//////////////////////////////////////////////	Posiciones angulares de los servomotores en la base		///////////////////////////////////////
float Mu1=  degToRad(19.2);
float Mu2=  degToRad(100.8);
float Mu3=  degToRad(139.2);
float Mu4=  degToRad(-139.2);         //220.8°
float Mu5=  degToRad(-100.8);         //259.2°
float Mu6=  degToRad(-19.2);          //340.8°
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////	MODIFICABLE DE ACUERDO A LAS CARACTERISTICAS DE LA PLATAFORMA	//////////////////////////////
///////////////////////////////////////////////	Medidas de la plataforma Stewart-Gough[cm]	/////////////////////////////////////////////////
//-	-	-	-	-	-	-	-	-	-	-	-	En esta parte se definen las caracteristicas de la plataforma	-	-	-	-	-	-	-	-
#define Rb 9.109  //Radio de la base
#define Rp 8      //Radio de la plataforma
#define Cs 2.4    //Cuerno del servomotor
#define Cs2 4.8   //Doble del cuerno del servomotor
#define Cs_2 5.76 //Cs^2
#define L 16.5    //Longitud del eslabon
#define H 16.2    //Altura inicial de la plataformafloat

char S1[15],S2[15],S3[15],S4[15],S5[15],S6[15];
//////////////////////////////////////////////	Declaracion de POSICION y ORIENTACION del efector final	/////////////////////////////////////
//	-	-	-	-	-	-	-	-	-	-	-	En esta seccion se definen los valores inciales de roll(alfa), pitch(beta) y yaw(gamma)	-	-
// float alfa=0;		//Roll  De -15 a +15
// float beta=0;		//Pitch
// float gamma=0;      //Yaw
// float Alfa = degToRad(alfa);  //Son los mismos valores de Roll, Pitch y Yaw
// float Beta = degToRad(beta);  //pero convertidos a radianes para procesarlos
// float Gamma = degToRad(gamma); //en la matriz de rotacion
// //	-	-	-	-	-	-	-	-	-	-	-	En esta seccion se definen los valores inciales de posicion (X, Y, Z)	-	-	-	-	-	-
// float x=0;
// float y=0;
// float z=0;     //Altura deseada  De -2.6 a 1.99 respecto a sistema móvil de coordenadas {P} y con roll, pitch and yaw = (0,0,0)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int i,j,k;    //Variables para realizar operaciones con matrices


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////	EMPIEZA EL MERJURGE	////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------	POSICIÓN DE PUNTOS {Pi}		--------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------
// -	-	-	-	-	-	-	-	-	-	-	-	-	Matriz de traslación de origen Oxyz a Ouvw	-	-	-	-	-	-	-	-	-	-
float b_T_Op[4][4]={};
float R[4][4]={};

void setTR(float x, float y, float z, float alfa, float beta, float gamma){
	b_T_Op[0][0]= 1;
	b_T_Op[0][1]= 0;
	b_T_Op[0][2]= 0;
	b_T_Op[0][3]= x;
	
	b_T_Op[1][0]= 0;
	b_T_Op[1][1]= 1;
	b_T_Op[1][2]= 0;
	b_T_Op[1][3]= y;

	b_T_Op[2][0]= 0;
	b_T_Op[2][1]= 0;
	b_T_Op[2][2]= 1;
	b_T_Op[2][3]= H+z;

	b_T_Op[3][0]= 0;
	b_T_Op[3][1]= 0;
	b_T_Op[3][2]= 0;
	b_T_Op[3][3]= 1;

	float Alfa = degToRad(alfa);  //Son los mismos valores de Roll, Pitch y Yaw
	float Beta = degToRad(beta);  //pero convertidos a radianes para procesarlos
	float Gamma = degToRad(gamma); //en la matriz de rotacion

	R[0][0]= cos(Gamma)*cos(Beta);
	R[0][1]= -sin(Gamma)*cos(Alfa)+cos(Gamma)*sin(Beta)*sin(Alfa);
	R[0][2]= sin(Gamma)*sin(Alfa)+cos(Gamma)*sin(Beta)*cos(Alfa);
	R[0][3]= 0;

	R[1][0]= sin(Gamma)*cos(Beta);
	R[1][1]= cos(Gamma)*cos(Alfa)+sin(Gamma)*sin(Beta)*sin(Alfa);
	R[1][2]= -cos(Gamma)*sin(Alfa)+sin(Gamma)*sin(Beta)*cos(Alfa);
	R[1][3]= 0;

	R[2][0]= -sin(Beta);
	R[2][1]= cos(Beta)*sin(Alfa);
	R[2][2]= cos(Beta)*cos(Alfa);
	R[2][3]= 0;

	R[3][0]= 0;
	R[3][1]= 0;
	R[3][2]= 0;
	R[3][3]= 1;
}

//	-	-	-	-	-	-	-	-	-	-	-	-	-	Matriz de posición y orientación de sistema {P} respecto a {B}	-	-	-	-	-	-	-	-	-
float bTRp[4][4]={};
void multiplicarTR(){								//	Esta matriz multiplica la matriz de traslacion y la de rotacion obteniendo bTRp
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			bTRp[i][j]=0;
			for(k=0 ; k<=3 ; k++){
			bTRp[i][j] += b_T_Op[i][k]*R[k][j];}}}
		}

//	-	-	-	-	-	-	-	-	-	-	-	-	-	Posición angular de puntos Pi (rótulas superiores)	-	-	-	-	-	-	-	-	-	-	-	-
//	-	-	-	-	-	-	-	-	-	-	-	-	-	respecto marco {P} de coordenadas	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-
float Theta1= degToRad(51.24);
float Theta2= degToRad(68.77);
float Theta3= degToRad(171.24);
float Theta4= degToRad(-171.24);    //188.76
float Theta5= degToRad(-68.77);     //291.23
float Theta6= degToRad(-51.24);     //308.75

float P1[4][4]={{ 1 , 0 , 0 , Rp*cos(Theta1) },
				{ 0 , 1 , 0 , Rp*sin(Theta1) },
				{ 0 , 0 , 1 ,        0       },
				{ 0 , 0 , 0 ,        1       }};

float P2[4][4]={{ 1 , 0 , 0 , Rp*cos(Theta2) },
				{ 0 , 1 , 0 , Rp*sin(Theta2) },
				{ 0 , 0 , 1 ,        0       },
				{ 0 , 0 , 0 ,        1       }};

float P3[4][4]={{ 1 , 0 , 0 , Rp*cos(Theta3) },
				{ 0 , 1 , 0 , Rp*sin(Theta3) },
				{ 0 , 0 , 1 ,        0       },
				{ 0 , 0 , 0 ,        1       }};

float P4[4][4]={{ 1 , 0 , 0 , Rp*cos(Theta4) },
				{ 0 , 1 , 0 , Rp*sin(Theta4) },
				{ 0 , 0 , 1 ,        0       },
				{ 0 , 0 , 0 ,        1       }};

float P5[4][4]={{ 1 , 0 , 0 , Rp*cos(Theta5) },
				{ 0 , 1 , 0 , Rp*sin(Theta5) },
				{ 0 , 0 , 1 ,        0       },
				{ 0 , 0 , 0 ,        1       }};

float P6[4][4]={{ 1 , 0 , 0 , Rp*cos(Theta6)},
				{ 0 , 1 , 0 , Rp*sin(Theta6)},
				{ 0 , 0 , 1 ,        0      },
				{ 0 , 0 , 0 ,        1      }};

//	-	-	-	-	-	-	-	-	-	-	-	-	-	Posición de las rótulas superiores	-	-	-	-	-	-	-	-	-	-	-	-	-	-
float b_T_p1[4][4]={};
float b_T_p2[4][4]={};
float b_T_p3[4][4]={};
float b_T_p4[4][4]={};
float b_T_p5[4][4]={};
float b_T_p6[4][4]={};

void posicionRS(){									//	Multiplicacion para determinar la posicion de las rotulas superiores	-	-	-	-	-
	multiplicarTR();
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			b_T_p1[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				b_T_p1[i][j] += bTRp[i][k]*P1[k][j];}}}
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			b_T_p2[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				b_T_p2[i][j] += bTRp[i][k]*P2[k][j];}}}
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			b_T_p3[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				b_T_p3[i][j] += bTRp[i][k]*P3[k][j];}}}
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			b_T_p4[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				b_T_p4[i][j] += bTRp[i][k]*P4[k][j];}}}
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			b_T_p5[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				b_T_p5[i][j] += bTRp[i][k]*P5[k][j];}}}
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			b_T_p6[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				b_T_p6[i][j] += bTRp[i][k]*P6[k][j];}}}
}

//	-	-	-	-	-	-	-	-	-	-	-	-	-	-	Invertida de Matrices de transformacion de posicion	-	-	-	-	-	-	-	-	-	-	-
float inv_B1[4][4]={{   0    ,     0   , 1 ,  -0   },
					{-0.3289 ,  0.9444 , 0 ,   0   },
					{-0.9444 , -0.3289 , 0 , 9.1090},
					{   0    ,    0    , 0 ,   1   }};

float inv_B2[4][4]={{0.9823 ,  0.1874 , 0 ,   -0  },
					{  -0   ,    0    , 1 ,   -0  },
					{0.1874 , -0.9823 , 0 , 9.1090},
					{   0   ,    0    , 0 ,    1  }};

float inv_B3[4][4]={{   -0   ,    0    , 1 ,   -0  },
					{-0.6534 , -0.7570 , 0 ,    0  },
					{0.7570  , -0.6534 , 0 , 9.1090},
					{   0    ,    0    , 0 ,    1  }};

float inv_B4[4][4]={{-0.6534 , 0.7570 , 0 ,   0   },
					{   -0   ,   -0   , 1 ,  -0   },
					{ 0.7570 , 0.6534 , 0 , 9.1090},
					{   0    ,    0   , 0 ,   1   }};

float inv_B5[4][4]={{  -0   ,     0   , 1 ,   0   },
					{0.9823 , -0.1874 , 0 ,   0   },
					{0.1874 ,  0.9823 , 0 , 9.1090},
					{   0   ,     0   , 0 ,   1   }};

float inv_B6[4][4]={{-0.3289 , -0.9444 , -0 ,   -0  },
					{   0    ,    -0   ,  1 ,   -0  },
					{-0.9444 ,  0.3289 ,  0 , 9.1090},
					{   0    ,     0   ,  0 ,    1  }};

//	-	-	-	-	-	-	-	-	-	-	-	-	-	-	Posición de las rótulas superiores vector	-	-	-	-	-	-	-	-	-	-	-
float Pos_P1[4][1]={};
float Pos_P2[4][1]={};
float Pos_P3[4][1]={};
float Pos_P4[4][1]={};
float Pos_P5[4][1]={};
float Pos_P6[4][1]={};
float v_identidad[4][1]={{0},{0},{0},{1}};

void posicionPi(){										//	Multiplicacion para determinar x[i], y[i], z[i] de Pos_P[i]
	posicionRS();
	float Pos_P1A[4][4]={};								//	Matrices para almacenar la primer multiplicacion de Pos_P[i]
	float Pos_P2A[4][4]={};
	float Pos_P3A[4][4]={};
	float Pos_P4A[4][4]={};
	float Pos_P5A[4][4]={};
	float Pos_P6A[4][4]={};

	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			Pos_P1A[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P1A[i][j] += inv_B1[i][k]*b_T_p1[k][j];}}}
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=0 ; j++){
			Pos_P1[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P1[i][j] += Pos_P1A[i][k]*v_identidad[k][j];}}}
//------------------------------------------------------------
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			Pos_P2A[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P2A[i][j] += inv_B2[i][k]*b_T_p2[k][j];}}}
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=0 ; j++){
			Pos_P2[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P2[i][j] += Pos_P2A[i][k]*v_identidad[k][j];}}}
//------------------------------------------------------------
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			Pos_P3A[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P3A[i][j] += inv_B3[i][k]*b_T_p3[k][j];}}}
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=0 ; j++){
			Pos_P3[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P3[i][j] += Pos_P3A[i][k]*v_identidad[k][j];}}}
//------------------------------------------------------------
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			Pos_P4A[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P4A[i][j] += inv_B4[i][k]*b_T_p4[k][j];}}}
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=0 ; j++){
			Pos_P4[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P4[i][j] += Pos_P4A[i][k]*v_identidad[k][j];}}}
//------------------------------------------------------------
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			Pos_P5A[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P5A[i][j] += inv_B5[i][k]*b_T_p5[k][j];}}}
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=0 ; j++){
			Pos_P5[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P5[i][j] += Pos_P5A[i][k]*v_identidad[k][j];}}}
//------------------------------------------------------------
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=3 ; j++){
			Pos_P6A[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P6A[i][j] += inv_B6[i][k]*b_T_p6[k][j];}}}
	for(i=0 ; i<=3 ; i++){
		for(j=0 ; j<=0 ; j++){
			Pos_P6[i][j]=0;
			for(k=0 ; k<=3 ; k++){
				Pos_P6[i][j] += Pos_P6A[i][k]*v_identidad[k][j];}}}
}

//	-	-	-	-	-	-	-	-	-	-	-	-	-	Calcular sigma (angulo de giro de servos)	-	-	-	-	-	-	-	-	-	-	-	-
float sigma1 = 0;
float sigma2 = 0;
float sigma3 = 0;
float sigma4 = 0;
float sigma5 = 0;
float sigma6 = 0;

void sigma(){
	float x1 = Pos_P1[0][0];
	float y1 = Pos_P1[1][0];
	float z1 = Pos_P1[2][0];

	float x2 = Pos_P2[0][0];
	float y2 = Pos_P2[1][0];
	float z2 = Pos_P2[2][0];

	float x3 = Pos_P3[0][0];
	float y3 = Pos_P3[1][0];
	float z3 = Pos_P3[2][0];

	float x4 = Pos_P4[0][0];
	float y4 = Pos_P4[1][0];
	float z4 = Pos_P4[2][0];

	float x5 = Pos_P5[0][0];
	float y5 = Pos_P5[1][0];
	float z5 = Pos_P5[2][0];

	float x6 = Pos_P6[0][0];
	float y6 = Pos_P6[1][0];
	float z6 = Pos_P6[2][0];

	//	-	-	-	-	-	-	-	-	-	SIGMA 1	-	-	-	-	-	-	-	-	-	-	-	-	-	-
	float SIGMA1 = radToDeg(atan(x1/y1));
	float delta1 = radToDeg(acos((-266.49+pow(x1,2)+pow(y1,2)+pow(z1,2)) / (4.8*(sqrt(pow(x1,2)+y1)))));
	sigma1 = SIGMA1-delta1;

	//	-	-	-	-	-	-	-	-	-	SIGMA 2	-	-	-	-	-	-	-	-	-	-	-	-	-	-
	float SIGMA2 = radToDeg(atan(y2/x2));
	float delta2 = radToDeg(acos((pow(x2,2)+pow(y2,2)+pow(z2,2)-266.49) / (4.8*(sqrt(pow(x2,2)+pow(y2,2))))));
	sigma2 = SIGMA2-delta2;

	//	-	-	-	-	-	-	-	-	-	SIGMA 3	-	-	-	-	-	-	-	-	-	-	-	-	-	-
	float SIGMA3 = radToDeg(atan(x3/y3));
	float delta3 = radToDeg(acos((-266.49+pow(x3,2)+pow(y3,2)+pow(z3,2)) / (4.8*(sqrt(pow(x3,2)+y1)))));
	sigma3 = SIGMA3-delta3;

	//	-	-	-	-	-	-	-	-	-	SIGMA 4	-	-	-	-	-	-	-	-	-	-	-	-	-	-
	float SIGMA4 = radToDeg(atan(y4/x4));
	float delta4 = radToDeg(acos((pow(x4,2)+pow(y4,2)+pow(z4,2)-266.49) / (4.8*(sqrt(pow(x4,2)+pow(y4,2))))));
	sigma4 = SIGMA4-delta4;

	//	-	-	-	-	-	-	-	-	-	SIGMA 5	-	-	-	-	-	-	-	-	-	-	-	-	-	-
	float SIGMA5 = radToDeg(atan(x5/y5));
	float delta5 = radToDeg(acos((-266.49+pow(x5,2)+pow(y5,2)+pow(z5,2)) / (4.8*(sqrt(pow(x5,2)+y1)))));
	sigma5 = SIGMA5-delta5;

	//	-	-	-	-	-	-	-	-	-	SIGMA 6	-	-	-	-	-	-	-	-	-	-	-	-	-	-
	float SIGMA6 = radToDeg(atan(y6/x6));
	float delta6 = radToDeg(acos((pow(x6,2)+pow(y6,2)+pow(z6,2)-266.49) / (4.8*(sqrt(pow(x6,2)+pow(y6,2))))));
	sigma6 = SIGMA6-delta6;
}

void cinematica_inversa(float x, float y, float z, float alfa, float beta, float gamma){
	setTR(x,y,z,alfa,beta,gamma);
	multiplicarTR();
	posicionRS();
	posicionPi();
	sigma();
	dtostrf(sigma1,2,4,S1);
	dtostrf(sigma2,2,4,S2);
	dtostrf(sigma3,2,4,S3);
	dtostrf(sigma4,2,4,S4);
	dtostrf(sigma5,2,4,S5);
	dtostrf(sigma6,2,4,S6);
}
#endif /* CINEMATICA_H_ */