#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <vector>



using namespace std;


string input;
vector<int> indicePar;
int compteurEtat = 1;
int compteurTransition = 1;
int tailleMemoire = 0;
string memoire;
int crochet = 0;
const char* fileAutomate = "automate.txt";
const char* fileEtat = "etat.txt";
const char* fileTransition = "transition.txt";


void creation(string input);

//////////////////
///// PARSER /////
//////////////////
void parser(){
//ouverture de input.txt
	char caractere;
	string mot;
	int commentaire = 0;
	string nameFile = "input.txt";
	ifstream fichier(nameFile.c_str(), ios::in);

	if (fichier){
	cout << "Ouverture de " << nameFile << endl;
	while (fichier.get(caractere)){
		if (caractere == '/'){
			commentaire = (commentaire + 1)%2;
		}
		//defini l'expression reguliere
		else if (caractere == ';'){
			input = mot;
			mot = "";
		}
		//defini la taille Memoire
		else if (caractere == ':'){
			memoire = mot;
			tailleMemoire = memoire.size();
			mot = "";
		}
		else if (isalnum(caractere) or caractere == '!' or caractere == '#' or caractere == ',' or caractere == '?' or caractere == '*' or caractere == '+' or caractere == '|' or caractere == '[' or caractere == ']' or caractere == '{' or caractere == '}' or caractere == '(' or caractere == ')' or caractere == '^' or caractere == '.'){
			if (commentaire == 0){
				mot += caractere;
			}
		}
	}
//fermeture du fichier
	fichier.close();
		cout << "Fermeture du fichier : " << nameFile << endl;	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
//initialisation des fichier etat.txt et transition.txt
	ofstream fichier1("etat.txt", ios::out | ios::trunc);
	if(fichier1){
		fichier1.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}ofstream fichier2("transition.txt", ios::out | ios::trunc);
	if(fichier2){
		fichier2.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}

///////////////////
///// OUTFILE /////
///////////////////
void outFile(int taille){
	if (memoire == ""){
		memoire = string(taille,'#');
	}
	ofstream fichier1(fileAutomate);
	if(fichier1){
		ifstream fichier2("etat.txt", ios::in);
		if(fichier2){
			ifstream fichier3("transition.txt", ios::in);
			if(fichier3){
				fichier1 << "/Taille Memoire:" << endl;
				fichier1 << taille << ";" << endl;
				fichier1 << "/Memoire Initial:" << endl;
				fichier1 << memoire << ";"  << endl;
				fichier1 << "/Etat:" << endl;
				fichier1 << "Etat0,true,false,-1;" << endl;
				string ligne;
        			while(getline(fichier2, ligne)){
					fichier1 << ligne << endl;
				}
				fichier1 << "Etat" << compteurEtat << ",false,true,-1;" << endl;
				fichier1 << "/Transition:" << endl;
        			while(getline(fichier3, ligne)){
					fichier1 << ligne << endl;
				}
				fichier1 << "Transition" << compteurTransition << ",Etat" << compteurEtat-1 << ",Etat" << compteurEtat << ",-1;" << endl;		
				fichier3.close();	
			}
			else{
				cerr << "Impossible d'ouvrir le fichier !" << endl;
			}
			fichier2.close();	
		}
		else{
			cerr << "Impossible d'ouvrir le fichier !" << endl;
		}		
		fichier1.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}

////////////////////////
///// VERIFICATION /////
////////////////////////
void verification(){
//ajout des '.' pour la concatenation
	int indiceInput = 0;
	while (indiceInput < input.size()-1){
		if ((isalnum(input[indiceInput]) or input[indiceInput] == ']' or input[indiceInput] == ')' or input[indiceInput] == '!' or input[indiceInput] == '*' or input[indiceInput] == '+') and (input[indiceInput+1] == '(' or input[indiceInput+1] == '[' )){
			input.insert(input.begin()+indiceInput+1,'.');
		}
		else if ((input[indiceInput] == ']' or input[indiceInput] == ')' or input[indiceInput] == '!' or input[indiceInput] == '*' or input[indiceInput] == '+') and (input[indiceInput+1] == '(' or input[indiceInput+1] == '[' or isalnum(input[indiceInput+1]))){
			input.insert(input.begin()+indiceInput+1,'.');
		}
		indiceInput++;
	}

}


///////////////
///// CUT /////
///////////////
string cut(string expr, int indice){
	string exprCut = "";
	int indiceExpr = 0;
	while (indiceExpr < indice){
		exprCut += expr[indiceExpr];
		indiceExpr++;
	}
	return exprCut;
}

//////////////////
///// ETOILE /////
//////////////////
void etoile(int etatInitial){
//transition *
	ofstream fichier(fileTransition, ios::out | ios::app);
	if(fichier){
		fichier << "Transition" << compteurTransition << ",Etat" << compteurEtat-1 << ",Etat" << etatInitial << ",-1;" << endl;
		compteurTransition++;
		fichier.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}


/////////////////////
///// OPTIONNEL /////
/////////////////////
void optionnel(int etatInitial){
//transition ?
	ofstream fichier(fileTransition, ios::out | ios::app);
	if(fichier){
		fichier << "Transition" << compteurTransition << ",Etat" << etatInitial << ",Etat" << compteurEtat-1 << ",-1;" << endl;
		compteurTransition++;
		fichier.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}

/////////////////////
///// CONCAT ////////
/////////////////////
void concat(int etatInitial,int etatFinal){
//transition .
	ofstream fichier(fileTransition, ios::out | ios::app);
	if(fichier){
		fichier << "Transition" << compteurTransition << ",Etat" << etatInitial << ",Etat" << etatFinal << ",-1;" << endl;
		compteurTransition++;
		fichier.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}

//////////////////////
///// PARENTHESE /////
//////////////////////
int parenthese(){
//creation d'un etat pour l'ouverture ou la fermeture de parenthese
	ofstream fichier(fileEtat, ios::out | ios::app);
	if(fichier){
		fichier << "Etat" << compteurEtat << ",false,false,-1;" << endl;
		compteurEtat++;
		fichier.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
	return compteurEtat-1;
}

////////////////////
///// MULTI IN /////
////////////////////
void multiIn(vector<int> etatInitial){
//creation d'un etat relié a tous les etats initiaux pour une transition | ou [...]
	ofstream fichier(fileTransition, ios::out | ios::app);
	if(fichier){
		int indice = 0;
		while (indice < etatInitial.size()){
			fichier << "Transition" << compteurTransition << ",Etat" << compteurEtat << ",Etat" << etatInitial[indice] << ",-1;" << endl;
			compteurTransition++;
			indice++;
		}
		fichier.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
	ofstream fichier1(fileEtat, ios::out | ios::app);
	if(fichier1){
		fichier1 << "Etat" << compteurEtat << ",false,false,-1;" << endl;
		compteurEtat++;
		fichier1.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}

/////////////////////
///// MULTI OUT /////
/////////////////////
void multiOut(vector<int> etatFinal){
//creation d'un etat relié a tous les etats finaux pour une transition | ou [...]
	ofstream fichier(fileTransition, ios::out | ios::app);
	if(fichier){
		int indice = 0;
		while (indice < etatFinal.size()){
			fichier << "Transition" << compteurTransition << ",Etat" <<  etatFinal[indice] << ",Etat" << compteurEtat << ",-1;" << endl;
			compteurTransition++;
			indice++;
		}
		fichier.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
	ofstream fichier1(fileEtat, ios::out | ios::app);
	if(fichier1){
		fichier1 << "Etat" << compteurEtat << ",false,false,-1;" << endl;
		compteurEtat++;
		fichier1.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}


/////////////////////////////////
///// Verification Terminal /////
/////////////////////////////////
string verificationTerminal(string expr){
// verifie si un terminal est bien composé uniquement de Chiffre( voir le corrige)
	int indice=0;
	while (indice< expr.size()){
		if (isdigit(expr[indice]) == false){
			cout << "Un terminal est incorrecte : " << expr << endl;
			expr.erase(expr.begin()+indice);
		}
		indice++;
	}
	return expr;
}

////////////////////
///// Terminal /////
////////////////////
void terminal(string expr){
// creation des etats et transitions pour un Terminal sans modification de memoire
	expr = verificationTerminal(expr);
	int intExpr = atoi(expr.c_str());
	if (intExpr > tailleMemoire){
		tailleMemoire = intExpr+1;
		cout << "Attention la taille de la memoire vient d'etre augmente a "<< tailleMemoire << endl;
	}
	ofstream fichier(fileTransition, ios::out | ios::app);
	if(fichier){
		fichier << "Transition" << compteurTransition << ",Etat" << compteurEtat << ",Etat" << compteurEtat+1 << "," << expr << ";" << endl;
		compteurTransition++;
		fichier.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
	ofstream fichier1(fileEtat, ios::out | ios::app);
	if(fichier1){
		fichier1 << "Etat" << compteurEtat << ",false,false,-1;" << endl;
		compteurEtat++;			
		fichier1 << "Etat" << compteurEtat << ",false,false,-1;" << endl;
		compteurEtat++;
		fichier1.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}


////////////////////////////
///// Terminal Ecriture/////
////////////////////////////
void terminalWrite(string expr){
// creation des etats et transitions pour un Terminal avec modification de memoire
	verificationTerminal(expr);
	int intExpr = atoi(expr.c_str());
	if (intExpr > tailleMemoire){
		tailleMemoire = intExpr+1;
		cout << "Attention la taille de la memoire vient d'etre augmente a "<< tailleMemoire << endl;
	}
	ofstream fichier(fileTransition, ios::out | ios::app);
	if(fichier){
		fichier << "Transition" << compteurTransition << ",Etat" << compteurEtat << ",Etat" << compteurEtat+1 << "," << expr << ";" << endl;
		compteurTransition++;
		fichier.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
	ofstream fichier1(fileEtat, ios::out | ios::app);
	if(fichier1){
		fichier1 << "Etat" << compteurEtat << ",false,false," << expr << ";" << endl;
		compteurEtat++;			
		fichier1 << "Etat" << compteurEtat << ",false,false,-1;" << endl;
		compteurEtat++;
		fichier1.close();	
	}
	else{
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}

//////////////////////
///// EXPRESSION /////
//////////////////////
int expression(string expr){
//fonction gerant les differentes expressions 
	int etatFinalPrec = compteurEtat-1;
	cout << "Expr " << expr << endl;
	// ()* ou ()+ ou ()? ou ()|... ou ()|()
	if (expr[0] == '('){
		string exprPar = expr;
		int indice = 0;
		int indiceSave = expr.size();
		int compteurParenthese = 0;
		while (indice < expr.size()){
			if (expr[indice] == '|'and compteurParenthese == 0){
				exprPar.erase(exprPar.begin()+indice+1,exprPar.end());
				indiceSave = indice;
			}
			if (expr[indice] == '('){
			compteurParenthese++;				
			}
			if (expr[indice] == ')'){
			compteurParenthese--;				
			}
			indice++;
		}
		int in = parenthese();
		exprPar.erase(exprPar.end()-1);
		if (exprPar[exprPar.size()] == ')'){
			exprPar.erase(exprPar.begin()+0);
			exprPar.erase(exprPar.end()-1);
		}
		else{
			exprPar.erase(exprPar.end()-1);
		}
		creation(exprPar);
		int out = parenthese();
		concat(compteurEtat-2,out);
		if (expr[indiceSave-1] == '*'){
			concat(out,in);
		}
		else if (expr[indiceSave-1] == '?'){
			optionnel(etatFinalPrec);
		}
		else if (expr[indiceSave-1] == '+'){
			creation(exprPar);
			etoile(compteurEtat-4);
		}
		if (expr[indiceSave] == '|'){
			vector<int> etatInitial;
			etatInitial.push_back(in);
			etatInitial.push_back(compteurEtat);
			vector<int> etatFinal;
			etatFinal.push_back(out);
			expr.erase(expr.begin()+0,expr.begin()+indiceSave+1);
			int indice = 0;
			while (isdigit(expr[indice])){
				indice++;
			}	
			if (expr[indice] == '!'){
				terminalWrite(cut(expr,indice));
				indice++;
			}
			else if (expr[indice] != '(' ){
				terminal(cut(expr,indice));
			}
			if (expr[indice] == '*'){
				etoile(etatInitial[1]);
			}
			if (expr[indice] == '?'){
				optionnel(etatInitial[1]);
			}
			if (expr[indice] == '+'){
				compteurEtat--;
				if (expr[indice-1] == '!'){			
					terminalWrite(cut(expr,indice-1));
					etoile(compteurEtat-2);
				}
				else if (expr[indice] != '(' ){
					terminal(cut(expr,indice));
				}
			}
			if (expr[indice] == '('){
				expr.erase(expr.begin()+0,expr.begin()+indice);
				int in = parenthese();
				string exprPar = expr;
				exprPar.erase(exprPar.begin()+0);
				exprPar.erase(exprPar.end()-1);
				creation(exprPar);
				int out = parenthese();
				concat(compteurEtat-2,out);
				if (expr[expr.size()-1] == '*'){
					concat(out,in);
					return out;
				}
				else if (expr[expr.size()-1] == '?'){
					optionnel(etatFinalPrec);
					return out;
				}
				else if (expr[expr.size()-1] == '+'){
					creation(exprPar);
					etoile(compteurEtat-4);
					return out;
				}
				concat(compteurEtat,in);
			}
			etatFinal.push_back(compteurEtat-1);
			multiIn(etatInitial);
			concat(etatFinalPrec,compteurEtat-1);				
			multiOut(etatFinal);
			return compteurEtat-2;
		}
		else{
			concat(etatFinalPrec,in);
			return out;
		}
	}
	// [ ... ] ou [...]* ou [...]+ ou [...]? 
	else if (expr[0] == '['){
		expr.erase(expr.begin()+0);
		crochet = 1;
		int indice = 0;
		string mot = "";
		vector<int> etatInitial;
		vector<int> etatFinal;
		// [^...]
		if (expr[0] == '^'){
			expr.erase(expr.begin()+0);
			// creation d'une liste correspondant a toute les cases memoires
			vector<int> list;
			while (indice < tailleMemoire){
				list.push_back(indice);
				indice++;
			}
			// suppression de case de la liste 
			indice = 0;
			while (indice < expr.size()){
				if (expr[indice] == ',' or expr[indice] == ']'){
					int indiceMot = 0;
					while (indiceMot < list.size()){
						if (atoi(mot.c_str()) == list[indiceMot]){
							list.erase(list.begin()+indiceMot);
						}
						indiceMot++;
					}
					mot = "";
				}
				else{
					mot += expr[indice];
				}
				indice++;
			}
			// creation des cases restantes
			indice = 0;
			while (indice < list.size()){
				ostringstream convert;
				convert << list[indice];
				mot = convert.str(); 
				cout << "Mot " << mot << endl;				
				etatInitial.push_back(expression(mot));
				etatFinal.push_back(compteurEtat-1);
				indice++;
			}
		}
		// [...]
		else {
			while (indice < expr.size()){
				if (expr[indice] == ',' or expr[indice] == ']'){
					if (expr[indice-1] == '!'){
						terminalWrite(mot);
						etatInitial.push_back(compteurEtat-2);
						etatFinal.push_back(compteurEtat-1);
						mot = "";
					}
					else{	
						terminal(mot);
						etatInitial.push_back(compteurEtat-2);
						etatFinal.push_back(compteurEtat-1);
						mot = "";
					}
				}
				else{
					mot += expr[indice];
				}
				indice++;
			}
		}
		multiIn(etatInitial);
		concat(etatFinalPrec,compteurEtat-1);
		multiOut(etatFinal);
		// [...]*
		if (expr[expr.size()-1] == '*'){
			etoile(compteurEtat-2);			
			crochet = 0;
			return compteurEtat-2;
		}
		// [...]?
		else if (expr[expr.size()-1] == '?'){
			optionnel(compteurEtat-2);			
			crochet = 0;
			return compteurEtat-2;
		}
		// [...]+
		else if (expr[expr.size()-1] == '+'){
			int etatFinalPrec = compteurEtat-1;
			indice = 0;
			string mot = "";
			vector<int> etatInitial;
			vector<int> etatFinal;
			while (indice < expr.size()){
				if (expr[indice] == '.' or expr[indice] == ']'){				
					etatInitial.push_back(expression(mot));
					etatFinal.push_back(compteurEtat-1);
					mot = "";
				}
				else{
					mot += expr[indice];
				}
				indice++;
			}
			multiIn(etatInitial);
			//transition via fonction optionnel		
			optionnel(etatFinalPrec);
			multiOut(etatFinal);
			etoile(compteurEtat-2);
			crochet = 0;
			return compteurEtat-4;
			
		}
		else if (expr[expr.size()-1] == ']'){			
			crochet = 0;
			return compteurEtat-2;
		}
	}
	// ... | ... et ...
	else{
		int indice=0;
		vector<int> noDigitIndice;
		vector<char> noDigit;
		// Recherche des caracteres
		while (indice < expr.size()){
			if (isdigit(expr[indice]) == false){
				noDigitIndice.push_back(indice);
				noDigit.push_back(expr[indice]);
			}
			if (expr[indice] == '|' and expr[indice+1] == '(' and (expr[indice-1] == ')' or expr[indice-2] == ')')){
				expr = '(' + expr + ')';
				expression(expr);
				return compteurEtat-2;
			}
			indice++;
		}
		indice=0;
		if (noDigit.size() == 0){
			terminal(expr);
			if (crochet == 0){
				concat(etatFinalPrec,compteurEtat-2);
			}
			return compteurEtat-2;
		}
		else {
			vector<int> etatInitial;
			etatInitial.push_back(compteurEtat);
			while (isdigit(expr[indice])){
				indice++;
			}
			if (expr[indice] == '!'){
				terminalWrite(cut(expr,indice));
				indice++;
			}
			else{
				terminal(cut(expr,indice));
			}
			if (expr[indice] == '*'){
				etoile(etatInitial[0]);
				indice++;
			}
			if (expr[indice] == '?'){
				optionnel(etatInitial[0]);
				indice++;
			}
			if (expr[indice] == '+'){
				compteurEtat--;
				if (expr[indice-1] == '!'){
					terminalWrite(cut(expr,indice-1));
					etoile(compteurEtat-2);
					indice++;
				}
				else {
					terminal(cut(expr,indice));
					etoile(compteurEtat-2);
					indice++;
				}
			} 
			if (expr[indice] == '|'){
				vector<int> etatFinal;
				etatFinal.push_back(compteurEtat-1);
				indice++;
				expr.erase(expr.begin()+0,expr.begin()+indice);
				indice=0;
				etatInitial.push_back(compteurEtat);
				while (isdigit(expr[indice])){
					indice++;
				}	
				if (expr[indice] == '!'){
					terminalWrite(cut(expr,indice));
					indice++;
				}
				else if (expr[indice] != '(' ){
					terminal(cut(expr,indice));
				}
				if (expr[indice] == '*'){
					etoile(etatInitial[1]);
				}
				if (expr[indice] == '?'){
					optionnel(etatInitial[1]);
				}
				if (expr[indice] == '+'){
					compteurEtat--;
					if (expr[indice-1] == '!'){			
						terminalWrite(cut(expr,indice-1));
						etoile(compteurEtat-2);
					}
					else{			
						terminal(cut(expr,indice));
						etoile(compteurEtat-2);
					}
				}
				if (expr[indice] == '('){
					int in = parenthese();
					string exprPar = expr;
					exprPar.erase(exprPar.begin()+0);		
					exprPar.erase(exprPar.end()-1);
					creation(exprPar);
					int out = parenthese();
					concat(compteurEtat-2,out);
					if (expr[expr.size()-1] == '*'){
						concat(out,in);
						return out;
					}
					else if (expr[expr.size()-1] == '?'){
						optionnel(etatFinalPrec);
						return out;
					}
					else if (expr[expr.size()-1] == '+'){
						creation(exprPar);
						etoile(compteurEtat-4);
						return out;
					}
				}
				etatFinal.push_back(compteurEtat-1);
				multiIn(etatInitial);
				concat(etatFinalPrec,compteurEtat-1);				
				multiOut(etatFinal);
				return compteurEtat-2;
			}
			else {				
				concat(etatFinalPrec,etatFinalPrec+1);
				return etatInitial[0];
			}
		}
	}
}

////////////////////
///// CREATION /////
////////////////////
void creation(string input){
// fonction servant a découper l'expression reguliere initial en expressions plus simple
	int indiceInput = 0;
	int indiceDebut = 0;
	string expr = "";
	int parenthese = 0;
	int crochetCreation = 0;
	while (indiceInput <= input.size()){
		if (input[indiceInput] == '('){
			parenthese++;
		}
		else if (input[indiceInput] == ')'){
			parenthese--;
		}
		if (input[indiceInput] == '['){
			crochetCreation++;
		}
		else if (input[indiceInput] == ']'){
			crochetCreation--;
		}
		if ((input[indiceInput] == '.' or indiceInput == input.size()) and parenthese == 0 and crochetCreation == 0){
			if (expr[0] == '(' and expr[expr.size()-1] == ')'){
				expr.erase(expr.begin()+0);				
				expr.erase(expr.end()-1);
				creation(expr);
				expr = "";
			}
			else {
				expression(expr);
				expr = "";
				}
		}
		else{
			expr.push_back(input[indiceInput]);
		}
		indiceInput++;
	}
}

int main(){
//fonction main
parser();
cout << "Input : " << input << endl;
verification();
cout << "Input : " << input << endl;
creation(input);
outFile(tailleMemoire);
return 0;
}
