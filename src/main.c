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
    for (i = 1; i < argc; i++){
        /* Open the input file */
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            perror("Error opening file");
            return EXIT_FAILURE;
        }

        /* Create a fixed output file */
        char* base_name = "test";

        /*
            Create/write to relevant .ob file
        */

        char ob_path[256];
        snprintf(ob_path, sizeof(ob_path), "./outputs/%s.ob", base_name);
        FILE* ob = fopen(ob_path, "w");
        if (ob == NULL) {
            printf("Error opening .ob file to write!\n");
            fclose(file);
            return EXIT_FAILURE;
        }


        /*
            Create/write to relevant .ext file
        */

        char ext_path[256];
        snprintf(ext_path, sizeof(ext_path), "./outputs/%s.ext", base_name);
        FILE* ext = fopen(ext_path, "w");
        if (ext == NULL) {
            printf("Error opening .ext file to write!\n");
            fclose(file);
            return EXIT_FAILURE;
        }

        /*
            Create/write to relevant .ent file
        */

        char ent_path[256];
        snprintf(ent_path, sizeof(ent_path), "./outputs/%s.ent", base_name);
        FILE* ent = fopen(ent_path, "w");
        if (ent == NULL) {
            printf("Error opening .ent file to write!\n");
            fclose(file);
            return EXIT_FAILURE;
        }

        assemble(file, ob);
        fclose(file);
        fclose(ob);
        fclose(ext);
        fclose(ent);
    }
    
    return EXIT_SUCCESS;
}