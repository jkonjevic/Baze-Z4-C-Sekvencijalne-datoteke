#include <stdio.h>
#include <stdlib.h>
#include "operacije_nad_datotekom.h"
int main()
{

	int running = 1;
	int userInput;

	FILE *fajl = NULL;
    printf("Hello world!\n");

    	while (running) {
		printf("Odaberite opciju:\n");
		printf("1 - Otvaranje datoteke\n");
		printf("2 - Pretraga datoteke\n");
		printf("3 - Unos sloga\n");
		printf("4 - Ispis svi slogova\n");
		printf("5 - Brisanje sloga (logicko)\n");
        printf("6 - KREIRAJ DATOTEKU\n");
        printf("7 - Pronadji maksimalnu kolicinu prenesnih sredstava \n");
        printf("8 - Uvecaj kolicinu sredstava za 10% \n");
        printf("9 - Prosjecna kolicina sredstava sluzbenika \n");
		printf("0 - Izlaz\n");

		if (fajl == NULL) {
			printf("!!! PAZNJA: datoteka jos uvek nije otvorena !!!\n");
		}
		scanf("%d", &userInput);
		getc(stdin);


switch(userInput) {
			case 1:
				{
					char filename[20];
					printf("Unesite ime datoteke za otvaranje: ");
					scanf("%s", &filename[0]);
					fajl = otvoriDatoteku(filename);
                    printf("\n");
					break;
				}
			case 2:
				{
					int sifraTransakcije;
					printf("Unesite sifru transakcije koju trazite: ");
					scanf("%d", &sifraTransakcije);
					SLOG *slog = pronadjiSlog(fajl, sifraTransakcije);
					if (slog == NULL) {
                        printf("Trazeni slog ne postoji!\n");

					} else {
	                    printf("sifraTrans idSluzbenika datum kolicina idKlijenta  brRacunaK\n");

                        ispisiSlog(slog);
                        printf("\n");
					}
                    printf("\n");
					break;
				}
			case 3:
				{
					SLOG dolazak;
					printf("sifraTransakcije(broj): ");
					scanf("%d", &dolazak.sifraTransakcije);
					printf("idSluzbenika (6 karaktera): ");
					scanf("%s", dolazak.idSluzbenika);
					printf("Datum(10 karaktera)");
    				scanf("%s", dolazak.datum);
					printf("kolicina Sredstava(broj): ");
					scanf("%f", &dolazak.kolicinaPrenetihSredstava);
					printf("idKlijenta (6 karaktera): ");
					scanf("%s", dolazak.idKlijenta);
                    printf("brojRacunaKlijenta(broj): ");
					scanf("%d", &dolazak.brojRacunaKlijenta);
					dolazak.deleted = 0;
					dodajSlog(fajl, &dolazak);
                    printf("\n");
					break;
				}
			case 4:
				{

					ispisiSveSlogove(fajl);
                    printf("\n");
					break;
				}

			case 5:
				{
					int sifraTransakcije;
					printf("Unesite sifru Transakcije sloga za logicko brisanje: ");
					scanf("%d", &sifraTransakcije);
					obrisiSlogLogicki(fajl, sifraTransakcije);
                    printf("\n");
					break;
				}
			case 6:
				{
					char filename[20];
					printf("Unesite ime datoteke za kreiranje: ");
					scanf("%s", filename);
					kreirajDatoteku(filename);
                    printf("\n");
					break;
				}
            case 7:
                {
                SLOG *slog = pronadjiMaxSlog(fajl);
                if (slog == NULL) {
                        printf("Trazeni slog ne postoji!\n");

					} else {
	                    printf("\n sifraTrans   idSluzbenika      datum  kolicinaSredstava idKlijenta  brRacunaKlijenta\n");

                        ispisiSlog(slog);
                        printf("\n");
					}
                    printf("\n");
					break;
                }
            case 8:
                {
                    char idKlijenta[7];
                    printf("Unesite idKlijenta ciju kolicinu sredstava zelite da uvecate 10 procenata : \n");
                    scanf("%s",idKlijenta);
                    azurirajSlog(fajl,idKlijenta);
                    break;
                }
            case 9:
                {
                    char idSluzbenika[7];
                    printf("Unesite idSluzbenika ciju prosjecnu kolicinu sredstava zelite ispisete: \n");
                    scanf("%s",idSluzbenika);
                    ispisiProsjekSredstava(fajl,idSluzbenika);
                    break;
                }

			case 0:
				{
					//zatvori datoteku
					if (fajl != NULL) {
						fclose(fajl);
					}
					//i zavrsi program
					running = 0;
				}
		}
	}

	return 0;

}

