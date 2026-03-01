#ifndef EDIT_H
#define EDIT_H

#include "type.h"

typedef struct _Edit_MP3
{
    char *audio_fname; 
    FILE *fptr_audio;
    FILE *fptr_new; 

    char *tag_to_edit;
    char *new_data;

}Edit_MP3;

/* Editing function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);    //check read or edit

Status open_edit_files(Edit_MP3 *editmp3);

Status read_and_validate_edit_args(char *argv[], Edit_MP3 *editmp3);

Status do_editing(Edit_MP3 *editmp3);

Status convert_big_endianess(unsigned int *value);

Status copy_header(Edit_MP3 *editmp3);

Status process_tags(Edit_MP3 *editmp3);

Status copy_remaining_data(Edit_MP3 *editmp3);

#endif
