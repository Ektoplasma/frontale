#include "groupe.h"

char * recup_groupe()
{
	FILE *fichier_groupe ;
	char *group = malloc (sizeof (char*) *2048);
	char *ligne = NULL;
	ssize_t lecture;
	ssize_t longueur;

	fichier_groupe = fopen ("groupe.ini", "r");

	if(fichier_groupe == NULL) puts("Erreur ouverture fichier");

	int group_compt = 0;

	while ( ( lecture = getline (&ligne, &longueur, fichier_groupe) ) != -1 )
	{
		int i;

		for(i = 0; ligne[i] != '\n' && ligne[i] != EOF; i++)
		{
			group[group_compt] = ligne[i];
			group_compt++;
		}
		group[group_compt++] = ';';
	}

	close(fichier_groupe);

	return group;
}

void create_group(char * group_name)
{
	FILE *fichier_groupe;

	fichier_groupe = fopen("groupe.ini", "w");

	fprintf(fichier_groupe, "%s\n", group_name);

	close(fichier_groupe);
}


int groupe_correspondance(char *demande)
{
	char *mes_groupes = recup_groupe();
	int compt = 0;
	int nb_groupe = 1;
	char * save_ptr1;
	char * save_ptr2;
	char *tmp = malloc (sizeof (char) * 1024);
	char *cmp = malloc (sizeof (char) * 1024);
	char *tmp2 = malloc (sizeof (char) * 1024);
	char *cmp2 = malloc (sizeof (char) * 1024);

	int compteur_mes_groupes, compteur_demande;

	compteur_mes_groupes = compteur_groupe(mes_groupes);
	compteur_demande = compteur_groupe(demande);

	int i, j;

	strcpy(tmp, demande);
	strcpy(cmp, mes_groupes);

	for (i=0;i<compteur_demande;i++)
	{
		if(i==0)
			tmp2 = strtok(tmp, ";");
		else
			tmp2 = strtok(NULL, ";");

		for (j = 0; j < compteur_demande; j++)
		{
			if (j == 0)
				cmp2 = strtok(cmp, ";");
			else
				cmp2 = strtok(NULL, ";");
			
			if (strcmp(tmp2, cmp2) == 0)
			{
				return 1;
			}
		}
		strcpy(cmp, mes_groupes);
	}

	return 0;
}

int compteur_groupe(char *chaine_groupe)
{
	int compt1 = 1;
	int i = 0;

	while(chaine_groupe[i] != '\0')
	{
		if(chaine_groupe[i] == ';') compt1++;
	}

	return compt1;
}
