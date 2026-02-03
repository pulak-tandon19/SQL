#ifndef __SQL_CREATE__
#define __SQL_CREATE__

#include <stdlib.h>
#include <stdbool.h>
#include "sql_const.h"
#include "../SqlParser/SqlEnums.h"
#include "../BPlusTreeLib/BPlusTree.h"


typedef struct sql_create_data_ {

    char table_name [SQL_TABLE_NAME_MAX_SIZE];

    int n_cols;

    struct {

        char col_name [SQL_COLUMN_NAME_MAX_SIZE];
        sql_dtype_t dtype;
        int dtype_len;
        bool is_primary_key;

    }  column_data [SQL_MAX_COLUMNS_SUPPORTED_PER_TABLE];

}  sql_create_data_t; 

void 
sql_process_create_query (sql_create_data_t *cdata) ;

 void 
sql_create_data_destroy (sql_create_data_t *cdata) ;

key_mdata_t *
sql_construct_table_key_mdata (sql_create_data_t *cdata, int *key_mdata_size) {

    int i, j;
    int primary_key_count = 0;

   for (i = 0; i < cdata->n_cols; i++) {

      if (cdata->column_data[i].is_primary_key) primary_key_count++;
   }

   if (primary_key_count == 0) {
      *key_mdata_size = 0;
      return NULL;
   }

    key_mdata_t *key_mdata = (key_mdata_t *)calloc (primary_key_count, sizeof (key_mdata_t));

    for (i = 0, j = 0; i < cdata->n_cols; i++) {

        if (cdata->column_data[i].is_primary_key) {
            key_mdata[j].dtype = cdata->column_data[i].dtype;
            key_mdata[j].size = cdata->column_data[i].dtype_len;
            j++;
        }
    }

    *key_mdata_size = j;
    return key_mdata;
}

#endif 