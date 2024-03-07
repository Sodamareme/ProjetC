
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

#define NB_UTILISATEURS 15
#define NB_ETUDIANTS_PAR_CLASSE 5
#define NB_admin 2

// Structure pour représenter un utilisateur
typedef struct {
    char login[20];
    char password[20];
    char classe[20];
} Utilisateur;

typedef struct {
   char admin[20];
   char passwordAdmin[20];
   char groupe[20];
} userAdmin;

userAdmin userAdmins[NB_admin]={
    {"admin1","1616","groupeadmin"},
    {"admin2","1717","groupeadmin1"}
};

// Tableau pour stocker les utilisateurs
Utilisateur utilisateurs[NB_UTILISATEURS] = {
    {"user1", "123", "classeGL"},
    {"user2", "456", "classeGL"},
    {"user3", "789", "classeGL"},
    {"user4", "321", "classeGL"},
    {"user5", "654", "classeGL"},
    {"user6", "987", "classeIAGE"},
    {"user7", "135", "classeIAGE"},
    {"user8", "246", "classeIAGE"},
    {"user9", "357", "classeIAGE"},
    {"user10", "468", "classeIAGE"},
    {"user11", "579", "classeRESEAU"},
    {"user12", "135", "classeRESEAU"},
    {"user13", "246", "classeRESEAU"},
    {"user14", "357", "classeRESEAU"},
    {"user15", "468", "classeRESEAU"}
};
// Structure pour représenter une présence
typedef struct {
    char login[20];
    char classe[20];
    char date[20]; // Date de la présence
    char heure[20]; // Heure de la présence
} Presence;


//FONCTIONS MENUS
void afficherMenuAdmin() {
    printf("\n---------------MENU ADMIN--------------\n");
    printf("1--------GESTION DES ÉTUDIANTS\n");
    printf("2--------GÉNÉRATION DE FICHIERS----\n");
    printf("3--------MARQUER LES PRÉSENCES\n");
    printf("4--------ENVOYER UN MESSAGE\n");
    printf("5--------QUITTER\n");
}

void afficherMenuApprenant() {
    printf("\n---------------MENU APPRENANT--------------\n");
    printf("1------------MARQUER MA PRÉSENCE\n");
    printf("2------------NOMBRE DE MINUTES D'ABSENCE\n");
    printf("3------------MES MESSAGES\n");
    printf("4------------QUITTER\n");
}

// Fonction pour masquer la saisie avec des éto
void masquerSaisie() {
    struct termios old, new_term;
    if (tcgetattr(STDIN_FILENO, &old) != 0) {
        perror("Erreur lors de la récupération des attributs du terminal");
        exit(EXIT_FAILURE);
    }
    new_term = old;
    new_term.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) != 0) {
        perror("Erreur lors de la configuration des attributs du terminal");
        exit(EXIT_FAILURE);
    }
}

// Fonction pour restaurer l'écho de la saisie
void restaurerSaisie() {
    struct termios old;
    tcgetattr(STDIN_FILENO, &old);
    old.c_lflag |= (ECHO | ECHOE | ECHOK | ECHONL | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
}
// pour afficher les étoiles
char getch() {
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}
void effacerTerminal() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
// Afficher la liste des classes
void afficherListeClasses() {
    printf("\nListe des classes :\n");
    printf("1. classeGL\n");
    printf("2. classeIAGE\n");
    printf("3. classeRESEAU\n");
}

// Afficher la liste des étudiants pour une classe donnée
void afficherListeEtudiants(const char *classe) {
    printf("\nListe des étudiants pour la classe %s :\n", classe);
    for (int i = 0; i < NB_UTILISATEURS; i++) {
        if (strcmp(utilisateurs[i].classe, classe) == 0) {
            printf("- %s\n", utilisateurs[i].login);
        }
    }
}

// Fonction pour vérifier le mot de passe admin
int verifierMotDePasseAdmin() {
    char password[20];
    printf("Entrez votre mot de passe admin (Q pour annuler): ");
    fflush(stdout);
    masquerSaisie();
    scanf("%s", password);
    restaurerSaisie();
    printf("\n");

    // Vérification du mot de passe
    for (int i = 0; i < NB_admin; i++) {
        if (strcmp(password, userAdmins[i].passwordAdmin) == 0) {
            return 1; // Mot de passe correct
        }
    }
    return 0; // Mot de passe incorrect
}
void marquerPresence(const char *login, const char *classe) {
    // Obtenir la date et l'heure actuelles
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Créer une structure de présence
    Presence presence;
    strcpy(presence.login, login);
    strcpy(presence.classe, classe);
    sprintf(presence.date, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday); 
    sprintf(presence.heure, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec); 

    // Écrire la présence dans un fichier
    FILE *fichier = fopen("liste_presences.txt", "a"); 
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    fprintf(fichier, "Login: %s, Classe: %s, Date: %s, Heure: %s\n", presence.login, presence.classe, presence.date, presence.heure);
    fclose(fichier);
}

//FONCTION CONNEXION
int fonctionConnexion() {
    char login[20];
    char password[20];
    char c;
    int i;
    int trouve = 0;

    do {     
        printf("Entrer votre login: ");
        scanf("%s", login);
        
        printf("Entrez votre mot de passe: ");
         i = 0; // Initialisation de i ici
        while (1) {
            c = getch();
            if (c == '\n') {
                password[i] = '\0';
                break;
            } else if (c == 127) {
                if (i > 0) {
                    i--;
                    printf("\b \b");
                }
            } else {
                password[i++] = c;
                printf("*");
            }
        }
        printf("\n");

        // Vérification des informations de connexion pour les utilisateurs normaux
        for (i = 0; i < NB_UTILISATEURS; i++) {
            if (strcmp(login, utilisateurs[i].login) == 0 && strcmp(password, utilisateurs[i].password) == 0) {
                trouve = 1;
                if (strcmp(utilisateurs[i].classe, "classeGL") == 0 || strcmp(utilisateurs[i].classe, "classeIAGE") == 0 || strcmp(utilisateurs[i].classe, "classeRESEAU") == 0) {
                    afficherMenuApprenant();
                } 
            }
        }

        // Vérification des informations de connexion pour les utilisateurs admin
        for (i = 0; i < NB_admin; i++) {
            if (strcmp(login, userAdmins[i].admin) == 0 && strcmp(password, userAdmins[i].passwordAdmin) == 0) {
                trouve = 1;
                char login;
                char classe;
                int choix;
                effacerTerminal();
                afficherMenuAdmin();
                printf("Choisissez une option : ");
                scanf("%d", &choix);
                switch (choix) {
                    case 1:
                        // Gestion des étudiants
                        break;
                    case 2:
                        marquerPresence( &login,&classe);
                        break;
                    case 3:
                        // Marquer les présences
                        while (1) {
                            effacerTerminal();
                            
                                while (1) {
                                    printf("Entrez le mot de passe de l'étudiant (Q pour quitter): ");
                                  
                                       i = 0; // Initialisation de i ici
                                        while (1) {
                                            c = getch();
                                            if (c == '\n') {
                                                password[i] = '\0';
                                                break;
                                            } else if (c == 127) {
                                                if (i > 0) {
                                                    i--;
                                                    printf("\b \b");
                                                }
                                            } else {
                                                password[i++] = c;
                                                printf("*");
                                            }
                                        }
                                        printf("\n");
                                    if (strcmp(password, "Q") == 0 || strcmp(password, "q") == 0) { // Correction de la condition de sortie
                                        if (verifierMotDePasseAdmin()) {
                                            return 0; // Sortir de la fonction
                                        } else {
                                            printf("Mot de passe admin incorrect.\n");
                                        }
                                    }
                                    // Vérification du mot de passe de l'étudiant
                                    for (int j = 0; j < NB_UTILISATEURS; j++) {
                                        if ( strcmp(utilisateurs[j].password, password) == 0) {
                                            printf("Présence marquée pour l'étudiant %s.\n", utilisateurs[j].login);
                                            break;
                                        }
                                        if (j == NB_UTILISATEURS - 1) {
                                            printf("Mot de passe incorrect.\n");
                                        }
                                    }
                                }char classeChoisie[20]; // Changer le type de classeChoisie en tableau de caractères
                            scanf("%s", classeChoisie);

                             if (strcmp(classeChoisie, "Q") == 0 || strcmp(classeChoisie, "q") == 0) { // Correction de la condition de sortie
                                if (verifierMotDePasseAdmin()) {
                                    return 0; // Sortir de la fonction
                                } else {
                                    printf("Mot de passe admin incorrect.\n");
                                }
                            } else {
                                printf("Classe invalide.\n");
                            }
                        }
                        break;
                    case 4:
                        // Envoyer un message
                        break;
                    case 5:
                        // Quitter
                        break;
                    default:
                        printf("Option invalide.\n");
                        break;
                }
            }
        }
        
        if (!trouve) {
            printf("Login ou mot de passe incorrect.\n");
        }
    } while (!trouve);

    return 0;
}

//FONCTION DECONNEXION
int deconnexion() {
    char choix;
    printf("Voulez-vous vous déconnecter ? (o/n): ");
    scanf(" %c", &choix);
    if (choix == 'o' || choix == 'O') {
        printf("Déconnexion réussie.\n");
        return 1;
    } else if (choix == 'n' || choix == 'N') {
        printf("Retour au menu principal.\n");
        return 0;
    } else {
        printf("Choix invalide.\n");
        return deconnexion(); // Demander à nouveau
    }
}
