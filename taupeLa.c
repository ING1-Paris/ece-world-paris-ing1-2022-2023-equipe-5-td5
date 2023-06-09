#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define SCREEN_W 1500 //largeur de la fenêtre allegro
#define SCREEN_H 1000  //hauteur de la fenêtre allegro

typedef struct banqueImage //  Structure utilisée pour stocker les images utilisées pour le projet
{
    BITMAP * fondMenu; //fond du menu principal
    BITMAP * joueur; // sprite du joueur
    BITMAP * batMinigame[3]; //batiments représentant les mini jeux
    BITMAP * finishLine; // lgne d'arrivée de la course hippique
    BITMAP * taupe; //taupe

}t_banqueImage;


typedef struct banquePolice //  Structure utilisée pour stocker les polices utilisées pour le projet
{
    FONT * Roller_72;
    FONT * Roller_it_24;
    FONT * Pixel_11;
    FONT * System_14;
    FONT * Hudson_26;

}t_banquePolice;

typedef struct joueur //structure chargée de stocker les informations d'un joueur
{
    char* name; //nom du joueur
    int taillenom; //taille du nom  
    int tickets;  //nombre de tickets  
    int score;  //score
    int color;  //couleur

} t_joueur;

typedef struct minigame //structure chargée de stocker les informations d'une attraction
{   
    char titre[20]; //nom   
    int difficulty; //difficulées
    //specs
    int coordBat[2][2]; //tab de cases prises par le bat   
    int coordPlay[2]; //coord de la case permettant de lancer le minijeu

} t_minigame;

typedef struct terrain //structure permettant de stocker les informations de la carte où l'on se déplace pour atteindre les différentes attractions
{
    int occupied; //0=non (traversable) 1=oui (pas traversable)
    int isStartAttraction; //savoir si la case est un emplacement pour démarrer une attraction
    int isPlayerPos; //savoir si le joueur se trouve sur cette case
} t_terrain;

typedef struct partie //structure principale chargée de stocker toutes les infos de la partie
{
    t_joueur playerBase[2];   //tab de 2 joueurs   
    t_minigame jeux[4]; //tab de 4 minigame   
    t_terrain plateau[15][15]; //carte du jeu
} t_partie;

void initAllegro(); //SSPG chargé d'initialiser les fonctions d'allegro
int isClickInCoord(int x1, int x2, int y1, int y2);
void barreChargement();
t_banquePolice loadBanquePolices();
t_banqueImage loadBanqueImage();
t_partie taupeLa(t_partie partie, t_banqueImage image, t_banquePolice police);
int isClickInRadius(int x, int y, int rad);

int main()
{
    //les taupes apparaissent par vagues toutes les 2 à 3 sec
    //elles restent entre 1 et 2 sec
    //20 sec par round

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
    
    partie=taupeLa(partie, image, police);
    return 0;
    
}END_OF_MAIN()
    
t_partie taupeLa(t_partie partie, t_banqueImage image, t_banquePolice police)
{
   int score=0;

    BITMAP * buffer = create_bitmap(SCREEN_W,SCREEN_H); //déclaration du buffer

    int tabTaupes[13][3]; //tableau d'entier en 13x3, 2 coords pour chaque emplacement + un indicateur si la taupe s'affiche

    ///COORDONNEES DES TROUS DE TAUPES
    tabTaupes[0][0] = (243*1)+20;
    tabTaupes[0][1] = (200*1)+20;

    tabTaupes[1][0] = (243*1)+20;
    tabTaupes[1][1] = (200*2)+20;

    tabTaupes[2][0] = (243*1)+20;
    tabTaupes[2][1] = (200*3)+20;

    tabTaupes[3][0] = (243*2)+20;
    tabTaupes[3][1] = (266*1)+20;

    tabTaupes[4][0] = (243*2)+20;
    tabTaupes[4][1] = (266*2)+20;

    tabTaupes[5][0] = (243*3)+20;
    tabTaupes[5][1] = (200*1)+20;

    tabTaupes[6][0] = (243*3)+20;
    tabTaupes[6][1] = (200*2)+20;

    tabTaupes[7][0] = (243*3)+20;
    tabTaupes[7][1] = (200*3)+20;

    tabTaupes[8][0] = (243*4)+20;
    tabTaupes[8][1] = (266*1)+20;

    tabTaupes[9][0] = (243*4)+20;
    tabTaupes[9][1] = (266*2)+20;

    tabTaupes[10][0] = (243*5)+20;
    tabTaupes[10][1] = (200*1)+20;

    tabTaupes[11][0] = (243*5)+20;
    tabTaupes[11][1] = (200*2)+20;

    tabTaupes[12][0] = (243*5)+20;
    tabTaupes[12][1] =  (200*3)+20;

    ///INITIALISATION DU STATUS DES TROUS
    for(int i=0; i<13; i++)
    {
        tabTaupes[i][2] = 1; //le trou n'as pas de taupe
    }
    int flip=1;
     while(!key[KEY_ESC])
    {
        clear_bitmap(buffer);
        printf("%d ", score);

        ///AFFICHAGE INTERFACE
        rectfill(buffer, 0,0, SCREEN_W, SCREEN_H, makecol(203,32,38)); //affichage des bords
        rectfill(buffer, 20,20, SCREEN_W-20, SCREEN_H-20, makecol(114,95,79)); //affichage du fond

        rectfill(buffer, 0 ,SCREEN_H-200, SCREEN_W, SCREEN_H, makecol(0,0,0));
        rectfill(buffer, 0+10,SCREEN_H-200+10, SCREEN_W-10, SCREEN_H-10, makecol(136,140,141)); //affichage bordereau d'information

        rectfill(buffer, 486-3, SCREEN_H-200, 486+3,  SCREEN_H, makecol(0,0,0));
        rectfill(buffer, 972-3, SCREEN_H-200, 972+3,  SCREEN_H, makecol(0,0,0));

        ///AFFICHAGE TROUS TAUPES

        for(int i=0; i<13; i++)
        {
            circlefill(buffer, tabTaupes[i][0], tabTaupes[i][1], 75, makecol(0,0,0));
            if(tabTaupes[i][2]==1)
            {
                //on affiche la taupe
                circlefill(buffer, tabTaupes[i][0], tabTaupes[i][1], 75, makecol(255,255,255));
                masked_blit(image.taupe, buffer,0,0, tabTaupes[i][0]-35, tabTaupes[i][1]-35, 75,75 );
            }
        }

        ///DETECT CLICK TAUPE
        //on boucle pour chaque emplacement de taupe possible
        for(int i=0; i<13; i++)
        {
            if(isClickInRadius(tabTaupes[i][0], tabTaupes[i][1], 75)&&tabTaupes[i][2]==1) //calcul si le click est dans le rayon de la taupe et qu'il y en a une
            {
                tabTaupes[i][2]=0; //on supprime la taupe
                score++; //on augmente le score
            }
        }
        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }
   // partie=taupeLa(partie, image, police);
    return 0;
}
END_OF_MAIN()

t_partie taupeLa(t_partie partie, t_banqueImage image, t_banquePolice police)
{
    int score=0;
    BITMAP * buffer = create_bitmap(SCREEN_W,SCREEN_H); //déclaration du buffer
    int tabTaupes[13][3]; //tableau d'entier en 13x3, 2 coords pour chaque emplacement + un indicateur si la taupe s'affiche
    int playerScore[2];

    ///COORDONNEES DES TROUS DE TAUPES
    tabTaupes[0][0] = (243*1)+20;
    tabTaupes[0][1] = (200*1)+20;

    tabTaupes[1][0] = (243*1)+20;
    tabTaupes[1][1] = (200*2)+20;

    tabTaupes[2][0] = (243*1)+20;
    tabTaupes[2][1] = (200*3)+20;

    tabTaupes[3][0] = (243*2)+20;
    tabTaupes[3][1] = (266*1)+20;

    tabTaupes[4][0] = (243*2)+20;
    tabTaupes[4][1] = (266*2)+20;

    tabTaupes[5][0] = (243*3)+20;
    tabTaupes[5][1] = (200*1)+20;

    tabTaupes[6][0] = (243*3)+20;
    tabTaupes[6][1] = (200*2)+20;

    tabTaupes[7][0] = (243*3)+20;
    tabTaupes[7][1] = (200*3)+20;

    tabTaupes[8][0] = (243*4)+20;
    tabTaupes[8][1] = (266*1)+20;

    tabTaupes[9][0] = (243*4)+20;
    tabTaupes[9][1] = (266*2)+20;

    tabTaupes[10][0] = (243*5)+20;
    tabTaupes[10][1] = (200*1)+20;

    tabTaupes[11][0] = (243*5)+20;
    tabTaupes[11][1] = (200*2)+20;

    tabTaupes[12][0] = (243*5)+20;
    tabTaupes[12][1] =  (200*3)+20;
    
    
    time_t start_time = time(NULL);  // Temps de départ
    time_t elapsed_time = 0;         // Temps écoulé
    time_t duration = 15;            // Durée du round
    time_t remaining_time; //vairable de temps utilisée pour afficher le temps restant
    time_t tick; //variable utilisée pour détecter des actions dans le temps

    for(int i=0; i<2; i++)
    {
        partie.playerBase[i].tickets--; //on retire un ticket à chaque joueur
    }
for(int pTurn=0; pTurn<2; pTurn++)
    {
        ///DEBUT DU JEU

        //INITIALISATION DU STATUS DES TROUS
        for(int i=0; i<13; i++)
        {
            tabTaupes[i][2] = 0; //le trou n'as pas de taupe
        }
        //initialisation du score du joueur à 0
        playerScore[pTurn]=0;

        loadStartGame(police, partie,3, pTurn );

        ///DEROULEMENT DU JEU

        //remise à 0 du temps
        start_time = time(NULL);
        elapsed_time = 0;
        tick = 0;


        while(elapsed_time < duration)
        {
            clear_bitmap(buffer);

            remaining_time = duration - elapsed_time;
            elapsed_time = time(NULL) - start_time;
            
 
            ///AFFICHAGE INTERFACE
            rectfill(buffer, 0,0, SCREEN_W, SCREEN_H, makecol(203,32,38)); //affichage des bords
            rectfill(buffer, 20,20, SCREEN_W-20, SCREEN_H-20, makecol(114,95,79)); //affichage du fond

            rectfill(buffer, 0,SCREEN_H-200, SCREEN_W, SCREEN_H, makecol(0,0,0));
            rectfill(buffer, 0+10,SCREEN_H-200+10, SCREEN_W-10, SCREEN_H-10, makecol(136,140,141)); //affichage bordereau d'information


            rectfill(buffer, 486-3, SCREEN_H-200, 486+3,  SCREEN_H, makecol(0,0,0)); //délimitation des cases
            rectfill(buffer, 972-3, SCREEN_H-200, 972+3,  SCREEN_H, makecol(0,0,0));

            //tour du joueur actuel
            textout_centre_ex(buffer, police.Hudson_26, "Tour de :",(SCREEN_W)/7+40, SCREEN_H-175, makecol(255,255,255),-1 );
            textprintf_centre_ex(buffer, police.Hudson_26, (SCREEN_W)/7+40, SCREEN_H-125,  partie.playerBase[pTurn].color, -1, "%s", partie.playerBase[pTurn].name);

            //temps restant
            textout_centre_ex(buffer, police.Hudson_26, "Temps :",(SCREEN_W)/2, SCREEN_H-175, makecol(255,255,255),-1 );
            textprintf_centre_ex(buffer, police.Hudson_26, SCREEN_W/2, SCREEN_H-125, makecol(255,255,255), -1, "%lds", (long  int)remaining_time);


            //score
            textout_centre_ex(buffer, police.Hudson_26, "Score:",(SCREEN_W)*6/7-40, SCREEN_H-175, makecol(255,255,255),-1 );
            textprintf_centre_ex(buffer, police.Hudson_26, (SCREEN_W)*6/7-40, SCREEN_H-125,  makecol(255,255,255), -1, "%d", playerScore[pTurn]);


            ///CALCUL APPARITION/DISPARITION TAUPES
            if(elapsed_time>tick) //si une seconde est passée
            {
                tick = elapsed_time;//on remet le tick au temps actuel

                for(int i=0; i<13; i++)
                {
                    //si la taupe est déjà présente

                    if(tabTaupes[i][2]>0)
                    {
                        tabTaupes[i][2]--;//on lui retire une seconde de son temps d'apparition restant
                    }
                    //sinon
                    else if(tabTaupes[i][2]==0)
                    {
                        if(rand()%5==0)//on tire au hasard voir si elle apparait
                        {
                            tabTaupes[i][2] = rand()%3 +1; //si elle apparait, on lui donne une durée aléatoire
                        }
                    }

                }
            }

            ///AFFICHAGE TROUS TAUPES

            for(int i=0; i<13; i++)
            {
                circlefill(buffer, tabTaupes[i][0], tabTaupes[i][1], 75, makecol(0,0,0));
                if(tabTaupes[i][2]>0)
                {
                    //on affiche la taupe
                    circlefill(buffer, tabTaupes[i][0], tabTaupes[i][1], 75, makecol(255,255,255));
                    masked_blit(image.taupe, buffer,0,0, tabTaupes[i][0]-35, tabTaupes[i][1]-35, 75,75 );
                }
            }


            ///DETECT CLICK TAUPE
            //on boucle pour chaque emplacement de taupe possible
            for(int i=0; i<13; i++)
            {
                if(isClickInRadius(tabTaupes[i][0], tabTaupes[i][1], 75)&&tabTaupes[i][2]>0) //calcul si le click est dans le rayon de la taupe et qu'il y en a une
                {
                    tabTaupes[i][2]=0; //on supprime la taupe
                    playerScore[pTurn]++; //on augmente le score
                }
            }
            blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        }

        ///FIN DU JEU
    }

     start_time = time(NULL);  // Temps de départ
    elapsed_time = 0;         // Temps écoulé
     duration = 3;
 
    
        //RESULTAT DU JEU 
    if(playerScore[0]<playerScore[1])//si le joueur 1 a un score plus bas que le joueur 2
    {
        partie.playerBase[1].tickets++;
        //victoire joueur 2
        while(elapsed_time < duration)
        {


            elapsed_time = time(NULL) - start_time;

            clear_bitmap(buffer);


            rectfill(buffer, 0,0, SCREEN_W, SCREEN_H, makecol(203,32,38)); //affichage des bords
            rectfill(buffer, 20,20, SCREEN_W-20, SCREEN_H-20, makecol(114,95,79)); //affichage du fond

            rectfill(buffer, 0,SCREEN_H-200, SCREEN_W, SCREEN_H, makecol(0,0,0));
            rectfill(buffer, 0+10,SCREEN_H-200+10, SCREEN_W-10, SCREEN_H-10, makecol(136,140,141)); //affichage bordereau d'information


            rectfill(buffer, 486-3, SCREEN_H-200, 486+3,  SCREEN_H, makecol(0,0,0)); //délimitation des cases
            rectfill(buffer, 972-3, SCREEN_H-200, 972+3,  SCREEN_H, makecol(0,0,0));




            rectfill(buffer,SCREEN_W/4-20,SCREEN_H/4-20, SCREEN_W*3/4+20,SCREEN_H*3/4+20, makecol(0,0,0));
            rectfill(buffer,SCREEN_W/4,SCREEN_H/4, SCREEN_W*3/4,SCREEN_H*3/4, makecol(100,100,100)); // affichage de l'encarte

            textprintf_centre_ex(buffer, police.Hudson_26, SCREEN_W/2, SCREEN_H/2-150, makecol(255,255,255), -1, "Victoire  de %s !", partie.playerBase[1].name);

            blit(buffer, screen,0,0,0,0,SCREEN_W,SCREEN_H);
        }


    }
    else if(playerScore[0]>playerScore[1])//si le joueur 1 a un score plus haut que le joueur 2
    {
        //victoire joueur 1

        partie.playerBase[0].tickets++;

        while(elapsed_time < duration)
        {


            elapsed_time = time(NULL) - start_time;

            clear_bitmap(buffer);


            rectfill(buffer, 0,0, SCREEN_W, SCREEN_H, makecol(203,32,38)); //affichage des bords
            rectfill(buffer, 20,20, SCREEN_W-20, SCREEN_H-20, makecol(114,95,79)); //affichage du fond

            rectfill(buffer, 0,SCREEN_H-200, SCREEN_W, SCREEN_H, makecol(0,0,0));
            rectfill(buffer, 0+10,SCREEN_H-200+10, SCREEN_W-10, SCREEN_H-10, makecol(136,140,141)); //affichage bordereau d'information


            rectfill(buffer, 486-3, SCREEN_H-200, 486+3,  SCREEN_H, makecol(0,0,0)); //délimitation des cases
            rectfill(buffer, 972-3, SCREEN_H-200, 972+3,  SCREEN_H, makecol(0,0,0));




            rectfill(buffer,SCREEN_W/4-20,SCREEN_H/4-20, SCREEN_W*3/4+20,SCREEN_H*3/4+20, makecol(0,0,0));
            rectfill(buffer,SCREEN_W/4,SCREEN_H/4, SCREEN_W*3/4,SCREEN_H*3/4, makecol(100,100,100)); // affichage de l'encarte

            textprintf_centre_ex(buffer, police.Hudson_26, SCREEN_W/2, SCREEN_H/2-150, makecol(255,255,255), -1, "Victoire  de %s !", partie.playerBase[0].name);

            blit(buffer, screen,0,0,0,0,SCREEN_W,SCREEN_H);
        }
    }
    else
    {
        //égalité, les 2 joueurs gagnent un ticket
        partie.playerBase[0].tickets++;
        partie.playerBase[1].tickets++;
        //victoire joueur 2
        while(elapsed_time < duration)
        {


            elapsed_time = time(NULL) - start_time;

            clear_bitmap(buffer);


            rectfill(buffer, 0,0, SCREEN_W, SCREEN_H, makecol(203,32,38)); //affichage des bords
            rectfill(buffer, 20,20, SCREEN_W-20, SCREEN_H-20, makecol(114,95,79)); //affichage du fond

            rectfill(buffer, 0,SCREEN_H-200, SCREEN_W, SCREEN_H, makecol(0,0,0));
            rectfill(buffer, 0+10,SCREEN_H-200+10, SCREEN_W-10, SCREEN_H-10, makecol(136,140,141)); //affichage bordereau d'information


            rectfill(buffer, 486-3, SCREEN_H-200, 486+3,  SCREEN_H, makecol(0,0,0)); //délimitation des cases
            rectfill(buffer, 972-3, SCREEN_H-200, 972+3,  SCREEN_H, makecol(0,0,0));




            rectfill(buffer,SCREEN_W/4-20,SCREEN_H/4-20, SCREEN_W*3/4+20,SCREEN_H*3/4+20, makecol(0,0,0));
            rectfill(buffer,SCREEN_W/4,SCREEN_H/4, SCREEN_W*3/4,SCREEN_H*3/4, makecol(100,100,100)); // affichage de l'encarte

            textprintf_centre_ex(buffer, police.Hudson_26, SCREEN_W/2, SCREEN_H/2-150, makecol(255,255,255), -1, "Egalite!");

            blit(buffer, screen,0,0,0,0,SCREEN_W,SCREEN_H);
        }

    }


    //remplacement des high score et score de partie
    for(int i=0; i<2; i++)
    {
        //on check si le score que le joueur viens de faire est son meilleur score ou son premier
        if(partie.playerBase[i].score[2]>playerScore[i]||partie.playerBase[i].score[2]==-1)
        {
            partie.playerBase[i].score[2]=playerScore[i];
        }
        //on check si le score que le joueur viens de faire est le meilleur score de tout les temps
        if(partie.playerBase[i].score[2]<partie.tabHighScore[2])
        {
            partie.tabHighScore[2]=partie.playerBase[i].score[2];
        }
    }
    //on échange l'ordre de jeu des 2 joueurs
    t_joueur jtemp;//structure de stockage temporaire

    jtemp=partie.playerBase[0];
    partie.playerBase[0]=partie.playerBase[1];
    partie.playerBase[1]=jtemp;

    return partie;
}

            //---------------------------A CONTINUER------------------------------------------
    
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
    set_color_depth(desktop_color_depth());  //initialisation de la palette de couleur

    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,SCREEN_W,SCREEN_H,0,0))!=0) //paramètres fenêtre
    {
        //problème avec fenêtre allegro
        allegro_message("Pb de mode graphique");
        allegro_exit();    //fermeture allegro
        exit(EXIT_FAILURE);  //sortie forcée du programme
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
