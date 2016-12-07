//objects

enum IDS {PLAYER, BULLET, ENEMY , BOSS};
enum SKILLS {BARRIER,SMALLRAY};

//player

struct SpaceShip {
	int ID;
	int x;
	int y;
	int lives;
	int mana;
	int maxmana;
	int boundx;
	int boundy;
	int tomato;
	
	int atk;
	int spell;
	double aspd;
	int speed;

	int skill;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	int animationRow;

	ALLEGRO_BITMAP *image;
};

struct Bullet {
	int ID;
	int x;
	int y;
	bool live;
	int speed;

};

struct Comet {
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;
	int hp;
	double aspd;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	ALLEGRO_BITMAP *image;
};

struct Explosion {
	int x;
	int y;
	bool live;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	ALLEGRO_BITMAP *image;
};

struct Background
{
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;

	int width;
	int height;

	ALLEGRO_BITMAP *image;
};

struct Music
{
	int unlock;
	char name[100];
	char condition[100];

	ALLEGRO_SAMPLE *musiku;
};