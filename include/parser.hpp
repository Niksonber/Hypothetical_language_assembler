/**
*	ALUNOS: NIKSON BERNARDES - 15/0143231
*			THALES GONÇALVES GRILO - 14/0163603
*	MATERIA: SOFTWARE BÁSICO
**/



#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map> 

using namespace std;


//TRANSFORMA STRING EM VETOR DE STRINGS SEPARANDO POR ESPAÇO
vector<string> split(string str, char sep);

//BUSCA EM MAP (UTILIZADO PARA VEFIFICAR EXISTENCIA NA TABELA DE SIMBOLOS, USO E DEFINICOES)

pair<string,int> search(string token, map<string, pair<string,int> > list);

pair<int,char> search(string token, map<string,pair<int,char>> list);


int search(string token, map<string,int> list);

//EXECUTA DIRETIVA CONST E SPACE
string exec_diretive(vector<string> line);

//VERIFICA ERROS DIRETIVA NA SECAO ERRADA
void erro_dire(string token, char section, int line);

//PRIMEIRO PASSE - ADICIONA ELEMENTOS NA TABELA DE SIMBOLO
map<string, pair<int,char>>  first_pass(string filename);


//VERIFICA ERROS SEMANTICOS, TIPO: PULAR PRO LUGAR ERRADO, DIVIDIR POR ZERO, SUBSTITUIR CONSTANTE
void erro(vector<string> tokens, int i, int line, map<string, pair<int,char>> TS);


//SEGUNDA PASSAGEM
void second_pass(string filename, bool modular, map<string, pair<int,char>> TS);
