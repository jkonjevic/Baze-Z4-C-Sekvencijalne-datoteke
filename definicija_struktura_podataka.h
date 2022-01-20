#ifndef DEFINICIJA_STRUKTURA_PODATAKA_H_INCLUDED
#define DEFINICIJA_STRUKTURA_PODATAKA_H_INCLUDED

#define FBLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE -1


typedef struct Slog {
	int sifraTransakcije;
	char idSluzbenika[7];
	char datum[11];
	float kolicinaPrenetihSredstava;
	char idKlijenta[7];
	int brojRacunaKlijenta;
	int deleted;
} SLOG;

typedef struct Blok {
	SLOG slogovi[FBLOKIRANJA];
} BLOK;


#endif // DEFINICIJA_STRUKTURA_PODATAKA_H_INCLUDED
