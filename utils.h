#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <Windows.h>
#include <math.h>
#include "defs.h"

#ifndef UTILS_H
#define UTILS_H

double rand_double(double mn, double mx);
double expo_double(double mn, double mx, double lambda);
double fudge_double(double arg);
int Movetoxy(int x, int y );
void print_at_xy(char *text, int x, int y);
void print_log(char *text);
void crash(int num);

//Returns a random uniformly distributed double between min and max.
double rand_double(double mn, double mx)
{
    double factor = 10000000.0, random, within, tries = 0.0;
    do
    {
        random = (((mn*factor) + (double)(rand())/( (double)(RAND_MAX/((mx*factor)-(mn*factor)))))/factor);

        if (mn > mx)
            within = (mx <= random) && (random <= mn);
        else if (mn == mx)
            within = true;
        else
            within = (mn <= random) && (random <= mx);

        tries++;

        /*
        char logtext[100];
        sprintf(logtext, "Min = %f | Max = %f | Rand = %f | Within = %i\n",min,max,random,(int)within);
        print_log(logtext);
        */
    }
    while (!within && tries < 500);
    return random;
}

//Returns a random exponentially distributed double between min and max.
double expo_double(double mn, double mx, double lambda)
{
    double u_min = exp(-mn * lambda);
    double u_max = exp(-mx * lambda);
    double random;

    do
    {
        random = -log(rand_double(u_min, u_max)) / lambda;

        /*
        char logtext[100];
        sprintf(logtext, "Min = %f | Max = %f | Rand = %f | Within = %i\n",min,max,random,(int)!((random > max) || (random < min)));
        print_log(logtext);
        */
    }
    while ((random > mx) || (random < mn));

    return random;
}

//Randomly varies arg by FUDGE_FACTOR %.
double fudge_double(double arg)
{
    return rand_double(arg*(1.0-FUDGE_FACTOR), arg*(1.0+FUDGE_FACTOR));
}

//Returns the distance which is in the given resonant ratio with the given distance
double resonant_dist(double a, double numer, double denom)
{
    return (a * pow(numer, 2.0/3.0)) / pow(denom, 2.0/3.0);
}

//Moves the cursor to coords (x, y).
int Movetoxy(int x, int y )
{
    COORD xy;

    xy.X = x;
    xy.Y = y;
    int value = SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), xy );

    if (!value)
        crash(-1);

    return value;
}

//Prints text starting at coords (x, y).
void print_at_xy(char *text, int x, int y)
{
    int cur_y = y;

    Movetoxy(x, cur_y);
    for (int i = 0; true; i++)
    {
        if (text[i] == '\n')
        {
            cur_y++;
            Movetoxy(x, cur_y);
            continue;
        }

        if (text[i] == '\0')
            break;

        if ((unsigned)text[i] < 0x02)
            continue;

        putchar(text[i]);
    }
}

/**
 * This function displays text at a given location, wrapped to be a given max width.
 * @param char[] text  - the text to display
 * @param int    x     - the X position to display at
 * @param int    y     - the Y position to display at
 * @param int    width - the maximum width of the text
 */
void print_at_xy_wrapped(char *text, int x, int y, int width)
{
    //Move to the given coords
    Movetoxy(x, y);
    //Line character counter
    int line = 0;
    //For each character in the string
    for (int i = 0; text[i] != '\0'; i++)
    {
        //If the character is a newline
        if (text[i] == '\n')
        {
            //Move to the next line
            y++;
            Movetoxy(x, y);
            //Reset the line character counter
            line = 0;
            continue;
        }

        //Type the character
        putchar(text[i]);
        //Increment the character counter
        line++;

        //If this character is at the end of a word
        if (text[i] == ' ')
        {
            int next = 0;
            //Scan forward to the end of the next word or the end of the string
            for (int j = i+1; (text[j] != '\0'); j++)
            {
                next++;
                //If the next word goes over the max width
                if (line+next > width)
                {
                    //Move to the next line
                    y++;
                    Movetoxy(x, y);
                    //Reset the line character counter
                    line = 0;
                    break;
                }

                //If the loop reaches the next space before going over, then it's safe to continue
                if (text[j] == ' ' || text[j] == '\0')
                    break;
            }
        }
    }
}

//Gets the ordinal suffix corresponding to num.
char* get_ordinal(int num)
{
    switch (num)
        {
            case 10:
            case 11:
            case 12:
            case 13:
                return "th";
            default:
                switch (num%10)
                {
                    case 0:
                        return "st";
                    case 1:
                        return "nd";
                    case 2:
                        return "rd";
                    default:
                        return "th";
                }
        }
}

//Sets the cursor color.
void Set_Color(int color)
{
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), color);
}

//Prints text to the log file.
void print_log(char *text)
{
    FILE *log = fopen("log.txt", "a");

    fprintf(log, text);

    fclose(log);
}

//Moves the cursor off the main screen and displays crash message.
void crash(int num)
{
    Movetoxy(0, 29);
    Set_Color(0x07);

    if (num >= 0)
        exit(num);

    printf("Error code %i: ", num);

    /*
    001-099        special
    100-199        memory
        100-109        top-level gen function
        110-119        generating star
        120-129        generating planet foundations
        130-139        generating atmosphere
        140-149        generating satellite system
        150-159        drawing screen
        160-169        displaying statblocks
        170-199        displaying descriptions
    200-299        generation errors
        200-209        top-level gen function
        210-219        generating star
        220-229        generating planet foundations
        230-239        generating planet's orbit
        240-249        generating atmosphere
        250-259        generating satellite system
    300-399        display errors
    */

    switch (num)
    {
        case -1:
            printf("failed to move cursor, coordinates were too large");
            break;
        case -100:
            printf("could not malloc main planet array");
            break;
        case -101:
            printf("could not malloc inner belt gap array");
            break;
        case -110:
            printf("could not malloc star");
            break;
        case -111:
            printf("could not malloc photosphere array");
            break;
        case -120:
            printf("could not malloc orbit array");
            break;
        case -121:
            printf("could not malloc initial planet types array");
            break;
        case -122:
            printf("could not malloc final planet types array");
            break;
        case -130:
            printf("could not malloc atmosphere components array");
            break;
        case -131:
            printf("could not malloc detailed atmosphere components array");
            break;
        case -140:
            printf("could not malloc satellite array for terrestrial planet");
            break;
        case -141:
            printf("could not malloc satellite array for habitable planet");
            break;
        case -142:
            printf("could not malloc satellite array for giant planet");
            break;
        case -143:
            printf("could not malloc satellite array for dwarf planet");
            break;
        case -150:
            printf("could not malloc ordinal string while populating options");
            break;
        case -151:
            printf("could not malloc description string while populating options");
            break;
        case -160:
            printf("could not malloc statblock for star");
            break;
        case -161:
            printf("could not malloc statblock for planet");
            break;
        case -162:
            printf("could not malloc statblock for asteroid belt");
            break;
        case -163:
            printf("could not malloc statblock for satellite");
            break;
        case -170:
            printf("could not malloc description text for star");
            break;
        case -171:
            printf("could not malloc color description text for star");
            break;
        case -172:
            printf("could not malloc magnitude description text for star");
            break;
        case -173:
            printf("could not malloc description text for planet");
            break;
        case -174:
            printf("could not malloc type description text for planet");
            break;
        case -175:
            printf("could not malloc atmosphere description text for planet");
            break;
        case -176:
            printf("could not malloc cloud description text for planet");
            break;
        case -177:
            printf("could not malloc major rocky moon description text for planet");
            break;
        case -178:
            printf("could not malloc minor rocky moon description text for planet");
            break;
        case -179:
            printf("could not malloc major icy moon description text for planet");
            break;
        case -180:
            printf("could not malloc minor icy moon description text for planet");
            break;
        case -181:
            printf("could not malloc ring description text for planet");
            break;
        case -182:
            printf("could not malloc description text for inner asteroid belt");
            break;
        case -183:
            printf("could not malloc description text for kuiper asteroid belt");
            break;
        case -184:
            printf("could not malloc plutino description text for kuiper asteroid belt");
            break;
        case -185:
            printf("could not malloc cubewano description text for kuiper asteroid belt");
            break;
        case -186:
            printf("could not malloc twotino description text for kuiper asteroid belt");
            break;
        case -187:
            printf("could not malloc scattered disk object description text for kuiper asteroid belt");
            break;
        case -188:
            printf("could not malloc sednoid description text for kuiper asteroid belt");
            break;
        case -189:
            printf("could not malloc description text for satellite");
            break;
        case -190:
            printf("could not malloc type description text for satellite");
            break;
        case -191:
            printf("could not malloc feature description text for satellite");
            break;
        case -192:
            printf("could not malloc feature description text for feature \'cratered\'");
            break;
        case -193:
            printf("could not malloc feature description text for feature \'volcanic\'");
            break;
        case -194:
            printf("could not malloc feature description text for feature \'continents\'");
            break;
        case -195:
            printf("could not malloc feature description text for feature \'continents\'");
            break;
        case -196:
            printf("could not malloc feature description text for feature \'ice\'");
            break;
        case -197:
            printf("could not malloc feature description text for feature \'cryovolcanic\'");
            break;
        case -198:
            printf("could not malloc feature description text for feature \'polygon\'");
            break;
        case -199:
            printf("could not malloc feature description text for feature \'storm\'");
            break;
        case -200:
            printf("failed to calculate apparent magnitude of star");
            break;
        case -220:
            printf("unknown planet type encountered while assigning planet properties");
            break;
        case -230:
            printf("unknown planet type encountered while assigning orbit parameters");
            break;
        case -240:
            printf("unknown planet type encountered while generating atmosphere");
            break;
        case -241:
            printf("unknown planet type encountered while whilst calculating albedo and temperature");
            break;
        case -250:
            printf("unknown planet type encountered while generating satellite system");
            break;
        default:
            printf("Unknown error code");
    }

    //printf("\n");
    exit(num);
}

#endif // UTILS_H
