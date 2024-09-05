#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <conio.h>
#include "defs.h"
#include "utils.h"
#include "gen_system.h"
#include "ui.h"

int main()
{
    //Seed rand
    srand((unsigned) time(NULL));

    //Set character encoding
    SetConsoleOutputCP(CP_UTF8);

    //Set console title
    SetConsoleTitle("Consystem Generator");

    //Clear log
    FILE *log = fopen("log.txt", "w");
    fclose(log);

    //Generate
    int num_orbits = 0;
    struct Star   *sol     = nullptr;
    struct Planet *planets = nullptr;

    //UI
    int option1 = 0;
    int option2 = 0;
    int quit    = false;
    int gen     = true;
    char input;

    do
    {
        if (gen)
        {
            //Clear log
            FILE *log = fopen("log.txt", "w");
            fclose(log);

            //Generate
            if (sol != nullptr)
            {
                //Free memory from previous system
                for (int i = 0; i < num_orbits; i++)
                {
                    if (planets[i].moons != nullptr)
                        free((void*)planets[i].moons);

                    if (planets[i].rings != nullptr)
                        free((void*)planets[i].rings);

                    if (planets[i].atmosphere != nullptr)
                        free((void*)planets[i].atmosphere);

                    if (planets[i].feature != nullptr)
                        free((void*)planets[i].feature);
                }

                free((void*)(sol->photosphere));
                free((void*)sol    );
                free((void*)planets);
            }

            sol = (struct Star*)malloc(sizeof(struct Star));
            if (sol == nullptr)
                crash(-110);

            planets = generate_system(sol, &num_orbits);

            //Reset UI
            option1 = 0;
            option2 = 0;
            gen     = false;
        }

        draw_frame();

        //Rollover check
        if (option1 > num_orbits+1)
            option1 = 1;

        if (option1 < 0)
            option1 = num_orbits+1;

        if (option1 >= 2)
        {
            if (option2 > planets[option1-2].num_moons)
                option2 = 0;

            if (option2 < 0)
                option2 = planets[option1-2].num_moons;
        }
        else
        {
            if (option2 > 5)
                option2 = 0;

            if (option2 < 0)
                option2 = 5;
        }

        pop_options(sol, planets, num_orbits, option1, option2);

        //Menu options
        if (option1 == 0)
        {
            if (option2 == 0)
                print_title(sol, planets, num_orbits);
            else if (option2 == 1)
                print_units();
            else if (option2 == 2)
                print_desc();
            else if (option2 == 3)
                print_orbit();
            else if (option2 == 4)
            {
                print_atmo_comps();
            }
            else if (option2 == 5)
            {
                print_atmo_types();
            }
        }
        else if (option1 == 1) //Star
        {
            print_star_props(sol);
            print_star_desc(sol);
        }
        else //A planet
        {
            if (option2 == 0)
            {
                if ((planets[option1-2]).type == TYPE_BLT_INNER)
                {
                    print_belt_props(&(planets[option1-2]));
                    print_belt_desc(&(planets[option1-2]), option1-2);
                }
                else if ((planets[option1-2]).type == TYPE_BLT_KUIPR)
                {
                    print_belt_props(&(planets[option1-2]));
                    print_kuiper_desc(planets, num_orbits);
                }
                else if ((planets[option1-2]).type == TYPE_DWF_PLNTSML ||
                         (planets[option1-2]).type == TYPE_DWF_PLUTINO ||
                         (planets[option1-2]).type == TYPE_DWF_CUBEWNO ||
                         (planets[option1-2]).type == TYPE_DWF_TWOTINO ||
                         (planets[option1-2]).type == TYPE_DWF_SCATTER ||
                         (planets[option1-2]).type == TYPE_DWF_SEDNOID  )
                {
                    print_planet_props(&(planets[option1-2]));
                    print_dwarf_desc(&(planets[option1-2]));
                }
                else
                {
                    print_planet_props(&(planets[option1-2]));
                    print_planet_desc(&(planets[option1-2]));
                }
            }
            else
            {
                print_moon_props(&(planets[option1-2].moons[option2-1]));
                print_moon_desc(&(planets[option1-2].moons[option2-1]));
            }
        }

        Movetoxy(0,0);

        input = getch();

        switch (input)
        {
            case 's':
                option1++;
                option2 = 0;
                break;
            case 'w':
                option1--;
                option2 = 0;
                break;
            case 'd':
                if (option1 != 1)
                    option2++;
                else
                    option2 = 0;
                break;
            case 'e':
                if (option1 != 1)
                    option2--;
                else
                    option2 = 0;
                break;
            case 'q':
                quit = true;
                break;
            case 'r':
                gen = true;
                break;
        }
    }
    while (!quit);

    //Free variables
    for (int i = 0; i < num_orbits; i++)
    {
        if (planets[i].moons != nullptr)
            free((void*)planets[i].moons);

        if (planets[i].rings != nullptr)
            free((void*)planets[i].rings);

        if (planets[i].atmosphere != nullptr)
            free((void*)planets[i].atmosphere);

        if (planets[i].feature != nullptr)
            free((void*)planets[i].feature);
    }

    free((void*)(sol->photosphere));
    free((void*)sol    );
    free((void*)planets);

    crash(0);
}
