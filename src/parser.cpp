/**
*	ALUNOS: NIKSON BERNARDES - 15/0143231
*			THALES GONÇALVES GRILO - 14/0163603
*	MATERIA: SOFTWARE BÁSICO
**/


/*
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map> */
#include "../include/parser.hpp"

using namespace std;

map<string, pair<string, int> > instructions_2 = {
{"ADD", {"1", 2}},
{"SUB", {"2", 2}},
{"MULT",{"3", 2}},
{"DIV", {"4", 2}},
{"JMP", {"5", 2}},
{"JMPN",{"6", 2}},
{"JMPP",{"7", 2}},
{"JMPZ",{"8", 2}},
{"COPY",{"9", 3}},
{"LOAD",{"10", 2}},
{"STORE",{"11", 2}},
{"INPUT",{"12", 2}},
{"OUTPUT",{"13", 2}},
{"STOP", {"14", 1}}
};

map<string, int> directives2 = {
{"CONST", 1},
{"SPACE", 1},
{"SECTION", 0},
{"BEGIN", 0},
{"EXTERN",0},
{"PUBLIC",0},
{"END",   0},
};


//TABELAS DE SIMBOLOS
//map<string, pair<int,char>> TS;

//TRANSFORMA STRING EM VETOR DE STRINGS SEPARANDO POR ESPAÇO
vector<string> split(string str, char sep){
	istringstream iss(str);
	string tmp;
	vector<string> res;
	while(getline(iss, tmp, sep)){
		if(tmp != ""){
			if(tmp.front()==';')
				break;
			if(tmp.back()==',')
				tmp.pop_back();
			res.push_back(tmp);
		}

	}
	return res;
}

//BUSCA EM MAP (UTILIZADO PARA VEFIFICAR EXISTENCIA NA TABELA DE SIMBOLOS, USO E DEFINICOES)

pair<string,int> search(string token, map<string, pair<string,int> > list){
	map<string,pair<string,int> > ::iterator it;
	it = list.find(token);
  	if (it != list.end()){
    	list.erase(it);
    	return (it->second);
  	}else{
  		return {"",-1};
  	}
}

pair<int,char> search(string token, map<string,pair<int,char>> list){
	map<string,pair<int,char>>::iterator it;
	it = list.find(token);
  	if (it != list.end()){
    	list.erase(it);
    	return it->second;
  	}else{
  		return {-1,'0'};
  	}
}


int search(string token, map<string,int> list){
	map<string,int>::iterator it;
	it = list.find(token);
  	if (it != list.end()){
    	list.erase(it);
    	return it->second;
  	}else{
  		return -1;
  	}
}

int find_plus(vector<string> tokens){
	int i = 0;
	for (; i< tokens.size()-1; i++){
		if(tokens[i]=="+")
			break;
	}
	return i;
}

//EXECUTA DIRETIVA CONST E SPACE
string exec_diretive(vector<string> line){
	string tmp = "";
	 int i;
	if(line.size()>1){
		if (line[1] == "CONST"){
			stringstream stream;
			int tmp2;
			try{
				stringstream(line[2]) >> std::hex >> tmp2;
				stream << tmp2 ;
				stream >> tmp  ;
				tmp +=  " ";
			}
			catch(...){
				tmp = line[2] + " ";
			}
			
		}
		else if (line[1] == "SPACE"){
			tmp += "0 ";
			if(line.size()==3){
				for (i = 0; i < stoi(line[2]) - 1; i++){
					tmp+="0 ";
				}
			}
		}
	}
	return tmp;
}

//VERIFICA ERROS DIRETIVA NA SECAO ERRADA
void erro_dire(string token, char section,  int line){
	if (section=='t'){
		if(token=="CONST" || token=="SPACE"){
			cout << "semantico, diretiva na secao errada -> linha: " << line << endl;
		}
	}else if (section=='b'){
		if(token=="BEGIN" || token=="CONST" || token=="EXTERN" || token=="PUBLIC" ){
			cout << "semantico, diretiva na secao errada -> linha: " << line << endl;
		}
	}else if (section=='d'){
		if(token=="BEGIN" || token=="SPACE" || token=="EXTERN" || token=="PUBLIC"){
			cout << "semantico, diretiva na secao errada -> linha: " << line << endl;
		}
	}

}

//PRIMEIRO PASSE - ADICIONA ELEMENTOS NA TABELA DE SIMBOLO
map<string, pair<int,char>> first_pass(string filename){
	map<string, pair<int,char>> TS;
	int line = 1;
	int position = 0;
	int p ;
	char section = '0';
	bool tex=false;
	string line_string;
	ifstream code(filename + ".pre");
	if(code.is_open()){
		while(getline (code,line_string)){
			vector<string> tokens = split(line_string, ' ');
			//if (tokens.size()>1) cout << tokens[1] + " " << line << endl;
			if(tokens[0]=="SECTION"){
				if(tokens[1]=="TEXT") {
					section = 't';
					tex=true;
				}else if(tokens[1]=="DATA"){
					section = 'd';
				}else if(tokens[1]=="BSS"){
					section = 'b';
				}else
					cout << "Secao desconhecida -> liha: "  << line << endl;
			}
			 int i=0;
			if(tokens[i].back()== ':'){
				if(tokens.size() > 1 && tokens[i+1].back()== ':') 
					cout << "sintatico, dois rotulos na mesma linha -> linha: "<< line << endl; 
				p = (search(tokens[i], TS)).first;
				if(p!=-1){
					cout << "semantico, Erro simbolo redefinido -> linha: "<<  line  << endl;	
				}else{
					//adiciona na tabela de simbolos (rotulo e posicao)
					if(tokens.size()==3+i && tokens[i+1]=="CONST" && (tokens[i+2]=="0" || tokens[i+2]=="0x0"))
							TS[tokens[i]] = {position, '0'};
					else{
						TS[tokens[i]] = {position,section};
					}
				}
				if (tokens.size() > 1)
					i++;
			}
			p = (search(tokens[i], instructions_2)).second;
			if(p!=-1){
				if (section == 'b' || section == 'd')
					cout << "semantico, secao errada -> linha: " << line << endl;
				position += p; 
			}else{
				p = search(tokens[i], directives2);
				if(p!=-1){
					erro_dire(tokens[i], section, line);
					if (tokens.size()>1 && tokens[1] == "SPACE"){
						if(tokens.size()==3)
							position += stoi(tokens[2]) - 1; 
					
					}
					position += p; 
				}else{
					cout << "Operacao nao identificada -> linha : " << line  << endl;
				}
			}	
			line++;
		}
	}
	if(!tex)
		cout << "Faltando section text, line: 1" << endl;
	return TS;
}


//VERIFICA ERROS SEMANTICOS, TIPO: PULAR PRO LUGAR ERRADO, DIVIDIR POR ZERO, SUBSTITUIR CONSTANTE
void erro(vector<string> tokens, int i,  int line, map<string, pair<int,char>> TS){
	if (tokens.size()> i+1 && (tokens[i]=="JMP" || tokens[i]=="JMPZ" || tokens[i]=="JMPP" || tokens[i]=="JMPN")){
		pair<int, char> b = search(tokens[i+1]+":", TS);
		if(b.second!='t'){
			cout << "erro semantico, pulo para secao errada -> linha: " << line << endl;
		}
	}else if(tokens.size()> i+1 && tokens[i]=="DIV"){
		pair<int, char> b = search(tokens[i+1]+":", TS);
		if(b.second=='0'){
			cout << "erro semantico, pulo para secao errada -> linha: " << line << endl;
		}	

	}else if(tokens.size()> i+1  && tokens[i]=="STORE"){
		pair<int, char> b = search(tokens[i+1]+":", TS);
		if(b.second=='d'){
			cout << "erro semantico, subreescreve constante -> linha: " << line << endl;
		}else if(b.second=='t'){

		}
	}else if(tokens.size()> i+2 && tokens[i]=="COPY"){
		pair<int, char> b = search(tokens[i+2]+":", TS);
		if(b.second=='d'){
			cout << "erro semantico, subreescreve constante -> linha: " << line << endl;
		}
	}


}


//SEGUNDA PASSAGEM
void second_pass(string filename, bool modular, map<string, pair<int,char>> TS){
	int position = 0;
	int line = 1;
	map<string, int> tab_uso;
	stringstream uso;
	stringstream relative;
	vector<string> def;
	pair<string,int> p ;
	string line_string;
	stringstream maq;
	ifstream code(filename + ".pre");
	ofstream out(filename + ".obj");
	string name_mod="";
	bool sum;
	if(code.is_open()){
		while(getline (code,line_string)){
			sum =false;
			vector<string> tokens = split(line_string, ' ');
			 int i=0;
			if(tokens[i].back()== ':' && tokens.size()>1){
				i++;
			}
			//procura instrucao
			p = search(tokens[i], instructions_2);
			if(p.second!=-1){
				position += p.second;
				relative << '0';
				if (p.second>0){
					for ( int k = 0; k < p.second-1; k++){
						relative << '1'; 
					}
				}
				erro(tokens, i, line, TS);
				vector<string> semi_token;
				if (tokens.size() > i+1){
					if(find_plus(tokens) !=  tokens.size()-2)
						sum = true;
				}
				if(tokens.size()==p.second + i || sum){
					maq << string(p.first) << " ";
					if(tokens.size()>1){
						for( int j=i+1; j<tokens.size(); j++){
							try{
								maq << stoi(tokens[j]) << " ";
							}catch(...){
								pair<int,char> b = search(tokens[j]+":", TS);
								semi_token = split(tokens[j], '+');
								if(b.first!=-1){
									int us = search(tokens[j]+":", tab_uso);
									if(us!=-1)
										uso << tokens[j] << " " << position - (tokens.size() - (j-i)) << " ";
									if (j+2<tokens.size() && tokens[j+1]=="+")
										maq << b.first +  stoi(tokens[j+2])  << " ";
									else
										maq << b.first << " ";
								}else{
									cout << "Lexico, Erro simbolo indefinido -> linha: " << line << endl;
								}
							}
						}
					}
				}else{
					cout << "ERRO SINTATICO - operando invalido -> linha: " << line << endl;
				}
			}else{
				int b = search(tokens[i], directives2);
				if(b!=-1){
					if(modular && tokens[i]=="BEGIN")
						name_mod =  tokens[i-1];
					else if(modular && tokens[i]=="EXTERN")
						tab_uso[tokens[i-1]]=0;
					else if(modular && tokens[i]=="PUBLIC") 
						def.push_back(tokens[i+1]);
					//executa sub rotina
					maq << exec_diretive(tokens);
					if(tokens[i]=="CONST")
						relative << "0";
					else if(tokens[i]=="SPACE"){
						relative << "0";
						if(tokens.size()==3){
							for ( int l=0; l< stoi(tokens[i+1]); l++){
								relative << "0";
							};
						}
					}
					position += b; 
				}else{
					cout << "Operacao nao identificada" << "linha: " << line << endl;
					}
			}		
			line++;
		}
		if(modular){
			name_mod.resize (name_mod.size () - 1);
			out << name_mod << endl;
			out << split(maq.str(), ' ').size() << endl;
			out << relative.str() << endl;
			out << name_mod << " " << "0" << " ";
			for ( int i=0; i<def.size(); i++){
				out << def[i] << " " << TS[def[i]+":"].first << " ";
			}
			out << endl;
			out << uso.str() << endl;
		}
		out << maq.str();
		out.close();
	}
}


/*
int main(int argc, char const *argv[])
{
	first_pass("fat_mod_A");
	second_pass("fat_mod_A",true);
	return 0;
}*/