// ConversorDeColores.cpp : Defines the entry point for the console application.
//
#include "stdafx.h";
#include <iostream>
#include <string>
using namespace std;
//Uso de estructuras solo para cuestiones de practica
struct RGB {
	int R, G, B;
};
struct MYCK
{
	float M, Y, C, K;
};
struct MYC
{
	float M, Y, C;
};
struct Lab
{
	//L* axis(lightness) ranges from 0 to 100
	//a* and b* (color attributes) axis range from - 128 to + 127

	int L, a, b;
};
struct HSB
{
	//H 0 a 360
	// S y B de 0 a 100
	int H, S, B;
};

struct XYZ 
{
	float X, Y, Z;
};

//Declaracion de la clase convertidor
class Convertidor {
	private:
		RGB *rgb;
		MYCK *myck;
		MYC *myc;
		Lab *lab;
		HSB *hsb;
		XYZ *xyz;
		double srgb[3][3] = {
			{ 0.4124564 , 0.3575761 , 0.1804375 },
			{ 0.2126729 , 0.7151522 , 0.0721750 },
			{ 0.0193339 , 0.1191920 , 0.9503041 }
		};
		double xn = 95.047;
		double yn = 100;
		double zn = 108.883;

	public:
		Convertidor() {
			rgb = new RGB;
			myck = new MYCK;
			myc = new MYC;
			lab = new Lab;
			hsb = new HSB;
			xyz = new XYZ;
		}
		void eliminador() {
			delete rgb;
			delete myck;
			delete myc;
			delete lab;
			delete hsb;
			delete xyz;
		}
		void rgb2myck() {
			float rprima = (float)rgb->R / (float)2.55;
			float gprima = (float)rgb->G / (float)2.55;
			float bprima = (float)rgb->B / (float)2.55;
			float max;
			if (rprima > gprima && rprima > bprima) {
				max = rprima;
			}
			else if (gprima > rprima && gprima > bprima) {
				max = gprima;
			}
			else {
				max = bprima;
			}
			myck->K = round(100 - max);
			myck->C = round(((100 - rprima - myck->K) / (100 - myck->K))*100);
			myck->M = round(((100 - gprima - myck->K) / (100 - myck->K))*100);
			myck->Y = round(((100 - bprima - myck->K) / (100 - myck->K))*100);
			cout << "C:" << myck->C << " M:" << myck->M << " Y:" << myck->Y << " K:" << myck->K << endl;
		}
		void rgb2myc() {
			myc->C = round((float)(255 - rgb->R) / (float)2.55);
			myc->M = round((float)(255 - rgb->G) / (float)2.55);
			myc->Y = round((float)(255 - rgb->B) / (float)2.55);
			cout << "C:" << myc->C << " M:" << myc->M << " Y:" << myc->Y << endl;
		}
		void rgb2xyz() {
			xyz->X = srgb[0][0] * ((double)rgb->R / 2.55) + srgb[0][1] * ((double)rgb->G / 2.55) + srgb[0][2] * ((double)rgb->B / 2.55);
			xyz->Y = srgb[1][0] * ((double)rgb->R / 2.55) + srgb[1][1] * ((double)rgb->G / 2.55) + srgb[1][2] * ((double)rgb->B / 2.55);
			xyz->Z = srgb[2][0] * ((double)rgb->R / 2.55) + srgb[2][1] * ((double)rgb->G / 2.55) + srgb[2][2] * ((double)rgb->B / 2.55);
			cout << "X:" << xyz->X << " Y:" << xyz->Y << " Z:" << xyz->Z << endl;
		}
		void rgn2hsb() {
			float rprima = (float)rgb->R / (float)2.55;
			float gprima = (float)rgb->G / (float)2.55;
			float bprima = (float)rgb->B / (float)2.55;
			float max;
			float min;
			int identificador;
			if (rprima > gprima && rprima > bprima) {
				max = rprima;
				identificador = 1;
			}
			else if (gprima > rprima && gprima > bprima) {
				max = gprima;
				identificador = 2;
			}
			else {
				max = bprima;
				identificador = 3;
			}
			if (rprima < gprima && rprima < bprima) {
				min = rprima;
			}
			else if (gprima < rprima && gprima < bprima) {
				min = gprima;
			}
			else {
				min = bprima;
			}
			float delta = max - min;
			if (delta == 0) {
				hsb->H = 60 * 0x80000000;
			}
			else if (identificador == 1) {
				float valor = 60 * fmodf(((gprima - bprima) / delta),  6);
				if (valor < 0) {
					hsb->H = 360 - valor;
				}
				else {
					hsb->H = valor;
				}
			}
			else if (identificador == 2) {
				hsb->H = 60 * (((bprima - rprima) / delta) + 2);
			}
			else {
				hsb->H = 60 * (((rprima - gprima) / delta) + 4);
			}
			if (max == 0) {
				hsb->S = 0;
			}
			else {
				hsb->S = (delta / max)*100;
			}
			//hsb->B = 0.3*rprima + 0.59*gprima + 0.11*bprima;
			hsb->B = max;
			cout << "H:" << hsb->H << " S:" << hsb->S << " B:" << hsb->B << endl;

		}
		void myck2rgb() {

		}
		void myc2rgb() {

		}
		void hsb2rgb() {

		}
		void xyz2rgb() {

		}
		void xyz2lab() {

		}
		void lab2xyz() {

		}

		void desdergb() {
			cout << "Valores RGB van desde 0 a 255"<<endl;
			cout << "R:";
			cin >> rgb->R;
			cout << "G:";
			cin >> rgb->G;
			cout << "B:";
			cin >> rgb->B;
			system("cls");
			cout << "R:" << rgb->R << " G:" << rgb->G << " B:" << rgb->B << endl;
			rgb2myck();
			rgb2myc();
			rgn2hsb();
			rgb2xyz();
			cin.get();
			cin.get();
		}


};

int main()
{
	int opcion = 0;
	bool salir = true;
	Convertidor convertidor = Convertidor();
	while (salir) {
		system("cls");
		cout << "Menu:" << endl;
		cout << "1.Conversion de RGB" << endl; 
		cout << "2.Conversion de CMY" << endl;
		cout << "3.Conversion de CMYK" << endl;
		cout << "4.Conversion de HSB" << endl;
		cout << "5.Conversion de XYZ" << endl;
		cout << "6.Conversion de Lab" << endl;
		cout << "7.Salir" << endl;
		cin>>opcion;
		system("cls");
		switch (opcion) {
			case 1: //
				convertidor.desdergb();
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				salir = false;
				break;
			default:
				break;
		}
	}
	system("cls");
	cout << "Press Enter to Continue";
	convertidor.eliminador();
	cin.get();
	cin.get();
    return 0;
}

