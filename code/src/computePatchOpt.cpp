/*!
 * \file      computePatchOpt.cpp
 * \author    Jérémie Piellard
 * \author    Léo Gouttefarde
 * \date      November, 2015
 * \brief     Implements the computePatchOpt program.
 * \warning   Usage: applyPatch patchFile originalFile 
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
	//int c;
	//std::string str;
} OptOp;

OptOp opt[9000][9000];


int N, M;

std::ifstream entree;
std::ifstream sortie;



std::string ligne_entree;
std::string ligne_sortie;



int compter_lignes(std::ifstream& stream)
{
	if (stream.is_open()) {
		int count = std::count(std::istreambuf_iterator<char>(stream),
						std::istreambuf_iterator<char>(), '\n') + 1;

		stream.clear();
		stream.seekg(0, std::ios::beg);

		return count;
	}

	else
		return 0;
}

int taille_ligne(int j)
{
	int taille = ligne_sortie.size();

	if (j < M-1)
		taille++;

	return taille;
}

int calc_cout_subst(int j)
{
	if (ligne_entree == ligne_sortie) {
		return 0;
	}

	else {
		return 10 + taille_ligne(j);
	}
}


void print_sol2()
{
	int i = 0, j = 0;

	while (i <= N && j <= M && !(i == N  && j == M)) {
		// cout << "i = " << i << "j = " << j << endl;
		//int c = opt[i][j].c;
		switch (opt[i][j].op) {
			case AJOUT:
				cout << "+ " << i << endl;

				//cout << opt[i][j].str;

				if (j < M-1)
					cout << endl;

				j++;
				break;

			case SUBST:
				//if (opt[i][j].str.size() > 0) {
					cout << "= " << i+1 << endl;
					//cout << opt[i][j].str << endl;
					i++;
					j++;
				//}
				//else {
				//	i++;
				//	j++;
				//	continue;
				//}
				break;

			case SUPPR:
				cout << "d " << i+1 << endl;
				i++;
				break;

			case MSUPPR:
				cout << "D " << i+1 << " " << opt[i][j].k << endl;
				i += opt[i][j].k;
				break;
		}

		// cout << "  : " << c << endl;
		// cout << endl;
	}
}

void print_sol()
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			OptOp op = opt[i][j];

			cout << op.op << " ";
		}

		cout << endl;
	}
}

int B(int i, int j)
{
	// Fin rec
	if (i == N && j == M) {
		return 0;
	}


	int cout[NB_OPS] = { -1, -1, -1, -1 };
	int k = -1;

	std::string s_str, a_str;

	// Si fichier de sortie écrit, plus que suppressions possibles (d pour 1 ligne, D pour plus)
	if (j < M) {
		int taille = taille_ligne(j);

		streampos pos = sortie.tellg();
		std::string prev = ligne_sortie;

		a_str = ligne_sortie;

		getline(sortie, ligne_sortie);

		cout[AJOUT] = 10 + taille + B(i, j + 1);

		sortie.clear();
		sortie.seekg(pos);
		ligne_sortie = prev;
	}

	// Si fichier d'entrée parcouru, seuls ajouts possibles
	if (i < N) {
		cout[MSUPPR] = -1;


		for (int l = 2; l <= N; l++) {
			if ((i+l) > N)
				break;

			streampos pos = entree.tellg();
			std::string prev = ligne_entree;


			for (int p = 0; p < l; p++)
				getline(entree, ligne_entree);

			int cur_cout = 15 + B(i + l, j);

			entree.clear();
			entree.seekg(pos);
			ligne_entree = prev;

			if (cur_cout < cout[MSUPPR] || cout[MSUPPR] == -1) {
				cout[MSUPPR] = cur_cout;
				k = l;
			}
		}


		std::string prev = ligne_entree;
		streampos pos = entree.tellg();
		getline(entree, ligne_entree);

		cout[SUPPR] = 10 + B(i + 1, j);

		entree.clear();
		entree.seekg(pos);
		ligne_entree = prev;


		// Si fichier de sortie écrit, plus que suppressions possibles (d pour 1 ligne, D pour plus)
		if (j < M) {
			int cout_subst = calc_cout_subst(j);

			streampos e_pos = entree.tellg();
			streampos s_pos = sortie.tellg();
			std::string prev_e = ligne_entree;
			std::string prev_s = ligne_sortie;

			if (cout_subst > 0)
				s_str = ligne_sortie;


			getline(entree, ligne_entree);
			getline(sortie, ligne_sortie);

			cout[SUBST] = cout_subst + B(i + 1, j + 1);

			entree.clear();
			entree.seekg(e_pos);
			ligne_entree = prev_e;
			ligne_sortie = prev_s;

			sortie.clear();
			sortie.seekg(s_pos);
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

	opt[i][j].op = op;
	opt[i][j].k = k;
	//opt[i][j].c = cur_cout;

	//if (op == AJOUT)
	//	opt[i][j].str = a_str;
////
	//else if (op == SUBST)
	//	opt[i][j].str = s_str;
//

	return cur_cout;
}


int main(int argc, char **argv)
{
	if (argc < 3) {
		cout << "Usage : " << argv[0] << " <unpatched> <patched>" << endl;
		return EXIT_SUCCESS;
	}

	entree.open(argv[1], std::ifstream::in);
	sortie.open(argv[2], std::ifstream::in);

	N = compter_lignes(entree);
	M = compter_lignes(sortie);

	// if (N > 512 || M > 512) {
	// 	cout << "Current file too big for this test version" << endl;
	// 	exit(1);
	// }


	getline(entree, ligne_entree);
	getline(sortie, ligne_sortie);

	cout << "N = " << N << endl;
	cout << "M = " << M << endl;

	int out = B(0, 0);

	cout << "Coût = " << out << endl;

	entree.close();
	sortie.close();

	// print_sol();
	print_sol2();

	return EXIT_SUCCESS;
}

