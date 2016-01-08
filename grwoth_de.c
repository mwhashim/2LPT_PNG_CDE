/* By M. Hashim */
#include <math.h>
#include "allvars.h"
#include "proto.h"

static int NGrowthTable;
static struct growth_table
{
    double aa, Hubble_aa, growth_aa, F_Omegaa, F2_Omegaa;
}
 *GrowthTable;

//------------------------------------------
double omega_a(double a)
{
    return Omega/(Omega + a * (1 - Omega - OmegaLambda) + a * a * a * OmegaLambda);
}

//-------- Hubble function -----------------
double Hubble_func(double a)
{
    double Hubble;
    switch (WhichGrowth)
    {
        case 0:
            Hubble = sqrt(Omega/(a * a * a) + (1 - Omega - OmegaLambda) / (a * a) + OmegaLambda);
            break;
            
        case 1:
            Hubble =  HubbleFunc_Tabulated(a);
            break;
        default:
            Hubble = sqrt(Omega/(a * a * a) + (1 - Omega - OmegaLambda) / (a * a) + OmegaLambda);
            break;
    }
    return Hubble;
}

//--------- Growth rate --------------------
double GrowthFactor(double astart, double aend)
{
    return growth(aend)/growth(astart);
}

double growth(double a)
{
    double Growth;
    switch (WhichGrowth)
    {
        case 0:
            Growth = Hubble_func(a) * qromb(growth_int, 0, a);
            break;

        case 1:
            Growth =  GrowthFunc_Tabulated(a);
            break;
        default:
            Growth = Hubble_func(a) * qromb(growth_int, 0, a);
            break;
    }
    return Growth;
}

double F_Omega(double a)
{
    double FOmega;
    switch (WhichGrowth)
    {
        case 0:
            FOmega = pow(omega_a(a), 0.6);
            break;
            
        case 1:
            FOmega = F_Omega_Tabulated(a);
            break;
            
        default:
            FOmega = pow(omega_a(a), 0.6);
            break;
    }
    return FOmega;
}

double F2_Omega(double a)
{
    double F2Omega;
    switch (WhichGrowth)
    {
        case 0:
            F2Omega = 2 * pow(omega_a(a), 4./7.);
            break;
            
        case 1:
            F2Omega = F2_Omega_Tabulated(a);
            break;
            
        default:
            F2Omega = 2 * pow(omega_a(a), 4./7.);
            break;
    }
    return F2Omega;
}

//---------------------------------------
double growth_int(double a)
{
    return pow(a * omega_a(a)/Omega, 1.5);
}

//--------- Tabulated Growth & Background functions ------------
void read_growth_table(void)
{
    FILE *fd;
    char buf[500];
    double a, Hubble_a, growth_a, F_Omega, F2_Omega;
    int i;
    sprintf(buf, FileWithInputGrowth);
    if(!(fd = fopen(buf, "r")))
    {
        printf("can't read input Growth function in file '%s' on task %d\n", buf, ThisTask);
        FatalError(17);
    }
    
    NGrowthTable = 0;
    do
    {
        if(fscanf(fd, "%le %le %le %le %le", &a, &Hubble_a, &growth_a, &F_Omega, &F2_Omega) == 5)
            NGrowthTable++;
        else
            break;
    }
    while(1);
    fclose(fd);
    
    if(ThisTask == 0)
    {
        printf("found %d pairs of values in input growth function table\n", NGrowthTable);
        fflush(stdout);
    }
    
    GrowthTable = malloc(NGrowthTable * sizeof(struct growth_table));
    
    sprintf(buf, FileWithInputGrowth);
    
    if(!(fd = fopen(buf, "r")))
    {
        printf("can't read input transfer function in file '%s' on task %d\n", buf, ThisTask);
        FatalError(18);
    }
    
    NGrowthTable = 0;
    do
    {
        if(fscanf(fd, "%le %le %le %le %le", &a, &Hubble_a, &growth_a, &F_Omega, &F2_Omega) == 5)
        {
            GrowthTable[NGrowthTable].aa = a;
            GrowthTable[NGrowthTable].Hubble_aa = Hubble_a;
            GrowthTable[NGrowthTable].growth_aa = growth_a;
            GrowthTable[NGrowthTable].F_Omegaa = F_Omega;
            GrowthTable[NGrowthTable].F2_Omegaa = F2_Omega;
            NGrowthTable++;
        }
        else
        break;
    }
    while(1);
    fclose(fd);
}

//-------------------------------------------------
double HubbleFunc_Tabulated(double a)
{
    double lga, lgHubble_a, u, dlga;
    int binlow, binhigh, binmid;
    
    lga = log10(a);
    
    if(lga < log10(GrowthTable[0].aa) || lga > log10(GrowthTable[NGrowthTable - 1].aa))
    return 0;
    
    binlow = 0;
    binhigh = NGrowthTable - 1;
    
    while(binhigh - binlow > 1)
    {
        binmid = (binhigh + binlow) / 2;
        if(lga < log10(GrowthTable[binmid].aa))
            binhigh = binmid;
        else
            binlow = binmid;
    }
    
    dlga = log10(GrowthTable[binhigh].aa) - log10(GrowthTable[binlow].aa);
    
    if(dlga == 0)
    FatalError(777);
    
    u = (lga - log10(GrowthTable[binlow].aa))/dlga;
    lgHubble_a = (1 - u) * log10(GrowthTable[binlow].Hubble_aa) + u * log10(GrowthTable[binhigh].Hubble_aa);
    return pow(10.0, lgHubble_a);
}

double GrowthFunc_Tabulated(double a)
{
    double lga, lgGrowth_a, u, dlga;
    int binlow, binhigh, binmid;
    
    lga = log10(a);
    
    if(lga < log10(GrowthTable[0].aa) || lga > log10(GrowthTable[NGrowthTable - 1].aa))
    return 0;
    
    binlow = 0;
    binhigh = NGrowthTable - 1;
    
    while(binhigh - binlow > 1)
    {
        binmid = (binhigh + binlow) / 2;
        if(lga < log10(GrowthTable[binmid].aa))
            binhigh = binmid;
        else
            binlow = binmid;
    }
    
    dlga = log10(GrowthTable[binhigh].aa) - log10(GrowthTable[binlow].aa);
    
    if(dlga == 0)
        FatalError(777);
    
    u = (lga - log10(GrowthTable[binlow].aa))/dlga;
    lgGrowth_a = (1 - u) * log10(GrowthTable[binlow].growth_aa) + u * log10(GrowthTable[binhigh].growth_aa);
    return pow(10.0, lgGrowth_a);
}

double F_Omega_Tabulated(double a)
{
    double lga, lgF_Omega, u, dlga;
    int binlow, binhigh, binmid;
    
    lga = log10(a);
    
    if(lga < log10(GrowthTable[0].aa) || lga > log10(GrowthTable[NGrowthTable - 1].aa))
    return 0;
    
    binlow = 0;
    binhigh = NGrowthTable - 1;
    
    while(binhigh - binlow > 1)
    {
        binmid = (binhigh + binlow) / 2;
        if(lga < log10(GrowthTable[binmid].aa))
        binhigh = binmid;
        else
        binlow = binmid;
    }
    
    dlga = log10(GrowthTable[binhigh].aa) - log10(GrowthTable[binlow].aa);
    
    if(dlga == 0)
    FatalError(777);
    
    u = (lga - log10(GrowthTable[binlow].aa))/dlga;
    lgF_Omega = (1 - u) * log10(GrowthTable[binlow].F_Omegaa) + u * log10(GrowthTable[binhigh].F_Omegaa);
    return pow(10.0, lgF_Omega);
}

double F2_Omega_Tabulated(double a)
{
    double lga, lgF2_Omega, u, dlga;
    int binlow, binhigh, binmid;
    
    lga = log10(a);
    
    if(lga < log10(GrowthTable[0].aa) || lga > log10(GrowthTable[NGrowthTable - 1].aa))
    return 0;
    
    binlow = 0;
    binhigh = NGrowthTable - 1;
    
    while(binhigh - binlow > 1)
    {
        binmid = (binhigh + binlow) / 2;
        if(lga < log10(GrowthTable[binmid].aa))
        binhigh = binmid;
        else
        binlow = binmid;
    }
    
    dlga = log10(GrowthTable[binhigh].aa) - log10(GrowthTable[binlow].aa);
    
    if(dlga == 0)
    FatalError(777);
    
    u = (lga - log10(GrowthTable[binlow].aa))/dlga;
    lgF2_Omega = (1 - u) * log10(GrowthTable[binlow].F2_Omegaa) + u * log10(GrowthTable[binhigh].F2_Omegaa);
    return pow(10.0, lgF2_Omega);
}


//-------------------------------------------
void initialize_growthfunction(void)
{
    if(WhichGrowth == 1)
        read_growth_table();
}
