#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <random>
using namespace std;

ALLEGRO_FONT *font;
ALLEGRO_DISPLAY *disp;

//generate this many points before starting again
uint32_t target_points=1000000; //TODO: Load from parameter
//display every this many points
uint32_t display_every=300; //TODO: Load from parameter

//points that are both in square and circle
double hits=0;
//points that are only in square
double miss=0;

//display size
uint16_t disp_w; //TODO: Load from parameter
uint16_t disp_h; //TODO: Load from parameter

//preview size
uint16_t preview_s;

//preview position
uint16_t preview_x;
uint16_t preview_y;
ALLEGRO_BITMAP *preview;

double abs_f(double f) {
    return (f<0.0?-f:f);
}

int main() {
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    
    ALLEGRO_COLOR bg_preview=al_map_rgb(0,0,0);
    ALLEGRO_COLOR bg_program=al_map_rgb(64,64,64);
    ALLEGRO_COLOR font_color=al_map_rgb(255,255,255);
    ALLEGRO_COLOR circle_color=al_map_rgb(255,255,0);
    //random number generator
    mt19937 gen{random_device{}()};
    uniform_real_distribution<double> points{-1.0, 1.0};
    uniform_real_distribution<double> colors{0, 255};
    
    disp_w=1000;
    disp_h=1000;

    preview_s=((disp_w>disp_h)?disp_h:disp_w)*0.8;
    preview_x=(disp_w-preview_s)/2;
    preview_y=(disp_h-preview_s)/2;

    //TODO: Load from parameter
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    
    disp=al_create_display(disp_w, disp_h);
    if(!disp) {
        cout<<__FILE__<<":"<<__LINE__<<" : "<<"Failed to create display!"<<endl;
        return 1;
    }
    preview=al_create_bitmap(preview_s, preview_s);
    if(!disp) {
        cout<<__FILE__<<":"<<__LINE__<<" : "<<"Failed to create bitmap!"<<endl;
        return 1;
    }
    //TODO: Load TTF
    font=al_create_builtin_font();
    if(!font) {
        cout<<__FILE__<<":"<<__LINE__<<" : "<<"Failed to create font!"<<endl;
        return 1;
    }
    //Real font height
    int fh=al_get_font_line_height(font);


    float x;
    float y;
    ALLEGRO_COLOR c;
    double pi=0;
    while(1) {
        al_set_target_bitmap(preview);
        al_clear_to_color(bg_preview);
        hits=0;
        miss=0;
        for(uint32_t i=0;i<target_points;i++) {
            x=points(gen);
            y=points(gen);
            miss+=1;
            if(x*x+y*y <= 1)
                hits+=1;
            c=al_map_rgb(colors(gen),colors(gen),colors(gen));
            al_draw_pixel(x*float(preview_s), y*float(preview_s), c);
            if(i%display_every == 0) {
                al_draw_circle(preview_s/2.0, preview_s/2.0, preview_s/2.0, circle_color, 3);
                al_set_target_backbuffer(disp);
                al_clear_to_color(bg_program);
                pi=4.0*hits/miss;
                al_draw_textf(font, font_color, 0, 0, 0, "Points: %u/%u",i, target_points);
                al_draw_textf(font, font_color, 0, fh, 0, "PI: %f", pi);
                al_draw_textf(font, font_color, 0, fh*2, 0, "Error: %f%%", 100.0*(abs_f(pi-M_PI)/M_PI));
                al_draw_bitmap(preview, preview_x, preview_y, 0);
                al_flip_display();
                al_set_target_bitmap(preview);
            }
        }
    }
    al_destroy_bitmap(preview);
    al_destroy_display(disp);
    al_destroy_font(font);
    return 0;
}