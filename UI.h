#define COLOR_DEFAULT_BACK  0x00
#define COLOR_DEFAULT_TEXT  0x07
#define COLOR_SELECT        0x80

#define COLOR_STAR_K        0x06
#define COLOR_STAR_G        0x0E
#define COLOR_STAR_F        0x0F

#define COLOR_PLANET_DENSE  0x07
#define COLOR_PLANET_DESRT  0x06
#define COLOR_PLANET_WATER1 0x03
#define COLOR_PLANET_WATER2 0x09
#define COLOR_PLANET_WATER3 0x01
#define COLOR_PLANET_GREEN  0x0A

#define COLOR_PLANET_GIANT1 0x0E //For type 1 or 2
#define COLOR_PLANET_GIANT2 0x0C //For type 3
#define COLOR_PLANET_GIANT3 0x04 //For type 4 or 5
#define COLOR_PLANET_ICEGT1 0x0F
#define COLOR_PLANET_ICEGT2 0x0B
#define COLOR_PLANET_ICEGT3 0x03
#define COLOR_PLANET_GASDWF 0x0D

#define COLOR_PLANET_ICEDWF 0x05
#define COLOR_PLANET_RCKDWF 0x0D

#define COLOR_MOON_MAJOR_I  0x0B
#define COLOR_MOON_MINOR_I  0x03
#define COLOR_MOON_MAJOR_R  0x0F
#define COLOR_MOON_MINOR_R  0x07

#define COLOR_ATMO_HYDROGEN 0x0F
#define COLOR_ATMO_ARGON    0x0D
#define COLOR_ATMO_NITROGEN 0x09
#define COLOR_ATMO_OXYGEN   0x0B
#define COLOR_ATMO_FLUORINE 0x0E
#define COLOR_ATMO_CHLORINE 0x02
#define COLOR_ATMO_BROMINE  0x06
#define COLOR_ATMO_IODINE   0x05
#define COLOR_ATMO_NEON     0x0C
#define COLOR_ATMO_KRYPTON  0x0F
#define COLOR_ATMO_XENON    0x03
#define COLOR_ATMO_PHOSPHOR 0x04
#define COLOR_ATMO_WATER    0x01
#define COLOR_ATMO_SULFUR   0x0E
#define COLOR_ATMO_CARBON   0x08
#define COLOR_ATMO_ALKALI   0x07
#define COLOR_ATMO_THOLINS  0x04
#define COLOR_ATMO_SILICON  0x06
#define COLOR_ATMO_IRON     0x07

#define rows 17

#ifndef UI_H
#define UI_H

void draw_frame(void);
void pop_options(struct Star *star_ptr, struct Planet *planets, int num_planets, int planet_option, int moon_option);

void print_star_props(struct Star *star_ptr);
void print_planet_props(struct Planet *planet_ptr);
void print_moon_props(struct Moon *moon_ptr);

void print_star_desc(struct Star *star_ptr);
void print_planet_desc(struct Planet *planet_ptr);
void print_belt_desc(struct Planet *planet_ptr, int num);
void print_kuiper_desc(struct Planet *planets, int num);
void print_moon_desc(struct Moon *moon_ptr);

void print_units(void);
void print_desc(void);
void print_orbit(void);
void print_title(struct Star *star_ptr, struct Planet *planets, int num_planets);

//This function draws the frame around the edges of the boxes.
void draw_frame(void)
{
    Movetoxy(0,0);
    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    printf("┌──────────────────────────╥───────────────────────────────────────────────────────────────────────────────────────────┐"
         "\n│           MENU           ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ╠═══════════════════════════════════════════════════════════════════════════════════════════╡"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n│                          ║                                                                                           │"
         "\n└──────────────────────────╨───────────────────────────────────────────────────────────────────────────────────────────┘");
}

//This function populates the options on the left side of the screen.
void pop_options(struct Star *star_ptr, struct Planet *planets, int num_planets, int planet_option, int moon_option)
{
    int color_back, color_text;

    //The 'menu' option
    Movetoxy(1,1);
    color_text = COLOR_DEFAULT_TEXT;
    if (planet_option == 0)
        color_back = COLOR_SELECT;
    else
        color_back = COLOR_DEFAULT_BACK;
    Set_Color(color_back + color_text);
    printf("           MENU           ");

    int offset = (
        planet_option - rows + 1 > 0
        ? planet_option - rows + 1
        : 0
    );

    int moon_offset = (
        floor(moon_option / 4.0) - (26-rows) + 1 > 0
        ? floor(moon_option / 4.0) - (26-rows) + 1
        : 0
    );

    //The star option
    if (offset == 0)
    {
        Movetoxy(1,2);
        if (planet_option == 1)
            color_back = COLOR_SELECT;
        else
            color_back = COLOR_DEFAULT_BACK;

        if (star_ptr->type == 'K')
            color_text = COLOR_STAR_G;
        else if (star_ptr->type == 'G')
            color_text = COLOR_STAR_G;
        else if (star_ptr->type == 'F')
            color_text = COLOR_STAR_F;

        Set_Color(color_back + color_text);
        printf(" %c-Type Star              ", star_ptr->type);
    }

    //The planet options
    char *ordinal = (char*)malloc(sizeof(char)* 3);
    char *descrip = (char*)malloc(sizeof(char)*20);
    int is_belt_inner = false;

    if (ordinal == nullptr)
        crash(-150);
    if (descrip == nullptr)
        crash(-151);

    int kuip = 0;
    for (int i = 1+(offset > 0 ? 0 : 1); i < rows; i++) //13
    {
        Movetoxy(1, i+1);

        if (i > num_planets+1)
        {
            if (planet_option == i)
                Set_Color(COLOR_SELECT);
            else
                Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);

            printf("                         ");
            continue;
        }

        int curr = i-2+offset;
        ordinal  = get_ordinal(curr-is_belt_inner);

        if (planet_option-offset == i)
            color_back = COLOR_SELECT;
        else
            color_back = COLOR_DEFAULT_BACK;

        switch (planets[curr].type)
        {
            case TYPE_BLT_INNER:
                color_text    = COLOR_PLANET_DENSE;
                is_belt_inner = 1;
                Set_Color(color_back + color_text);
                printf(" Inner asteroid belt      ");
                continue;
            case TYPE_DWF_PLNTSML:
                if (planets[curr].bulk_ices > planets[curr].bulk_rock)
                    color_text = COLOR_PLANET_RCKDWF;
                else
                    color_text = COLOR_PLANET_ICEDWF;
                is_belt_inner = 2;
                Set_Color(color_back + color_text);
                printf("      1st: dwarf planet   ");
                continue;
            case TYPE_DWF_PLUTINO:
                if (planets[curr].bulk_ices > planets[curr].bulk_rock)
                    color_text = COLOR_PLANET_RCKDWF;
                else
                    color_text = COLOR_PLANET_ICEDWF;
                is_belt_inner = 2;
                Set_Color(color_back + color_text);
                printf("     %2i%2s: plutino        ", kuip+1, get_ordinal(kuip));
                kuip++;
                continue;
            case TYPE_DWF_CUBEWNO:
                if (planets[curr].bulk_ices > planets[curr].bulk_rock)
                    color_text = COLOR_PLANET_RCKDWF;
                else
                    color_text = COLOR_PLANET_ICEDWF;
                is_belt_inner = 2;
                Set_Color(color_back + color_text);
                printf("     %2i%2s: cubewano       ", kuip+1, get_ordinal(kuip));
                kuip++;
                continue;
            case TYPE_DWF_TWOTINO:
                if (planets[curr].bulk_ices > planets[curr].bulk_rock)
                    color_text = COLOR_PLANET_RCKDWF;
                else
                    color_text = COLOR_PLANET_ICEDWF;
                is_belt_inner = 2;
                Set_Color(color_back + color_text);
                printf("     %2i%2s: twotino        ", kuip+1, get_ordinal(kuip));
                kuip++;
                continue;
            case TYPE_DWF_SCATTER:
                if (planets[curr].bulk_ices > planets[curr].bulk_rock)
                    color_text = COLOR_PLANET_RCKDWF;
                else
                    color_text = COLOR_PLANET_ICEDWF;
                is_belt_inner = 2;
                Set_Color(color_back + color_text);
                printf("     %2i%2s: disk object    ", kuip+1, get_ordinal(kuip));
                kuip++;
                continue;
            case TYPE_DWF_SEDNOID:
                if (planets[curr].bulk_ices > planets[curr].bulk_rock)
                    color_text = COLOR_PLANET_RCKDWF;
                else
                    color_text = COLOR_PLANET_ICEDWF;
                is_belt_inner = 2;
                Set_Color(color_back + color_text);
                printf("     %2i%2s: sednoid        ", kuip+1, get_ordinal(kuip));
                kuip++;
                continue;
            case TYPE_BLT_KUIPR:
                color_text    = COLOR_PLANET_DENSE;
                Set_Color(color_back + color_text);
                printf(" Kuiper asteroid belt     ");
                continue;
            case TYPE_RCK_DENSE:
                sprintf(descrip, "cannonball planet");
                color_text = COLOR_PLANET_DENSE;
                break;
            case TYPE_RCK_DESRT:
                sprintf(descrip, "desert planet");
                color_text = COLOR_PLANET_DESRT;
                break;
            case TYPE_WTR_HYCN:
                sprintf(descrip, "ocean planet");
                switch (planets[curr].subtype[1])
                {
                    case '1':
                        color_text = COLOR_PLANET_WATER1;
                        break;
                    case '2':
                        color_text = COLOR_PLANET_WATER2;
                        break;
                    default:
                        color_text = COLOR_PLANET_WATER3;
                }
                break;
            case TYPE_WTR_GREEN:
                sprintf(descrip, "ocean planet");
                color_text = COLOR_PLANET_GREEN;
                break;
            case TYPE_RCK_GREEN:
                sprintf(descrip, "habitable planet");
                color_text = COLOR_PLANET_GREEN;
                break;
            case TYPE_GAS_GIANT:
                sprintf(descrip, "gas giant");
                switch (planets[curr].subtype[1])
                {
                    case '3':
                        color_text = COLOR_PLANET_GIANT2;
                        break;
                    case '4':
                    case '5':
                        color_text = COLOR_PLANET_GIANT3;
                        break;
                    default:
                        color_text = COLOR_PLANET_GIANT1;
                }
                break;
            case TYPE_GAS_SUPER:
                sprintf(descrip, "gas supergiant");
                switch (planets[curr].subtype[1])
                {
                    case '3':
                        color_text = COLOR_PLANET_GIANT2;
                        break;
                    case '4':
                    case '5':
                        color_text = COLOR_PLANET_GIANT3;
                        break;
                    default:
                        color_text = COLOR_PLANET_GIANT1;
                }
                break;
            case TYPE_GAS_PUFFY:
                sprintf(descrip, "puffy gas giant");
                switch (planets[curr].subtype[1])
                {
                    case '3':
                        color_text = COLOR_PLANET_GIANT2;
                        break;
                    case '4':
                    case '5':
                        color_text = COLOR_PLANET_GIANT3;
                        break;
                    default:
                        color_text = COLOR_PLANET_GIANT1;
                }
                break;
            case TYPE_GAS_HOT:
                sprintf(descrip, "hot Jupiter");
                switch (planets[curr].subtype[1])
                {
                    case '3':
                        color_text = COLOR_PLANET_GIANT2;
                        break;
                    case '4':
                    case '5':
                        color_text = COLOR_PLANET_GIANT3;
                        break;
                    default:
                        color_text = COLOR_PLANET_GIANT1;
                }
                break;
            case TYPE_ICE_DWARF:
                sprintf(descrip, "gas dwarf");
                color_text = COLOR_PLANET_GASDWF;
                break;
            case TYPE_ICE_GIANT:
                sprintf(descrip, "ice giant");
                switch (planets[curr].subtype[1])
                {
                    case '2':
                        color_text = COLOR_PLANET_ICEGT2;
                        break;
                    case '3':
                        color_text = COLOR_PLANET_ICEGT3;
                        break;
                    default:
                        color_text = COLOR_PLANET_ICEGT1;
                }
                break;
            default:
                sprintf(descrip, "GENERATION ERROR");
                color_text = COLOR_DEFAULT_TEXT;
        }

        Set_Color(color_back + color_text);
        printf(" %2i%-2s: %-18s ", curr+1-is_belt_inner, ordinal, descrip);
    }

    //The moon options
    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    Movetoxy(0, rows+1);
    printf("╞══════════════════════════╣");

    if (planet_option == 0)
    {
        Movetoxy(2, rows+2);
        Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
        printf("        OPTIONS          ");

        Movetoxy(2, rows+3);
        color_text = COLOR_DEFAULT_TEXT;
        if (moon_option == 0)
            color_back = COLOR_SELECT;
        else
            color_back = COLOR_DEFAULT_BACK;
        Set_Color(color_back + color_text);
        printf(" Controls               ");

        Movetoxy(2, rows+4);
        color_text = COLOR_DEFAULT_TEXT;
        if (moon_option == 1)
            color_back = COLOR_SELECT;
        else
            color_back = COLOR_DEFAULT_BACK;
        Set_Color(color_back + color_text);
        printf(" Unit Definitions       ");

        Movetoxy(2, rows+5);
        color_text = COLOR_DEFAULT_TEXT;
        if (moon_option == 2)
            color_back = COLOR_SELECT;
        else
            color_back = COLOR_DEFAULT_BACK;
        Set_Color(color_back + color_text);
        printf(" Planet Classes         ");

        Movetoxy(2, rows+6);
        color_text = COLOR_DEFAULT_TEXT;
        if (moon_option == 3)
            color_back = COLOR_SELECT;
        else
            color_back = COLOR_DEFAULT_BACK;
        Set_Color(color_back + color_text);
        printf(" Orbital Parameters     ");

        Movetoxy(2, rows+7);
        color_text = COLOR_DEFAULT_TEXT;
        if (moon_option == 4)
            color_back = COLOR_SELECT;
        else
            color_back = COLOR_DEFAULT_BACK;
        Set_Color(color_back + color_text);
        printf(" Regenerate             ");

        Movetoxy(2, rows+8);
        color_text = COLOR_DEFAULT_TEXT;
        if (moon_option == 5)
            color_back = COLOR_SELECT;
        else
            color_back = COLOR_DEFAULT_BACK;
        Set_Color(color_back + color_text);
        printf(" Quit                   ");
    }
    else if (planet_option == 1)
    {}
    else
    {
        Movetoxy(9, rows+2);
        Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
        printf("SATELLITES");

        int maximum = min(((26-rows) + moon_offset)*4.0, planets[planet_option-2].num_moons+1);
        for (int i = moon_offset*4.0; i < maximum; i++)
        {
            if (!planets[planet_option-2].num_moons)
                break;

            if (i == 0)
            {
                Movetoxy(2, rows+3);

                if (moon_option == i)
                    Set_Color(COLOR_SELECT);
                else
                    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);

                printf("      ");
                continue;
            }

            Movetoxy(((i) % 4)*6 + 2, rows+3 + ((i)/4)- moon_offset);

            ordinal = get_ordinal(i-1);

            if (moon_option == i)
                color_back = COLOR_SELECT;
            else
                color_back = COLOR_DEFAULT_BACK;

            switch (planets[planet_option-2].moons[i-1].type)
            {
                case TYPE_SAT_MAJOR:
                case TYPE_SAT_MOONB:
                    if (planets[planet_option-2].moons[i-1].bulk_ices > planets[planet_option-2].moons[i-1].bulk_rock)
                        color_text = COLOR_MOON_MAJOR_I;
                    else
                        color_text = COLOR_MOON_MAJOR_R;
                    break;
                default:
                    if (planets[planet_option-2].moons[i-1].bulk_ices > planets[planet_option-2].moons[i-1].bulk_rock)
                        color_text = COLOR_MOON_MINOR_I;
                    else
                        color_text = COLOR_MOON_MINOR_R;
                    break;
            }

            Set_Color(color_back + color_text);
            printf(" %2i%2s ", i, ordinal);
        }
    }

    free((void*)ordinal);
    free((void*)descrip);
}



//This function prints the properties of the selected star to the screen.
void print_star_props(struct Star *star_ptr)
{
    char *text = (char*)malloc(sizeof(char)*4000);

    if (text == nullptr)
        crash(-160);

    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);

    /*
    sprintf(text, "Mass        = %6.3f M☉    Formation Min = %6.3f AU"
                "\nRadius      = %6.3f R☉    Formation Max = %6.3f AU"
                "\nLuminosity  = %6.3f L☉    Habitable Min = %6.3f AU"
                "\nTemperature = %6.3f T☉    Habitable Max = %6.3f AU"
                "\nLifetime    = %6.3f by🜨   Frost line    = %6.3f AU",
                   star_ptr->mass,         star_ptr->zone_form_min,
                   star_ptr->radius,       star_ptr->zone_form_max,
                   star_ptr->lumin,        star_ptr->zone_hab_min,
                   star_ptr->temp/5772.0,  star_ptr->zone_hab_max,
                   star_ptr->life,         star_ptr->zone_frost
    );
    */

    sprintf(text, "╥───────────────────────────────"  "┬──────────────────────────────"   "┬"
                "\n║           Properties          "  "│          Radiance            "   "│"
                "\n╟───────────────┬───────────────"  "┼──────────────┬───────────────"   "┤"
                "\n║  Mass         │ %8.3f M☉   "     "│  Eff. temp.  │  %8.3f K   "      "│"
                "\n║  Radius       │ %8.3f R☉   "     "│  B-V index   │  %8.3f     "      "│"
                "\n║  Gravity      │ %8.3f G    "     "│              │               "   "│"
                "\n║  Escape vel.  │ %8.3f km/h "     "│  Luminosity  │  %8.3f L☉  "      "│"
                "\n║  Metallicity  │ %8.3f %%    "    "│  Abs. Magn.  │  %8.3f     "      "│"
                "\n║  Lifetime     │ %8.3f by🜨  "     "│  Rel. Magn.  │  %8.3f     "      "│"
                "\n╟───────────────┴───────────────"  "┼──────────────┴───────────────"   "┤"
                "\n║       Position Parameters     "  "│         Photosphere          "   "│"
                "\n╟───────────────┬───────────────"  "┼──────────────┬───────────────"   "┤"
                "\n║  Galactic yr  │ %8.3f my🜨  "     "│              │               "   "│"
                "\n║               │               "  "│              │               "   "│"
                "\n║  Formation    │ %8.3f AU - "     "│              │               "   "│"
                "\n║         Zone  │ %8.3f AU   "     "│              │               "   "│"
                "\n║               │               "  "│              │               "   "│"
                "\n║  Goldilocks   │ %8.3f AU - "     "│              │               "   "│"
                "\n║         Zone  │ %8.3f AU   "     "│              │               "   "│"
                "\n║               │               "  "│              │               "   "│"
                "\n║  Frost Line   │ %8.3f AU   "     "│              │               "   "│"
                "\n╠═══════════════╧═══════════════"  "╧══════════════╧═══════════════"   "╧",


                   star_ptr->mass,                      star_ptr->temp,
                   star_ptr->radius,                    star_ptr->bv,
                   star_ptr->surf_grav,
                   star_ptr->esc_v/60.0,                star_ptr->lumin,
                   star_ptr->metal*100.0,               star_ptr->abs_mag,
                   star_ptr->life,                      star_ptr->rel_mag,

                   star_ptr->y,

                   star_ptr->zone_form_min,
                   star_ptr->zone_form_max,

                   star_ptr->zone_hab_min,
                   star_ptr->zone_hab_max,

                   star_ptr->zone_frost
    );

    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy(text, 27, 0);

    Movetoxy(27, rows+1);
    printf("╣");

    //Print photosphere info
    sprintf(text, "┼──────────────┬──────────────"
                "\n│"
                "\n│"
                "\n│"
                "\n│"
                "\n│"
                "\n│"
                "\n│"
                "\n│"
                "\n│"
                "\n╧══════════════╧══════════════"
    );

    print_at_xy(text, 59, 11);

    int colors[13] = {
        COLOR_ATMO_HYDROGEN, //Hydrogen
        COLOR_ATMO_HYDROGEN, //Helium
        COLOR_ATMO_CARBON  , //Carbon
        COLOR_ATMO_NITROGEN, //Nitrogen
        COLOR_ATMO_OXYGEN  , //Oxygen
        COLOR_ATMO_NEON    , //Neon
        COLOR_ATMO_ALKALI  , //Sodium
        COLOR_ATMO_ALKALI  , //Magnesium
        COLOR_ATMO_SILICON , //Silicon
        COLOR_ATMO_PHOSPHOR, //Phosphorous
        COLOR_ATMO_SULFUR  , //Sulfur
        COLOR_ATMO_ARGON   , //Argon
        COLOR_ATMO_IRON      //Iron
    };

    char *atmo_names[] = {
        "Hydrogen",
        "Helium",
        "Carbon",
        "Nitrogen",
        "Oxygen",
        "Neon",
        "Sodium",
        "Magnesium",
        "Silicon",
        "Phosphorous",
        "Sulfur",
        "Argon",
        "Iron"
    };

    double atmosphere[13];

    for (int i = 0; i < 13; i++)
        atmosphere[i] = star_ptr->photosphere[i];

    //Sort the components in descending order
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12-i; j++)
        {
            if (atmosphere[j] < atmosphere[j+1])
            {
                double temp_d   = atmosphere[j+1];
                atmosphere[j+1] = atmosphere[j]  ;
                atmosphere[j]   = temp_d         ;

                char *temp_c    = atmo_names[j+1];
                atmo_names[j+1] = atmo_names[j]  ;
                atmo_names[j]   = temp_c         ;

                int temp_i  = colors[j+1];
                colors[j+1] = colors[j]  ;
                colors[j]   = temp_i     ;
            }
        }
    }

    for (int i = 0; i < 9; i++)
    {
        //Determine unit
        if ((atmosphere[i])*1000000000000.0 < 1.0)
            continue;
        else if ((atmosphere[i])*1000000000.0 < 0.1)
        {
            sprintf(text, "%-11s │ %9.5f ppt",
                atmo_names[i],
                atmosphere[i]*1000000000000.0
            );
        }
        else if ((atmosphere[i])*1000000.0 < 0.1)
        {
            sprintf(text, "%-11s │ %9.5f ppb",
                atmo_names[i],
                atmosphere[i]*1000000000.0
            );
        }
        else if ((atmosphere[i])*1000000.0 < 100.0)
        {
            sprintf(text, "%-11s │ %9.5f ppm",
                atmo_names[i],
                atmosphere[i]*1000000.0
            );
        }
        else
        {
            sprintf(text, "%-11s │ %9.5f %%",
                atmo_names[i],
                atmosphere[i]*100.0
            );
        }

        //Print
        Set_Color(colors[i]);
        print_at_xy(text, 62, 12+i);
    }

    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy("│\n│\n│\n│\n│\n│\n│\n│\n│", 74, 12);

    free((void*)text);
}

//This function prints the properties of the selected planet to the screen.
void print_planet_props(struct Planet *planet_ptr)
{
    char *text = (char*)malloc(sizeof(char)*3500);
    if (text == nullptr)
        crash(-161);

    double mass = (
        planet_ptr->type != TYPE_DWF_PLNTSML &&
        planet_ptr->type != TYPE_DWF_PLUTINO &&
        planet_ptr->type != TYPE_DWF_CUBEWNO &&
        planet_ptr->type != TYPE_DWF_TWOTINO &&
        planet_ptr->type != TYPE_DWF_SCATTER &&
        planet_ptr->type != TYPE_DWF_SEDNOID &&
        planet_ptr->mass >= 317.8*(1.0-FUDGE_FACTOR)
        ? planet_ptr->mass/317.8
        : (
            planet_ptr->mass < 0.001
            ? planet_ptr->mass*5972200.0
            : planet_ptr->mass
        )
    );

    double rad = (
        planet_ptr->type != TYPE_DWF_PLNTSML &&
        planet_ptr->type != TYPE_DWF_PLUTINO &&
        planet_ptr->type != TYPE_DWF_CUBEWNO &&
        planet_ptr->type != TYPE_DWF_TWOTINO &&
        planet_ptr->type != TYPE_DWF_SCATTER &&
        planet_ptr->type != TYPE_DWF_SEDNOID &&
        planet_ptr->radius >= GAS_RADIUS_NORM*(1.0-FUDGE_FACTOR)
        ? planet_ptr->radius/GAS_RADIUS_NORM
        : planet_ptr->radius
    );

    sprintf(text, "╥───────────────────────────────"  "┬──────────────────────────────"   "┬────────────────────────────┐"
                "\n║           Properties          "  "│       Bulk Composition       "   "│           Rings            │"
                "\n╟───────────────┬───────────────"  "┼──────────────┬───────────────"   "┼────────────────────────────┤"
                "\n║  Mass         │ %8.3f %2s   "    "│  Metal mass  │  %6.3f %%     "   "│"
                "\n║  Radius       │ %8.3f %2s   "    "│  Rocky mass  │  %6.3f %%     "   "│"
                "\n║  Gravity      │ %8.3f G    "     "│  Water mass  │  %6.3f %%     "   "│"
                "\n║  Escape vel.  │ %8.3f km/s "     "│  Ices  mass  │  %6.3f %%     "   "│"
                "\n║  Temperature  │ %8.3f °C   "     "│  H₂+He mass  │  %6.3f %%     "   "│"
                "\n║  Bond albedo  │ %8.3f %%    "    "│  Density     │  %6.3f g/cm³ "    "│"
                "\n╟───────────────┴───────────────"  "┼──────────────┴───────────────"   "┤"
                "\n║       Position Parameters     "  "│          Atmosphere          "   "│"
                "\n╟───────────────┬───────────────"  "┼──────────────────────────────"   "┤"
                "\n║  Smjr. axis   │ %8.3f AU   "     "│                              "   "│"
                "\n║  Eccentricity │ %8.3f %%    "    "│                              "   "│"
                "\n║  Inclination  │ %+8.3f °    "    "│                              "   "│"
                "\n║  Longitude    │ %8.3f °    "     "│                              "   "│"
                "\n║  Argument     │ %8.3f °    "     "│                              "   "│"
                "\n║  Orbit vel.   │ %8.3f km/s "     "│                              "   "│"
                "\n║  Year length  │ %8.3f y🜨   "     "│                              "   "│"
                "\n║  Day  length  │ %8.3f hr   "     "│                              "   "│"
                "\n║  Axial tilt   │ %8.3f °    "     "│                              "   "│"
                "\n╠═══════════════╧═══════════════"  "╧══════════════════════════════"   "╧",

                      mass, (planet_ptr->type != TYPE_DWF_PLNTSML &&
                             planet_ptr->type != TYPE_DWF_PLUTINO &&
                             planet_ptr->type != TYPE_DWF_CUBEWNO &&
                             planet_ptr->type != TYPE_DWF_TWOTINO &&
                             planet_ptr->type != TYPE_DWF_SCATTER &&
                             planet_ptr->type != TYPE_DWF_SEDNOID &&
                             planet_ptr->mass >= 317.8*(1.0-FUDGE_FACTOR) ? "M♃" :
                            (planet_ptr->type != TYPE_DWF_PLNTSML &&
                             planet_ptr->type != TYPE_DWF_PLUTINO &&
                             planet_ptr->type != TYPE_DWF_CUBEWNO &&
                             planet_ptr->type != TYPE_DWF_TWOTINO &&
                             planet_ptr->type != TYPE_DWF_SCATTER &&
                             planet_ptr->type != TYPE_DWF_SEDNOID &&
                             planet_ptr->mass < 0.001 ? "Zg" : "M🜨")),
                                                                                                                    planet_ptr->bulk_metal*100.0,
                      rad,  (planet_ptr->type != TYPE_DWF_PLNTSML &&
                             planet_ptr->type != TYPE_DWF_PLUTINO &&
                             planet_ptr->type != TYPE_DWF_CUBEWNO &&
                             planet_ptr->type != TYPE_DWF_TWOTINO &&
                             planet_ptr->type != TYPE_DWF_SCATTER &&
                             planet_ptr->type != TYPE_DWF_SEDNOID &&
                             planet_ptr->radius/GAS_RADIUS_NORM >= GAS_RADIUS_NORM*(1.0-FUDGE_FACTOR) ? "R♃" :
                            (planet_ptr->type != TYPE_DWF_PLNTSML &&
                             planet_ptr->type != TYPE_DWF_PLUTINO &&
                             planet_ptr->type != TYPE_DWF_CUBEWNO &&
                             planet_ptr->type != TYPE_DWF_TWOTINO &&
                             planet_ptr->type != TYPE_DWF_SCATTER &&
                             planet_ptr->type != TYPE_DWF_SEDNOID  ? "km" : "R🜨")),
                                                                                                                    planet_ptr->bulk_rock *100.0,
                      planet_ptr->surf_grav,                                                                        planet_ptr->bulk_water*100.0,
                      planet_ptr->esc_v/1000.0,                                                                     planet_ptr->bulk_ices *100.0,
                      planet_ptr->surf_temp-KELVIN,                                                                 planet_ptr->bulk_noble*100.0,
                      planet_ptr->albedo*100.0,                                                                     planet_ptr->bulk_dens,

                      planet_ptr->a,
                      planet_ptr->e*100.0,
                      planet_ptr->i,
                      planet_ptr->l,
                      planet_ptr->p,
                      planet_ptr->v,
                      planet_ptr->y,
                      planet_ptr->day_length,
                      planet_ptr->axial_tilt
    );

    /*
    sprintf(text,    "        Properties:          "   "      Bulk Composition:     "   "              Ring System:"
                "\n" "Mass         =  %10s     "       "Density    = %6.3f g/cm³   "
                "\n" "Radius       =  %10s     "       "Metal mass = %6.3f %%       "
                "\n" "Gravity      = %8.3f G      "    "Rock  mass = %6.3f %%       "
                "\n" "Esc velocity = %8.3f km/s   "    "Water mass = %6.3f %%       "
                "\n" "Average temp = %8.3f °C     "    "Ices  mass = %6.3f %%       "
                "\n" "Bond albedo  = %8.3f %%      "   "H+He  mass = %6.3f %%       "
                "\n" "                               " "                            "
                "\n" "        Atmosphere:            " "     Position Parameters:   "
                "\n" "                               " "Semimajor axis   = %7.3f AU  "
                "\n" "                               " "Eccentricity     = %7.3f %%  "
                "\n" "                               " "Inclination      = %+7.3f °  "
                "\n" "                               " "Longitude of AN  = %7.3f °   "
                "\n" "                               " "Arg of Periapsis = %7.3f °   "
                "\n" "                               " "Orbital velocity = %7.3f km/s"
                "\n" "                               " "Year length      = %7.3f y🜨  "
                "\n" "                               " "Day  length      = %7.3f hr  "
                "\n" "                               " "Axial tilt       = %7.3f °   ",
    );
    */

    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy(text, 27, 0);

    Movetoxy(27, rows+1);
    printf("╣");

    //Print atmosphere info

    if (planet_ptr->has_atmo)
    {
        sprintf(text, "┼──────────────┬──────────────"
                    "\n│"
                    "\n│"
                    "\n│"
                    "\n│"
                    "\n│"
                    "\n│"
                    "\n│"
                    "\n│  Molar Wgt.  │ %6.2f kg/mol"
                    "\n│  Scale Hgt.  │ %6.2f km    "
                    "\n╧══════════════╧══════════════",
            planet_ptr->atmo_dens,
            planet_ptr->atmo_high/1000.0
        );

        print_at_xy(text, 59, 11);

        int colors[24] = {
            COLOR_ATMO_HYDROGEN, //Hydrogen
            COLOR_ATMO_HYDROGEN, //Helium
            COLOR_ATMO_ARGON   , //Argon
            COLOR_ATMO_NITROGEN, //Nitrogen
            COLOR_ATMO_OXYGEN  , //Oxygen
            COLOR_ATMO_FLUORINE, //Fluorine
            COLOR_ATMO_CHLORINE, //Chlorine
            COLOR_ATMO_BROMINE , //Bromine
            COLOR_ATMO_IODINE  , //Iodine
            COLOR_ATMO_NEON    , //Neon
            COLOR_ATMO_KRYPTON , //Krypton
            COLOR_ATMO_XENON   , //Xenon
            COLOR_ATMO_HYDROGEN, //Deuteride
            COLOR_ATMO_CARBON  , //Methane
            COLOR_ATMO_CARBON  , //Ethane
            COLOR_ATMO_NITROGEN, //Ammonia
            COLOR_ATMO_PHOSPHOR, //Phosphine
            COLOR_ATMO_WATER   , //Water
            COLOR_ATMO_SULFUR  , //Hydrogen sulfide
            COLOR_ATMO_CARBON  , //Carbon oxides
            COLOR_ATMO_SULFUR  , //Sulfur oxides
            COLOR_ATMO_NITROGEN, //Nitrogen oxides
            COLOR_ATMO_THOLINS , //Tholins
            COLOR_ATMO_NITROGEN  //Cyanide
        };

        char *atmo_names[] = {
            "Hydrogen",
            "Helium",
            "Argon",
            "Nitrogen",
            "Oxygen",
            "Fluorine",
            "Chlorine",
            "Bromine",
            "Iodine",
            "Neon",
            "Krypton",
            "Xenon",
            "Deuteride",
            "Methane",
            "Ethane",
            "Ammonia",
            "Phosphine",
            "Water",
            "H-Sulfide",
            "C-Oxides",
            "S-Oxides",
            "N-Oxides",
            "Tholins",
            "Cyanides"
        };

        double atmosphere[24];

        for (int i = 0; i < 24; i++)
            atmosphere[i] = planet_ptr->atmosphere[i];

        //Sort the components in descending order
        for (int i = 0; i < 23; i++)
        {
            for (int j = 0; j < 23-i; j++)
            {
                if (atmosphere[j] < atmosphere[j+1])
                {
                    double temp_d   = atmosphere[j+1];
                    atmosphere[j+1] = atmosphere[j]  ;
                    atmosphere[j]   = temp_d         ;

                    char *temp_c    = atmo_names[j+1];
                    atmo_names[j+1] = atmo_names[j]  ;
                    atmo_names[j]   = temp_c         ;

                    int temp_i  = colors[j+1];
                    colors[j+1] = colors[j]  ;
                    colors[j]   = temp_i     ;
                }
            }
        }

        for (int i = 0; i < 7; i++)
        {
            //Determine unit
            if ((atmosphere[i])*1000000000000.0 < 1.0)
                continue;
            else if ((atmosphere[i])*1000000000.0 < 1.0)
            {
                sprintf(text, "%-11s │ %9.5f ppt",
                    atmo_names[i],
                    atmosphere[i]*1000000000000.0
                );
            }
            else if ((atmosphere[i])*1000000.0 < 1.0)
            {
                 sprintf(text, "%-11s │ %9.5f ppb",
                    atmo_names[i],
                    atmosphere[i]*1000000000.0
                );
            }
            else if ((atmosphere[i])*1000000.0 < 1000.0)
            {
                sprintf(text, "%-11s │ %9.5f ppm",
                    atmo_names[i],
                    atmosphere[i]*1000000.0
                );
            }
            else
            {
                sprintf(text, "%-11s │ %9.5f %%",
                    atmo_names[i],
                    atmosphere[i]*100.0
                );
            }

            //Print
            Set_Color(colors[i]);
            print_at_xy(text, 62, 12+i);
        }

        Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
        print_at_xy("│\n│\n│\n│\n│\n│\n│", 74, 12);

            /*
            sprintf(text, "\n╟───────────────┬──────────────"
                          "\n║  Carbons      │   %8.5f %%"
                          "\n║  Hydrogen     │   %8.5f %%"
                          "\n╣  Nitrogens    │   %8.5f %%"
                          "\n║  Noble gases  │   %8.5f %%"
                          "\n║  Oxygen       │   %8.5f %%"
                          "\n║  Sulfurs      │   %8.5f %%"
                          "\n║  Volatiles    │   %8.5f %%"
                          "\n║  Water vapor  │   %8.5f %%"
                          "\n║  Other gases  │   %8.5f %%"
                          "\n╠═══════════════╧══════════════",
                planet_ptr->atmo_carbons*100.0,
                planet_ptr->atmo_hydrogen*100.0,
                planet_ptr->atmo_nitros*100.0,
                planet_ptr->atmo_nobles*100.0,
                planet_ptr->atmo_oxygen*100.0,
                planet_ptr->atmo_sulfurs*100.0,
                planet_ptr->atmo_volatiles*100.0,
                planet_ptr->atmo_water*100.0,
                planet_ptr->atmo_trace_gas*100.0
            );
            print_at_xy(text, 27, 10);

        sprintf(text, "Carbons      = %8.5f %%    "
                 "\n" "Hydrogen     = %8.5f %%    "
                 "\n" "Nitrogens    = %8.5f %%    "
                 "\n" "Noble gases  = %8.5f %%    "
                 "\n" "Oxygen       = %8.5f %%    "
                 "\n" "Sulfurs      = %8.5f %%    "
                 "\n" "Volatiles    = %8.5f %%    "
                 "\n" "Water vapor  = %8.5f %%    "
                 "\n" "Other gases  = %8.5f %%    ",
        );
        */
    }
    else
    {
        print_at_xy("┼──────────────┴──────────────"
                  "\n│          Atmosphere         "
                  "\n┼─────────────────────────────"
                  "\n│                             "
                  "\n│                             "
                  "\n│                             "
                  "\n│            (N/A)            "
                  "\n│                             "
                  "\n│                             "
                  "\n│                             "
                  "\n│                             "
                  "\n│                             "
                  "\n╧═════════════════════════════",
            59,
            9
        );
    }

    //Print ring info
    if (planet_ptr->rings != nullptr)
    {
        if (planet_ptr->type == TYPE_ICE_DWARF ||
            planet_ptr->type == TYPE_GAS_GIANT ||
            planet_ptr->type == TYPE_GAS_SUPER ||
            planet_ptr->type == TYPE_GAS_PUFFY ||
            planet_ptr->type == TYPE_GAS_HOT   ||
            planet_ptr->type == TYPE_ICE_GIANT  )
        {
            print_at_xy("┬────────────────────────────┐"
                      "\n│           Rings            │"
                      "\n┼──────┬─────────────────────┤"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n┤      │                     │"
                      "\n│      │                     │"
                      "\n┤      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n│      │                     │"
                      "\n╧══════╧═════════════════════╡",
                90,
                0
            );

            int j = 0;
            for (int i = 0; i < planet_ptr->num_rings; i++)
            {
                sprintf(text, "%2i%s │ %6.3f to %6.3f R🜨",
                    j+1,
                    get_ordinal(j),
                    planet_ptr->rings[i],
                    planet_ptr->rings[i+1]
                );
                i++;

                print_at_xy(text, 92, 2+(int)ceil((double)i/2.0));
                j++;
            }
        }
    }
    else
    {
        print_at_xy("┬────────────────────────────┐"
                  "\n│           Rings            │"
                  "\n┼────────────────────────────┤",
            90,
            0
        );

        sprintf(text, "\n\n\n\n\n\n\n\n     (No rings)");
        print_at_xy(text, 95, 3);
    }

    free((void*)text  );
}

//This function prints the properties of the selected asteroid belt to the screen.
void print_belt_props(struct Planet *planet_ptr)
{
    char *text = (char*)malloc(sizeof(char)*3500);
    if (text == nullptr)
        crash(-162);

    sprintf(text, "╥───────────────────────────────"   "┬──────────────────────────────"   "┬────────────────────────────┐"
                "\n║           Properties          "   "│   Composition of Asteroids   "   "│        Kirkwood Gaps       │"
                "\n╟───────────────┬───────────────"   "┼──────────────┬───────────────"   "┼────────────────────────────┤"
                "\n║  Mass         │ %8.3f %2s   "     "│  Carbonous   │  %6.3f %%     "   "│"
                "\n║  Width        │  %7.3f AU   "     "│  Siliceous   │  %6.3f %%     "   "│"
                "\n║  Height       │ ±%7.3f °    "     "│  Metallic    │  %6.3f %%     "   "│"
                "\n║               │               "   "│  Icy         │  %6.3f %%     "   "│"
                "\n║               │               "   "│              │               "   "│"
                "\n║  Avg. albedo  │  %7.3f %%    "    "│  Avg. dens.  │  %6.3f g/cm³ "    "│"
                "\n╟───────────────┴───────────────"   "┼──────────────┴───────────────"   "┤"
                "\n║       Position Parameters     "   "│                              "   "│"
                "\n╟───────────────┬───────────────"   "┤                              "   "│"
                "\n║  Average a    │ %8.3f AU   "      "│                              "   "│"
                "\n║  Average e    │ %8.3f %%    "     "│                              "   "│"
                "\n║  Average i    │ %+8.3f °    "     "│                              "   "│"
                "\n║  Average Ω    │ %8.3f °    "      "│                              "   "│"
                "\n║  Average ω    │ %8.3f °    "      "│                              "   "│"
                "\n║  Avg. vel.    │ %8.3f km/s "      "│                              "   "│"
                "\n║  Avg. year    │ %8.3f y🜨   "      "│                              "   "│"
                "\n║               │               "   "│                              "   "│"
                "\n║               │               "   "│                              "   "│"
                "\n╠═══════════════╧═══════════════"   "╧══════════════════════════════"   "╧",

                      planet_ptr->mass, (planet_ptr->mass < 0.001 ? "Zg" : "M🜨"),
                      planet_ptr->bulk_carbon*100.0,
                      planet_ptr->radius,
                      planet_ptr->bulk_rock*100.0,
                      planet_ptr->height,
                      planet_ptr->bulk_metal*100.0,
                      planet_ptr->bulk_ices*100.0,
                      planet_ptr->albedo*100.0,
                      planet_ptr->bulk_dens,
                      planet_ptr->a,
                      planet_ptr->e*100.0,
                      planet_ptr->i,
                      planet_ptr->l,
                      planet_ptr->p,
                      planet_ptr->v,
                      planet_ptr->y
    );

    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy(text, 27, 0);

    Movetoxy(27, rows+1);
    printf("╣");

    //Print gap info
    if (planet_ptr->rings != nullptr)
    {
        print_at_xy("┬────────────────────────────┐"
                  "\n│        Kirkwood Gaps       │"
                  "\n┼──────┬─────────────────────┤"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n┤      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n│      │                     │"
                  "\n╧══════╧═════════════════════╡",
                90,
                0
            );

            char *resonances[] = {
                "5:1",
                "4:1",
                "3:1",
                "5:2",
                "7:3",
                "2:1",
                "3:2",
                "4:3"
            };

            for (int i = 0; i < planet_ptr->num_rings; i++)
            {
                sprintf(text, " %s │ %6.3f AU",
                    resonances[i],
                    planet_ptr->rings[i]
                );

                print_at_xy(text, 92, 3+i);
            }
    }
    else
    {
        print_at_xy("┬────────────────────────────┐"
                  "\n│        Kirkwood Gaps       │"
                  "\n┼────────────────────────────┤",
            90,
            0
        );

        sprintf(text, "\n\n\n\n\n\n\n\n        (N/A)");
        print_at_xy(text, 95, 3);
    }

    free((void*)text);
}

//This function prints the properties of the selected moon to the screen.
void print_moon_props(struct Moon *moon_ptr)
{
    char *text = (char*)malloc(sizeof(char)*4000);
    if (text == nullptr)
        crash(-163);

    sprintf(text, "╥───────────────────────────────"  "┬──────────────────────────────"   "┬"
                "\n║           Properties          "  "│       Bulk Composition       "   "│"
                "\n╟───────────────┬───────────────"  "┼──────────────┬───────────────"   "┤"
                "\n║  Mass         │ %8.3f %2s   "    "│  Density     │  %6.3f g/cm³ "    "│"
                "\n║  Radius A     │ %8.3f km   "     "│  Metal mass  │  %6.3f %%     "   "│"
                "\n║  Radius B     │ %8.3f km   "     "│  Rocky mass  │  %6.3f %%     "   "│"
                "\n║  Radius C     │ %8.3f km   "     "│  Water mass  │  %6.3f %%     "   "│"
                "\n║  Gravity      │ %8.3f m/s² "     "│  Ices  mass  │  %6.3f %%     "   "│"
                "\n║  Escape vel.  │ %8.3f m/s  "     "│  H₂+He mass  │  %6.3f %%     "   "│"
                "\n╟───────────────┴───────────────"  "┼──────────────┴───────────────"   "┘"
                "\n║       Position Parameters     "  "│                              "   " "
                "\n╟───────────────┬───────────────"  "┤                              "   " "
                "\n║  Smjr. axis   │ %8.3f %2s   "    "│                              "   " "
                "\n║  Eccentricity │ %8.3f %%    "    "│                              "   " "
                "\n║  Inclination  │ %+8.3f °    "    "│                              "   " "
                "\n║  Longitude    │ %8.3f °    "     "│                              "   " "
                "\n║  Argument     │ %8.3f °    "     "│                              "   " "
                "\n║  Orbit vel.   │ %8.3f km/s "     "│                              "   " "
                "\n║  Year length  │ %8.3f %2s   "    "│                              "   " "
                "\n║  Day  length  │ %8.3f hr   "     "│                              "   " "
                "\n║  Axial tilt   │ %8.3f °    "     "│                              "   " "
                "\n╠═══════════════╧═══════════════"  "╧══════════════════════════════"   "═",

                      (moon_ptr->mass < 0.001 ? moon_ptr->mass*5972200.0 : moon_ptr->mass), (moon_ptr->mass < 0.001 ? "Zg" : "M🜨"),
                                                      moon_ptr->bulk_dens,
                      moon_ptr->rad_a,                moon_ptr->bulk_metal*100.0,
                      moon_ptr->rad_b,                moon_ptr->bulk_rock*100.0,
                      moon_ptr->rad_c,                0.0,
                      moon_ptr->surf_grav*GRAVITY,    moon_ptr->bulk_ices*100.0,
                      moon_ptr->esc_v,                0.0,

                                                        (moon_ptr->a >= 9999.0 ? moon_ptr->a*(6378000.0/149597870700.0) : moon_ptr->a), ((moon_ptr->a < 9999.0) ? "R🜨" : "AU"),
                                                        moon_ptr->e*100.0,
                                                        moon_ptr->i,
                                                        moon_ptr->l,
                                                        moon_ptr->p,
                                                        moon_ptr->v,
                                                        (moon_ptr->y >= 1000.0 ? moon_ptr->y/EARTH_YEAR : moon_ptr->y), (moon_ptr->y >= 1000.0 ? "y🜨" : "d🜨"),
                                                        moon_ptr->day_length,
                                                        moon_ptr->axial_tilt
    );

    /*
    char *text = (char*)malloc(sizeof(char)*(1500));

    sprintf(text, "          Size:          "  "       Bulk Composition:      "
                "\nMass     = %11s      "      "Density    = %6.3f g/cm³      "
                "\nRadius A = %8.3f km      "  "Metal mass = %6.3f %%          "
                "\nRadius B = %8.3f km      "  "Rock  mass = %6.3f %%          "
                "\nRadius C = %8.3f km      "  "Ices  mass = %6.3f %%          "
                "\n"
                "\n         Conditions:     "  "        Orbital Parameters:   "
                "\nGravity      = %7.3f m/s² " "Semimajor axis   = %10s  "
                "\nEsc velocity = %7.3f m/s  " "Eccentricity     = %7.3f %%   "
                "\nDay length   = %7.3f hr   " "Inclination      = %7.3f °   "
                "\nAxial tilt   = %7.3f °    " "Longitude of AN  = %7.3f °   "
                "\nYear length  = %10s   "     "Arg of Periapsis = %7.3f °   "
                "\n              (%10s)  "     "Orbital velocity = %7.3f km/s",

                   mass,                        moon_ptr->bulk_dens,
                   moon_ptr->rad_a,           moon_ptr->bulk_metal*100.0,
                   moon_ptr->rad_b,           moon_ptr->bulk_rock*100.0,
                   moon_ptr->rad_c,           moon_ptr->bulk_ices*100.0,
                   moon_ptr->surf_grav*9.80665, a,
                   moon_ptr->esc_v,           moon_ptr->e*100.0,
                   moon_ptr->day_length,      moon_ptr->i,
                   moon_ptr->axial_tilt,      moon_ptr->l,
                   year_earth,                  moon_ptr->p,
                   year_planet,                 moon_ptr->v
    );
    */

    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy(text, 27, 0);

    Movetoxy(27, rows+1);
    printf("╣");

    free((void*)text);
}



//This function prints a text description of a star.
void print_star_desc(struct Star *star_ptr)
{
    char *text = (char*)malloc(sizeof(char)*2500);
    if (text == nullptr)
        crash(-170);

    char *color = (char*)malloc(sizeof(char)*100);
    if (color == nullptr)
        crash(-171);

    if (star_ptr->bv <= 0.6)
        sprintf(color, "It is a cool white color to human eyes.");
    else if (star_ptr->bv <= 0.8)
        sprintf(color, "It is a warm white color to human eyes, like Sol.");
    else if (star_ptr->bv <= 1.4)
        sprintf(color, "It is a cream color to human eyes.");
    else
        sprintf(color, "It is an orange color to human eyes.");


    char *mag = (char*)malloc(sizeof(char)*100);
    if (mag == nullptr)
        crash(-172);

    if (star_ptr->rel_mag <= -26.8 - FUDGE_FACTOR)
        sprintf(mag, "brighter than Sol (-26.74)");
    else if (star_ptr->rel_mag <= -26.8 + FUDGE_FACTOR)
        sprintf(mag, "about as bright as Sol (-26.74)");
    else if (star_ptr->rel_mag <= -24.0)
        sprintf(mag, "slightly dimmer than Sol (-26.74)");
    else if (star_ptr->rel_mag <= -14.74)
        sprintf(mag, "brighter than our full moon (-12.74), but dimmer than Sol (-26.74)");
    else if (star_ptr->rel_mag <= -12.74 - FUDGE_FACTOR)
        sprintf(mag, "slightly brighter than our full moon (-12.74)");
    else if (star_ptr->rel_mag <= -12.74 + FUDGE_FACTOR)
        sprintf(mag, "about as bright as our full moon (-12.74)");
    else if (star_ptr->rel_mag <= -4.92 - FUDGE_FACTOR)
        sprintf(mag, "brighter than Venus (-4.92), but dimmer than our full moon (-12.74)");
    else if (star_ptr->rel_mag <= -4.92 + FUDGE_FACTOR)
        sprintf(mag, "about as bright as Venus (-4.92)");
    else if (star_ptr->rel_mag <= -2.94 - FUDGE_FACTOR)
        sprintf(mag, "brighter than Jupiter or Mars (-2.94), but dimmer than Venus (-4.92)");
    else if (star_ptr->rel_mag <= -2.9 + FUDGE_FACTOR)
        sprintf(mag, "about as bright as Jupiter or Mars (-2.94)");
    else if (star_ptr->rel_mag <= -1.47 - FUDGE_FACTOR)
        sprintf(mag, "brighter than Sirius (-1.47), but dimmer than Jupiter or Mars (-2.94)");
    else if (star_ptr->rel_mag <= -1.47 + FUDGE_FACTOR)
        sprintf(mag, "about as bright as Sirius (-1.47)");
    else if (star_ptr->rel_mag <= 1.25 - FUDGE_FACTOR)
        sprintf(mag, "brighter than Deneb (1.25), but dimmer than Sirius (-1.47)");
    else if (star_ptr->rel_mag <= 1.25 + FUDGE_FACTOR)
        sprintf(mag, "about as bright as Deneb (1.25)");
    else if (star_ptr->rel_mag <= 1.97 - FUDGE_FACTOR)
        sprintf(mag, "brighter than Polaris (1.97), but dimmer than Deneb (1.25)");
    else if (star_ptr->rel_mag <= 1.97 + FUDGE_FACTOR)
        sprintf(mag, "about as bright as Polaris (1.97)");
    else if (star_ptr->rel_mag <= 3.5 - FUDGE_FACTOR)
        sprintf(mag, "brighter than Tau Ceti (3.5), but dimmer than Polaris (1.97)");
    else if (star_ptr->rel_mag <= 3.5 + FUDGE_FACTOR)
        sprintf(mag, "about as bright as Tau Ceti (3.5)");
    else if (star_ptr->rel_mag <= 5.32 - FUDGE_FACTOR)
        sprintf(mag, "brighter than Uranus (5.32), but dimmer than Tau Ceti (3.5)");
    else if (star_ptr->rel_mag <= 5.32 + FUDGE_FACTOR)
        sprintf(mag, "about as bright as Uranus (5.32)");
    else if (star_ptr->rel_mag <= 6.5)
        sprintf(mag, "dimmer than Uranus (5.32)");
    else
        sprintf(mag, "too dim to detect with human eyes");

    sprintf(text, "This star is a main sequence star, with a spectral type %c. %s "
                  "From the habitable planet, it would have an apparent magnitude of %+.3f; appearing %s. "
                  "It is currently %.3f billion Earth years old, about a third of the way through its life.",
        star_ptr->type,
        color,
        star_ptr->rel_mag,
        mag,
        star_ptr->age
    );
    print_at_xy_wrapped(text, 30, 23, 87);

    free((void*)text );
    free((void*)color);
    free((void*)mag  );
}

//This function prints a text description of a planet.
void print_planet_desc(struct Planet *planet_ptr)
{
    char *text = (char*)malloc(sizeof(char)*2500);
    if (text == nullptr)
        crash(-173);

    //Determine the type of planet
    char *type = (char*)malloc(sizeof(char)*100);
    if (type == nullptr)
        crash(-174);

    switch (planet_ptr->type)
    {
        case TYPE_RCK_DENSE:
            sprintf(type, "a class-%c super-Mercury", planet_ptr->subtype[0]);
            break;
        case TYPE_RCK_DESRT:
            sprintf(type, "a class-%c rocky desert planet", planet_ptr->subtype[0]);
            break;
        case TYPE_WTR_HYCN:
            sprintf(type, "a class-H ocean planet, comprised %s",
                (
                    planet_ptr->subtype[1] == '1'
                    ? "primarily of rock"
                    : (
                        planet_ptr->subtype[1] == '3'
                        ? "primarily of water"
                        : "of roughly equal parts water and rock"
                    )
                )
            );
            break;
        case TYPE_WTR_GREEN:
            sprintf(type, "a class-K habitable ocean planet, comprised primarily of rock");
            break;
        case TYPE_RCK_GREEN:
            sprintf(type, "a class-E habitable terrestrial planet");
            break;
        case TYPE_DWF_PLNTSML:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky inner belt object");
            else
                sprintf(type, "an icy inner belt object");
            break;
        case TYPE_DWF_PLUTINO:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky plutino, orbiting in 2:3 mean motion resonance with the outermost giant");
            else
                sprintf(type, "an icy plutino, orbiting in 2:3 mean motion resonance with the outermost giant");
            break;
        case TYPE_DWF_CUBEWNO:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky cubewano, orbiting between the 2:3 and 1:2 mean motion resonances of the outermost giant");
            else
                sprintf(type, "an icy cubewano, orbiting between the 2:3 and 1:2 mean motion resonances of the outermost giant");
            break;
        case TYPE_DWF_TWOTINO:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky twotino, orbiting in 1:2 mean motion resonance with the outermost giant");
            else
                sprintf(type, "an icy twotino, orbiting in 1:2 mean motion resonance with the outermost giant");
            break;
        case TYPE_DWF_SCATTER:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky scattered disk object");
            else
                sprintf(type, "an icy scattered disk object");
            break;
        case TYPE_DWF_SEDNOID:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky sednoid, orbiting with extreme distance and eccentricity");
            else
                sprintf(type, "an icy sednoid, orbiting with extreme distance and eccentricity");
            break;
        case TYPE_GAS_GIANT:
            switch (planet_ptr->subtype[1])
            {
                case '1':
                    sprintf(type, "a class-1 gas giant");
                    break;
                case '2':
                    sprintf(type, "a class-2 gas giant");
            }
            break;
        case TYPE_GAS_SUPER:
            switch (planet_ptr->subtype[1])
            {
                case '1':
                    sprintf(type, "a class-1 gas supergiant");
                    break;
                case '2':
                    sprintf(type, "a class-2 gas supergiant");
            }
            break;
        case TYPE_GAS_PUFFY:
            sprintf(type, "a class-3 puffy gas giant");
            break;
        case TYPE_GAS_HOT:
            switch (planet_ptr->subtype[1])
            {
                case '4':
                    sprintf(type, "a class-4 hot Jupiter");
                    break;
                case '5':
                    sprintf(type, "a class-5 hot Jupiter");
            }
            break;
        case TYPE_ICE_DWARF:
            sprintf(type, "a gas dwarf, comprised %s",
                (
                    planet_ptr->subtype[1] == '1'
                    ? "primarily of volatile gases"
                    : (
                        planet_ptr->subtype[1] == '3'
                        ? "primarily of water"
                        : "of roughly equal parts water and volatiles"
                    )
                )
            );
            break;
        case TYPE_ICE_GIANT:
            sprintf(type, "an ice giant, comprised %s",
                (
                    planet_ptr->subtype[1] == '1'
                    ? "primarily of volatile gases"
                    : (
                        planet_ptr->subtype[1] == '3'
                        ? "primarily of water"
                        : "of roughly equal parts water and volatiles"
                    )
                )
            );
    }

    //Determine the type of atmosphere
    char *atmo = (char*)malloc(sizeof(char)*115);
    if (atmo == nullptr)
        crash(-175);
    if (planet_ptr->atmo_dens == 0.0)
        sprintf(atmo, "no atmosphere");
    else if (planet_ptr->atmosphere[0] > 0.5)
        sprintf(atmo, "a primordial atmosphere");
    else if (planet_ptr->type == TYPE_WTR_HYCN)
        sprintf(atmo, "a wet secondary (hycean) atmosphere");
    else if (planet_ptr->atmosphere[19] > 0.5)
        sprintf(atmo, "a dry secondary atmosphere");
    else
        sprintf(atmo, "a bio-terraformed atmosphere");

    //Determine the type of clouds
    char *clouds = (char*)malloc(sizeof(char)*60);
    if (clouds == nullptr)
        crash(-176);
    double cloud_cover = (planet_ptr->cld_cover_thk+planet_ptr->cld_cover_thn)*100.0;
    if (cloud_cover != 0.0)
        sprintf(clouds, ", with clouds covering about %.0f%% of the planet", cloud_cover);
    else if ((planet_ptr->type == TYPE_GAS_GIANT ||
              planet_ptr->type == TYPE_GAS_SUPER ||
              planet_ptr->type == TYPE_GAS_PUFFY ||
              planet_ptr->type == TYPE_GAS_HOT)  &&
              planet_ptr->subtype[1] == '1'       )
        sprintf(clouds, ", with ammonia-based clouds");
    else if ((planet_ptr->type == TYPE_GAS_GIANT ||
              planet_ptr->type == TYPE_GAS_SUPER ||
              planet_ptr->type == TYPE_GAS_PUFFY ||
              planet_ptr->type == TYPE_GAS_HOT)  &&
              planet_ptr->subtype[1] == '2'       )
        sprintf(clouds, ", with water vapor-based clouds");
    else if ((planet_ptr->type == TYPE_GAS_GIANT ||
              planet_ptr->type == TYPE_GAS_SUPER ||
              planet_ptr->type == TYPE_GAS_PUFFY ||
              planet_ptr->type == TYPE_GAS_HOT)  &&
              planet_ptr->subtype[1] == '3'       )
        sprintf(clouds, ", with little cloud cover");
    else if ((planet_ptr->type == TYPE_GAS_GIANT ||
              planet_ptr->type == TYPE_GAS_SUPER ||
              planet_ptr->type == TYPE_GAS_PUFFY ||
              planet_ptr->type == TYPE_GAS_HOT)  &&
              planet_ptr->subtype[1] == '4'       )
        sprintf(clouds, ", with alkali metal-based clouds");
    else if ((planet_ptr->type == TYPE_GAS_GIANT ||
              planet_ptr->type == TYPE_GAS_SUPER ||
              planet_ptr->type == TYPE_GAS_PUFFY ||
              planet_ptr->type == TYPE_GAS_HOT)  &&
              planet_ptr->subtype[1] == '5'       )
        sprintf(clouds, ", with silicate-based clouds");
    else if (planet_ptr->type == TYPE_ICE_DWARF ||
             planet_ptr->type == TYPE_ICE_GIANT  )
        sprintf(clouds, ", covered in clouds of volatile compounds and organic haze");
    else
        sprintf(clouds, "");

    //Determine the number and type of satellites
    int icy_major = 0, icy_minor = 0, rck_major = 0, rck_minor = 0;
    for (int i = 0; i < planet_ptr->num_moons; i++)
    {
        if (planet_ptr->moons[i].type == TYPE_SAT_MAJOR || planet_ptr->moons[i].type == TYPE_SAT_MOONB)
        {
            if (planet_ptr->moons[i].bulk_rock > planet_ptr->moons[i].bulk_ices)
                rck_major++;
            else
                icy_major++;
        }
        else
        {
            if (planet_ptr->moons[i].bulk_rock > planet_ptr->moons[i].bulk_ices)
                rck_minor++;
            else
                icy_minor++;
        }
    }

    char *moon_rck_major = (char*)malloc(sizeof(char)*30);
    if (moon_rck_major == nullptr)
        crash(-177);
    if (rck_major == 0)
        sprintf(moon_rck_major, "");
    else if (rck_major == 1)
        sprintf(moon_rck_major, "a rocky major moon");
    else
        sprintf(moon_rck_major, "%i rocky major moons", rck_major);

    char *moon_rck_minor = (char*)malloc(sizeof(char)*30);
    if (moon_rck_minor == nullptr)
        crash(-178);
    if (rck_minor == 0)
        sprintf(moon_rck_minor, "");
    else if (rck_minor == 1)
        sprintf(moon_rck_minor, "%s%sa rocky minor moon",
            (
                (rck_major)
                ? ", "
                : ""
            ),
            (
                (rck_major && !icy_major && !icy_minor)
                ? "and "
                : ""
            )
        );
    else
        sprintf(moon_rck_minor, "%s%s%i rocky minor moons",
            (
                (rck_major)
                ? ", "
                : ""
            ),
            (
                (rck_major && !icy_major && !icy_minor)
                ? "and "
                : ""
            ),
            rck_minor
        );

    char *moon_icy_major = (char*)malloc(sizeof(char)*30);
    if (moon_icy_major == nullptr)
        crash(-179);
    if (icy_major == 0)
        sprintf(moon_icy_major, "");
    else if (icy_major == 1)
        sprintf(moon_icy_major, "%s%san icy major moon",
            (
                (rck_major || rck_minor)
                ? ", "
                : ""
            ),
            (
                ((rck_major || rck_minor) && !icy_minor)
                ? "and "
                : ""
            )
        );
    else
        sprintf(moon_icy_major, "%s%s%i icy major moons",
            (
                (rck_major || rck_minor)
                ? ", "
                : ""
            ),
            (
                ((rck_major || rck_minor) && !icy_minor)
                ? "and "
                : ""
            ),
            icy_major
        );

    char *moon_icy_minor = (char*)malloc(sizeof(char)*30);
    if (moon_icy_minor == nullptr)
        crash(-180);
    if (icy_minor == 0)
        sprintf(moon_icy_minor, "");
    else if (icy_minor == 1)
        sprintf(moon_icy_minor, "%san icy minor moon",
            (
                (rck_major || rck_minor || icy_major)
                ? ", and "
                : ""
            )
        );
    else
        sprintf(moon_icy_minor, "%s%i icy minor moons",
            (
                (rck_major || rck_minor || icy_major)
                ? ", and "
                : ""
            ),
            icy_minor
        );

    //Check if the rings are visible
    char *rings = (char*)malloc(sizeof(char)*50);
    if (rings == nullptr)
        crash(-181);
    if (planet_ptr->rings != nullptr)
        if (planet_ptr->rings_vis)
            sprintf(rings, " The planet's rings are icy and clearly visible.");
        else
            sprintf(rings, " The planet's rings are dusty and faint.");
    else
        sprintf(rings, "");

    sprintf(text, "This planet is %s. It has %s%s.%s%s %i satellite%s orbit%s this planet%s%s%s%s%s.%s",
        type,
        atmo,
        clouds,
        (
            planet_ptr->feature == nullptr
            ? ""
            : planet_ptr->feature
        ),
        rings,
        planet_ptr->num_moons,
        (
            (planet_ptr->num_moons == 1)
            ? ""
            : "s"
        ),
        (
            (planet_ptr->num_moons == 1)
            ? "s"
            : ""
        ),
        (
            (planet_ptr->num_moons != 0)
            ? ": "
            : ""
        ),
        moon_rck_major,
        moon_rck_minor,
        moon_icy_major,
        moon_icy_minor,
        (
            planet_ptr->trojans == 1
            ? " It has also captured swarms of trojans from the asteroid belt."
            : ""
        )
    );

    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy_wrapped(text, 30, 23, 87);

    free((void*)type          );
    free((void*)atmo          );
    free((void*)clouds        );
    free((void*)rings         );
    free((void*)moon_icy_major);
    free((void*)moon_icy_minor);
    free((void*)moon_rck_major);
    free((void*)moon_rck_minor);
    free((void*)text          );
}

//This function prints a text description of an inner asteroid belt.
void print_belt_desc(struct Planet *planet_ptr, int num)
{
    char *text = (char*)malloc(sizeof(char)*2500);
    if (text == nullptr)
        crash(-182);

    sprintf(text, "The asteroid belt formed when mean motion resonances with the %i%s planet disrupted planet formation in this region.%s",
        num+1,
        get_ordinal(num),
        (
            planet_ptr->subtype[1] == '1'
            ? " It contains one dwarf planet."
            : " It does not contain a dwarf planet."
        )
    );

    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy_wrapped(text, 30, 23, 87);

    free((void*)text);
}

//This function prints a text description of a kuiper asteroid belt.
void print_kuiper_desc(struct Planet *planets, int num)
{
    char *text = (char*)malloc(sizeof(char)*2500);
    if (text == nullptr)
        crash(-183);

    int plutinos  = 0,
        cubewanos = 0,
        twotinos  = 0,
        scattered = 0,
        sednoids  = 0;

    for (int i = 0; i < num; i++)
    {
        switch (planets[i].subtype[0])
        {
            case TYPE_DWF_PLUTINO:
                plutinos++;
                break;
            case TYPE_DWF_CUBEWNO:
                cubewanos++;
                break;
            case TYPE_DWF_TWOTINO:
                twotinos++;
                break;
            case TYPE_DWF_SCATTER:
                scattered++;
                break;
            case TYPE_DWF_SEDNOID:
                sednoids++;
                break;
        }
    }

    char *s_plutino = (char*)malloc(sizeof(char)*30);
    if (s_plutino == nullptr)
        crash(-184);
    if (plutinos == 0)
        sprintf(s_plutino, "");
    else if (plutinos == 1)
        sprintf(s_plutino, "a plutino");
    else
        sprintf(s_plutino, "%i plutinos", plutinos);

    char *s_cubewano = (char*)malloc(sizeof(char)*30);
    if (s_cubewano == nullptr)
        crash(-185);
    if (cubewanos == 0)
        sprintf(s_cubewano, "");
    else if (cubewanos == 1)
        sprintf(s_cubewano, "%s%sa cubewano",
            (
                (plutinos)
                ? ", "
                : ""
            ),
            (
                (plutinos && !twotinos && !scattered && !sednoids)
                ? "and "
                : ""
            )
        );
    else
        sprintf(s_cubewano, "%s%s%i cubewanos",
            (
                (plutinos)
                ? ", "
                : ""
            ),
            (
                (plutinos && !twotinos && !scattered && !sednoids)
                ? "and "
                : ""
            ),
            cubewanos
        );

    char *s_twotino = (char*)malloc(sizeof(char)*30);
    if (s_twotino == nullptr)
        crash(-186);
    if (twotinos == 0)
        sprintf(s_twotino, "");
    else if (twotinos == 1)
        sprintf(s_twotino, "%s%sa twotino",
            (
                (plutinos || cubewanos)
                ? ", "
                : ""
            ),
            (
                ((plutinos || cubewanos) && !scattered && !sednoids)
                ? "and "
                : ""
            )
        );
    else
        sprintf(s_twotino, "%s%s%i twotinos",
            (
                (plutinos || cubewanos)
                ? ", "
                : ""
            ),
            (
                ((plutinos || cubewanos) && !scattered && !sednoids)
                ? "and "
                : ""
            ),
            twotinos
        );

    char *s_scattered = (char*)malloc(sizeof(char)*30);
    if (s_scattered == nullptr)
        crash(-187);
    if (scattered == 0)
        sprintf(s_scattered, "");
    else if (scattered == 1)
        sprintf(s_scattered, "%s%sa scattered disk object",
            (
                (plutinos || cubewanos || twotinos)
                ? ", "
                : ""
            ),
            (
                ((plutinos || cubewanos || twotinos) && !sednoids)
                ? "and "
                : ""
            )
        );
    else
        sprintf(s_scattered, "%s%s%i scattered disk objects",
            (
                (plutinos || cubewanos || twotinos)
                ? ", "
                : ""
            ),
            (
                ((plutinos || cubewanos || twotinos) && !sednoids)
                ? "and "
                : ""
            ),
            scattered
        );

    char *s_sednoid = (char*)malloc(sizeof(char)*30);
    if (s_sednoid == nullptr)
        crash(-188);
    if (sednoids == 0)
        sprintf(s_sednoid, "");
    else if (sednoids == 1)
        sprintf(s_sednoid, "%sa sednoid",
            (
                (plutinos || cubewanos || twotinos || scattered)
                ? ", and "
                : ""
            )
        );
    else
        sprintf(s_sednoid, "%s%i sednoids",
            (
                (plutinos || cubewanos || twotinos || scattered)
                ? ", and "
                : ""
            ),
            sednoids
        );


    sprintf(text, "The kuiper belt formed when gravitational interactions with the giant planets in the early system ejected material onto wide orbits. It contains %i objects of interest: %s%s%s%s%s.",
        plutinos+cubewanos+twotinos+scattered+sednoids,
        s_plutino,
        s_cubewano,
        s_twotino,
        s_scattered,
        s_sednoid
    );

    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy_wrapped(text, 30, 23, 87);

    free((void*)text);
    free((void*)s_plutino);
    free((void*)s_cubewano);
    free((void*)s_twotino);
    free((void*)s_scattered);
    free((void*)s_sednoid);
}

//This function prints a text description of a moon.
void print_moon_desc(struct Moon *moon_ptr)
{
    char *text = (char*)malloc(sizeof(char)*2500);
    if (text == nullptr)
        crash(-189);

    //Determine the type of moon
    char *type = (char*)malloc(sizeof(char)*100);
    if (type == nullptr)
        crash(-190);
    switch (moon_ptr->type)
    {
        case TYPE_SAT_MINOR:
        case TYPE_SAT_FOR_B:
        case TYPE_SAT_REV_B:
            if (moon_ptr->bulk_rock > moon_ptr->bulk_ices)
                sprintf(type, "a rocky minor moon");
            else
                sprintf(type, "an icy minor moon");
            break;
        case TYPE_SAT_MAJOR:
        case TYPE_SAT_MOONB:
            if (moon_ptr->bulk_rock > moon_ptr->bulk_ices)
                sprintf(type, "a rocky major moon");
            else
                sprintf(type, "an icy major moon");
            break;
        case TYPE_SAT_MOONA:
            if (moon_ptr->bulk_rock > moon_ptr->bulk_ices)
                sprintf(type, "a type-A rocky minor moon");
            else
                sprintf(type, "a type-A icy minor moon");
            break;
        case TYPE_SAT_MOONC:
            if (moon_ptr->bulk_rock > moon_ptr->bulk_ices)
                sprintf(type, "a type-C%c captured rocky asteroid", moon_ptr->subtype);
            else
                sprintf(type, "a type-C%c captured icy asteroid", moon_ptr->subtype);
            break;
        default:
            sprintf(type, "GENERATION ERROR : TYPE %c", moon_ptr->type);
    }

    //If the moon is a lagrangian companion, determine which moon it is companion to
    int index = 0, host_index = -1, i;
    if (moon_ptr->type == TYPE_SAT_FOR_B || moon_ptr->type == TYPE_SAT_REV_B)
    {
        //First find this moon
        for (i = 0; i < moon_ptr->host->num_moons; i++)
            if (moon_ptr->host->moons[i].mass == moon_ptr->mass)
                break;

        index = i;

        //Now find the major moon immediately previous
        if (moon_ptr->host->moons[i-1].type == TYPE_SAT_MOONB)
            host_index = index-1;
        else
            host_index = index-2;
    }

    //If the moon is resonant to another moon, determine which one
    double resonance, precision = 5.0;
    int res_index = -1;
    for (i = 0; i < moon_ptr->host->num_moons; i++)
    {
        resonance = resonant_dist(moon_ptr->host->moons[i].a, 1.0,2.0);
        if ((resonance*(1.0-(FUDGE_FACTOR/precision)) <= moon_ptr->a) && (moon_ptr->a <= resonance*(1.0+(FUDGE_FACTOR/precision)))) //res_index is a major moon causing the gap
        {
            res_index = i;
            break;
        }

        resonance = resonant_dist(moon_ptr->host->moons[i].a, 2.0,1.0);
        if ((resonance*(1.0-(FUDGE_FACTOR/precision)) <= moon_ptr->a) && (moon_ptr->a <= resonance*(1.0+(FUDGE_FACTOR/precision)))) //res_index is a minor moon caught in the gap
        {
            res_index = i;
            break;
        }
    }

    char *desc = (char*)malloc(sizeof(char)*110);
    if (desc == nullptr)
        crash(-191);
    else if (moon_ptr->type == TYPE_SAT_FOR_B)
        sprintf(desc, " It is an L₄ (forward) lagrangian companion to the %i%s moon.",
            host_index+1,
            get_ordinal(host_index+1)
        );
    else if (moon_ptr->type == TYPE_SAT_REV_B)
        sprintf(desc, " It is an L₅ (reverse) lagrangian companion to the %i%s moon.",
            host_index+1,
            get_ordinal(host_index+1)
        );
    else if (moon_ptr->type == TYPE_SAT_MOONA && moon_ptr->a < moon_ptr->host->rings_max)
    {
        if (res_index >= 0)
            sprintf(desc, " It orbits within a resonant gap in the rings caused by the %i%s moon.",
                res_index+1,
                get_ordinal(res_index)
            );
        else
            sprintf(desc, " It orbits within its host's ring system, causing gaps in the rings as it knocks material out of its orbit.");
    }
    else if (moon_ptr->type == TYPE_SAT_MOONA)
    {
        if (res_index >= 0)
            sprintf(desc, " It orbits just outside its host's ring system, caught in a 1:2 mean motion resonance with the %i%s moon.",
                res_index+1,
                get_ordinal(res_index)
            );
        else
            sprintf(desc, " It orbits just outside its host's ring system.");

    }
    else if (moon_ptr->host->rings_min < (moon_ptr->a / pow(2, 2.0/3.0)) && (moon_ptr->a / pow(2, 2.0/3.0)) < moon_ptr->host->rings_max)
    {
        if (res_index >= 0)
            sprintf(desc, " The %i%s moon has a 1:2 mean motion resonance with it, causing a substantial gap in the rings.",
                res_index+1,
                get_ordinal(res_index)
            );
        else
            sprintf(desc, " There is a major gap in the planet's ring system at its 1:2 mean motion resonance.");
    }
    else
        sprintf(desc, " There is nothing particularly interesting about it.");

    sprintf(text, "This moon is %s. Its surface is visibly cratered.%s",
        type,
        desc
    );
    print_at_xy_wrapped(text, 30, 23, 87);

    free((void*)type);
    free((void*)text);
    free((void*)desc);
}

//This function prints a text description of a dwarf planet.
void print_dwarf_desc(struct Planet *planet_ptr)
{
    char *text = (char*)malloc(sizeof(char)*2500);
    if (text == nullptr)
        crash(-71);

    //Determine the type of dwarf planet
    char *type = (char*)malloc(sizeof(char)*100);
    if (type == nullptr)
        crash(-72);
    switch (planet_ptr->type)
    {
        case TYPE_DWF_PLNTSML:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky inner belt object");
            else
                sprintf(type, "an icy inner belt object");
            break;
        case TYPE_DWF_PLUTINO:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky plutino, orbiting in 2:3 mean motion resonance with the outermost giant");
            else
                sprintf(type, "an icy plutino, orbiting in 2:3 mean motion resonance with the outermost giant");
            break;
        case TYPE_DWF_CUBEWNO:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky cubewano, orbiting bewteen the 2:3 and 1:2 mean motion resonances of the outermost giant");
            else
                sprintf(type, "an icy cubewano, orbiting bewteen the 2:3 and 1:2 mean motion resonances of the outermost giant");
            break;
        case TYPE_DWF_TWOTINO:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky twotino, orbiting in 1:2 mean motion resonance with the outermost giant");
            else
                sprintf(type, "an icy twotino, orbiting in 1:2 mean motion resonance with the outermost giant");
            break;
        case TYPE_DWF_SCATTER:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky scattered disk object");
            else
                sprintf(type, "an icy scattered disk object");
            break;
        case TYPE_DWF_SEDNOID:
            if (planet_ptr->bulk_rock > planet_ptr->bulk_ices)
                sprintf(type, "a rocky sednoid, orbiting with extreme distance and eccentricity");
            else
                sprintf(type, "an icy sednoid, orbiting with extreme distance and eccentricity");
            break;
        default:
            sprintf(type, "GENERATION ERROR : TYPE %c", planet_ptr->type);
    }

    //Determine the number and type of satellites
    int icy_major = 0, icy_minor = 0, rck_major = 0, rck_minor = 0;
    for (int i = 0; i < planet_ptr->num_moons; i++)
    {
        if (planet_ptr->moons[i].type == TYPE_SAT_MAJOR || planet_ptr->moons[i].type == TYPE_SAT_MOONB)
        {
            if (planet_ptr->moons[i].bulk_rock > planet_ptr->moons[i].bulk_ices)
                rck_major++;
            else
                icy_major++;
        }
        else
        {
            if (planet_ptr->moons[i].bulk_rock > planet_ptr->moons[i].bulk_ices)
                rck_minor++;
            else
                icy_minor++;
        }
    }

    char *moon_rck_major = (char*)malloc(sizeof(char)*30);
    if (moon_rck_major == nullptr)
        crash(-56);
    if (rck_major == 0)
        sprintf(moon_rck_major, "");
    else if (rck_major == 1)
        sprintf(moon_rck_major, "a rocky major moon");
    else
        sprintf(moon_rck_major, "%i rocky major moons", rck_major);

    char *moon_rck_minor = (char*)malloc(sizeof(char)*30);
    if (moon_rck_minor == nullptr)
        crash(-57);
    if (rck_minor == 0)
        sprintf(moon_rck_minor, "");
    else if (rck_minor == 1)
        sprintf(moon_rck_minor, "%s%sa rocky minor moon",
            (
                (rck_major)
                ? ", "
                : ""
            ),
            (
                (rck_major && !icy_major && !icy_minor)
                ? "and "
                : ""
            )
        );
    else
        sprintf(moon_rck_minor, "%s%s%i rocky minor moons",
            (
                (rck_major)
                ? ", "
                : ""
            ),
            (
                (rck_major && !icy_major && !icy_minor)
                ? "and "
                : ""
            ),
            rck_minor
        );

    char *moon_icy_major = (char*)malloc(sizeof(char)*30);
    if (moon_icy_major == nullptr)
        crash(-58);
    if (icy_major == 0)
        sprintf(moon_icy_major, "");
    else if (icy_major == 1)
        sprintf(moon_icy_major, "%s%san icy major moon",
            (
                (rck_major || rck_minor)
                ? ", "
                : ""
            ),
            (
                ((rck_major || rck_minor) && !icy_minor)
                ? "and "
                : ""
            )
        );
    else
        sprintf(moon_icy_major, "%s%s%i icy major moons",
            (
                (rck_major || rck_minor)
                ? ", "
                : ""
            ),
            (
                ((rck_major || rck_minor) && !icy_minor)
                ? "and "
                : ""
            ),
            icy_major
        );

    char *moon_icy_minor = (char*)malloc(sizeof(char)*30);
    if (moon_icy_minor == nullptr)
        crash(-59);
    if (icy_minor == 0)
        sprintf(moon_icy_minor, "");
    else if (icy_minor == 1)
        sprintf(moon_icy_minor, "%san icy minor moon",
            (
                (rck_major || rck_minor || icy_major)
                ? ", and "
                : ""
            )
        );
    else
        sprintf(moon_icy_minor, "%s%i icy minor moons",
            (
                (rck_major || rck_minor || icy_major)
                ? ", and "
                : ""
            ),
            icy_minor
        );

    sprintf(text, "This %s is %s. Its surface is visibly cratered. %i satellite%s orbit%s this dwarf%s%s%s%s%s.",
        (
            planet_ptr->type == TYPE_DWF_SEDNOID
            ? "asteroid"
            : "dwarf planet"
        ),
        type,
        planet_ptr->num_moons,
        (
            (planet_ptr->num_moons == 1)
            ? ""
            : "s"
        ),
        (
            (planet_ptr->num_moons == 1)
            ? "s"
            : ""
        ),
        (
            (planet_ptr->num_moons != 0)
            ? ": "
            : ""
        ),
        moon_rck_major,
        moon_rck_minor,
        moon_icy_major,
        moon_icy_minor
    );
    print_at_xy_wrapped(text, 30, 23, 87);

    free((void*)type);
    free((void*)moon_icy_major);
    free((void*)moon_icy_minor);
    free((void*)moon_rck_major);
    free((void*)moon_rck_minor);
    free((void*)text);
}


//This function prints a table of unit conversions.
void print_units(void)
{
    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy("╥───────────────────────────────────────────────┬───────────────────────────────────────────┐"
              "\n║                  Mass Units                   │                Length Units               │"
              "\n╟─────┬───────────────────────┬─────────────────┼────┬─────────────────────┬────────────────┤"
              "\n║  M☉ │  Solar Mass           │  2.000×10³⁰ kg  │ AU │  Astronomical Unit  │  1.496×10¹¹ m  │"
              "\n║  M♃ │  Jupiter Mass         │  1.898×10²⁷ kg  │ R☉ │  Solar Radius       │  6.957×10⁸  m  │"
              "\n║  M🜨 │  Earth Mass           │  5.972×10²⁴ kg  │ R♃ │  Jupiter Radius     │  72,492,000 m  │"
              "\n║  Zg │  Zettagram            │  1.000×10²¹ kg  │ R🜨 │  Earth Radius       │       6,378 m  │"
              "\n╟─────┴───────────────────────┴─────────────────┼────┴─────────────────────┴────────────────┤"
              "\n║                  Time Units                   │                                           │"
              "\n╟─────┬───────────────────────┬─────────────────┤                                           │"
              "\n║ by🜨 │  Billion Earth Years  │         10⁹ y🜨  │                                           │"
              "\n║  y🜨 │  Earth Year           │    365.2515 d🜨  │                                           │"
              "\n║  yₚ │  Local Year           │      varies     │                                           │"
              "\n║  d🜨 │  Earth day            │          24 hr  │                                           │"
              "\n║  dₚ │  Local day            │      varies     │                                           │"
              "\n╟─────┴───────────────────────┴─────────────────┤                                           │"
              "\n║                  Misc Units                   │                                           │"
              "\n╟─────┬───────────────────────┬─────────────────┤                                           │"
              "\n║  L☉ │  Solar Lumen          │  3.828×10²⁶ W   │                                           │"
              "\n║  T☉ │  Solar Temperature    │       5,772 K   │                                           │"
              "\n║   G │  Earth gravity        │       9.8 m/s²  │                                           │"
              "\n╠═════╧═══════════════════════╧═════════════════╧═══════════════════════════════════════════╡",
        27,
        0
    );

    Movetoxy(27, rows+1);
    printf("╣");

    print_at_xy_wrapped("This program regularly uses astronomical, unconventional, or otherwise unfamiliar units and abbreviations. "
                        "This page lists these units, their abbreviations, and their equivalent values in metric units.",
        30,
        23,
        87
    );
}

//This function prints a list of planet and class definitions.
void print_desc(void)
{
    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy("╥───────────────────────────────────────────────────────────────────────────────────────────┐"
              "\n║                                                                                           │"
              "\n║                                    TERRESTRIAL PLANETS                                    │"
              "\n║                                                                                           │"
              "\n║  Class-M: super-Mercury with no atmosphere. Named after Mercury.                          │"
              "\n║  Class-R: super-Mercury with a primordial atmosphere. Stands for 'rocky'.                 │"
              "\n║  Class-D: rocky planet with no atmosphere. Stands for 'desert'.                           │"
              "\n║  Class-V: rocky planet with a secondary atmosphere. Stands for 'volcanic'.                │"
              "\n║  Class-H: ocean planet with a secondary atmosphere. Stands for 'hycean'.                  │"
              "\n║  Class-K: ocean planet with a tertiary atmosphere. Named after Kamino.                    │"
              "\n║  Class-E: rocky planet with a tertiary atmosphere. Named after Earth.                     │"
              "\n║                                                                                           │"
              "\n║                                                                                           │"
              "\n║                                       GIANT PLANETS                                       │"
              "\n║                                                                                           │"
              "\n║  Class-1: Temperature below   -120 °C, albedo about 57%, and ammonia-based clouds.        │"
              "\n║  Class-2: Temperature between -120 and   52 °C, albedo about 80%, and water clouds.       │"
              "\n║  Class-3: Temperature between   52 and  580 °C, albedo about 12%, and no cloud cover.     │"
              "\n║  Class-4: Temperature between  580 and 1100 °C, albedo about  3%, and metallic clouds.    │"
              "\n║  Class-5: Temperature above   1100 °C, albedo about 55%, and silicate-based clouds.       │"
              "\n║                                                                                           │"
              "\n╠═══════════════════════════════════════════════════════════════════════════════════════════╡",
        27,
        0
    );

    Movetoxy(27, rows+1);
    printf("╣");

    print_at_xy_wrapped("Most planets are listed with a 'class' in their description. For gas giants, this class is Sudarsky's classification. "
                        "For terrestrial planets, the class is of my own invention. This page lists each class with a short description.",
        30,
        23,
        87
    );
}

//This function prints an explanation of the atmospheres.
void print_atmo(void)
{
    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy("Atmospheres in this program are composed of nine substances:"
              "\n"
              "\nHYDROGEN, OXYGEN, WATER VAPOR and OTHER GASES are exactly what they sound like."
              "\n"
              "\nNITROGENS are primarily diatomic nitrogen, with small quantities of oxides like NO, N₂O,"
              "\n          and NO₂."
              "\n"
              "\nCARBONS are primarily carbon dioxide, with a small amount of carbon monoxide."
              "\n"
              "\nSULFURS are primarily sulfur dioxide on dry worlds and primarily hydrogen sulfide on wet"
              "\n        worlds. Each contains small quantities of the other chemical and of sulfur"
              "\n        trioxide."
              "\n"
              "\nNOBLE GASES are primarily helium on giant worlds and primarily argon on terrestrial"
              "\n            worlds. This category does include all noble gases in each case."
              "\n"
              "\nVOLATILES are more complex gases like ammonia, methane, and other gases common in ice"
              "\n          giants.",
        30,
        2
    );
}

//This function prints an explanation of the orbital parameters.
void print_orbit(void)
{
    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy("╥────────────────────┬─────┬────────────────────────────────────────────────────────────────┐"
              "\n║  Semimajor Axis    │  a  │  The average distance between the body and the center of its   │"
              "\n║                    │     │  orbit. This center is not always the host it orbits.          │"
              "\n╟────────────────────┼─────┼────────────────────────────────────────────────────────────────┤"
              "\n║  Eccentricity      │  e  │  The orbit's deviation from a perfect circle.                  │"
              "\n╟────────────────────┼─────┼────────────────────────────────────────────────────────────────┤"
              "\n║                    │     │  The orbit's pitch relative to the ecliptic plane, defined as  │"
              "\n║  Inclination       │  i  │  the plane of the orbit of the habitable planet. Said planet   │"
              "\n║                    │     │  always has an i of 0°.                                        │"
              "\n╟────────────────────┼─────┼────────────────────────────────────────────────────────────────┤"
              "\n║                    │     │  The orbits's yaw relative to the origin of longitude, which   │"
              "\n║  Longitude of the  │  Ω  │  is the direction of the habitable planet's periapsis along    │"
              "\n║   Ascending Node   │     │  its line of apses. Said planet always has an Ω of 90°.        │"
              "\n╟────────────────────┼─────┼────────────────────────────────────────────────────────────────┤"
              "\n║                    │     │  The orbit's roll, defined by the angle between its periapsis  │"
              "\n║  Argument of       │  ω  │  and its ascending node, in the orbit's own plane. The         │"
              "\n║   Periapsis        │     │  habitable planet's ω is always undefined.                     │"
              "\n╟────────────────────┼─────┼────────────────────────────────────────────────────────────────┤"
              "\n║                    │     │  The angle between the object's current position and the       │"
              "\n║  True Anomaly      │  θ  │  origin of longitude, in the orbit's own plane. This program   │"
              "\n║                    │     │  does not provide this parameter.                              │"
              "\n╠════════════════════╧═════╧════════════════════════════════════════════════════════════════╡",
        27,
        0
    );

    Movetoxy(27, rows+1);
    printf("╣");

    print_at_xy_wrapped("An object's orbit is uniquely defined by six parameters. This page lists those parameters, their symbols, and their descriptions."
                      "\nThe periapsis is the point at which the body is closest to the host it orbits."
                      "\nThe line of apses is the line from the periapsis to the apoapsis (furthest point)."
                      "\nThe ascending node is where the orbit intersects the ecliptic plane from below.",
        30,
        23,
        87
    );
}

//This function prints the title screen.
void print_title(struct Star *star_ptr, struct Planet *planets, int num_planets)
{
    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy("╥───────────────────────────────────────────────────────────────────────────────────────────┐"
              "\n║                                                                                           │"
              "\n║                                                                                           │"
              "\n║                                    ____                           _                       │"
              "\n║                                   / __ \\                         | |                      │"
              "\n║                                  | /  \\/ ___  _ __  ___ _   _ ___| |_ ___ _ __ ___        │"
              "\n║                                  | |    / _ \\| '_ \\/ __| | | / __| __/ _ \\ '_ ` _ \\       │"
              "\n║                                  | \\__/\\ (_) | | | \\__ \\ |_| \\__ \\ ||  __/ | | | | |      │"
              "\n║                                   \\____/\\___/|_| |_|___/\\__, |___/\\__\\___|_| |_| |_|      │"
              "\n║                                   / __ \\                 __/ |   | |                      │"
              "\n║                                  | |  \\/ ___ _ __   ____|___/__ _| |_ ___  _ __           │"
              "\n║                                  | | __ / _ \\ '_ \\ / _ \\ '__/ _` | __/ _ \\| '__|          │"
              "\n║                                  | |_\\ \\  __/ | | |  __/ | | (_| | || (_) | |             │"
              "\n║                                   \\____/\\___|_| |_|\\___|_|  \\__,_|\\__\\___/|_|             │"
              "\n║                                                                                           │"
              "\n║                                             by TacticalHampster                           │"
              "\n║                                                                                           │"
              "\n║                                ◐                                                          │"
              "\n║                                                                                           │"
              "\n║                                                                                           │"
              "\n║                                                                                           │"
              "\n╠═══════════════════════════════════════════════════════════════════════════════════════════╡",
        27,
        0
    );

    Movetoxy(27, rows+1);
    printf("╣");

    //Randomize the stars
    int num_stars = 90;
    int choice = 0, x = 0, y = 0;
    int overlap = false;

    for (int i = 0; i < num_stars; i++)
    {
        x = (int)round(rand_double(28.0, 118.0));
        y = (int)round(rand_double( 1.0,  20.0));

        overlap =            ((62 <= x && x <= 113) && ( 5 <= y && y <=  8)); //Star overlaps "Consystem"
        overlap = overlap || ((62 <= x && x <= 109) && (10 <= y && y <= 13)); //Star overlaps "Generator"
        overlap = overlap || ((62 <= x && x <=  69) && ( 3 <= y && y <= 13)); //Star overlaps "C"
        overlap = overlap || ((94 <= x && x <=  98) && ( 3 <= y && y <= 13)); //Star overlaps "t"
        overlap = overlap || ((86 <= x && x <=  92) && ( 5 <= y && y <= 13)); //Star overlaps "y"
        overlap = overlap || ((73 <= x && x <=  92) && ( y == 15 ));          //Star overlaps "by TacticalHampster"
        overlap = overlap || (( x == 60 )           && ( y == 17 ));          //Star overlaps moon

        if (overlap)
        {
            i--;
            continue;
        }

        choice = (int)floor(rand_double(0.0, 3.0));
        if (choice == 0)
            Set_Color(0x08);
        else if (choice == 1)
            Set_Color(0x07);
        else
            Set_Color(0x0F);

        print_at_xy(".", x, y);
    }

    //Draw the star
    Set_Color(0xE0);
    print_at_xy("            |  "
              "\n            |  "
              "\n           /  "
              "\n         ,  ▄"
              "\n       ,  ▄"
              "\n___.   ▄▄"
              "\n▂▂▂▂▂",
        28,
        1
    );
    if (star_ptr->type == 'F')
        Set_Color(0xFF);
    else if (star_ptr->type == 'G')
        Set_Color(0xFE);
    else if (star_ptr->type == 'K')
        Set_Color(0xEF);

    //print_at_xy("░", 38, 4);
    //print_at_xy("░", 36, 5);
    //print_at_xy("░░", 32, 6);

    print_at_xy("▒▒▒▒▒▒▒▒▒▒▒░░"
              "\n▒▒▒▒▒▒▒▒▒▒░░ "
              "\n▒▒▒▒▒▒▒▒░░░ "
              "\n▒▒▒▒▒▒░░░░"
              "\n▒▒▒░░░░░"
              "\n░░░░",
        28,
        1
    );

    //Draw the gas giant
    Set_Color(0x06);
    print_at_xy("▁▂▂▁", 115, 13);
    print_at_xy("▗", 112, 15);
    print_at_xy("▝", 112, 16);

    Set_Color(0x60);
    print_at_xy("▆▅▅▆", 115, 18);

    Set_Color(0x0E);
    print_at_xy("▗", 113, 14);
    print_at_xy("▝", 113, 17);

    Set_Color(0xE4);
    print_at_xy("     ", 114, 14);
    print_at_xy("     ", 114, 17);

    Set_Color(0x64);
    print_at_xy("      \n      ", 113, 15);

    Set_Color(0x6E);
    print_at_xy("▆▅▅▆", 115, 17);
    print_at_xy("▆▅▅▆", 115, 14);

    Set_Color(0xE6);
    print_at_xy("▆▅▅▆", 115, 15);

    //Color the other planets
    if (planets[0].type == TYPE_RCK_DENSE)
        Set_Color(COLOR_PLANET_DENSE);
    else if (planets[0].type == TYPE_RCK_DESRT)
        Set_Color(COLOR_PLANET_DESRT);
    else if (planets[0].type == TYPE_GAS_PUFFY || planets[0].type == TYPE_GAS_HOT)
        Set_Color(COLOR_PLANET_GIANT3);
    print_at_xy("●", 42, 9);

    if (planets[1].type == TYPE_RCK_DENSE)
        Set_Color(COLOR_PLANET_DENSE);
    else if (planets[1].type == TYPE_RCK_DESRT)
        Set_Color(COLOR_PLANET_DESRT);
    else if (planets[0].type == TYPE_GAS_PUFFY || planets[0].type == TYPE_GAS_HOT)
        Set_Color(COLOR_PLANET_GIANT3);
    print_at_xy("●", 54, 7);

    char hab_ocean = '0';
    for (int i = 0; i < num_planets; i++)
    {
        if (planets[i].subtype[0] == 'K')
        {
            hab_ocean = planets[i].subtype[1];
            break;
        }
        else if (planets[i].type == TYPE_RCK_GREEN)
        {
            hab_ocean = '0';
            break;
        }
    }

    if (hab_ocean == '1')
        Set_Color(COLOR_PLANET_WATER1);
    else if (hab_ocean == '2')
        Set_Color(COLOR_PLANET_WATER2);
    else if (hab_ocean == '3')
        Set_Color(COLOR_PLANET_WATER3);
    else
        Set_Color(COLOR_PLANET_GREEN);

    print_at_xy(" ▗▄▄▖ "
              "\n      "
              "\n ▝--▘ ",
        54,
        17
    );

    if (hab_ocean == '1')
        Set_Color(COLOR_PLANET_WATER1*0x10);
    else if (hab_ocean == '2')
        Set_Color(COLOR_PLANET_WATER2*0x10);
    else if (hab_ocean == '3')
        Set_Color(COLOR_PLANET_WATER3*0x10);
    else
        Set_Color(COLOR_PLANET_GREEN*0x10);

    print_at_xy("    ", 55, 18);
    print_at_xy("▃▃", 56, 19);

    Set_Color(COLOR_DEFAULT_BACK + COLOR_DEFAULT_TEXT);
    print_at_xy_wrapped("This program has created a randomly generated Sol-like system, based on Artifexian's con-worlding series on YouTube. "
                        "Use the W-S keys to select options in the top-left box. Use the E-D keys to select options in the bottom-left box. "
                        "Press R to regenerate the system. Press Q to quit the program. "
                        "Have fun!",
        30,
        23,
        87
    );
}

#endif // UI_H
