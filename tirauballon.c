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


} t_banqueImage;

typedef struct banquePolice //  Structure utilisée pour stocker les polices utilisées pour le projet
{
    FONT * Roller_72;
    FONT * Roller_it_24;
    FONT * Pixel_11;
    FONT * System_14;
    FONT * Hudson_26;

} t_banquePolice;

typedef struct joueur //structure chargée de stocker les informations d'un joueur
{
    //nom
    char* name; //nom du joueur
    //taille du nom
    int taillenom;
    //nombre de tickets
    int tickets;
    //score
    int score[3];
    //couleur
    int color;

} t_joueur;


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

} t_minigame;


typedef struct terrain //structure permettant de stocker les informations de la carte où l'on se déplace pour atteindre les différentes attractions
{
    int occupied; //0=non (traversable) 1=oui (pas traversable)
    int isStartAttraction; //savoir si la case est un emplacement pour démarrer une attraction
    int isPlayerPos; //savoir si le joueur se trouve sur cette case
} t_terrain;



typedef struct partie //structure principale chargée de stocker toutes les infos de la partie
{
    //tab de 2 joueurs
    t_joueur playerBase[2];
    //tab de 4 minigame
    t_minigame jeux[4];
    //carte du jeu
    t_terrain plateau[15][15];

    int tabHighScore[3]; //tableau stockant les meilleurs scores de tout les temps



} t_partie;

void initAllegro(); //SSPG chargé d'initialiser les fonctions d'allegro
int isClickInCoord(int x1, int x2, int y1, int y2);

t_banquePolice loadBanquePolices();
t_banqueImage loadBanqueImage();
t_partie tirAuBallon(t_partie partie, t_banqueImage image, t_banquePolice police );
void creerCoordBallons(int tabCoordBallon[10][2]);
int collision(int tabCoordBallon[10][2], int nbGen);
void loadStartGame(t_banquePolice police, t_partie partie, int game, int actualP);

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

    partie.playerBase[0].color=makecol(17,75,200);
    partie.playerBase[1].color=makecol(129,30,30);





    partie = tirAuBallon(partie, image, police);





    return 0;
}
END_OF_MAIN()


t_partie tirAuBallon(t_partie partie, t_banqueImage image, t_banquePolice police )
{

    BITMAP * buffer = create_bitmap(SCREEN_W,SCREEN_H); //déclaration du buffer

    srand(time(NULL));

    time_t start_time = time(NULL);  // Temps de départ
    time_t elapsed_time = 0;         // Temps écoulé



    //10 ballon, rouge, radius de 55px
    int tabCoordBallon[10][2]; //tableaux d'entier en 2x10, pour une paire de coord pour chaque ballons
    int tabVectBallon[10][2]; //tableaux d'entier en 2x10, pour un vecteur de direction par ballon
    int score;
    int playerTemps[2];

    for(int i=0; i<2; i++)
    {
        partie.playerBase[i].tickets--; //on retire un ticket à chaque joueur
    }

    for(int pTurn=0; pTurn<2; pTurn++) //on boucle pour les deux joueurs
    {
        ///INITIALISATION DU JEU

        loadStartGame(police, partie, 1, pTurn);//on affiche a quel joueur ça va être le tour

        score=0;
        start_time = time(NULL);  // Temps de départ
        elapsed_time = 0;         // Temps écoulé

        ///CREATION DES BALLONS
        creerCoordBallons(tabCoordBallon);

        ///ASSIGNATION DES VECTEURS
        for(int i=0; i<10; i++)//pour chaque ballon
        {
            do
            {
                tabVectBallon[i][0]= rand() % (6) -3; //on génère aléatoirement 2 vecteurs de déplacement
                tabVectBallon[i][1]= rand() % (6) -3;
            }
            while(tabVectBallon[i][0]==0 || tabVectBallon[i][1]==0);//on reccomence tant que les vecteurs sont nuls
        }

        ///DEROULEMENT DU JEU

        //boucle de jeu
        while(score<10)
        {
            clear_bitmap(buffer);

            ///CALCUL DU TEMPS
            elapsed_time = time(NULL) - start_time;

            ///AFFICHAGE INTERFACE
            rectfill(buffer, 0,0, SCREEN_W, SCREEN_H, makecol(203,32,38)); //affichage des bords
            rectfill(buffer, 20,20, SCREEN_W-20, SCREEN_H-20, makecol(114,95,79)); //affichage du fond

            rectfill(buffer, 0,SCREEN_H-200, SCREEN_W, SCREEN_H, makecol(0,0,0));
            rectfill(buffer, 0+10,SCREEN_H-200+10, SCREEN_W-10, SCREEN_H-10, makecol(136,140,141)); //affichage bordereau d'information


            rectfill(buffer, 486-3, SCREEN_H-200, 486+3,  SCREEN_H, makecol(0,0,0)); //délimitation des cases
            rectfill(buffer, 972-3, SCREEN_H-200, 972+3,  SCREEN_H, makecol(0,0,0));

            //pseudo du joueur
            textout_centre_ex(buffer, police.Hudson_26, "Tour de :",(SCREEN_W)/7+40, SCREEN_H-175, makecol(255,255,255),-1 );
            textprintf_centre_ex(buffer, police.Hudson_26, (SCREEN_W)/7+40, SCREEN_H-125,  partie.playerBase[pTurn].color, -1, "%s", partie.playerBase[pTurn].name);

            //temps écoulé
            textout_centre_ex(buffer, police.Hudson_26, "Temps :",(SCREEN_W)/2, SCREEN_H-175, makecol(255,255,255),-1 );
            textprintf_centre_ex(buffer, police.Hudson_26, (SCREEN_W)/2, SCREEN_H-125,  makecol(255,255,255), -1, "%lds",  (long  int)elapsed_time);

            //score
            textout_centre_ex(buffer, police.Hudson_26, "Score:",(SCREEN_W)*6/7-40, SCREEN_H-175, makecol(255,255,255),-1 );
            textprintf_centre_ex(buffer, police.Hudson_26, (SCREEN_W)*6/7-40, SCREEN_H-125,  makecol(255,255,255), -1, "%d /10", score);


            ///CALCUL SCORE AFFICHEE
            score=0;
            for(int i=0; i<10; i++)
            {
                if(tabCoordBallon[i][0]==0 && tabCoordBallon[i][1]==0) score++;
            }



            ///CALCUL TRAJECTOIRE BALLON ET COLLISION

            for(int i=0; i<10; i++)
            {
                //on regarde si le ballon existe toujours
                if(tabCoordBallon[i][0]!=0 && tabCoordBallon[i][1]!=0)
                {
                    //si on sors du plateau en x
                    if(tabCoordBallon[i][0]+tabVectBallon[i][0]>SCREEN_W-20-55 ||tabCoordBallon[i][0]+tabVectBallon[i][0]<20+55)
                    {
                        tabVectBallon[i][0] = - tabVectBallon[i][0];//on ajuste le vexteurs x
                    }
                    //si on sors du plateau en y
                    if(tabCoordBallon[i][1]+tabVectBallon[i][1]>SCREEN_H-200-55||tabCoordBallon[i][1]+tabVectBallon[i][1]<55+20)
                    {
                        tabVectBallon[i][1] = - tabVectBallon[i][1];//on ajuste le vecteur y
                    }

                    //on actualise les coordonnées du ballon en fonction des vecteurs
                    tabCoordBallon[i][0]= tabCoordBallon[i][0]+tabVectBallon[i][0];
                    tabCoordBallon[i][1]= tabCoordBallon[i][1]+tabVectBallon[i][1];

                    //on check si il y a des collision
                    for(int j=0; j<i; j++)
                    {
                        //si il y une collision entre 2 ballons
                        if(110>sqrt(pow(tabCoordBallon[i][0] - tabCoordBallon[j][0], 2) + pow(tabCoordBallon[i][1]- tabCoordBallon[j][1], 2)))
                        {
                            //on inverse les vecteurs des 2 ballons
                            tabVectBallon[i][0] = - tabVectBallon[i][0];
                            tabVectBallon[i][1] = - tabVectBallon[i][1];
                            tabVectBallon[j][0] = - tabVectBallon[j][0];
                            tabVectBallon[j][1] = - tabVectBallon[j][1];
                        }
                    }

                    if(rand()%100==0)// 1 chance sur 100 de modifier aléatoirement le vecteur x
                    {
                        do
                        {
                            tabVectBallon[i][0]= rand() % (6) -3;

                        }
                        while(tabVectBallon[i][0]==0 );

                    }
                    if(rand()%100==0)// 1 chance sur 100 de modifier aléatoirement le vecteur y
                    {
                        do
                        {
                            tabVectBallon[i][1]= rand() % (6) -3;

                        }
                        while(tabVectBallon[i][1]==0 );
                    }

                }

            }



            ///AFFICHAGE BALLON
            for(int i=0; i<10; i++)
            {
                if(tabCoordBallon[i][0]!=0 && tabCoordBallon[i][1]!=0) circlefill(buffer, tabCoordBallon[i][0], tabCoordBallon[i][1], 55, makecol(255,0,0));
            }


            //calcul si le click est dans le rayon du ballon
            for(int i=0; i<10; i++)
            {
                if(mouse_b&1 && (55>= sqrt(pow(tabCoordBallon[i][0]-mouse_x,2)+pow(tabCoordBallon[i][1]-mouse_y, 2))) )
                {
                    tabCoordBallon[i][0]=0;
                    tabCoordBallon[i][1]=0;
                }
            }








            blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        }

        playerTemps[pTurn]=  elapsed_time;//on stocke le temps dans le tableau des scores
        ///FIN DU JEU
    }

    start_time = time(NULL);  // Temps de départ
    elapsed_time = 0;         // Temps écoulé
    time_t duration = 3;


    //résultat du jeu
    if(playerTemps[0]>playerTemps[1])//si le joueur 1 a un plus haut temps que le joueur 2
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
    else if(playerTemps[0]<playerTemps[1])//si le joueur 1 a un temps plus bas que le joueur 2
    {
        //victoire joueur 1

        partie.playerBase[0].tickets++;
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
        if(partie.playerBase[i].score[0]>playerTemps[i]||partie.playerBase[i].score[0]==-1)
        {
            partie.playerBase[i].score[0]=playerTemps[i];
        }
        //on check si le score que le joueur viens de faire est le meilleur score de tout les temps
        if(partie.playerBase[i].score[0]<partie.tabHighScore[0])
        {
            partie.tabHighScore[0]=partie.playerBase[i].score[0];
        }
    }


    //on échange l'ordre de jeu des 2 joueurs
    t_joueur jtemp;//structure de stockage temporaire

    jtemp=partie.playerBase[0];
    partie.playerBase[0]=partie.playerBase[1];
    partie.playerBase[1]=jtemp;





    return partie;
}



void creerCoordBallons(int tabCoordBallon[10][2])
{
    for (int i = 0; i < 10; i++) //on boucle pour tout les ballons
    {
        do //on génère des coord aléatoire sur l'espace disponible
        {
            tabCoordBallon[i][0] = rand() % (SCREEN_W - 2 * (55 + 20)) + 55 + 20;
            tabCoordBallon[i][1] = rand() % (SCREEN_H - (55 + 20)-(55+200)) + 55 + 20;
        }
        while (collision(tabCoordBallon, i)); //on reccomence l'aléatoire si il y a des collision entre les ballons
    }
}



int collision(int tabCoordBallon[10][2], int nbGen)
{
    for (int i = 0; i < nbGen; i++) //on regarde pour tout les ballons déjà créés
    {
        int distance = sqrt(pow(tabCoordBallon[i][0] - tabCoordBallon[nbGen][0], 2) + pow(tabCoordBallon[i][1]- tabCoordBallon[nbGen][1], 2)); //on check si le ballon qui est généré est en collision avec les anciens ballons
        if (distance < 2 * 55) //si oui, on renvoie 1
        {
            return 1;
        }
    }
    return 0; //sinon 0
}



void loadStartGame(t_banquePolice police, t_partie partie, int game, int actualP)
{
    //barre de chargement cutsom en fonction du joueur qui va jouer et du minijeu
    //reprendre le concept de la barre de chargement

    //afficher à %name% de jouer! avec l'ui du jeu en question (screenshot chargé en fond)

    BITMAP * buffer =create_bitmap(SCREEN_W,SCREEN_H);//creation du bitmap buffer

    time_t start_time = time(NULL);  // Temps de départ
    time_t elapsed_time = 0;         // Temps écoulé

    time_t duration = 3;            // Durée d'attente

    time_t remaining_time;

    while(elapsed_time < duration)
    {

        remaining_time = duration - elapsed_time;
        elapsed_time = time(NULL) - start_time;

        clear_bitmap(buffer);

        switch(game)//on affiche les interfaces des jeux respectifs en fond
        {
        case 1: //tir au ballon
            rectfill(buffer, 0,0, SCREEN_W, SCREEN_H, makecol(203,32,38)); //affichage des bords
            rectfill(buffer, 20,20, SCREEN_W-20, SCREEN_H-20, makecol(114,95,79)); //affichage du fond

            rectfill(buffer, 0,SCREEN_H-200, SCREEN_W, SCREEN_H, makecol(0,0,0));
            rectfill(buffer, 0+10,SCREEN_H-200+10, SCREEN_W-10, SCREEN_H-10, makecol(136,140,141)); //affichage bordereau d'information


            rectfill(buffer, 486-3, SCREEN_H-200, 486+3,  SCREEN_H, makecol(0,0,0)); //délimitation des cases
            rectfill(buffer, 972-3, SCREEN_H-200, 972+3,  SCREEN_H, makecol(0,0,0));
            break;

        case 2 :
            break;

        case 3:
            break;
        }


        rectfill(buffer,SCREEN_W/4-20,SCREEN_H/4-20, SCREEN_W*3/4+20,SCREEN_H*3/4+20, makecol(0,0,0));
        rectfill(buffer,SCREEN_W/4,SCREEN_H/4, SCREEN_W*3/4,SCREEN_H*3/4, makecol(100,100,100)); // affichage de l'encarte

        textprintf_centre_ex(buffer, police.Hudson_26, SCREEN_W/2, SCREEN_H/2-150, makecol(255,255,255), -1, "A %s de jouer!", partie.playerBase[actualP].name);
        textprintf_centre_ex(buffer, police.Hudson_26, SCREEN_W/2, SCREEN_H/2+50, makecol(255,255,255), -1, "%ld", (long  int)remaining_time);

        blit(buffer, screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }





    //faire un switch case pour l'image à charger: 1=tir au ballon 2=taupe là 3= paris hippiques


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

