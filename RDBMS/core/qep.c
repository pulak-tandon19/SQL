#include "../BPlusTreeLib/BPlusTree.h"
#include "stdio.h"
#include "stdbool.h"
#include <stdlib.h>
#include "qep.h"
#include "sql_const.h"
#include "rdbms_struct.h"

extern BPlusTree_t TableCatalogDef;

static bool

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

bool 
sql_query_initialize_select_column_list  (qep_struct_t *qep, BPlusTree_t *tcatalog) {   
    return true;
}


static bool 
sql_query_init_execution_plan (qep_struct_t *qep, BPlusTree_t *tcatalog) {

    int i;
    bool rc;
   
     /* Initialize Joined Row*/


    rc = sql_query_initialize_join_clause (qep, tcatalog) ;
    if (!rc) return rc;

    rc = sql_query_initialize_select_column_list (qep, tcatalog) ;
    if (!rc) return rc;

    qep->joined_row_tmplate = (joined_row_t *)calloc (1, sizeof (joined_row_t));
    joined_row_t *joined_row_tmplate = qep->joined_row_tmplate;
    joined_row_tmplate->size = qep->join.table_cnt;
    joined_row_tmplate->key_array = (BPluskey_t **) calloc (qep->join.table_cnt, sizeof (BPluskey_t *));
    joined_row_tmplate->rec_array = (void **) calloc (qep->join.table_cnt, sizeof (void *));
    joined_row_tmplate->table_id_array = (int *) calloc (qep->join.table_cnt, sizeof (int));
    for (int i = 0; i < qep->join.table_cnt; i++) {
        joined_row_tmplate->table_id_array[i] = i;
    }

    return true;
}


void
sql_execute_qep (qep_struct_t *qep) {

    bool rc;
    if (!sql_query_init_execution_plan (qep, &TableCatalogDef)) {

        printf ("Error : Failed to initialize Query Execution Plan\n");
        return;
    }
}

void 
qep_deinit (qep_struct_t *qep) {

    int i;
    qp_col_t *qp_col;

    if (qep->select.n) {

        for (i = 0; i < qep->select.n; i++) {

            qp_col = qep->select.sel_colmns[i];
            if (qp_col->sql_tree) {
                 sql_destroy_exp_tree (qp_col->sql_tree);
                qp_col->sql_tree = NULL;
            }

            free(qp_col);            
        }
    }


    if (qep->joined_row_tmplate) {
        free (qep->joined_row_tmplate->key_array);
        free (qep->joined_row_tmplate->rec_array);
        free(qep->joined_row_tmplate);
    }



}
