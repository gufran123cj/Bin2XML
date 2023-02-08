#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct record
{
    char name[64];    //utf16
    char surname[32]; //utf8
    char gender;
    char email[32];
    char phone_number[16];
    char address[32];
    char level_of_education[8];
    unsigned int income_level; // given little-endian
    unsigned int expenditure;  // given big-endian
    char currency_unit[16];
    char currentMood[32];
    float height;
    unsigned int weight;
};
struct headers
{
    char name[64];    //utf16
    char surname[32]; //utf8
    char gender;
    char email[32];
    char phone_number[16];
    char address[32];
    char level_of_education[8];
    unsigned int income_level; // given little-endian
    unsigned int expenditure;  // given big-endian
    char currency_unit[16];
    char currentMood[32];
    float height;
    unsigned int weight;
};
unsigned int tobigEndian(unsigned int number)
{
    number = ((number >> 24) & 0xff) |      // move byte 3 to byte 0
             ((number << 8) & 0xff0000) |   // move byte 1 to byte 2
             ((number >> 8) & 0xff00) |     // move byte 2 to byte 1
             ((number << 24) & 0xff000000); // byte 0 to byte 3
    return number;
}
int main(int argc, char const *argv[])
{
    struct record entry;
    struct headers header;
    FILE *finput;
    FILE *foutput;
    int entrycount = 0;
    if ((finput = fopen(argv[1], "rb,ccs=UTF-16LE")) == NULL) //open file with UTF-16
    {
        printf("\nError! File cannot be opened.\n"); //display error message if file doesnt exists
        exit(1);
    }
    foutput = fopen(argv[2], "w");
    fread(&header, sizeof(struct headers), 1, finput); //read the headers
    printf("\nName list with UTF-16:\n");
    do
    {
        fread(&entry, sizeof(struct record), 1, finput); //read a entry to a struct
        printf("%s\n", entry.name);
        entrycount = entrycount + 1;
    } while (entry.height != 0);
    fclose(finput);
    finput = fopen(argv[1], "rb");
    fread(&header, sizeof(struct headers), 1, finput); //read the headers
    //write as xml file
    char file_name[strlen(argv[2] - 4)];
    for(int i=0;i<strlen(argv[2])-4;i++){
        file_name[i]=argv[2][i];
    }
    fprintf(foutput,"<%s>\n",file_name);
    for (int i = 1; i < entrycount; i++)
    {
        fread(&entry, sizeof(struct record), 1, finput); //read a entry to a struct
        fprintf(foutput, "\t<row id=\"%d\">\n", i);
        fprintf(foutput, "\t\t<%s>%s</%s>\n", header.name, entry.name, header.name);
        fprintf(foutput, "\t\t<%s>%s</%s>\n", header.surname, entry.surname, header.surname);
        //correct g as gender
        if (header.gender = 'g')
        {
            fprintf(foutput, "\t\t<gender>%c</gender>\n", entry.gender);
        }
        else
        {
            fprintf(foutput, "\t\t<%s>%c</%s>\n", header.gender, entry.gender, header.gender);
        }
        fprintf(foutput, "\t\t<%s>%s</%s>\n", header.email, entry.email, header.email);
        fprintf(foutput, "\t\t<%s>%s</%s>\n", header.phone_number, entry.phone_number, header.phone_number);
        fprintf(foutput, "\t\t<%s>%s</%s>\n", header.address, entry.address, header.address);
        //correct level_of_ed as level_of_education
        if (strcmp(header.level_of_education, "level_of_ed") == 0)
        {
            fprintf(foutput, "\t\t<level_of_education>%s</level_of_education>\n", entry.level_of_education);
        }
        else
        {
            fprintf(foutput, "\t\t<%s>%s</%s>\n", header.level_of_education, entry.level_of_education, header.level_of_education);
        }
        fprintf(foutput, "\t\t<income_level bigEnd=\"%u\">%d</income_level>\n", tobigEndian(entry.income_level), entry.income_level);
        entry.expenditure = tobigEndian(entry.expenditure);
        fprintf(foutput, "\t\t<expenditure bigEnd=\"%u\">%d</expenditure>\n", tobigEndian(entry.expenditure), entry.expenditure);
        fprintf(foutput, "\t\t<%s>%s</%s>\n", header.currency_unit, entry.currency_unit, header.currency_unit);
        fprintf(foutput, "\t\t<%s>%s</%s>\n", header.currentMood, entry.currentMood, header.currentMood);
        fprintf(foutput, "\t\t<height>%.2f</height>\n", entry.height);
        fprintf(foutput, "\t\t<weight>%d</weight>\n", entry.weight);
        fprintf(foutput, "</row>\n\n");
    }
    fprintf(foutput,"</%s>\n",file_name);
    fclose(finput);
    fclose(foutput);
    printf("\nFile converted into xml.\n");
    return 0;
}
