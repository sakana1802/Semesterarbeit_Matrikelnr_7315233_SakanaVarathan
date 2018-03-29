#include <iostream>
using namespace std;
#include <string.h>

#include "nelement3.h"

ClElement::ClElement(
char                *zeichenkette)
{
zahl=0;
zahlAttribute=0;
strcpy(name,zeichenkette);
status=frei;
naechstes=NULL;
}
void ClElement::addTag(
char                  *zeichenkette)
{
strcpy(tags[zahl],zeichenkette);
zahl++;
}
void ClElement::addAttribute(
char                        *zeichenkette)
{
strcpy(attributes[zahlAttribute],zeichenkette);
zahlAttribute++;
}
void ClElement::druckeElement(
int                           ebene,
ClElement                          *wurzel)
{
int j,i;
ClElement *abhaengig;

//---------------------------------------------------------------------------------------------------------------------------------------

status=inBearbeitung;
for (j=0;j<ebene;j++) cout << "|   ";
cout << "Element " << name << " erlaubt die Attribute: " << endl;
for (i=0;i<zahlAttribute;i++)
    {
    for (j=0;j<ebene;j++) cout << "|   ";
    cout << "  -" << attributes[i] << endl;
    }
for (j=0;j<ebene;j++) cout << "|   ";
cout << "und enthaelt die Tags: " << endl;
for (i=0;i<zahl;i++)
    {
    for (j=0;j<ebene;j++) cout << "|   ";
    cout << "   " << tags[i] << endl;
    abhaengig=sucheElement(tags[i],wurzel);
    if (abhaengig != NULL) abhaengig->druckeElement(ebene+1,wurzel);
    }
status=frei;
}

ClElement *ClElement::sucheElement(
char                              *suche,
ClElement                               *element)
{
ClElement *resultat;

if (element==NULL) return NULL;
if (!strcmp(suche,element->name))
   if (element->status==frei) return element;
   else return NULL;
return sucheElement(suche,element->naechstes);
}

//------------------------------------------------------------------------------------------------------------------------

int ClElement::elementIstErlaubt(
char                            *name)
{
for (int i=0;i<zahl;i++)
    if (!strcmp(name,tags[i])) return i;

return -1;
}

int ClElement::attributeIstErlaubt(
char                            *name)
{
for (int i=0;i<zahlAttribute;i++)
    if (!strcmp(name,attributes[i])) return i;

return -1;
}

