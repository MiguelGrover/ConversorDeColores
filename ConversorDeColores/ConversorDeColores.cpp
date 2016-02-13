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
		double isrgb[3][3] = {
			{3.2404542, -1.5371385, -0.4985314},
			{-0.9692660,  1.8760108,  0.0415560},
			{0.0556434, -0.2040259,  1.0572252}
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
			float max = 0;
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
			if (myck->K != 100) {
				myck->C = round(((100 - rprima - myck->K) / (100 - myck->K)) * 100);
				myck->M = round(((100 - gprima - myck->K) / (100 - myck->K)) * 100);
				myck->Y = round(((100 - bprima - myck->K) / (100 - myck->K)) * 100);
			}
			else {
				myck->C = 0;
				myck->M = 0;
				myck->Y = 0;
			}
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
				float valor = round(60 * fmodf(((gprima - bprima) / delta),  6));
				if (valor < 0) {
					hsb->H = 360 + valor;
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
			cout << "H:" << hsb->H << " S:" << hsb->S << " V:" << hsb->B << endl;

		}
		void myck2rgb() {
			rgb->R = round(255 * (1 - myck->C / 100)*(1 - myck->K / 100));
			rgb->G = round(255 * (1 - myck->M / 100)*(1 - myck->K / 100));
			rgb->B = round(255 * (1 - myck->Y / 100)*(1 - myck->K / 100));
			cout << "R:" << rgb->R << " G:" << rgb->G << " B:" << rgb->B << endl;
		}
		void myc2rgb() {
			rgb->R = 255 - myc->C;
			rgb->G = 255 - myc->M;
			rgb->B = 255 - myc->Y;
			cout << "R:" << rgb->R << " G:" << rgb->G << " B:" << rgb->B << endl;
		}
		void hsb2rgb() {
			double C = (((double)hsb->B/ (double)100)*((double)hsb->S/ (double)100));
			double hprima = (double)hsb->H / (double)60,aux = 2;
			double rprima, gprima, bprima;
			double x = (C*(1 - abs(fmod(hprima,aux)-1)));
			if (hprima >= 0 && hprima < 1) {
				rprima = C;
				gprima = x;
				bprima = 0;
			}else if (hprima >= 1 && hprima < 2) {
				rprima = x;
				gprima = C;
				bprima = 0;
			}else if (hprima >= 2 && hprima < 3) {
				rprima = 0;
				gprima = C;
				bprima = x;
			}
			else if (hprima >= 3 && hprima < 4) {
				rprima = 0;
				gprima = x;
				bprima = C;
			}
			else if (hprima >= 4 && hprima < 5) {
				rprima = x;
				gprima = 0;
				bprima = C;
			}
			else if (hprima >= 5 && hprima < 6) {
				rprima = C;
				gprima = 0;
				bprima = x;
			}
			else {
				rprima = 0;
				gprima = 0;
				bprima = 0;
			}
			double m = ((double)hsb->B/(double)100) - C;
			rgb->R = round((rprima + m)*255);
			rgb->G = round((gprima + m)*255);
			rgb->B = round((bprima + m)*255);
			cout << "R:" << rgb->R << " G:" << rgb->G << " B:" << rgb->B << endl;
		}
		void xyz2rgb() {
			rgb->R = round(isrgb[0][0] * xyz->X + isrgb[0][1] * xyz->Y + isrgb[0][2] * xyz->Z)*2.55;
			rgb->G = round(isrgb[1][0] * xyz->X + isrgb[1][1] * xyz->Y + isrgb[1][2] * xyz->Z)*2.55;
			rgb->B = round(isrgb[2][0] * xyz->X + isrgb[2][1] * xyz->Y + isrgb[2][2] * xyz->Z)*2.55;
			cout << "R:" << rgb->R << " G:" << rgb->G << " B:" << rgb->B << endl;
		}
		void xyz2lab() {
			double	xprima, yprima, zprima;
			double fx, fy, fz;
			xprima = (xyz->X) / xn;
			yprima = (xyz->Y) / yn;
			zprima = (xyz->Z) / zn;
			/*
			if (var_X > 0.008856) var_X = var_X ^ (1 / 3)
			else                    var_X = (7.787 * var_X) + (16 / 116)
				if (var_Y > 0.008856) var_Y = var_Y ^ (1 / 3)
				else                    var_Y = (7.787 * var_Y) + (16 / 116)
					if (var_Z > 0.008856) var_Z = var_Z ^ (1 / 3)
					else                    var_Z = (7.787 * var_Z) + (16 / 116)

						CIE - L* = (116 * var_Y) - 16
						CIE - a* = 500 * (var_X - var_Y)
						CIE - b* = 200 * (var_Y - var_Z)
			*/
			double n = pow(((double)6 / (double)29), 3);
				if (xprima > n) {
					fx = cbrt(xprima);
				}
				else {
					fx = ((double)1 / (double)3)*pow(((double)29 / (double)6), (double)2)*xprima + ((double)4 / (double)29);
				}
				if (yprima > n) {
					fy = cbrt(yprima);
				}
				else {
					fy = ((double)1 / (double)3)*pow(((double)29 / (double)6), (double)2)*yprima + ((double)4 / (double)29);
				}
				if (zprima > n) {
					fz = cbrt(zprima);
				}
				else {
					fz = ((double)1 / (double)3)*pow(((double)29 / (double)6), (double)2)*zprima + ((double)4 / (double)29);
				}
				lab->L = ((double)116 * fy) - (double)16;
				lab->a = (double)500 * (fx - fy);
				lab->b = (double)200 * (fy - fz);
				if (lab->L < (double)0) lab->L = (double)0;

				cout << "L:" << lab->L << " a:" << lab->a << " b:" << lab->b << endl;
		}
		void lab2xyz() {
			double fx, fy, fz;
			double xprima, yprima, zprima;
			fy = (lab->L + (double)16) / (double)116;
			fx = (fy + (lab->a / (double)500));
			fz = fy - (lab->b / (double)200);
			double n = (double)6 / (double)29;
			if (fx > n) {
				xprima = pow(fx, 3);
			}
			else {
				xprima = (double)3 * pow(((double)6 / (double)29), 2)*(fx - ((double)16 / (double)116));
			}
			if (fy > n) {
				yprima = pow(fy, 3);
			}
			else {
				yprima = (double)3 * pow(((double)6 / (double)29), 2)*(fy - ((double)16 / (double)116));
			}
			if (fz > n) {
				zprima = pow(fz, 3);
			}
			else {
				zprima = (double)3 * pow(((double)6 / (double)29), 2)*(fz - ((double)16 / (double)116));
			}
			xyz->X = xn * xprima;
			xyz->Y = yn * yprima;
			xyz->Z = zn * zprima;
			cout << "X:" << xyz->X << " Y:" << xyz->Y << " Z:" << xyz->Z << endl;
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
			xyz2lab();
			cin.get();
			cin.get();
		}

		void desdemyck() {
			cout << "Valores MYCK van desde 0 a 100" << endl;
			cout << "C:";
			cin >> myck->C;
			cout << "M:";
			cin >> myck->M;
			cout << "Y:";
			cin >> myck->Y;
			cout << "K:";
			cin >> myck->K;
			system("cls");
			myck2rgb();
			cout << "C:" << myck->C << " M:" << myck->M << " Y:" << myck->Y << " K:" << myck->K << endl;
			rgb2myc();
			rgn2hsb();
			rgb2xyz();
			xyz2lab();
			cin.get();
			cin.get();
		}

		void desdecmy() {
			cout << "Valores CMY van desde 0 a 100" << endl;
			cout << "C:";
			cin >> myc->C;
			cout << "M:";
			cin >> myc->M;
			cout << "Y:";
			cin >> myc->Y;
			system("cls");
			myc2rgb();
			rgb2myck();
			cout << "C:" << myc->C << " M:" << myc->M << " Y:" << myc->Y << endl;
			rgn2hsb();
			rgb2xyz();
			xyz2lab();
			cin.get();
			cin.get();
		}

		void desdehsv() {
			cout << "Valores HSV" << endl;
			cout << "H es de 0 a 360" << endl;
			cout << "S y V de 0 a 100" << endl;
			cout << "H:";
			cin >> hsb->H;
			cout << "S:";
			cin >> hsb->S;
			cout << "V:";
			cin >> hsb->B;
			system("cls");
			hsb2rgb();
			rgb2myck();
			rgb2myc();
			cout << "H:" << hsb->H << " S:" << hsb->S << " V:" << hsb->B << endl;
			rgb2xyz();
			xyz2lab();
			cin.get();
			cin.get();
		}

		void desdexyz() {
			cout << "Valores XYZ" << endl;
			cout << "X:";
			cin >> xyz->X;
			cout << "Y:";
			cin >> xyz->Y;
			cout << "Z:";
			cin >> xyz->Z;
			system("cls");
			xyz2rgb();
			rgb2myck();
			rgb2myc();
			rgn2hsb();
			cout << "X:" << xyz->X << " Y:" << xyz->Y << " Z:" << xyz->Z << endl;
			xyz2lab();
			cin.get();
			cin.get();
		}

		void desdelab() {
			cout << "Valores Lab" << endl;
			cout << "L:";
			cin >> lab->L;
			cout << "a:";
			cin >> lab->a;
			cout << "b:";
			cin >> lab->b;
			system("cls");
			lab2xyz();
			xyz2rgb();
			rgb2myck();
			rgb2myc();
			rgn2hsb();
			cout << "L:" << lab->L << " a:" << lab->a << " b:" << lab->b << endl;
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
				convertidor.desdecmy();
				break;
			case 3:
				convertidor.desdemyck();
				break;
			case 4:
				convertidor.desdehsv();
				break;
			case 5:
				convertidor.desdexyz();
				break;
			case 6:
				convertidor.desdelab();
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

