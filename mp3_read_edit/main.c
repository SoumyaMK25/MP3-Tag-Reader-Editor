#include <stdio.h>
#include <string.h>
#include "type.h"
#include "reading.h"
#include "edit.h"


int main(int argc, char *argv[])   
{
   Read_MP3 readmp3;
   Edit_MP3 editmp3;
   Status ret =  check_operation_type(argv); 

   if(ret == e_unsupported)
  {
    printf("Invalid arguments\n");
    return 0;
  }

  if(ret == e_read)
  {
   // reading
      Status ret =  read_and_validate_read_args(argv, &readmp3);
      if(ret == e_failure)
      {
        printf("Invalid arguments\n");
        return e_failure;
      }
   
 
      ret = do_reading(&readmp3);
      if(ret == e_failure)
      {
        printf("Reading failed\n");
        return e_failure;
      }
      else
      {
        printf("........Reading is successfull.......\n");
        printf("\n");
        return e_success;
      }
  

    }
  if(ret == e_edit)
   {
      //editing
      Status ret =  read_and_validate_edit_args(argv, &editmp3);
      if(ret == e_failure)
      {
        printf("Invalid arguments\n");
        return 0;
      }
   
 
      ret = do_editing(&editmp3);
      if(ret == e_failure)
      {
        printf("Editing failed\n");
        return e_failure;
      }
      else
      {
        printf("........Editing is successfull.......\n");
        printf("\n");
        return e_success;
      }
   }
   return 0;
  }

  

OperationType check_operation_type(char *argv[]) 
{
    
    if(argv[1] == NULL)
    {
      return e_unsupported;
    }

    if(strcmp(argv[1], "-r") == 0)
    {
      return e_read;
    }

    if(strcmp(argv[1], "-e") == 0)
    {
      return e_edit;
    }
    return e_unsupported;
}  


