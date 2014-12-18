#include "lstring.h"
#include <string.h>


Lstring::Lstring(string c, string v){
	axiom = c;
	current = c;
	V = v;
}

void Lstring::addRule(Rule *r){
	rules.push_back(r);
}

string Lstring::getGeneration(int index){
	return generations[index];
}

string Lstring::getCurrent(){
	return current;
}

MyRule Lstring::searchRules(char c){
	MyRule rs;
	for (int i = 0; i < rules.size(); i++){
		if (rules[i]->symbol == c)
			rs.push_back(rules[i]);
	}
	return rs;
}

void Lstring::addVariables(string v){
	V = v;
}

bool Lstring::isVariable(char c){
	for (int i = 0; i < V.length(); i++)
		if (c == V[i])
			return true;
	return false;
}

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

void Lstring::generate(){
	for (int i = 0; i < current.length(); ){
		char c = current[i];
		if (!isVariable(c)){
			i++;
			continue;
		}
		MyRule rs = searchRules(c);
		Rule* choose = NULL;
		if (rs.size() == 1)
			choose = rs[0];
		else{
			double random = (double)rand() / RAND_MAX;
			double base = 0.0;
			for (int j = 0; j < rs.size(); j++){
				if (random >= base && random <= (base + rs[j]->prob)){
					choose = rs[j];
					break;
				}
				base += (rs[j]->prob);
			}
		}
		current.replace(i, 1, choose->word);
		i += (choose->word).length();
	}
	generations.push_back(current);
}