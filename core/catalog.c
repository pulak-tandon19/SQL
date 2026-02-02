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
Catalog_create_schema_table_records (BPlusTree_t *schema_table, sql_create_data_t *cdata) {}

bool 
Catalog_insert_new_table (BPlusTree_t *catalog, sql_create_data_t *cdata) {

    int i;
    BPluskey_t bpkey;

    // Implementation : Creation and initialization of Catalog table
    static key_mdata_t key_mdata1[] = {  {SQL_STRING, SQL_TABLE_NAME_MAX_SIZE}};
    BPlusTree_t *schema_table = (BPlusTree_t *)calloc (1, sizeof (BPlusTree_t));


    BPlusTree_init (catalog, 
                    rdbms_key_comp_fn,
                    NULL, 
                    NULL,
                    SQL_BTREE_MAX_CHILDREN_CATALOG_TABLE, 
                    catalog_table_free_fn,
                    key_mdata1, 
                    sizeof(key_mdata1) / sizeof (key_mdata1[0]));

    /* Schema table has been created, now insert records in it. Each record is of the type : 
       key::  <column name>   value :: <catalog_rec_t >  */
    Catalog_create_schema_table_records (schema_table, cdata);
}


