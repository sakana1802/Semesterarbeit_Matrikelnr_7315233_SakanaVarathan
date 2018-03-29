#include <iostream>
using namespace std;
#include <fstream>
#include <string.h>
#include "ntoken3.h"


ClElement *verarbeite(ifstream& datei);
enum ezustand {direktive, element, attribute, abhaengigElement,abhaengigAttribute, inID, noise};

int main()
{
char dateiname[100];
char wahl;
char zeichen;
char puffer[100];
ifstream ein;
ClToken *token;
ClElement *jetzt=NULL, *wurzel;
int zaehler;

//------------------------------------------------Menüführung-----------------------------------------------------------------------
cout <<"Menue"<< endl<<endl;
cout << "Geben Sie (c) fuer die 'convert data' Funktion ein "<<endl<<"Geben Sie (h) fuer die 'help' Funktion ein" <<endl<<endl;
cin >> wahl;


switch(wahl)
   {
case 'c':
   cout <<"Sie haben die Konvertierfunktion ausgewaehlt! Geben Sie erneut 'c' ein um fortzufahren! "<<endl;
   break;

case 'h':
   cout << "Mit der Konvertieroption koennen Sie die XML Datei und die TXT in eine neue Instanz zusammenfuehren" <<endl<<endl<<
           "Sie werden zur Konvertierungsaufforderung zurueckgeleitet. Geben Sie erneut 'c' ein um fortzufahren! "<<endl<<endl;
   break;

default:
   cout <<"Fehler!! Starten Sie das Programm erneut und waehlen Sie aus den oben genannten Punkte aus! " <<endl<<endl;
   return wahl;
   break;
   }

cout << "convert data Funktion"<<endl<<endl;
cin >> wahl;

switch(wahl)
   {
case 'c':
   cout <<"Sie haben die Konvertierfunktion ausgewaehlt! "<<endl;
   break;
default:
   cout <<"Fehler!! Starten Sie das Programm erneut und waehlen Sie aus den oben genannten Punkte aus! " <<endl<<endl;
   return wahl;
   break;
   }


// -----------------------------------------------------DTD Einlesen-----------------------------------------------------------------------------------


cout << "DTD-Dateiname: " << endl;
cin >> dateiname;
ein.open(dateiname);

wurzel=verarbeite(ein);
for (jetzt=wurzel;jetzt!=NULL;jetzt=jetzt->getNext())
    jetzt->druckeElement(0,wurzel);
ein.close();

cout << "DTD wird angezeigt." << endl;

// -----------------------------------------------------XML Einlesen-----------------------------------------------------------------------------------

cout << "XML-Dateiname:" << endl;
cin >> dateiname;

ein.open(dateiname);
ein.open("Semesterarbeit.xml");

cout << "XML wird angezeigt." << endl;


// -----------------------------------------------------------------------------------------------------------------------------------------------------


token=new ClToken;

if (token->getToken(ein,NULL,wurzel)!=0) token->druckeToken(1);
ein.close();

// -----------------------------------------------------TXT Einlesen und KOnvertieren-----------------------------------------------------------------------------------

cout << "TXT-Dateiname:" << endl;
cin >> dateiname;
ein.open(dateiname);
ifstream dataname("Semesterarbeit.xml", ios::binary);
ifstream texti ("Regie.txt");
ofstream kopie("Semesterarbeitcopy.xml", ios::binary);

kopie << dataname.rdbuf();
kopie.close();

ofstream txtoutput("Semesterarbeitcopy.xml", ios::app);
txtoutput << "\n\n\n"<<texti.rdbuf();

for (zaehler=0;;)
  {
ein.get(zeichen);
    if (ein.eof()) break;

    switch(zeichen)
       {
    case '<':
         puffer[zaehler]='\0';
         cout << " Name:  " << puffer << endl;
         zaehler=0;
         break;
    case '/':
         puffer[zaehler]='\0';
         zaehler=0;
         break;
    case '>':
         puffer[zaehler]='\0';
         cout << " " << puffer << endl;
         zaehler=0;
         break;
    case '\n':
          zaehler=0;
          break;
    default:
          puffer[zaehler]=zeichen;
          zaehler++;
          break;
        }
    }
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------------

ClElement *verarbeite(ifstream& datei)
{
char zeichen, letztes;
char puffer[100];
int zaehler;
enum ezustand zustand = noise;
ClElement *jetzt=NULL, *wurzel=NULL, *neu;

for (datei.get(zeichen);!datei.eof();datei.get(zeichen))
    {
    switch(zeichen)
       {
    case '<':
       zustand=direktive;
       zaehler=0;
       break;
    case '>':
       if (letztes!=' ')
          {
          puffer[zaehler]='\0';
          switch(zustand)
             {
          case abhaengigElement:
         jetzt->addTag(puffer);
             break;
          case abhaengigAttribute:
             jetzt->addAttribute(puffer);
             break;
          case element:
             neu=new ClElement(puffer);
         if (jetzt!=NULL) jetzt->setNext(neu);
         else wurzel=neu;
         jetzt=neu;
         zustand=abhaengigElement;
             break;
         }
          }
       zustand=noise;
       break;
    case ' ':
       if (letztes==' ') continue;
       puffer[zaehler]='\0';
       zaehler=0;
       switch(zustand)
          {
       case direktive:
          if (!strcmp(puffer,"!ELEMENT")) zustand=element;
          else if (!strcmp(puffer,"!ATTLIST")) zustand=attribute;
          else
         {
         cout << endl << "Diese Direktive verstehe ich nicht: " << puffer;
         zustand=noise;
         }
          break;
       case element:
          neu=new ClElement(puffer);
      if (jetzt!=NULL) jetzt->setNext(neu);
      else wurzel=neu;
      jetzt=neu;
      zustand=abhaengigElement;
          break;
       case attribute:
          if (wurzel==NULL) neu=NULL;
          else neu=wurzel->sucheElement(puffer,wurzel);
          if (neu==NULL)
             {
             cout << endl << "Es gibt kein Element mit dem Namen: " << puffer << endl;
             zustand=noise;
             }
          else
             {
             zustand=abhaengigAttribute;
             jetzt=neu;
             }
          break;
       case abhaengigElement:
          jetzt->addTag(puffer);
          break;
       case abhaengigAttribute:
          jetzt->addAttribute(puffer);
          break;
      }
       break;
    default:
       if (zustand!=noise) puffer[zaehler] = zeichen;
       zaehler++;
       break;
       }
    letztes=zeichen;
    }

return wurzel;
}

