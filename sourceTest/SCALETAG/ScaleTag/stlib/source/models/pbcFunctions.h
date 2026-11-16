#ifndef pbcFunctions_H
#define pbcFunctions_H

#include<iostream>
#include<fstream>
#include<sstream>

using namespace std;

string createMarcWorkFile(const string& GivenFile);
//{


/* 
The purpose of this function is in long term to create a templated function that can tranform geometric data file from different software like Abaqus, Marc MSC to data format usable by the software in creation

Right now, it is just usable with Marc MSC
*/

// The workFile is created and the name of the file created is returned
//string GivenFile(WorkFile.begin()+4, WorkFile.end());

/*string WorkFile="Work"+GivenFile;
ofstream sortie(WorkFile, ios::out);
ifstream entree(GivenFile, ios::in);

int i, NumLigne=-1, NbreLignes, dim, skipdata1, skipdata2;
string ligne;
int NumPoint; double x, y, z;
int taille;
do{
NumLigne++;
getline(entree, ligne, '\n');
//cout<<ligne<<endl;
//taille=ligne.size();
i=ligne.find("coordinates");
//if (i<0 || i>taille){continue;} else exit;
}while(!entree.eof() && ((i<0 || i>70))); //(Au lieu de i>70, mettre i>11( longueur du mot coordinates) )
entree>>dim>>NbreLignes>>skipdata1>>skipdata2;

//cout<<"dim : "<<dim<<endl;
//cout<<"Number of vertices : "<<NbreLignes<<endl;

sortie<<dim<<" "<<NbreLignes<<" "<<skipdata1<<" "<<skipdata2<<"\n";
getline(entree, ligne, '\n');
for (int i=0; i<NbreLignes; i++){
getline(entree, ligne, '\n');
//cout<<entree.gcount();

istringstream InputLine(ligne);
string InputLineX, InputLineY, InputLineZ;
//InputLine>>setw(10)>>NumPoint>>setw(20)>>InputLineX>>setw(20)>>InputLineY>>setw(20)>>InputLineZ;
InputLine>>NumPoint;
InputLineX=ligne.substr(10,20);
InputLineY=ligne.substr(30,20);
InputLineZ=ligne.substr(50,20);

if(InputLineX.find('-',1)>0 && InputLineX.find('-',1)<InputLineX.size()) InputLineX.insert((InputLineX.begin()+InputLineX.find('-',1)),'e');
if(InputLineX.find('+',1)>0 && InputLineX.find('+',1) <InputLineX.size()) InputLineX.insert((InputLineX.begin()+InputLineX.find('+',1)),'e');

if(InputLineY.find('-',1)>0 && InputLineY.find('-',1)< InputLineY.size()) InputLineY.insert((InputLineY.begin()+InputLineY.find('-',1)),'e');
if(InputLineY.find('+',1)>0 && InputLineY.find('+',1)< InputLineY.size()) InputLineY.insert((InputLineY.begin()+InputLineY.find('+',1)),'e');

if(InputLineZ.find('-',1)>0 && InputLineZ.find('-',1)<InputLineZ.size()) InputLineZ.insert((InputLineZ.begin()+InputLineZ.find('-',1)),'e');
if(InputLineZ.find('+',1)>0 && InputLineZ.find('+',1)<InputLineZ.size()) InputLineZ.insert((InputLineZ.begin()+InputLineZ.find('+',1)),'e');

ligne=InputLineX + ' ' + InputLineY + ' ' + InputLineZ;

istringstream InputLineXYZ(ligne);

InputLineXYZ>>x>>y>>z;
//sortie<<setw(10)<<NumPoint<<" "<<scientific<<setw(21)<<x<<" "<<setw(22)<<y<<" "<<setw(22)<<z<<"\n";
sortie<<NumPoint<<" "<<scientific<<x<<" "<<y<<" "<<z<<"\n";
}
sortie.close();
entree.close();
return WorkFile;
}*/

string marcLineParser(string line="");
#endif
