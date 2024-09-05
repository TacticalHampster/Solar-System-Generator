#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs.h"
#include "utils.h"

#ifndef ATMO_H
#define ATMO_H

void gen_planet_atmos(struct Star *star_ptr, struct Planet *planet_ptr);

void round_robin(double *comps, double *remain, int num, int names[]);
char* get_comp_name(int num);
void calc_albedo(struct Star *star_ptr, struct Planet *planet_ptr);

//This is the top level function. It should be the only function called outside this file.
void gen_planet_atmos(struct Star *star_ptr, struct Planet *planet_ptr)
{
    char logtext[100];

    planet_ptr->has_atmo = true;
    double remain  = 1.0;
    double frac    = 0.0;
    double density = 0.0;
    double comps[11];

    //Create atmo array
    double *atmo = (double*)malloc(sizeof(double)*24);
    if (atmo == nullptr)
        crash(-130);

    //Zero array
    for (int i = 0; i < 24; i++)
        atmo[i] = 0;

    //Zero comps array
    for (int i = 0; i < 11; i++)
        comps[i] = 0;

    /*
     0    "Hydrogen"
     1    "Helium"
     2    "Argon"
     3    "Nitrogen"
     4    "Oxygen"
     5    "Fluorine"
     6    "Chlorine"
     7    "Bromine"
     8    "Iodine"
     9    "Neon"
    10    "Krypton"
    11    "Xenon"
    12    "Deuteride"
    13    "Methane"
    14    "Ethane"
    15    "Ammonia"
    16    "Phosphine"
    17    "Water vapor"
    18    "H-Sulfide"
    19    "C-Oxides"
    20    "S-Oxides"
    21    "N-Oxides"
    22    "Tholins"
    23    "Cyanide"
    */

    switch (planet_ptr->type)
    {
        case TYPE_RCK_DENSE:
        case TYPE_RCK_DESRT:

            //Major components
            print_log("      Generating major components:\n");

            for (;;)
            {
                frac = fudge_double(0.0001);

                //Nitrogen
                atmo[3] = fudge_double(0.03-(0.03*FUDGE_FACTOR));
                remain -= atmo[3];
                sprintf(logtext, "         Generated %.15f nitrogen,         %.15f remaining\n", atmo[3], remain);
                print_log(logtext);

                //Argon
                atmo[2] = rand_double(0.005, 0.02);
                remain -= atmo[2];
                sprintf(logtext, "         Generated %.15f argon,            %.15f remaining\n", atmo[3], remain);
                print_log(logtext);

                //Carbons
                atmo[19] = remain-frac;
                remain  -= atmo[19];
                sprintf(logtext, "         Generated %.15f carbons,          %.15f remaining\n", atmo[19], remain);
                print_log(logtext);

                //Make sure remain is in an interesting range
                if (!(0.0001 < remain && remain <= 0.0005))
                {
                    print_log("            Remainder is outside range, rerandomizing\n");

                    //Reset
                    atmo[ 3] = 0;
                    atmo[ 2] = 0;
                    atmo[19] = 0;
                    remain = 1.0;
                }
                else
                    break;
            }

            /*
                Minor components
                    Oxygen
                    Fluorine
                    Chlorine
                    Bromine
                    Iodine
                    Neon
                    Krypton
                    Xenon
                    Methane
                    Ammonia
                    Phosphine
                    Hydrogen sulfide
                    S-Oxides
                    N-Oxides
             */

            sprintf(logtext, "      Attempting round robin for volcanic minor components:\n");
            print_log(logtext);

            int names_v[] = {6, 9, 4, 13, 15, 16, 18, 20, 21};
            round_robin(comps, &remain, 9, names_v);

            //Pick a halogen
            frac = floor(rand_double(0.0, 7.0));
            if (frac < 3.0)
                atmo[6] = comps[0]; //Chlorine
            else if (frac < 5.0)
                atmo[5] = comps[0]; //Fluorine
            else if (frac == 5.0)
                atmo[7] = comps[0]; //Bromine
            else
                atmo[8] = comps[0]; //Iodine

            //Pick a noble gas
            frac = floor(rand_double(0.0, 5.0));
            if (frac < 2.0)
                atmo[ 9] = comps[1]; //Neon
            else if (frac < 4.0)
                atmo[10] = comps[1]; //Krypton
            else
                atmo[11] = comps[1]; //Xenon

            atmo[ 4] = comps[2]; //Oxygen
            atmo[13] = comps[3]; //Methane
            atmo[15] = comps[4]; //Ammonia
            atmo[16] = comps[5]; //Phosphine
            atmo[18] = comps[6]; //Hydrogen sulfide
            atmo[20] = comps[7]; //Sulfur oxides
            atmo[21] = comps[8]; //Nitrogen oxides
            break;

        case TYPE_WTR_HYCN:

            //Major components
            print_log("      Generating major components:\n");

            for (;;)
            {
                frac = fudge_double(0.0001);

                //Water vapor
                atmo[17] = fudge_double(0.03-(0.03*FUDGE_FACTOR));
                remain  -= atmo[17];
                sprintf(logtext, "         Generated %.15f water vapor,      %.15f remaining\n", atmo[17], remain);
                print_log(logtext);

                //Nitrogen
                atmo[3] = fudge_double(0.03-(0.03*FUDGE_FACTOR));
                remain -= atmo[3];
                sprintf(logtext, "         Generated %.15f nitrogen,         %.15f remaining\n", atmo[3], remain);
                print_log(logtext);

                //Carbons
                atmo[19] = remain-frac;
                remain  -= atmo[19];
                sprintf(logtext, "         Generated %.15f carbons,          %.15f remaining\n", atmo[19], remain);
                print_log(logtext);

                //Make sure remain is in an interesting range
                if (!(0.0001 < remain && remain <= 0.0005))
                {
                    print_log("            Remainder is outside range, rerandomizing\n");

                    //Reset
                    atmo[17] = 0;
                    atmo[ 3] = 0;
                    atmo[19] = 0;
                    remain = 1.0;
                }
                else
                    break;
            }

            /*
                Minor components
                    Oxygen
                    Fluorine
                    Chlorine
                    Bromine
                    Iodine
                    Argon
                    Neon
                    Krypton
                    Xenon
                    Methane
                    Ammonia
                    Phosphine
                    Hydrogen sulfide
                    S-Oxides
                    N-Oxides
             */

            sprintf(logtext, "      Attempting round robin for hycean minor components:\n");
            print_log(logtext);

            int names_w[] = {6, 9, 2, 4, 13, 15, 16, 18, 20, 21};
            round_robin(comps, &remain, 10, names_w);

            //Pick a halogen
            frac = floor(rand_double(0.0, 7.0));
            if (frac < 3.0)
                atmo[6] = comps[0]; //Chlorine
            else if (frac < 5.0)
                atmo[5] = comps[0]; //Fluorine
            else if (frac == 5.0)
                atmo[7] = comps[0]; //Bromine
            else
                atmo[8] = comps[0]; //Iodine

            //Pick a noble gas
            frac = floor(rand_double(0.0, 5.0));
            if (frac < 2.0)
                atmo[ 9] = comps[1]; //Neon
            else if (frac < 4.0)
                atmo[10] = comps[1]; //Krypton
            else
                atmo[11] = comps[1]; //Xenon

            atmo[ 2] = comps[2]; //Argon
            atmo[ 4] = comps[3]; //Oxygen
            atmo[13] = comps[4]; //Methane
            atmo[15] = comps[5]; //Ammonia
            atmo[16] = comps[6]; //Phosphine
            atmo[18] = comps[7]; //Hydrogen sulfide
            atmo[20] = comps[8]; //Sulfur oxides
            atmo[21] = comps[9]; //Nitrogen oxides
            break;

        case TYPE_RCK_GREEN:
        case TYPE_WTR_GREEN:

            //Major components
            print_log("      Generating major components:\n");

            for (;;)
            {
                frac = fudge_double(0.0005);

                //Oxygen
                atmo[4] = fudge_double((1.0/3.0)-((1.0/3.0)*FUDGE_FACTOR));
                remain -= atmo[4];
                sprintf(logtext, "         Generated %.15f oxygen,           %.15f remaining\n", atmo[4], remain);
                print_log(logtext);

                //Argon
                atmo[2] = fudge_double(0.01);
                remain -= atmo[2];
                sprintf(logtext, "         Generated %.15f argon,            %.15f remaining\n", atmo[2], remain);
                print_log(logtext);

                //Nitros
                atmo[3] = remain-frac;
                remain -= atmo[3];
                sprintf(logtext, "         Generated %.15f nitrogen,         %.15f remaining\n", atmo[3], remain);
                print_log(logtext);

                //Make sure remain is in an interesting range
                if (!(0.0001 < remain && remain <= 0.0005))
                {
                    print_log("            Remainder is outside range, rerandomizing\n");

                    //Reset
                    atmo[4] = 0;
                    atmo[2] = 0;
                    atmo[3] = 0;
                    remain = 1.0;
                }
                else
                    break;
            }

            /*
                Minor components
                    Fluorine
                    Chlorine
                    Bromine
                    Iodine
                    Neon
                    Krypton
                    Xenon
                    Methane
                    Ammonia
                    Phosphine
                    Water vapor
                    H-Sulfide
                    C-Oxides
                    S-Oxides
                    N-Oxides
             */

            sprintf(logtext, "      Attempting round robin for green minor components:\n");
            print_log(logtext);

            int names_h[] = {6, 9, 13, 15, 16, 17, 18, 19, 20, 21};
            round_robin(comps, &remain, 10, names_h);

            //Pick a halogen
            frac = floor(rand_double(0.0, 7.0));
            if (frac < 3.0)
                atmo[6] = comps[0]; //Chlorine
            else if (frac < 5.0)
                atmo[5] = comps[0]; //Fluorine
            else if (frac == 5.0)
                atmo[7] = comps[0]; //Bromine
            else
                atmo[8] = comps[0]; //Iodine

            //Pick a noble gas
            frac = floor(rand_double(0.0, 5.0));
            if (frac < 2.0)
                atmo[ 9] = comps[1]; //Neon
            else if (frac < 4.0)
                atmo[10] = comps[1]; //Krypton
            else
                atmo[11] = comps[1]; //Xenon

            //Transfer other compounds
            atmo[13] = comps[2]; //Methane
            atmo[15] = comps[3]; //Ammonia
            atmo[16] = comps[4]; //Phosphine
            atmo[17] = comps[5]; //Water vapor
            atmo[18] = comps[6]; //Hydrogen sulfide
            atmo[19] = comps[7]; //Carbon oxides
            atmo[20] = comps[8]; //Sulfur oxides
            atmo[21] = comps[9]; //Nitrogen oxides
            break;

        case TYPE_GAS_GIANT:
        case TYPE_GAS_SUPER:
        case TYPE_GAS_PUFFY:
        case TYPE_GAS_HOT:
        case TYPE_ICE_DWARF:
        case TYPE_ICE_GIANT:
            //Major components
            print_log("      Generating major components:\n");

            for (;;)
            {
                //Hydrogen
                atmo[0] = rand_double(0.85,0.91);
                remain -= atmo[0];
                sprintf(logtext, "         Generated %.15f hydrogen,         %.15f remaining\n", atmo[0], remain);
                print_log(logtext);

                //Helium
                atmo[1] = remain*fudge_double(0.9);
                remain -= atmo[1];
                sprintf(logtext, "         Generated %.15f helium,           %.15f remaining\n", atmo[1], remain);
                print_log(logtext);

                //Methane
                atmo[13] = remain*fudge_double(0.9);
                remain -= atmo[13];
                sprintf(logtext, "         Generated %.15f methane,          %.15f remaining\n", atmo[13], remain);
                print_log(logtext);

                //Make sure remain is in an interesting range
                if (!(0.0001 < remain && remain <= 0.0005))
                {
                    print_log("            Remainder is outside range, rerandomizing\n");

                    //Reset
                    atmo[ 0] = 0;
                    atmo[ 1] = 0;
                    atmo[13] = 0;
                    remain = 1.0;
                }
                else
                    break;
            }

            /*
                Minor components:
                    Oxygen
                    Neon
                    Deuteride
                    Ethane
                    Ammonia
                    Phosphine
                    Water
                    Hydrogen sulfide
                    Sulfur oxides
                    Tholins
                    Cyanide
            */

            sprintf(logtext, "      Attempting round robin for primordial minor components:\n");
            print_log(logtext);

            int names_g[] = {4, 9, 12, 14, 15, 16, 17, 18, 19, 22, 23};
            round_robin(comps, &remain, 11, names_g);

            atmo[ 4] = comps[ 0]; //Oxygen
            atmo[ 9] = comps[ 1]; //Neon
            atmo[12] = comps[ 2]; //Deuteride
            atmo[14] = comps[ 3]; //Ethane
            atmo[15] = comps[ 4]; //Ammonia
            atmo[16] = comps[ 5]; //Phosphine
            atmo[17] = comps[ 6]; //Water
            atmo[18] = comps[ 7]; //Hydrogen sulfide
            atmo[19] = comps[ 8]; //Sulfur oxide
            atmo[22] = comps[ 9]; //Tholins
            atmo[23] = comps[10]; //Cyanide
            break;

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
            crash(-240);
    }

    //Make sure all numbers are positive
    if (remain < 0.0)
    {
        print_log("         Remainder is negative, rerandomizing\n");
        free((void*)atmo);
        gen_planet_atmos(star_ptr, planet_ptr);
    }

    //Transfer
    planet_ptr->atmosphere = atmo;

    frac = 0;

    //Create detailed atmo array for density and mol weight calculations
    atmo = (double*)malloc(sizeof(double)*28);
    if (atmo == nullptr)
        crash(-131);

    //Atomic weights
    double weights[28] = {
        0.00201588,  //Hydrogen
        0.0040026,   //Helium
        0.039948,    //Argon
        0.0280134,   //Nitrogen
        0.031988,    //Oxygen
        0.0379968,   //Fluorine
        0.0709056,   //Chlorine
        0.159808,    //Bromine
        0.2538089,   //Iodine
        0.0201797,   //Neon
        0.083798,    //Krypton
        0.131293,    //Xenon
        0.00302204,  //Deuteride
        0.01604,     //Methane
        0.03006904,  //Ethane
        0.017031,    //Ammonia
        0.033997581, //Phosphine
        0.01801528,  //Water
        0.03408,     //Hydrogen sulfide
        0.02801,     //CO
        0.04401,     //CO2
        0.064066,    //S02
        0.080066,    //S03
        0.03001,     //NO
        0.0460055,   //NO2
        0.044013,    //N2O
        rand_double(0.01, 0.2), //Tholins
        0.0270253    //Cyanide
    };

    /*
     0    "Hydrogen"
     1    "Helium"
     2    "Argon"
     3    "Nitrogen"
     4    "Oxygen"
     5    "Fluorine"
     6    "Chlorine"
     7    "Bromine"
     8    "Iodine"
     9    "Neon"
    10    "Krypton"
    11    "Xenon"
    12    "Deuteride"
    13    "Methane"
    14    "Ethane"
    15    "Ammonia"
    16    "Phosphine"
    17    "Water vapor"
    18    "H-Sulfide"
    19    "C-Oxides"
    20    "S-Oxides"
    21    "N-Oxides"
    22    "Other gases"
    */

    calc_albedo(star_ptr, planet_ptr);

    print_log("      Calculating atomic weight:\n");

    //Make sure there is an atmosphere
    if (planet_ptr->has_atmo)
    {
        for (int i = 0; i < 19; i++)
        {
            atmo[i] = planet_ptr->atmosphere[i];
        }

        sprintf(logtext, "         Transferred simple compounds\n");
        print_log(logtext);

        //Carbons
        frac = fudge_double(0.99);
        atmo[20] = planet_ptr->atmosphere[19]*(frac > 1.0 ? 1.0 : frac);     //Carbon monoxide    CO
        atmo[19] = planet_ptr->atmosphere[19]-atmo[20]                 ;     //Carbon dioxide     CO2

        sprintf(logtext, "         Calculated carbon oxides\n");
        print_log(logtext);

        //Sulfurs
        frac = fudge_double(0.99);
        atmo[21] = planet_ptr->atmosphere[20]*(frac > 1.0 ? 1.0 : frac);     //Sulfur dioxide     SO2
        atmo[22] = planet_ptr->atmosphere[20]-atmo[21]                 ;     //Sulfur trioxide    SO3

        sprintf(logtext, "         Calculated sulfur oxides\n");
        print_log(logtext);

        //Nitros
        frac = fudge_double(0.95);
        atmo[23] =  planet_ptr->atmosphere[21]*(frac > 1.0 ? 1.0 : frac)  ; //Nitric oxide       NO
        atmo[24] = (planet_ptr->atmosphere[21]-atmo[23])*fudge_double(0.5); //Nitrogen dioxide   NO2
        atmo[25] = (planet_ptr->atmosphere[21]-atmo[23]-atmo[24])         ; //Nitrous oxide      N20

        sprintf(logtext, "         Calculated nitrogen oxides\n");
        print_log(logtext);

        //Tholins
        atmo[26] = planet_ptr->atmosphere[22];

        //Cyanide
        atmo[27] = planet_ptr->atmosphere[23];

        //Calculate average atomic weight
        planet_ptr->atmo_dens = 0.0;
        for (int i = 0; i < 28; i++)
        {
            density += (atmo[i]) * (weights[i]);
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
            sprintf(logtext, " %-17s"                  , get_comp_name(names[choice]));
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

//Gets a string name for an atmo comp based on its id
char* get_comp_name(int num)
{
    switch (num)
    {
        case  0: return "hydrogen,"        ;
        case  1: return "helium,"          ;
        case  2: return "argon,"           ;
        case  3: return "nitrogen,"        ;
        case  4: return "oxygen,"          ;
        case  5: return "random halogen,"  ;
        case  6: return "random halogen,"  ;
        case  7: return "random halogen,"  ;
        case  8: return "random halogen,"  ;
        case  9: return "random noble gas,";
        case 10: return "random noble gas,";
        case 11: return "random noble gas,";
        case 12: return "deuteride,"       ;
        case 13: return "methane,"         ;
        case 14: return "ethane,"          ;
        case 15: return "ammonia,"         ;
        case 16: return "phosphine,"       ;
        case 17: return "water, "          ;
        case 18: return "hydrogen sulfide,";
        case 19: return "carbon oxides,"   ;
        case 20: return "sulfur oxides,"   ;
        case 21: return "nitrogen oxides," ;
        case 22: return "tholins,"         ;
        case 23: return "cyanides,"        ;
        //Special cases for the photosphere
        case 24: return "neon,"            ;
        case 25: return "sodium,"          ;
        case 26: return "magnesium,"       ;
        case 27: return "silicon,"         ;
        case 28: return "phosphorous,"     ;
        case 29: return "sulfur,"          ;
        default: return "other gases,"     ;
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

#endif // ATMO_H
