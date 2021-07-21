/**
 * climate.c
 *
 * Performs analysis on climate data provided by the
 * National Oceanic and Atmospheric Administration (NOAA).
 *
 * Input:    Tab-delimited file(s) to analyze.
 * Output:   Summary information about the data.
 *
 * Compile:  run make
 *
 * Example Run:      ./climate data_tn.tdv data_wa.tdv
 *
 *
 * Opening file: data_tn.tdv
 * Opening file: data_wa.tdv
 * States found: TN WA
 * -- State: TN --
 * Number of Records: 17097
 * Average Humidity: 49.4%
 * Average Temperature: 58.3F
 * Max Temperature: 110.4F 
 * Max Temperatuer on: Mon Aug  3 11:00:00 2015
 * Min Temperature: -11.1F
 * Min Temperature on: Fri Feb 20 04:00:00 2015
 * Lightning Strikes: 781
 * Records with Snow Cover: 107
 * Average Cloud Cover: 53.0%
 * -- State: WA --
 * Number of Records: 48357
 * Average Humidity: 61.3%
 * Average Temperature: 52.9F
 * Max Temperature: 125.7F
 * Max Temperature on: Sun Jun 28 17:00:00 2015
 * Min Temperature: -18.7F 
 * Min Temperature on: Wed Dec 30 04:00:00 2015
 * Lightning Strikes: 1190
 * Records with Snow Cover: 1383
 * Average Cloud Cover: 54.5%
 *
 * TDV format:
 *
 * CA» 1428300000000»  9prcjqk3yc80»   93.0»   0.0»100.0»  0.0»95644.0»277.58716
 * CA» 1430308800000»  9prc9sgwvw80»   4.0»0.0»100.0»  0.0»99226.0»282.63037
 * CA» 1428559200000»  9prrremmdqxb»   61.0»   0.0»0.0»0.0»102112.0»   285.07513
 * CA» 1428192000000»  9prkzkcdypgz»   57.0»   0.0»100.0»  0.0»101765.0» 285.21332
 * CA» 1428170400000»  9prdd41tbzeb»   73.0»   0.0»22.0»   0.0»102074.0» 285.10425
 * CA» 1429768800000»  9pr60tz83r2p»   38.0»   0.0»0.0»0.0»101679.0»   283.9342
 * CA» 1428127200000»  9prj93myxe80»   98.0»   0.0»100.0»  0.0»102343.0» 285.75
 * CA» 1428408000000»  9pr49b49zs7z»   93.0»   0.0»100.0»  0.0»100645.0» 285.82413
 *
 * Each field is separated by a tab character \t and ends with a newline \n.
 *
 * Fields:
 *      state code (e.g., CA, TX, etc),
 *      timestamp (time of observation as a UNIX timestamp),
 *      geolocation (geohash string),
 *      humidity (0 - 100%),
 *      snow (1 = snow present, 0 = no snow),
 *      cloud cover (0 - 100%),
 *      lightning strikes (1 = lightning strike, 0 = no lightning),
 *      pressure (Pa),
 *      surface temperature (Kelvin)
 */
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_STATES 50

/* TODO: Add elements to the climate_info struct as necessary. */
struct climate_info {
    //creating variables
    char code[3];
    unsigned long num_records;
    long double sum_temperature;
    long double max_temperature;
    long double min_temperature;
    long sum_cloud_coverage;
    long sum_humidity;
    int lightning_strike;
    int snow;
    time_t max_time;
    time_t min_time;
};

void analyze_file(FILE *file, struct climate_info *states[], int num_states);
void print_report(struct climate_info *states[], int num_states);

int main(int argc, char *argv[]) {

    /* TODO: fix this conditional. You should be able to read multiple files. */
    if (argc < 2) {
        printf("Usage: %s tdv_file1 tdv_file2 ... tdv_fileN \n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Let's create an array to store our state data in. As we know, there are
     * 50 US states. */
    struct climate_info *states[NUM_STATES] = { NULL };
    FILE *fp;
    char *file_name = "";
    int i;

    for (i = 1; i < argc; ++i) {
        /* TODO: Open the file for reading */
        file_name = argv[i];
        printf("Opening file: %s\n",file_name);
        fp = fopen(file_name,"r");

        /* TODO: If the file doesn't exist, print an error message and move on
         * to the next file. */
        if(fp == NULL)
        {
            printf("Error in opening file.\n");
            return EXIT_FAILURE;
        }

        /* TODO: Analyze the file */
        /* analyze_file(file, states, NUM_STATES); */
        analyze_file(fp, states, NUM_STATES);
        fclose(fp);
    }

    /* Now that we have recorded data for each file, we'll summarize them: */
    print_report(states, NUM_STATES);

    return 0;
}

void analyze_file(FILE *file, struct climate_info **states, int num_states) {
    const int line_sz = 100;
    char line[line_sz];
    //storing data from the file to struct array
    char code[3];
    double timestamp;
    char *geo_location;
    double humidity;
    double snow;
    double cloud_coverage;
    double lightning_strikes;
    double presssure;
    double temperature;
    time_t current;
    char *token;
    int count = 0;
    int state_code; // how many states exist already

    //chekc how many states already exist in the state array, and get the index
    for(count = 0; count < num_states; count++)
    {
        if(states[count]==NULL)
            {
                state_code = count;
                break;
            }
    }

    while (fgets(line, line_sz, file) != NULL) 
    {
        //get the state code
        token = strtok(line, "\t");
        strcpy(code, token);
        //get the timestamp
        token = strtok(NULL, "\t");
        timestamp = atof(token) / 1000;
        current = timestamp;
        //strcpy(timestamp, token);
        
        //get the geolocation
        token = strtok(NULL, "\t");
        //get the humidity
        token = strtok(NULL, "\t");
        humidity = atof(token);
        //get the snow count
        token = strtok(NULL, "\t");
        snow = atof(token);
        //get the cloud cover
        token = strtok(NULL, "\t");
        cloud_coverage = atof(token);
        //get the lightning strikes count
        token = strtok(NULL, "\t");
        lightning_strikes = atof(token);
        //get the pressure
        token = strtok(NULL, "\t");
        presssure = atof(token);
        //get the temperature
        token = strtok(NULL, "\t");
        temperature = atof(token);


        //create struct climate_info
        //storing data
        struct climate_info *ci = (struct climate_info*)malloc(sizeof(struct climate_info)*1);
        strcpy(ci->code, code); 
        ci->num_records = 1;
        ci->sum_temperature = temperature;
        ci->max_temperature = temperature;
        ci->min_temperature = temperature;
        ci->sum_cloud_coverage = cloud_coverage;
        ci->sum_humidity = humidity;
        ci->lightning_strike = lightning_strikes;
        ci->snow = snow;
        ci->max_time = current;
        ci->min_time = current;

        for(int i = 0; i < count; i++)
        {
            //if multiple file detected,
            if((states[state_code]==NULL) && (strcmp(ci->code, states[state_code-1]->code) != 0))
            {
                states[state_code] = ci; //new state found
                break;
            }
            //check state if already exist
            if(strcmp(states[state_code]->code, ci->code) == 0)
            {
                states[state_code]->num_records++;
                states[state_code]->lightning_strike += lightning_strikes;
                //find max_temperature
                if(states[state_code]->max_temperature < temperature)
                {
                    states[state_code]->max_temperature = temperature;
                    states[state_code]->max_time = current;
                }
                //find min_temperature
                if(states[state_code]->min_temperature > temperature)
                {
                    states[state_code]->min_temperature = temperature;
                    states[state_code]->min_time = current;
                }
                states[state_code]->snow += snow;
                states[state_code]->sum_cloud_coverage += cloud_coverage;
                states[state_code]->sum_humidity += humidity;
                states[state_code]->sum_temperature += temperature;
                break;
           }
           //check if it is a new state in the same file
            if(strcmp(states[state_code]->code,ci->code) != 0)
            {
                state_code+=1;
                states[state_code] = ci;
                break;
            }
        }
        //create first state
        if(!count)
        {
            // first new state found
            states[state_code] = ci;
            count+=1;
        }        
    }
}

void print_report(struct climate_info *states[], int num_states) {
    printf("States found:\n");
    int i;
    for (i = 0; i < num_states; ++i) {
        if (states[i] != NULL) {
            struct climate_info *info = states[i];
            printf("%s ", info->code);
        }
    }
    printf("\n");

    /* TODO: Print out the summary for each state. See format above. */
    for(i = 0; i < num_states; i++)
    {
        if(states[i] != NULL)
        {
            printf("\n-- State: %s --\n", states[i]->code);
            printf("Number of Records: %lu\n", states[i]->num_records);
            printf("Average Humidity: %.1f%%\n", (double)states[i]->sum_humidity/states[i]->num_records);
            printf("Average Temperature: %.1LfF\n", (states[i]->sum_temperature/states[i]->num_records)*1.8-459.67);
            printf("Max Temperature: %LfF\n", (states[i]->max_temperature)*1.8-459.67);
            printf("Max Temperature on: %s", ctime(&states[i]->max_time));
            printf("Min Temperature: %LfF\n", (states[i]->min_temperature)*1.8-459.67);
            printf("Min Temperature on: %s", ctime(&states[i]->min_time));
            printf("Lightning Strikes: %d\n", states[i]->lightning_strike);
            printf("Records with Snow Cover: %d\n", states[i]->snow);
            printf("Average Cloud Cover: %.1f%%\n", (double)states[i]->sum_cloud_coverage/states[i]->num_records);
        }
    }
}