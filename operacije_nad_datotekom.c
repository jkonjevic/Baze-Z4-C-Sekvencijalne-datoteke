#include "operacije_nad_datotekom.h"

FILE *otvoriDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "rb+");
	if (fajl == NULL) {
		printf("Doslo je do greske pri otvaranju datoteke \"%s\"! Moguce da datoteka ne postoji.\n", filename);
	} else {
		printf("Datoteka \"%s\" uspesno otvorena!\n", filename);
	}
	return fajl;
}
void ispisiSveSlogove(FILE *fajl) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int rbBloka = 0;
	printf("BL SL sifraTransidSluzbenika datum kolicina idKlijenta brRacuna\n");
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraTransakcije==OZNAKA_KRAJA_DATOTEKE) {
				printf("B%d S%d *******************\n", rbBloka, i);
                return;
			} else if (!blok.slogovi[i].deleted) {
                printf("B%d S%d ", rbBloka, i);
                ispisiSlog(&blok.slogovi[i]);
                printf("\n");
            }
		}

		rbBloka++;
	}
}

void ispisiSlog(SLOG *slog) {
	printf("%7d  %8s  %11s %7f %7s %8d",
        slog->sifraTransakcije,
		slog->idSluzbenika,
		slog->datum,
		slog->kolicinaPrenetihSredstava,
		slog->idKlijenta,
		slog->brojRacunaKlijenta
	);
}


void kreirajDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "wb");
	if (fajl == NULL) {
		printf("Doslo je do greske prilikom kreiranja datoteke \"%s\"!\n", filename);
	} else {
		BLOK blok;
		blok.slogovi[0].sifraTransakcije=OZNAKA_KRAJA_DATOTEKE;
		fwrite(&blok, sizeof(BLOK), 1, fajl);
		printf("Datoteka \"%s\" uspesno kreirana.\n", filename);
		fclose(fajl);
	}
}

SLOG *pronadjiSlog(FILE *fajl, int sifraTransakcije) {
	if (fajl == NULL) {
	        printf("ne otvaram fajl dobro pocetak pronadji slog\n");
		return NULL;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraTransakcije==OZNAKA_KRAJA_DATOTEKE ||
                blok.slogovi[i].sifraTransakcije > sifraTransakcije) {

				return NULL;

			} else if (blok.slogovi[i].sifraTransakcije==sifraTransakcije) {
				//Pronadjen trazeni slog

				if (!blok.slogovi[i].deleted) {
                    SLOG *slog = (SLOG *)malloc(sizeof(SLOG));
                    memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
                    return slog;
				}

			}
		}
	}

	return NULL;
}

void dodajSlog(FILE *fajl, SLOG *slog) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	SLOG slogKojiUpisujemo;
	memcpy(&slogKojiUpisujemo, slog, sizeof(SLOG));

	BLOK blok;
	fseek(fajl, 0, SEEK_SET);
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {

			if (blok.slogovi[i].sifraTransakcije==OZNAKA_KRAJA_DATOTEKE) {

				memcpy(&blok.slogovi[i], &slogKojiUpisujemo, sizeof(SLOG));



				if (i != FBLOKIRANJA-1) {
			        blok.slogovi[i+1].sifraTransakcije=OZNAKA_KRAJA_DATOTEKE;

					//To je to, mozemo vratiti blok u datoteku
					fseek(fajl, -sizeof(BLOK), SEEK_CUR);
					fwrite(&blok, sizeof(BLOK), 1, fajl);

					printf("Novi slog evidentiran u datoteci.\n");
					return;

				} else {
					fseek(fajl, -sizeof(BLOK), SEEK_CUR);
					fwrite(&blok, sizeof(BLOK), 1, fajl);

					BLOK noviBlok;
			        noviBlok.slogovi[0].sifraTransakcije=OZNAKA_KRAJA_DATOTEKE;
					fwrite(&noviBlok, sizeof(BLOK), 1, fajl);

					printf("Novi slog evidentiran u datoteci.\n");
					printf("(dodat novi blok)\n");
					return;

				}

			} else 	if(blok.slogovi[i].sifraTransakcije==slogKojiUpisujemo.sifraTransakcije) {


                if (!blok.slogovi[i].deleted) {
                    printf("Slog sa tom vrednoscu kljuca vec postoji!\n");
                    return;
                } else {

                    memcpy(&blok.slogovi[i], &slogKojiUpisujemo, sizeof(SLOG));


                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
					fwrite(&blok, sizeof(BLOK), 1, fajl);

					printf("Novi slog evidentiran u datoteci.\n");
					printf("(prepisan preko logicki izbrisanog)\n");
					return;
                }

            } else if (blok.slogovi[i].sifraTransakcije > slogKojiUpisujemo.sifraTransakcije) {
				SLOG tmp;
				memcpy(&tmp, &blok.slogovi[i], sizeof(SLOG));
				memcpy(&blok.slogovi[i], &slogKojiUpisujemo, sizeof(SLOG));
				memcpy(&slogKojiUpisujemo, &tmp, sizeof(SLOG));
				if (i == FBLOKIRANJA-1) {
					fseek(fajl, -sizeof(BLOK), SEEK_CUR);
					fwrite(&blok, sizeof(BLOK), 1, fajl);
					fflush(fajl);
				}
			}
		}
	}
}
void obrisiSlogLogicki(FILE *fajl, int sifraTransakcije) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {

            if (blok.slogovi[i].sifraTransakcije==OZNAKA_KRAJA_DATOTEKE ||
                blok.slogovi[i].sifraTransakcije > sifraTransakcije) {

                printf("Slog koji zelite obrisati ne postoji!\n");
                printf("sifraTrans > sifra trans \n");
                return;

            } else if (blok.slogovi[i].sifraTransakcije==sifraTransakcije) {

				if (blok.slogovi[i].deleted == 1) {
					printf("Slog koji zelite obrisati ne postoji!\n");
					printf("U IFU NE DA\n");
					return;
				}
				blok.slogovi[i].deleted = 1;
				fseek(fajl, -sizeof(BLOK), SEEK_CUR);
				fwrite(&blok, sizeof(BLOK), 1, fajl);
                printf("Brisanje sloga zavrseno.\n");
				return;

			}
		}
	}
}

SLOG *pronadjiMaxSlog(FILE *fajl) {
	if (fajl == NULL) {
	        printf("ne otvaram fajl dobro pocetak pronadji slog\n");
		return NULL;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	SLOG *slog = NULL;
	float max = 0;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			 if (blok.slogovi[i].kolicinaPrenetihSredstava>max) {
			    //printf("max1 je %f\n",max);
			    if (!blok.slogovi[i].deleted) {
                    max = blok.slogovi[i].kolicinaPrenetihSredstava;
                  //  printf("maxn je %f\n",max);

                    slog = (SLOG *)malloc(sizeof(SLOG));
                    memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
                  //  printf("prvi ispis sloga\n");
                 //   ispisiSlog(slog);;
				}

			}
                    if(blok.slogovi[i].sifraTransakcije==OZNAKA_KRAJA_DATOTEKE)
                   return slog;

		}
           // return slog;
	}

    return NULL;
}

void azurirajSlog(FILE *fajl, char *idKlijenta){
    if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {

            if (blok.slogovi[i].sifraTransakcije==OZNAKA_KRAJA_DATOTEKE)
                return;
         if (blok.slogovi[i].deleted) {

                    printf("Slog koji zelite modifikovati ne postoji!\n");
                    continue;
                }

             if (strcmp(blok.slogovi[i].idKlijenta, idKlijenta) == 0) {


                blok.slogovi[i].kolicinaPrenetihSredstava=(float)1.1*blok.slogovi[i].kolicinaPrenetihSredstava;

				fseek(fajl, -sizeof(BLOK), SEEK_CUR);
				fwrite(&blok, sizeof(BLOK), 1, fajl);
				fflush(fajl);

				printf("Slog izmenjen.\n");
			}
		}
	}

}

void ispisiProsjekSredstava(FILE *fajl, char *idSluzbenika){
    if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	float suma = 0;
	int brojSluzbenika=0;
	float rez = 0;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
            if (blok.slogovi[i].sifraTransakcije==OZNAKA_KRAJA_DATOTEKE){
            	rez =(float)suma/brojSluzbenika;
	            printf("projescna sredstva sluzbenika su: %f \n",rez);
                return;
            }

             if (strcmp(blok.slogovi[i].idSluzbenika, idSluzbenika) == 0) {
                if (blok.slogovi[i].deleted) {

                    printf("Slog koji zelite modifikovati ne postoji!\n");
                    continue;
                }
                brojSluzbenika++;
                suma += blok.slogovi[i].kolicinaPrenetihSredstava;


			}
		}
	}
	rez =(float)suma/brojSluzbenika;
	printf("projescna sredstva sluzbenika su: %f \n",rez);

}


