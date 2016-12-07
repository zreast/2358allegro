/*
PROJECT 2358
2 Minutes to Midnight

By KMITL Computer Engineering students (CE1D)

Code Developer : Satthathan Chanchartree
*/

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "objects.h"

//global
const int WIDTH = 800;
const int HEIGHT = 600;
const int NUM_BULLETS = 500;
const int NUM_COMETS = 555;
const int NUM_EXPLOSIONS = 20;
const int NUMMUSIC=6;
const int KEYCHEAT = 1;
const int SAVEBUFFER = 25;
const int BOSSNUM = 1;

SpaceShip ship;
Bullet bullets[NUM_BULLETS],barrier[3],bossbullet[700];
Comet comets[NUM_COMETS],boss[BOSSNUM];
Explosion explosions[NUM_EXPLOSIONS];


ALLEGRO_DISPLAY *display = NULL;

ALLEGRO_SAMPLE_ID two;
ALLEGRO_SAMPLE_ID carry;
ALLEGRO_SAMPLE_ID frontier;
ALLEGRO_SAMPLE_ID player;
ALLEGRO_SAMPLE_ID bgm;
ALLEGRO_SAMPLE_ID over;
ALLEGRO_SAMPLE_ID natkk;
ALLEGRO_SAMPLE *natk;
ALLEGRO_SAMPLE_ID boomm;
ALLEGRO_SAMPLE *boom;
ALLEGRO_SAMPLE_ID barriermm;
ALLEGRO_SAMPLE *barrierm;
ALLEGRO_SAMPLE_ID nopess;
ALLEGRO_SAMPLE *nopes;
ALLEGRO_SAMPLE_ID ulttt;
ALLEGRO_SAMPLE *ultt;

ALLEGRO_BITMAP *shipImage;
ALLEGRO_BITMAP *cometImage;
ALLEGRO_BITMAP *expImage;	
ALLEGRO_BITMAP *casset = NULL;
ALLEGRO_BITMAP *casset1 = NULL;
ALLEGRO_BITMAP *casset2 = NULL;
ALLEGRO_BITMAP *menu0 = NULL;
ALLEGRO_BITMAP *notes = NULL;
ALLEGRO_BITMAP *enterkey = NULL;
ALLEGRO_BITMAP *enterkey2 = NULL;
ALLEGRO_BITMAP *archive = NULL;
ALLEGRO_BITMAP *credittext = NULL;
ALLEGRO_BITMAP *nbullet = NULL;
ALLEGRO_BITMAP *bnbullet = NULL;
ALLEGRO_BITMAP *barrierimp = NULL;
ALLEGRO_BITMAP *barrierimp2 = NULL;
ALLEGRO_BITMAP *ult1 = NULL;
ALLEGRO_BITMAP *ult2 = NULL;
ALLEGRO_BITMAP *selectt = NULL;
ALLEGRO_BITMAP *tt = NULL;
ALLEGRO_BITMAP *t0 = NULL;
FILE * fp;

Background credittextt;
int men=0,cursor=1,mode,newgame=0,slow=0,enter=0,playsong=0;
clock_t initial,final;
int pause,cassetcount=0,unlocker=-1,unlockid,cre=0,curhp;
int dying=-1,broken=0,bimp=0,regencount=0,reggae,damage=0,ult=0,type=0,opex,mov,volt,bossinit=1,end;
int savebuffer[100],skill=0,curmana,drain,nope=0,cooldown;
int curbgm;
double temp_cpu,temp_cpu2,oldbbullet,oldbullet,starttime,oldskill,unlocktime;
char keybuffer[30],keycheat[KEYCHEAT][30],top;
double flaytime;

enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE, ESCAPE, ENTER, Z, X, LSHIFT};
enum STATE{MENU, PLAYING,SHOP,PAUSE,STORY, GAMEOVER, MUSICBOX,ENTERKEY ,CREDIT,SELECT};
bool keys[11] = {false, false, false, false, false,false, false, false, false, false};
int state = MENU;


Music music[NUMMUSIC];

//prototypes
void DrawBoss (Comet bosses[],int id); 
void InitBoss(Comet bosses[], ALLEGRO_BITMAP *image);
void CollideRay(Comet comets[], SpaceShip &ship);
void printult(void);
void printpause(void);
void liststage(void);
void UpdateBarrier(Bullet bullet[], int size);
void skillbarrier(void);
void archiveunlocked (int misc);
void save(int saveslot,int value);
void loadsave(void);
void activekeycheat(int id);
void printunlockcheat(void);
void initkeycheat(void);
void initmusic(void);
void listmusic(void);
void shopstate(void);
void fadeout(ALLEGRO_BITMAP *imagem, int velocity);
void fadein(ALLEGRO_BITMAP *imagem, int velocity);

void ChangeState(int &state, int newState);
void InitBackground(Background &back, float x, float y, float velx, float vely, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image);
void UpdateBackground(Background &back);
void UpdateBackgroundYAXIS(Background &back);
void DrawBackground(Background &back);
void InitShip(SpaceShip &ship, ALLEGRO_BITMAP *image);
void ResetShipAnimation(SpaceShip &ship, int position);
void DrawShip(SpaceShip &ship);
void MoveShipUp(SpaceShip &ship);
void MoveShipDown(SpaceShip &ship);
void MoveShipLeft(SpaceShip &ship);
void MoveShipRight(SpaceShip &ship);

void InitBullet(Bullet bullet[], int size);
void InitBarrier(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void DrawBarrier(Bullet bullet[], int size);
void DrawBarrier2(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, SpaceShip &ship);

void InitBossBullet(Bullet bullet[], int size);
void FireBossBullet(Bullet bullet[], int size, Comet bosses[]);
void DrawBossBullet(Bullet bullet[], int size);
void UpdateBossBullet(Bullet bullet[], int size);

void UpdateBullet(Bullet bullet[], int size);
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize);
void CollideBossBullet(Bullet bullet[], int bSize, SpaceShip &ship, Explosion explosions[], int eSize);
void CollideBarrier(Comet comets[], SpaceShip &ship, Explosion explosions[]);

void InitComet(Comet comets[], int size, ALLEGRO_BITMAP *image);
void DrawComet(Comet comets[], int size);
void StartComet(Comet comets[], int size);
void UpdateComet(Comet comets[], int size);
void UpdateBoss(Comet bosses[], int size);
void CollideComet(Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize);

void InitExplosions(Explosion explosions[], int size, ALLEGRO_BITMAP *image);
void DrawExplosions(Explosion explosions[], int size);
void StartExplosions(Explosion explosions[], int size, int x, int y);
void UpdateExplosions(Explosion explosions[], int size);

void superperfectverycoolkeyboardtype (ALLEGRO_EVENT typer);
void menuselect(void);

int main(void)
{
	//primitive variable
	bool done = false;
	bool redraw = true;
	bool pause = false;
	const int FPS = 60;
	bool isGameOver = false;
	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;
	int pos_x = WIDTH/ 2;
	int pos_y = HEIGHT/ 2;


	//Allegro variables
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *gui;
	ALLEGRO_BITMAP *s0 = NULL;
	ALLEGRO_BITMAP *s00 = NULL;
	ALLEGRO_BITMAP *s1 = NULL;
	ALLEGRO_BITMAP *s2 = NULL;
	ALLEGRO_BITMAP *s3 = NULL;
	ALLEGRO_BITMAP *s4 = NULL;
	ALLEGRO_BITMAP *s5 = NULL;
	ALLEGRO_BITMAP *s6 = NULL;
	ALLEGRO_BITMAP *s7 = NULL;
	ALLEGRO_BITMAP *s8 = NULL;
	ALLEGRO_BITMAP *s9 = NULL;
	ALLEGRO_BITMAP *s10 = NULL;
	Background stage0;
	Background stage00;
	Background stage1;
	Background stage2;
	Background stage3;
	Background stage4;
	Background stage5;
	Background stage6;
	Background stage7;
	Background stage8;
	Background stage9;
	Background stage10;


	//initial
	if(!al_init())										
		return -1;
	display = al_create_display(WIDTH, HEIGHT);	
	if(!display)										
		return -1;
	
	al_set_window_title(display, "2 Minutes to Midnight");

	//GAME TITLE (display once)
	
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_BITMAP *iconn=al_load_bitmap("iconn.png");
	al_set_display_icon(display,iconn);
	
	al_clear_to_color(al_map_rgb(0,0,0));
	
	ALLEGRO_FONT *fload = al_load_font("CPU.ttf",30,0);
	al_draw_textf(fload, al_map_rgb(184, 184, 184), 780, 550, ALLEGRO_ALIGN_RIGHT, "LOADING...",1);
	al_flip_display();

		//Addon
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	

	
	//Set Sample
	al_reserve_samples(10);

	//set event
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	initmusic();

	//Bitmap
	shipImage  = al_load_bitmap("spaceship_by_arboris.png");
	al_convert_mask_to_alpha(shipImage, al_map_rgb(255, 0, 255));
	cometImage = al_load_bitmap("asteroid-1-96.png");
	expImage = al_load_bitmap("explosion_3_40_128.png");
	gui = al_load_bitmap("ui.png");
	ALLEGRO_BITMAP *life=al_load_bitmap("life.png");
	ALLEGRO_BITMAP *patk=al_load_bitmap("atk.png");
	ALLEGRO_BITMAP *pspell=al_load_bitmap("spell.png");
	ALLEGRO_BITMAP *pmspd=al_load_bitmap("mspd.png");
	ALLEGRO_BITMAP *paspd=al_load_bitmap("aspd.png");
	ALLEGRO_BITMAP *ultf=al_load_bitmap("ult-finished.png");
	ALLEGRO_BITMAP *ultf2=al_load_bitmap("ult-cooldown.png");
	ALLEGRO_BITMAP *slot2=al_load_bitmap("itemslot2.png");
	ALLEGRO_BITMAP *slot3=al_load_bitmap("itemslot3.png");
	ALLEGRO_BITMAP *demo=al_load_bitmap("demo.png");
	menu0=al_load_bitmap("23580.png");
	ALLEGRO_BITMAP *menu1=al_load_bitmap("2358.png");
	ALLEGRO_BITMAP *menu2=al_load_bitmap("23581.png");
	ALLEGRO_BITMAP *menu3=al_load_bitmap("23582.png");
	ALLEGRO_BITMAP *curs=al_load_bitmap("curs.png");
	ALLEGRO_BITMAP *shop=al_load_bitmap("shop.png");
	enterkey=al_load_bitmap("enterkey.png");
	enterkey2=al_load_bitmap("enterkey2.png");
	casset=al_load_bitmap("casset.png");
	casset1=al_load_bitmap("casset1.png");
	casset2=al_load_bitmap("casset2.png");
	notes=al_load_bitmap("note.png");
	archive=al_load_bitmap("archive.png");
	ALLEGRO_SAMPLE *deny=al_load_sample("deny.ogg");
	ALLEGRO_BITMAP *credit = al_load_bitmap("credit.png");
	ALLEGRO_BITMAP *credit1 = al_load_bitmap("credit1.png");
	ALLEGRO_BITMAP *credit2 = al_load_bitmap("credit2.png");
	credittext = al_load_bitmap("credittext.png");
	nbullet = al_load_bitmap("nbullet.png");
	bnbullet = al_load_bitmap("bossb.png");
	barrierimp = al_load_bitmap("barrier.png");
	barrierimp2 = al_load_bitmap("barrier1.png");
	ALLEGRO_BITMAP *pausee = al_load_bitmap("pause.png");
	ALLEGRO_BITMAP *over1 = al_load_bitmap("over1.png");
	ALLEGRO_BITMAP *over2 = al_load_bitmap("over2.png");
	ALLEGRO_BITMAP *over3 = al_load_bitmap("over3.png");
	ALLEGRO_BITMAP *nopek = al_load_bitmap("nopeknight.png");
	ult1 = al_load_bitmap("ray1.png");
	ult2 = al_load_bitmap("ray2.png");
	selectt = al_load_bitmap("select.png");
	tt = al_load_bitmap("lock.png");
	t0 = al_load_bitmap("0thumb.png");
	ALLEGRO_BITMAP *boss0imp = al_load_bitmap("boss0.png");
	ALLEGRO_BITMAP *voltic1 = al_load_bitmap("voltic1.png");
	ALLEGRO_BITMAP *voltic2 = al_load_bitmap("voltic2.png");
	
	//initial stage
	s0 = al_load_bitmap("stage0.png");
	s00 = al_load_bitmap("stage00.png");
	s8 = al_load_bitmap("stage8.png");

	InitBackground(stage0, 0, 0, 1, 0, 3898, 600, -1, 1, s0);
	InitBackground(stage00, 0, 0, 16, 0, 1000, 600, -1, 1, s00);

	srand(time(NULL));

	
	//Fonts
	ALLEGRO_FONT *fnaipol = al_load_font("Naipol.ttf", 18, 0);
	ALLEGRO_FONT *farmageddon = al_load_font("Armageddon.ttf",24,0);
	ALLEGRO_FONT *farmageddonm = al_load_font("Armageddon.ttf",18,0);
	ALLEGRO_FONT *farmageddons = al_load_font("Armageddon.ttf",14,0);
	ALLEGRO_FONT *fisoc = al_load_font("isocpeui.ttf",20,0);
	ALLEGRO_FONT *fadler = al_load_font("adler.ttf",16,0);
	ALLEGRO_FONT *fcpu = al_load_font("CPU.ttf",30,0);
	ALLEGRO_FONT *fcpub = al_load_font("CPU.ttf",34,0);
	ALLEGRO_FONT *fcpubigmak = al_load_font("CPU.ttf",44,0);
	ALLEGRO_FONT *fcpubigmakmak = al_load_font("CPU.ttf",60,0);
	ALLEGRO_FONT *fdigital7 = al_load_font("digital7mi.ttf",22,0);
	ALLEGRO_FONT *fdigital77 = al_load_font("digital7.ttf",22,0);
	ALLEGRO_FONT *farl = al_load_font("ARL.ttf",26,0);
	ALLEGRO_FONT *fbgoth = al_load_font("bgothm.ttf",26,0);
	
	
	//Events
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);
	oldbullet=0;
	initial=clock();
	
	ALLEGRO_BITMAP *title1=al_load_bitmap("6661-title-1.png");
	fadeout(title1, 3);
	al_rest(2.0);
    fadein(title1, 3);
	
	ALLEGRO_BITMAP *title2=al_load_bitmap("6661-title-2.png");
	fadeout(title2, 10);
	al_draw_bitmap(title2,0,0,0);		 	al_flip_display(); al_rest(0.3);
	al_clear_to_color(al_map_rgb(0,0,0));	al_flip_display(); al_rest(0.02);
	al_draw_bitmap(title2,0,0,0);			al_flip_display(); al_rest(0.2);
	al_clear_to_color(al_map_rgb(0,0,0));	al_flip_display(); al_rest(0.02);
	al_draw_bitmap(title2,0,0,0);			al_flip_display(); al_rest(0.1);
	al_draw_bitmap(title2,50,60,0);			al_flip_display(); al_rest(0.02);
	al_draw_bitmap(title2,0,0,0);			al_flip_display(); al_rest(1.0);
    fadein(title2, 7);
	
	fadeout(menu0, 100);
	fp = fopen ("ohmygod.omg", "r");
	for(int jjj=0;jjj<SAVEBUFFER;jjj++)
	fscanf(fp, "%d", &savebuffer[jjj]);
    fclose(fp);
	loadsave();
	
	al_play_sample(music[0].musiku,1,0,1,ALLEGRO_PLAYMODE_LOOP,&two);
	
	//Start Game
	while(!done)
	{ 
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		
		//TIMER
		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			frames++;
			if(al_current_time() - gameTime >= 1)
			{
				gameTime = al_current_time();
				gameFPS = frames;
				frames = 0;
			}

			if(state == MENU)
			{
				if(newgame)
				{
					if(cursor>4) cursor=4;
					if(cursor<1) cursor=1;
				}
				else
				{
					if(cursor>6) cursor=6;
					if(cursor<1) cursor=1;
				}
				if(enter&&cursor==1&&newgame)
				{
					enter=0; mode=1; ChangeState(state, PLAYING); newgame=0;
				}
				else if(enter&&cursor==2&&newgame)
				{
					enter=0; mode=2; ChangeState(state, PLAYING); newgame=0;
				}
				else if(enter&&cursor==3&&newgame)
				{
					enter=0; mode=3; ChangeState(state, PLAYING); newgame=0;
				}
				else if(enter&&cursor==4&&newgame)
				{
					enter=0; newgame=0; cursor=1;
				}
				else if(enter&&cursor==1)
					{enter=0; newgame=1;}
				else if(enter&&cursor==2)
					{enter=0; fadein(menu0,80); ChangeState(state, SELECT);}	
				else if(enter&&cursor==3)
					{enter=0; ChangeState(state, CREDIT);}	
				else if(enter&&cursor==4)
					{enter=0; fadein(menu0,80); ChangeState(state, ENTERKEY);}	
				else if(enter&&cursor==5)
					{enter=0; fadein(menu0,80); ChangeState(state, MUSICBOX);}					
				else if(enter&&cursor==6)
					{enter=0; done=true;}
			}
			else if(state == CREDIT)
			{
				UpdateBackgroundYAXIS(credittextt);
			}
			else if(state == PLAYING && pause == 0)
			{
				if(keys[LSHIFT]&&slow==0)
				{
					ship.speed/=2; slow=1;
				}
				if(keys[UP])
					MoveShipUp(ship);
				else if(keys[DOWN])
					MoveShipDown(ship);
				else
					ResetShipAnimation(ship, 1);

				if(keys[LEFT])
					MoveShipLeft(ship);
				else if(keys[RIGHT])
					MoveShipRight(ship);
				else
					ResetShipAnimation(ship, 2);
				
				if(ult)
				{
					if(ult>=180)
					{
						ult=0; cooldown=0;
					}
				}
				else if(keys[Z]&&skill!=1)
				{
					FireBullet(bullets, NUM_BULLETS, ship);
				}
				else if(skill==1)
				{
					if(ship.skill == BARRIER)
					{
						if(((double)clock()-oldskill)/CLOCKS_PER_SEC >=1.0)
						{
							skill=0;
							barrier[0].live=false;
						}
						else
						skillbarrier();
					}
					else skill=0;
					
				}
				UpdateBackground(stage0);
				UpdateBackground(stage00);
				UpdateExplosions(explosions, NUM_EXPLOSIONS);
				UpdateBullet(bullets, NUM_BULLETS);
				if(skill==2) UpdateBarrier(barrier,1 );
				
				
				if(((int)temp_cpu)/60 <1)
				StartComet(comets, NUM_COMETS);
				if(temp_cpu > 60 && temp_cpu <= 61) 
				{
					bossinit =1;
				}
				
				if(bossinit==1)
				{
					oldbbullet=0;
					InitBoss(boss,boss0imp);
					InitBossBullet(bossbullet,500);
					boss[0].live = true;
					mov=0;
					curhp=boss[0].hp;
					bossinit=0;
				}
				
				if(boss[0].live == true)
				{
					if(mov==0)
					{
						opex = rand()%3;
						if(opex == 0)
						{
							mov=1;
						}
						else if(opex == 1)
							mov=2;
						else
						{
							mov=3;
							volt=1;
						}
					}
					else
					{
						UpdateBoss(boss, BOSSNUM);
					}
					if(mov!=3)
					{
						FireBossBullet(bossbullet,500,boss);
					}
					else
					{
						if(ship.y<boss[0].y+27 && ship.y > boss[0].y-8&&damage==0)
						{
							ship.lives--;
							damage=1;
							dying=0;
							StartExplosions(explosions, NUM_EXPLOSIONS, ship.x, ship.y);
						}
					}
					UpdateBossBullet(bossbullet,500);
					CollideBossBullet(bossbullet,500, ship, explosions, NUM_EXPLOSIONS);
					
				}

				
				
				
				
				UpdateComet(comets, NUM_COMETS);
				CollideBullet(bullets, NUM_BULLETS, comets, NUM_COMETS, ship, explosions, NUM_EXPLOSIONS);
				if(damage==0)
				CollideComet(comets, NUM_COMETS, ship, explosions, NUM_EXPLOSIONS);
				if(damage) damage++;
				if(damage>61) damage=0;
				if(skill==2) CollideBarrier(comets, ship, explosions);
				if(ult) CollideRay(comets,ship);

				if(ship.lives <= 0)
				{
					flaytime += ((double)clock() - starttime) / CLOCKS_PER_SEC;
					ChangeState(state, GAMEOVER);
				}
					

		
			}
			else if(state == PLAYING && pause == 1)
			{
				if(cursor>3) cursor=3;
				if(cursor<1) cursor=1;
				if(enter)
				{
					if(cursor==1)
					{
						pause=0;
						keys[UP]=keys[DOWN]=keys[LEFT]=keys[RIGHT]=false;
						
						starttime = clock();
					}
					else if(cursor==2)
					{
						pause=0;
						ChangeState(state,MENU);
					}
					else if(cursor==3) done=true;
					enter=0;

				}
			}
			else if(state == GAMEOVER)
			{
					if(music[4].unlock==0)
					{
						music[4].unlock=1;
						save(14,1);
						unlocker=0; unlockid=4; unlocktime=clock();
					}	
					
				if(cursor>4) cursor=4;
				if(cursor<1) cursor=1;
				if(enter)
				{
					if(cursor==1)
					{
						al_stop_sample(&bgm);
						ChangeState(state,PLAYING);
					}
					else if(cursor==2)
					{
						ChangeState(state,MENU);
					}
					else if(cursor==3) done=true;
					else if(cursor==4) nope=1;
					enter=0;

				}
			}
			else if(state == ENTERKEY)
			{
				if(cursor>2){ cursor=2; }
				if(cursor<1) {cursor=1; }
			
			}
			else if(state == SELECT)
			{
				if(newgame==0)
				{
					if(cursor>4){ cursor=4; }
					if(cursor<1) {cursor=1; }
				}
				if(newgame==1)
				{
					if(cursor>4){ cursor=4; }
					if(cursor<1) {cursor=1; }
				}
				if(enter&&newgame==0)
				{
					cursor=1;	
					if(cursor==1) 
					{
						newgame=1;
					}
					else if(cursor==4) ChangeState(state,MENU);
					enter=0;
				}
				else if(enter&&newgame==1)
				{
					cursor=1;
					if(cursor==1) 
					{
						mode=1; ChangeState(state, PLAYING);
					}
					if(cursor==2) 
					{
						mode=2; ChangeState(state, PLAYING);
					}
					if(cursor==3) 
					{
						mode=3; ChangeState(state, PLAYING);
					}
					newgame=0;
					enter=0;
				}
			
			}
			else if(state == MUSICBOX)
			{
				if(cursor>NUMMUSIC+1){ cursor=1; }
				if(cursor<1) {cursor=NUMMUSIC+1; }
				if(enter&&cursor==NUMMUSIC+1)
				{
						ChangeState(state,MENU);
						enter = 0;
				}
				else if(enter)
				{
					al_stop_sample(&two);
					if(music[0+cursor-1].unlock)
					{
						playsong=1;
						al_stop_sample(&player);
						al_play_sample(music[0+cursor-1].musiku,1,0,1,ALLEGRO_PLAYMODE_LOOP,&player);
					}
					else
					{
						playsong=1;
						al_stop_sample(&player);
						al_play_sample(deny,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
					}
					enter=0;
				}

			}
			
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			
			if(state == CREDIT)
			{
				if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE || ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
				{
					ChangeState(state, MENU);
				}
			}
			else if(state == PLAYING && pause == 0)
			{
				switch(ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:{  pause=1; cursor=1; 
				flaytime += ((double)clock() - starttime) / CLOCKS_PER_SEC;
										} break;
				case ALLEGRO_KEY_UP:
					keys[UP] = true;
					break;
				case ALLEGRO_KEY_DOWN:
					keys[DOWN] = true;
					break;
				case ALLEGRO_KEY_LEFT:
					keys[LEFT] = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					keys[RIGHT] = true;
					break;
				case ALLEGRO_KEY_Z:
					keys[Z] = true;
					break;
				case ALLEGRO_KEY_X:
					{
						if(ship.mana>=50 && skill==0)
						{
							al_play_sample(barrierm,1,0,1,ALLEGRO_PLAYMODE_ONCE,&barriermm);
							skill=1; ship.mana -= 50;
							oldskill=clock();
						}
					}
					break;
				case ALLEGRO_KEY_LSHIFT:
					keys[LSHIFT] = true;
					break;
				case ALLEGRO_KEY_SPACE:
					{
						if(cooldown>=3600&&ult==0) 
						{
							ult=1;
							al_play_sample(ultt,1,0,1,ALLEGRO_PLAYMODE_ONCE,&ulttt);
						}			
					}
					break;
			
				}
			}
			else if(state == PLAYING && pause )
			{
				switch(ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:{starttime = clock(); pause=0; keys[UP]=keys[DOWN]=keys[LEFT]=keys[RIGHT]=false; } break;
				case ALLEGRO_KEY_UP:
					cursor--;
					break;
				case ALLEGRO_KEY_DOWN:
					cursor++;
					break;				
				case ALLEGRO_KEY_ENTER:
					enter=1;
					break;
			
				}
			}
			else if(state == MENU || state == MUSICBOX || state == GAMEOVER || state == SELECT)
			{
				switch(ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_UP:
					cursor--;
					break;
				case ALLEGRO_KEY_DOWN:
					cursor++;
					break;
				case ALLEGRO_KEY_ENTER:
					{enter=1; }
					break;	
				}
			}
			else if(state == ENTERKEY)
			{
				if(ev.keyboard.keycode == ALLEGRO_KEY_UP) cursor--;
				else if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN) cursor++;
				else superperfectverycoolkeyboardtype(ev);
			}
			
			
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			if(state == PLAYING&&pause==0)
			{
				switch(ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_UP:
					keys[UP] = false;
					break;
				case ALLEGRO_KEY_DOWN:
					keys[DOWN] = false;
					break;
				case ALLEGRO_KEY_LEFT:
					keys[LEFT] = false;
					break;
				case ALLEGRO_KEY_RIGHT:
					keys[RIGHT] = false;
					break;
				case ALLEGRO_KEY_Z:
					keys[Z] = false;
					break;
				case ALLEGRO_KEY_LSHIFT:
					{keys[LSHIFT] = false; slow=0; ship.speed*=2;}
					break;
				}
			}
			
		}

		
		//Update PER frame
		if(redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
			if(state == MENU)
			{
				if(men>=0&&men<20)
				{
					al_draw_bitmap(menu1, 0, 0, 0);
					menuselect();
					men++;
				}
				else if(men>=20&&men<40)
				{
					al_draw_bitmap(menu2, 0, 0, 0);
					menuselect();
					men++;
				}
				else if(men>=40&&men<60)
				{
					al_draw_bitmap(menu3, 0, 0, 0);
					menuselect();
					men++;
				}
				else if(men>=60&&men<80)
				{
					al_draw_bitmap(menu2, 0, 0, 0);
					menuselect();
					men++;
				}
				if(men>=80) men=0;
				
			}
			else if(state == CREDIT)
			{
				if(cre>=60)
					{al_draw_bitmap(credit1,0,0,0); cre++;}
				else if(cre>=40)
					{al_draw_bitmap(credit2,0,0,0); cre++;}
				else if(cre>=20)
					{al_draw_bitmap(credit1,0,0,0); cre++;}
				else if(cre>=0)
					{al_draw_bitmap(credit,0,0,0); cre++;}
				if(cre>=80) cre=0;
				DrawBackground(credittextt);

			}
			else if(state == MUSICBOX)
			{
				listmusic();
			}
			else if(state == SELECT)
			{
				liststage();
			}
			else if(state == PLAYING || state == SHOP || state == GAMEOVER)
			{
				if(state == PLAYING && end)
				{
					al_draw_bitmap(demo,200,0,0);
				}
				else if(state == PLAYING&&pause==0)
				{
					DrawBackground(stage0);
					
					if(dying>=55){		dying++;	}
					else if(dying>=50){		DrawShip(ship);		dying++;	}
					else if(dying>=45){		dying++;	}
					else if(dying>=40){		DrawShip(ship);		dying++;	}
					else if(dying>=35){		dying++;	}
					else if(dying>=30){		DrawShip(ship);		dying++;	}
					else if(dying>=25){		dying++;	}
					else if(dying>=20){		DrawShip(ship);		dying++;	}
					else if(dying>=15){		dying++;	}
					else if(dying>=10){		DrawShip(ship);		dying++;	}
					else if(dying>=5){		dying++;	}
					else if(dying>=0){		DrawShip(ship);		dying++;	}
					if(dying>60) dying=-1;
					if(dying==-1) DrawShip(ship);
					
					DrawBullet(bullets, NUM_BULLETS);
					if(skill==1) DrawBarrier(barrier, 1);
					if(skill==2) DrawBarrier2(barrier, 1);
					DrawComet(comets, NUM_COMETS);
					if(boss[0].live==true)
					{
						DrawBossBullet(bossbullet,500);
						if(mov==3&&volt)
						{
							if(volt%4==0||volt%4==1)
							al_draw_bitmap(voltic1,50,boss[0].y-5,0);
							else
							al_draw_bitmap(voltic2,50,boss[0].y-5,0);

							volt++;
							if(volt==100) volt=1;
						}
						DrawBoss(boss,0);
						if(curhp>boss[0].hp) curhp--;
						if(curhp<boss[0].hp) curhp++;
						al_draw_filled_rectangle(210, 40, 210+(curhp/2), 45, al_map_rgb(255, 0, 0));
					}
					DrawExplosions(explosions, NUM_EXPLOSIONS);
					DrawBackground(stage00);
					if(ult)
					{
						printult();
					}
					for(int ite=0;ite<ship.lives;ite++)
					{
						al_draw_bitmap(life,210+ite*35,HEIGHT-50,0);
					}
					if(curmana>ship.mana) curmana--;
					if(curmana<ship.mana) curmana++;
					al_draw_filled_rectangle(210, HEIGHT-20, 210+curmana, HEIGHT-15, al_map_rgb(43, 173, 217));

					cooldown+=1;					
				}
				if(state == SHOP)
					al_draw_bitmap(shop,200,0,0);
				if(state == GAMEOVER)
				{
					if(reggae>=120)
					{
						al_draw_bitmap(over3,200,0,0); reggae++;
					}
					else if(reggae>=60)
					{
						al_draw_bitmap(over2,200,0,0); reggae++;
					}
					else if(reggae>=0)
					{
						al_draw_bitmap(over1,200,0,0); reggae++;
					}

					al_draw_textf(fcpubigmakmak, al_map_rgb(255, 255, 255), 491, 150, ALLEGRO_ALIGN_CENTRE, "GAME OVER",666);
					
					if(cursor==1)
					al_draw_textf(fcpubigmak, al_map_rgb(255, 255, 255), 491, 240, ALLEGRO_ALIGN_CENTRE, "PLAY AGAIN",666);
					else
					al_draw_textf(fcpubigmak, al_map_rgb(0, 0, 0), 491, 240, ALLEGRO_ALIGN_CENTRE, "PLAY AGAIN",666);
					if(cursor==2)
					al_draw_textf(fcpubigmak, al_map_rgb(255, 255, 255), 491, 280, ALLEGRO_ALIGN_CENTRE, "RETURN TO MENU",666);
					else
					al_draw_textf(fcpubigmak, al_map_rgb(0, 0, 0), 491, 280, ALLEGRO_ALIGN_CENTRE, "RETURN TO MENU",666);
					if(cursor==3)
					al_draw_textf(fcpubigmak, al_map_rgb(255, 255, 255), 491, 320, ALLEGRO_ALIGN_CENTRE, "QUIT GAME",666);
					else
					al_draw_textf(fcpubigmak, al_map_rgb(0, 0, 0), 491, 320, ALLEGRO_ALIGN_CENTRE, "QUIT GAME",666);
					if(cursor==4)
					al_draw_textf(fcpubigmak, al_map_rgb(255, 255, 255), 491, 360, ALLEGRO_ALIGN_CENTRE, "*STAGE HINT*",666);
					else
					al_draw_textf(fcpubigmak, al_map_rgb(0, 0, 0), 491, 360, ALLEGRO_ALIGN_CENTRE, "*STAGE HINT*",666);
					if(nope==1)
					{
						al_play_sample(nopes,1,0,1,ALLEGRO_PLAYMODE_ONCE,&nopess);
					}
					if(nope)
					{

						al_draw_bitmap(nopek,440,400,0);
						al_draw_textf(fcpubigmak, al_map_rgb(255, 255, 255), 480, 500, ALLEGRO_ALIGN_CENTRE, "NOPE",666);
						nope++;
					}
					if(nope>=60) nope=0;
					
					if(reggae>180) reggae=0;
				}
				
				
				if(pause)
				{
					al_draw_bitmap(pausee, 200, 0, 0);
					printpause();
				}
				al_draw_bitmap(gui, 0, 0, 0);
			
				al_draw_textf(farl, al_map_rgb(255, 255, 255), 35, 30, 0, " STAGE %i",0);

				//Time Catcher
				final=clock();
				temp_cpu = (((double)final-starttime) / CLOCKS_PER_SEC) + flaytime;
				if(pause==0 &&state!=GAMEOVER && end==0) al_draw_textf(fdigital7, al_map_rgb(181, 168, 128), 10, 80, 0, "TIME   : %02i:%02i:%02i",((int)temp_cpu)/60,((int)temp_cpu)%60,((int)(temp_cpu*100))%100);
				else al_draw_textf(fdigital7, al_map_rgb(181, 168, 128), 10, 80, 0, "TIME   : %02i:%02i:%02i",((int)flaytime)/60,((int)flaytime)%60,((int)(flaytime*100))%100);
				
				al_draw_textf(fdigital7, al_map_rgb(181, 168, 128), 10, 100, 0, "TOMATO : %07i",ship.tomato);
				regencount++;
				regencount%=1000;
				if(regencount%30==0)
				{
					if(ship.mana<ship.maxmana)
					ship.mana++;
				}
				al_draw_bitmap(patk, 10, 170, 0);
				al_draw_textf(fcpu, al_map_rgb(255, 255,255), 45, 165, 0, "%03i",ship.atk);
				al_draw_bitmap(pspell, 100, 170, 0);
				al_draw_textf(fcpu, al_map_rgb(255, 255, 255), 140, 165, 0, "%03i",ship.spell);
				al_draw_bitmap(pmspd, 10, 200, 0);
				al_draw_textf(fcpu, al_map_rgb(255,255, 255), 45, 195, 0, "%03i",ship.speed);
				al_draw_bitmap(paspd, 100, 200, 0);
				al_draw_textf(fcpu, al_map_rgb(255, 255, 255), 140, 195, 0, "%.2f",ship.aspd);
				if(cooldown<=3600)
				{
					al_draw_bitmap(ultf2, 10,230, 0);
					al_draw_textf(fdigital77, al_map_rgb(0, 0, 0), 20, 246, 0, "%2i%%",((int)(cooldown/36.0))%100);
					
				}
				else
				{	
					if((((int)(temp_cpu*100))%100)%4==0||(((int)(temp_cpu*100))%100)%4==1)
					al_draw_bitmap(ultf, 10,230, 0);
					
				}

				
				al_draw_textf(farmageddons, al_map_rgb(0, 0, 0), 60, 235, 0, "LIFEFORM",666);
				al_draw_textf(farmageddons, al_map_rgb(0, 0, 0), 60, 245, 0, "DISINTEGRATION",666);
				al_draw_textf(farmageddons, al_map_rgb(0, 0, 0), 60, 255, 0, "RAY",666);
				al_draw_bitmap(slot3, 30, 340, 0);
				al_draw_bitmap(slot2, 95, 340, 0);
				al_draw_bitmap(slot2, 30, 410, 0);
				al_draw_bitmap(slot2, 95, 410, 0);

				al_draw_textf(farmageddonm, al_map_rgb(255, 255, 255), 10, 510, 0, "%s",music[curbgm].name);    
				if(mode==1)
				al_draw_textf(farmageddonm, al_map_rgb(255, 255, 255), 65, 570, 0, "HARD",666);    
				if(mode==2)
				al_draw_textf(farmageddonm, al_map_rgb(255, 255, 255), 55, 570, 0, "EXPERT",666);    
				if(mode==3)
				al_draw_textf(farmageddonm, al_map_rgb(255, 255, 255), 45, 570, 0, "LUNATIC",666);    
				
				al_draw_textf(farmageddons, al_map_rgb(255, 0, 0), WIDTH-85, HEIGHT-30, 0, "FPS: %i", gameFPS);
			}
			else if(state == ENTERKEY)
			{				
				if(cursor==1)
				{
				al_draw_bitmap(enterkey, 0, 0, 0);
				printunlockcheat();
				al_draw_textf(fbgoth, al_map_rgb(255, 255, 255), 220, 105, 0, "%s",keybuffer);
				al_draw_textf(fcpu, al_map_rgb(144, 144, 144), 60, 550, 0, "BACK",666);
				}
				if(cursor==2)
				{
				strcpy(keybuffer,"                         ");
				top =0;
				al_draw_bitmap(enterkey2, 0, 0, 0);
				printunlockcheat();
				al_draw_textf(fcpu, al_map_rgb(255, 255, 255), 60, 550, 0, "BACK",666);
				}			
			}
			if(unlocker==0)
			{
				archiveunlocked(unlockid);
				if((((double)clock() - (double)unlocktime) / CLOCKS_PER_SEC) >=4.0)
					unlocker=-1;
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
						
			
		}
	}

	//Free Memory
	al_destroy_bitmap(expImage);
	al_destroy_bitmap(cometImage);
	al_destroy_bitmap(shipImage);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);					
	al_destroy_sample(natk);
	al_destroy_sample(boom);
	al_destroy_sample(barrierm);
	al_destroy_sample(nopes);
	al_destroy_sample(ultt);
	al_destroy_bitmap(casset);
	al_destroy_bitmap(casset1);
	al_destroy_bitmap(casset2);
	al_destroy_bitmap(menu0);
	al_destroy_bitmap(notes);
	al_destroy_bitmap(enterkey);
	al_destroy_bitmap(enterkey2);
	al_destroy_bitmap(archive);
	al_destroy_bitmap(credittext);
	al_destroy_bitmap(nbullet);
	al_destroy_bitmap(bnbullet);
	al_destroy_bitmap(barrierimp);
	al_destroy_bitmap(barrierimp2);
	al_destroy_bitmap(ult1);
	al_destroy_bitmap(ult2);
	al_destroy_bitmap(selectt);
	al_destroy_bitmap(tt);
	al_destroy_bitmap(t0);
	al_destroy_font(fnaipol);
	al_destroy_font(farmageddon);
	al_destroy_font(farmageddonm);
	al_destroy_font(farmageddons);
	al_destroy_font(fisoc);
	al_destroy_font(fadler);
	al_destroy_font(fcpu);
	al_destroy_font(fcpub);
	al_destroy_font(fdigital7);
	al_destroy_font(fdigital77);
	al_destroy_font(farl);
	al_destroy_font(fbgoth);
	return 0;
}

void InitShip(SpaceShip &ship, ALLEGRO_BITMAP *image)
{

	ship.x = 20+200;
	ship.y = HEIGHT / 2;
	ship.ID = PLAYER;
	ship.lives = 3;
	ship.mana = 100;
	ship.maxmana = 100;
	ship.boundx = 10;
	ship.boundy = 12;
	ship.tomato = 0;
	
	ship.atk = 50;
	ship.spell = 50;
	ship.aspd = 1;
	ship.speed = 200;

	ship.skill = BARRIER;

	ship.maxFrame = 3;
	ship.curFrame = 0;
	ship.frameCount = 0;
	ship.frameDelay = 50;
	ship.frameWidth = 46;
	ship.frameHeight = 41;
	ship.animationColumns = 3;
	ship.animationDirection = 1;

	ship.animationRow = 1;

	ship.image = image;
}

void InitBoss(Comet bosses[], ALLEGRO_BITMAP *image)
{
	for(int i = 0; i < BOSSNUM; i++)
	{
		bosses[i].ID = ENEMY;
		bosses[i].live = false;
		bosses[i].speed = 8;
		if(mode==1)
		{
		bosses[i].aspd = 3.5;
		bosses[i].hp = 1000+i*500;
		}
		if(mode==2)
		{
		bosses[i].aspd = 5;
		bosses[i].hp = 1500+i*500;
		}
		if(mode==3)
		{
		bosses[i].aspd = 10;
		bosses[i].hp = 2500+i*500;
		}

		bosses[i].boundx = 70;
		bosses[i].boundy = 100;
		
		bosses[i].x = 530;
		bosses[i].y = HEIGHT/2-70;


		bosses[i].maxFrame = 143;
		bosses[i].curFrame = 0;
		bosses[i].frameCount = 0;
		bosses[i].frameDelay = 2;
		bosses[i].frameWidth = 96;
		bosses[i].frameHeight = 96;
		bosses[i].animationColumns = 21;

		bosses[i].image = image;
	}
}

void ResetShipAnimation(SpaceShip &ship, int position)
{
	if(position == 1)
		ship.animationRow = 1;
	else
		ship.curFrame = 0;
}
void DrawShip(SpaceShip &ship)
{
	int fx =(ship.curFrame % ship.animationColumns) * ship.frameWidth;
	int fy = ship.animationRow * ship.frameHeight;
	if(broken>=3)
	{
		al_draw_bitmap_region(ship.image, fx, fy, ship.frameWidth,
		ship.frameHeight, ship.x - ship.frameWidth / 2, ship.y - ship.frameHeight / 2, 0);
		broken++;
	}
	else
	{
		al_draw_bitmap_region(ship.image, fx, fy, ship.frameWidth,
		ship.frameHeight, ship.x - ship.frameWidth / 2 , ship.y - ship.frameHeight / 2-1, 0);
		broken++;
	}
	if(broken>6) broken=0;
}
void MoveShipUp(SpaceShip &ship)
{
	ship.animationRow = 0;
	ship.y -= ship.speed/100.0;
	if(ship.y < 0)
		ship.y = 0;
}
void MoveShipDown(SpaceShip &ship)
{
	ship.animationRow = 2;
	ship.y += ship.speed/100.0;
	if(ship.y > HEIGHT)
		ship.y = HEIGHT;
}
void MoveShipLeft(SpaceShip &ship)
{
	ship.curFrame = 2;
	ship.x -= ship.speed/100.0;
	if(ship.x < 225)
		ship.x = 225;
}
void MoveShipRight(SpaceShip &ship)
{
	ship.curFrame = 1;
	ship.x += ship.speed/100.0;
	if(ship.x > WIDTH)
		ship.x = WIDTH;
}

void InitBullet(Bullet bullet[], int size)
{
	for(int i = 0; i < size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 15;
		bullet[i].live = false;
	}
}
void InitBossBullet(Bullet bullet[], int size)
{
	for(int i = 0; i < size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 5;
		bullet[i].live = false;
	}
}

void InitBarrier(Bullet bullet[], int size)
{
		barrier[0].ID = BULLET;
		barrier[0].speed = 40;
		barrier[0].live = false;
}
void DrawBullet(Bullet bullet[], int size)
{
	for( int i = 0; i < size; i++)
	{
		if(bullet[i].live)
			al_draw_bitmap(nbullet,bullet[i].x,bullet[i].y-3,0);
	}
}
void DrawBossBullet(Bullet bullet[], int size)
{
	for( int i = 0; i < size; i++)
	{
		if(bullet[i].live)
			al_draw_bitmap(bnbullet,bullet[i].x,bullet[i].y-3,0);
	}
}
void DrawBarrier(Bullet bullet[], int size)
{
	if(bimp>=3)
	{
		al_draw_bitmap(barrierimp,ship.x +10,ship.y -42,0); bimp++;
	}
	else if(bimp>=0)
	{
		al_draw_bitmap(barrierimp2,ship.x +10,ship.y -42,0); bimp++;
	}

	
	if(bimp>6) bimp=0;
}
void DrawBarrier2(Bullet bullet[], int size)
{
	if(bimp>=3)
	{
		al_draw_bitmap(barrierimp,barrier[0].x,barrier[0].y-42,0); bimp++;
	}
	else if(bimp>=0)
	{
		al_draw_bitmap(barrierimp2,barrier[0].x,barrier[0].y-42,0); bimp++;
	}
	if(bimp>6) bimp=0;
}
void FireBullet(Bullet bullet[], int size, SpaceShip &ship)
{
	final=clock();
	temp_cpu2 = ((double)final-(double)initial) / CLOCKS_PER_SEC ;
	//printf("%lf %lf %lf\n",temp_cpu,oldbullet,(double)ship.aspd);
	for( int i = 0; i < size; i++)
	{
		if(!bullet[i].live && temp_cpu2 - oldbullet >= (1.0 / (double)ship.aspd))
		{
			al_play_sample(natk,1,0,1,ALLEGRO_PLAYMODE_ONCE,&natkk);
			bullet[i].x = ship.x + 17;
			bullet[i].y = ship.y;
			bullet[i].live = true;
			oldbullet = temp_cpu2;
			break;
		}
	}
}
void FireBossBullet(Bullet bullet[], int size, Comet bosses[])
{
	final=clock();
	temp_cpu2 = ((double)final-(double)initial) / CLOCKS_PER_SEC ;
	for( int i = 0; i < size; i++)
	{
		if(!bullet[i].live && temp_cpu2 - oldbbullet >= (1.0 / (double)bosses[0].aspd))
		{
			bullet[i].x = bosses[0].x + 17;
			bullet[i].y = bosses[0].y - 5;
			bullet[i].live = true;
			oldbbullet = temp_cpu2;
			break;
		}
	}
}
void UpdateBullet(Bullet bullet[], int size)
{
	for(int i = 0; i < size; i++)
	{
		if(bullet[i].live)
		{
			bullet[i].x += bullet[i].speed;
			if(bullet[i].x > WIDTH)
				bullet[i].live = false;
		}
	}
}
void UpdateBossBullet(Bullet bullet[], int size)
{
	for(int i = 0; i < size; i++)
	{
		if(bullet[i].live)
		{
			bullet[i].x -= bullet[i].speed;
			if(bullet[i].x < 0)
				bullet[i].live = false;
		}
	}
}
void UpdateBarrier(Bullet bullet[], int size)
{

			barrier[0].x += barrier[0].speed;
			if(barrier[0].x > WIDTH)
			{
			barrier[0].live = false; skill=0;
			}

}
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize)
{
	for(int i = 0; i < bSize; i++)
	{
		if(bullet[i].live)
		{
			for(int j =0; j < cSize; j++)
			{
				if(comets[j].live)
				{
					if(bullet[i].x > (comets[j].x - comets[j].boundx) &&
						bullet[i].x < (comets[j].x + comets[j].boundx) &&
						bullet[i].y > (comets[j].y - comets[j].boundy) &&
						bullet[i].y < (comets[j].y + comets[j].boundy))
					{
						comets[j].hp-=ship.atk;
						if(comets[j].hp<=0) comets[j].live = false;
						bullet[i].live = false;

						if(rand()%3==0)
						ship.tomato+=50;

						StartExplosions(explosions, eSize, bullet[i].x, bullet[i].y);
					}
				}
			}
			for(int j =0; j < BOSSNUM; j++)
			{
				if(boss[j].live)
				{
					if(bullet[i].x > (boss[j].x - boss[j].boundx+70) &&
						bullet[i].x < (boss[j].x + boss[j].boundx+70) &&
						bullet[i].y > (boss[j].y - (boss[j].boundy-10)) &&
						bullet[i].y < (boss[j].y + (boss[j].boundy-10)))
					{
						boss[j].hp-=ship.atk-10;
						if(boss[j].hp<=0)
						{
							boss[j].live = false; end=1;
						}
						bullet[i].live = false;

						StartExplosions(explosions, eSize, bullet[i].x, bullet[i].y);
					}
				}
			}
		}

	}
}


void CollideBossBullet(Bullet bullet[], int bSize, SpaceShip &ship, Explosion explosions[], int eSize)
{
	for(int i = 0; i < bSize; i++)
	{
		if(bullet[i].live)
		{
					if(bullet[i].x > (ship.x - ship.boundx) &&
						bullet[i].x < (ship.x + ship.boundx) &&
						bullet[i].y > (ship.y - ship.boundy) &&
						bullet[i].y < (ship.y + ship.boundy))
					{

						if(damage==0)
						{
							ship.lives--;
							damage=1;
							dying=0;
							bullet[i].live = false;
							StartExplosions(explosions, eSize, bullet[i].x, bullet[i].y);
						}
					}
				
		}
	}
}

void CollideBarrier(Comet comets[], SpaceShip &ship, Explosion explosions[])
{
		if(barrier[0].live)
		{
			for(int j =0; j < NUM_COMETS; j++)
			{
				if(comets[j].live)
				{
					if(comets[j].x - comets[j].boundx < barrier[0].x + 15  &&
						comets[j].x + comets[j].boundx > barrier[0].x - 15 &&
						comets[j].y - comets[j].boundy < barrier[0].y + 40 &&
						comets[j].y + comets[j].boundy > barrier[0].y - 40)
					{
						comets[j].hp -=70;
						if(comets[j].hp <=0) comets[j].live = false;

						ship.tomato+=50;

						StartExplosions(explosions, NUM_EXPLOSIONS, comets[j].x, comets[j].y);
					}
				}
			}
			for(int j =0; j < BOSSNUM; j++)
			{
				if(boss[j].live)
				{
					if(boss[j].x - boss[j].boundx+75 < barrier[0].x + 15  &&
						boss[j].x + boss[j].boundx+75 > barrier[0].x - 15 &&
						boss[j].y - (boss[j].boundy-10) < barrier[0].y + 40 &&
						boss[j].y + (boss[j].boundy-10) > barrier[0].y - 40)
					{
						boss[j].hp -=50;
						if(boss[j].hp <=0)
						{
							boss[j].live = false; end=1;
						}

						StartExplosions(explosions, NUM_EXPLOSIONS, barrier[0].x, barrier[0].y);
					}
				}
			}
		}

}
void CollideRay(Comet comets[], SpaceShip &ship)
{

			for(int j =0; j < NUM_COMETS; j++)
			{
				if(comets[j].live)
				{
					if(	comets[j].x + comets[j].boundx > ship.x - 15 &&
						comets[j].y - comets[j].boundy < ship.y + 80 &&
						comets[j].y + comets[j].boundy > ship.y - 80)
					{
						comets[j].live = false;

						if(rand()%2==0)
						ship.tomato+=50;
						
						StartExplosions(explosions, NUM_EXPLOSIONS, comets[j].x, comets[j].y);
					}
				}
			}
			for(int j =0; j < BOSSNUM; j++)
			{
				if(boss[j].live)
				{
					if(	boss[j].y - boss[j].boundy < ship.y + 80 &&
						boss[j].y + boss[j].boundy > ship.y - 80)
					{
						boss[j].hp-=3;
						if(boss[j].hp==0)
						{
							boss[j].live = false;
							StartExplosions(explosions, NUM_EXPLOSIONS, comets[j].x, comets[j].y);
							end=1;
						}

					}
				}
			}
	

}

void InitComet(Comet comets[], int size, ALLEGRO_BITMAP *image)
{
	for(int i = 0; i < size; i++)
	{
		comets[i].ID = ENEMY;
		comets[i].live = false;
		comets[i].speed = 8;
		comets[i].boundx = 35;
		comets[i].boundy = 35;
		comets[i].hp = 50;

		comets[i].maxFrame = 143;
		comets[i].curFrame = 0;
		comets[i].frameCount = 0;
		comets[i].frameDelay = 2;
		comets[i].frameWidth = 96;
		comets[i].frameHeight = 96;
		comets[i].animationColumns = 21;

		if(rand() % 2)
			comets[i].animationDirection = 1;
		else
			comets[i].animationDirection = -1;

		comets[i].image = image;
	}
}
void DrawComet(Comet comets[], int size)
{
	for(int i = 0; i < size; i++)
	{
		if(comets[i].live)
		{
			int fx = (comets[i].curFrame % comets[i].animationColumns) * comets[i].frameWidth;
			int fy = (comets[i].curFrame / comets[i].animationColumns) * comets[i].frameHeight;

			al_draw_bitmap_region(comets[i].image, fx, fy, comets[i].frameWidth,
				comets[i].frameHeight, comets[i].x - comets[i].frameWidth / 2, comets[i].y - comets[i].frameHeight / 2, 0);

		}
	}
}
void DrawBoss(Comet bosses[], int id)
{
		if(boss[id].live)
		{
			al_draw_bitmap(bosses[id].image, bosses[id].x,bosses[id].y-boss[id].boundy, 0);

		}
}
void StartComet(Comet comets[], int size)
{
	int sizecomet,cometstate;
	if(mode==1) sizecomet=20;
	if(mode==2) sizecomet=50;
	if(mode==3) sizecomet=100;
	for(int i = 0; i < sizecomet; i++)
	{
		if(!comets[i].live)
		{
				if(rand() % 300 == 0)
				{
					cometstate=rand()%6;
					if(cometstate==1)
					{
						comets[i].live = true;
						comets[i].x = WIDTH;
						comets[i].y = 15+rand()%10;
					}
					else if(cometstate==2)
					{
						comets[i].live = true;
						comets[i].x = WIDTH;
						comets[i].y = HEIGHT-15-rand()%10;
					}
					else
					{
						comets[i].live = true;
						comets[i].x = WIDTH;
						comets[i].y = 30 + rand() % (HEIGHT - 60);

					}
					
					break;
				}	
		}
	}
}
void UpdateComet(Comet comets[], int size)
{
	for(int i = 0; i < size; i++)
	{
		if(comets[i].live)
		{
			if(++comets[i].frameCount >= comets[i].frameDelay)
			{
				comets[i].curFrame += comets[i].animationDirection;
				if(comets[i].curFrame >= comets[i].maxFrame)
					comets[i].curFrame = 0;
				else if( comets[i].curFrame <= 0)
					comets[i].curFrame = comets[i].maxFrame - 1;

				comets[i].frameCount = 0;
			}

			comets[i].x -= comets[i].speed;
		}
	}
}

void UpdateBoss(Comet bosses[], int size)
{
	for(int i = 0; i < size; i++)
	{
		if(bosses[i].live)
		{

			if(mov==1)
			{
				bosses[i].y-=5;
				if(bosses[i].y<=0)
				{
					bosses[i].y=0;
					mov=0;
				}
			}
			else if(mov==2)
			{
				bosses[i].y+=5;
				if(bosses[i].y>=HEIGHT-80)
				{
					bosses[i].y=HEIGHT-80;
					mov=0;
				}
			}
			else if(mov==3)
			{
				if(bosses[i].y<HEIGHT/2-80) bosses[i].y++;
				else if(bosses[i].y>HEIGHT/2-80) bosses[i].y--;
				else
				{
					volt=0;
					mov=0;
				}
			}
		}
	}
}

void CollideComet(Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize)
{
	for(int i = 0; i < cSize; i++)
	{
		if(comets[i].live)
		{
			if(comets[i].x - comets[i].boundx < ship.x + ship.boundx &&
				comets[i].x + comets[i].boundx > ship.x - ship.boundx &&
				comets[i].y - comets[i].boundy < ship.y + ship.boundy &&
				comets[i].y + comets[i].boundy > ship.y - ship.boundy)
			{
				if(damage==0)
				{
					ship.lives--;
					damage=1;
					dying=0;
					comets[i].live = false;
					StartExplosions(explosions, eSize, ship.x, ship.y);
				}
				
			}
			else if(comets[i].x < 0)
			{
				comets[i].live = false;
			}
		}
	}
}




void InitExplosions(Explosion explosions[], int size, ALLEGRO_BITMAP *image)
{
	for(int i = 0; i < size; i++)
	{
		explosions[i].live = false;

		explosions[i].maxFrame = 31;
		explosions[i].curFrame = 0;
		explosions[i].frameCount = 0;
		explosions[i].frameDelay = 1;
		explosions[i].frameWidth = 128;
		explosions[i].frameHeight = 128;
		explosions[i].animationColumns = 8;
		explosions[i].animationDirection = 1;

		explosions[i].image = image;
	}
}

void DrawExplosions(Explosion explosions[], int size)
{
	for(int i = 0; i < size; i++)
	{
		if(explosions[i].live)
		{
			int fx = (explosions[i].curFrame % explosions[i].animationColumns) * explosions[i].frameWidth;
			int fy = (explosions[i].curFrame / explosions[i].animationColumns) * explosions[i].frameHeight;

			al_draw_bitmap_region(explosions[i].image, fx, fy, explosions[i].frameWidth,
				explosions[i].frameHeight, explosions[i].x - explosions[i].frameWidth / 2, explosions[i].y - explosions[i].frameHeight / 2, 0);
		}
	}
}
void StartExplosions(Explosion explosions[], int size, int x, int y)
{
	for(int i = 0; i < size; i++)
	{
		if(!explosions[i].live)
		{
			al_play_sample(boom,1,0,1,ALLEGRO_PLAYMODE_ONCE,&boomm);
			explosions[i].live = true;
			explosions[i].x = x;
			explosions[i].y = y;
			break;
		}
	}
}
void UpdateExplosions(Explosion explosions[], int size)
{
	for(int i = 0; i < size; i++)
	{
		if(explosions[i].live)
		{
			if(++explosions[i].frameCount >= explosions[i].frameDelay)
			{
				explosions[i].curFrame += explosions[i].animationDirection;
				if(explosions[i].curFrame >= explosions[i].maxFrame)
				{
					explosions[i].curFrame = 0;
					explosions[i].live = false;
				}

				explosions[i].frameCount = 0;
			}
		}
	}
}
void InitBackground(Background &back, float x, float y, float velx, float vely, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image)
{
	back.x = x;
	back.y = y;
	back.velX = velx;
	back.velY = vely;
	back.width = width;
	back.height = height;
	back.dirX = dirX;
	back.dirY = dirY;
	back.image = image;
}
void UpdateBackground(Background &back)
{
	back.x += back.velX * back.dirX;
	if(back.x + back.width <= 0)
		back.x = 0;
}
void UpdateBackgroundYAXIS(Background &back)
{
	back.y += back.velY * back.dirY;
	if(back.y + back.height <= 0)
		back.y = 0;
}
void DrawBackground(Background &back)
{
	al_draw_bitmap(back.image, back.x, back.y, 0);

	if(back.x + back.width < WIDTH)
		al_draw_bitmap(back.image, back.x + back.width, back.y, 0);
}

void menuselect(void)
{
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_FONT *fbgoth = al_load_font("bgothm.ttf",20,0);
	ALLEGRO_FONT *fbgothb = al_load_font("bgothm.ttf",22,0);
	if(newgame)
	{
		if(cursor==1)
		al_draw_textf(fbgothb, al_map_rgb(255, 255, 255), 780, 300, ALLEGRO_ALIGN_RIGHT, "HARD MODE",1);
		else
		al_draw_textf(fbgoth, al_map_rgb(144, 144, 144), 780, 300, ALLEGRO_ALIGN_RIGHT, "HARD MODE",1);
		if(cursor==2)
		al_draw_textf(fbgothb, al_map_rgb(255, 255, 255), 780, 350, ALLEGRO_ALIGN_RIGHT, "EXPERT MODE",1);
		else

		al_draw_textf(fbgoth, al_map_rgb(144, 144, 144), 780, 350, ALLEGRO_ALIGN_RIGHT, "EXPERT MODE",1);
		if(cursor==3)
		al_draw_textf(fbgothb, al_map_rgb(255, 255, 255), 780, 400, ALLEGRO_ALIGN_RIGHT, "LUNATIC MODE",1);
		else
		al_draw_textf(fbgoth, al_map_rgb(144, 144, 144), 780, 400, ALLEGRO_ALIGN_RIGHT, "LUNATIC MODE",1);
		if(cursor==4)
		al_draw_textf(fbgothb, al_map_rgb(255, 255, 255), 780, 450, ALLEGRO_ALIGN_RIGHT, "BACK",1);
		else
		al_draw_textf(fbgoth, al_map_rgb(144, 144, 144), 780, 450, ALLEGRO_ALIGN_RIGHT, "BACK",1);
		al_destroy_font(fbgoth);
	}
	else
	{
		if(cursor==1)
		al_draw_textf(fbgothb, al_map_rgb(255, 255, 255), 780, 300, ALLEGRO_ALIGN_RIGHT, "NEW GAME",1);
		else
		al_draw_textf(fbgoth, al_map_rgb(144, 144, 144), 780, 300, ALLEGRO_ALIGN_RIGHT, "NEW GAME",1);
		if(cursor==2)
		al_draw_textf(fbgothb, al_map_rgb(255, 255, 255), 780, 350, ALLEGRO_ALIGN_RIGHT, "STAGE SELECT",1);
		else
		al_draw_textf(fbgoth, al_map_rgb(144, 144, 144), 780, 350, ALLEGRO_ALIGN_RIGHT, "STAGE SELECT",1);
		if(cursor==3)
		al_draw_textf(fbgothb, al_map_rgb(255, 255, 255), 780, 400, ALLEGRO_ALIGN_RIGHT, "CREDITS",1);
		else
		al_draw_textf(fbgoth, al_map_rgb(144, 144, 144), 780, 400, ALLEGRO_ALIGN_RIGHT, "CREDITS",1);
		if(cursor==4)
		al_draw_textf(fbgothb, al_map_rgb(255, 255, 255), 780, 450, ALLEGRO_ALIGN_RIGHT, "ENTER KEY",1);
		else
		al_draw_textf(fbgoth, al_map_rgb(144, 144, 144), 780, 450, ALLEGRO_ALIGN_RIGHT, "ENTER KEY",1);
		if(cursor==5)
		al_draw_textf(fbgothb, al_map_rgb(255, 255, 255), 780, 500, ALLEGRO_ALIGN_RIGHT, "MUSIC ARCHIVE",1);
		else
		al_draw_textf(fbgoth, al_map_rgb(144, 144, 144), 780, 500, ALLEGRO_ALIGN_RIGHT, "MUSIC ARCHIVE",1);
		if(cursor==6)
		al_draw_textf(fbgothb, al_map_rgb(255, 255, 255), 780, 550, ALLEGRO_ALIGN_RIGHT, "EXIT",1);
		else
		al_draw_textf(fbgoth, al_map_rgb(144, 144, 144), 780, 550, ALLEGRO_ALIGN_RIGHT, "EXIT",1);
		al_destroy_font(fbgoth); al_destroy_font(fbgothb);
	}
		
}
void ChangeState(int &state, int newState)
{
	int oldstate=state;
	
    if(state == MENU)
	{
		if(newState == PLAYING)
				al_stop_sample(&two);
	}
	else if(state == PLAYING)
	{
		ult=0;
	}
	else if(state == GAMEOVER)
	{

	}
	else if(state == MUSICBOX)
	{
		al_stop_sample(&player);
		fadein(casset,80);
	}
	else if(state == ENTERKEY)
	{
		fadein(enterkey2,80);
	}
	else if(state == CREDIT)
	{
		enter=0;
	}
	if(state == SELECT)
	{
	}

	state = newState;

	if(state == MENU)
	{

		fadeout(menu0, 80);
		al_stop_sample(&bgm);
		
		if(oldstate == PLAYING || oldstate == GAMEOVER) 
		al_play_sample(music[0].musiku,1,0,1,ALLEGRO_PLAYMODE_LOOP,&two);
		if(oldstate == MUSICBOX&&playsong==1)
		al_play_sample(music[0].musiku,1,0,1,ALLEGRO_PLAYMODE_LOOP,&two);

		cursor=1;
	}
	else if(state ==CREDIT)
	{
		
		InitBackground(credittextt, 0, 0, 0, 1, 800, 2500, 1, -1, credittext);
	}
	else if(state == PLAYING)
	{
		boss[0].live = false;
		bossinit=0;
		end=0;
		dying=-1;
		pause = 0;
		flaytime=0;
		cooldown=0;
		curmana=ship.maxmana;
		
		al_stop_sample(&two);
		keys[UP]=keys[DOWN]=keys[LEFT]=keys[RIGHT]=false;
		starttime=clock();
		al_play_sample(music[1].musiku,1,0,1,ALLEGRO_PLAYMODE_LOOP,&bgm);
		curbgm=1;
		InitShip(ship,shipImage);
		InitBullet(bullets, NUM_BULLETS);
		InitBarrier(barrier, 1);
		InitComet(comets, NUM_COMETS,cometImage);
		InitExplosions(explosions, NUM_EXPLOSIONS,expImage);

	}
	else if(state == GAMEOVER)
	{
		cursor=1;
		al_stop_sample(&bgm);
		al_play_sample(music[4].musiku,1,0,1,ALLEGRO_PLAYMODE_LOOP,&bgm);
		reggae = 0;
	}
	else if(state == MUSICBOX)
	{
		playsong=0;
		fadeout(casset, 80);
		cursor=1;
	}
	else if(state == ENTERKEY)
	{
		initkeycheat();
		cursor=1;
		top=0;
		strcpy(keybuffer,"                         ");
		fadeout(enterkey, 80);
	}

	else if(state == SELECT)
	{
		newgame=0;
		cursor=1;
	}
}
 
void fadeout(ALLEGRO_BITMAP *imagem, int velocity)
{
    if (velocity < 0)
    {
        velocity = 1;
    }
    else if (velocity > 15)
    {
        velocity = 15;
    }
 
    int alfa;
    for (alfa = 0; alfa <= 255; alfa += velocity)
    {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_tinted_bitmap(imagem, al_map_rgba(alfa, alfa, alfa, alfa), 0, 0, 0);
        al_flip_display();
        al_rest(0.005);
    }
}

void fadein(ALLEGRO_BITMAP *imagem, int velocity)
{
    if (velocity < 0)
    {
        velocity = 1;
    }
    else if (velocity > 15)
    {
        velocity = 15;
    }
 
    int alfa;
    for (alfa = 255; alfa >=0; alfa -= velocity)
    {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_tinted_bitmap(imagem, al_map_rgba(alfa, alfa, alfa, alfa), 0, 0, 0);
        al_flip_display();
        al_rest(0.005);
    }
}

void shopstate(void)
{
	ALLEGRO_BITMAP *curitem;

}

void listmusic(void)
{
	ALLEGRO_FONT *fbgoth = al_load_font("bgothm.ttf",22,0);
	ALLEGRO_FONT *fbgothb = al_load_font("bgothm.ttf",24,0);
	

	if(cassetcount>=30)
	{
			al_draw_bitmap(casset1,0,0,0); cassetcount++;
		}
	else if(cassetcount>=20)
	{
			al_draw_bitmap(casset,0,0,0); cassetcount++;
	}
	else if(cassetcount>=10)
	{
			al_draw_bitmap(casset1,0,0,0); cassetcount++;
	}
	else if(cassetcount>=0)
	{
			al_draw_bitmap(casset2,0,0,0); cassetcount++;
	}
	if(cassetcount==40) cassetcount=0;
	
	
	for(int k=0;k<NUMMUSIC&&k<=10;k++)
	{
		if(cursor==k+1)
		al_draw_bitmap(notes,20,100+30*k,0);
		if(music[k].unlock)
		al_draw_textf(fbgoth, al_map_rgb(255, 255, 255), 50 , 100+30*k , ALLEGRO_ALIGN_LEFT, "%s",music[k].name);
		else
		{
		al_draw_textf(fbgoth, al_map_rgb(144, 144, 144), 50 , 100+30*k , ALLEGRO_ALIGN_LEFT, "%s",music[k].condition);
		}
	}

	if(cursor==NUMMUSIC+1)
	al_draw_bitmap(notes,40,560,0);
	al_draw_textf(fbgothb, al_map_rgb(255, 255, 255), 70 , 560 , ALLEGRO_ALIGN_LEFT, "BACK",cursor);
	

	al_destroy_font(fbgoth);
	al_destroy_font(fbgothb);
}

void initmusic(void)
{
	natk = al_load_sample("natk.ogg");
	boom = al_load_sample("boom.ogg");
	nopes = al_load_sample("nope.ogg");
	barrierm = al_load_sample("barrier.ogg");
	ultt = al_load_sample("ray.ogg");
	music[0].musiku = al_load_sample("2 Minutes to Midnight.ogg");
	strcpy(music[0].name,"2 Minutes to Midnight");
	music[1].unlock = 1;
	music[1].musiku = al_load_sample("Carry On.ogg");
	strcpy(music[1].name,"Carry On");
	strcpy(music[1].condition,"Clear stage 1");
	music[2].unlock = 1;
	music[2].musiku = al_load_sample("The final frontier.ogg");
	strcpy(music[2].name,"The Final Frontier");
	strcpy(music[2].condition,"Clear stage 8");
	music[3].unlock = 0;
	music[3].musiku = al_load_sample("Rainmaker.ogg");
	strcpy(music[3].name,"Rainmaker");
	strcpy(music[3].condition,"Clear any stage without dying");
	music[4].unlock = 0;
	music[4].musiku = al_load_sample("Buffalo Soldier.ogg");
	strcpy(music[4].name,"Buffalo Soldier");
	strcpy(music[4].condition,"First Game Over");
	music[5].unlock = 0;
	music[5].musiku = al_load_sample("Radioactive.ogg");
	strcpy(music[5].name,"Radioactive");
	strcpy(music[5].condition,"UNLOCK KEY : RADIOPXXXXXX");
}

void superperfectverycoolkeyboardtype(ALLEGRO_EVENT typer)
{
	int ii,jj;
	if(top<=20)
	{
		if(typer.keyboard.keycode == ALLEGRO_KEY_A)	{	keybuffer[top] = 'A'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_B)	{	keybuffer[top] = 'B'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_C)	{	keybuffer[top] = 'C'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_D)	{	keybuffer[top] = 'D'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_E)	{	keybuffer[top] = 'E'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_F)	{	keybuffer[top] = 'F'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_G)	{	keybuffer[top] = 'G'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_H)	{	keybuffer[top] = 'H'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_I)	{	keybuffer[top] = 'I'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_J)	{	keybuffer[top] = 'J'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_K)	{	keybuffer[top] = 'K'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_L)	{	keybuffer[top] = 'L'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_M)	{	keybuffer[top] = 'M'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_N)	{	keybuffer[top] = 'N'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_O)	{	keybuffer[top] = 'O'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_P)	{	keybuffer[top] = 'P'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_Q)	{	keybuffer[top] = 'Q'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_R)	{	keybuffer[top] = 'R'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_S)	{	keybuffer[top] = 'S'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_T)	{	keybuffer[top] = 'T'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_U)	{	keybuffer[top] = 'U'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_V)	{	keybuffer[top] = 'V'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_W)	{	keybuffer[top] = 'W'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_X)	{	keybuffer[top] = 'X'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_Y)	{	keybuffer[top] = 'Y'; top++; }
		if(typer.keyboard.keycode == ALLEGRO_KEY_Z)	{	keybuffer[top] = 'Z'; top++; }
	
	}
	if(typer.keyboard.keycode == ALLEGRO_KEY_ENTER)
	{	
		if(cursor==1)
		{
			for(ii=0;ii<KEYCHEAT;ii++)
			{
				for(jj=0;jj<top;jj++)
				{
					if(keybuffer[jj] != keycheat[ii][jj])
						break;
				}
				if(jj==top)
				{
					activekeycheat(ii);
					break;
				}
			}

			strcpy(keybuffer,"                         "); top=0;
		}
		else
		{
			ChangeState(state,MENU);
		}

		
	}
	if(typer.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)	{	keybuffer[top-1] = ' '; top--; }
	if(top<0) top=0;
}

void activekeycheat (int id)
{
	if(id==0&&savebuffer[15]==0)
	{
		music[5].unlock=1;
		save(15,1); save(16,1);
		unlocker=0; unlockid=5; unlocktime=clock();
	}

}

void initkeycheat(void)
{
	keycheat[0][0]='R';keycheat[0][1]='A';keycheat[0][2]='D';keycheat[0][3]='I';keycheat[0][4]='O';keycheat[0][5]='P';keycheat[0][6]='A';keycheat[0][7]='S';keycheat[0][8]='S';keycheat[0][9]='I';keycheat[0][10]='V';keycheat[0][11]='E';
}

void save(int saveslot,int value)
{
	fp = fopen ("ohmygod.omg", "w");
	savebuffer[saveslot]=value;
	for(int jjj=0;jjj<SAVEBUFFER;jjj++)
	{
		fprintf(fp,"%d ",savebuffer[jjj]);
	}
	fclose(fp);
}

void loadsave(void)
{
	//MUSIC UNLOCK
	for(int jjj=10;jjj<10+NUMMUSIC;jjj++)
	{
		music[jjj-10].unlock = savebuffer[jjj];
	}
	//CHEAT UNLOCK
	for(int jjj=10+NUMMUSIC;jjj<10+NUMMUSIC+KEYCHEAT;jjj++)
	{
		if(savebuffer[jjj])
		activekeycheat(jjj-10+NUMMUSIC);
	}
}

void printunlockcheat(void)
{
	ALLEGRO_FONT *fcpub = al_load_font("CPU.ttf",34,0);
	if(savebuffer[15])
	{
			al_draw_textf(fcpub, al_map_rgb(255, 255, 255), 30, 210, 0, "UNLOCK MUSIC : RADIOACTIVE",666);
	}
	al_destroy_font(fcpub);
}

void archiveunlocked (int misc)
{
	al_draw_bitmap(archive,WIDTH-300,HEIGHT-140,0);
	ALLEGRO_FONT *fcpub = al_load_font("CPU.ttf",40,0);
	al_draw_textf(fcpub, al_map_rgb(255, 255, 255), WIDTH-285, HEIGHT-60, 0, "%s",music[misc].name);
	al_destroy_font(fcpub);
}

void skillbarrier(void)
{
	barrier[0].live=true;
	for(int i = 0; i < NUM_COMETS; i++)
	{
		if(comets[i].live)
		{
			if(comets[i].x - comets[i].boundx < ship.x + ship.boundx + 15 &&
				comets[i].x + comets[i].boundx > ship.x - ship.boundx + 15 &&
				comets[i].y - comets[i].boundy < ship.y + ship.boundy + 40 &&
				comets[i].y + comets[i].boundy > ship.y - ship.boundy - 40 )
			{
				comets[i].live = false;
				barrier[0].x = ship.x + 17;
				barrier[0].y = ship.y;
				skill=2;
				
				ship.tomato+=50;
				
				StartExplosions(explosions, NUM_EXPLOSIONS, comets[i].x, comets[i].y);

				return;
			}
		}
	}
	for(int j =0; j < 500; j++)
			{
				if(bossbullet[j].live)
				{
					if(bossbullet[j].x > (ship.x - ship.boundx+15) &&
						bossbullet[j].x < (ship.x + ship.boundx+15) &&
						bossbullet[j].y > (ship.y - ship.boundy-40) &&
						bossbullet[j].y < (ship.y + ship.boundy+40))
					{
						bossbullet[j].live = false;
						barrier[0].x = ship.x + 17;
						barrier[0].y = ship.y;
						skill=2;

						StartExplosions(explosions, NUM_EXPLOSIONS, bossbullet[j].x, bossbullet[j].y);
					}
				}
			}
}

void printpause (void)
{
	ALLEGRO_FONT *fcpub = al_load_font("CPU.ttf",40,0);
	if(cursor==1)
	al_draw_textf(fcpub, al_map_rgb(255, 255, 255), 491, 70, ALLEGRO_ALIGN_CENTRE, "RESUME",666);
	else
	al_draw_textf(fcpub, al_map_rgb(144, 144, 144), 491, 70, ALLEGRO_ALIGN_CENTRE, "RESUME",666);
	if(cursor==2)
	al_draw_textf(fcpub, al_map_rgb(255, 255, 255), 491, 100, ALLEGRO_ALIGN_CENTRE, "QUIT TO MENU",666);
	else
	al_draw_textf(fcpub, al_map_rgb(144, 144, 144), 491, 100, ALLEGRO_ALIGN_CENTRE, "QUIT TO MENU",666);
	if(cursor==3)
	al_draw_textf(fcpub, al_map_rgb(255, 255, 255), 491, 130, ALLEGRO_ALIGN_CENTRE, "QUIT GAME",666);
	else
	al_draw_textf(fcpub, al_map_rgb(144, 144, 144), 491, 130, ALLEGRO_ALIGN_CENTRE, "QUIT GAME",666);
	al_destroy_font(fcpub);
}

void printult (void)
{
	ult++;
	if(ult%3==0) type=(type+1)%2;
	if(type) al_draw_bitmap(ult2,ship.x+20,ship.y-124,0);
	else al_draw_bitmap(ult1,ship.x+20,ship.y-124,0);
}

void liststage (void)
{
	ALLEGRO_FONT *fcpub = al_load_font("CPU.ttf",40,0);
	ALLEGRO_FONT *fgoth = al_load_font("bgothm.ttf",24,0);
	al_draw_bitmap(selectt,0,0,0);
	if(newgame==1)
	{
		if(cursor==1)
		{
			al_draw_textf(fgoth, al_map_rgb(255, 255, 255), 20, 50, 0, "HARD MODE",666);
		}
		else al_draw_textf(fgoth, al_map_rgb(144, 144, 144), 20, 50, 0, "HARD MODE",666);
		if(cursor==2)
		{
			al_draw_textf(fgoth, al_map_rgb(255, 255, 255), 20, 110, 0, "EXPERT MODE",666);
		}
		else al_draw_textf(fgoth, al_map_rgb(144, 144, 144), 20, 110, 0, "EXPERT MODE",666);
		if(cursor==3)
		{
			al_draw_textf(fgoth, al_map_rgb(255, 255, 255), 20, 170, 0, "LUNATIC MODE",666);
		}
		else al_draw_textf(fgoth, al_map_rgb(144, 144, 144), 20, 170, 0, "LUNATIC MODE",666);
		if(cursor==4)
		{
			al_draw_textf(fgoth, al_map_rgb(255, 255, 255), 20, 230, 0, "BACK",666);
		}
		else al_draw_textf(fgoth, al_map_rgb(144, 144, 144), 20, 230, 0, "BACK",666);
		al_destroy_font(fcpub);
		al_destroy_font(fgoth);
		return;
	}
	if(cursor==1)
	{
		al_draw_bitmap(t0,0,40,0);
		al_draw_textf(fgoth, al_map_rgb(255, 255, 255), 20, 50, 0, "STAGE O : Past the edge of the Galaxy",666);
	}
	else al_draw_textf(fgoth, al_map_rgb(144, 144, 144), 20, 50, 0, "STAGE O : Past the edge of the Galaxy",666);
	if(cursor==2)
	{
		al_draw_bitmap(tt,0,100,0);
		al_draw_textf(fgoth, al_map_rgb(255, 255, 255), 20, 110, 0, "STAGE 1",666);
	}
	else al_draw_textf(fgoth, al_map_rgb(144, 144, 144), 20, 110, 0, "STAGE 1",666);
	if(cursor==3)
	{
		al_draw_bitmap(tt,0,160,0);
		al_draw_textf(fgoth, al_map_rgb(255, 255, 255), 20, 170, 0, "STAGE 2",666);
	}
	else al_draw_textf(fgoth, al_map_rgb(144, 144, 144), 20, 170, 0, "STAGE 2",666);
	if(cursor==4)
	{
		al_draw_textf(fcpub, al_map_rgb(255, 255, 255), 30, 550, 0, "BACK",666);
	}
	else
		al_draw_textf(fcpub, al_map_rgb(144, 144, 144), 30, 550, 0, "BACK",666);
	al_destroy_font(fcpub);
	al_destroy_font(fgoth);
}

