#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <math.h>
#include <string.h>
#include <time.h>


#define SCREEN_W 1500 //largeur fenêtre allegro
#define SCREEN_H 1000  //hauteur fenêtre allegro



typedef struct banqueImage  //Structure pour stocker les images 
{
    BITMAP * fondMenu;       //fond du menu principal
    BITMAP * joueur;         //sprite du joueur
    BITMAP * batMinigame[3]; //batiments représentant les mini jeux
    BITMAP * finishLine;     //ligne d'arrivée de la course hippique
    BITMAP * taupe;          //taupe


}t_banqueImage;

typedef struct banquePolice   //Structure pour stocker les polices
{
    FONT * Roller_72;
    FONT * Roller_it_24;
    FONT * Pixel_11;
    FONT * System_14;
    FONT * Hudson_26;

}t_banquePolice;

typedef struct joueur  //structure chargée de stocker les informations d'un joueur
{
    char* name;       //nom du joueur
    int taillenom;    //taille du nom
    int tickets;      //nombre de tickets
    int score;        //score
    int color;        //couleur

} t_joueur;


typedef struct minigame //structure chargée de stocker les informations d'une attraction
{
    char titre[20];   //nom
    int difficulty;   //difficulées
    //specs

    //tab de cases prises par le bat
    int coordBat[2][2];
    //coord de la case permettant de lancer le minijeu
    int coordPlay[2];

} t_minigame;


typedef struct terrain  //structure pour stocker les informations de la carte où l'on se déplace pour atteindre les différentes attractions
{
    int occupied;          //0=non (traversable) 1=oui (pas traversable)
    int isStartAttraction; //savoir si la case est un emplacement pour démarrer une attraction
    int isPlayerPos;       //savoir si le joueur se trouve sur cette case
} t_terrain;



typedef struct partie //structure principale chargée de stocker toutes les infos de la partie
{                              
    t_joueur playerBase[2]; //tab de 2 joueurs
    t_minigame jeux[4]; //tab de 4 minigames
    t_terrain plateau[15][15]; //carte du jeu
    
} t_partie;



void initAllegro(); //SSPG chargé d'initialiser les fonctions d'allegro
int isClickInCoord(int x1, int x2, int y1, int y2);
void barreChargement();
t_banquePolice loadBanquePolices();
t_banqueImage loadBanqueImage();
t_partie tirAuBallon(t_partie partie, t_banqueImage image, t_banquePolice police );



int main()
{

    t_partie partie;
    t_banqueImage image;
    t_banquePolice police;

    partie.playerBase[0].name =malloc(sizeof(strlen("Jean")));
    strcpy(partie.playerBase[0].name, "Jean");

    partie.playerBase[1].name = malloc(sizeof(strlen("Valjean")));
    strcpy(partie.playerBase[1].name, "Valjean");


    initAllegro(); //on initialise tout le nécessaire en rapport avec allegro

    police = loadBanquePolices();
    image = loadBanqueImage();
   // barreChargement();

  // partie = tirAuBallon(partie, image, police);

 BITMAP * buffer = create_bitmap(SCREEN_W,SCREEN_H); //déclaration du buffer


    //10 ballon, rouge, radius de 55px
    int tabCoordBallon[10][2]; //tableaux d'entier en 2x10, pour une paire de coord pour chaque ballons
    int tabVectBallon[10][2]; //tableaux d'entier en 2x10, pour un vecteur de direction par ballon
    int flip=1;



    while(!key[KEY_ESC])
    {
        clear_bitmap(buffer);


        ///AFFICHAGE INTERFACE
        rectfill(buffer, 0,0, SCREEN_W, SCREEN_H, makecol(203,32,38)); //affichage des bords
        rectfill(buffer, 20,20, SCREEN_W-20, SCREEN_H-20, makecol(114,95,79)); //affichage du fond

        rectfill(buffer, 0 ,SCREEN_H-200, SCREEN_W, SCREEN_H, makecol(0,0,0));
        rectfill(buffer, 0+10,SCREEN_H-200+10, SCREEN_W-10, SCREEN_H-10, makecol(136,140,141)); //affichage bordereau d'information



        ///CALCUL TRAJECTOIRE BALLON

        ///AFFICHAGE BALLON
        if(flip)circlefill(buffer, SCREEN_W/2, SCREEN_H/2, 55, makecol(255,0,0));


        //calcul si le click est dans le rayon du ballon
        if(mouse_b&1 && (55>= sqrt(pow(SCREEN_W/2-mouse_x,2)+pow(SCREEN_H/2-mouse_y, 2))) ) flip=0;





        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }




    return 0;
}
END_OF_MAIN()


t_partie tirAuBallon(t_partie partie, t_banqueImage image, t_banquePolice police)
{

    BITMAP * buffer = create_bitmap(SCREEN_W,SCREEN_H); //déclaration du buffer


    //10 ballon, rouge, radius de 55px
    int tabCoordBallon[10][2]; //tableaux d'entier en 2x10, pour une paire de coord pour chaque ballons
    int tabVectBallon[10][2]; //tableaux d'entier en 2x10, pour un vecteur de direction par ballon
    int flip=1;

    //coord de l'espace des ballons: 20/20 -> screen_W-20, screen_H-200

    ///CREATION DES BALLONS


    //quand un ballon est cliqué, on met ses coords à 0/0
    //pour détecter le score/la fin de partie, on check combien/si une des coords dans le tab est pas 0

    //pour générer les coords on prends 2 coords rand dans l'esapce praticable tant qu'on est pas en colision avec les autres ballons


    ///GESTION DES COLISIONS

    //on check pour tout les ballons dont les coords ne sont pas 0/0 (ballons déjà éclatés)
    //on check si leurs coords sont pas à 55 px d'un bord ou 110px d'un autre ballon (centre)
        //si oui, on re-random leur vect dir dans des directions opposées
    //on déplace les ballons selons leurs coords


    ///DETECTION D'UN CLIC ET DISPARITION DU BALLON
    //si un clic est fait et qu'on détecte du rouge (pas forcément utile?)
        //on check dans toutes les coords des ballons encore affichés si on est dans le rayon
            //si oui, on set les coords du ballon à 0/0
    //un ballon d'est affiché que si ces coords sont diff de 0/0




    while(!key[KEY_ESC])
    {
        clear_bitmap(buffer);


        ///AFFICHAGE INTERFACE
        rectfill(buffer, 0,0, SCREEN_W, SCREEN_H, makecol(203,32,38)); //affichage des bords
        rectfill(buffer, 20,20, SCREEN_W-20, SCREEN_H-20, makecol(114,95,79)); //affichage du fond

        rectfill(buffer, 0 ,SCREEN_H-200, SCREEN_W, SCREEN_H, makecol(0,0,0));
        rectfill(buffer, 0+10,SCREEN_H-200+10, SCREEN_W-10, SCREEN_H-10, makecol(136,140,141)); //affichage bordereau d'information



        ///CALCUL TRAJECTOIRE BALLON

        ///AFFICHAGE BALLON
        if(flip)circlefill(buffer, SCREEN_W/2, SCREEN_H/2, 55, makecol(255,0,0));


        //calcul si le click est dans le rayon du ballon
        if(mouse_b&1 && (55>= sqrt(pow(SCREEN_W/2-mouse_x,2)+pow(SCREEN_H/2-mouse_y, 2))) ) flip=0;





        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }



    //on affiche le tour du joueur en question

    //décompte puis début du timer



    //10 ballons apparaissent random sur l'écran ( on les faits s'écarter au plus possible au spawn)
    //ballons = drawcircle de couleur rand sur fond marron
    //si click, on check si la souris est à la distance du rayon d'un centre de ballon (stockés dans un tableau)
    //si oui, on depop le ballon en question

    //on check si on a toujours un ballon d'affiché
    //si oui, on cut le timer et on le sauvegarde

    //si c'est la fin du tour du 1er, on recommence pour le 2e

    //si c'est la fin du 2e, on compare les timers

    //le joueur ayant le timer le plus bas est affiché comme vainqueur et recoit 1 ticket



    //toute les 10 secondes, le ballon change de direction
    //xdir et ydir rand (-3,3) pour la frame d'affichage

    return partie;
}


void loadStartGame(t_banquePolice police, t_partie partie)
{
    //barre de chargement cutsom en fonction du joueur qui va jouer et du minijeu
    //reprendre le concept de la barre de chargement

    //afficher à %name% de jouer! avec l'ui du jeu en question (screenshot chargé en fond)

    //faire un switch case pour l'image à charger: 1=tir au ballon 2=taupe là 3= paris hippiques


}



void barreChargement()
{
    BITMAP * buffer2x;
    buffer2x=create_bitmap(SCREEN_W,SCREEN_H);//creation du bitmap buffer double

    //Kabel_28= load_font("polices/kabelMono_28.pcx", NULL, NULL);//on load la police Kabel en 28

   // BITMAP *MenuDecor;//création du fond d'écran
   // MenuDecor=load_bitmap("images/FondMono.bmp",NULL); //chargement de l'image de fond
    //blit(MenuDecor,buffer2x,0,0,0,0,SCREEN_W,SCREEN_H);//affichage du decor du menu sur le buffer

    rectfill(buffer2x,0,0,SCREEN_W, SCREEN_H, makecol(255,255,255));

    int gauge=0;

    while(gauge<800)
    {
        clear_bitmap(buffer2x);

        rectfill(buffer2x,0,0,SCREEN_W, SCREEN_H, makecol(255,255,255));
        //blit(MenuDecor,buffer2x,0,0,0,0,SCREEN_W,SCREEN_H);//affichage du decor du menu sur le buffer

        gauge = gauge + (10 + (rand()%10 -5));

        rect(buffer2x, 199, 299, 1001,401, makecol(255,255,255)); //fond de la barre
        rectfill(buffer2x, 200, 300, 200+gauge, 400, makecol(0,255,0));//barre de chargement

        textout_centre_ex(buffer2x, font, "Chargement", SCREEN_W/2, 315, makecol(255,0,0), -1);//on affiche

        blit(buffer2x,screen,0,0,0,0,SCREEN_W,SCREEN_H);//affichage du double buffer sur l'écran

        sleep((rand()%50+51)/100);
    }






    clear_bitmap(buffer2x);//clear du buffer en debut de boucle



}



//SSPG permettant le chargement des images dans la structure
t_banqueImage loadBanqueImage()
{
    t_banqueImage image;

    image.fondMenu= load_bitmap("images/fondMenu.bmp", NULL);
    image.finishLine= load_bitmap("images/finishLine.bmp", NULL);
    image.taupe = load_bitmap("images/taupe.bmp", NULL);

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
    police.Hudson_26 = load_font("polices/hudson_party_26.pcx", NULL, NULL);
    return police;
}




void initAllegro()
{

    srand(time(NULL)); //on initialise la fonction random 
    allegro_init();    //initialisation allegro
    install_keyboard(); //on permet l'utilisation du clavier
    install_mouse();  //on permet l'utilisation de la souris
    set_color_depth(desktop_color_depth());    //initialisation de la palette de couleur




    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,SCREEN_W,SCREEN_H,0,0))!=0)      //paramètres fenêtre
    {
        //problème avec fenêtre allegro
        allegro_message("Pb de mode graphique");
        allegro_exit();                                             //fermeture allegro
        exit(EXIT_FAILURE);                                         //sortie forcée du programme
    }

    show_mouse(screen); //on affiche la souris à l'écran




}


int isClickInCoord(int x1, int x2, int y1, int y2)
{


    if((mouse_x>x1 && mouse_x<x2)&&(mouse_y>y1 && mouse_y<y2)&&mouse_b&1)
    {

        return 1;


    }
    else return 0;


}
