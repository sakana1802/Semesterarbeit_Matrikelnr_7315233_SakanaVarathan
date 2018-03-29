#ifndef NTOKEN3_H
#define NTOKEN3_H
#include <fstream>
#include "natt3.h"
using namespace std;



class ClToken
   {
public:
   ClToken();
   char *name() { return tokenName; }
   char ubergang[100];
   ClToken *child() { return tokenChild; }
   char *inhalt() { return tokenInhalt; }
   void druckeToken(int ebene);
   int getToken(ifstream &datei,ClElement *element,ClElement *wurzel);
   ClattToken att;
private:
   void cleanToken();
   void druckeTokenEbene(int ebene);
   int fillToken(int mode);
   char tokenName[64];
   ClToken *tokenChild;
   ClToken *tokenSibling;
   char *tokenInhalt;
   } ;
enum zustand { istAnfang, istStartTag, isterwarteID, istEndTag } ;


#endif // NTOKEN3_H
