#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "edit.h"
#include "type.h"

Status open_edit_files(Edit_MP3 *editmp3)
{
    editmp3->fptr_audio = fopen(editmp3->audio_fname, "rb");
    if (editmp3->fptr_audio == NULL)
    {
    	perror("fopen"); 
    	fprintf(stderr, "ERROR: Unable to open file %s\n", editmp3->audio_fname);
    	return e_failure;
    }

    editmp3->fptr_new = fopen("temp.mp3", "wb");
    if(editmp3->fptr_new == NULL)
    {
        printf("ERROR: Unable to create temp file\n");
        return e_failure;
    }

    printf("File opened successfully\n");
    return e_success;
}

Status read_and_validate_edit_args(char *argv[], Edit_MP3 *editmp3)
{
  
         if(argv[2] == NULL)
         {
         printf("Error : Tag option not found\n");
         return e_failure;
         }
         
        if(strcmp(argv[2], "-t") == 0)
        {
          editmp3->tag_to_edit = "TIT2";  
        }
        else if(strcmp(argv[2], "-a") == 0)
        {
           editmp3->tag_to_edit = "TPE1"; 
        }
        else if(strcmp(argv[2], "-A") == 0)
        {
          editmp3->tag_to_edit = "TALB"; 
        }
        else if(strcmp(argv[2], "-y") == 0)
        {
           editmp3->tag_to_edit = "TYER"; 
        }
        else if(strcmp(argv[2], "-g") == 0)
        {
           editmp3->tag_to_edit = "TCON"; 
        }
        else if(strcmp(argv[2], "-c") == 0)
        {
           editmp3->tag_to_edit = "COMM"; 
        }
        else
        {
        printf("Error : Invalid tag option\n");
        return e_failure;
        }


     if(argv[3] == NULL)
     {
        printf("Error : New tag data not provided\n");
        return e_failure;
     }
      editmp3->new_data = argv[3];


      if(argv[4] == NULL)
      {
        printf("Error : mp3 file not provided\n");
        return e_failure;
      }

      if(strstr(argv[4], ".mp3") == NULL)
      {
        printf("Error : Invalid audio file\n");
        return e_failure;
      }
     editmp3->audio_fname = argv[4];

     return e_success;
}


Status do_editing(Edit_MP3 *editmp3)
{
   Status ret = open_edit_files(editmp3);
     if (ret == e_failure)
     {
        return e_failure;
     }

    ret = copy_header(editmp3);
    if (ret == e_failure)
    {
        return e_failure;
    }

    ret = process_tags(editmp3);
     if (ret == e_failure)
    {
        return e_failure;
    }

    ret = copy_remaining_data(editmp3);
     if (ret == e_failure)
    {
        return e_failure;
    }

    fclose(editmp3->fptr_audio);
    fclose(editmp3->fptr_new);

    remove(editmp3->audio_fname);
    rename("temp.mp3", editmp3->audio_fname);

    printf(".........Tag edited successfully.........\n");
     return e_success;
}

Status copy_header(Edit_MP3 *editmp3)
{
    char temp[10];
    fseek(editmp3->fptr_audio, 0, SEEK_SET);
    if(fread(temp, 10, 1, editmp3->fptr_audio) != 1)
    {
        return e_failure;
    }

    if(fwrite(temp, 10, 1, editmp3->fptr_new) != 1)
    {
        return e_failure;
    }
    printf("Successfulyy 10 bytes of header copied!...\n");
    printf("After copying 10 byte header then offset at %ld\n", ftell(editmp3->fptr_new));
    printf("\n");
    return e_success;  
}

Status convert_big_endianess(unsigned int *value)
{
    *value = ((*value >> 24) & 0x000000FF) |
             ((*value >> 8)  & 0x0000FF00) |
             ((*value << 8)  & 0x00FF0000) |
             ((*value << 24) & 0xFF000000);

             return e_success;
}



Status process_tags(Edit_MP3 *editmp3)
{
    char tag[5];
    tag[4] = '\0';

    unsigned int tag_size;
    char flag[2];

    while(fread(tag, 4, 1, editmp3->fptr_audio) == 1)
    {
       if(tag[0] == 0)
       {
        break;
       }

        if(fread(&tag_size, 4, 1, editmp3->fptr_audio) != 1)
        {
            return e_failure;
        }
        convert_big_endianess(&tag_size);

       if(fread(flag, 2, 1, editmp3->fptr_audio) != 1)
       {
        return e_failure;
       }

        if(strcmp(tag, editmp3->tag_to_edit) == 0)
        {
            fwrite(tag, 4, 1, editmp3->fptr_new);
            unsigned int temp = tag_size;

            convert_big_endianess(&temp);

            fwrite(&temp, 4, 1, editmp3->fptr_new);
            fwrite(flag, 2, 1, editmp3->fptr_new);

           char *buffer = calloc(tag_size, 1);
           if(buffer == NULL)
           {
            return e_failure;
           }

           buffer[0] = 0x03;
           strncpy(buffer+1, editmp3->new_data, tag_size-1);
           fwrite(buffer, tag_size, 1, editmp3->fptr_new);

           free(buffer);

            fseek(editmp3->fptr_audio, tag_size, SEEK_CUR);
        }
        else
        {
            fwrite(tag, 4, 1, editmp3->fptr_new);

            unsigned int temp = tag_size;
            convert_big_endianess(&temp);

            fwrite(&temp, 4, 1, editmp3->fptr_new);
            fwrite(flag, 2, 1, editmp3->fptr_new);

            char *buffer = malloc(tag_size);
            if(buffer == NULL)
            {
                return e_failure;
            }
            fread(buffer, tag_size, 1, editmp3->fptr_audio);
            fwrite(buffer, tag_size, 1, editmp3->fptr_new);

            free(buffer);

        }
    }
     return e_success;
}



Status copy_remaining_data(Edit_MP3 *editmp3)
{
    int ch;

    while((ch= fgetc(editmp3->fptr_audio)) != EOF)
      {
         fputc(ch, editmp3->fptr_new);
         // fwrite(&ch, 1, 1, fptr_new);
      }
       printf("Remaining data copied successfully!.....\n");
       printf("\n");
       return e_success;   
}



