/**
 * @file main.cpp
 * @author DECSAI
 * @note To be implemented by students either completely or by giving them
 * key functions prototipes to guide the implementation
 */

#include <iostream>
#include <string>
#include <fstream>

#include "language.h"
#include "bag.h"
#include "move.h"
#include "player.h"
#include "movelist.h"
using namespace std;

#define ERROR_ARGUMENTS 1
#define ERROR_OPEN 2
#define ERROR_DATA 3
/**
 * @brief Reports an important error and exits the program
 * @param errorcode An integer representing the error detected, which is represented
 * by several symbolic constants: ERROR_ARGUMENTS, ERROR_OPEN y ERROR_DATA
 * @param errorinfo Additional information regarding the error: "" (empty string) for 
 * errors parsing the arguments to main() and, for the case of errors opening or 
 * reading/writing data, the name of the file thas has failed.
 */
void errorBreak(int errorcode, const string & errorinfo);

/**
 * @brief Shows final data
 * @param l Language
 * @param random Random seed
 * @param b Final bag
 * @param p Final player
 * @param original
 * @param legal
 * @param accepted
 * @param rejected
 */
void HallOfFame(const Language &l, int random, const Bag &b, const Player &p, 
        const Movelist& original,const Movelist& legal,
        const Movelist& accepted,const Movelist& rejected);



/**
 * @brief Main function. 
 * @return 
 */
int main(int nargs, char * args[]) {
    Bag bag;
    Player player;
    Language language;
    Move move;
    Movelist movements,        /// Original list of movements
            legalmovements,    /// Movements with legal words upon the dictionary
            acceptedmovements, /// Movements accepted in the game
            rejectedmovements; /// Movements not accepted in the game
    string lang="", goodmoves="", badmoves="", ifilename="", ofilename="";
    int random=-1, nwords=0, score=0;
    ifstream ifile; ofstream ofile;
    istream *input=&cin; ostream *output;
    bool Lobligatorio=false, Pobligatorio = false, haybolsa=false;
    int contador=0;
    string fichero="",bolsa="";
    /// @warning: Declare more vars when needed
    
    /// @warning: Check arguments

    if(nargs < 3 || nargs > 9){
        
        errorBreak(1,"");
    }
    
    for(int i = 1; i < nargs-1; i++){
        
        string parametro = args[i];
        string siguienteparametro = args[i+1];
        
        
        if(parametro == "-l"){
                
            lang=siguienteparametro;
            language.setLanguage(lang);
            cout <<"LANGUAGE: "<<lang<<endl;
            cout << "ALLOWED LETTERS: " << toUTF(language.getLetterSet()) <<endl;
            Lobligatorio=true;
        }
        
        else if(parametro == "-p"){
            Pobligatorio = true;
             ifilename=siguienteparametro;
            ifile.open(ifilename.c_str());
            
            if(!ifile){
                
                errorBreak(2,ifilename);
            }
            
            input=&ifile;
            
            
        }

        else if(parametro == "-r"){
            
            random=stoi(siguienteparametro);
        }
        else if(parametro == "-b"){
            
            bolsa=siguienteparametro;
            haybolsa=true;
        }
    }
    
    
    if(!Lobligatorio or !Pobligatorio){
        
        errorBreak(1,"");
    }
    
    if(haybolsa==false){
        
        bag.setRandom(random);
        bag.define(language);
        cout<<"SEED: "<<random<<endl;
        cout<<"BAG"<<" ("<<bag.size()<<") :"<<bag.to_string()<<endl;
    }
    else{
        
        bolsa=toISO(bolsa);
        bag.set(bolsa);
        cout<<"SEED: "<<random<<endl;
        cout<<"BAG"<<" ("<<bag.size()<<") :"<<bag.to_string()<<endl;
    }
    

    cout<<"Reading from "<<ifilename<<endl;
    bool leido = movements.read(*input);

    if(!leido){
       errorBreak(3, ifilename);
    }
    
    
    else{
        
        //Crea Lista de movimientos legales en el lenguaje
        legalmovements.assign(movements);
        legalmovements.zip(language);  

        
        for(int i = 0; i < movements.size(); i++){
            string extraer=bag.extract(7-player.size());

            player.add(extraer);

            cout<<"PLAYER: "<<player.to_string()<<endl<<endl;
            
            
            Move aux = movements.get(i);
            string letrasMov = aux.getLetters();
            
            if(letrasMov.size() > 1){
                if(player.isValid(letrasMov)){
                    if(language.query(letrasMov)){
                        int puntuacion = aux.findScore(language);
                        aux.setScore(puntuacion);
                        acceptedmovements.add(aux);

                        cout <<"FOUND! "<<puntuacion<<" points"<<endl<<endl;

                    }
                    
                    else{
                        cout<<"NOT REGISTERED!"<<endl<<endl;

                    }
                    
                    player.extract(letrasMov);
                }
                
                else{
                    cout<<"INVALID!"<<endl<<endl;
                    rejectedmovements.add(aux);

                }
            }
        }
         
    }
        
    if(input != &cin){
        
        ifile.close();
    }
    


    HallOfFame(language, random, bag, player, 
            movements, legalmovements, acceptedmovements, rejectedmovements);
    return 0;
}

void HallOfFame(const Language &l, int random, const Bag &b, const Player &p, 
        const Movelist& original,const Movelist& legal,
        const Movelist& accepted,const Movelist& rejected) {
    cout << endl << "%%%OUTPUT" << endl << "LANGUAGE: "<<l.getLanguage()<< " ID: " << random << endl;
    cout << "BAG ("<<b.size()<<"): " << toUTF(b.to_string()) << endl;
    cout << "PLAYER (" <<p.size() << "): " << toUTF(p.to_string());
    cout << endl << endl << "ORIGINAL ("<<original.size()<<"): "<<endl; original.print(cout);
    cout << endl << endl << "LEGAL ("<<legal.size()<<"): "<<endl; legal.print(cout);
    cout << endl << endl << "ACCEPTED ("<<accepted.size()<<") SCORE "<<accepted.getScore()<< ": "<<endl; accepted.print(cout);
    cout << endl << endl << "REJECTED ("<<rejected.size()<<"): "<<endl; rejected.print(cout);
    cout << endl;
}

void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use:\n -l <language> -p <playfile> [-r <randomnumber>]"<<endl;
            break;
        case ERROR_OPEN:
            cerr<<"Error opening file "<<errordata << endl;
            break;
        case ERROR_DATA:
            cerr<<"Data error in file "<<errordata << endl;
            break;
    }
    std::exit(1);
}