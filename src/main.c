/* Local includes */
#include "../header/assembler.h"

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file.as>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int i;
    for (i = 1; i < argc; i++){ /* Iterate through all the terminal-arguments */
        /* Open the input file */
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            perror("Error opening file");
            return EXIT_FAILURE;
        }

        /* Create a fixed output file */
        char* base_name = strchr(argv[i], '/');
        if (base_name){
            base_name++;
        } else {
            base_name = argv[1];
        }

        char* base_name_end = strchr(base_name, '.');
        if (base_name_end){
            *base_name_end = '\0';
        }

        /*
            Create/write to relevant .am file
        */

        char path[256];
        snprintf(path, sizeof(path), "./outputs/%s.am", base_name);
        FILE* am = fopen(path, "w+");
        if (am == NULL) {
            printf("Error opening .am file to write!\n");
            return EXIT_FAILURE;
        }

        /*
            Create/write to relevant .ob file
        */

        snprintf(path, sizeof(path), "./outputs/%s.ob", base_name);
        FILE* ob = fopen(path, "w");
        if (ob == NULL) {
            printf("Error opening .ob file to write!\n");
            return EXIT_FAILURE;
        }


        /*
            Create/write to relevant .ext file
        */

        snprintf(path, sizeof(path), "./outputs/%s.ext", base_name);
        FILE* ext = fopen(path, "w");
        if (ext == NULL) {
            printf("Error opening .ext file to write!\n");
            return EXIT_FAILURE;
        }

        /*
            Create/write to relevant .ent file
        */

        snprintf(path, sizeof(path), "./outputs/%s.ent", base_name);
        FILE* ent = fopen(path, "w");
        if (ent == NULL) {
            printf("Error opening .ent file to write!\n");
            return EXIT_FAILURE;
        }

        assemble(file, am, ob, ent, ext);
        fclose(file);
        fclose(am);
        fclose(ob);
        fclose(ext);
        fclose(ent);
    }
    
    return EXIT_SUCCESS;
}