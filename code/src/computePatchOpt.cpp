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
#include <iomanip>
#include <queue>
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
} OptOp;

static OptOp *opt;
static int *mem;


static uint32_t NB_CALC_TOT;
static uint32_t nb_calc_done = 0;


static bool VERBOSE = false;


static int N, M;

static vector<string> input;
static vector<string> output;


static inline int calc_cout_subst(int i, int j)
{
	if (input[i] == output[j]) {
		return 0;
	}

	else {
		return 10 + output[j].size();
	}
}

static inline uint32_t get(int i, int j)
{
	return i * (M+1) + j;
	// return j * (N+1) + i;
}

static inline void displayPatch()
{
	int i = 0, j = 0;

	if (VERBOSE)
		cerr << endl;

	while (i <= N && j <= M && !(i == N  && j == M)) {

		// cout << "i = " << i << "j = " << j << endl;
		//int c = mem[get(i, j)].c;

		switch (opt[get(i, j)].op) {
			case AJOUT:
				cout << "+ " << i << endl;
				cout << output[j] << endl;

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


	// Si fichier de sortie écrit, plus que suppressions possibles (d pour 1 ligne, D pour plus)
	if (j < M) {
		cout[AJOUT] = 10 + output[j].size() + B(i, j + 1);
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
			const int cout_subst = calc_cout_subst(i, j);

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

	mem[get(i, j)] = cur_cout;

	nb_calc_done++;

	if (VERBOSE) {
		std::cerr << std::fixed << std::setw( 3 ) << std::setprecision( 2 );
		std::cerr << "\rAvancée : " << nb_calc_done << " / " << NB_CALC_TOT << ", " << ((float)nb_calc_done*100)/NB_CALC_TOT << " %";
	}
	// std::cout << "i = " << i << "j = " << j << endl;

	return cur_cout;
}

typedef struct Dist_ {
	uint32_t val;
	uint32_t last;
} Dist;

static inline int dijkstra()
{
	// Ordre des champs : priorité, id
	priority_queue< pair<uint32_t, uint32_t> > queue;
	Dist dist[(N+1)*(M+1)];

	for (int i = 0; i <= N; i++) {
		for (int j = 0; j<= M; j++) {
			dist[get(i, j)].val = UINT32_MAX;
			dist[get(i, j)].last = 0;
		}
	}

	dist[get(N, M)].val = 0;

	queue.push(make_pair(get(N, M), 0));


	pair<uint32_t, uint32_t> n;
	uint32_t idNode;

	while (!queue.empty()) {

		n = queue.top();
		queue.pop();

		idNode = n.second;

		// https://fr.wikipedia.org/wiki/Algorithme_de_Dijkstra#Pseudo-code


		// if (idNode) ...
		// for f in noeuds suivants n

			// if (dist[f].val > dist[idNode].val + cout) {
			// 	f.val = dist[idNode].val + cout;
			// 	f.last = idNode;
			// }


			// for u in 0 .. Last loop
			// 	for v in 0 .. Last loop

			// 		cArc := G.Matrice(u, v);

			// 		if cArc.isValid then
			// 			if (Distance(u).Val + cArc.Cout) < Distance(v).Val then
			// 				Distance(v).Val := Distance(u).Val + cArc.Cout;
			// 				Attente.Enqueue(New_Item => ( Distance(v).Val, v ));


			// 				Nat_Lists.Clear( Distance(v).Chemin );

			// 				Position := Nat_Lists.First( Distance(u).Chemin );

			// 				-- Copy u to v
			// 				while Nat_Lists.Has_Element( Position ) loop
			// 					Nat_Lists.Append( Distance(v).Chemin, Nat_Lists.Element(Position) );
			// 					Nat_Lists.Next( Position );
			// 				end loop;

			// 				-- Add u to path
			// 				Nat_Lists.Append( Distance(v).Chemin, u );
			// 			end if;
			// 		end if;

			// 	end loop;
			// end loop;

	}
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

	if (argc == 4)
		VERBOSE = true;

	readFile(argv[1], input);
	readFile(argv[2], output);

	// cout << "N = " << input.size() << endl;
	// cout << "M = " << output.size() << endl;

	N = input.size();
	M = output.size();

	NB_CALC_TOT = (N+1) * (M+1) - 1;


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

	// const int out = B(0, 0);

	dijkstra();


	// displayPatch();

	// cerr << endl << "Coût = " << out << endl;


	return EXIT_SUCCESS;
}

