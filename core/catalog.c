#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "Catalog.h"
#include "../BPlusTreeLib/BPlusTree.h"
#include "sql_create.h"

/* Global Default Catalog Table, one per Database */
BPlusTree_t TableCatalogDef;

extern int 
rdbms_key_comp_fn ( BPluskey_t *key1, BPluskey_t *key2, key_mdata_t *key_mdata, int key_mdata_size) ;

static void
Catalog_create_schema_table_records (BPlusTree_t *schema_table, sql_create_data_t *cdata) {}

/* A fn used to free the 'value' of catalog table*/
static void 
schema_table_record_free ( void *ptr) {
    free(ptr);
}

static void 
catalog_table_free_fn (void *ptr) {}

static void
Catalog_create_schema_table_records (BPlusTree_t *schema_table, sql_create_data_t *cdata) {

    int i;
    int offset = 0;
    BPluskey_t bpkey_tmplate;
    schema_rec_t *schema_rec;

    for (i = 0; i < cdata->n_cols; i++) {

        /* Setup the key*/
        bpkey_tmplate.key = (void *)calloc (1, SQL_COLUMN_NAME_MAX_SIZE);
        strncpy ((char *) bpkey_tmplate.key, 
                        cdata->column_data[i].col_name,
                        SQL_COLUMN_NAME_MAX_SIZE);
        bpkey_tmplate.key_size = SQL_COLUMN_NAME_MAX_SIZE;

        /* Set up the value (a.k.a record)*/
        schema_rec = (schema_rec_t *)calloc (1, sizeof (schema_rec_t));
        strncpy (schema_rec->column_name,
                        cdata->column_data[i].col_name, 
                        SQL_COLUMN_NAME_MAX_SIZE);
        schema_rec->dtype = cdata->column_data[i].dtype;
        schema_rec->dtype_size = cdata->column_data[i].dtype_len;
        schema_rec->offset = offset;
        offset += cdata->column_data[i].dtype_len;
        schema_rec->is_primary_key = cdata->column_data[i].is_primary_key;

        /* Insert into Schema Table*/
        assert (BPlusTree_Insert (schema_table, &bpkey_tmplate, (void *)schema_rec));
    }
}


bool 
Catalog_insert_new_table (BPlusTree_t *catalog, sql_create_data_t *cdata) {

    int i;
    BPluskey_t bpkey;

    // Implementation : Creation and initialization of Catalog table
    static key_mdata_t key_mdata2[] = {   {SQL_STRING, SQL_COLUMN_NAME_MAX_SIZE}  }  ;

    BPlusTree_t *schema_table = (BPlusTree_t *)calloc (1, sizeof (BPlusTree_t));

    BPlusTree_init (schema_table, 
                            rdbms_key_comp_fn, 
                            NULL, 
                            NULL,
                            SQL_BTREE_MAX_CHILDREN_SCHEMA_TABLE,
                            schema_table_record_free, 
                            key_mdata2, sizeof(key_mdata2) / sizeof (key_mdata2[0]));


    /* Schema table has been created, now insert records in it. Each record is of the type : 
       key::  <column name>   value :: <catalog_rec_t >  */
    Catalog_create_schema_table_records (schema_table, cdata);

    BPlusTree_t *record_table = (BPlusTree_t *)calloc (1, sizeof (BPlusTree_t));

    /* Construct key meta data for this Table Schema*/
    int key_mdata_size3;
    key_mdata_t *key_mdata3 = sql_construct_table_key_mdata (cdata, &key_mdata_size3);

    if (!key_mdata3) {
        BPlusTree_Destroy (record_table);
        BPlusTree_Destroy (schema_table);
        printf ("Error : Table Must have atleast one primary key\n");
        free(record_table);
        free(schema_table);
        free(bpkey.key);
        return false;
    }

    BPlusTree_init (record_table, 
                               rdbms_key_comp_fn,
                               NULL, NULL, 
                               SQL_BTREE_MAX_CHILDREN_RDBMS_TABLE, free,
                               key_mdata3, key_mdata_size3);
}


