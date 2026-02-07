#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>
#include "rdbms_struct.h"
#include "qep.h"
#include "../BPlusTreeLib/BPlusTree.h"
#include "SqlMexprIntf.h"
#include "sql_join.h"

bool 
sql_query_initialize_select_column_list  (qep_struct_t *qep, BPlusTree_t *tcatalog) {   
    return true;
}