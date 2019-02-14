#include <stdio.h>                                                //input output funktionen
#include <string.h>                                                //strcpy funktion
#include "encryption.h"

#define INPUTLEN 128                                            //max nachricht laenge
#define TEMPLEN 256
#define MAX_KEYLEN 16                                            //max schluessel laenge

int* conv_key(const char*);                                        //key in integer array umwandeln anfangend mit 1 und aufsteigend in 1er schritten
char* crypter(const char*, const char*);                        //verschluesselt die gegebene nachricht mit einem gegebenen key
char* decrypter(const char*, const char*);                        //entschluesselt die verschluesselte nachricht mit dem richtigen key

void encryption(){
    char message[INPUTLEN] = { NULL }, key[MAX_KEYLEN], decryptKey[MAX_KEYLEN] = { NULL };    //erstellt char arrays fuer eingegebene message und schluessel
    while (strcmp(key, "quit") != 0){
        gets(message);                                //texteingabe
        gets(key);                                //schluesseleingabe
        strcpy(message, crypter(message, key));
        puts(message);                                            //verschluesselte ausgabe
        puts(decrypter(message, decryptKey));                    //entschluesselte ausgabe
    }
}

int len(const char *a){
    if (a == NULL) { return NULL; }
    int ctr = 0;
    while (a[ctr] != '\0')                                        //z‰hlt die zeichen bis zum ende eines Strings (strlen hat nicht funktioniert)
    {
        ctr++;
    }
    return ctr;
}

int intlen(const int *d) {
    int l = 0;
    while (d[l] != 0)                                            //z‰hlt anzahl von aufeinanderfolgenden nicht 0 integern
    {
        l++;
    }
    return l;
}

int* conv_key(const char* key){                                    //convertiert buchstaben schluessel zu zahlen
    int int_key[MAX_KEYLEN] = { -1 };
    int smallest = 128;                                            //ascii max ist 127 deswegen 128 fuer den spaeteren vergleich
    for (int i = 0; i < len(key); i++){
        int_key[i] = (int)key[i];                                //weist int_key den ascii wert von key zu
        if (int_key[i] < smallest) {                            //kleinster eingegebener ascii wert wird festgehalten
            smallest = int_key[i];
        }
    }
    for (int i = 0; i < len(key); i++){
        int_key[i] -= smallest - 1;                                //alle werte werden gleichmaessig reduziert bis der kleinste wert 1 ist
    }
    
    for (int i = 0; i < len(key); i++) {                        //doppelte werden von links nach rechts angepasst, sodass doppelte eliminiert und durch aufsteigende numerierung ersetzt werden
        for (int j = 0; j < len(key); j++) {
            if (j != i && int_key[i] == int_key[j]) {
                for (int k = 0; k < len(key); k++) {
                    if (int_key[k] >= int_key[i] && i != k) {    //alle zahlen > doppelter zahl muessen auch erhoeht werden
                        int_key[k]++;
                    }
                }
            }
        }
    }
    
    int count = 1;
    do{
        for (int j = 0; j < len(key); j++){
            for (int i = 0; i < len(key); i++){
                if (int_key[i] == count){                        //zaehlt wie viele zahlen des arrays int_key sich im bereich der Laenge des Schluessels befinden
                    count++;                                    //count wird erhoeht, wenn die zahl count in dem array vorkommt
                }
            }
        }
        
        for (int i = 0; i < len(key); i++){
            if (int_key[i] > count){
                int_key[i]--;                                    //alle zahlen die hoeher als count sind werden um 1 verringert
            }
        }
    } while (count != len(key) + 1);                            //wird solange gemacht, bis zahlen von 1 bis laenge des keys mit abstaenden von 1 in dem array gespeichert sind
    
    return int_key;
}

char* crypter(const char* text, const char* key){
    int int_key[MAX_KEYLEN];
    char temp[INPUTLEN][MAX_KEYLEN] = { NULL };                    //2 dimensionales character array mit groesse der nachricht & schluessel laenge
    char out[INPUTLEN] = { NULL };                                //entschluesselte nachricht (selbe laenge wie klartext)
    int column = 0, counter = 0, adjust = 0;
    
    for (int i = 0; i < MAX_KEYLEN; i++){
        int_key[i] = conv_key(key)[i];                            //key wird zu angepasstem integerarray konvertiert
    }
    
    for (int i = 0, j = 0; i < len(text) + 1; i++){                //i = spalten, j = zeilen
        if (text[i] == '\0'){
            temp[j][i - j * len(key)] = '\0';                    //an letzter stelle ein \0 anfuegen
            break;
        }
        if (i != 0 && (i) % len(key) == 0){                     //prüfen ob spalte in die geschrieben werden soll größer als eigentliche anzahl in der matrix
            j++;                                                //zeilensprung im 2 dimensionalen array temp wenn so viele zeichen wie keylen in der zeile stehen
        }
        temp[j][i - j * len(key)] = text[i];                    //schreiben des buchstaben aus text in die richtige spalte an der richtigen position in temp
    }
    while (int_key[counter] != 0){
        for (int i = 0; i < MAX_KEYLEN; i++){                    //Zählt column entsprechend dem integer Key hoch
            if (int_key[i] == 0) {
                break;
            }
            if (int_key[i] == counter + 1){
                column = i;
            }
        }
        for (int i = 0; i < INPUTLEN / MAX_KEYLEN; i++){        //Liest Nachricht aus der Matrix im bezug auf int key aus
            if (temp[i][column] == '\0'){
                if (counter == intlen(int_key)) {               // Fügt am ende einesstrings '\0' an
                    out[adjust] = '\0';
                }
                break;
            }
            else{
                out[adjust] = temp[i][column];                  //Gibt nachricht aus matrix in ein dimensionalem character array aus
            }
            adjust++;
        }
        counter++;
    }
    return out;
}




char* decrypter(const char* text, const char* decryptKey)
{
    int dIntKey[MAX_KEYLEN] = { NULL };                            //integer schluessel des neu eigegebenen schluessels zur entschluesslung
    int rowcount[MAX_KEYLEN] = { NULL };
    char temp[INPUTLEN][MAX_KEYLEN] = { NULL };
    char out[INPUTLEN] = { NULL };
    int outindex = 0, ctr = 0;
    
    //gets(decryptKey);                                //eingabe des schluessels zum entschluesseln
    
    for (int i = 0; i < MAX_KEYLEN; i++){                        //char key in integer key convertieren
        dIntKey[i] = conv_key(decryptKey)[i];
    }
    
    int rows = len(text) / len(decryptKey);                        //anzahl reihen = laenge nachricht / laenge des keys, zaehlt unkomplette reihen nicht
    int lastrow = len(text) % len(decryptKey);
    for (int i = 0; i < len(decryptKey); i++){
        if (i < lastrow){                                       //rowcount = int Array mit int schlüssel in richtiger reihenfolge
            rowcount[dIntKey[i] - 1] = rows + 1;
        }
        else{                                                    
            rowcount[dIntKey[i] - 1] = rows;                    
        }
    }
    
    while (len(decryptKey) > ctr){                                //schreibt nachricht ins 2 dimensionale array, nur so viele spalten wie schluessel laenge
        for (int i = 0; i < rowcount[ctr]; i++){                //geht durch die reihen durch bis zur letzten, dann springt zu 0
            temp[i][ctr] = text[outindex];                        //fügt geheimtext spaltenweise zum array hinzu, nicht bezogen auf int key
            outindex++;
        }
        ctr++;
    }
    
    outindex = 0;
    for (int i = 0; i <= rows; i++){
        for (int j = 0; j < len(decryptKey); j++){                //geht reihenweise durch das array und holt buchstaben bezogen auf wert des integer keys
            out[outindex] = temp[i][dIntKey[j] - 1];            //schreibt diese ins array 'out' an aufeinander folgenden stellen
            if (out[outindex] != '\0') { outindex++; }
        }
    }
    return out;
    
}

