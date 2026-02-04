#include<bits/stdc++.h>
#include<stdio.h>

using namespace std;

#include "MexprTree/MexprTree.h"
#include "Dtypes/Dtype.h"
#include "Operators/Operators.h"
#include "Enums/MexprcppEnums.h"
#include "lex_parser/ParserExport.h"

extern parse_rc_t E();
extern parse_rc_t Q();
extern parse_rc_t S();

extern lex_data_t **mexpr_convert_infix_to_postfix (
                                 lex_data_t *infix, 
                                 int sizein, 
                                 int *size_out);

static void
postfix_array_free(lex_data_t **lex_data_array, int size) {
   
    for (int i = 0; i < size; i++) {

        if (lex_data_array[i])
        {
            if (lex_data_array[i]->token_val) free(lex_data_array[i]->token_val);
            free(lex_data_array[i]);
        }
    }

    free(lex_data_array);
}

static map<string, string> mp= {

    {"a", "2"},
    {"b", "3"},
    {"c", "4.5"},
    {"d", "Hello"}
};

static Dtype*
compute_fn (char *opnd_name, void *data_src) {

    map<string, string> *mp = reinterpret_cast <map<string, string> *> (data_src);
    string opnd_name_str (opnd_name);
    string value = (*mp)[opnd_name_str];

    if (opnd_name_str == "a" || opnd_name_str == "b") {

        Dtype *dtype = Dtype::factory(MATH_CPP_INT);
        dtype->SetValue ((void *)value.c_str());
        return dtype;
    }

    else if  (opnd_name_str == "c") {

        Dtype *dtype = Dtype::factory(MATH_CPP_DOUBLE);
        dtype->SetValue ((void *)value.c_str());
        return dtype;
    }


    else if  (opnd_name_str == "d") {

        Dtype *dtype = Dtype::factory(MATH_CPP_STRING);
        dtype->SetValue ((void *)value.c_str());
        return dtype;
    }

    return NULL;
}


int main() {

    parse_rc_t err;
    int token_code;

    while (1) {
        printf("Input:- ");

        fgets(lex_buffer, sizeof(lex_buffer), stdin);

        if (lex_buffer[0] == '\n') {
            lex_buffer[0] = 0;
            continue;
        }

        lex_set_scan_buffer(lex_buffer);

        parser_stack_reset();

        err = S();

        if (err == PARSE_ERR) {

            err = Q();

            if (err == PARSE_ERR) {

                err = E();

                if (err == PARSE_ERR) {

                    printf ("Error : Parsing Error, Invalid Expression\n");
                    continue;
                }
            }
        }

        token_code = cyylex();

        if (token_code != PARSER_EOL) {
            printf ("Error : Parsing Error, Entire Input String is not a Valid Expression.\n");
            continue;
        }

        int size_out = 0;

        lex_data_t **postfix = mexpr_convert_infix_to_postfix(undo_stack.data, undo_stack.top + 1, &size_out);

        MexprTree *tree = new MexprTree(postfix, size_out);

        if (!tree) {
            printf ("Error : Failed to build Expression Tree\n");
            continue;
        }

        if (!tree->validate (tree->root)) {
            printf ("Error : Expression Tree failed Loose-Validation Test\n");
            tree->destroy();
            continue;
        }

         /* Resolve the Expression Tree now*/
        MexprNode *opnd_node;
        MexprTree_Iterator_Operands_Begin(tree, opnd_node) {

            Dtype_VARIABLE *dvar = dynamic_cast < Dtype_VARIABLE *> (opnd_node);

            if (dvar->dtype.variable_name == "a" || dvar->dtype.variable_name == "b") {
                dvar->ResolveOperand (MATH_CPP_INT, (void *)&mp, compute_fn);
            }
            else if (dvar->dtype.variable_name == "c") {
                dvar->ResolveOperand (MATH_CPP_DOUBLE, (void *)&mp, compute_fn);
            }
            else  if (dvar->dtype.variable_name == "d") {
                dvar->ResolveOperand (MATH_CPP_STRING, (void *)&mp, compute_fn);
            }

        } MexprTree_Iterator_Operands_End;

        if (!tree->validate (tree->root)) {
            printf ("Error : Expression Tree failed Strict-Validation Test\n");
            tree->destroy();
            continue;
        }

        Dtype *res = tree->evaluate (tree->root);

        tree->destroy();

        if (!res ) {
            printf ("Error : Expression Tree Could not be evaluated\n");
            continue;
        }

        printf ("Result : ");

        switch (res->did) {

            case MATH_CPP_INT:
                printf ("%d\n",  reinterpret_cast <Dtype_INT *> (res)->dtype.int_val);
                break;
            case MATH_CPP_DOUBLE:
                printf ("%lf\n",  reinterpret_cast <Dtype_DOUBLE *> (res)->dtype.d_val);
                break;
            case MATH_CPP_STRING:
                printf ("%s\n",  reinterpret_cast <Dtype_STRING *> (res)->dtype.str_val.c_str());
                break;
            case MATH_CPP_BOOL:
            {
                Dtype_BOOL *dtype_b = reinterpret_cast <Dtype_BOOL *>(res);
                dtype_b->dtype.b_val == true ? printf ("True") : printf("False");
                printf("\n");
            }
            break;
            default:
                assert(0);
        }

        delete res;
        
    }

    return 0;
}

