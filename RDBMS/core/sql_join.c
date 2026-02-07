#include "../BPlusTreeLib/BPlusTree.h"
#include "stdio.h"
#include "stdbool.h"
#include <stdlib.h>
#include "sql_const.h"
#include "rdbms_struct.h"
#include "SqlMexprIntf.h"
#include "sql_join.h"
#include "qep.h"

extern BPlusTree_t TableCatalogDef;

bool
sql_query_initialize_join_clause  (qep_struct_t *qep, BPlusTree_t *tcatalog) {

    int i;
    BPluskey_t bpkey;

    bpkey.key_size = SQL_TABLE_NAME_MAX_SIZE;

    for (i = 0; i < qep->join.table_cnt; i++) {
        bpkey.key = (void *)qep->join.tables[i].table_name;
        qep->join.tables[i].ctable_val = (ctable_val_t *)BPlusTree_Query_Key

                                                            (tcatalog, &bpkey);

       if (!qep->join.tables[i].ctable_val) {
            printf ("Error : Could not find table %s\n", qep->join.tables[i].table_name);
            return false;
       }
    }
    return true;
}


void 
table_iterators_init (qep_struct_t *qep,
                                table_iterators_t **_titer) {

    int i;

    (*_titer) = (table_iterators_t *)calloc (1, 
                            sizeof(table_iterators_t) +  
                            sizeof(table_iter_data_t) * qep->join.table_cnt);

    table_iterators_t *titer = *_titer;

    titer->table_cnt = qep->join.table_cnt;

    for (i = 0; i < titer->table_cnt; i++) {
        titer->table_iter_data[i].bpnode = NULL;
        titer->table_iter_data[i].index = 0;
        titer->table_iter_data[i].ctable_val = qep->join.tables[i].ctable_val;
    }

}
