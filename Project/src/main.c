#include <stdio.h>
#include <stdlib.h>
#include "../libs/log.h"

int main(int argc, char const *argv[])
{
    setup();
    loop();
    return 0;
}

/**
 * Funktion zum Vorbereiten der eigentlichen Anwendung
 */
void setup()
{
    setup_logging();
}

/**
 * Funktion zum Vorbereiten der Loggingfunktion der Anwendung
 */
void setup_logging()
{
    log_set_quiet(true);

    // erstellt die Datei, wenn sie noch nicht existiert 
    // oder nutzt sie, wenn schon existent
    FILE* fp = fopen("log.txt", "ab+");

    log_add_fp(fp, LOG_TRACE);
}

/**
 * Funktion, in der das eigentliche Hauptprogramm ausgeführt wird 
 */
void loop()
{
    //log_info("first line");
    //log_info("second line");
}

/**
 * Funktion um Benutzereingaben während der Ausführung dynamisch einzulesen
 */
char* get_input_line(void) {
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}