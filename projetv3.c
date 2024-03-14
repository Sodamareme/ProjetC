
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_STUDENTS 100

// Structure pour représenter un étudiant
typedef struct {
    int code;
    char nom[50];
    char classe[50];
    char motDePasse[50];
} Etudiant;

// Structure pour représenter un administrateur
typedef struct {
    char login[50];
    char motDePasse[50];
} Admin;

// Prototypes de fonction
int verifierAdmin(Admin admin, Etudiant etudiants[], int nombreEtudiants, int code);
int aDejaMarqueAujourdhui(int codeEtudiant);
void marquerPresence(Etudiant etudiants[], int *nombreEtudiants);
int verifierMotDePasseAdmin(char motDePasseSaisi[], Admin admin);

// Fonction pour afficher le menu de l'administrateur
void afficherMenuAdmin() {
    printf("\nMenu Administrateur :\n");
    printf("1. GESTION DES ÉTUDIANTS\n");
    printf("2. GÉNÉRATION DE FICHIERS\n");
    printf("3. MARQUER LES PRÉSENCES\n");
    printf("4. ENVOYER UN MESSAGE\n");
    printf("5. QUITTER\n");
}

// Fonction pour afficher le menu de l'étudiant
void afficherMenuEtudiant() {
    printf("\nMenu Étudiant :\n");
    printf("1. MARQUER MA PRÉSENCE\n");
    printf("2. NOMBRE DE MINUTES D'ABSENCE\n");
    printf("3. MES MESSAGES (0)\n");
    printf("4. QUITTER\n");
}
int verifierAdmin(Admin admin, Etudiant etudiants[], int nombreEtudiants, int code) {
    char login[50], motDePasse[50];
    if (code == -1) {
        // Vérification des identifiants de l'administrateur
        printf("Entrez votre login : ");
        scanf("%49s", login);
        printf("Entrez votre mot de passe : ");
        scanf("%49s", motDePasse);

        if (strcmp(admin.login, login) == 0 && strcmp(admin.motDePasse, motDePasse) == 0)
            return 1; // Identifiants corrects
    } else {
        // Vérification du code de l'étudiant
        int i;
        for (i = 0; i < nombreEtudiants; i++) {
            if (etudiants[i].code == code)
                return 2; // Code de l'étudiant correct
        }
    }
    
    return 0; // Identifiants incorrects ou code de l'étudiant introuvable
}
// Fonction pour vérifier le mot de passe de l'administrateur lorsqu'il souhaite quitter
int verifierMotDePasseAdmin(char motDePasseSaisi[], Admin admin) {
    return strcmp(admin.motDePasse, motDePasseSaisi) == 0;
}
int aDejaMarqueAujourdhui(int codeEtudiant) {
    FILE *fichier = fopen("liste_presences.txt", "r");
    if (fichier != NULL) {
        char ligne[100];
        char code[10], date[20];

        // Récupérer la date d'aujourd'hui
        time_t maintenant = time(NULL);
        struct tm *dateActuelle = localtime(&maintenant);
        strftime(date, sizeof(date), "%Y-%m-%d", dateActuelle);

        // Lire chaque ligne du fichier
        while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
            sscanf(ligne, "Code: %s %*s", code);

            // Vérifier si l'entrée correspond à l'étudiant et à la date d'aujourd'hui
            if (atoi(code) == codeEtudiant) {
                fclose(fichier);
                return 1; // L'étudiant a déjà marqué sa présence aujourd'hui
            }
        }

        fclose(fichier);
    }
    
    return 0; // L'étudiant n'a pas encore marqué sa présence aujourd'hui
}

void marquerPresence(Etudiant etudiants[], int *nombreEtudiants) {
    char motDePasse[50];
    printf("Entrez votre mot de passe étudiant (Q pour quitter) : ");
    scanf("%s", motDePasse);

    if (strcmp(motDePasse, "Q") == 0 || strcmp(motDePasse, "q") == 0) {
        printf("Vous avez choisi de quitter le processus de marquage de présence.\n");
        return;
    }

    // Vérifier si le mot de passe est valide et trouver l'étudiant correspondant
    int i;
    for (i = 0; i < *nombreEtudiants; i++) {
        if (strcmp(etudiants[i].motDePasse, motDePasse) == 0) {
            printf("Vous avez marqué votre présence.\n");

            // Vérifier si l'étudiant a déjà marqué sa présence aujourd'hui
            if (aDejaMarqueAujourdhui(etudiants[i].code)) {
                printf("Vous avez déjà marqué votre présence aujourd'hui.\n");
                return;
            }

            // Enregistrer la présence dans le fichier
            FILE *fichier = fopen("liste_presences.txt", "a");
            if (fichier != NULL) {
                fprintf(fichier, "Code: %d, Nom: %s, Classe: %s\n", etudiants[i].code, etudiants[i].nom, etudiants[i].classe);
                fclose(fichier);
            } else {
                printf("Erreur lors de l'ouverture du fichier.\n");
            }

            return;
        }
    }

    printf("Mot de passe incorrect ou étudiant introuvable.\n");
}

int main() {
    // Initialisation de l'administrateur
    Admin admin = {"admin", "password"};

    // Création de la liste d'étudiants
    Etudiant etudiants[MAX_STUDENTS] = {
        {101, "Jean Dupont", "Première", "mdp1"},
        {102, "Marie Martin", "Terminale", "mdp2"},
        // Ajouter d'autres étudiants ici...
    };
    int nombreEtudiants = 2; // Mettre à jour le nombre d'étudiants ajoutés

    int choix = 0;
    unsigned int code = -1; // Initialisation de la variable code

    // Appel du menu de l'administrateur
    int verif;
    char motDePasseAdmin[50];
    do {
        if ((verif = verifierAdmin(admin, etudiants, nombreEtudiants, code)) == 1) {
            afficherMenuAdmin();
            printf("Choisissez une option : ");
            scanf("%d", &choix);
            switch (choix) {
                case 1:
                    // GESTION DES ÉTUDIANTS
                    // (à implémenter)
                    break;
                case 2:
                    // GÉNÉRATION DE FICHIERS
                    // (à implémenter)
                    break;
                case 3:
                    // MARQUER LES PRÉSENCES
                    marquerPresence(etudiants, &nombreEtudiants);
                    break;
                case 4:
                    // ENVOYER UN MESSAGE
                    // (à implémenter)
                    break;
                case 5:
                    // QUITTER
                    printf("Voulez-vous vraiment quitter ? (o/n) : ");
                    char reponse;
                    scanf(" %c", &reponse);
                    if (reponse == 'o' || reponse == 'O') {
                        printf("Entrez votre mot de passe pour confirmer : ");
                        scanf("%49s", motDePasseAdmin);
                        if (verifierMotDePasseAdmin(motDePasseAdmin, admin)) {
                            printf("Déconnexion de l'administrateur.\n");
                            choix = 5;
                        } else {
                            printf("Mot de passe incorrect. Retour au menu principal.\n");
                        }
                    }
                    break;
                default:
                    printf("Option invalide. Veuillez réessayer.\n");
            }
        } else if (verif == 2) {
            printf("Entrez un autre code d'étudiant : ");
            scanf("%u", &code); // Utilisation de %u pour la saisie d'un entier non signé
        } else {
            printf("Identifiants incorrects. Veuillez réessayer.\n");
        }
    } while (choix != 5);

    return 0;
}
