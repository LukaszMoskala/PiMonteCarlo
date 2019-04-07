/*
Copyright (C) 2019 Łukasz Konrad Moskała

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <random>
using namespace std;

ALLEGRO_FONT *font;
ALLEGRO_DISPLAY *disp;

//generate this many points before starting again
uint32_t target_points=1000000;
//display every this many points
uint32_t display_every=300;

//points that are both in square and circle
double hits=0;

//display size
uint16_t disp_w;
uint16_t disp_h;

//preview size
uint16_t preview_s;

//preview position
uint16_t preview_x;
uint16_t preview_y;
ALLEGRO_BITMAP *preview;

double abs_f(double f) {
    return (f<0.0?-f:f);
}

//if set to false, program will exit gracefully
bool work=true;

ALLEGRO_COLOR hextocolor(const string& hexstr) {
    if(hexstr.size() != 6) {
        throw new invalid_argument("Hex color is not in 6-character format");
    }
    //assuming that user isn't dumb, and hexstr is actually HEX
    string r=hexstr.substr(0, 2);
    string g=hexstr.substr(2, 2);
    string b=hexstr.substr(4, 2);
    return al_map_rgb(strtol(r.c_str(), NULL, 16),
                      strtol(g.c_str(), NULL, 16),
                      strtol(b.c_str(), NULL, 16));
}
int args;
char **argv;
//reads argument from command line, returns defval if doesnt exist
string getarg(const string &shortname, const string &longname, const string &defval) {
  for(int i=1;i<args;i++) {
    string ca(argv[i]);
    if( ( ca == "--"+longname  || ca == "-"+shortname ) && i+1 < args ) {
      return string(argv[i+1]);
    }
  }
  return defval;
}
//return true if argument exists
bool argexist(const string &shortname, const string &longname) {
  for(int i=1;i<args;i++) {
    string ca(argv[i]);
    if( ca == "--"+longname  || ca == "-"+shortname ) {
      return true;
    }
  }
  return false;
}

int main(int _args, char** _argv) {
    args=_args;
    argv=_argv;

    if(argexist("h","help")) {
        cerr<<"PiMonteCarlo Copyright (C) 2019 Łukasz Konrad Moskała ukasz_moskala8@tlen.pl"<<endl;
        cerr<<"This program comes with ABSOLUTELY NO WARRANTY."<<endl;
        cerr<<"This is free software, and you are welcome to redistribute it"<<endl;
        cerr<<"under certain conditions; Read attached license file for details."<<endl;
        cerr<<endl;
        cerr<<"You should have received a copy of the GNU General Public License"<<endl;
        cerr<<"along with this program.  If not, see <https://www.gnu.org/licenses/>."<<endl;
        cerr<<endl;

        cerr<<"Usage: "<<endl;
        cerr<<" -p --bgpreview      set preview background color  - default: 000000"<<endl;
        cerr<<" -P --bgprogram      set program background color  - default: 444444"<<endl;
        cerr<<" -c --fontcolor      set font color                - default: ffffff"<<endl;
        cerr<<" -C --circlecolor    set circle color              - default: ffff00"<<endl;
        cerr<<" -W --width          set window width              - default: 1000"<<endl;
        cerr<<" -H --height         set window height             - default: 1000"<<endl;
        cerr<<" -n --points         how many points generate      - default: 1000000"<<endl;
        cerr<<" -d --disp           how often update display      - default: 100 (every 100 points)"<<endl;
        cerr<<" -f --font           set font to use            [1]- default: <none>"<<endl;
        cerr<<" -s --fontsize       set font size (char height)[2]- default: 24"<<endl;
        cerr<<" -F --fullscreen     run in fullscreen mode"<<endl;
        cerr<<endl;
        cerr<<"[1]: If not specified, builtin font will be used"<<endl;
        cerr<<"     example: --font myfancyfont.ttf"<<endl;
        cerr<<"     Place font in directory with program"<<endl;
        cerr<<"[2]: Works only if font is specified, otherwise is set to 8"<<endl;
        cerr<<endl;
        cerr<<"All colors are specified in format"<<endl;
        cerr<<"    RRGGBB - just like in HTML but without #"<<endl;
        return 0;
    }

    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();

    ALLEGRO_COLOR bg_preview=hextocolor(getarg("p","bgpreview","000000"));
    ALLEGRO_COLOR bg_program=hextocolor(getarg("P","bgprogram","444444"));
    ALLEGRO_COLOR font_color=hextocolor(getarg("c","fontcolor","ffffff"));
    ALLEGRO_COLOR circle_color=hextocolor(getarg("C","circlecolor","ffff00"));;
    //random number generator
    mt19937 gen{random_device{}()};
    uniform_real_distribution<double> points{-1.0, 1.0};
    uniform_real_distribution<double> colors{0, 255};
    
    disp_w=atoi(getarg("W","width","1000").c_str());
    disp_h=atoi(getarg("H","height","1000").c_str());

    target_points=strtoul(getarg("n","points","1000000").c_str(), NULL, 10);
    display_every=strtoul(getarg("d","disp","100").c_str(), NULL, 10);
    

    preview_s=((disp_w>disp_h)?disp_h:disp_w)*0.8;
    preview_x=(disp_w-preview_s)/2;
    preview_y=(disp_h-preview_s)/2;

    if(argexist("F","fullscreen")) {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    }
    else {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
    }
    
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
    //try to load font.ttf if no font was specified
    string f=getarg("f","font","font.ttf");
    int size=atoi(getarg("s","fontsize","24").c_str());
    //Why using negative size value?
    //well, https://www.allegro.cc/manual/5/al_load_ttf_font
            
    //"The size parameter determines the size the font will be rendered at,
    //specified in pixel. The standard font size is measured in units per
    //EM, if you instead want to specify the size as the total height
    //of glyphs in pixel, pass it as a negative value."
    font=al_load_ttf_font(f.c_str(), -size, 0);
    //if loading custom font failed, fall back to builtin
    if(!font) {
        font=al_create_builtin_font();
    }
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
    ALLEGRO_KEYBOARD_STATE kbstate;
    while(work) {
        al_set_target_bitmap(preview);
        al_clear_to_color(bg_preview);
        hits=0;
        for(uint32_t i=0;i<target_points && work;i++) {
            al_get_keyboard_state(&kbstate);
            if(al_key_down(&kbstate, ALLEGRO_KEY_ESCAPE) ||
               al_key_down(&kbstate, ALLEGRO_KEY_Q)) {
                work=false;
                cout<<"Escape pressed, exiting!"<<endl;
            }
            if(al_key_down(&kbstate, ALLEGRO_KEY_ENTER)) {
                cout<<"Restarting!"<<endl;
                while(al_key_down(&kbstate, ALLEGRO_KEY_ENTER))
                   al_get_keyboard_state(&kbstate);

                break;
            }
            x=points(gen);
            y=points(gen);
            if(x*x+y*y <= 1) {
                hits+=1;
            }
            c=al_map_rgb(colors(gen),colors(gen),colors(gen));
            al_draw_pixel(x*float(preview_s), y*float(preview_s), c);
            if(i%display_every == 0) {
                al_draw_circle(preview_s/2.0, preview_s/2.0, preview_s/2.0, circle_color, 3);
                al_set_target_backbuffer(disp);
                al_clear_to_color(bg_program);
                pi=4.0*hits/double(i+1);
                al_draw_textf(font, font_color, 0, 0, 0, "Points: %u/%u",i, target_points);
                al_draw_textf(font, font_color, 0, fh, 0, "PI: %f", pi);
                al_draw_textf(font, font_color, 0, fh*2, 0, "Error: %f%%", 100.0*(abs_f(pi-M_PI)/M_PI));
                al_draw_bitmap(preview, preview_x, preview_y, 0);
                al_draw_text(font, font_color, 0, disp_h-2*fh, 0, "(C) 2019 Łukasz Konrad Moskała");
                al_draw_text(font, font_color, 0, disp_h-fh, 0, "github.com/LukaszMoskala/PiMonteCarlo");
                al_flip_display();
                al_set_target_bitmap(preview);
            }
        }
    }
    al_destroy_bitmap(preview);
    al_destroy_display(disp);
    al_destroy_font(font);
    al_uninstall_keyboard();
    return 0;
}