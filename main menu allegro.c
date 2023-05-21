
#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <math.h>
#include <string.h>
#include <time.h>


#define SCREEN_W 1500 //largeur de la fenêtre allegro
#define SCREEN_H 1000  //hauteur de la fenêtre allegro


///--------------------STRUCTURES---------------------------///

typedef struct joueur //structure chargée de stocker les informations d'un joueur
{
    //nom
    char name[20];
    //nombre de tickets
    int tickets;
    //score
    int score;

}t_joueur;

typedef struct minigame //structure chargée de stocker les informations d'une attraction
{
    //nom
    char titre[20];
    //difficulées
    int difficulty;
    //specs

    //tab de cases prises par le bat
    int coordBat[2][2];
    //coord de la case permettant de lancer le minijeu
    int coordPlay[2];

}t_minigame;


typedef struct terrain //structure permettant de stocker les informations de la carte où l'on se déplace pour atteindre les différentes attractions
{
    int occupied; //0=non (traversable) 1=oui (pas traversable)
    int isStartAttraction; //savoir si la case est un emplacement pour démarrer une attraction
}t_terrain;

typedef struct partie //structure principale chargée de stocker toutes les infos de la partie
{
    //tab de 2 joueurs
    t_joueur playerBase[2];
    //tab de 4 minigame
    t_minigame jeux[4];
    //carte du jeu
    t_terrain plateau[20][20];

    int playerPos[2];//position du joueur sur la carte

}t_partie;

typedef struct banqueImage //  Structure utilisée pour stocker les images utilisées pour le projet
{
    BITMAP * fondMenu;
    BITMAP * joueur;
    BITMAP * batMinigame[3];


}t_banqueImage;

typedef struct banquePolice //  Structure utilisée pour stocker les polices utilisées pour le projet
{
    FONT * Roller_72;
    FONT * Roller_it_24;
    FONT * Pixel_11;
    FONT * System_14;

}t_banquePolice;

///--------------------- SOUS-PROGS -----------------------///

//-------- Système -------//
void initAllegro(); //SSPG chargé d'initialiser les fonctions d'allegro
t_banqueImage loadBanqueImage(); //SSPG chargé d'initaliser et de charger les différentes images dans la structure de stockage
t_banquePolice loadBanquePolices();//idem mais pour les polices d'"critures utilisées
t_partie initPartie(); //SSPG chargé d'initialiser les données d'une nouvelles parties

//---------Menu-----------//
void menuPrincipal(t_partie partie, t_banqueImage image, t_banquePolice police); //SSPG chargé de l'affichage du menu principal et d'enregistrer le choix du joueur


//SSPG chargé de l'initialisation des fonctions relatives à allegro
void initAllegro()
{

    srand(time(NULL)); //on initialise la fonction random (utile pour plus tard)
    allegro_init();    //initialisation allegro
    install_keyboard(); //on permet l'utilisation du clavier
    install_mouse();  //on permet l'utilisation de la souris
    set_color_depth(desktop_color_depth());    //initialisation de la palette de couleur


    //on initialise les fonctions de son
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0) != 0) {
        printf("Error initialising sound: %s\n", allegro_error);
        exit(1);
    }

    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,SCREEN_W,SCREEN_H,0,0))!=0)      //paramètres fenêtre
    {
        //problème avec fenêtre allegro
        allegro_message("Pb de mode graphique");
        allegro_exit();                                             //fermeture allegro
        exit(EXIT_FAILURE);                                         //sortie forcée du programme
    }

    show_mouse(screen); //on affiche la souris à l'écran

}


//SSPG chargé d'initialiser les données d'une nouvelle partie
t_partie initPartie()
{
    t_partie partie;


    partie.playerBase[0].score=0;
    partie.playerBase[0].tickets=5;


    partie.playerBase[1].score=0;
    partie.playerBase[1].tickets=5;



    strcpy(partie.jeux[0].titre, "Taupe-la!");
    strcpy(partie.jeux[1].titre, "Tir au ballons");
    strcpy(partie.jeux[2].titre, "Paris hippiques");




    return partie;
}


//SSPG permettant le chargement des images dans la structure
t_banqueImage loadBanqueImage()
{
    t_banqueImage image;

    image.fondMenu= load_bitmap("images/fondMenu.bmp", NULL);


    return image;
}

//SSPG permettant le chargement des polices dans la structure
t_banquePolice loadBanquePolices()
{
    t_banquePolice police;


    police.Pixel_11 = load_font("polices/PPixel_11.pcx", NULL, NULL);
    police.System_14 = load_font("polices/system_14.pcx", NULL, NULL);
    police.Roller_it_24 = load_font("polices/roller_it_24.pcx", NULL, NULL);
    police.Roller_72 = load_font("polices/roller_72.pcx", NULL, NULL);

    return police;
}




/*
    SSPG chargé d'afficher les options du menu principal et d'enregistrer le choix du joueur
*/
void menuPrincipal(t_partie partie, t_banqueImage image, t_banquePolice police)
{
    //déclaration des différentes variables
    int choix=0; //variable pour stocker le choix du joueur, initialisée à 0

    BITMAP * buffer; //on utilise un buffer pour un affichage plus fluide
    buffer=create_bitmap(SCREEN_W,SCREEN_H);


    ///choix du joueur
    while(choix==0)
    {
        clear_bitmap(buffer); //on vide le buffer

        blit(image.fondMenu, buffer, 0,0,0,0,SCREEN_W,SCREEN_H); //on affiche l'image de fond sur le buffer


        ///-------------------------------------AFFICHAGE TITRE DU JEU----------------------------------------------///

        textout_centre_ex(buffer, police.Roller_72, "ECE", (SCREEN_W*5)/6-25+6, SCREEN_H/10+2,  makecol(0,0,0), -1 );
        textout_centre_ex(buffer, police.Roller_72, "World", (SCREEN_W*5)/6+25+6, SCREEN_H/10+75+2,  makecol(0,0,0), -1 );

        textout_centre_ex(buffer, police.Roller_72, "ECE", (SCREEN_W*5)/6-25, SCREEN_H/10, makecol(255,255,255), -1 );
        textout_centre_ex(buffer, police.Roller_72, "World", (SCREEN_W*5)/6+25, SCREEN_H/10+75, makecol(250,218,94), -1 );



        ///-------------------------------------AFFICHAGE CASES CHOIX MENU----------------------------------------------///

        for(int i=0; i<4; i++) //on boucle pour chaque case du tableau
        {
            rectfill(buffer, 50+50*i, 235+100*i, 400+50*i, 300+100*i, makecol(171, 224, 215));//on dessine une case bleu ciel

            //bords noir des cases
            rectfill(buffer, 45+50*i,230+100*i, 405+50*i, 235+100*i,  makecol(0,0,0)); //haut de la case
            rectfill(buffer, 45+50*i,230+100*i, 50+50*i, 305+100*i,  makecol(0,0,0)); //gauche de la case
            rectfill(buffer, 400+50*i, 230+100*i, 405+50*i, 305+100*i,  makecol(0,0,0)); //droit de la case
            rectfill(buffer, 45+50*i, 300+100*i, 405+50*i, 305+100*i,  makecol(0,0,0)); //bas de la case


            if((mouse_x > 50+50*i)&&(mouse_x<400+50*i)&&(mouse_y>235+100*i)&&(mouse_y<300+100*i)) //si le joueur passe son curseur sur une case
            {
                rectfill(buffer, 50+50*i, 235+100*i, 400+50*i, 300+100*i, makecol(34, 101, 140));//on affiche la case ensurbrillance bleu gris

                if(mouse_b==1)//si il clique sur la case
                {
                    choix=i+1; //on set le choix correspondant
                    rectfill(buffer, 50+50*i, 235+100*i, 400+50*i, 300+100*i, makecol(4, 45, 148)); //on réaffiche la case en bleu foncé

                }

            }

        }

        ///----------------------------------AFFICHAGE OPTIONS MENU----------------------------------///


        textout_centre_ex(buffer, police.Roller_it_24, "Nouvelle Partie", 224-1, 234+2,  makecol(0,0,0), -1);
        textout_centre_ex(buffer, police.Roller_it_24, "Charger Partie", 274-1, 334+2,  makecol(0,0,0), -1);
        textout_centre_ex(buffer, police.Roller_it_24, "Classement", 324-1, 434+2,  makecol(0,0,0), -1);
        textout_centre_ex(buffer, police.Roller_it_24, "Quitter", 374-1, 535+2,  makecol(0,0,0), -1);

        //on affiche les textes des différentes options du menu
        textout_centre_ex(buffer, police.Roller_it_24, "Nouvelle Partie", 225, 235, makecol(95, 137, 140), -1);
        textout_centre_ex(buffer, police.Roller_it_24, "Charger Partie", 275, 335, makecol(95, 137, 140), -1);
        textout_centre_ex(buffer, police.Roller_it_24, "Classement", 325, 435, makecol(95, 137, 140), -1);
        textout_centre_ex(buffer, police.Roller_it_24, "Quitter", 375, 535, makecol(95, 137, 140), -1);

        blit(buffer, screen, 0,0,0,0, SCREEN_W,SCREEN_H); //on affiche le contenu du buffer à l'écran



    }


    ///résultats du choix
    switch(choix)
    {
        //nouvelle partie
    case 1:
        nouvellePartie(partie, image, police);
        break;

        //charger partie
    case 2:
        break;

        //classement
    case 3:
        break;

        //quitter le jeu
    case 4:
        exit(0);
        break;

    }
}

int main()
{
    ///DECLARATION DES STRUCTURES PRINCIPALES
    t_banqueImage banqueImage;
    t_banquePolice banquePolice;
    t_partie partie;


    initAllegro(); //on initialise tout le nécessaire en rapport avec allegro

    banqueImage = loadBanqueImage(); //on charge les images utilisées dans la structure
    banquePolice = loadBanquePolices(); //idem avec les polices

    //initialisation de la carte


    while(!key[KEY_ESC]) //boucle de jeu
    {
        //menu principal
        menuPrincipal(partie, banqueImage, banquePolice);
    }


    return 0;
}
END_OF_MAIN()
