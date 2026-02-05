#ifndef __SQL_MEXPR_INTF__
#define __SQL_MEXPR_INTF__
#include <stdbool.h>
#include <string>
#include "../SqlParser/SqlEnums.h"
#include "../../Mexpr/headers/Enums/MexprcppEnums.h"



class MexprTree;

typedef struct sql_exptree_ {

    MexprTree *tree;

} sql_exptree_t;

sql_exptree_t *
sql_create_exp_tree_compute ();

void 
sql_destroy_exp_tree (sql_exptree_t *tree);

#endif