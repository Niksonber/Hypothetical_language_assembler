#include <iostream>
#include <sstream>
#include <algorithm>
#include "../include/prep.hpp"
// #include "../include/lexer.hpp"

bool read_source_line(std::string line, vector_of_tokens* Tokens, int *linha){
    if (line.empty())   return false;
    Token Token_1;
    while (line[0] == ' ') line.erase(0,1); // Trim Spaces
    if    (line.size() == 1){
        *linha= *linha+1;
        return false;
    }    
    if    (line[0] == ';') return false;  // Ignore Comments
    std::string buf;            // Have a buffer std::string
    std::stringstream ss(line); // Insert the std::string into a stream

    while (ss  >> buf){
        std::transform(buf.begin(), buf.end(),buf.begin(), ::toupper);
        if (buf.compare(0,1,";")==0){
            break;
        }
        Token_1.text = buf;
        Token_1.line = *linha;
        Tokens->push_back(Token_1);
    }
    *linha=*linha+1;
    return true;
}

bool treating_if(vector_of_tokens* Tokens){
    std::string label;
    int ifzero = 0;
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if (it->label_equ == true){
            for (vector_of_tokens::iterator it_2 = it ; it_2 != Tokens->end(); ++it_2){
                label = it->text;
                label.erase(label.size()-1,1);
                if(it_2->text.compare(label) == 0){
                    it_2->text = (it+2)->text;
               } 
            }
            Tokens->erase(it);
            Tokens->erase(it);
            Tokens->erase(it);
            it=it-1;

        }
    }

    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if (it->text.compare("IF") == 0){
            if ((it+1) != Tokens->end()){
                if ((it+1)->text.compare("1") == 0){
                  Tokens->erase(it);
                  Tokens->erase(it);
                  it = it-1;  
                }
                else{
                    if((it+2) != Tokens->end()){
                        ifzero = (it+2)->line;
                        while((it+2) != Tokens->end() && ((it+2)->line)==ifzero){
                            Tokens->erase(it+2);   
                        }
                    }
                Tokens->erase(it);
                Tokens->erase(it);
                }
            }
        }
    }
    return true;
}

void update_label_flags(vector_of_tokens* Tokens){
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if((it+1) != Tokens->end()){
            if (it->text.compare(((it->text.size())-1),1,":") == 0){
              if((it+1)->text.compare("EQU") == 0){
                it->label_equ = true;
              }
              else{
                it->label = true;
              } 
            }
        }
    }
}

void label_tokens(vector_of_tokens* Tokens){
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if(it != Tokens->begin()){
            if(it->text.compare(0,1,":") == 0){
                Tokens->erase(it);
                (it-1)->text =(it-1)->text +':';
            }
        }
    }
}

void if_equ_output(vector_of_tokens* Tokens,vector_of_strings* output){
    int linha = Tokens->begin()->line;
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if (linha != it->line){
            output->push_back("\n");
            linha= it->line;
        }
        output->push_back(it->text + " ");
    }
}

void separe_copy_tokens(vector_of_tokens *Tokens){
    bool Flag_copy = false;
    Token Token_1;
    int linha = 0;
    std::string temp_str;
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if(Flag_copy == true){//separe where are commas into differents tokens
            std::stringstream temp((it)->text);
            linha = (it->line);
            it = Tokens->erase(it);
            while( temp.good() ){
            std::getline( temp, temp_str, ',' );
            Token_1.text = temp_str;
            Token_1.line = linha;
            (it) = Tokens->insert(it,Token_1);
            it++;
            }
            Flag_copy = false;
        }
        if (it->text.compare("COPY") == 0){
            Flag_copy = true;//set flag to true to separe tokens with commas
        }
    }
}

void section_erros(vector_of_tokens* Tokens){
    bool Flag_section_text = false;
    bool Flag_section_data = false;

    if(Tokens->size()>=2){
        vector_of_tokens::iterator it = Tokens->begin()+1;
        for (; it != Tokens->end(); ++it){
            if((it-1)->text.compare("SECTION")==0){
                if(it->text.compare("TEXT")==0){
                    Flag_section_text = true;
                }
                else if(it->text.compare("DATA")==0){
                    Flag_section_data = true;
                }
                else{
                    it->flag(DIF_SECTION);  
                }
            }
        }
    }
    if(!Flag_section_data){
        std::cout<<"(semantic error) Section DATA missing"<<std::endl;
    }
    if(!Flag_section_text){
        std::cout<<"(semantic error) Section TEXT missing"<<std::endl;
    }
}

void double_label_line(vector_of_tokens* Tokens){
    int linha = Tokens->begin()->line;
    bool Flag_label = false;
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if (linha != it->line){
            linha= it->line;
            Flag_label = false;
        }
        if(Flag_label &&(it->label_equ  || it->label)){
            it->flag(DOUBLE_LABEL_LINE);
        }
        if(it->label_equ || it->label){
           Flag_label = true; 
        }

    }  

}
void label_redeclared(vector_of_tokens* Tokens){
    std::string temp;
    vector_of_tokens::iterator it_2;
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if(it->label_equ  || it->label){
            temp = it->text;
            it_2 = it+1;
            for(;it_2 !=Tokens->end();it_2++){
                if(it_2->text.compare(temp)==0){
                    it_2->flag(REDECLARATION);
                }
            }
        }
    } 
}

void lexical_error(vector_of_tokens* Tokens){
    int tamanho;
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        tamanho = it->text.size();
        if (tamanho>20){
            it->flag(ILLEGAL_NAME);
        }
        for(int i = 0;i<tamanho;i++){
            if((it->text[i] > 90) &&(it->text[i] != 95)){//depois dos maiusculos
              it->flag(ILLEGAL_NAME);  
            }
            else if((it->text[i] < 65) &&(it->text[i] > 58)){//entre maiusculo e :
              it->flag(ILLEGAL_NAME);  
            }
            else if((it->text[i] < 48) && (it->text[i] > 32) && (it->text[i] != 38) && (it->text[i] != 44)){//entre 0 e espaço
              it->flag(ILLEGAL_NAME);  
            }
        }

    }
    if(Tokens->size()>=2){    
         for (vector_of_tokens::iterator it = Tokens->begin()+1 ; it != Tokens->end(); ++it){
            tamanho = it->text.size();
            for(int i = 0;i<tamanho;i++){
                if((it->text[0] > 47) && (it->text[0] < 58) && ((it-1)->text.compare("CONST")!=0) && ((it-1)->text.compare("EQU")!=0)){//começa por num e n vem depois de const
                    it->flag(ILLEGAL_NAME);   
                }else if((it->text[i] == ',') && ((it-1)->text.compare("COPY")!=0)){//tem , e nao vem depois de copy
                   it->flag(ILLEGAL_NAME);
                } 
            }
        }
    }     
}

void printing_errors(vector_of_tokens* Tokens){
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if (it->haserror){
            error::print(it->errcode, it->line,it->text);
        }

    } 

}


void preprocess(vector_of_strings& file, vector_of_strings* output, bool flag,vector_of_tokens* Tokens){
    if(file.empty()) return;
    int linha = 1;
    for (auto l : file){
        read_source_line(l,Tokens,&linha);//make the vector of tokens
        #ifdef DEBUG_PREP_INPUT
            std::cout << line << std::endl;
        #endif // DEBUG_PREP_INPUT
    }

    label_tokens(Tokens);// if there is a token ':', put : in the final of previous token
    section_erros(Tokens);
    update_label_flags(Tokens);// update label Flags
    double_label_line(Tokens);
    label_redeclared(Tokens);
    lexical_error(Tokens);


    #ifdef DEBUG_PREP_PRINT_TOKENS
    linha = Tokens->begin()->line;
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if (linha != it->line){
            std::cout << '\n';
            linha= it->line;
        }
        std::cout << it->text<<'('<<it->line<<')'<<' '
        <<"eq"<<it->label_equ <<"label"
        <<it->label<<" ";
    }
    std::cout << '\n';
    #endif

    treating_if(Tokens);//treating IF EQU clauses

    #ifdef DEBUG_PREP_OUTPUT
    linha = Tokens->begin()->line;
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if (linha != it->line){
            std::cout << '\n';
            linha= it->line;
        }
        std::cout << it->text<<' ';
    } 
    #endif
    if(!flag){
        if_equ_output(Tokens,output);//make the outputfile when -p
        return ; 
    }
 

    separe_copy_tokens(Tokens);

    linha = Tokens->begin()->line;
    for (vector_of_tokens::iterator it = Tokens->begin() ; it != Tokens->end(); ++it){
        if (linha != it->line){
            output->push_back("\n");
            linha= it->line;
        }
        if(it !=Tokens->begin()){
            if((it-1)->text.compare("COPY") == 0){
               output->push_back(it->text + ","); 
            }else{
                output->push_back(it->text + " ");
            }
        }else{
            output->push_back(it->text + " ");
        }
    }

}