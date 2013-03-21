/* main.h */


#include <stdlib.h>
#include <sys/stat.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>


#define VERSION "0.1-1"

#define EVENT_CARDS 3

#define VIDEO_X 800
#define VIDEO_Y 600
#define VIDEO_BPP 16

#define DATA_DIR "data/"

#define TICK_INTERVAL 15

#define AQUARIUM_BOTTOM 40
#define AQUARIUM_TOP 50
#define AQUARIUM_SIDE 30

#define LEVEL1_TANK_FISH 7		/* 0 */
#define LEVEL2_TANK_FISH 15		/* 1 */
#define LEVEL3_TANK_FISH 22		/* 2 */

#define MAX_FISH LEVEL3_TANK_FISH

struct fish_definition {
	int width;
	int height;
	int price;
	int pollution;
	int feeding_costs;
	char name[50];
	char filename[50];
	SDL_Surface * fish_surface;
};

struct fish {
	unsigned int active;
	unsigned int type;
	unsigned int x;
	unsigned int y;
	unsigned int direction;
	unsigned int health;
	unsigned int age;
	unsigned int duration;
	unsigned int pollution;
};

#define NEONFISH_TYPE 0
#define NEONFISH_NAME "Neonfish"
#define NEONFISH_WIDTH 88
#define NEONFISH_HEIGHT 39
#define NEONFISH_PRICE 150
#define NEONFISH_POLLUTION 1
#define NEONFISH_FEEDING_COSTS 1
#define GOLDFISH_TYPE 1
#define GOLDFISH_NAME "Goldfish"
#define GOLDFISH_WIDTH 100
#define GOLDFISH_HEIGHT 46
#define GOLDFISH_PRICE 200
#define GOLDFISH_POLLUTION 1
#define GOLDFISH_FEEDING_COSTS 2

#define FILTERLIGHT_CLEANING 10
#define FILTERLIGHT_PRICE 700
#define FILTERLIGHT_TYPE 1

#define POLLUTION_TIMER 10000
/* events appear pollution_timer * event_multiplier */
/* should be a multiple of 2 */
#define EVENT_MULTIPLIER 10

/* 5 per cent of LOAN */
#define INTEREST_LOAN 10
/* start capital */
#define LOAN 1000


