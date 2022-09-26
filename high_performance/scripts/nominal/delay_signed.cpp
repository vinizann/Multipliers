#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

class Arco
{
public:
	int numarc;			// number of output
	int linha1, linha2; // inputs that compose an arch
	int riseout, risein;
	int difin, difout; // bit diff input, bit diff output
	char saida1, saida2;
};

// convert to bin
char *bindec(int num, int tam)
{
	int x;
	int i = (tam - 1);
	int dimension = tam + 1;
	char *bit = (char *)malloc(sizeof(char) * dimension);

	// fill vector with 0s
	for (x = 0; x < dimension; x++)
	{
		bit[x] = '0';
	}
	while (num > 0)
	{
		if ((num % 2) == 0)
			bit[i] = '0';

		else
			bit[i] = '1';

		i = i - 1;
		num = num / 2;
	}
	bit[tam] = '\0';
	return bit;
}

// check if the number is positive or negative
char *conversor(int num, int tam)
{
	int numAux, tamAux, dif;
	// if positive invokes bindec
	if (num > 0)
	{
		return bindec(num, tam);
	}
	// else invokes bindec with the equivalent positive number
	else
	{
		numAux = num * (-1);
		if (tam == 4)
		{
			tamAux = 8;
		}
		else
		{
			tamAux = 128;
		}
		dif = tamAux - numAux;
		num = tamAux + dif;
		return bindec(num, tam);
	}
}

int main()
{
	int ctl_a = -8, ctl_b = -8;				   // variables to perfom multiplications
	int a = 0, b = 0, resultado = 0, cont = 0; // variables to perfom multiplications
	int i, j, k, dif = 0;
	int col_tab = 0;
	int linha_tab = 0;
	char *vetor_a, *vetor_b, *vetor_r;
	char entrada[256][8], saida[256][8], linha[8];

	vector<Arco> lista;
	Arco meuArc;
	ofstream font, measure;

	// 256 combinations
	// multiplication between two numbers
	// convert the result to bin
	for (i = 0; i <= 15; i++)
	{
		for (j = 0; j <= 15; j++)
		{
			if (i < 8 && j < 8)
			{
				a = i;
				b = j;
				resultado = a * b;
				vetor_a = conversor(a, 4);
				vetor_b = conversor(b, 4);
				vetor_r = conversor(resultado, 8);
			}
			else if (i < 8 && j >= 8)
			{
				a = i;
				b = ctl_b;
				ctl_b++;
				resultado = a * b;
				vetor_a = conversor(a, 4);
				vetor_b = conversor(b, 4);
				vetor_r = conversor(resultado, 8);
			}
			else if (i >= 8 && j < 8)
			{
				a = ctl_a;
				b = j;
				resultado = a * b;
				vetor_a = conversor(a, 4);
				vetor_b = conversor(b, 4);
				vetor_r = conversor(resultado, 8);
			}
			else if (i >= 8 && j >= 8)
			{
				a = ctl_a;
				b = ctl_b;
				ctl_b++;
				resultado = a * b;
				vetor_a = conversor(a, 4);
				vetor_b = conversor(b, 4);
				vetor_r = conversor(resultado, 8);
			}
			strcpy(linha, vetor_a);
			strcat(linha, vetor_b);
			for (col_tab = 0; col_tab < 8; col_tab++)
			{
				// each matrix row stores the concat of two inputs
				entrada[linha_tab][col_tab] = linha[col_tab];
				// stores the bin
				saida[linha_tab][col_tab] = vetor_r[col_tab];
			}
			linha_tab++;
		}
		ctl_b = -8;
		if (i >= 8)
		{
			ctl_a++;
		}
	}
	free(vetor_a);
	free(vetor_b);
	free(vetor_r);

	// compare all inputs
	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			if (i != j)
			{
				// if an index of row i is different than j, increment CONT and stores the index
				for (k = 0; k < 8; k++)
				{
					if (entrada[i][k] != entrada[j][k])
					{
						cont++;
						dif = k;
					}
				}
			}
			// if 1 bit of diff, then compare outputs (I,J)
			if (cont == 1)
			{
				for (k = 0; k < 8; k++)
				{
					if (saida[i][k] != saida[j][k])
					{
						meuArc.linha1 = i;
						meuArc.linha2 = j;
						meuArc.difin = dif;
						meuArc.difout = k;
						if (entrada[i][dif] == '0')
						{
							meuArc.risein = 1;
						}
						else
							meuArc.risein = 0;
						meuArc.numarc = (k - 7) * (-1);
						if (saida[i][k] == '0')
						{
							meuArc.riseout = 1;
						}
						else
							meuArc.riseout = 0;
						lista.push_back(meuArc);
					}
				}
			}
			cont = 0;
		}
	}

	// waves of transition (4-8)
	// max supply 0.7
	string pwlh = " PWL (0n 0 2n 0 2.01n 0.7 4n 0.7)";
	string pwhl = " PWL(0n 0.7 2n 0.7 2.01n 0 4n 0)";

	// for each arch in the list
	for (i = 0; i < lista.size(); i++)
	{
		// open supply and measure files
		font.open("fontes.txt");
		measure.open("measure.txt");
		font << "*descricao de fontes" << endl;

		// iterate through all input indexes
		for (j = 0; j < 8; j++)
		{
			if (j == lista[i].difin)
			{
				if (lista[i].risein == 1)
				{
					font << "vf" << j << " f" << j << "in gnd" << pwlh << endl;
				}
				else
				{
					font << "vf" << j << " f" << j << "in gnd" << pwhl << endl;
				}
			}
			else
			{
				if (entrada[lista[i].linha1][j] == '0')
				{
					font << "vf" << j << " f" << j << "in gnd 0" << endl;
				}
				else
				{
					font << "vf" << j << " f" << j << "in gnd 0.7" << endl;
				}
			}
		}
		// measures
		measure << "*measures\n\n.control" << endl
				<< "set noaskquit"
				<< "\n\n\trun" << endl
				<< endl;
		measure << "*-------------------Atrasos---------------------------" << endl
				<< endl;

		// composing the measures, linha1 e linha2 are the input that compose an arch
		// numArc is the output affected by the transition
		if (lista[i].numarc == 7)
		{
			if (lista[i].risein == 1 && lista[i].riseout == 1)
			{
				measure << "\tmeas tran TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 rise=1";
				measure << " targ v(p" << lista[i].numarc - 1 << ") val = 0.35 rise=1" << endl;
				measure << "\techo TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2;
				measure << " >> Atrasos.txt" << endl
						<< endl;
			}
			else if (lista[i].risein == 0 && lista[i].riseout == 0)
			{
				measure << "\tmeas tran TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 fall=1";
				measure << " targ v(p" << lista[i].numarc - 1 << ") val = 0.35 fall=1" << endl;
				measure << "\techo TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2;
				measure << " >> Atrasos.txt" << endl
						<< endl;
			}
			else if (lista[i].risein == 1 && lista[i].riseout == 0)
			{
				measure << "\tmeas tran TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 rise=1";
				measure << " targ v(p" << lista[i].numarc - 1 << ") val = 0.35 fall=1" << endl;
				measure << "\techo TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2;
				measure << " >> Atrasos.txt" << endl
						<< endl;
			}
			else if (lista[i].risein == 0 && lista[i].riseout == 1)
			{
				measure << "\tmeas tran TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 fall=1";
				measure << " targ v(p" << lista[i].numarc - 1 << ") val = 0.35 rise=1" << endl;
				measure << "\techo TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2;
				measure << " >> Atrasos.txt" << endl
						<< endl;
			}
		}
		else
		{
			if (lista[i].risein == 1 && lista[i].riseout == 1)
			{
				measure << "\tmeas tran TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 rise=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 rise=1" << endl;
				measure << "\techo TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2;
				measure << " >> Atrasos.txt" << endl
						<< endl;
			}
			else if (lista[i].risein == 0 && lista[i].riseout == 0)
			{
				measure << "\tmeas tran TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 fall=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 fall=1" << endl;
				measure << "\techo TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2;
				measure << " >> Atrasos.txt" << endl
						<< endl;
			}
			else if (lista[i].risein == 1 && lista[i].riseout == 0)
			{
				measure << "\tmeas tran TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 rise=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 fall=1" << endl;
				measure << "\techo TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_HL_" << lista[i].linha1 << "_" << lista[i].linha2;
				measure << " >> Atrasos.txt" << endl
						<< endl;
			}
			else if (lista[i].risein == 0 && lista[i].riseout == 1)
			{
				measure << "\tmeas tran TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " trig v(f" << lista[i].difin << ") val = 0.35 fall=1";
				measure << " targ v(p" << lista[i].numarc << ") val = 0.35 rise=1" << endl;
				measure << "\techo TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&TP_LH_" << lista[i].linha1 << "_" << lista[i].linha2;
				measure << " >> Atrasos.txt" << endl
						<< endl;
			}
		}

		// measure of energy during all transition
		measure << endl
				<< "*------------------Energia----------------------------" << endl
				<< endl;
		measure << "\tmeas tran energia_" << lista[i].linha1 << "_" << lista[i].linha2 << " INTEG i(Vvdd) from=0n to=4n" << endl;
		measure << "\techo energia_" << lista[i].linha1 << "_" << lista[i].linha2 << " $&energia_" << lista[i].linha1 << "_" << lista[i].linha2 << " >> energia.txt" << endl
				<< endl;
		measure << "\n\n"
				<< "quit"
				<< "\n.endc" << endl;

		// invoke ngspice, change path and circuit.txt
		char arquivo[100] = "ngspice_con booth.txt";
		system(arquivo);
		font.close();
		measure.close();
	}
	cout << "FINISH!!" << endl;
	return 0;
}
