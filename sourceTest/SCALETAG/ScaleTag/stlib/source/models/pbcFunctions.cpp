#include "pbcFunctions.h"

//The aim of marcLineParser is to prepare marc coordinate lines for reading

string marcLineParser(string line){
    string InputLineX, InputLineY, InputLineZ;
    //InputLine>>NumPoint;
    InputLineX=line.substr(10,20);
    InputLineY=line.substr(30,20);
    InputLineZ=line.substr(50,20);

    if(InputLineX.find('-',1)>0 && InputLineX.find('-',1)<InputLineX.size()) InputLineX.insert((InputLineX.begin()+InputLineX.find('-',1)),'e');
    if(InputLineX.find('+',1)>0 && InputLineX.find('+',1) <InputLineX.size()) InputLineX.insert((InputLineX.begin()+InputLineX.find('+',1)),'e');

    if(InputLineY.find('-',1)>0 && InputLineY.find('-',1)< InputLineY.size()) InputLineY.insert((InputLineY.begin()+InputLineY.find('-',1)),'e');
    if(InputLineY.find('+',1)>0 && InputLineY.find('+',1)< InputLineY.size()) InputLineY.insert((InputLineY.begin()+InputLineY.find('+',1)),'e');

    if(InputLineZ.find('-',1)>0 && InputLineZ.find('-',1)<InputLineZ.size()) InputLineZ.insert((InputLineZ.begin()+InputLineZ.find('-',1)),'e');
    if(InputLineZ.find('+',1)>0 && InputLineZ.find('+',1)<InputLineZ.size()) InputLineZ.insert((InputLineZ.begin()+InputLineZ.find('+',1)),'e');

    //ligne=InputLineX + ' ' + InputLineY + ' ' + InputLineZ;

    return (InputLineX + ' ' + InputLineY + ' ' + InputLineZ);

}
string createMarcWorkFile(const string& GivenFile){

/* 
The purpose of this function is in long term to create a templated function that can tranform geometric data file from different software like Abaqus, Marc MSC to data format usable by the software in creation

Right now, it is just usable with Marc MSC
*/

// The workFile is created and the name of the file created is returned
//string GivenFile(WorkFile.begin()+4, WorkFile.end());

//string WorkFile="Work"+GivenFile;
string WorkFile="WorkFile.dat";
ofstream sortie(WorkFile, ios::out);
ifstream entree(GivenFile, ios::in);

int NumLigne=-1, NbreLignes, dim, skipdata1, skipdata2;
string ligne;
int NumPoint; double x, y, z;

do{
NumLigne++;
getline(entree, ligne, '\n');
if( (ligne.find("coordinates")) != string::npos ){
entree>>dim>>NbreLignes>>skipdata1>>skipdata2;
sortie<<"coordinates\n";
sortie<<dim<<" "<<NbreLignes<<" "<<skipdata1<<" "<<skipdata2<<"\n";
getline(entree, ligne, '\n');  //Just because the last end of line is not read
for (int i=0; i<NbreLignes; i++){
getline(entree, ligne, '\n');
istringstream InputLine(ligne);
InputLine>>NumPoint;
ligne=marcLineParser(ligne); //put space between field so that they can be easily read
istringstream InputLineXYZ(ligne);
InputLineXYZ>>x>>y>>z;
sortie<<NumPoint<<" "<<scientific<<x<<" "<<y<<" "<<z<<"\n";
}
}

}while(!entree.eof());
sortie.close();
entree.close();
return WorkFile;
}
