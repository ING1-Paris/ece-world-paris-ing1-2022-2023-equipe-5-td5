#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>

typedef struct t_serpent
{
    int CoordX[100];
    int CoordY[100];
    int CodeColor[100];
    int Longueur;
    int Direction; //1=haut 2=droite 3=bas 4=gauche
    int Statut; //0=Ok 1=Mur 2=Serpent 3=Cado
} t_serpent;

typedef struct t_cado
{
    int CoordX;
    int CoordY;
    int CodeColor;
} t_cado;

//========================================================================


//Génération d’un nouveau cadeau à manger
t_cado nouveau_cado(t_serpent Serpent1,t_serpent Serpent2) {

    t_cado Cado;
    int i;
    int CadoOk = 0;

    do {
        Cado.CoordX = rand() % 80;
        Cado.CoordY = rand() % 50;
        Cado.CodeColor = makecol(50+rand() % 200, 50+rand() % 200, 50+rand() % 200);
        CadoOk = 0;
        //Test si si le bonus n'est pas sur le serpent ?
        for (i = 0; i < Serpent1.Longueur; i++) {
            if (Serpent1.CoordX[i] == Cado.CoordX && Serpent1.CoordY[i] == Cado.CoordY)
                CadoOk = 1;
        }
        for (i = 0; i < Serpent2.Longueur; i++) {
            if (Serpent2.CoordX[i] == Cado.CoordX && Serpent2.CoordY[i] == Cado.CoordY)
                CadoOk = 1;
        }
    } while (CadoOk == 1);
    //allegro_message("nouveau_cado : (%d,%d)",Cado.CoordX,Cado.CoordY);
    return Cado;
}



//Fait avancer serpent (retourne 0 si perdu, 1 sinon)
t_serpent fait_avancer_serpent(t_serpent Serpent,t_serpent SerpentAdverse,t_cado Cado) {

    int i;
    int posX_suivante=0,posY_suivante=0;

    //Détermination de la nouvelle case, en fonction de la valeur de “Direction” et des coordonnées du dernier maillon du serpent
    //1=haut 2=droite 3=bas 4=gauche

    //allegro_message("FCT : Direction : %d - Premier/Dernier Maillon : (%d,%d)/(%d,%d)",Serpent.Direction,Serpent.CoordX[0],Serpent.CoordY[0],Serpent.CoordX[Serpent.Longueur-1],Serpent.CoordY[Serpent.Longueur-1]);
    switch (Serpent.Direction)
    {
        case 1:
            posX_suivante = Serpent.CoordX[Serpent.Longueur-1];
            posY_suivante = Serpent.CoordY[Serpent.Longueur-1]-1;
            break;
        case 2:
            posX_suivante = Serpent.CoordX[Serpent.Longueur-1]+1;
            posY_suivante = Serpent.CoordY[Serpent.Longueur-1];
            break;
        case 3:
            posX_suivante = Serpent.CoordX[Serpent.Longueur-1];
            posY_suivante = Serpent.CoordY[Serpent.Longueur-1]+1;
            break;
        case 4:
            posX_suivante = Serpent.CoordX[Serpent.Longueur-1]-1;
            posY_suivante = Serpent.CoordY[Serpent.Longueur-1];
            break;
        default:
            break;
    }


    //allegro_message("Case suivante : %d,%d",posX_suivante,posY_suivante);

    //Test des limites de terrain (0-80 et 0-50)
    if (posX_suivante<0 || posX_suivante>=80 || posY_suivante<0|| posY_suivante>=50)
    {
        //Serpent heurte le mur ! Hors limite !
        //allegro_message("Dans le mur !");
        Serpent.Statut = 1;
        return Serpent;
    }

    //Test si pas sur serpent !
    int PasSerpent=0;
    for (i = 0; i < Serpent.Longueur; i++) {
        if (Serpent.CoordX[i] == posX_suivante && Serpent.CoordY[i] == posY_suivante)
            PasSerpent = 1;
    }
    for (i = 0; i < SerpentAdverse.Longueur; i++) {
        if (SerpentAdverse.CoordX[i] == posX_suivante && SerpentAdverse.CoordY[i] == posY_suivante)
            PasSerpent = 1;
    }
    if (PasSerpent==1)
    {
        //Serpent heurte le serpent !!!
        //Beep(800, 1000); // 1000 Hz, 500 ms
        //rest(1000);
        //allegro_message("On se mort la queue ?");
        Serpent.Statut = 2;
        return Serpent;
    }

    //Suis-je sur une CaseCado ?
    //allegro_message("Case suivante : %d,%d / Cado %d,%d",posX_suivante,posY_suivante,Cado.CoordX,Cado.CoordY);

    if (posX_suivante==Cado.CoordX && posY_suivante==Cado.CoordY)
    {
        //je suis sur un cadeau !!! Tou-li !
        //Beep(400, 500);
        //Beep(1000, 500);
        //allegro_message("Bravo !");

        //rest(1000);

        //On ajoute le Bonus dans le serpent
        Serpent.Longueur++;
        Serpent.CoordX[Serpent.Longueur-1]=Cado.CoordX;
        Serpent.CoordY[Serpent.Longueur-1]=Cado.CoordY;
        Serpent.CodeColor[Serpent.Longueur-1]=Cado.CodeColor;
        Serpent.Statut = 3;
        return Serpent;
    }
    //allegro_message("On avance : %d,%d",posX_suivante,posY_suivante);
    // Si on est là, c'est qu'on avance normalement
    //on décalle toutes les positions (on perd la première)

    //on efface le premier maillon
    rectfill(screen,10+10*Serpent.CoordX[0], 10+10*Serpent.CoordY[0], 20+(10*Serpent.CoordX[0]), 20+(10*Serpent.CoordY[0]), makecol(0, 0, 0));

    for (i = 0; i < Serpent.Longueur-1; i++)
    {
        Serpent.CoordX[i] = Serpent.CoordX[i+1];
        Serpent.CoordY[i] = Serpent.CoordY[i+1];
        //Serpent.CodeColor[i] = Serpent.CodeColor[i+1];
    }
    Serpent.CoordX[Serpent.Longueur-1]=posX_suivante;
    Serpent.CoordY[Serpent.Longueur-1]=posY_suivante;
    //Serpent.CodeColor[Serpent.Longueur-1]=Serpent.CodeColor[Serpent.Longueur-2];
    //on avance "gravement" (300Hz)
    //Beep(300, 500);
    rest(50);
    //allegro_message("Direction : %d - Dernier Maillon : (%d,%d)",Serpent.Direction,Serpent.CoordX[Serpent.Longueur-1],Serpent.CoordY[Serpent.Longueur-1]);
    Serpent.Statut = 0;
    return Serpent;
}

//========================================================================
int main() {
    allegro_init();
    srand(time(NULL));
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 820,520, 0, 0);
    //page noire
    BITMAP *buffer;
    buffer = create_bitmap(SCREEN_W,SCREEN_H);
    //vérifie si les bitmaps ont été créés
    if (!buffer){
        allegro_message("impossible de trouver buffer");
        exit(EXIT_FAILURE);
    }


    t_serpent Serpent1,Serpent2;
    t_cado Cadeau;
    int Perdu=0;
    int PosX_max = 80;
    int PosY_max = 50;
    int i;

    //initialisation serpent
    Serpent1.Longueur=4;
    Serpent1.CoordX[0]=25;
    Serpent1.CoordX[1]=26;
    Serpent1.CoordX[2]=27;
    Serpent1.CoordX[3]=28;
    Serpent1.CoordY[0]=25;
    Serpent1.CoordY[1]=25;
    Serpent1.CoordY[2]=25;
    Serpent1.CoordY[3]=25;
    Serpent1.CodeColor[0]=makecol(150, 250, 150);
    Serpent1.CodeColor[1]=makecol(150, 250, 150);
    Serpent1.CodeColor[2]=makecol(150, 250, 150);
    Serpent1.CodeColor[3]=makecol(150, 250, 150);
    Serpent1.Direction=2;
    Serpent1.Statut=0;

    Serpent2.Longueur=4;
    Serpent2.CoordX[0]=25;
    Serpent2.CoordX[1]=26;
    Serpent2.CoordX[2]=27;
    Serpent2.CoordX[3]=28;
    Serpent2.CoordY[0]=10;
    Serpent2.CoordY[1]=10;
    Serpent2.CoordY[2]=10;
    Serpent2.CoordY[3]=10;
    Serpent2.CodeColor[0]=makecol(250, 150, 150);
    Serpent2.CodeColor[1]=makecol(250, 150, 150);
    Serpent2.CodeColor[2]=makecol(250, 150, 150);
    Serpent2.CodeColor[3]=makecol(250, 150, 150);
    Serpent2.Direction=2;
    Serpent2.Statut=0;


    Cadeau = nouveau_cado(Serpent1,Serpent2);
    //allegro_message("Cadeau: (%d,%d)",Cadeau.CoordX,Cadeau.CoordY);
    //circlefill(screen,10+(10*Cadeau.CoordX)+5,10+(10*Cadeau.CoordY)+5,5,Cadeau.CodeColor);

    //boucle
    while (!key[KEY_ESC]){

        //clear_bitmap(buffer);
        //clear_bitmap(screen);

        //1=haut 2=droite 3=bas 4=gauche
        if (key[KEY_UP])
            Serpent1.Direction=1;
        if (key[KEY_RIGHT])
            Serpent1.Direction=2;
        if (key[KEY_DOWN])
            Serpent1.Direction=3;
        if (key[KEY_LEFT])
            Serpent1.Direction=4;


        //1=haut 2=droite 3=bas 4=gauche
        if (key[KEY_W])
            Serpent2.Direction=1;
        if (key[KEY_D])
            Serpent2.Direction=2;
        if (key[KEY_S])
            Serpent2.Direction=3;
        if (key[KEY_A])
            Serpent2.Direction=4;


        //Grille de "positions" 80 x 50 (10 pixels de large pour le serpent)
        //Equivalence en pixels :
        // Grille de jeux (10,10) à (810,510)
        // Ecran (0,0) à (820,520)
        rect(screen,10, 10, 10+(PosX_max*10), 10+(PosY_max*10), makecol(250, 250, 250));

        //affichage du serpent1
        for (i=0;i<Serpent1.Longueur;i++)
        {
            rectfill(screen,10+10*Serpent1.CoordX[i], 10+10*Serpent1.CoordY[i], 20+(10*Serpent1.CoordX[i]), 20+(10*Serpent1.CoordY[i]), Serpent1.CodeColor[i]);
            //allegro_message("Wagon %d (%d,%d) color=%d",i,Serpent1.CoordX[i],Serpent1.CoordY[i],Serpent1.CodeColor[i]) ;
        }
        //affichage du serpent2
        for (i=0;i<Serpent2.Longueur;i++)
        {
            rectfill(screen,10+10*Serpent2.CoordX[i], 10+10*Serpent2.CoordY[i], 20+(10*Serpent2.CoordX[i]), 20+(10*Serpent2.CoordY[i]), Serpent2.CodeColor[i]);
            //allegro_message("Wagon %d (%d,%d) color=%d",i,Serpent1.CoordX[i],Serpent1.CoordY[i],Serpent1.CodeColor[i]) ;
        }


        //Affichage Cado
        circlefill(screen,10+(10*Cadeau.CoordX)+5,10+(10*Cadeau.CoordY)+5,5,Cadeau.CodeColor);

        //Fait avancer le serpent 1
        Serpent1 = fait_avancer_serpent(Serpent1,Serpent2,Cadeau);
        if (Serpent1.Statut==3)
        {
            //Nouveau Cado !
            Cadeau=nouveau_cado(Serpent1,Serpent2);
            Serpent1.Statut=0;
        }
        //Fait avancer le serpent 2
        Serpent2 = fait_avancer_serpent(Serpent2,Serpent1,Cadeau);
        if (Serpent2.Statut==3)
        {
            //Nouveau Cado !
            Cadeau=nouveau_cado(Serpent1,Serpent2);
            Serpent2.Statut=0;
        }

        if (Perdu==1)
            allegro_message("Perdu !");

    }

    destroy_bitmap(buffer);
    remove_keyboard();
    allegro_exit();
    return 0;
}
END_OF_MAIN();
