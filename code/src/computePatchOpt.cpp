/*!
 * \file      computePatchOpt.cpp
 * \author    Jérémie Piellard
 * \author    Léo Gouttefarde
 * \date      November, 2015
 * \brief     Implements the computePatchOpt program.
 * \warning   Usage: computePatchOpt originalFile patchedFile
 * \copyright GNU Public License.
 *
 * This is the computePatchOpt main program.
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <stdint.h>

using namespace std;


enum Operation {
	AJOUT,
	SUBST,
	SUPPR,
	MSUPPR,
	NB_OPS
};

typedef struct OptOp_ {
	int k;
	enum Operation op;
	int c;
} OptOp;

OptOp *opt;
int *mem;


int N, M;

vector<string> input;
vector<string> output;


static inline int taille_ligne(int j)
{
	int taille = output[j].size();

	if (j < M-1)
		taille++;

	return taille;
}

static inline int calc_cout_subst(int i, int j)
{
	if (input[i] == output[j]) {
		return 0;
	}

	else {
		return 10 + taille_ligne(j);
	}
}

static inline uint32_t get(int i, int j)
{
	return i * (M+1) + j;
	// return j * (N+1) + i;
}

void print_sol2()
{
	int i = 0, j = 0;

	while (i <= N && j <= M && !(i == N  && j == M)) {
		// cout << "i = " << i << "j = " << j << endl;
		//int c = opt[i][j].c;
		switch (opt[get(i, j)].op) {
			case AJOUT:
				cout << "+ " << i << endl;

				cout << output[j];

				if (j < M-1)
					cout << endl;

				j++;
				break;

			case SUBST:
				if (input[i] != output[j]) {
					cout << "= " << i+1 << endl;
					cout << output[j] << endl;
					i++;
					j++;
				}
				else {
					i++;
					j++;
					continue;
				}
				break;

			case SUPPR:
				cout << "d " << i+1 << endl;
				i++;
				break;

			case MSUPPR:
				cout << "D " << i+1 << " " << opt[get(i, j)].k << endl;
				i += opt[get(i, j)].k;
				break;

			default:
				exit(-1);
		}

		// cout << "  : " << c << endl;
		// cout << endl;
	}
}

void print_sol()
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			OptOp op = opt[get(i, j)];

			cout << op.op << " ";
		}

		cout << endl;
	}
}

static inline int B(int i, int j)
{
	// Fin rec
	if (i == N && j == M) {
		return 0;
	}

	if (mem[get(i, j)] >= 0)
		return mem[get(i, j)];



	int cout[NB_OPS] = { -1, -1, -1, -1 };
	int k = -1;

	string s_str, a_str;

	// Si fichier de sortie écrit, plus que suppressions possibles (d pour 1 ligne, D pour plus)
	if (j < M) {
		int taille = taille_ligne(j);

		a_str = output[j];

		cout[AJOUT] = 10 + taille + B(i, j + 1);
	}

	// Si fichier d'entrée parcouru, seuls ajouts possibles
	if (i < N) {
		cout[MSUPPR] = -1;


		for (int l = 2; l <= N; l++) {
			if ((i+l) > N)
				break;

			int cur_cout = 15 + B(i + l, j);

			if (cur_cout < cout[MSUPPR] || cout[MSUPPR] == -1) {
				cout[MSUPPR] = cur_cout;
				k = l;
			}
		}


		cout[SUPPR] = 10 + B(i + 1, j);


		// Si fichier de sortie écrit, plus que suppressions possibles (d pour 1 ligne, D pour plus)
		if (j < M) {
			int cout_subst = calc_cout_subst(i, j);

			if (cout_subst > 0)
				s_str = output[j];


			cout[SUBST] = cout_subst + B(i + 1, j + 1);
		}
	}


	enum Operation op = (enum Operation)0;
	int cur_cout = cout[0];

	for (int l = 1; l < NB_OPS; l++) {
		if ((cout[l] < cur_cout && cout[l] != -1)
			|| cur_cout == -1) {
			cur_cout = cout[l];
			op = (enum Operation)l;
		}
	}

	opt[get(i, j)].op = op;
	opt[get(i, j)].k = k;
	opt[get(i, j)].c = cur_cout;

	mem[get(i, j)] = cur_cout;

	// std::cout << "i = " << i << "j = " << j << endl;

	return cur_cout;
}

static inline bool readFile(char *path, vector<string>& oLines)
{
	ifstream file(path, ifstream::in);

	if (file.is_open()) {

		string line;
		while (std::getline(file, line))
			oLines.push_back(line);

		file.close();
		return true;
	}

	return false;
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		cout << "Usage : " << argv[0] << " <originalFile> <patchedFile> " << endl;
		return EXIT_SUCCESS;
	}


	readFile(argv[1], input);
	readFile(argv[2], output);

	// cout << "N = " << input.size() << endl;
	// cout << "M = " << output.size() << endl;

	N = input.size();
	M = output.size();


	mem = (int*)malloc((N+1) * (M+1) * sizeof(int));

	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= M; j++) {
			mem[get(i, j)] = -1;
		}
	}


	// mem = (int**)calloc(N+1, sizeof(int*));

	// for (int i = 0; i <= N; i++) {
	// 	mem[i] = (int*)calloc(M+1, sizeof(int));

	// 	for (int j = 0; j <= M; j++) {
	// 		mem[i][j] = -1;
	// 	}
	// }



	opt = (OptOp*)calloc((N+1) * (M+1), sizeof(OptOp));
	for (int i = 0; i <= N; i++) {

		// Unused operation value
		for (int j = 0; j <= M; j++) {
			// cout << get(i,j) << " ";
			opt[get(i, j)].op = (Operation)-1;
		}
		// cout << endl;
	}

	// opt = (OptOp**)calloc(N+1, sizeof(OptOp*));

	// for (int i = 0; i <= N; i++) {
	// 	opt[i] = (OptOp*)calloc(M+1, sizeof(OptOp));

	// 	// Unused operation value
	// 	for (int j = 0; j <= M; j++) {
	// 		opt[i][j].op = (Operation)-1;
	// 	}
	// }

	int out = B(0, 0);


	// print_sol();
	print_sol2();

	// cout << endl << "Cout = " << out << endl;


	return EXIT_SUCCESS;
}

