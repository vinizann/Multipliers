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
	// convert
	while (num > 0)
	{
		if ((num % 2) == 0)
		{
			bit[i] = '0';
		}
		else
		{
			bit[i] = '1';
		}
		i = i - 1;
		num = num / 2;
	}
	bit[tam] = '\0';
	return bit;
}

int main()
{
	int a = 0, b = 0, resultado = 0; // variables to perfom multiplications
	int i, j, k;
	int col_tab = 0;
	int linha_tab = 0;
	char *vetor_a, *vetor_b, *vetor_r;
	char entrada[256][8], saida[256][8], linha[8];

	ofstream font, measure, inout;
	
	/*256 commbinations */
	for (i = 0; i <= 15; i++)
	{ 
		for (j = 0; j <= 15; j++)
		{
			// multiplication between two numbers
			// convert the result to bin
			a = i;
			b = j;
			resultado = a * b;

			vetor_a = bindec(a, 4);
			vetor_b = bindec(b, 4);
			vetor_r = bindec(resultado, 8);

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
	}
	free(vetor_a);
	free(vetor_b);
	free(vetor_r);

	// waves of transition (4-8)
	// max supply 0.68
	string pwlh = " PWL (0n 0 2n 0 2.01n 0.68 10n 0.68)";
	string pwhl = " PWL(0n 0.68 2n 0.68 2.01n 0 10n 0)";

	// for each posible input
	for (i = 0; i < 256; i++)
	{ 
		// open supply and measure files
		font.open("fontes.txt");
		measure.open("measure.txt");

		font << "*descricao de fontes" << endl;

		// iterate through all input indexes
		for (j = 0; j < 8; j++)
		{
			if (entrada[i][j] == '0')
			{
				font << "vf" << j << " f" << j << "in gnd 0" << endl;
			}
			else
			{
				font << "vf" << j << " f" << j << "in gnd 0.68" << endl;
			}
		}

		// composing measures
		measure << "*measures" << endl
				<< endl;
		measure << ".control" << endl;
		measure << "set noaskquit" << endl
				<< endl;
		measure << "\t"
				<< "run" << endl
				<< endl;

		measure << "*-------------------Operacoes--------------------------" << endl
				<< endl;
		measure << "\t"
				<< "meas tran s7 find v(p7) at=10n" << endl;
		measure << "\t"
				<< "meas tran s6 find v(p6) at=10n" << endl;
		measure << "\t"
				<< "meas tran s5 find v(p5) at=10n" << endl;
		measure << "\t"
				<< "meas tran s4 find v(p4) at=10n" << endl;
		measure << "\t"
				<< "meas tran s3 find v(p3) at=10n" << endl;
		measure << "\t"
				<< "meas tran s2 find v(p2) at=10n" << endl;
		measure << "\t"
				<< "meas tran s1 find v(p1) at=10n" << endl;
		measure << "\t"
				<< "meas tran s0 find v(p0) at=10n" << endl;
		measure << "\t"
				<< "echo $&s7 $&s6 $&s5 $&s4 $&s3 $&s2 $&s1 $&s0 >> resultado.txt" << endl
				<< endl;

		// measure of energy over all transition
		measure << endl
				<< "*------------------Energia----------------------------" << endl
				<< endl;
		measure << "\t"
				<< "meas tran energia_" << i << " INTEG i(Vvdd) from=0n to=10n" << endl;
		measure << "\techo energia_" << i << " $&energia_" << i << " >> energia.txt" << endl
				<< endl;

		measure << "quit" << endl;
		measure << ".endc" << endl;

		// invoke ngspice, change path and circuit.txt
		char arquivo[100] = "ngspice_con array.txt";
		system(arquivo);
		font.close();
		measure.close();
	}

	cout << "FINISH!!" << endl;
	return 0;
}
