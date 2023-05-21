#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <math.h>
#include <string.h>
#include <time.h>


#define SCREEN_W 1500 //largeur fenêtre allegro
#define SCREEN_H 1000  //hauteur fenêtre allegro


typedef struct banqueImage   //Structure utilisée pour stocker les images utilisées 
{
    BITMAP * fondMenu;          //fond du menu principal
    BITMAP * joueur;            // sprite du joueur
    BITMAP * batMinigame[3];    //batiments représentant les mini jeux
    BITMAP * finishLine;        //ligne d'arrivée de la course hippique
    BITMAP * taupe;             //taupe

}t_banqueImage;

typedef struct banquePolice //Structure utilisée pour stocker les polices utilisées
{
    FONT * Roller_72;
    FONT * Roller_it_24;
    FONT * Pixel_11;
    FONT * System_14;
    FONT * Hudson_26;

}t_banquePolice;



typedef struct joueur //structure chargée de stocker les informations d'un joueur
{
    char* name;     //nom du joueur
    int taillenom;  //taille du nom
    int tickets;    //nombre de tickets
    int score;      //score
    int color;      //couleur

} t_joueur;



typedef struct minigame  //structure chargée de stocker les informations d'une attraction
{
    char titre[20];      //nom
    int difficulty;      //difficulées
    //specs
    int coordBat[2][2];  //tab de cases prises par le bat
    int coordPlay[2];    //coord de la case permettant de lancer le minijeu

} t_minigame;




typedef struct terrain      //structure permettant de stocker les informations de la carte où l'on se déplace pour atteindre les différentes attractions
{
    int occupied;            //0=non (traversable) 1=oui (pas traversable)
    int isStartAttraction;   //savoir si la case est un emplacement pour démarrer une attraction
    int isPlayerPos;         //savoir si le joueur se trouve sur cette case
    
} t_terrain;



typedef struct partie //structure principale chargée de stocker toutes les infos de la partie
{
    t_joueur playerBase[2];     //tab de 2 joueurs
    t_minigame jeux[4];         //tab de 4 minigame
    t_terrain plateau[15][15];  //carte du jeu

} t_partie;



void initAllegro();      //SSPG chargé d'initialiser les fonctions d'allegro
int isClickInCoord(int x1, int x2, int y1, int y2);
void barreChargement();
t_banquePolice loadBanquePolices();
t_partie parisHippiques(t_partie partie, t_banqueImage image, t_banquePolice police );
int isClickInRadius(int x, int y, int rad);
t_banqueImage loadBanqueImage();




//---------------------------------MAIN-------------------------------------

int main()
{

    //4 options pour miser sur un cheval (Rouge, Vert, Bleu, Jaune)
    //chaque joueur paris sur un cheval (possible que ce soit le même pour les 2 joueurs

    //4 vitesses préexistantes, assigné random au lancement de la course

    //si un ou plusieurs joueur mise sur le bon cheval, il gagne un ticket


    t_partie partie;
    t_banqueImage image;
    t_banquePolice police;


    initAllegro();   //initialisation d'allegro

    police = loadBanquePolices();
    image = loadBanqueImage();




    partie.playerBase[0].name =malloc(sizeof(strlen("Jean")));
    strcpy(partie.playerBase[0].name, "Jean");

    partie.playerBase[1].name = malloc(sizeof(strlen("Valjean")));
    strcpy(partie.playerBase[1].name, "Valjean");
    
    
    BITMAP * buffer = create_bitmap(SCREEN_W,SCREEN_H);  //déclaration du buffer



    int tabHorseSpeed[4];  //tableau des différentes vitesses selon les chevaux
    int tabPlayerBet[2];   //tableau des paris des joueurs
    int temp;
    srand(time(NULL));    //aléatoire
    int rand1, rand2;     //variables pour le mélange


    //vitesse des différents chevaux
    tabHorseSpeed[0]= 1;
    tabHorseSpeed[1]= 2;
    tabHorseSpeed[2]= 3;
    tabHorseSpeed[3]= 4;

    //paris des deux joueurs à 0
    tabPlayerBet[0]=0; //pari du joueur 1
    tabPlayerBet[1]=0; //pari du joueur 2


    ///MELANGE ALEATOIRE DES VITESSES
    for(int i=0; i<4*4; i++) //on boucle sur 2 fois la taille du tableau
    {
        rand1=(rand()% 4); //on génère un premier random entre 0 et 4 (taille max de playerBase)
        do
        {
            rand2=(rand()%4); //on en génère un deuxième obligatoirement différent du deuxième
        }
        while(rand1==rand2);

        //on mélange le tableau avant chaque début de partie pour que les choix soient aléatoires
        temp = tabHorseSpeed[rand1];
        tabHorseSpeed[rand1] = tabHorseSpeed[rand2];
        tabHorseSpeed[rand2]=temp;
    }





    while(!key[KEY_ESC])
    {
        clear_bitmap(buffer);


        rectfill(buffer, 0,0, SCREEN_W, SCREEN_H, makecol(65,107,223));
        rectfill(buffer, 0+20,0+20, SCREEN_W-20, SCREEN_H-20, makecol(62,136,51));


        textprintf_centre_ex(buffer, police.Hudson_26, (SCREEN_W-40)/3, 20,  makecol(17,75,200), -1, "%s", partie.playerBase[0].name);
        textprintf_centre_ex(buffer, police.Hudson_26, ((SCREEN_W-40)*2)/3, 20,  makecol(129,30,30), -1, "%s", partie.playerBase[1].name);

        //affichage des lignes de courses
        for(int i=0; i<4; i++)
        {
            rectfill(buffer, 250, 200+150*i, 1250, 225+150*i, makecol(0,0,0));
            rect(buffer, 175, 175+150*i, 250, 250+150*i, makecol(255,255,255));
        }

        masked_blit(image.finishLine, buffer, 0,0,1250, 175, 60, 525 );


        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }



    //barreChargement();


  //  partie= parisHippiques(partie, image, police);


    return 0;
}
END_OF_MAIN()


    
    
t_partie parisHippiques(t_partie partie, t_banqueImage image, t_banquePolice police )
{

    BITMAP * buffer = create_bitmap(SCREEN_W,SCREEN_H); //déclaration du buffer


    int tabHorseSpeed[4]; //tableau des différentes vitesses selon les chevaux
    int tabPlayerBet[2]; //tableau des paris des joueurs
    int temp;
    srand(time(NULL)); //utilisation de l'aléatoire
    int rand1, rand2; //variables utilisées pour le mélange


    //set la vitesse des différents chevaux
    tabHorseSpeed[0]= 1;
    tabHorseSpeed[1]= 2;
    tabHorseSpeed[2]= 3;
    tabHorseSpeed[3]= 4;

    //set le paris des deux joueurs à 0
    tabPlayerBet[0]=0; //pari du joueur 1
    tabPlayerBet[1]=0; //pari du joueur 2


    ///MELANGE ALEATOIRE DES VITESSES
    for(int i=0; i<4*4; i++) //on boucle sur 2 fois la taille du tableau
    {
        rand1=(rand()% 4); //on génère un premier random entre 0 et 4 (taille max de playerBase)
        do
        {
            rand2=(rand()%4); //on en génère un deuxième obligatoirement différent du deuxième
        }
        while(rand1==rand2);

        //on mélange le tableau avant chaque début de partie pour que les choix soient aléatoires
        temp = tabHorseSpeed[rand1];
        tabHorseSpeed[rand1] = tabHorseSpeed[rand2];
        tabHorseSpeed[rand2]=temp;
    }


    while(!key[KEY_ESC])
    {
        clear_bitmap(buffer);


        rectfill(buffer, 0,0, SCREEN_W, SCREEN_H, makecol(65,107,223));
        rectfill(buffer, 0+20,0+20, SCREEN_W-20, SCREEN_H-20, makecol(62,136,51));


        textprintf_centre_ex(buffer, police.Hudson_26, (SCREEN_W-40)/3, 20,  makecol(17,75,200), -1, "%s", partie.playerBase[0].name);
        textprintf_centre_ex(buffer, police.Hudson_26, ((SCREEN_W-40)*2)/3, 20,  makecol(129,30,30), -1, "%s", partie.playerBase[1].name);

        //affichage des lignes de courses
        for(int i=0; i<4; i++)
        {
            rectfill(buffer, 250, 200+150*i, 1250, 225+150*i, makecol(0,0,0));
            rect(buffer, 175, 175+150*i, 250, 250+150*i, makecol(255,255,255));
        }

        masked_blit(image.finishLine, buffer, 0,0,1250, 175, 60, 525 );


        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }

    return partie;
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

    srand(time(NULL)); //on initialise la fonction random (utile pour plus tard)
    allegro_init();    //initialisation allegro
    install_keyboard(); //on permet l'utilisation du clavier
    install_mouse();  //on permet l'utilisation de la souris
    set_color_depth(desktop_color_depth());    //initialisation de la palette de couleur


    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,SCREEN_W,SCREEN_H,0,0))!=0)      //paramètres fenêtre
    {
        //problème avec fenêtre allegro
        allegro_message("Pb de mode graphique");
        allegro_exit();                         //fermeture allegro
        exit(EXIT_FAILURE);                    //sortie forcée du programme
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



int isClickInRadius(int x, int y, int rad)
{

    if(mouse_b&1 && (rad>= sqrt(pow(x-mouse_x,2)+pow(y-mouse_y, 2))) )
    {
        return 1;
    }
    else return 0;
    
}


