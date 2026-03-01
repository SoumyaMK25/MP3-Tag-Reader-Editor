#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reading.h"
#include "type.h"

Status open_files(Read_MP3 *readmp3)
{
    readmp3->fptr_audio = fopen(readmp3->audio_fname, "rb");
    if (readmp3->fptr_audio == NULL)
    {
    	perror("fopen");    
    	fprintf(stderr, "ERROR: Unable to open file %s\n", readmp3->audio_fname);
    	return e_failure;
    }
    printf("File opened successfully\n");
    return e_success;
}


Status read_and_validate_read_args(char *argv[], Read_MP3 *readmp3)
{
  
         if(argv[2] == NULL)
         {
         printf("The .mp3 file not found\n");
         return e_failure;
         }
         
        if(strstr(argv[2], ".mp3") == NULL)
          {
           printf("Invalid Audio File Name\n");
           return e_failure;
          }
           
     readmp3->audio_fname = argv[2];
     return e_success;
}


Status do_reading(Read_MP3 *readmp3)
{
    char tag[5];
    unsigned int size;
    Status ret;

    ret = open_files(readmp3);
    if (ret == e_failure)
    {
        return e_failure;
    }

    ret = skip_header(readmp3->fptr_audio);
    if (ret == e_failure)
    {
        return e_failure;
    }

    while(read_tag(readmp3->fptr_audio, tag) == e_success)
    {
        ret = read_tag_size(readmp3->fptr_audio, &size);
        if (ret == e_failure)
        {
           return e_failure;
        }

        if (size == 0)
        {
           return e_failure;
        }

        ret = skip_flag(readmp3->fptr_audio);
        if (ret == e_failure)
        {
            return e_failure;
        }

        if (compare_tags(tag) == e_success)
        {
            printf("Tag : %s\n", tag);

            ret = read_tag_data(readmp3->fptr_audio, size);
            if (ret == e_failure)
            {
                return e_failure;
            }
        }
        else
        {
            fseek(readmp3->fptr_audio, size, SEEK_CUR);     
        }
    }

    fclose(readmp3->fptr_audio);
    return e_success;
}

        
Status skip_header(FILE *fptr_audio)
{

    fseek(fptr_audio, 10, SEEK_SET);
    printf("Successfulyy skipped 10 byte header!...\n");
    printf("After skipping 10 byte header then offset at %ld\n", ftell(fptr_audio));
    printf("\n");
    return e_success;  
}


Status read_tag(FILE *fptr_audio, char *tag)
{
    if (fread(tag, 4, 1, fptr_audio) != 1)
    {
        return e_failure;
    }
    tag[4] = '\0';
    if (tag[0] == 0)
    {
        return e_failure;
    }
    return e_success;
}


Status compare_tags(char *tag)
{
   char *tag_names[6] = {"TIT2", "TALB", "TYER", "TCON", "COMM", "TPE1"};
  
   for(int i=0; i<6; i++)
  {
     if(strcmp(tag, tag_names[i]) == 0)
     {
        return e_success;
     }
  }
   return e_failure;
}



Status read_tag_size(FILE *fptr_audio, unsigned int *value)
{
    unsigned char size[4];

    if (fread(size, 4, 1, fptr_audio) != 1)
    {
        printf("ERROR : Unable to read size\n");
        return e_failure;
    }

    *value = (size[0] << 24) |
             (size[1] << 16) |
             (size[2] << 8)  |
             (size[3]);

    return e_success;
}


Status skip_flag(FILE *fptr_audio)
{
    fseek(fptr_audio, 2, SEEK_CUR);
    return e_success;
}


Status read_tag_data(FILE *fptr_audio, unsigned int size)
{
    char *data = malloc(size + 1);
    if (data == NULL)
    {
        printf("Memory allocation failed\n");
        return e_failure;
    }

    if (fread(data, size, 1, fptr_audio) != 1)
    {
        printf("ERROR: Unable to read tag data\n");
        free(data);
        return e_failure;
    }

    data[size] = '\0'; //This makes the buffer a proper C string
   
    if (data[0] == 0 || data[0] == 3)
    {
        printf("Tag Data: %s\n", data + 1);
    }
   
    else if (data[0] == 1)
    {
        printf("Tag Data: ");

        for (int i=3; i<size; i=i+2)
        {
            printf("%c", data[i]);
        }
        printf("\n");
    }

    printf("Offset after reading data: %ld\n\n", ftell(fptr_audio));

    free(data);
    return e_success;
}





