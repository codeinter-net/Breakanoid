// BREAKANOID
// Version ORIC ATMOS
//
//	Par Pascal Barlier
// Début le 11/04/2003

// x Faire 30 niveaux ou plus
// x Extra ball tous les 10000 points
// ~ Sonorisation
// x Régler au pixel la détection des briques
// x Dans certains cas les bonus ne descendent plus
// ~ Finir de distribuer les bonus dans les niveaux
// - Changer l'animation d'intro
// X Permettre un accès direct aux tableaux
// x Balle plus rapide quand on touche une brique à 10 points ou plus
// x Gérer le hi-score
// x Supprimer le bug qui affiche des '@' parasites dans la zone de texte
// x Ajouter les bonus:
//   x 1  Raquette plus large
//   x 2  Raquette moins large (+100 bonus points)
//   x 3  Balle plus rapide (+100 bonus points)
//   x 4  Balle moins rapide
//   x 5  Balle troueuse
//   x 6  Extra ball
//   x 7  100 Bonus points
//   x 8  200 Bonus points
//   x 9  300 Bonus points
//   x 10 Fin de tableau
//   x 11 Début de tableau

#include <..\include\stdlib.h>
#include <..\include\lib.h>
#include <..\include\defs.h>
#include "bonus.h"
#include "levels.h"

// ---------- PROTOTYPES ----------

void nobrick(char x,char y);
void player();

// ---------- VARIABLES GLOBALES ----------

char demo;			// Mode démo
char width;			// Largeur de la raquette (partie centrale seulement)
char bgcolor;		// Couleur de fond
char fwcolor;		// Inverse de la couleur de fond
char nbr;			// Nombre de briques
char speed;			// Vitesse de la balle
char topspeed;		// Indique si l'on a déjà touché le haut
char turbo;			// Balle perforante;
byte x;				// Position de la raquette
byte xx;				// Position fine
char Bonus;			// Bonus à affecter
char BonusCount;	// Compteur de bonus
char BonusPhase;	// Phase de l'animation des bonus
char BonusX;
char BonusPos;
char *BonusPtr;
char BonusList[17];

char TextKey[8];
char KeyLen;

char SoundNum1,SoundNum2,SoundNum3;
char SoundIndex1,SoundIndex2,SoundIndex3;
char SoundCount;	// Compteur pour ralentir la lecture des sons

char score[5]={0,0,0,0,0};	// Score
char high[5]={0,0,0,0,0};	// High score
char level=0;		// Tableau de jeu
char lives=0;		// Nombre de vies

char bg1[8]={0x41,0,0,0,0,0,0,0};			// Fond d'écran
char bg2[8]={0,0,0x50,0,0x44,0,0x50,0};

char curlevel[16][8];	// Copie du tableau en cours

// ---------- GRAPHISMES ET TEXTES ----------

char paddle[6][9][3]={{
{0x4F,0x7E,0x40},{0x5D,0x57,0x40},{0x5F,0x7F,0x40},{0x7D,0x57,0x60},{0x7F,0x7F,0x60},
{0x7D,0x57,0x60},{0x5F,0x7F,0x40},{0x5D,0x57,0x40},{0x4F,0x7E,0x40}
},{
{0x47,0x7F,0x00},{0x4E,0x6B,0x60},{0x4F,0x7F,0x60},{0x5E,0x6B,0x70},{0x5F,0x7F,0x70},
{0x5E,0x6B,0x70},{0x4F,0x7F,0x60},{0x4E,0x6B,0x60},{0x47,0x7F,0x00}
},{
{0x43,0x7F,0x60},{0x47,0x55,0x70},{0x47,0x7F,0x70},{0x4F,0x55,0x78},{0x4F,0x7F,0x78},
{0x4F,0x55,0x78},{0x47,0x7F,0x70},{0x47,0x55,0x70},{0x43,0x7F,0x60}
},{
{0x41,0x7F,0x70},{0x43,0x6A,0x78},{0x43,0x7F,0x78},{0x47,0x6A,0x7C},{0x47,0x7F,0x7C},
{0x47,0x6A,0x7C},{0x43,0x7F,0x78},{0x43,0x6A,0x78},{0x41,0x7F,0x70}
},{
{0x40,0x7F,0x78},{0x41,0x75,0x5C},{0x41,0x7F,0x7C},{0x43,0x75,0x5E},{0x43,0x7F,0x7E},
{0x43,0x75,0x5E},{0x41,0x7F,0x7C},{0x41,0x75,0x5C},{0x40,0x7F,0x78}
},{
{0x40,0x5F,0x7C},{0x40,0x7A,0x6E},{0x40,0x7F,0x7E},{0x41,0x7A,0x6F},{0x41,0x7F,0x7F},
{0x41,0x7A,0x6F},{0x40,0x7F,0x7E},{0x40,0x7A,0x6E},{0x40,0x5F,0x7C}
}};

char ball[6][5][2]={
//{{0x5C,0x00},{0x7E,0x00},{0x7E,0x00},{0x7E,0x00},{0x5C,0x00}},
{{0x4E,0x00},{0x5F,0x00},{0x5F,0x00},{0x5F,0x00},{0x4E,0x00}},
{{0x47,0x00},{0x4F,0x60},{0x4F,0x60},{0x4F,0x60},{0x47,0x00}},
{{0x43,0x60},{0x47,0x70},{0x47,0x70},{0x47,0x70},{0x43,0x60}},
{{0x41,0x70},{0x43,0x78},{0x43,0x78},{0x43,0x78},{0x41,0x70}},
{{0x40,0x78},{0x41,0x7C},{0x41,0x7C},{0x41,0x7C},{0x40,0x78}},
{{0x40,0x5C},{0x40,0x7E},{0x40,0x7E},{0x40,0x7E},{0x40,0x5C}}
};

byte dys[9]={-1,-2,-3,-4,-4,-4,-3,-2,-1};	// Trajectoires en Y en fonction de X

char buffer[25];	// Buffer pour la copie de l'écran sous la balle

char filler[2][9]={	// Motifs pour remplir la raquette
{0x7F,0x55,0x7F,0x55,0x7F,0x55,0x7F,0x55,0x7F},
{0x7F,0x6A,0x7F,0x6A,0x7F,0x6A,0x7F,0x6A,0x7F}
};

char charset[40]={
0x10,0x28,0x15,0xA,0,0,0,0,
0,0,0x15,0x2A,0,0,0,0,
1,2,0x15,0x2A,0,0,0,0,
0,0,0,0,0,0,0,0,
8,8,0x1C,0x3E,0x1c,8,8,0};

char info[41]={16,' ',7,'_',2,'B','A','L','L',' ','0',
7,'_',1,'S','C','O','R','E',' ','0','0','0','0','0',
7,'_',3,'H','I',' ','0','0','0','0','0',7,'_',' ',' ',0};

char info1[41]={1,19,'B',4,16,18,'R',6,16,
17,'E',3,16,21,'A',2,16,20,'K',5,16,22,'A',3,16,21,'N',6,16,
17,'O',4,16,18,'I',1,16,19,'D',' ',0};

char info2[41]={16,' ',' ',' ',' ',7,'_',3,'(','c',')',' ','0','4','/','2','0','0','3',' ',
'P','a','s','c','a','l',' ','B','a','r','l','i','e','r',7,'_',' ',' ',' ',' ',0};

char info3[41]={16,' ',' ',' ',' ',' ',' ',' ',7,'_',1,'<','R','E','T','U','R','N','>',
7,'=',3,'S','t','a','r','t',' ','G','a','m','e',7,'_',' ',' ',' ',' ',' ',' ',0};

char info9[41]={16,' ',' ',' ',' ',' ',' ',' ',7,'_',3,'L','E','V','E','L',' ','0','0',' ',
7,'_',1,'1','2','3','4','5','6','7','8',7,'_',' ',' ',' ',' ',' ',' ',' ',0};

int sounds[5][5]={
{1200,0,0,0,0},
{1000,0,0,0,0},
{96,92,80,60,0},
{96,92,200,500,0},
{1000,0,0,0,0},
};

// ---------- DEBUT DU CODE ----------

// Pour faire une tempo

int wait(int timing)
{
int t,z;

	for(t=0;t<=timing;t++)
		z<<=1;
	return(z);
}

void InitSound()
{
	*(int*)(0x2e1)=1;
	*(int*)(0x2e3)=0;
	*(int*)(0x2e5)=0;
	call(0xfb40);
	*(int*)(0x2e1)=2;
	call(0xfb40);
	*(int*)(0x2e1)=3;
	call(0xfb40);
	*(int*)(0x2e1)=0;
	call(0xfb40);
	*(int*)(0x2e1)=7;
	*(int*)(0x2e3)=0;
	*(int*)(0x2e5)=0;
	*(int*)(0x2e7)=0 ;
	call(0xfbd0);
}

void Sound(int s,char v)
{
	*(int*)(0x2e1)=1;
	*(int*)(0x2e3)=s;
	*(int*)(0x2e5)=v;
	call(0xfb40);
}

// Affiche un texte dans la ligne réservée pour

void displaytext(char *txt)
{
static char	*ptr;

	ptr=(char*)TEXTVRAM+(25*40)+40;
	while(*txt) *ptr++=*txt++;
}

void DisplayLevelName(void)
{
int	i;

	info9[17]=((level+1)/10)|'0';
	info9[18]=((level+1)%10)|'0';

	for(i=0;i<8;i++)
		if((info9[23+i]=KeyLevels[i][level])==' ') break;
	for(;i<8;i++)
		info9[23+i]=' ';

	displaytext(info9);
}

// Affichage du score

void DisplayScore(char full)
{
static byte z;
static char	*ptr;

	if(full)
	{
		displaytext(info);
		ptr=(char*)TEXTVRAM+(25*40)+50;
		if(lives<10)
			*ptr=lives|'0';
		else
			*ptr='+';
		ptr=(char*)TEXTVRAM+(25*40)+71;
		for(z=5;--z>=0;)
			*ptr++=high[z]|'0';
	}
	ptr=(char*)TEXTVRAM+(25*40)+60;
	for(z=5;--z>=0;)
		*ptr++=score[z]|'0';
}

// Gestion du hi-score

void hiscore(void)
{
static byte z;

	for(z=5;--z>=0;)
		if(score[z]>high[z])
		{
			high[0]=score[0];
			high[1]=score[1];
			high[2]=score[2];
			high[3]=score[3];
			high[4]=score[4];
			return;
		}
		else if(score[z]<high[z])
			return;
}

// Affichage du fond de l'écran de jeu

void background()
{
static byte	x,y,z,color,inv;
static char	s1,s2;
static char	*ptr,*src;
static char	*pbg1,*pbg2;

	ptr=(char*)(VIDEO+4);
	inv=0x10;
	for(y=25;--y>=0;)
	{
		pbg1=bg1+8;
		pbg2=bg2+8;
		for(z=8;--z>=0;)
		{
			if(y<2)	// Zone de la raquette
			{
				s1=s2=0x40;
				if((y==0)&&(z<7)) color=A_FWBLACK; //A_FWWHITE;
				else if(((y==1)&&(z==7))||((y==0)&&(z==7))) color=levels[level][15][1];
				else color=(z&1)?levels[level][15][2]:levels[level][15][3];
				*++ptr=color;
			}
			else if(y<24)	// partie centrale
			{
				s1=*--pbg1;
				if(!s1) s1=fwcolor;
				s2=*--pbg2;
				if(!s2) s2=fwcolor;
				*++ptr=fwcolor;
			}
			else		// Barre du haut
			{
				s1=s2=0xD5;
				*++ptr=fwcolor;
			}
			*++ptr=bgcolor|inv;
			for(x=14;--x>=0;)
			{
				*++ptr=s1;
				*++ptr=s2;
			}
			*++ptr=bgcolor|inv;
			*++ptr=A_BGBLACK;
			*++ptr=A_FWWHITE;
			ptr+=7;
			inv^=0x80;
		}
	}
	src=charset;
	ptr=(char*)STDCHRHIRES+8*91;
	for(z=sizeof(charset);--z>=0;)
		*ptr++=*src++;

	ptr=(char*)TEXTVRAM+(25*40);
	ptr[0]=A_BGBLACK;
	ptr[1]=A_FWWHITE;
	ptr[40]=A_BGBLACK;
	ptr[41]=A_FWWHITE;
	ptr[80]=A_BGBLACK;
	ptr[81]=A_FWWHITE;
}

// Affichage du tableau de briques

void bricks()
{
static byte	x,y,z,color;
static char	*base,*ptr;

	nbr=0;
	base=(char*)VIDEO+966;
	for(x=7;--x>=0;)
		for(y=15;--y>=0;)
		{
			color=levels[level][y][x];
			curlevel[y][x]=color;
			ptr=base+x*4+y*320;
			if(color==16)			// Vide
				for(z=8;--z>=1;)
				{
					*++ptr=bgcolor|0x10;
					*++ptr=bg2[z];
					*++ptr=bgcolor|0x10;
					*++ptr=bg2[z];
					ptr+=36;
				}
			else						// Brique
			{
				if(color&7)
					++nbr;
				if(color&8) color=7;
				for(z=7;--z>=0;)
				{
					*++ptr=0x97-color;
					*++ptr=fwcolor|0x80;
					*++ptr=0xC0;
					*++ptr=0xC1;
					ptr+=36;
				}
			}
		}
	for(y=16;--y>=0;)
		BonusList[y]=levels[level][y][7];
	BonusList[16]=0;
}

// Affichage et gestion des bonus

void DisplayBonus(void)
{
static byte	ex,ey,z;
static char color;
static char	*src,*dst;

	if(!BonusPhase)
		return;
	switch(BonusPhase)
	{
	case 1:	// Dessin du bonus
		dst=(char*)(VIDEO+36);
		src=BonusIndex[Bonus-1]-1;
		color=BonusColor[Bonus-1];
		for(z=20;--z>=0;)
		{
			*++dst=color;
			*++dst=*++src;
			*++dst=*++src;
			dst+=37;
		}
		BonusPos=90;
		++BonusPhase;
		dst=(char*)(VIDEO+879);
		break;
	case 2:
		dst[0]=0x60;
		dst[39]=0x41;
		dst+=80;
		if(!--BonusPos)
			++BonusPhase;
		break;
	case 3:
		*--dst=']';
		BonusPos=29-BonusX;
		++BonusPhase;
		break;
	case 4:
		if(--BonusPos)
			*--dst='\\';
		else
			++BonusPhase;
		break;
	case 5:
		*--dst='[';
		++BonusPhase;
		break;
	case 6:
		dst-=202;
		dst[0]=0x6a;dst[1]=0x6a;dst[2]=0x6a;dst[3]=0x6a;
		dst[40]=0x55;dst[41]=0x55;dst[42]=0x55;dst[43]=0x55;
		dst[80]=0x6a;dst[81]=0x6a;dst[82]=0x6a;dst[83]=0x6a;
		dst[120]=0x55;dst[121]=0x55;dst[122]=0x55;dst[123]=0x55;
		++BonusPhase;
		BonusPos=30;	// Durée de présence du bonus (30)
		break;
	case 7:
		if((x<=BonusX+3)&&(x+width+2>=BonusX))
			BonusPhase=20;
		else if(!--BonusPos)
			++BonusPhase;
		break;
	case 8:
		dst[0]=0x40;dst[1]=0x40;dst[2]=0x40;dst[3]=0x40;
		dst[40]=0x40;dst[41]=0x40;dst[42]=0x40;dst[43]=0x40;
		dst[80]=0x40;dst[81]=0x40;dst[82]=0x40;dst[83]=0x40;
		dst[120]=0x40;dst[121]=0x40;dst[122]=0x40;dst[123]=0x40;
		++BonusPhase;
		dst+=201;
		BonusPos=31-BonusX;
		break;
	case 9:
		if(--BonusPos)
			*++dst=' ';
		else
		{
			++BonusPhase;
			BonusPos=45;
			++dst;
		}
		break;
	case 10:
		dst-=160;
		dst[0]=0x40;
		dst[39]=0x40;
		dst[80]=0x40;
		dst[119]=0x40;
		if(!--BonusPos)
			++BonusPhase;
		break;
	case 11:
		dst=(char*)(VIDEO+37);
		for(z=20;--z>=0;)
		{
			*++dst=0x40;
			*++dst=0x40;
			dst+=38;
		}
		BonusPhase++;
		break;
	case 12:
		BonusPhase=0;
		break;
	case 20:
		dst=(char*)(VIDEO+37);
		for(z=20;--z>=0;)
		{
			*++dst=0x40;
			*++dst=0x40;
			dst+=38;
		}
		BonusPos=30;
		++BonusPhase;
		dst=(char*)(VIDEO+879);
		break;
	case 21:
		dst[0]=0x40;
		dst[39]=0x40;
		dst[80]=0x40;
		dst[119]=0x40;
		dst[160]=0x40;
		dst[199]=0x40;
		dst+=240;
		if(!--BonusPos)
		{
			++BonusPhase;
			BonusPos=31-BonusX;
		}
		break;
	case 22:
		if(--BonusPos)
		{
			*--dst=' ';
			if(--BonusPos)
			{
				*--dst=' ';
				if(--BonusPos)
					*--dst=' ';
			}
		}
		if(!BonusPos)
			++BonusPhase;
		break;
	case 23:
		dst-=202;
		dst[0]=0x40;dst[1]=0x40;dst[2]=0x40;dst[3]=0x40;
		dst[40]=0x40;dst[41]=0x40;dst[42]=0x40;dst[43]=0x40;
		dst[80]=0x40;dst[81]=0x40;dst[82]=0x40;dst[83]=0x40;
		dst[120]=0x40;dst[121]=0x40;dst[122]=0x40;dst[123]=0x40;
		++BonusPhase;
		break;
	case 24:
		switch(Bonus)
		{
		case 1:	// Plus large
			if(width<8)
				++width;
			if(x+width>15)
				--x;
			player();
			break;
		case 2:	// Moins large
			if(width>0)
				--width;
			if(x+width>15)
				++x;
			player();
			goto _bonus100_;
		case 3:	// Plus vite
			if(speed>1)
				--speed;
			goto _bonus100_;
		case 4:	// Moins vite
			++speed;
			break;
		case 5:	// Perforante
			turbo=true;
			break;
		case 6:	// Extra balle
			lives++;
			DisplayScore(1);
			break;
		case 9:	// 300 points
			score[2]++;
		case 8:	// 200 points
			score[2]++;
		case 7:	// 100 points
_bonus100_:
			score[2]++;
			while(score[2]>9) {score[2]-=10;score[3]++;}
			while(score[3]>9) {score[3]-=10;score[4]++;lives++;DisplayScore(1);}	// Extra ball tous les 10000 points
			DisplayScore(0);
			break;
		case 10:	// Fin de tableau
			for(ey=15;--ey>=0;)
				for(ex=7;--ex>=0;)
				{
					while(curlevel[ey][ex]&&(curlevel[ey][ex]<16))
					{
						nobrick(ex,ey);
						wait(100);
					}
				}
			break;
		case 11:	// Début de tableau
			level--;
			nbr=0;
			break;
		}
		BonusPhase++;
		break;
	case 25:
		BonusPhase=0;
		break;
	}
}

// Supprime les briques, augmente le score et lance les bonus

void nobrick(char x,char y)
{
static byte	z,color;
static char	*ptr;

	if(!topspeed&&(y<2))
	{
		topspeed=true;
		if(speed>2) --speed;
	}
	color=curlevel[y][x];
	ptr=(char*)VIDEO+966+x*4+y*320;
	if(color&8)	// Brique double
	{
		color&=7;
		curlevel[y][x]=color;
		for(z=7;--z>=0;)
		{
			*++ptr=0x97-color;
			*++ptr=fwcolor|0x80;
			*++ptr=0xC0;
			*++ptr=0xC1;
			ptr+=36;
		}
	}
	else			// Brique simple
	{
		if(!color)	// brique dure
			return;
		curlevel[y][x]=16;
		for(z=8;--z>=1;)
		{
			*++ptr=bgcolor|0x10;
			*++ptr=bg2[z];
			*++ptr=bgcolor|0x10;
			*++ptr=bg2[z];
			ptr+=36;
		}
		if(!demo)
		{
			--nbr;
			if(!(nbr&4)&&!BonusPhase&&BonusList[BonusCount])	// Gestion des bonus
			{
				Bonus=BonusList[BonusCount];
				BonusCount++;
				BonusPhase=1;
				BonusX=x<<2;
				DisplayBonus();
			}
		}
	}
	if(!demo)		// Incrémente le score
	{
		score[0]+=15-y;
		while(score[0]>9) {score[0]-=10;score[1]++;}
		while(score[1]>9) {score[1]-=10;score[2]++;}
		while(score[2]>9) {score[2]-=10;score[3]++;}
		while(score[3]>9) {score[3]-=10;score[4]++;lives++;DisplayScore(1);}	// Extra ball tous les 10000 points
		DisplayScore(0);
	}
	SoundNum1=2;SoundIndex1=0;
}

// Affichage de la raquette

void player()
{
static byte	y,z;
static char f,step;
static char	*src,*dst,*fil;
static char	old_x=-1;	// static
static char	old_width=-1;	// static


	if(old_x!=-1)
	{

		if(old_x<x)	// La raquette va vers la droite
		{
			while(old_x<x)
			{
				dst=(char*)(VIDEO+7367)+old_x;
				for(y=9;--y>=0;)
				{
					*dst=0x40;
					dst+=40;
				}
				++old_x;
			}
		}
		else if(old_x+old_width>x+width)		// La raquette va vers la gauche (ou a rétréci)
		{
			while(old_x+old_width>x+width)
			{
				dst=(char*)(VIDEO+7369)+old_x+old_width;
				for(y=9;--y>=0;)
				{
					*dst=0x40;
					dst+=40;
				}
				if(old_x>x) --old_x;
				else --old_width;
			}
		}
	}
	dst=(char*)(VIDEO+7366)+x;
	src=paddle[xx][0]-1;
	fil=filler[xx&1]-1;
	step=37-width;
	if(xx<4)
		for(y=9;--y>=0;)
		{
			*++dst=*++src;
			f=*++fil;
			for(z=width;--z>=0;)
				*++dst=f;
			*++dst=*++src;
			*++dst=*++src;
			dst+=step;
		}
	else
		for(y=9;--y>=0;)
		{
			*++dst=*++src;
			*++dst=*++src;
			f=*++fil;
			for(z=width;--z>=0;)
				*++dst=f;
			*++dst=*++src;
			dst+=step;
		}
	old_x=x;
	old_width=width;
}

// Efface la raquette

void noplayer()
{
static byte	x,y;
char	*dst;

	dst=(char*)(VIDEO+7366);
	for(y=9;--y>=0;)
	{
		for(x=28;--x>=0;)
			*++dst=0x40;
		dst+=12;
	}
}

// Dessine la balle

void drawball(byte x,byte xx,char y,byte mode)
{
static byte	i;
static char	x4;
static char	*src,*dst,*buf,*tmp;
static char	*bak=0;	// static

	src=ball[xx][0];
	tmp=dst=(char*)VIDEO+325+x+y*40;
	buf=buffer;
	x4=x&4;
	if(!bak||(mode==3))
	{
		--dst;
		--buf;
		for(i=5;--i>=0;)
		{
			*++buf=*++dst;
			*++buf=*++dst;
			*++buf=*++dst;
			*++buf=*++dst;
			*++buf=*++dst;
			dst+=35;
		}
	}
	else if(mode==4)
	{
		--buf;
		--bak;
		for(i=5;--i>=0;)
		{
			*++bak=*++buf;
			*++bak=*++buf;
			*++bak=*++buf;
			*++bak=*++buf;
			*++bak=*++buf;
			bak+=35;
		}

	}
	else if((unsigned int)bak<=(unsigned int)dst)	// La balle descend
	{
		src+=10;
		bak+=165;
		dst+=165;
		buf+=25;
		for(i=5;--i>=0;)
		{
			*--bak=*--buf;
			*buf=*--dst;
			if((mode>=0)&&!(*dst&0x80)) *dst=fwcolor;

			*--bak=*--buf;
			*buf=*--dst;
			*dst=*--src;

			*--bak=*--buf;
			*buf=*--dst;
			*dst=*--src;

			*--bak=*--buf;
			*buf=*--dst;

			if(mode)
			{
				if((*dst&0x80)||(dst[-1]&0x80))
					*dst=bgcolor|0x10;
				else
					*dst=A_FWBLACK;
			}

			*--bak=*--buf;
			*buf=*--dst;
			if(!mode) *dst=A_FWBLACK;
			else if((*dst&0x80)&&x4)
				*dst=bgcolor|0x10;

			bak-=35;
			dst-=35;
		}
	}
	else						// La balle monte
	{
		--src;
		--dst;
		--buf;
		--bak;
		for(i=5;--i>=0;)
		{
			*++bak=*++buf;
			*buf=*++dst;
			if(!mode) *dst=A_FWBLACK;
			else if((*dst&0x80)&&x4)
				*dst=bgcolor|0x10;

			*++bak=*++buf;
			*buf=*++dst;
			if(mode)
			{
				if((*dst&0x80)||(dst[-1]&0x80))
					*dst=bgcolor|0x10;
				else
					*dst=A_FWBLACK;
			}

			*++bak=*++buf;
			*buf=*++dst;
			*dst=*++src;

			*++bak=*++buf;
			*buf=*++dst;
			*dst=*++src;

			*++bak=*++buf;
			*buf=*++dst;
			if((mode>=0)&&!(*dst&0x80)) *dst=fwcolor;

			bak+=35;
			dst+=35;
		}
	}
	bak=tmp;
}

// Routine principale du jeu

void play()
{
static byte	bx;
static byte	bxx;
static char	by;
static byte	count,count2,msg;
static byte	dx;
static byte	dy;
static char	d;
static byte	mode;
static char	px,py;
static byte ex,exx;
static char sect;
static char end;
static char sound;
static char touch;
static char c;

	bx=2;
	bxx=0;
	by=171;
	count=0;
	count2=0;
	msg=0;
	end=false;
	if(!demo)
	{
		player();
		dx=2;
		dy=dys[dx];
	}
	else
	{
		noplayer();
		dx=1;
		dy=-1;
	}
	drawball(bx,bxx,by,3);
	while(true)
	{
		if(demo)							// Lecture du cheat code
		{
			asm("jsr $23B");			// Key$
			asm("sta %v",c);
			if(c==13) return;
			if((c>32)&&(c<96)&&(KeyLen<8))
				TextKey[KeyLen++]=c;
			else if((c==' ')||(KeyLen>=8))
				KeyLen=0;
		}
		else
		{
			if(!(char)((d=*(char*)0x20E)&0x20))	// Test du clavier
				switch(*(char*)0x2DF)
				{
				case 137:			// Raquette à droite
					if(end) break;
					xx+=4-(d>>3);
					if(xx>5)
						if(++x==26-width)
							{x=25-width;xx=5;}
						else
							xx-=6;
					break;
				case 136:			// Raquette à gauche
					if(end) break;
					xx-=4-(d>>3);
					if(xx<0)
						if(--x<0)
							{x=0;xx=0;}
						else
							xx+=6;
					break;
				case 141:			// Jouer
					if(demo)
						return;
				default:
				}
			player();
		}
		if(++count>=speed)	// Déplacement de la balle
		{
			count=0;
			bxx+=dx;
			if(bxx>5)
			{
				bxx-=6;
				if(bx==26)
					{dx=-dx;bxx=5-bxx;SoundNum2=1;SoundIndex2=0;}
				else
					bx++;
			}
			else if(bxx<0)
			{
				bxx+=6;
				if(bx==0)
					{dx=-dx;bxx=5-bxx;SoundNum2=1;SoundIndex2=0;}
				else
					bx--;
			}
			if((by>=14)&&(by<134))		// Brique touchée par le côté
			{
				py=(char)((byte)(by-14)>>3);
				if(dx<0)
					px=(char)bx>>2;
				else if(dx>0)
					px=(char)(bx+1)>>2;
				if(dx&&(curlevel[py][px]!=16))
				{
					drawball(bx,bxx,by,4);
					nobrick(px,py);
					drawball(bx,bxx,by,3);
					if(!turbo) dx=-dx;
					bxx+=dx;
					if(bxx>5)
						{bxx-=6;bx++;}
					else if(bxx<0)
						{bxx+=6;bx--;}
				}
			}

			by+=dy;
			if((dy<0)&&(by>=15)&&(by<135))	// Brique touchée par le bas
			{
				touch=false;
				py=(char)((byte)(by-15)>>3);
				px=(char)bx;
				if(bxx>3) px++;
				px=(char)((char)px>>2);
				if(curlevel[py][px]!=16)
				{
					drawball(bx,bxx,by,4);
					nobrick(px,py);
					drawball(bx,bxx,by,3);
					touch=true;
				}
				px=(char)bx;
				if(bxx>0) px++;
				px=(char)((char)px>>2);
				if(curlevel[py][px]!=16)
				{
					drawball(bx,bxx,by,4);
					nobrick(px,py);
					drawball(bx,bxx,by,3);
					touch=true;
				}
				if(touch&&!turbo)					// Rebond de la balle
				{
					dy=-dy;
					py=((char)by-1)&7;
					by+=(char)(7-py)<<1;
				}
			}
			if((dy>0)&&(by>=12)&&(by<132))	// Brique touchée par le haut
			{
				touch=false;
				py=(char)((byte)(by-12)>>3);
				px=(char)bx;
				if(bxx>3) px++;
				px=(char)((char)(px)>>2);
				if(curlevel[py][px]!=16)
				{
					drawball(bx,bxx,by,4);
					nobrick(px,py);
					drawball(bx,bxx,by,3);
					touch=true;
				}
				px=(char)bx;
				if(bxx>0) px++;
				px=(char)((char)(px)>>2);
				if(curlevel[py][px]!=16)
				{
					drawball(bx,bxx,by,4);
					nobrick(px,py);
					drawball(bx,bxx,by,3);
					touch=true;
				}
				if(touch&&!turbo)
				{
					dy=-dy;
					py=((char)by-3)&7;
					by-=py<<1;
				}
			}
			else if(by>240)	// Sommet de l'aire de jeu (en négatif)
			{
				dy=-dy;
				by=-dy;
				SoundNum2=1;SoundIndex2=0;
			}
			else if(by>171)	// Bas de l'aire de jeu
			{
				if(demo)
				{
					dy=-dy;
					by=171-by;
					by+=171;
				}
				else if(end)	// Fin de jeu
				{
					if(by>182)
					{
						drawball(bx,bxx,by,4);
						return;
					}
				}
				else				// Calcul du rebond sur la raquette
				{
					ex=bx-x+1;
					exx=bxx-xx;
					if(exx<0)
						{exx+=6;ex-=1;}
					if(exx>5)
						{exx-=6;ex+=1;}

					if(((ex>0)&&(ex<width+2))||(!ex&&(exx>3))||((ex==width+2)&&(exx<3)))
					{
						by=171-by;
						by+=171;
						sect=(char)(((char)(ex<<2)+(char)(ex<<1)+exx-4)*5)/((char)(width<<2)+(char)(width<<1)+7);
						dx+=sect+2;
						if(dx<0) dx=0;
						else if(dx>8) dx=8;
						dy=dys[dx];
						dx-=4;
						SoundNum2=0;SoundIndex2=0;
					}
					else
					{
//						dy=-dy;		// Pour ne pas perdre la balle
						end=true;
					}
				}
			}

			if(!bx) mode=0;
			else if(bx==26) mode=-1;
			else mode=1;
			drawball(bx,bxx,by,mode);
			if(!nbr) return;
			if(demo)
			{
				if(!++count2)	// Messages affichés pendant la démo
				{
					switch((++msg)&3)
					{
						case 0: DisplayScore(1); break;
						case 1: displaytext(info1); break;
						case 2: displaytext(info2); break;
						case 3: displaytext(info3); break;
					}
				}
			}
		}
		else if(!demo)
		{
			DisplayBonus();
		}
		if(!demo)
		{
			if(!(SoundCount&3))
			{
				if(SoundIndex1<5)
				{
					*(int*)(0x2e1)=1;
					*(int*)(0x2e3)=sound=sounds[SoundNum1][SoundIndex1];
					*(int*)(0x2e5)=sound?10:0;
					call(0xfb40);
					++SoundIndex1;
				}
				if(SoundIndex2<5)
				{
					*(int*)(0x2e1)=2;
					*(int*)(0x2e3)=sound=sounds[SoundNum2][SoundIndex2];
					*(int*)(0x2e5)=sound?10:0;
					call(0xfb40);
					++SoundIndex2;
				}
				if(SoundIndex3<5)
				{
					*(int*)(0x2e1)=1;
					*(int*)(0x2e3)=sound=sounds[SoundNum3][SoundIndex3];
					*(int*)(0x2e5)=sound?10:0;
					call(0xfb40);
					++SoundIndex3;
				}
			}
			++SoundCount;
		}
	}
}

// Sélectionne un niveau particulier si le code a été entré

void SelectLevel(void)
{
static char l,i;

	for(l=0;l<30;l++)
	{
		for(i=0;i<8;i++)
			if(KeyLevels[i][l]!=TextKey[i]) break;
		if((i==8)||((i==KeyLen)&&(KeyLevels[i][l]==' ')))
		{
			level=l;
			return;
		}
	}
}

// Animation avant le début du jeu

void anim(void)
{
static char	z;

	while(x<25-width)
	{
		for(xx=0;xx<6;++xx)
		{
			player();
			for(z=80;--z;);
		}
		x++;
	}
	while(x>=0)
	{
		for(xx=5;xx>=0;--xx)
		{
			player();
			for(z=60;--z;);
		}
		xx=6;
		x--;
	}
	x=0;
	xx=0;
	player();
}

// Début de tableau

void start(void)
{
	bgcolor=levels[level][15][0];
	fwcolor=7-bgcolor;
	background();
	bricks();
	x=0;
	xx=0;
	BonusCount=0;
	Bonus=0;
}

// Paramètres de jeu par défaut

void init(void)
{
	width=4;
	speed=3;
	topspeed=false;
	turbo=false;
	SoundNum1=0;
	SoundNum2=0;
	SoundNum3=0;
	SoundIndex1=5;
	SoundIndex2=5;
	SoundIndex3=5;
	SoundCount=0;
}

// Point d'entrée du programme

int main(void)
{
// static char	*ptr;

	hires();
	InitSound();
	setflags(SCREEN|PROTECT|NOKEYCLICK);
	start();

	while(true)
	{
		demo=true;
		KeyLen=0;
		init();
		play();				// Demo
		demo=false;
		lives=5;				// Cheat mode
		level=0;				// Cheat mode (max=29)
		SelectLevel();
		score[0]=score[1]=score[2]=score[3]=score[4]=0;
		DisplayLevelName();
		start();
		do
		{
			anim();
			init();
			DisplayScore(1);
			play();			// Jeu
			while(BonusPhase) DisplayBonus();
			width=4;
			if(x>25-width) {x=25-width;xx=5;}
			if(nbr)			// On a perdu
				lives--;
			else				// On change de niveau
			{
				level++;
				DisplayLevelName();
				start();
			}
		} while(lives);
		hiscore();
		DisplayScore(1);
	}
	return(EXIT_SUCCESS);
}
