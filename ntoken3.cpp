#include <iostream>
using namespace std;
#include <fstream>
#include <string.h>
#include "ntoken3.h"


ClToken::ClToken()
{
*tokenName='\0';
tokenChild=NULL;
tokenSibling=NULL;
tokenInhalt=new char[1];
*tokenInhalt='\0';
}
int  ClToken::getToken(
ifstream              &datei,
ClElement                   *element,
ClElement                           *wurzel)
{
int zaehler;
enum zustand zustand;
char zeichen;
char *ziel, *quelle;
char puffer[100];
ClToken *child;
int isChild;
cleanToken();

//--------------------------------------------------------------------------------------------------------------------------------------------

for (zaehler=0;;)
    {
    datei.get(zeichen);
    if (datei.eof())
       {
       if (*tokenName == '\0' && tokenChild == NULL && tokenInhalt == NULL)
          return fillToken(0);
       return fillToken(1);
       }
    switch(zeichen)
       {
    case '<':
       datei.get(zeichen);
       if (zeichen=='/')
          {
          zustand = istEndTag;
          if (zaehler!=0)
             {
             puffer[zaehler]='\0';
             tokenInhalt = new char[zaehler+1];
             strcpy(tokenInhalt,puffer);
             }
          }
       else
          {
          datei.putback(zeichen);
          if (*tokenName!='\0')
             {
             datei.putback('<');
         if (tokenChild==NULL)
            {
                tokenChild=new ClToken;
                if (tokenChild->getToken(datei,element,wurzel)==0)
                   return fillToken(0);
        }
         else
            {
        for (child=tokenChild;;child=child->tokenSibling)
            {
            if (child->tokenSibling==NULL)
               {
               child->tokenSibling=new ClToken;
                       if (child->tokenSibling->getToken(datei,element,wurzel)==0)
                  return fillToken(0);
               break;
               }
            }
        }
             }
          else zustand=istStartTag;
          }
       zaehler=0;
       break;
    case '=':

            if (zustand==istStartTag)
            {
                puffer[zaehler]='\0';
                zustand=isterwarteID;

            }

            zaehler=0;
            break;
    case ' ':
        if(zustand==isterwarteID){
         puffer[zaehler]='\0';

         zustand=istStartTag;
         zaehler=0;
    }
       break;
    case '>':
       puffer[zaehler]='\0';
       if (zustand==istEndTag)
          {
      if (strcmp(tokenName,puffer))
         {
         cout << "Fehlerhaftes End Tag" << endl;
         cout << "Erhalten: '" << puffer << "'; erwartet: '"
              << tokenName << "'" << endl;
             return fillToken(0);
         }
      return fillToken(1);
      }
       if (zustand==istStartTag)
          {
          for (ziel=tokenName,quelle=puffer;*quelle!=' ' && *quelle!= '\0';
               ziel++,quelle++) *ziel=*quelle;
          *ziel='\0';
          if (!strcmp(tokenName,wurzel->getName())) element=wurzel;
          else
             {
         if (element==NULL) isChild=-1;
         else isChild=element->elementIstErlaubt(tokenName);
         if (isChild = 0)
            {
            cout << "Fehlerhaftes Start Tag: '" << tokenName << "'" << endl;
                return fillToken(0);
                }
         element=element->sucheElement(tokenName,wurzel);
             }
          att.getAttList(puffer,element);
          }

       zaehler=0;
       break;
    case '\n':
       break;
    default:
       puffer[zaehler]=zeichen;
       zaehler++;
       break;
       }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------

int ClToken::fillToken(
int                    mode)
{
if (*tokenName=='\0')
   strcpy(tokenName,"Unbekanntes Element");
if (tokenInhalt==NULL)
   {
   tokenInhalt=new char[1];
   *tokenInhalt='\0';
   }

return mode;
}

void ClToken::cleanToken(void)
{
*tokenName='\0';
if (tokenChild!=NULL)
   {
   delete tokenChild;
   tokenChild=NULL;
   }
if (tokenInhalt!=NULL)
   {
   delete tokenInhalt;
   tokenInhalt=NULL;
   }
}

void ClToken::druckeToken(
int                       ebene)
{
ClToken *child;

druckeTokenEbene(ebene);
cout << "Token: " << name() << " - " << inhalt() << endl;
if (att.zahlAtt() > 0)
   {
   for (int i=0;i<att.zahlAtt();i++)
       {
       druckeTokenEbene(ebene);
       cout << "Attribut " << att.zeigeAttName(i) << " hat den Wert "
            << att.zeigeAttWert(i) << endl;
       }
   }
if (tokenChild!=NULL) tokenChild->druckeToken(ebene+1);
if (tokenSibling!=NULL) tokenSibling->druckeToken(ebene);
}

void ClToken::druckeTokenEbene(
int                            ebene)
{
while (ebene > 0)
      {
      cout << "| ";
      ebene = ebene - 1;
      }
}
