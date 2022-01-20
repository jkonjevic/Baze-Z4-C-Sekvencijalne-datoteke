#ifndef OPERACIJE_NAD_DATOTEKOM_H_INCLUDED
#define OPERACIJE_NAD_DATOTEKOM_H_INCLUDED
#include "definicija_struktura_podataka.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

FILE *otvoriDatoteku(char *filename);
SLOG *pronadjiSlog(FILE *fajl, int sifraTransakcije);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void obrisiSlogLogicki(FILE *fajl, int sifraTransakcije);
void ispisiSlog(SLOG *slog);
void kreirajDatoteku(char *filename);
SLOG *pronadjiMaxSlog(FILE *file);
void azurirajSlog(FILE *fajl, char *idKlijenta);
void ispisiProsjekSredstava(FILE *fajl, char *idSluzbenika);


#endif // OPERACIJE_NAD_DATOTEKOM_H_INCLUDED
