#ifndef READING_H
#define READING_H

#include "type.h" 

typedef struct _Read_MP3
{
    char *audio_fname; 
    FILE *fptr_audio;

}Read_MP3;

/* Reading function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);    //check read or edit

Status open_files(Read_MP3 *readmp3);

Status read_and_validate_read_args(char *argv[], Read_MP3 *readmp3);

Status do_reading(Read_MP3 *readmp3);

Status skip_header(FILE *fptr_audio);

Status read_tag(FILE *fptr_audio, char *tag);

Status compare_tags(char *tag);

Status read_tag_size(FILE *fptr_audio, unsigned int *value);

Status convert_little_endianess(unsigned int *value);

Status skip_flag(FILE *fptr_audio);

Status read_tag_data(FILE *fptr_audio, unsigned int size);


#endif


