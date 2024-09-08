#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs.h"
#include "utils.h"

//The molar masses of various substances
#define MASS_HYDROGEN   2.016

//The id numbers of various substances
#define ID_HYDROGEN      0
#define ID_DEUTERIDE     1
#define ID_HELIUM        2
#define ID_CARBON_MONOX  3
#define ID_CARBON_DIOX   4
#define ID_METHYLENE     5
#define ID_METHANE       6
#define ID_ACETYLENE     7
#define ID_ETHYLENE      8
#define ID_ETHANE        9
#define ID_PROPANE      10
#define ID_BUTADIYNE    11
#define ID_THOLINS      12
#define ID_CYANOGEN     13
#define ID_H_CYANIDE    14
#define ID_M_CYANIDE    15
#define ID_NITROGEN     16
#define ID_AMMONIA      17
#define ID_AZIC         18
#define ID_NITRIC       19
#define ID_NITRO_DIOX   20
#define ID_NITROUS      21
#define ID_OXYGEN       22
#define ID_WATER        23
#define ID_PEROXIDE     24
#define ID_OZONE        25
#define ID_FLUORINE     26
#define ID_HYDRO_F      27
#define ID_BORON_F3     28
#define ID_BORON2_F4    29
#define ID_CARBON_F4    30
#define ID_SILICON_F4   31
#define ID_PHOSPH_F3    32
#define ID_PHOSPH_F5    33
#define ID_SULFUR_F4    34
#define ID_SULFUR_F6    35
#define ID_NEON         36
#define ID_SILANE       37
#define ID_PHOSPHINE    38
#define ID_HYDRO_S      39
#define ID_SULFUR_DIOX  40
#define ID_SULFUR_TRIOX 41
#define ID_CHLORINE     42
#define ID_HYDRO_CL     43
#define ID_BORON_CL3    44
#define ID_CHLOROFORM   45
#define ID_CARBON_CL4   46
#define ID_SILICON_CL4  47
#define ID_NITROGEN_CL3 48
#define ID_PHOSPH_CL5   49
#define ID_ARGON        50
#define ID_BROMINE      51
#define ID_HYDRO_BR     52
#define ID_BORON_BR3    53
#define ID_SILICON_BR4  54
#define ID_PHOSPH_BR3   55
#define ID_ARSENIC_BR3  56
#define ID_KRYPTON      57
#define ID_XENON        58

#ifndef ATMO_H
#define ATMO_H

void gen_planet_atmos(struct Star *star_ptr, struct Planet *planet_ptr);

void pick_major_class(struct Planet *planet_ptr, double *remain);
void pick_minor_class(struct Planet *planet_ptr, double *remain);
void round_robin(double *comps, double *remain, int num, int names[]);
void calc_atomic_weight(struct Planet *planet_ptr);
void calc_albedo(struct Star *star_ptr, struct Planet *planet_ptr);
char* get_comp_name(int num);

void log_atmo_gen(struct Planet *planet_ptr, int id, double remain);

//This is the top level function. It should be the only function called outside this file.
void gen_planet_atmos(struct Star *star_ptr, struct Planet *planet_ptr)
{
    double remain = 1.0;

    //Determine if the planet will take an atmosphere
    switch (planet_ptr->type)
    {
        case TYPE_RCK_DENSE:
        case TYPE_BLT_INNER:
        case TYPE_BLT_KUIPR:
        case TYPE_DWF_PLNTSML:
        case TYPE_DWF_PLUTINO:
        case TYPE_DWF_CUBEWNO:
        case TYPE_DWF_TWOTINO:
        case TYPE_DWF_SCATTER:
        case TYPE_DWF_SEDNOID:
            planet_ptr->has_atmo = false;
            break;
        default:
            planet_ptr->has_atmo = true;
    }

    //Albedo doesn't require knowledge of atmo composition, so we can do it right away
    calc_albedo(star_ptr, planet_ptr);

    //If the planet needs no atmosphere, we're done
    if (!planet_ptr->has_atmo)
        return;

    //Otherwise, start by creating the atmosphere array
    double *atmo = (double*)malloc(sizeof(double)*60);
    if (atmo == nullptr)
        crash(-130);

    //Zero array
    for (int i = 0; i < 60; i++)
        atmo[i] = 0;

    //Transfer
    planet_ptr->atmosphere = atmo;

    print_log("      Generating major components:\n");
    pick_major_class(planet_ptr, &remain);

    print_log("      Generating minor components:\n");
    pick_minor_class(planet_ptr, &remain);

    //Adjust certain minor components, for realism's sake
    double temp;
    if (planet_ptr->atmosphere[ID_OZONE] > planet_ptr->atmosphere[ID_OXYGEN])
    {
        temp                              = planet_ptr->atmosphere[ID_OZONE];
        planet_ptr->atmosphere[ID_OZONE]  = planet_ptr->atmosphere[ID_OXYGEN];
        planet_ptr->atmosphere[ID_OXYGEN] = temp;
    }
    if (planet_ptr->atmosphere[ID_SULFUR_TRIOX] > planet_ptr->atmosphere[ID_SULFUR_DIOX])
    {
        temp                                    = planet_ptr->atmosphere[ID_SULFUR_DIOX];
        planet_ptr->atmosphere[ID_SULFUR_DIOX]  = planet_ptr->atmosphere[ID_SULFUR_TRIOX];
        planet_ptr->atmosphere[ID_SULFUR_TRIOX] = temp;
    }

    if (planet_ptr->atmosphere[ID_SILANE] > planet_ptr->atmosphere[ID_HYDRO_CL])
    {
        temp                                = planet_ptr->atmosphere[ID_SILANE];
        planet_ptr->atmosphere[ID_SILANE]   = planet_ptr->atmosphere[ID_HYDRO_CL];
        planet_ptr->atmosphere[ID_HYDRO_CL] = temp;
    }

    print_log("      Calculating atomic weight:\n");
    calc_atomic_weight(planet_ptr);
}

//This function determines the major components of the atmosphere that take up the 99%.
void pick_major_class(struct Planet *planet_ptr, double *remain)
{
    char logtext[100];

    //Determine if the planet's gravity can retain hydrogen
    double hydrogen_rms = sqrt((3.0 * GAS_CONST * ((planet_ptr->surf_temp + KELVIN)) * 1500.0) / 0.00201588);
    double hydrogen_lim = planet_ptr->esc_v/6.0;
    int is_super = (hydrogen_rms < hydrogen_lim);

    if (is_super)
        sprintf(logtext, "         Planet is able to retain hydrogen (rms = %f, lim = %f, div = %f)\n", hydrogen_rms, hydrogen_lim, (hydrogen_rms / hydrogen_lim));
    else
        sprintf(logtext, "         Planet is not able to retain hydrogen (rms = %f, lim = %f, div = %f)\n", hydrogen_rms, hydrogen_lim, (hydrogen_rms / hydrogen_lim));
    print_log(logtext);

    double frac = fudge_double(0.0001);

    for (;;)
    {
        if ((planet_ptr->type == TYPE_RCK_DESRT) && (hydrogen_rms < hydrogen_lim)) //Planet is super-terrestrial
        {
            //Hydrogen
            planet_ptr->atmosphere[ID_HYDROGEN] = rand_double(0.85,0.91);
            *remain -= planet_ptr->atmosphere[ID_HYDROGEN];
            log_atmo_gen(planet_ptr, ID_HYDROGEN, *remain);

            //Helium
            planet_ptr->atmosphere[ID_HELIUM] = (*remain)*fudge_double(ATMO_DECAY);
            *remain -= planet_ptr->atmosphere[ID_HELIUM];
            log_atmo_gen(planet_ptr, ID_HELIUM, *remain);

            //Argon
            planet_ptr->atmosphere[ID_ARGON] = rand_double(0.005, 0.02);
            *remain -= planet_ptr->atmosphere[ID_ARGON];
            log_atmo_gen(planet_ptr, ID_ARGON, *remain);
        }
        else if ((planet_ptr->type == TYPE_RCK_DESRT) && (hydrogen_rms >= hydrogen_lim)) //Planet is a regular terrestrial
        {
            //Nitrogen
            planet_ptr->atmosphere[ID_NITROGEN] = fudge_double(0.03);
            *remain -= planet_ptr->atmosphere[ID_NITROGEN];
            log_atmo_gen(planet_ptr, ID_NITROGEN, *remain);

            //Argon
            planet_ptr->atmosphere[ID_ARGON] = rand_double(0.005, 0.02);
            *remain -= planet_ptr->atmosphere[ID_ARGON];
            log_atmo_gen(planet_ptr, ID_ARGON, *remain);

            //Carbon dioxide
            planet_ptr->atmosphere[ID_CARBON_DIOX] = (*remain)-frac;
            *remain  -= planet_ptr->atmosphere[ID_CARBON_DIOX];
            log_atmo_gen(planet_ptr, ID_CARBON_DIOX, *remain);
        }
        else if ((planet_ptr->type == TYPE_WTR_HYCN) && (hydrogen_rms < hydrogen_lim)) //Planet is a super-ocean (hycean)
        {
            //Hydrogen
            planet_ptr->atmosphere[ID_HYDROGEN] = rand_double(0.85,0.91);
            *remain -= planet_ptr->atmosphere[ID_HYDROGEN];
            log_atmo_gen(planet_ptr, ID_HYDROGEN, *remain);

            //Helium
            planet_ptr->atmosphere[ID_HELIUM] = (*remain)*fudge_double(ATMO_DECAY);
            *remain -= planet_ptr->atmosphere[ID_HELIUM];
            log_atmo_gen(planet_ptr, ID_HELIUM, *remain);

            //Water
            planet_ptr->atmosphere[ID_WATER] = rand_double(0.005, 0.02);
            *remain -= planet_ptr->atmosphere[ID_WATER];
            log_atmo_gen(planet_ptr, ID_WATER, *remain);
        }
        else if ((planet_ptr->type == TYPE_WTR_HYCN) && (hydrogen_rms >= hydrogen_lim)) //Planet is a regular ocean
        {
            //Nitrogen
            planet_ptr->atmosphere[ID_NITROGEN] = fudge_double(0.03);
            *remain -= planet_ptr->atmosphere[ID_NITROGEN];
            log_atmo_gen(planet_ptr, ID_NITROGEN, *remain);

            //Water
            planet_ptr->atmosphere[ID_WATER] = rand_double(0.005, 0.02);
            *remain -= planet_ptr->atmosphere[ID_WATER];
            log_atmo_gen(planet_ptr, ID_WATER, *remain);

            //Carbon dioxide
            planet_ptr->atmosphere[ID_CARBON_DIOX] = (*remain)-frac;
            *remain  -= planet_ptr->atmosphere[ID_CARBON_DIOX];
            log_atmo_gen(planet_ptr, ID_CARBON_DIOX, *remain);
        }
        else if (planet_ptr->type == TYPE_RCK_GREEN || planet_ptr->type == TYPE_WTR_GREEN) //Planet is habitable
        {
            //Oxygen
            planet_ptr->atmosphere[ID_OXYGEN] = fudge_double((1.0/3.0)-((1.0/3.0)*FUDGE_FACTOR));
            *remain -= planet_ptr->atmosphere[ID_OXYGEN];
            log_atmo_gen(planet_ptr, ID_OXYGEN, *remain);

            //Argon
            planet_ptr->atmosphere[ID_ARGON] = fudge_double(0.01);
            *remain -= planet_ptr->atmosphere[ID_ARGON];
            log_atmo_gen(planet_ptr, ID_ARGON, *remain);

            //Nitros
            planet_ptr->atmosphere[ID_NITROGEN] = (*remain)-frac;
            *remain -= planet_ptr->atmosphere[ID_NITROGEN];
            log_atmo_gen(planet_ptr, ID_NITROGEN, *remain);

        }
        else if (planet_ptr->type == TYPE_GAS_GIANT ||
                 planet_ptr->type == TYPE_GAS_SUPER ||
                 planet_ptr->type == TYPE_GAS_PUFFY ||
                 planet_ptr->type == TYPE_GAS_HOT   ||
                 planet_ptr->type == TYPE_ICE_DWARF ||
                 planet_ptr->type == TYPE_ICE_GIANT  ) //Planet is a giant
        {
            //Hydrogen
            planet_ptr->atmosphere[ID_HYDROGEN] = rand_double(0.85,0.91);
            *remain -= planet_ptr->atmosphere[ID_HYDROGEN];
            log_atmo_gen(planet_ptr, ID_HYDROGEN, *remain);

            //Helium
            planet_ptr->atmosphere[ID_HELIUM] = (*remain)*fudge_double(ATMO_DECAY);
            *remain -= planet_ptr->atmosphere[ID_HELIUM];
            log_atmo_gen(planet_ptr, ID_HELIUM, *remain);

            //Methane
            planet_ptr->atmosphere[ID_METHANE] = rand_double(0.005, 0.02);
            *remain -= planet_ptr->atmosphere[ID_METHANE];
            log_atmo_gen(planet_ptr, ID_METHANE, *remain);
        }

        //Make sure remain is in an interesting range
        if (!(0.0001 < (*remain) && (*remain) <= 0.0005))
        {
            print_log("            Remainder is outside range, rerandomizing\n");

            //Reset
            for (int i = 0; i < 60; i++)
                planet_ptr->atmosphere[i] = 0;
            (*remain) = 1.0;
            frac   = fudge_double(0.0001);
        }
        else
            break;
    }

    planet_ptr->is_super = is_super;
}

//This function determines the minor components of the atmosphere that make up the ppm and ppb range.
void pick_minor_class(struct Planet *planet_ptr, double *remain)
{
    //Organic, Nitrogenous, Volcanic, Oxygenated, Halogenated
    int allowed[5];
    int minor_class;
    int comps[10];
    char logtext[100];

    //Determine which minor classes are allowed based on planet type
    switch (planet_ptr->type)
    {
        case TYPE_RCK_DESRT:
        case TYPE_RCK_GREEN:
            allowed[0] = true;
            allowed[1] = true;
            allowed[2] = true;
            allowed[3] = true;
            allowed[4] = true;
            break;
        case TYPE_WTR_HYCN:
        case TYPE_WTR_GREEN:
            allowed[0] = true;
            allowed[1] = true;
            allowed[2] = true;
            allowed[3] = true;
            allowed[4] = false;
            break;
        case TYPE_GAS_GIANT:
        case TYPE_GAS_SUPER:
        case TYPE_GAS_PUFFY:
        case TYPE_GAS_HOT:
        case TYPE_ICE_DWARF:
        case TYPE_ICE_GIANT:
            allowed[0] = true;
            allowed[1] = false;
            allowed[2] = false;
            allowed[3] = false;
            allowed[4] = false;
            break;
        case TYPE_RCK_DENSE:
        case TYPE_BLT_INNER:
        case TYPE_BLT_KUIPR:
        case TYPE_DWF_PLNTSML:
        case TYPE_DWF_PLUTINO:
        case TYPE_DWF_CUBEWNO:
        case TYPE_DWF_TWOTINO:
        case TYPE_DWF_SCATTER:
        case TYPE_DWF_SEDNOID:
            return;
        default:
            crash(-240);
    }

    //Randomly pick one allowed minor class
    minor_class = -1;
    do
    {
        minor_class = round(rand_double(0.0, 4.0));
        sprintf(
            logtext,
            "         Picked minor class %i, allowed: %s\n",
            minor_class,
            (
                allowed[minor_class]
                ? "true"
                : "false"
            )
        );
        print_log(logtext);
    }
    while ((!allowed[minor_class]) && (minor_class != -1));

    //Determine minor components based on class
    int random_gas, random_gas2, d3;
    switch (planet_ptr->type)
    {
        case TYPE_RCK_DESRT:
        case TYPE_RCK_GREEN:
            random_gas = round(rand_double(0.0, 2.0));
            random_gas = (
                            random_gas == 0
                            ? ID_NEON
                            : (
                                random_gas == 1
                                ? ID_KRYPTON
                                : ID_XENON
                            )
                         );

            random_gas2 = round(rand_double(0.0, 2.0));
            random_gas2 = (
                             random_gas2 == 0
                             ? ID_NEON
                             : (
                                 random_gas2 == 1
                                 ? ID_KRYPTON
                                 : ID_XENON
                             )
                          );

            switch (minor_class)
            {
                case 0: //Organic
                    comps[0] = random_gas     ;
                    comps[1] = ID_CARBON_MONOX;
                    comps[2] = ID_METHANE     ;
                    comps[3] = ID_ETHANE      ;
                    comps[4] = ID_METHYLENE   ;
                    comps[5] = ID_ETHYLENE    ;
                    comps[6] = ID_ACETYLENE   ;
                    comps[7] = ID_PHOSPHINE   ;
                    comps[8] = ID_OZONE       ;
                    comps[9] = ID_OXYGEN      ;
                    break;

                case 1: //Nitrogenated
                    comps[0] = random_gas   ;
                    comps[1] = ID_NITRIC    ;
                    comps[2] = ID_NITROUS   ;
                    comps[3] = ID_NITRO_DIOX;
                    comps[4] = ID_AMMONIA   ;
                    comps[5] = ID_CYANOGEN  ;
                    comps[6] = ID_M_CYANIDE ;
                    comps[7] = ID_AZIC      ;
                    comps[8] = ID_H_CYANIDE ;
                    comps[9] = (planet_ptr->atmosphere[ID_CARBON_DIOX] > 0.0 ? ID_CARBON_MONOX : ID_CARBON_DIOX);
                    break;

                case 2: //Volcanic
                    comps[0] = random_gas     ;
                    comps[1] = ID_SULFUR_DIOX ;
                    comps[2] = ID_SULFUR_TRIOX;
                    comps[3] = ID_SULFUR_F6   ;
                    comps[4] = ID_OXYGEN      ;
                    comps[5] = ID_CARBON_MONOX;
                    comps[6] = ID_HYDRO_S     ;
                    comps[7] = ID_HYDRO_F     ;
                    comps[8] = ID_HYDRO_CL    ;
                    comps[9] = ID_METHANE     ;
                    break;

                case 3: //Oxygenated
                    comps[0] = random_gas     ;
                    comps[1] = ID_OXYGEN      ;
                    comps[2] = ID_OZONE       ;
                    comps[3] = (planet_ptr->atmosphere[ID_WATER] > 0.0 ? ID_PEROXIDE : ID_WATER);
                    comps[4] = ID_NITRIC      ;
                    comps[5] = ID_NITROUS     ;
                    comps[6] = ID_NITRO_DIOX  ;
                    comps[7] = ID_SULFUR_DIOX ;
                    comps[8] = ID_SULFUR_TRIOX;
                    comps[9] = (planet_ptr->atmosphere[ID_CARBON_DIOX] > 0.0 ? ID_CARBON_MONOX : ID_CARBON_DIOX);
                    break;

                case 4: //Halogenated
                    d3 = floor(rand_double(0.0, 3.0)); //Choose which halogen
                    switch (d3)
                    {
                        case 0: //Fluorine
                            comps[0] = random_gas   ;
                            comps[1] = ID_FLUORINE  ;
                            comps[2] = ID_HYDRO_F   ;
                            comps[3] = ID_CARBON_F4 ;
                            comps[4] = ID_BORON_F3  ;
                            comps[5] = ID_BORON2_F4 ;
                            comps[6] = ID_SILICON_F4;
                            comps[7] = ID_PHOSPH_F5 ;
                            comps[8] = ID_SULFUR_F4 ;
                            comps[9] = ID_SULFUR_F6 ;
                            break;

                        case 1: //Chlorine
                            comps[0] = random_gas     ;
                            comps[1] = ID_CHLORINE    ;
                            comps[2] = ID_HYDRO_CL    ;
                            comps[3] = ID_CARBON_CL4  ;
                            comps[4] = ID_BORON_CL3   ;
                            comps[5] = ID_CHLOROFORM  ;
                            comps[6] = ID_SILICON_CL4 ;
                            comps[7] = ID_PHOSPH_CL5  ;
                            comps[8] = ID_NITROGEN_CL3;
                            comps[9] = ID_SILANE      ;
                            break;

                        case 2: //Bromine
                        case 3:
                            comps[0] = random_gas    ;
                            comps[1] = random_gas2   ;
                            comps[2] = ID_BROMINE    ;
                            comps[3] = ID_HYDRO_BR   ;
                            comps[4] = ID_BORON_BR3  ;
                            comps[5] = ID_PHOSPH_BR3 ;
                            comps[6] = ID_SILICON_BR4;
                            comps[7] = ID_ARSENIC_BR3;
                            comps[8] = ID_HYDRO_F    ;
                            comps[9] = ID_HYDRO_CL   ;
                            break;
                    }

            }
            break;

        case TYPE_WTR_HYCN:
        case TYPE_WTR_GREEN:
            random_gas = round(rand_double(0.0, 2.0));
            random_gas = (
                            random_gas == 0
                            ? ID_NEON
                            : (
                                random_gas == 1
                                ? ID_KRYPTON
                                : ID_XENON
                            )
                         );

            switch (minor_class)
            {
                case 0: //Organic
                    comps[0] = random_gas     ;
                    comps[1] = ID_CARBON_MONOX;
                    comps[2] = ID_METHANE     ;
                    comps[3] = ID_ETHANE      ;
                    comps[4] = ID_METHYLENE   ;
                    comps[5] = ID_ETHYLENE    ;
                    comps[6] = ID_ACETYLENE   ;
                    comps[7] = ID_PHOSPHINE   ;
                    comps[8] = ID_OZONE       ;
                    comps[9] = ID_OXYGEN      ;
                    break;

                case 1: //Nitrogenated
                    comps[0] = random_gas   ;
                    comps[1] = ID_NITRIC    ;
                    comps[2] = ID_NITROUS   ;
                    comps[3] = ID_NITRO_DIOX;
                    comps[4] = ID_AMMONIA   ;
                    comps[5] = ID_CYANOGEN  ;
                    comps[6] = ID_M_CYANIDE ;
                    comps[7] = ID_AZIC      ;
                    comps[8] = ID_H_CYANIDE ;
                    comps[9] = (planet_ptr->atmosphere[ID_CARBON_DIOX] > 0.0 ? ID_CARBON_MONOX : ID_CARBON_DIOX);
                    break;

                case 2: //Volcanic
                    comps[0] = random_gas     ;
                    comps[1] = ID_SULFUR_DIOX ;
                    comps[2] = ID_SULFUR_TRIOX;
                    comps[3] = ID_SULFUR_F6   ;
                    comps[4] = ID_OXYGEN      ;
                    comps[5] = ID_CARBON_MONOX;
                    comps[6] = ID_HYDRO_S     ;
                    comps[7] = ID_HYDRO_F     ;
                    comps[8] = ID_HYDRO_CL    ;
                    comps[9] = ID_METHANE     ;
                    break;

                case 3: //Oxygenated
                    comps[0] = random_gas     ;
                    comps[1] = ID_OXYGEN      ;
                    comps[2] = ID_OZONE       ;
                    comps[3] = (planet_ptr->atmosphere[ID_WATER] > 0.0 ? ID_PEROXIDE : ID_WATER);
                    comps[4] = ID_NITRIC      ;
                    comps[5] = ID_NITROUS     ;
                    comps[6] = ID_NITRO_DIOX  ;
                    comps[7] = ID_SULFUR_DIOX ;
                    comps[8] = ID_SULFUR_TRIOX;
                    comps[9] = (planet_ptr->atmosphere[ID_CARBON_DIOX] > 0.0 ? ID_CARBON_MONOX : ID_CARBON_DIOX);
                    break;
            }
            break;

        case TYPE_GAS_GIANT:
        case TYPE_GAS_SUPER:
        case TYPE_GAS_PUFFY:
        case TYPE_GAS_HOT:
        case TYPE_ICE_DWARF:
        case TYPE_ICE_GIANT:
            random_gas = round(rand_double(0.0, 1.0));
            random_gas = (
                            random_gas == 0
                            ? ID_ARGON
                            : ID_NEON
                         );

            comps[0] = random_gas     ;
            comps[1] = ID_WATER       ;
            comps[2] = ID_ETHANE      ;
            comps[3] = ID_HYDRO_S     ;
            comps[4] = ID_DEUTERIDE   ;
            comps[5] = ID_H_CYANIDE   ;
            comps[6] = ID_AMMONIA     ;
            comps[7] = ID_PHOSPHINE   ;
            comps[8] = ID_CARBON_MONOX;
            comps[9] = ID_THOLINS     ;
            break;
        default:
            crash(-240);
    }

    //Now that we have the substances to generate, calculate the percentages
    int reset_remain = (*remain);
    double amounts[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    for (;;)
    {
        round_robin(amounts, remain, 10, comps);

        if (*remain < 0.0)
        {
            (*remain) = reset_remain;
            for (int i = 0; i < 10; i++)
                amounts[i] = 0.0;
        }
        else
            break;
    }

    //Transfer to planet's atmosphere
    for (int i = 0; i < 10; i++)
        if (planet_ptr->atmosphere[comps[i]] == 0)
            planet_ptr->atmosphere[comps[i]] = amounts[i];
}

//This function round-robins the generation of minor atmospheric components.
void round_robin(double *comps, double *remain, int num, int names[])
{
    double frac, i = 0;
    int tries = 0, choice = 0;
    int already[num];
    char logtext[100];

    for (int j = 0; j < num; j++)
        already[j] = 0;

    do
    {
        choice = round(rand_double(0.0, (double)num));
        if (choice > num)
            choice = num;

        //sprintf(logtext, "         Choice=%i, i=%i, num=%i\n", choice, (int)i, num);
        //print_log(logtext);


        //If the choice hasn't already been generated
        if (!already[choice])
        {
            //Assign it
            frac = fudge_double(ATMO_DECAY);
            comps[choice] = (*remain)*(frac > 1.0 ? 0.99 : frac);
            (*remain) -= comps[choice];
            i++;

            //If the choice is less than 1 ppt set it to zero
            if (comps[choice]*1000000000000.0 < 1.0)
            {
                comps[choice] = 0.0;
                *remain       = 0.0;
                return;
            }

            sprintf(logtext, "         Generated %.15f", comps[choice]);
            print_log(logtext);
            sprintf(logtext, " %-24s"                  , get_comp_name(names[choice]));
            print_log(logtext);
            sprintf(logtext, " %.15f remaining"        , (*remain)    );
            print_log(logtext);
            sprintf(logtext, " (i=%i, "                , (int)i       );
            print_log(logtext);
            sprintf(logtext, "num=%i, "                , num          );
            print_log(logtext);
            sprintf(logtext, "choice=%i, "             , choice       );
            print_log(logtext);
            sprintf(logtext, "name=%i)\n"              , names[choice]);
            print_log(logtext);

            already[choice] = true;
        }
        tries++;

        //If all components haven't been generated after 500 tries
        if (false)
        {
            sprintf(logtext, "      Maximum tries exceeded, generating sequentially\n");

            //Walk through each one
            for (; i < num; i++)
            {
                //Skip if already generated
                if (already[(int)i])
                {
                    i--;
                    continue;
                }

                //Generate
                frac = fudge_double(ATMO_DECAY);
                comps[(int)i] = (*remain)*(frac > 1.0 ? 1.0 : frac);
                *remain -= comps[(int)i];
                i++;

                sprintf(logtext, "         Generated %.15f %-17s %.15f remaining (i=%i, num=%i)\n", comps[choice], get_comp_name(names[choice]), (*remain), (int)i, num);
                print_log(logtext);

                //Zero if less than 1 ppt
                if (comps[(int)i]*1000000000000.0 < 1.0)
                {
                    comps[(int)i] = 0.0;
                    *remain       = 0.0;
                }

                already[(int)i] = true;
            }
            return;
        }
    }
    while (i < num);
}

//Calculates the average atomic weight and scale height for a given atmosphere.
void calc_atomic_weight(struct Planet *planet_ptr)
{
    char logtext[100];

    //Create the weights array
    double weights[59];

    weights[ID_HYDROGEN    ] = 0.00201588;  //Hydrogen
    weights[ID_DEUTERIDE   ] = 0.00302204;  //Deuteride
    weights[ID_HELIUM      ] = 0.0040026 ;  //Helium
    weights[ID_CARBON_MONOX] = 0.02801   ;  //CO
    weights[ID_CARBON_DIOX ] = 0.04401   ;  //CO2
    weights[ID_METHYLENE   ] = 0.0140266 ;
    weights[ID_METHANE     ] = 0.01604   ;  //Methane
    weights[ID_ACETYLENE   ] = 0.026038  ;
    weights[ID_ETHYLENE    ] = 0.028054  ;
    weights[ID_ETHANE      ] = 0.03006904;  //Ethane
    weights[ID_PROPANE     ] = 0.0441    ;
    weights[ID_BUTADIYNE   ] = 0.05006   ;
    weights[ID_THOLINS     ] = rand_double(0.01, 0.2); //Tholins
    weights[ID_CYANOGEN    ] = 0.052036  ;
    weights[ID_H_CYANIDE   ] = 0.0270253 ;  //HCN
    weights[ID_M_CYANIDE   ] = 0.041053  ;
    weights[ID_NITROGEN    ] = 0.0280134 ;  //Nitrogen
    weights[ID_AMMONIA     ] = 0.017031  ;  //Ammonia
    weights[ID_AZIC        ] = 0.041053  ;
    weights[ID_NITRIC      ] = 0.03001   ;  //NO
    weights[ID_NITRO_DIOX  ] = 0.0460055 ;  //NO2
    weights[ID_NITROUS     ] = 0.044013  ;  //N2O
    weights[ID_OXYGEN      ] = 0.031988  ;  //Oxygen
    weights[ID_WATER       ] = 0.01801528;  //Water
    weights[ID_PEROXIDE    ] = 0.034014  ;
    weights[ID_OZONE       ] = 0.047997  ;
    weights[ID_FLUORINE    ] = 0.0379968 ;  //Fluorine
    weights[ID_HYDRO_F     ] = 0.020006  ;
    weights[ID_BORON_F3    ] = 0.06781   ;
    weights[ID_BORON2_F4   ] = 0.09761   ;
    weights[ID_CARBON_F4   ] = 0.0880043 ;
    weights[ID_SILICON_F4  ] = 0.104079  ;
    weights[ID_PHOSPH_F3   ] = 0.08796897;
    weights[ID_PHOSPH_F5   ] = 0.12596577;
    weights[ID_SULFUR_F4   ] = 0.10805   ;
    weights[ID_SULFUR_F6   ] = 0.14605   ;
    weights[ID_NEON        ] = 0.0201797 ;  //Neon
    weights[ID_SILANE      ] = 0.032117  ;
    weights[ID_PHOSPHINE   ] = 0.03399758;  //Phosphine
    weights[ID_HYDRO_S     ] = 0.03408   ;  //Hydrogen sulfide
    weights[ID_SULFUR_DIOX ] = 0.064066  ;  //S02
    weights[ID_SULFUR_TRIOX] = 0.080066  ;  //S03
    weights[ID_CHLORINE    ] = 0.0709056 ;  //Chlorine
    weights[ID_HYDRO_CL    ] = 0.03646   ;
    weights[ID_BORON_CL3   ] = 0.1172    ;
    weights[ID_CHLOROFORM  ] = 0.1194    ;
    weights[ID_CARBON_CL4  ] = 0.1538    ;
    weights[ID_SILICON_CL4 ] = 0.1699    ;
    weights[ID_NITROGEN_CL3] = 0.1204    ;
    weights[ID_PHOSPH_CL5  ] = 0.2082    ;
    weights[ID_ARGON       ] = 0.039948  ;  //Argon
    weights[ID_BROMINE     ] = 0.159808  ;  //Bromine
    weights[ID_HYDRO_BR    ] = 0.080912  ;
    weights[ID_BORON_BR3   ] = 0.25052   ;
    weights[ID_SILICON_BR4 ] = 0.3477    ;
    weights[ID_PHOSPH_BR3  ] = 0.27069   ;
    weights[ID_ARSENIC_BR3 ] = 0.31463   ;
    weights[ID_KRYPTON     ] = 0.083798  ;  //Krypton
    weights[ID_XENON       ] = 0.131293  ;  //Xenon

    //Calculate average atomic weight
    double density = 0.0;
    for (int i = 0; i < 59; i++)
    {
        density += (planet_ptr->atmosphere[i]) * (weights[i]);
    }

    density *= 10;

    sprintf(logtext, "         Average atomic weight: %f kg/mol\n", density);
    print_log(logtext);

    if (density > 0.0)
    {
        planet_ptr->atmo_pres = rand_double(0.0, 100.0);

        planet_ptr->atmo_dens = (planet_ptr->atmo_pres * 101325 * density) / (planet_ptr->surf_temp * GAS_CONST);

        planet_ptr->atmo_high = (planet_ptr->surf_temp * GAS_CONST) / (planet_ptr->surf_grav * GRAVITY * density);

        if (planet_ptr->type == TYPE_GAS_GIANT ||
            planet_ptr->type == TYPE_GAS_SUPER ||
            planet_ptr->type == TYPE_GAS_PUFFY ||
            planet_ptr->type == TYPE_GAS_HOT   ||
            planet_ptr->type == TYPE_ICE_DWARF ||
            planet_ptr->type == TYPE_ICE_GIANT  )
        {
            planet_ptr->atmo_high = (planet_ptr->atmo_high * 10.0) + fudge_double(20000.0);
        }
    }
}

//Gets a string name for an atmo comp based on its id
char* get_comp_name(int num)
{
    switch (num)
    {
        case ID_HYDROGEN    : return "hydrogen,"                ;
        case ID_DEUTERIDE   : return "hydrogen deuteride,"      ;
        case ID_HELIUM      : return "helium,"                  ;
        case ID_CARBON_MONOX: return "carbon monoxide,"         ;
        case ID_CARBON_DIOX : return "carbon dioxide,"          ;
        case ID_METHYLENE   : return "methylene,"               ;
        case ID_METHANE     : return "methane,"                 ;
        case ID_ACETYLENE   : return "acetylene,"               ;
        case ID_ETHYLENE    : return "ethylene,"                ;
        case ID_ETHANE      : return "ethane,"                  ;
        case ID_PROPANE     : return "propane,"                 ;
        case ID_BUTADIYNE   : return "butadiyne,"               ;
        case ID_THOLINS     : return "tholins,"                 ;
        case ID_CYANOGEN    : return "cyanogen,"                ;
        case ID_H_CYANIDE   : return "hydrogen cyanide,"        ;
        case ID_M_CYANIDE   : return "methyl cyanide,"          ;
        case ID_NITROGEN    : return "nitrogen,"                ;
        case ID_AMMONIA     : return "ammonia,"                 ;
        case ID_AZIC        : return "azic acid,"               ;
        case ID_NITRIC      : return "nitric oxide,"            ;
        case ID_NITRO_DIOX  : return "nitrogen dioxide,"        ;
        case ID_NITROUS     : return "nitrous oxide,"           ;
        case ID_OXYGEN      : return "oxygen,"                  ;
        case ID_WATER       : return "water vapor,"             ;
        case ID_PEROXIDE    : return "hydrogen peroxide,"       ;
        case ID_OZONE       : return "ozone,"                   ;
        case ID_FLUORINE    : return "fluorine,"                ;
        case ID_HYDRO_F     : return "hydrogen fluoride,"       ;
        case ID_BORON_F3    : return "boron trifluoride,"       ;
        case ID_BORON2_F4   : return "diboron tetrafluoride,"   ;
        case ID_CARBON_F4   : return "carbon tetrafluoride,"    ;
        case ID_SILICON_F4  : return "silicon tetrafluoride,"   ;
        case ID_PHOSPH_F3   : return "phosphorus trifluoride,"  ;
        case ID_PHOSPH_F5   : return "phosphorus pentafluoride,";
        case ID_SULFUR_F4   : return "sulfur tetrafluoride,"    ;
        case ID_SULFUR_F6   : return "sulfur hexafluoride,"     ;
        case ID_NEON        : return "neon,"                    ;
        case ID_SILANE      : return "silane,"                  ;
        case ID_PHOSPHINE   : return "phosphine,"               ;
        case ID_HYDRO_S     : return "hydrogen sulfide,"        ;
        case ID_SULFUR_DIOX : return "sulfur dioxide,"          ;
        case ID_SULFUR_TRIOX: return "sulfur trioxide,"         ;
        case ID_CHLORINE    : return "chlorine,"                ;
        case ID_HYDRO_CL    : return "hydrogen chloride,"       ;
        case ID_BORON_CL3   : return "boron trichloride,"       ;
        case ID_CHLOROFORM  : return "chloroform,"              ;
        case ID_CARBON_CL4  : return "carbon tetrachloride,"    ;
        case ID_SILICON_CL4 : return "silicon tetrachloride,"   ;
        case ID_NITROGEN_CL3: return "nitrogen trichloride,"    ;
        case ID_PHOSPH_CL5  : return "phosphorus pentachloride,";
        case ID_ARGON       : return "argon,"                   ;
        case ID_BROMINE     : return "bromine,"                 ;
        case ID_HYDRO_BR    : return "hydrogen bromide,"        ;
        case ID_BORON_BR3   : return "boron tribromide,"        ;
        case ID_SILICON_BR4 : return "silicon tetrabromide,"    ;
        case ID_PHOSPH_BR3  : return "phosphorus tribromide,"   ;
        case ID_ARSENIC_BR3 : return "arsenic tribromide,"      ;
        case ID_KRYPTON     : return "krypton,"                 ;
        case ID_XENON       : return "xenon,"                   ;
        default             : return "other gases,"             ;
    }
}

//Calculates the albedo and temperature of a given planet.
void calc_albedo(struct Star *star_ptr, struct Planet *planet_ptr)
{
    //Declare variables
    double   land = 0.0,                  ice = 0.0,   cloud_cover = 0.0,   cloud_thick = 0.0;
    double a_land = 0.0, a_ocean = 0.0, a_ice = 0.0, a_cloud_thin  = 0.0, a_cloud_thick = 0.0;
    char logtext[300];

    print_log("      Calculating albedo and surface temp:\n");

    switch (planet_ptr->type)
    {
        case TYPE_RCK_DENSE:
        case TYPE_RCK_DESRT:
        case TYPE_RCK_GREEN:
        case TYPE_WTR_HYCN:
        case TYPE_WTR_GREEN:
        case TYPE_DWF_PLNTSML:
        case TYPE_DWF_PLUTINO:
        case TYPE_DWF_CUBEWNO:
        case TYPE_DWF_TWOTINO:
        case TYPE_DWF_SCATTER:
        case TYPE_DWF_SEDNOID:

            //Generate clouds
            if (planet_ptr->has_atmo)
            {
                cloud_cover = rand_double(0.1, 0.7);
                cloud_thick = rand_double(0.0, 0.3);

                a_cloud_thick = cloud_cover *     cloud_thick * rand_double(0.50,0.90);
                a_cloud_thin  = cloud_cover *(1.0-cloud_thick)* rand_double(0.25,0.60);
            }

            sprintf(logtext, "         Generated %f percent cloud cover, %f percent thick (%f albedo) %f percent thin (%f albedo)\n", cloud_cover, cloud_thick, a_cloud_thick, (1.0-cloud_cover), a_cloud_thin);
            print_log(logtext);

            //Generate land
            if (planet_ptr->subtype[1] == '\0' && planet_ptr->bulk_water > 0.0)
                land  = fudge_double(rand_double((1.0/3.0), 0.5));
            else if (planet_ptr->subtype[1] == '\0' && planet_ptr->bulk_water == 0.0)
                land = 1.0;
            else if (planet_ptr->subtype[1] == '1')
                land  = fudge_double(rand_double(0.1, 0.15));
            else if (planet_ptr->subtype[1] == '2')
                land  = fudge_double(rand_double(0.05, 0.1));
            else
                land  = fudge_double(0.01);

            double sand = rand_double(0.0,1.0);
            a_land  = land*(1.0-sand)*rand_double(0.05,0.25) + land*sand*rand_double(0.2,0.4);
            a_land *= (1.0-cloud_cover);

            if (planet_ptr->bulk_water > 0.0)
            {
                //Freeze the polar caps
                if (planet_ptr->surf_temp > 273.15)
                    ice = ( 1.0 - cos( ( M_PI/4.0 ) - ( planet_ptr->axial_tilt * ( M_PI/180.0 ) ) ) );
                else
                    ice = 1.0;

                a_ocean = (1.0-land) * (1.0-ice) * rand_double(0.06,0.08) * (1.0-cloud_cover);
                a_ice   = (1.0-land) *      ice  * rand_double(0.65,0.75) * (1.0-cloud_cover);
            }

            sprintf(logtext, "         Generated surface, %f percent land (%f albedo) %f percent ocean (%f albedo)\n", land, a_land, (1.0-land), a_ocean);
            print_log(logtext);

            sprintf(logtext, "         Generated ice caps, %f percent of ocean (%f albedo)\n", ice, a_ice);
            print_log(logtext);

            sprintf(logtext,
                    "         Land albedo: %f | Ocean albedo: %f | Ice albedo: %f | Cloud albedo: %f | Total albedo: %f\n",
                    a_land,
                    a_ocean,
                    a_ice,
                    a_cloud_thick + a_cloud_thin,
                    a_land + a_ocean + a_ice + a_cloud_thick + a_cloud_thin
            );
            print_log(logtext);

            //Save percentages
            planet_ptr->rck_cover     =       land                     ;
            planet_ptr->ice_cover     =  (1.0-land) *      ice         ;
            planet_ptr->wtr_cover     =  (1.0-land) * (1.0-ice        );
            planet_ptr->cld_cover_thn = cloud_cover * (1.0-cloud_thick);
            planet_ptr->cld_cover_thk = cloud_cover *      cloud_thick ;

            //Sum albedo
            planet_ptr->albedo = a_land + a_ocean + a_ice + a_cloud_thick + a_cloud_thin;

            //Calculate temperature
            if ((star_ptr->zone_hab_min  <= planet_ptr->a) && (planet_ptr->a <= star_ptr->zone_hab_max))
            {
                planet_ptr->surf_temp = 65.0*fudge_double(1.1)*pow((1.0-planet_ptr->albedo)*340*(star_ptr->lumin/pow(planet_ptr->a, 2)), 0.25);
                planet_ptr->surf_temp += rand_double(35.0, 45.0);
            }
            else if (planet_ptr->has_atmo)
                planet_ptr->surf_temp = 65.0*rand_double(1.1,2.0)*pow(planet_ptr->albedo*340*(star_ptr->lumin/pow(planet_ptr->a, 2)), 0.25);
            else
                planet_ptr->surf_temp = 65.0*pow(planet_ptr->albedo*340*(star_ptr->lumin/pow(planet_ptr->a, 2)), 0.25);

            sprintf(logtext, "         Calculated surface temperature: %f K (%f °C)\n", planet_ptr->surf_temp, planet_ptr->surf_temp-KELVIN);
            print_log(logtext);

            if (planet_ptr->type == TYPE_WTR_HYCN  &&                //If the planet is an ocean planet
               (planet_ptr->surf_temp < KELVIN)    && (ice <  1.0))  //and the temp is below freezing but the planet isn't coved in ice
            {
                print_log("            Ocean planet is freezing but has no ice, rerandomizing\n");
                calc_albedo(star_ptr, planet_ptr); //redo
            }

            if (planet_ptr->type == TYPE_WTR_HYCN  &&               //If the planet is an ocean planet
               (planet_ptr->surf_temp > KELVIN)    && (ice == 1.0)) //and the temp is above freezing but the whole planet is frozen
            {
                print_log("            Ocean planet is above freezing but is frozen, rerandomizing\n");
                calc_albedo(star_ptr, planet_ptr); //redo
            }

            if ((planet_ptr->type == TYPE_RCK_GREEN || //If the planet is habitable
                 planet_ptr->type == TYPE_WTR_GREEN) &&
                (planet_ptr->surf_temp-KELVIN < 1.0) ) //and the temp is freezing
            {
                print_log("            Habitable planet is freezing, rerandomizing\n");
                calc_albedo(star_ptr, planet_ptr); //redo
            }
            break;

        case TYPE_GAS_GIANT:
        case TYPE_GAS_SUPER:
        case TYPE_GAS_PUFFY:
        case TYPE_GAS_HOT:
            //Decide albedo
            if (planet_ptr->subtype[1] == '1')
                planet_ptr->albedo = fudge_double(0.57*(2.0/3.0));
            else if (planet_ptr->subtype[1] == '2')
                planet_ptr->albedo = fudge_double(0.81);
            else if (planet_ptr->subtype[1] == '3')
                planet_ptr->albedo  = fudge_double(0.12);
            else if (planet_ptr->subtype[1] == '4')
                planet_ptr->albedo = fudge_double(0.03);
            else if (planet_ptr->subtype[1] == '5')
                planet_ptr->albedo = fudge_double(0.55);

            //Calculate received temp
            for (int tries = 0; true; tries++)
            {
                planet_ptr->surf_temp = 65.0*rand_double(1.0,2.0)*pow((1.0-planet_ptr->albedo)*340*(star_ptr->lumin/pow(planet_ptr->a, 2)), 0.25);
                print_log("         Calculated temperature\n");

                if (planet_ptr->type != TYPE_ICE_DWARF && planet_ptr->type != TYPE_ICE_GIANT)
                {
                    if (planet_ptr->surf_temp < 150)
                    {
                        if (planet_ptr->subtype[1] == '1')
                        {
                            if (planet_ptr->mass >= (GAS_RADIUS_LIM*(1.0+FUDGE_FACTOR)))
                                planet_ptr->type  = TYPE_GAS_SUPER;
                            else
                                planet_ptr->type  = TYPE_GAS_GIANT;

                            print_log("         Confirmed type as class 1\n");
                            break;
                        }
                        else
                        {
                            planet_ptr->subtype[1] = '1';
                            planet_ptr->albedo = fudge_double(0.57*(2.0/3.0));

                            print_log("         Reassigned type as class 1\n");
                        }
                    }
                    else if (150 < planet_ptr->surf_temp && planet_ptr->surf_temp < 250)
                    {
                        if (planet_ptr->subtype[1] == '2')
                        {
                            if (planet_ptr->mass >= (GAS_RADIUS_LIM*(1.0+FUDGE_FACTOR)))
                                planet_ptr->type  = TYPE_GAS_SUPER;
                            else
                                planet_ptr->type  = TYPE_GAS_GIANT;

                            print_log("         Confirmed type as class 2\n");

                            break;
                        }
                        else
                        {
                            planet_ptr->subtype[1] = '2';
                            planet_ptr->albedo = fudge_double(0.81);

                            print_log("         Reassigned type as class 2\n");
                        }
                    }
                    else if (250 < planet_ptr->surf_temp && planet_ptr->surf_temp < 850)
                    {
                        if (planet_ptr->subtype[1] == '3')
                        {
                            planet_ptr->type = TYPE_GAS_PUFFY;

                            print_log("         Confirmed type as class 3\n");

                            break;
                        }
                        else
                        {
                            planet_ptr->subtype[1] = '3';
                            planet_ptr->albedo = fudge_double(0.12);

                            print_log("         Reassigned type as class 3\n");
                        }
                    }
                    else if ( 850 < planet_ptr->surf_temp && planet_ptr->surf_temp < 1400)
                    {
                        if (planet_ptr->subtype[1] == '4')
                        {
                            planet_ptr->type = TYPE_GAS_HOT;

                            print_log("         Confirmed type as class 4\n");

                            break;
                        }
                        else
                        {
                            planet_ptr->subtype[1] = '4';
                            planet_ptr->albedo = fudge_double(0.03);

                            print_log("         Reassigned type as class 4\n");
                        }
                    }
                    else
                    {
                        if (planet_ptr->subtype[1] == '5')
                        {
                            planet_ptr->type = TYPE_GAS_HOT;

                            print_log("         Confirmed type as class 5\n");

                            break;
                        }
                        else
                        {
                            planet_ptr->subtype[1] = '5';
                            planet_ptr->albedo = fudge_double(0.55);

                            print_log("         Reassigned type as class 5\n");
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        case TYPE_ICE_DWARF:
        case TYPE_ICE_GIANT:
            planet_ptr->albedo    = fudge_double(0.3);
            planet_ptr->surf_temp = rand_double(50.0, 80.0);
            break;

        case TYPE_BLT_INNER:
        case TYPE_BLT_KUIPR:
            break;

        default:
            crash(-241);
    }
}

//Prints the given atmo gen to the log.
void log_atmo_gen(struct Planet *planet_ptr, int id, double remain)
{
    char logtext[100];
    sprintf(logtext, "         Generated %.15f %-26s %.15f remaining\n", planet_ptr->atmosphere[id], get_comp_name(id), remain);
    print_log(logtext);
}

#endif // ATMO_H
