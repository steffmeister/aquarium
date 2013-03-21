/* main.c */

#include "main.h"

SDL_Surface *screen;
SDL_Surface *aquarium;
SDL_Surface *board;
SDL_Surface *filter_light;

SDL_Surface *temp_surface;

SDL_Surface *board_tiles;

Uint32 transparent_color = 0;

SDL_Event event;

Uint32 flags = SDL_HWSURFACE;

char temp_string[255];

TTF_Font *hud_font;

SDL_Color yellow =	{255,255,0};
SDL_Color white =	{255,255,255};
SDL_Color red =		{255,0,0};

int money = LOAN;
int active_loan = LOAN + (LOAN/5);
int pollution = 0;

unsigned int shop_tab = 0;
// 0 ... fish
// 1 ... filters

unsigned int filter_type = 0;

unsigned int callback_disable = 0;

unsigned int total_fishes = 0;
unsigned int feeding_costs = 0;
int event_multiplier = 0;

unsigned int main_quit = 0;

/* we discard the very first event card */
int first_event_card = 1;
/* has event card been handled? */
int discard_event_card = 0;

/* fish tank level */
int fish_tank_level = 0;

int mouse_x;
int mouse_y;

char data_string[255];

struct fish fish_array[MAX_FISH];

struct fish_definition fish_defs[3];

Uint32 TimeLeft(void) {
    static Uint32 next_time = 0;
    Uint32 now, ret = 0;
    
    now = SDL_GetTicks();
    if (next_time <= now) {
        next_time = now + TICK_INTERVAL;
        return(ret);
    }
    ret = next_time - now;
    return(ret);
}

void clear_fish_arry(void) {
	unsigned int counter_fish = 0;
	while(counter_fish < (MAX_FISH)) {
		fish_array[counter_fish].active = 0;
		counter_fish++;
	}
}

void add_random_fish(int fish_type) {
	unsigned int counter_fish = 0;
	while ((counter_fish < MAX_FISH) && (fish_array[counter_fish].active != 0)) {
		counter_fish++;	
	}
	if (counter_fish < MAX_FISH) {
		fish_array[counter_fish].active = 1;
		fish_array[counter_fish].type = fish_type;
		fish_array[counter_fish].x = rand()%VIDEO_X;
		fish_array[counter_fish].y = rand()%VIDEO_Y;
		fish_array[counter_fish].health = 100;
		fish_array[counter_fish].age = 0;
		fish_array[counter_fish].direction = rand()%11;
		fish_array[counter_fish].duration = 0;
		
		fish_array[counter_fish].pollution = fish_defs[fish_array[counter_fish].type].pollution;
		if (fish_array[counter_fish].x > (VIDEO_X-30-fish_defs[fish_array[counter_fish].type].width)) fish_array[counter_fish].x = VIDEO_X-30-fish_defs[fish_array[counter_fish].type].width;
		if (fish_array[counter_fish].y > (VIDEO_Y-30-fish_defs[fish_array[counter_fish].type].height)) fish_array[counter_fish].y = VIDEO_Y-30-fish_defs[fish_array[counter_fish].type].height;
				
		if (fish_array[counter_fish].x < 30) fish_array[counter_fish].x = 30;
		if (fish_array[counter_fish].y < 30) fish_array[counter_fish].y = 30;
	}
}

/* populate fish definitions */
void populate_fish_defs(void) {
	/* NEONFISH */
	fish_defs[0].width = 88;
	fish_defs[0].height = 39;
	fish_defs[0].price = 150;
	fish_defs[0].pollution = 1;
	fish_defs[0].feeding_costs = 1;
	strcpy(fish_defs[0].name, "Neonfish");
	strcpy(fish_defs[0].filename, "neonfish.gif");
	/* GOLDFISH */
	fish_defs[1].width = 100;
	fish_defs[1].height = 46;
	fish_defs[1].price = 200;
	fish_defs[1].pollution = 1;
	fish_defs[1].feeding_costs = 2;
	strcpy(fish_defs[1].name, "Goldfish");
	strcpy(fish_defs[1].filename, "goldfish.gif");
}


/* get total amount of fishes */
unsigned int count_fishes(void) {
	unsigned int counter_fish = 0;
	unsigned int total_fish = 0;
	while (counter_fish < MAX_FISH) {
		if (fish_array[counter_fish].active == 1) total_fish++;
		counter_fish++;
	}
	return total_fish;
}

/* create a board with borders */
void draw_board(int width_t, int height_t, char *title) {
	int width_c = 0;
	int height_c = 0;
	int x_offset = VIDEO_X/2 - (width_t*32)/2;
	int y_offset = 100;
	SDL_Rect source;
	SDL_Rect dest;
	source.w = 32;
	source.h = 32;
	dest.w = 32;
	dest.h = 32;
	dest.x = x_offset;
	dest.y = y_offset;
	while(height_c <= height_t) {
		
		source.x = 32;
		source.y = 32;
		
		dest.x = x_offset + width_c*32;
		dest.y = y_offset + height_c*32;
		
		// upper left corner
		if ((height_c == 0) && (width_c == 0)) {
			source.x = 0;
			source.y = 0;
		/* lower left corner */
		} else if ((height_c == height_t) && (width_c == 0)) {
			source.x = 0;
			source.y = 64;
		/* upper right corner */
		} else if ((width_c == width_t) && (height_c == 0)) {
			source.x = 64;
			source.y = 0;
		/* lower right corner */
		} else if ((width_c == width_t) && (height_c == height_t)) {
			source.x = 64;
			source.y = 64;
		// upper border
		} else if (height_c == 0) {
			source.x = 32;
			source.y = 0;
		// right border
		} else if (width_c == width_t) {
			source.x = 64;
			source.y = 32;
		// left border
		} else if (width_c == 0) {
			source.x = 0;
			source.y = 32;
		/* lower border */
		} else if (height_c == height_t) {
			source.x = 32;
			source.y = 64;
		}
		
		SDL_BlitSurface(board_tiles, &source, screen, &dest);
		
		width_c++;
		if (width_c > width_t) {
			width_c = 0;
			height_c++;
		}
	}
	dest.x = x_offset + 16;
	dest.y = y_offset + 16;
	dest.w = 0;
	dest.h = 0;
	sprintf(temp_string, "%s", title);
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &dest);
}

void update_hud(void) {
	SDL_Rect buy_rect;
	buy_rect.x = 10;
	buy_rect.y = 570;
	sprintf(temp_string, "Shop");
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &buy_rect);
	buy_rect.x += 60;
	sprintf(temp_string, "Fishes");
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &buy_rect);
	buy_rect.x += 60;
	sprintf(temp_string, "Loan");
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &buy_rect);
	buy_rect.x += 60;
	sprintf(temp_string, "$ %d", money);
	if (money < 0) {
		temp_surface = TTF_RenderText_Solid(hud_font, temp_string, red);
	} else {
		temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	}
	SDL_BlitSurface(temp_surface, NULL, screen, &buy_rect);
	buy_rect.x += 200;
	sprintf(temp_string, "Pollution: %d%%", pollution);
	if (pollution > 60) {
		temp_surface = TTF_RenderText_Solid(hud_font, temp_string, red);
	} else {
		temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	}
	SDL_BlitSurface(temp_surface, NULL, screen, &buy_rect);
}

/* get the value of the fish */
unsigned int get_fish_value(unsigned int fish_id) {
	unsigned int value = (fish_defs[fish_array[fish_id].type].price / 2) - (100 - fish_array[fish_id].health) - fish_array[fish_id].age;
	printf("fish id %d, value: %d\n", fish_id, value);
	return value;
}


/* take / repay loan dialog */
void open_loan_dialog(void) {
	SDL_Rect board_r;
	SDL_Rect source_r;
	unsigned int quit = 0;

	callback_disable = 1;
	
	board_r.x = 260;
	board_r.y = 150;
	
	draw_board(10, 4, "ye olde loan dialog");

	sprintf(temp_string, "Current loan: $ %d", active_loan);
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
	
	board_r.y += 50;
	
	sprintf(temp_string, "Pay back $ 200");
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
	
	SDL_Flip(screen);
	
	while(!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_F3:
					case SDLK_ESCAPE:
						quit = 1;
					break;
				}
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_x, &mouse_y);
				if ((mouse_x >= 130) && (mouse_x <= 165) &&
					(mouse_y >= 570) && (mouse_y <= 590)) {
					quit = 1;
				} else if ((mouse_x >= 260) && (mouse_x <= 380) &&
							(mouse_y >= 200) && (mouse_y <= 220)) {
					if ((money >= 200) && (active_loan >= 200)) {
						money -= 200;
						active_loan -= 200;
					}
				}
				printf("mouse_x %d mouse_y %d", mouse_x, mouse_y);
			}
		}
		SDL_Delay(TimeLeft());
	}
	callback_disable = 0;	
}


/* fish list */
void open_fish_table(void) {
	SDL_Rect board_r;
	SDL_Rect source_r;
	unsigned int page = 0;
	unsigned int quit = 0;
	unsigned int redraw_shop = 1;
	unsigned int fish_list_counter = 0;
	unsigned int fish_counter = 0;
	unsigned int max_fishes = 0;
	unsigned int fish_amount = 0;
	int activated_fishes[MAX_FISH];
	char fish_name[50];
	int current_fishes[3];
	int value = 0;
	
	callback_disable = 1;
	
	
	
	while(!quit) {
		if (redraw_shop) {
		
			// reset and fill activated_fishes
			fish_counter = 0;
			fish_list_counter = 0;
			while (fish_counter < MAX_FISH) {
				activated_fishes[fish_counter] = -1;
				if (fish_array[fish_counter].active) {
					activated_fishes[fish_list_counter] = fish_counter;
					fish_list_counter++;
				}	
				fish_counter++;
			}
			// fish_list_counter contains now the amount of fishes?
			fish_amount = fish_list_counter;
			printf("fish amount %d\n", fish_amount);
			fish_counter = 0;
			fish_list_counter = 0;
	

			draw_board(19, 12, "");
			board_r.x = 130;
			board_r.y = 120;
			sprintf(temp_string, "fishy table (total %d fishes)", fish_amount);
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			
			
			board_r.y = 100;
			//board_r.x = 140;
			
			fish_counter = page*3;
			fish_list_counter = 0;
			
			// reset current fishes array
			current_fishes[0] = -1;
			current_fishes[1] = -1;
			current_fishes[2] = -1;
			
			//printf("page %d.\n", page);
			while((fish_list_counter < 3) && (fish_counter < fish_amount)) {
				if (activated_fishes[fish_counter] != -1) {
				
					//current_fishes[fish_list_counter] = fish_counter;
					current_fishes[fish_list_counter] = activated_fishes[fish_counter];
				
					board_r.y += 60;
				
					value = get_fish_value(activated_fishes[fish_counter]);
					if (value < 0) value = 0;
				
					source_r.x = 0;
					source_r.y = 0;
					board_r.x = 140;
					
					sprintf(fish_name, "%s", fish_defs[fish_array[activated_fishes[fish_counter]].type].name);
					source_r.w = fish_defs[fish_array[activated_fishes[fish_counter]].type].width;
					source_r.h = fish_defs[fish_array[activated_fishes[fish_counter]].type].height;
					
					SDL_BlitSurface(fish_defs[fish_array[activated_fishes[fish_counter]].type].fish_surface, &source_r, screen, &board_r);
					
					// show type
					board_r.x = 260;
					sprintf(temp_string, "%s", fish_name);
					temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
					SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
					
					// show id
					board_r.y += 20;
					sprintf(temp_string, "Id: %d", activated_fishes[fish_counter]);
					temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
					SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
					
					// show type
					board_r.y += 20;
					sprintf(temp_string, "Type: %d", fish_array[activated_fishes[fish_counter]].type);
					temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
					SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
					
					// show health
					board_r.y -= 40;
					board_r.x += 80;
					sprintf(temp_string, "Health: %d", fish_array[activated_fishes[fish_counter]].health);
					temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
					SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
					
					// show age
					board_r.y += 20;
					sprintf(temp_string, "Age: %d", fish_array[activated_fishes[fish_counter]].age);
					temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
					SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
					
					// show value
					board_r.y += 20;
 
					sprintf(temp_string, "Value: %d", value);
					temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
					SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
					
					// show value
					//board_r.y -= 40;
					board_r.x += 120;
					sprintf(temp_string, "Sell");
					temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
					SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
					fish_list_counter++;
				}
				fish_counter++;
			}
			
			board_r.x = 130;
			board_r.y = 470;
			sprintf(temp_string, "Previous");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			board_r.x += 80;
			sprintf(temp_string, "Next");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			
			SDL_Flip(screen);
			redraw_shop = 0;
		}
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_LEFT:
						if (page > 0) {
							page--;
							redraw_shop = 1;
						}
					break;
					case SDLK_RIGHT:
						if (page < (MAX_FISH/3)) {
							page++;
							redraw_shop = 1;
						}
					break;
					case SDLK_F2:
					case SDLK_ESCAPE:
						quit = 1;
					break;
				}
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_x, &mouse_y);
				if (event.button.button == SDL_BUTTON_LEFT) {
					// Previous
					if ((mouse_x >= 130) && (mouse_x <= 190) &&
						(mouse_y >= 470) && (mouse_y <= 490)) {
						if (page > 0) {
							page--;
							redraw_shop = 1;
						}
					// Next
					} else if ((mouse_x >= 210) && (mouse_x <= 250) &&
								(mouse_y >= 470) && (mouse_y <= 490)) {
						if (page < (MAX_FISH/3)) {
							page++;
							redraw_shop = 1;
						}
					} else if ((mouse_x >= 70) && (mouse_x <= 115) &&
							(mouse_y >= 570) && (mouse_y <= 590)) {
						quit = 1;
					// sell first displayed fish
					} else if ((mouse_x >= 460) && (mouse_x <= 485) &&
								(mouse_y >= 200) && (mouse_y <= 215)) {
						if (current_fishes[0] > -1) {
							printf("selling first fish...%d id %d\n", get_fish_value(current_fishes[0]), current_fishes[0]);
							fish_array[current_fishes[0]].active = 0;
							money += get_fish_value(current_fishes[0]);
							redraw_shop = 1;
						} else {
							printf("There is no first fish!\n");
						}
					} else if ((mouse_x >= 460) && (mouse_x <= 485) &&
								(mouse_y >= 300) && (mouse_y <= 315)) {
						if (current_fishes[1] > -1) {
							printf("selling second fish...%d id %d\n", get_fish_value(current_fishes[1]), current_fishes[1]);
							fish_array[current_fishes[1]].active = 0;
							money += get_fish_value(current_fishes[1]);
							redraw_shop = 1;
						} else {
							printf("There is no second fish!\n");
						}
					} else if ((mouse_x >= 460) && (mouse_x <= 485) &&
								(mouse_y >= 400) && (mouse_y <= 415)) {
						if (current_fishes[2] > -1) {
							printf("selling third fish...%d id %d\n", get_fish_value(current_fishes[2]), current_fishes[2]);
							fish_array[current_fishes[2]].active = 0;
							money += get_fish_value(current_fishes[2]);
							redraw_shop = 1;
						} else {
							printf("There is no third fish!\n");
						}
					} else {
						printf("x %d y %d\n", mouse_x, mouse_y);
					}
				}
			}
		}
		SDL_Delay(TimeLeft());
	}
	callback_disable = 0;
}

void open_shop(void) {
	SDL_Rect board_r;
	SDL_Rect source_r;
	unsigned int quit = 0;
	unsigned int redraw_shop = 1;

	callback_disable = 1;

	while(!quit) {
	
		if (redraw_shop) {
			board_r.x = 100;
			board_r.y = 100;
			draw_board(19, 12, "");
			board_r.x = 130;
			board_r.y = 120;


			// decorations
			if (shop_tab == 3) {
				sprintf(temp_string, "ye old upgrades shop");
				temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
				SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
				board_r.x = 120;
				board_r.y = 200;
				if (fish_tank_level == 0) {
					sprintf(temp_string, "Upgrade to level 2 fish tank: $ 1000");
				} else if (fish_tank_level == 1) {
					sprintf(temp_string, "Upgrade to level 3 fish tank: $ 5000");
				} else {
					sprintf(temp_string, "You reached the max level.");
				}
				temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
				SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
				board_r.y += 40;
				if (fish_tank_level == 0) {
					sprintf(temp_string, "Allows you to have %d fish instead of only %d.", LEVEL2_TANK_FISH, LEVEL1_TANK_FISH);
				} else if (fish_tank_level == 1) {
					sprintf(temp_string, "Allows you to have %d fish instead of only %d.", LEVEL3_TANK_FISH, LEVEL2_TANK_FISH);
				}
				printf("mouse_x %d mouse_y %d", mouse_x, mouse_y);
				temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
				SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			} else if (shop_tab == 2) {
				sprintf(temp_string, "ye old decoration shop");
				temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
				SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			// filters
			} else if (shop_tab == 1) {
				sprintf(temp_string, "ye old filters shop");
				temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
				SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
				board_r.x = 120;
				board_r.y = 200;
				sprintf(temp_string, "fishy filter light, %d$", FILTERLIGHT_PRICE);
				temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
				SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			// fishes
			} else {

				sprintf(temp_string, "ye old fishy shop");
				temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
				SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
	
				// list fish
				board_r.x = 120;
				board_r.y = 180;
				board_r.w = 88;
				board_r.h = 39;
				source_r.x = 0;
				source_r.y = 0;
				source_r.w = 88;
				source_r.h = 39;
				SDL_BlitSurface(fish_defs[0].fish_surface, &source_r, screen, &board_r);
				board_r.w = 0;
				board_r.h = 0;
				board_r.x = 230;
				board_r.y = 180;
				sprintf(temp_string, "%d$", NEONFISH_PRICE);
				temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
				SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			//	board_r.x = 230;
				board_r.y += 25;
				sprintf(temp_string, "Buy");
				temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
				SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
				
				// list fish
				board_r.x = 120;
				board_r.y = 250;
				board_r.w = 100;
				board_r.h = 46;
				source_r.x = 0;
				source_r.y = 0;
				source_r.w = 100;
				source_r.h = 46;
				SDL_BlitSurface(fish_defs[1].fish_surface, &source_r, screen, &board_r);
				board_r.w = 0;
				board_r.h = 0;
				board_r.x = 230;
				board_r.y = 250;
				sprintf(temp_string, "%d$", GOLDFISH_PRICE);
				temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
				SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			//	board_r.x = 230;
				board_r.y += 25;
				sprintf(temp_string, "Buy");
				temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
				SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
				
			}
			
			board_r.x = 130;
			board_r.y = 470;
			sprintf(temp_string, "Fish");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			board_r.x += 80;
			sprintf(temp_string, "Filters");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			board_r.x += 80;
			sprintf(temp_string, "Decorations");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			board_r.x += 130;
			sprintf(temp_string, "Upgrades");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			redraw_shop = 0;
			SDL_Flip(screen);
		}
	
	
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_F1:
					case SDLK_ESCAPE:
						quit = 1;
					break;
				}
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_x, &mouse_y);
				if (event.button.button == SDL_BUTTON_LEFT) {
					if ((mouse_x >= 230) && (mouse_x <= 250) &&
						(mouse_y >= 205) && (mouse_y <= 225) && (shop_tab == 0)) {
						if ((money - NEONFISH_PRICE) >= 0) {
							money -= NEONFISH_PRICE;
							add_random_fish(0);
							printf("add_random_fish(neon)\n");
						}
					} else if ((mouse_x >= 230) && (mouse_x <= 250) &&
								(mouse_y >= 275) && (mouse_y <= 295) && (shop_tab == 0)) {
						if ((money - GOLDFISH_PRICE) >= 0) {
							money -= GOLDFISH_PRICE;
							add_random_fish(1);
							printf("add_random_fish(gold)\n");
						}
					} else if ((mouse_x >= 120) && (mouse_x <= 280) &&
								(mouse_y >= 200) && (mouse_y <= 220) && (shop_tab == 1)) {
						if ((money - FILTERLIGHT_PRICE) >= 0) {
							money -= FILTERLIGHT_PRICE;
							filter_type = FILTERLIGHT_TYPE;
						}
					// fish tab
					} else if ((mouse_x >= 130) && (mouse_x <= 160) &&
						(mouse_y >= 470) && (mouse_y <= 490)) {
						if (shop_tab != 0) {
							shop_tab = 0;
							redraw_shop = 1;
						}
					// filters tab
					} else if ((mouse_x >= 210) && (mouse_x <= 260) &&
							(mouse_y >= 470) && (mouse_y <= 490)) {
						if (shop_tab != 1) {
							shop_tab = 1;
							redraw_shop = 1;
						}
					// decorations tab
					} else if ((mouse_x >= 290) && (mouse_x <= 350) &&
						(mouse_y >= 470) && (mouse_y <= 490)) {
						if (shop_tab != 2) {
							shop_tab = 2;
							redraw_shop = 1;
						}
					// upgrades tab
					} else if ((mouse_x >= 420) && (mouse_x <= 480) &&
						(mouse_y >= 470) && (mouse_y <= 490)) {
						if (shop_tab != 2) {
							shop_tab = 3;
							redraw_shop = 1;
						}
					} else if ((mouse_x >= 10) && (mouse_x <= 50) &&
						(mouse_y >= 570) && (mouse_y <= 590)) {
						quit = 1;
					} else {
						printf("x %d y %d\n", mouse_x, mouse_y);
					}
				}
			}
		}
		SDL_Delay(TimeLeft());
	}
	callback_disable = 0;
}

void open_event_card(unsigned int card) {
	SDL_Rect board_r;
	SDL_Rect source_r;
	unsigned int quit = 0;

	callback_disable = 1;
	board_r.x = 310;
	board_r.y = 130;
	draw_board(10, 5, "ye olde event card");

	switch(card) {
		case 0:
			board_r.y += 30;
			sprintf(temp_string, "While shopping for fish food you");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			board_r.y += 20;
			sprintf(temp_string, "were parking in a no parking area.");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			board_r.y += 20;
			sprintf(temp_string, "$ 20 fine.");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			money -= 20;
		break;
		case 1:
			board_r.y += 30;
			sprintf(temp_string, "You spare 10 $ from your");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			board_r.y += 20;
			sprintf(temp_string, "unemployment benefits");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			board_r.y += 20;
			sprintf(temp_string, "unexpectedly.");
			temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
			SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
			money += 10;
		break;
	}
	
	SDL_Flip(screen);
	
	while(!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = 1;
					break;
				}
			}
		}
		SDL_Delay(TimeLeft());
	}
	callback_disable = 0;	
}

void open_budget(unsigned total_fishes, unsigned int feeding_costs) {
	SDL_Rect board_r;
	SDL_Rect source_r;
	unsigned int quit = 0;

	callback_disable = 1;
	board_r.x = 260;
	board_r.y = 130;

	draw_board(10, 5, "ye olde budget dialog");
	
	board_r.y += 30;
	sprintf(temp_string, "Loan payments: $ %d", active_loan/INTEREST_LOAN);
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
	
	board_r.y += 20;
	sprintf(temp_string, "Feeding fees: $ %d", feeding_costs);
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);

	board_r.y += 30;
	sprintf(temp_string, "Cash: %d$ - %d$ = %d$", money, (feeding_costs + (active_loan/INTEREST_LOAN)), money - feeding_costs - (active_loan/INTEREST_LOAN));
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
	
	money = money - feeding_costs - (active_loan/INTEREST_LOAN);
	
	SDL_Flip(screen);
	
	while(!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_F5:
					case SDLK_ESCAPE:
						quit = 1;
					break;
				}
			}
		}
		SDL_Delay(TimeLeft());
	}
	callback_disable = 0;
}

void load_game(void) {
	fprintf(stderr, "load_game()\n");
}

void save_game(void) {
	fprintf(stderr, "save_game()\n");
}

void new_game(void) {
	fprintf(stderr, "new_game()\n");
	SDL_Rect board_r;
	SDL_Rect source_r;
	unsigned int quit = 0;

	callback_disable = 1;
	
	board_r.x = 200;
	board_r.y = 110;
	
	draw_board(14, 9, "new game");

	board_r.y += 35;
	sprintf(temp_string, "some fascinating story");
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
	
	board_r.y += 235;
	sprintf(temp_string, "accept challenge");
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
	
	SDL_Flip(screen);
	
	while(!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_SPACE:
					case SDLK_ESCAPE:
						quit = 1;
					break;
				}
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_x, &mouse_y);
				fprintf(stderr, "mouse_x %d mouse_y %d\n", mouse_x, mouse_y);
				if ((mouse_x >= 200) && (mouse_x <= 330) &&
					(mouse_y >= 383) && (mouse_y <= 396)) {
					quit = 1;
				}
			}
		}
		SDL_Delay(TimeLeft());
	}
	callback_disable = 0;
	
	/* reset everything */
	money = LOAN;
	active_loan = LOAN + (LOAN/5);
	pollution = 0;
	clear_fish_arry();
}

void open_main_menue(int not_in_game) {
	SDL_Rect board_r;
	SDL_Rect source_r;
	unsigned int quit = 0;

	callback_disable = 1;
	
	board_r.x = 310;
	board_r.y = 110;
	
	draw_board(7, 9, "ye olde main menue");

	board_r.y += 60;
	if (!not_in_game) {
		sprintf(temp_string, "Continue");
		temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
		SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
	}

	board_r.y += 25;
	sprintf(temp_string, "New Game");
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
	
	board_r.y += 25;
	sprintf(temp_string, "Load Game");
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);

	board_r.y += 25;	
	if (!not_in_game) {
		sprintf(temp_string, "Save Game");
		temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
		SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
	}
	
	board_r.y += 25;
/*	sprintf(temp_string, "Options");
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);*/
	
	board_r.y += 25;
	sprintf(temp_string, "Quit Game");
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);

	board_r.y += 60;
	sprintf(temp_string, "Version %s", VERSION);
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);

	board_r.y += 25;
	sprintf(temp_string, "by steffmeister software", VERSION);
	temp_surface = TTF_RenderText_Solid(hud_font, temp_string, yellow);
	SDL_BlitSurface(temp_surface, NULL, screen, &board_r);
	
	SDL_Flip(screen);
	
	while(!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_F10:
					case SDLK_ESCAPE:
						quit = 1;
					break;
				}
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_x, &mouse_y);
				fprintf(stderr, "mouse_x %d mouse_y %d\n", mouse_x, mouse_y);
				/* quit game */
				if ((mouse_x >= 310) && (mouse_x <= 400) &&
					(mouse_y >= 298) && (mouse_y <= 310)) {
					quit = 1;
					main_quit = 1;
				/* continue */
				} else if ((mouse_x >= 310) && (mouse_x <= 400) &&
							(mouse_y >= 170) && (mouse_y <= 186) && (!not_in_game)) {
					quit = 1;
				/* new game */
				} else if ((mouse_x >= 310) && (mouse_x <= 400) &&
							(mouse_y >= 196) && (mouse_y <= 210)) {
					new_game();
				/* load game */
				} else if ((mouse_x >= 310) && (mouse_x <= 400) &&
							(mouse_y >= 220) && (mouse_y <= 235)) {
					load_game();
				/* save game */
				} else if ((mouse_x >= 310) && (mouse_x <= 400) &&
							(mouse_y >= 247) && (mouse_y <= 261) && (!not_in_game)) {
					save_game();
				}
			}
		}
		SDL_Delay(TimeLeft());
	}
	callback_disable = 0;
}

void fish_die(unsigned int id) {
	fish_array[id].active = 0;
	fish_array[id].type = 0;
}

/* pollution timer 10s */
Uint32 pollution_timer(Uint32 interval, void *param) {
	unsigned int fish_counter = 0;
	unsigned int goldfish_counter = 0;
	unsigned int neonfish_counter = 0;
	
	if (!callback_disable) {
	
		total_fishes = 0;
		feeding_costs = 0;
	
		// filter work
		switch(filter_type) {
			case FILTERLIGHT_TYPE:
				pollution -= FILTERLIGHT_CLEANING;
			break;
			default:
			break;
		}
		if (pollution < 0) pollution = 0;
	
		// fish work
		while(fish_counter < MAX_FISH) {
			if (fish_array[fish_counter].active == 1) {
			
				pollution += fish_array[fish_counter].pollution;
			
				fish_array[fish_counter].age++;			
			
				if (pollution > 100) pollution = 100;
	
				if (pollution > 60) {
					fish_array[fish_counter].health -= 1;
				} else if (pollution > 65) {
					fish_array[fish_counter].health -= 2;
				} else if (pollution > 70) {
					fish_array[fish_counter].health -= 5;
				} else if (pollution > 90) {
					fish_array[fish_counter].health -= 7;
				}
				if ((fish_array[fish_counter].health < 1) ||
					((fish_array[fish_counter].age > 100) && (rand()%4))) {
					fish_die(fish_counter);
				}
				
				switch(fish_array[fish_counter].type) {
					case 1:
						goldfish_counter++;
						feeding_costs += GOLDFISH_FEEDING_COSTS;
					break;
					case 0:
					default:
						neonfish_counter++;
						feeding_costs += NEONFISH_FEEDING_COSTS;
					break;
				}
				/* count total active fishes */
				total_fishes++;
			}
			fish_counter++;
		}
	
		// Paarung erst ab 2 Fischen
		if ((goldfish_counter > 1) && (pollution < 70)) {
			// Paarungschance
			if ((rand()%(1+goldfish_counter)) == 0) {
				add_random_fish(GOLDFISH_TYPE);
			}
		}
		if ((neonfish_counter > 1) && (pollution < 70)) {
			// Paarungschance
			if ((rand()%(1+neonfish_counter)) == 0) {
				add_random_fish(NEONFISH_TYPE);
			}
		}
		
		/* show budget */
		printf("event_mulitplier: %d\n", event_multiplier);
		event_multiplier++;
	}
	
	return interval;
}

void game(void) {
	unsigned int card = 0;
	unsigned int fish_counter = 0;
	
	SDL_TimerID pollution_timer_id;
	
	SDL_Rect fish_target;
	SDL_Rect fish_source;
	SDL_Rect filter_target;
	
	// build first screen
	
	SDL_BlitSurface(aquarium, NULL, screen, NULL);
	SDL_Flip(screen);
	
	pollution_timer_id = SDL_AddTimer(POLLUTION_TIMER, pollution_timer, 0);
	
	while(!main_quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_SPACE:
						add_random_fish(GOLDFISH_TYPE);
					break;
					case SDLK_F1:
						open_shop();
					break;
					case SDLK_F2:
						open_fish_table();
					break;
					case SDLK_F3:
						open_loan_dialog();
					break;
					case SDLK_F5:
						open_budget(1,1);
					break;
					case SDLK_F6:
						open_event_card(rand()%EVENT_CARDS);
					break;
					case SDLK_F10:
						//open_version_dialog();
						open_main_menue(0);
					break;
					case SDLK_ESCAPE:
						main_quit = 1;
					break;
				}
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_x, &mouse_y);
				if (event.button.button == SDL_BUTTON_LEFT) {
					if ((mouse_x >= 10) && (mouse_x <= 50) &&
						(mouse_y >= 570) && (mouse_y <= 590)) {
						open_shop();
					} else if ((mouse_x >= 70) && (mouse_x <= 115) &&
							(mouse_y >= 570) && (mouse_y <= 590)) {
						open_fish_table();
					} else if ((mouse_x >= 130) && (mouse_x <= 165) &&
							(mouse_y >= 570) && (mouse_y <= 590)) {
						open_loan_dialog();
					}
				}
			}
		}
		
		// rebuild screen
		SDL_BlitSurface(aquarium, NULL, screen, NULL);
		switch(filter_type) {
			case FILTERLIGHT_TYPE:
				filter_target.x = 660;
				filter_target.y = 25;
				SDL_BlitSurface(filter_light, NULL, screen, &filter_target);
			break;
			default:
			break;
		}
		update_hud();
		
		fish_counter = 0;
		while(fish_counter < MAX_FISH) {
			if (fish_array[fish_counter].active == 1) {
				
				//printf("active fish %d\n", fish_counter);
				
				fish_source.x = 0;
				
				switch(fish_array[fish_counter].direction) {
					case 0:
					break;
					case 1:
						fish_array[fish_counter].x--;
						fish_array[fish_counter].y--;
					break;
					case 2:
						fish_array[fish_counter].x--;
						fish_array[fish_counter].y -= 0.5;
					break;
					case 3:
						fish_array[fish_counter].x--;
					break;
					case 4:
						fish_array[fish_counter].x--;
						fish_array[fish_counter].y += 0.5;
					break;
					case 5:
						
						fish_array[fish_counter].x--;
						fish_array[fish_counter].y++;
					break;
					case 6:
						fish_array[fish_counter].x++;
						fish_array[fish_counter].y--;
					break;
					case 7:
						fish_array[fish_counter].x++;
						fish_array[fish_counter].y -= 0.5;
					break;
					case 8:
						fish_array[fish_counter].x++;
					break;
					case 9:
						fish_array[fish_counter].x++;
						fish_array[fish_counter].y += 0.5;
					break;
					case 10:
						fish_array[fish_counter].x++;
						fish_array[fish_counter].y++;
					break;
				}
				
				fish_array[fish_counter].duration++;
				if (fish_array[fish_counter].duration > 50) {
					fish_array[fish_counter].direction = rand()%11;
					fish_array[fish_counter].duration = 0;
				}
				
				if (fish_array[fish_counter].x < AQUARIUM_SIDE) fish_array[fish_counter].x = AQUARIUM_SIDE;
				
				if (fish_array[fish_counter].y < AQUARIUM_TOP) fish_array[fish_counter].y = AQUARIUM_TOP;
				fish_source.y = 0;
				
				
				fish_target.x = fish_array[fish_counter].x;
				fish_target.y = fish_array[fish_counter].y;				
				
				if ((fish_array[fish_counter].direction >= 1) && (fish_array[fish_counter].direction <= 5)) {
					fish_source.x = fish_defs[fish_array[fish_counter].type].width;
				}
				if (fish_array[fish_counter].y > (VIDEO_Y-AQUARIUM_BOTTOM-fish_defs[fish_array[fish_counter].type].height)) fish_array[fish_counter].y = VIDEO_Y-AQUARIUM_BOTTOM-fish_defs[fish_array[fish_counter].type].height;
				
				if (fish_array[fish_counter].x > (VIDEO_X-AQUARIUM_SIDE-fish_defs[fish_array[fish_counter].type].width)) fish_array[fish_counter].x = VIDEO_X-AQUARIUM_SIDE-fish_defs[fish_array[fish_counter].type].width;
				
				
				fish_source.w = fish_defs[fish_array[fish_counter].type].width;
				fish_source.h = fish_defs[fish_array[fish_counter].type].height;
				fish_target.w = fish_defs[fish_array[fish_counter].type].width;
				fish_target.h = fish_defs[fish_array[fish_counter].type].height;
				SDL_BlitSurface(fish_defs[fish_array[fish_counter].type].fish_surface, &fish_source, screen, &fish_target);
			}
			fish_counter++;
		}
		
		if (event_multiplier >= EVENT_MULTIPLIER) {
			open_budget(total_fishes, feeding_costs);
			printf("budget should be called now\n");
			event_multiplier = 0;
			discard_event_card = 0;
		} else if (event_multiplier == (EVENT_MULTIPLIER/2)) {
			if (!discard_event_card) {
				if (first_event_card) {
					first_event_card = 0;
					discard_event_card = 1;
				} else {
					card = rand()%(EVENT_CARDS+4);
					printf("event card: %d\n", card);
					if (card < EVENT_CARDS) {
						open_event_card(card);
					}
					discard_event_card = 1;
				}
			}
		}
				
		// flip and wait
		SDL_Flip(screen);
		SDL_Delay(TimeLeft());
	}
}


int main(int argN, char * argS[]) {
	/* SDL initialisieren: VIDEO Subsystem */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		/* Init schlug fehl, Fehlermeldung ausgeben und beenden */
		fprintf(stderr, "SDL init failed:  %s\n", SDL_GetError());
		exit(1);
	}
	/* Beim Beenden des Programms aufräumen */
	atexit(SDL_Quit);
	
	/* Grafik Surface initialisieren mit 800x600 Pixel und 16Bit Farben */
	screen = SDL_SetVideoMode(VIDEO_X, VIDEO_Y, VIDEO_BPP, flags);
	/* Init schlug fehl wenn NULL, Fehlermeldung ausgeben und beenden */
	if (screen == NULL) {
		fprintf(stderr, "Unable to open window: %s\n", SDL_GetError());
		exit(1);
	}

	if (TTF_Init() == -1) {
		fprintf(stderr, "Unable to init sdl_ttf: %s\n", TTF_GetError());
		exit(1);
	}

	SDL_WM_SetCaption("aquarium", "aquarium");
	
	srand(time(NULL));
	
	sprintf(data_string, "%saquarium.jpg", DATA_DIR);
	temp_surface =  IMG_Load(data_string);
	aquarium = SDL_DisplayFormat(temp_surface);
	SDL_FreeSurface(temp_surface);
	
	sprintf(data_string, "%sboard.jpg", DATA_DIR);
	temp_surface =  IMG_Load(data_string);
	board = SDL_DisplayFormat(temp_surface);
	SDL_FreeSurface(temp_surface);
	
	sprintf(data_string, "%sfilter_light.jpg", DATA_DIR);
	temp_surface =  IMG_Load(data_string);
	filter_light = SDL_DisplayFormat(temp_surface);
	SDL_FreeSurface(temp_surface);

	sprintf(data_string, "%sboard_tiles.gif", DATA_DIR);
	temp_surface =  IMG_Load(data_string);
	board_tiles = SDL_DisplayFormat(temp_surface);
	SDL_FreeSurface(temp_surface);
	transparent_color = SDL_MapRGB(board_tiles->format, 0xFF, 0, 0xFF);
	SDL_SetColorKey(board_tiles, SDL_SRCCOLORKEY | SDL_RLEACCEL, transparent_color);	

	populate_fish_defs();	

	sprintf(data_string, "%sneonfish.gif", DATA_DIR);
	temp_surface =  IMG_Load(data_string);
	fish_defs[0].fish_surface = SDL_DisplayFormat(temp_surface);
	SDL_FreeSurface(temp_surface);
	transparent_color = SDL_MapRGB(fish_defs[0].fish_surface->format, 0xFF, 0, 0xFF);
	SDL_SetColorKey(fish_defs[0].fish_surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, transparent_color);	

	sprintf(data_string, "%sgoldfish.gif", DATA_DIR);
	temp_surface =  IMG_Load(data_string);
	fish_defs[1].fish_surface = SDL_DisplayFormat(temp_surface);
	SDL_FreeSurface(temp_surface);
	transparent_color = SDL_MapRGB(fish_defs[1].fish_surface->format, 0xFF, 0, 0xFF);
	SDL_SetColorKey(fish_defs[1].fish_surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, transparent_color);	
	
	hud_font = TTF_OpenFont("data/Vera.ttf", 16);
	
	
	game();
	
	SDL_FreeSurface(fish_defs[0].fish_surface);
	SDL_FreeSurface(fish_defs[1].fish_surface);
	TTF_CloseFont(hud_font);
	
	return 0;
}

